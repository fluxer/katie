/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdebug.h"
#include "qplatformdefs.h"
#include "qsettings.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qstandardpaths.h"
#include "qmutex.h"
#include "qcoreapplication.h"
#include "qlibraryinfo.h"

#ifndef QT_NO_SETTINGS

#include "qsettings_p.h"
#include "qfile.h"
#include "qdir.h"

#ifndef QT_NO_GEOM_VARIANT
#include "qsize.h"
#include "qpoint.h"
#include "qrect.h"
#endif // !QT_NO_GEOM_VARIANT

#include <stdlib.h>

QT_BEGIN_NAMESPACE

struct QSettingsCustomFormat
{
    QSettings::Format format;
    QString extension;
    QSettings::ReadFunc readFunc;
    QSettings::WriteFunc writeFunc;
};

typedef QVector<QSettingsCustomFormat> CustomFormatVector;
typedef QVector<QSettings*> QSettingsVector;
Q_GLOBAL_STATIC(QSettingsVector, qGlobalSettings)
Q_GLOBAL_STATIC(CustomFormatVector, customFormatVectorFunc)
Q_GLOBAL_STATIC(QMutex, qSettingsMutex)

static inline bool qt_isEvilFsTypeName(const char *name)
{
    return (qstrncmp(name, "nfs", 3) == 0
            || qstrncmp(name, "autofs", 6) == 0
            || qstrncmp(name, "cachefs", 7) == 0);
}

#if defined(Q_OS_BSD4) && !defined(Q_OS_NETBSD)
QT_BEGIN_INCLUDE_NAMESPACE
# include <sys/param.h>
# include <sys/mount.h>
QT_END_INCLUDE_NAMESPACE

static inline bool qIsLikelyToBeNfs(int handle)
{
    struct statfs buf;
    if (fstatfs(handle, &buf) != 0)
        return false;
    return qt_isEvilFsTypeName(buf.f_fstypename);
}

#elif defined(Q_OS_LINUX) || defined(Q_OS_HURD)
QT_BEGIN_INCLUDE_NAMESPACE
# include <sys/vfs.h>
# ifdef QT_LINUXBASE
   // LSB 3.2 has fstatfs in sys/statfs.h, sys/vfs.h is just an empty dummy header
#  include <sys/statfs.h>
# endif
QT_END_INCLUDE_NAMESPACE
# ifndef NFS_SUPER_MAGIC
#  define NFS_SUPER_MAGIC       0x00006969
# endif
# ifndef AUTOFS_SUPER_MAGIC
#  define AUTOFS_SUPER_MAGIC    0x00000187
# endif
# ifndef AUTOFSNG_SUPER_MAGIC
#  define AUTOFSNG_SUPER_MAGIC  0x7d92b1a0
# endif

static inline bool qIsLikelyToBeNfs(int handle)
{
    struct statfs buf;
    if (fstatfs(handle, &buf) != 0)
        return false;
    return buf.f_type == NFS_SUPER_MAGIC
           || buf.f_type == AUTOFS_SUPER_MAGIC
           || buf.f_type == AUTOFSNG_SUPER_MAGIC;
}

#elif defined(Q_OS_SOLARIS) || defined(Q_OS_AIX) || defined(Q_OS_HPUX) \
      || defined(Q_OS_OSF) || defined(Q_OS_SCO) \
      || defined(Q_OS_UNIXWARE) || defined(Q_OS_RELIANT) || defined(Q_OS_NETBSD)
QT_BEGIN_INCLUDE_NAMESPACE
# include <sys/statvfs.h>
QT_END_INCLUDE_NAMESPACE

static inline bool qIsLikelyToBeNfs(int handle)
{
    struct statvfs buf;
    if (fstatvfs(handle, &buf) != 0)
        return false;
#if defined(Q_OS_NETBSD)
    return qt_isEvilFsTypeName(buf.f_fstypename);
#else
    return qt_isEvilFsTypeName(buf.f_basetype);
#endif
}
#else
static inline inline bool qIsLikelyToBeNfs(int /* handle */)
{
    return true;
}
#endif

static bool unixLock(int handle, int lockType)
{
    /*
        NFS hangs on the fcntl() call below when statd or lockd isn't
        running. There's no way to detect this. Our work-around for
        now is to disable locking when we detect NFS (or AutoFS or
        CacheFS, which are probably wrapping NFS).
    */
    if (qIsLikelyToBeNfs(handle))
        return false;

    struct flock fl;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_type = lockType;
    return fcntl(handle, F_SETLKW, &fl) == 0;
}

// ************************************************************************
// Native format
static bool json_settings_read(QIODevice &device, QSettings::SettingsMap &map)
{
    QByteArray data = device.readAll();
    if (Q_UNLIKELY(data.isEmpty())) {
        return false;
    }

    QJsonDocument jsondoc = QJsonDocument::fromJson(data);
    if (Q_UNLIKELY(jsondoc.isNull())) {
        return false;
    }

    map = jsondoc.object().toVariantMap();

    // qDebug() << "json_settings_read" << jsondoc.toJson();
    return true;
}

static bool json_settings_write(QIODevice &device, const QSettings::SettingsMap &map)
{
    QJsonDocument jsondoc = QJsonDocument(QJsonObject::fromVariantMap(map));
    QByteArray jsondata = jsondoc.toJson();
    if (Q_UNLIKELY(jsondoc.isNull() || jsondata.isNull())) {
        return false;
    }

    // qDebug() << "json_settings_write" << jsondata;
    return device.write(jsondata);
}

// ************************************************************************
// INI format
static bool ini_settings_read(QIODevice &device, QSettings::SettingsMap &map)
{
    QByteArray section; // keys without section are allowed

    bool parsedsomething = false;
    while (!device.atEnd()) {
        const QByteArray line = device.readLine().trimmed();
        if (line.isEmpty() || line.startsWith(';')) {
            continue;
        } else if (line.startsWith('[') && line.endsWith(']')) {
            section = line.mid(1, line.size() - 2);
            continue;
        }

        const QList<QByteArray> splitline = line.split('=');
        if (splitline.size() < 2) {
            continue;
        }

        const QByteArray key = splitline.at(0).trimmed();
        const QByteArray value = splitline.at(1).trimmed();
        const QVariant variantvalue = QSettingsPrivate::stringToVariant(value);
        if (section.isEmpty()) {
            map.insert(key, variantvalue);
        } else {
            const QString actualkey = section + QLatin1Char('/') + key;
            map.insert(actualkey, variantvalue);
        }

        parsedsomething = true;
    }

    // qDebug() << "ini_settings_read" << map;
    return parsedsomething;
}

static bool ini_settings_write(QIODevice &device, const QSettings::SettingsMap &map)
{
    QString lastsection;
    foreach (const QString &key, map.keys()) {
        QString section;
        int keypos = 0;
        const QList<QString> splitkey = key.split(QLatin1Char('/'));
        if (splitkey.size() > 1) {
            section = QLatin1Char('[') + splitkey.at(0) + QLatin1String("]\n");
            keypos = 1;
        }

        QString keyname;
        for (int i = keypos; i < splitkey.size(); i++) {
            if (i == keypos) {
                keyname += splitkey.at(i);
            } else {
                keyname += QLatin1Char('/') + splitkey.at(i);
            }
        }

        if (section != lastsection) {
            // separate each section with two line separators, do only for
            // sections after the first one
            if (!lastsection.isEmpty() && !device.write("\n")) {
                return false;
            }
            if (!section.isEmpty() && !device.write(section.toAscii())) {
                return false;
            }
        }
        lastsection = section;

        if (!device.write(keyname.toAscii())) {
            return false;
        }

        const QString stringvalue = QSettingsPrivate::variantToString(map.value(key));
        const QString datavalue = QLatin1Char('=') + stringvalue + QLatin1Char('\n');
        if (!device.write(datavalue.toAscii())) {
            return false;
        }

        // qDebug() << "ini_settings_write" << section << keyname << variantvalue;
    }

    return true;
}


// ************************************************************************
// QSettingsPrivate
static QSettingsCustomFormat getSettingsFormat(QSettings::Format format)
{
    QSettingsCustomFormat result;

    if (format == QSettings::NativeFormat) {
        result.extension = QLatin1String(".json");
        result.readFunc = json_settings_read;
        result.writeFunc = json_settings_write;
        return result;
    } else if (format == QSettings::IniFormat) {
        result.extension = QLatin1String(".ini");
        result.readFunc = ini_settings_read;
        result.writeFunc = ini_settings_write;
        return result;
    }

    QMutexLocker locker(qSettingsMutex());
    const CustomFormatVector *customFormatVector = customFormatVectorFunc();
    for (int i = 0; i < customFormatVector->size(); i++) {
        const QSettingsCustomFormat &custom = customFormatVector->at(i);
        if (custom.format == format) {
            result.extension = custom.extension;
            result.readFunc = custom.readFunc;
            result.writeFunc = custom.writeFunc;
            return result;
        }
    }

    qWarning("QSettingsPrivate::getSettingsFormat: format not found %d", format);
    result.extension = QLatin1String(".json");
    result.readFunc = json_settings_read;
    result.writeFunc = json_settings_write;
    return result;
}

QString getSettingsPath(QSettings::Scope scope, const QString &filename, const QString &extension)
{
    QFileInfo info(filename);
    if (info.isAbsolute()) {
        return filename;
    }

    QString nameandext = filename;
    if (!filename.endsWith(extension)) {
        nameandext += extension;
    }

    QStringList locations = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    if (scope == QSettings::UserScope) {
        locations = QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation);
    }

    foreach (const QString &location, locations) {
        QDir dir(location);
        if (dir.exists(location)) {
            return location + QDir::separator() + nameandext;
        }
    }

    const QString fallback = QLibraryInfo::location(QLibraryInfo::SettingsPath);
    QDir fallbackdir(fallback);
    if (!fallbackdir.mkpath(fallback)) {
        qWarning("QSettingsPrivate::getSettingsPath: no settings location");
        fallbackdir.currentPath() + QDir::separator() + nameandext;
    }
    return fallback + QDir::separator() + nameandext;
}

QSettingsPrivate::QSettingsPrivate(QSettings::Format format, QSettings::Scope scope)
    : format(format), scope(scope), status(QSettings::NoError)
{
    QSettingsCustomFormat handler = getSettingsFormat(format);
    filename = getSettingsPath(scope, QCoreApplication::applicationName(), handler.extension);
    readFunc = handler.readFunc;
    writeFunc = handler.writeFunc;
}

QSettingsPrivate::QSettingsPrivate(const QString &fileName, QSettings::Format format)
    : format(format), scope(QSettings::UserScope), status(QSettings::NoError)
{
    QSettingsCustomFormat handler = getSettingsFormat(format);
    filename = getSettingsPath(scope, fileName, handler.extension);
    readFunc = handler.readFunc;
    writeFunc = handler.writeFunc;
}

QSettingsPrivate::~QSettingsPrivate()
{
}

void QSettingsPrivate::read()
{
    QFileInfo info(filename);
    if (!info.isReadable() || info.size() == 0) {
        status = QSettings::AccessError;
        // no warning, info.exists() may return false if not readable
        return;
    }

    QFile file(filename);
    if (Q_UNLIKELY(!file.open(QFile::ReadOnly))) {
        status = QSettings::AccessError;
        qWarning("QSettingsPrivate::read: failed to open %s", filename.toLocal8Bit().constData());
        return;
    }

    if (Q_UNLIKELY(!unixLock(file.handle(), F_RDLCK))) {
        status = QSettings::AccessError;
        qWarning("QSettingsPrivate::read: failed to lock %s", filename.toLocal8Bit().constData());
        return;
    }

    QSettings::SettingsMap readmap;
    if (Q_UNLIKELY(!readFunc(file, readmap))) {
        status = QSettings::FormatError;
        qWarning("QSettingsPrivate::read: could not read %s", filename.toLocal8Bit().constData());
        return;
    }

    foreach (const QString &key, readmap.keys()) {
        map.insert(key, readmap.value(key));
    }

    timestamp = info.lastModified();
}

void QSettingsPrivate::write()
{
    QSettings::SettingsMap checkmap = map;
    QFileInfo info(filename);
    const QDateTime newstamp = info.lastModified();
    if (timestamp < newstamp || !newstamp.isValid()) {
        QSettingsPrivate::read();
    }
    if (checkmap == map) {
        return;
    }

    QMutexLocker lock(qSettingsMutex());
    QFile file(filename);
    if (Q_UNLIKELY(!file.open(QFile::WriteOnly))) {
        status = QSettings::AccessError;
        qWarning("QSettingsPrivate::write: failed to open %s", filename.toLocal8Bit().constData());
        return;
    }

    if (Q_UNLIKELY(!unixLock(file.handle(), F_WRLCK))) {
        status = QSettings::AccessError;
        qWarning("QSettingsPrivate::write: failed to lock %s", filename.toLocal8Bit().constData());
        return;
    }

    if (Q_UNLIKELY(!writeFunc(file, map))) {
        status = QSettings::FormatError;
        qWarning("QSettingsPrivate::write: could not write %s", filename.toLocal8Bit().constData());
    }
}

void QSettingsPrivate::notify()
{
    Q_Q(QSettings);
    QMutexLocker locker(qSettingsMutex());
    for (int i = 0; i < qGlobalSettings()->size(); i++) {
        QSettings *setting = qGlobalSettings()->at(i);
        if (setting != q && setting->fileName() == q->fileName()) {
            setting->d_func()->map = map;
        }
    }
}

QString QSettingsPrivate::variantToString(const QVariant &v)
{
    QString result;

    switch (v.type()) {
        case QVariant::Invalid:
            result = QLatin1String("@Invalid()");
            break;

        case QVariant::ByteArray: {
            QByteArray a = v.toByteArray();
            result = QLatin1String("@ByteArray(");
            result += QString::fromLatin1(a.constData(), a.size());
            result += QLatin1Char(')');
            break;
        }

        case QVariant::String:
        case QVariant::LongLong:
        case QVariant::ULongLong:
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::Bool:
        case QVariant::Double:
        case QVariant::KeySequence: {
            result = v.toString();
            if (result.startsWith(QLatin1Char('@')))
                result.prepend(QLatin1Char('@'));
            break;
        }
#ifndef QT_NO_GEOM_VARIANT
        case QVariant::Rect: {
            QRect r = qvariant_cast<QRect>(v);
            result += QLatin1String("@Rect(");
            result += QString::number(r.x());
            result += QLatin1Char(' ');
            result += QString::number(r.y());
            result += QLatin1Char(' ');
            result += QString::number(r.width());
            result += QLatin1Char(' ');
            result += QString::number(r.height());
            result += QLatin1Char(')');
            break;
        }
        case QVariant::Size: {
            QSize s = qvariant_cast<QSize>(v);
            result += QLatin1String("@Size(");
            result += QString::number(s.width());
            result += QLatin1Char(' ');
            result += QString::number(s.height());
            result += QLatin1Char(')');
            break;
        }
        case QVariant::Point: {
            QPoint p = qvariant_cast<QPoint>(v);
            result += QLatin1String("@Point(");
            result += QString::number(p.x());
            result += QLatin1Char(' ');
            result += QString::number(p.y());
            result += QLatin1Char(')');
            break;
        }
#endif // !QT_NO_GEOM_VARIANT

        default: {
#ifndef QT_NO_DATASTREAM
            QByteArray a;
            {
                QDataStream s(&a, QIODevice::WriteOnly);
                s << v;
            }

            result = QLatin1String("@Variant(");
            result += QString::fromLatin1(a.constData(), a.size());
            result += QLatin1Char(')');
#else
            Q_ASSERT(!"QSettings: Cannot save custom types without QDataStream support");
#endif
            break;
        }
    }

    return result;
}


QVariant QSettingsPrivate::stringToVariant(const QString &s)
{
    if (s.startsWith(QLatin1Char('@'))) {
        if (s.endsWith(QLatin1Char(')'))) {
            if (s.startsWith(QLatin1String("@ByteArray("))) {
                return QVariant(s.toLatin1().mid(11, s.size() - 12));
            } else if (s.startsWith(QLatin1String("@Variant("))) {
#ifndef QT_NO_DATASTREAM
                QByteArray a(s.toLatin1().mid(9));
                QDataStream stream(&a, QIODevice::ReadOnly);
                QVariant result;
                stream >> result;
                return result;
#else
                Q_ASSERT(!"QSettings: Cannot load custom types without QDataStream support");
#endif
#ifndef QT_NO_GEOM_VARIANT
            } else if (s.startsWith(QLatin1String("@Rect("))) {
                QStringList args = QSettingsPrivate::splitArgs(s, 5);
                if (args.size() == 4)
                    return QVariant(QRect(args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt()));
            } else if (s.startsWith(QLatin1String("@Size("))) {
                QStringList args = QSettingsPrivate::splitArgs(s, 5);
                if (args.size() == 2)
                    return QVariant(QSize(args[0].toInt(), args[1].toInt()));
            } else if (s.startsWith(QLatin1String("@Point("))) {
                QStringList args = QSettingsPrivate::splitArgs(s, 6);
                if (args.size() == 2)
                    return QVariant(QPoint(args[0].toInt(), args[1].toInt()));
#endif
            } else if (s == QLatin1String("@Invalid()")) {
                return QVariant();
            }

        }
        if (s.startsWith(QLatin1String("@@")))
            return QVariant(s.mid(1));
    } else if (s.contains(QLatin1Char(','))) {
        return QVariant(s.split(QLatin1Char(',')));
    }

    return QVariant(s);
}

QStringList QSettingsPrivate::splitArgs(const QString &s, int idx)
{
    int l = s.length();
    Q_ASSERT(l > 0);
    Q_ASSERT(s.at(idx) == QLatin1Char('('));
    Q_ASSERT(s.at(l - 1) == QLatin1Char(')'));

    QStringList result;
    QString item;

    for (++idx; idx < l; ++idx) {
        QChar c = s.at(idx);
        if (c == QLatin1Char(')')) {
            Q_ASSERT(idx == l - 1);
            result.append(item);
        } else if (c == QLatin1Char(' ')) {
            result.append(item);
            item.clear();
        } else {
            item.append(c);
        }
    }

    return result;
}

/*!
    \class QSettings
    \brief The QSettings class provides persistent platform-independent application settings.

    \ingroup io

    \reentrant

    Users normally expect an application to remember its settings
    (window sizes and positions, options, etc.) across sessions.
    On Unix systems, in the absence of a standard, many applications
    (including the KDE applications) use INI text files.

    QSettings is an abstraction around these technologies, enabling
    you to save and restore application settings in a portable
    manner. It also supports \l{registerFormat()}{custom storage
    formats}.

    QSettings's API is based on QVariant, allowing you to save
    most value-based types, such as QString, QRect, and QImage,
    with the minimum of effort.

    If all you need is a non-persistent memory-based structure,
    consider using QMap<QString, QVariant> instead.

    \tableofcontents section1

    \section1 Basic Usage

    If you use QSettings from many places in your application, you
    might want to specify the application name using
    QCoreApplication::setApplicationName() and then use the default
    QSettings constructor:

    QSettings stores settings. Each setting consists of a QString
    that specifies the setting's name (the \e key) and a QVariant
    that stores the data associated with the key. To write a setting,
    use setValue().

    If there already exists a setting with the same key, the existing
    value is overwritten by the new value. For efficiency, the
    changes may not be saved to permanent storage immediately. (You
    can always call sync() to commit your changes.)

    You can get a setting's value back using value(). If there is no
    setting with the specified name, QSettings returns a null QVariant
    (which can be converted to the integer 0). You can specify
    another default value by passing a second argument to value():

    To test whether a given key exists, call contains(). To remove
    the setting associated with a key, call remove(). To obtain the
    list of all keys, call keys(). To get a map of all settings, call
    map(). To remove all keys, call clear().

    \section1 QVariant and GUI Types

    Because QVariant is part of the \l QtCore library, it cannot provide
    conversion functions to data types such as QColor, QImage, and
    QPixmap, which are part of \l QtGui. In other words, there is no
    \c toColor(), \c toImage(), or \c toPixmap() functions in QVariant.
    Instead, you can use the QVariant::value() or the qVariantValue()
    template function.

    The inverse conversion (e.g., from QColor to QVariant) is
    automatic for all data types supported by QVariant, including
    GUI-related types.

    Custom types registered using qRegisterMetaType() and
    qRegisterMetaTypeStreamOperators() can be stored using QSettings.

    \section1 Section and Key Syntax

    Setting keys can contain any Unicode characters. To avoid portability
    problems, follow these simple rules:

    \list 1
    \o Always refer to the same key using the same case. For example,
       if you refer to a key as "text fonts" in one place in your
       code, don't refer to it as "Text Fonts" somewhere else.

    \o Avoid key names that are identical except for the case. For
       example, if you have a key called "MainWindow", don't try to
       save another key as "mainwindow".

    \o Do not use slashes ('/' and '\\') in key names. The backslash
       character is used to separate sub keys (see below). On
       windows '\\' are converted by QSettings to '/', which makes
       them identical.
    \endlist

    You can form hierarchical keys using the '/' character as a
    separator, similar to Unix file paths.

    If you want to use specific format instead of the native, you
    can pass QSettings::IniFormat as the first argument to the
    QSettings constructor, followed by the scope.

    \section1 Restoring the State of a GUI Application

    QSettings is often used to store the state of a GUI application.
    See \l{Window Geometry} for a discussion on why it is better to
    call QWidget::resize() and QWidget::move() rather than
    QWidget::setGeometry() to restore a window's geometry.

    The \c readSettings() and \c writeSettings() functions must be
    called from the main window's constructor and close event handler.

    \section1 Accessing Settings from Multiple Threads or Processes Simultaneously

    QSettings is \l{reentrant}. This means that you can use distinct
    QSettings object in different threads simultaneously. This
    guarantee stands even when the QSettings objects refer to the
    same files on disk. If a setting is modified through one
    QSettings object, the change will immediately be visible in
    any other QSettings objects that operate on the same location
    and that live in the same process.

    QSettings can safely be used from different processes (which can
    be different instances of your application running at the same
    time or different applications altogether) to read and write to
    the same system locations. It uses advisory file locking and a
    smart merging algorithm to ensure data integrity. Note that sync()
    imports changes made by other processes (in addition to writing
    the changes from this QSettings).

    \section1 Platform-Specific Notes

    \section2 Locations Where Application Settings Are Stored

    QSettings stores settings for an application in two locations,
    depending on whether the settings are user-specific or
    system-wide. For simplicity, we're assuming the application is
    called MySoft.

    On Unix systems, if the file format is NativeFormat, the
    following files are used by default:

    \list 1
    \o \c{$HOME/.config/MySoft.json}
    \o \c{/etc/xdg/MySoft.json}
    \endlist

    If the file format is IniFormat, the following files are
    used on Unix:

    \list 1
    \o \c{$HOME/.config/MySoft.ini}
    \o \c{/etc/xdg/MySoft.ini}
    \endlist

    The paths for the \c .ini and \c .json files can be changed by the
    user by setting the \c XDG_CONFIG_HOME environment variable.

    \section2 Accessing Files Directly

    Sometimes you do want to access settings stored in a specific file.
    If you want to read an INI file directly, you can use the QSettings
    constructor that takes a file name as first argument and pass
    QSettings::IniFormat as second argument. You can then use the
    QSettings object to read and write settingsin the file.

    \section2 Platform Limitations

    While QSettings attempts to smooth over the differences between
    the different supported platforms, there are still a few
    differences that you should be aware of when porting your
    application:

    \list
    \o On Unix systems, the advisory file locking is disabled if NFS (or AutoFS
       or CacheFS) is detected to work around a bug in the NFS fcntl()
       implementation, which hangs forever if statd or lockd aren't running.

    \endlist

    \sa QVariant, QSessionManager
*/

/*! \enum QSettings::SettingsStatus

    The following status values are possible:

    \value NoError  No error occurred.
    \value AccessError  An access error occurred (e.g. trying to write to a read-only file).
    \value FormatError  A format error occurred (e.g. loading a malformed INI file).

    \sa status()
*/

/*! \enum QSettings::Format

    This enum type specifies the storage format used by QSettings.

    \value NativeFormat  Store the settings in JSON files.
    \value IniFormat  Store the settings in INI files.
    \value InvalidFormat Special value returned by registerFormat().
    \omitvalue CustomFormat1
    \omitvalue CustomFormat2
    \omitvalue CustomFormat3
    \omitvalue CustomFormat4
    \omitvalue CustomFormat5
    \omitvalue CustomFormat6
    \omitvalue CustomFormat7
    \omitvalue CustomFormat8
    \omitvalue CustomFormat9
    \omitvalue CustomFormat10
    \omitvalue CustomFormat11
    \omitvalue CustomFormat12
    \omitvalue CustomFormat13
    \omitvalue CustomFormat14
    \omitvalue CustomFormat15
    \omitvalue CustomFormat16

    The INI file format is a Windows file format that Qt supports on
    all platforms. In the absence of an INI standard, we try to
    follow what Microsoft does, with the following exceptions:

    \list
    \o  If you store types that QVariant can't convert to QString
        (e.g., QPoint, QRect, and QSize), Qt uses an \c{@}-based
        syntax to encode the type. For example:

        \snippet doc/src/snippets/code/src_corelib_io_qsettings.cpp 8

        To minimize compatibility issues, any \c @ that doesn't
        appear at the first position in the value or that isn't
        followed by a Qt type (\c Point, \c Rect, \c Size, etc.) is
        treated as a normal character.

    \o  Although backslash is a special character in INI files, most
        Windows applications don't escape backslashes (\c{\}) in file
        paths:

        \snippet doc/src/snippets/code/src_corelib_io_qsettings.cpp 9

        QSettings always treats backslash as a special character and
        provides no API for reading or writing such entries.

    \o  The INI file format has severe restrictions on the syntax of
        a key. Qt works around this by using \c % as an escape
        character in keys. In addition, if you save a top-level
        setting (a key with no slashes in it, e.g., "someKey"), it
        will appear in the INI file's "General" section. To avoid
        overwriting other keys, if you save something using the a key
        such as "General/someKey", the key will be located in the
        "%General" section, \e not in the "General" section.

    \o  Following the philosophy that we should be liberal in what
        we accept and conservative in what we generate, QSettings
        will accept Latin-1 encoded INI files, but generate pure
        ASCII files, where non-ASCII values are encoded using standard
        INI escape sequences.
    \endlist

    \sa registerFormat()
*/

/*! \enum QSettings::Scope

    This enum specifies whether settings are user-specific or shared
    by all users of the same system.

    \value UserScope  Store settings in a location specific to the
                      current user (e.g., in the user's home
                      directory).
    \value SystemScope  Store settings in a global location, so that
                        all users on the same machine access the same
                        set of settings.
*/

/*!
    Constructs a QSettings object for accessing settings of the
    application called \a application from the organization called \a
    organization, and with parent \a parent.

    If \a scope is QSettings::UserScope, the QSettings object searches
    user-specific settings only. If \a scope is QSettings::SystemScope,
    the QSettings object ignores user-specific settings and provides
    access to system-wide settings.

    The storage format is set to QSettings::NativeFormat.
*/
QSettings::QSettings(Scope scope, QObject *parent)
    : QObject(*new QSettingsPrivate(NativeFormat, scope), parent)
{
    QMutexLocker locker(qSettingsMutex());
    qGlobalSettings()->append(this);
    Q_D(QSettings);
    d->read();
}

/*!
    Constructs a QSettings object for accessing settings of the
    application called \a application from the organization called
    \a organization, and with parent \a parent.

    If \a scope is QSettings::UserScope, the QSettings object searches
    user-specific settings only. If \a scope is QSettings::SystemScope,
    the QSettings object ignores user-specific settings and provides
    access to system-wide settings.

    If \a format is QSettings::NativeFormat, the native JSON API is used
    for storing settings. If \a format is QSettings::IniFormat, the INI
    format is used.
*/
QSettings::QSettings(Format format, Scope scope, QObject *parent)
    : QObject(*new QSettingsPrivate(format, scope), parent)
{
    QMutexLocker locker(qSettingsMutex());
    qGlobalSettings()->append(this);
    Q_D(QSettings);
    d->read();
}

/*!
    Constructs a QSettings object for accessing the settings
    stored in the file called \a fileName, with parent \a parent. If
    the file doesn't already exist, it is created.

    The meaning of \a fileName depends on the format. If \a format is
    QSettings::NativeFormat, the filename will end with end with .json
    suffix, if it is QSettings::IniFormat it will be suffixed with .ini.

    If \a format is QSettings::IniFormat, \a fileName is the name of an INI
    file.

    \list
    \o QSettings provides no way of reading INI "path" entries, i.e., entries
       with unescaped slash characters. (This is because these entries are
       ambiguous and cannot be resolved automatically.)
    \o In INI files, QSettings uses the \c @ character as a metacharacter in some
       contexts, to encode Qt-specific data types (e.g., \c @Rect), and might
       therefore misinterpret it when it occurs in pure INI files.
    \endlist

    \sa fileName()
*/
QSettings::QSettings(const QString &fileName, Format format, QObject *parent)
    : QObject(*new QSettingsPrivate(fileName, format), parent)
{
    QMutexLocker locker(qSettingsMutex());
    qGlobalSettings()->append(this);
    Q_D(QSettings);
    d->read();
}

/*!
    Constructs a QSettings object for accessing settings of the
    application set previously with a call to
    QCoreApplication::setApplicationName().

    The scope is QSettings::UserScope and the format is
    QSettings::NativeFormat.

    \sa QCoreApplication::setApplicationName()
*/
QSettings::QSettings(QObject *parent)
    : QObject(*new QSettingsPrivate(QSettings::NativeFormat, UserScope), parent)
{
    QMutexLocker locker(qSettingsMutex());
    qGlobalSettings()->append(this);
    Q_D(QSettings);
    d->read();
}

/*!
    Destroys the QSettings object.

    Any unsaved changes will eventually be written to permanent
    storage.

    \sa sync()
*/
QSettings::~QSettings()
{
    QSettings::sync();
    QMutexLocker locker(qSettingsMutex());
    const int index = qGlobalSettings()->indexOf(this);
    qGlobalSettings()->remove(index);
}

/*!
    Removes all entries in the primary location associated to this
    QSettings object.

    \sa remove()
*/
void QSettings::clear()
{
    Q_D(QSettings);
    d->map.clear();
    d->notify();
}

/*!
    Writes any unsaved changes to permanent storage, and reloads any
    settings that have been changed in the meantime by another
    application.

    This function is called automatically from QSettings's destructor and
    by the event loop at regular intervals, so you normally don't need to
    call it yourself.

    \sa status()
*/
void QSettings::sync()
{
    Q_D(QSettings);
    d->write();
    d->notify();
}

/*!
    Returns the path where settings written using this QSettings
    object are stored.

    \sa isWritable(), format()
*/
QString QSettings::fileName() const
{
    Q_D(const QSettings);
    return d->filename;
}

/*!
    \since 4.4

    Returns the format used for storing the settings.

    \sa fileName(), scope()
*/
QSettings::Format QSettings::format() const
{
    Q_D(const QSettings);
    return d->format;
}

/*!
    \since 4.4

    Returns the scope used for storing the settings.

    \sa format()
*/
QSettings::Scope QSettings::scope() const
{
    Q_D(const QSettings);
    return d->scope;
}

/*!
    Returns a status code indicating the first error that was met by
    QSettings, or QSettings::NoError if no error occurred.

    Be aware that QSettings delays performing some operations. For this
    reason, you might want to call sync() to ensure that the data stored
    in QSettings is written to disk before calling status().

    \sa sync()
*/
QSettings::SettingsStatus QSettings::status() const
{
    Q_D(const QSettings);
    return d->status;
}

/*!
    Returns a map of all keys, with their values, that the QSettings
    object holds.

    \sa keys()
*/
QSettings::SettingsMap QSettings::map() const
{
    Q_D(const QSettings);
    return d->map;
}

/*!
    Returns a list of all key top-level groups that contain keys that
    can be read using the QSettings object.

    You can navigate through the entire setting hierarchy using
    keys() and map().

    \sa map()
*/
QStringList QSettings::keys() const
{
    Q_D(const QSettings);
    return d->map.keys();
}

/*!
    Returns true if settings can be written using this QSettings
    object; returns false otherwise.

    One reason why isWritable() might return false is if
    QSettings operates on a read-only file.

    \warning This function is not perfectly reliable, because the
    file permissions can change at any time.

    \sa fileName(), status(), sync()
*/
bool QSettings::isWritable() const
{
    Q_D(const QSettings);
    QFileInfo info(d->filename);
    return info.isWritable();
}

/*!
  
  Sets the value of setting \a key to \a value. If the \a key already
  exists, the previous value is overwritten.

  \sa value(), remove(), contains()
*/
void QSettings::setValue(const QString &key, const QVariant &value)
{
    Q_D(QSettings);
    d->map.insert(key, value);
    d->notify();
}

/*!
    Removes the setting \a key.

    \sa setValue(), value(), contains()
*/
void QSettings::remove(const QString &key)
{
    Q_D(QSettings);
    d->map.remove(key);
    d->notify();
}

/*!
    Returns true if there exists a setting called \a key; returns
    false otherwise.

    \sa value(), setValue()
*/
bool QSettings::contains(const QString &key) const
{
    Q_D(const QSettings);
    return d->map.contains(key);
}

/*!
    Returns the value for setting \a key. If the setting doesn't
    exist, returns \a defaultValue.

    If no default value is specified, a default QVariant is
    returned.

    \sa setValue(), contains(), remove()
*/
QVariant QSettings::value(const QString &key, const QVariant &defaultValue) const
{
    Q_D(const QSettings);
    return d->map.value(key, defaultValue);
}

/*!
    \typedef QSettings::SettingsMap

    Typedef for QMap<QString, QVariant>.
	
    \sa registerFormat()
*/

/*!
    \typedef QSettings::ReadFunc

    Typedef for a pointer to a function with the following signature:

    \snippet doc/src/snippets/code/src_corelib_io_qsettings.cpp 27

    \c ReadFunc is used in \c registerFormat() as a pointer to a function
    that reads a set of key/value pairs. \c ReadFunc should read all the 
    options in one pass, and return all the settings in the \c SettingsMap 
    container, which is initially empty.

    \sa WriteFunc, registerFormat()
*/

/*!
    \typedef QSettings::WriteFunc

    Typedef for a pointer to a function with the following signature:

    \snippet doc/src/snippets/code/src_corelib_io_qsettings.cpp 28

    \c WriteFunc is used in \c registerFormat() as a pointer to a function 
    that writes a set of key/value pairs. \c WriteFunc is only called once,
    so you need to output the settings in one go.

    \sa ReadFunc, registerFormat()
*/

/*!
    \since 4.1
    \threadsafe

    Registers a custom storage format. On success, returns a special
    Format value that can then be passed to the QSettings constructor.
    On failure, returns InvalidFormat.

    The \a extension is the file
    extension associated to the format (without the '.').

    The \a readFunc and \a writeFunc parameters are pointers to
    functions that read and write a set of key/value pairs. The
    QIODevice parameter to the read and write functions is always
    opened in binary mode (i.e., without the QIODevice::Text flag).
*/
QSettings::Format QSettings::registerFormat(const QString &extension, ReadFunc readFunc,
                                            WriteFunc writeFunc)
{
    QMutexLocker locker(qSettingsMutex());
    CustomFormatVector *customFormatVector = customFormatVectorFunc();
    int index = customFormatVector->size();
    if (index == 16) // the QSettings::Format enum has room for 16 custom formats
        return QSettings::InvalidFormat;

    QSettingsCustomFormat info;
    info.format = static_cast<QSettings::Format>(QSettings::CustomFormat1 + index);
    info.extension = QLatin1Char('.');
    info.extension += extension;
    info.readFunc = readFunc;
    info.writeFunc = writeFunc;
    customFormatVector->append(info);

    return info.format;
}

#include "moc_qsettings.h"

QT_END_NAMESPACE

#endif // QT_NO_SETTINGS
