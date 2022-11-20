/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdebug.h"
#include "qplatformdefs.h"
#include "qsettings.h"
#include "qjsondocument.h"
#include "qstandardpaths.h"
#include "qcoreapplication.h"
#include "qcore_unix_p.h"

#ifndef QT_NO_SETTINGS

#include "qsettings_p.h"
#include "qfile.h"
#include "qdir.h"

QT_BEGIN_NAMESPACE

struct QSettingsFormat
{
    QSettings::Format format;
    QString extension;
    QSettingsReadFunc readFunc;
    QSettingsWriteFunc writeFunc;
};

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
        qWarning("json_settings_read: %s", jsondoc.errorString().toUtf8().constData());
        return false;
    }

    map = jsondoc.toVariant().toMap();

    // qDebug() << "json_settings_read" << jsondoc.toJson();
    return true;
}

static bool json_settings_write(QIODevice &device, const QSettings::SettingsMap &map)
{
    QJsonDocument jsondoc = QJsonDocument::fromVariant(map);
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
        // extract section
        } else if (line.startsWith('[') && line.endsWith(']')) {
            section = line.mid(1, line.size() - 2);
            continue;
        }

        // extract key/value
        const int separatorpos = line.indexOf('=');
        if (separatorpos < 1) {
            continue;
        }

        const QByteArray key = line.left(separatorpos).trimmed();
        const QByteArray value = line.mid(separatorpos + 1).trimmed();
        const QVariant variantvalue(QString::fromAscii(value.constData(), value.size()));
        if (section.isEmpty()) {
            map.insert(key, variantvalue);
        } else {
            map.insert(section + '/' + key, variantvalue);
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
        QString actualkey = key;
        // extract section from the key
        const int separatorpos = key.indexOf(QLatin1Char('/'));
        if (separatorpos > 1) {
            section = QLatin1Char('[') + key.left(separatorpos) + QLatin1String("]\n");
            actualkey = key.mid(separatorpos + 1, key.size() - separatorpos - 1);
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

        if (!device.write(actualkey.toAscii())) {
            return false;
        }

        const QVariant variantvalue = map.value(key);
        if (!variantvalue.canConvert<QString>()) {
            return false;
        }
        const QString stringvalue = variantvalue.toString();
        const QString datavalue = QLatin1Char('=') + stringvalue + QLatin1Char('\n');
        if (!device.write(datavalue.toAscii())) {
            return false;
        }

        // qDebug() << "ini_settings_write" << section << actualkey << stringvalue;
    }

    return true;
}

// ************************************************************************
// QSettingsPrivate
static QSettingsFormat getSettingsFormat(QSettings::Format format)
{
    QSettingsFormat result;

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

    qWarning("QSettingsPrivate::getSettingsFormat: format not found %d", format);
    result.extension = QLatin1String(".json");
    result.readFunc = json_settings_read;
    result.writeFunc = json_settings_write;
    return result;
}

static inline QString createLeadingDir(const QString &filename)
{
    QFileInfo info(filename);
    QDir().mkpath(info.absolutePath());
    return filename;
}

static QString getSettingsPath(const QString &filename, const QString &extension)
{
    QFileInfo info(filename);
    if (info.isAbsolute()) {
        return createLeadingDir(filename);
    }

    QString nameandext = filename;
    if (!filename.endsWith(extension)) {
        nameandext += extension;
    }

    const QStringList locations = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    foreach (const QString &location, locations) {
        QStatInfo locationinfo(location);
        if (locationinfo.isWritable()) {
            return createLeadingDir(location + QDir::separator() + nameandext);
        }
    }

    return createLeadingDir(locations.first() + QDir::separator() + nameandext);
}

QSettingsPrivate::QSettingsPrivate(QSettings::Format format)
    : format(format), status(QSettings::NoError), shouldwrite(false)
{
    QSettingsFormat handler = getSettingsFormat(format);
    filename = getSettingsPath(QCoreApplication::applicationName(), handler.extension);
    readFunc = handler.readFunc;
    writeFunc = handler.writeFunc;
}

QSettingsPrivate::QSettingsPrivate(const QString &fileName, QSettings::Format format)
    : format(format), status(QSettings::NoError), shouldwrite(false)
{
    QSettingsFormat handler = getSettingsFormat(format);
    filename = getSettingsPath(fileName, handler.extension);
    readFunc = handler.readFunc;
    writeFunc = handler.writeFunc;
}

QSettingsPrivate::~QSettingsPrivate()
{
}

void QSettingsPrivate::read()
{
    QStatInfo fileinfo(filename);
    if (!fileinfo.isReadable() || fileinfo.size() == 0) {
        status = QSettings::AccessError;
        // no warning, fileinfo.exists() may return false if not readable
        return;
    }

    QFile readfile(filename);
    if (Q_UNLIKELY(!readfile.open(QFile::ReadOnly))) {
        status = QSettings::AccessError;
        qWarning("QSettingsPrivate::read: failed to open %s", filename.toLocal8Bit().constData());
        return;
    }
    qt_lock_fd(readfile.handle(), true);

    if (Q_UNLIKELY(!readFunc(readfile, map))) {
        status = QSettings::FormatError;
        qWarning("QSettingsPrivate::read: could not read %s", filename.toLocal8Bit().constData());
        return;
    }
}

void QSettingsPrivate::write()
{
    if (!shouldwrite) {
        return;
    }

    QSettings::SettingsMap mergemap;
    {
        QFile readfile(filename);
        if (readfile.open(QFile::ReadOnly)) {
            qt_lock_fd(readfile.handle(), true);
            readFunc(readfile, mergemap);
        }
        foreach(const QString &key, map.keys()) {
            mergemap.insert(key, map.value(key));
        }
    }

    QFile writefile(filename);
    if (Q_UNLIKELY(!writefile.open(QFile::WriteOnly))) {
        status = QSettings::AccessError;
        qWarning("QSettingsPrivate::write: failed to open %s", filename.toLocal8Bit().constData());
        return;
    }
    qt_lock_fd(writefile.handle(), false);

    if (Q_UNLIKELY(!writeFunc(writefile, mergemap))) {
        status = QSettings::FormatError;
        qWarning("QSettingsPrivate::write: could not write %s", filename.toLocal8Bit().constData());
        return;
    }
}

QString QSettingsPrivate::toGroupKey(const QString &key) const
{
    if (group.isEmpty()) {
        return key;
    }
    return group + QLatin1Char('/') + key;
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
    manner.

    QSettings's API is based on QVariant, allowing you to save
    most value-based types, such as QString.

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
       character is used to separate sub keys (see below).
    \endlist

    You can form hierarchical keys using the '/' character as a
    separator, similar to Unix file paths.

    If you want to use specific format instead of the native, you
    can pass QSettings::IniFormat as the first argument to the
    QSettings constructor.

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
    the same system locations. Note that sync() imports changes made
    by other processes (in addition to writing the changes from this
    QSettings).

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

    Katie supports on both formats all platforms. In the absence of an
    INI standard, the following should be noted:

    \list
    \o  The INI file format has severe restrictions on the syntax of
        a key. If you save a top-level setting (a key with no slashes
        in it, e.g., "someKey"), it will appear in the INI file without
        section.

    \o  The codec used to read and write the settings files is the
        same codec used for C-strings, US-ASCII by default.
    \endlist
*/

/*!
    Constructs a QSettings object for accessing settings of the
    application set previously with a call to
    QCoreApplication::setApplicationName().

    \sa QCoreApplication::setApplicationName()
*/
QSettings::QSettings()
    : d_ptr(new QSettingsPrivate(QSettings::NativeFormat))
{
    Q_D(QSettings);
    d->read();
}

/*!
    Constructs a QSettings object for accessing settings.

    If \a format is QSettings::NativeFormat, the native JSON API is used
    for storing settings. If \a format is QSettings::IniFormat, the INI
    format is used.
*/
QSettings::QSettings(Format format)
    : d_ptr(new QSettingsPrivate(format))
{
    Q_D(QSettings);
    d->read();
}

/*!
    Constructs a QSettings object for accessing the settings stored in the
    file called \a fileName. If the file doesn't already exist, it is created.

    The meaning of \a fileName depends on the format. If \a format is
    QSettings::NativeFormat, the filename will end with end with .json
    suffix, if it is QSettings::IniFormat it will be suffixed with .ini.

    If \a format is QSettings::IniFormat, \a fileName is the name of an INI
    file.

    \list
    \o In INI files, QSettings uses the \c @ character as a metacharacter in some
       contexts, to encode Katie-specific data types (e.g., \c @Rect), and might
       therefore misinterpret it when it occurs in pure INI files.
    \endlist

    \sa fileName()
*/
QSettings::QSettings(const QString &fileName, Format format)
    : d_ptr(new QSettingsPrivate(fileName, format))
{
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
    delete d_ptr;
}

/*!
    Removes all entries in the file associated to this QSettings object.

    \sa remove()
*/
void QSettings::clear()
{
    Q_D(QSettings);
    d->map.clear();
    d->shouldwrite = true;
}

/*!
    Writes any unsaved changes to permanent storage, and reloads any
    settings that have been changed in the meantime by another
    application.

    This function is called automatically from QSettings's destructor,
    so you normally don't need to call it yourself.

    \sa status()
*/
void QSettings::sync()
{
    Q_D(QSettings);
    d->write();
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

    \sa fileName()
*/
QSettings::Format QSettings::format() const
{
    Q_D(const QSettings);
    return d->format;
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

    \sa map(), groupKeys()
*/
QStringList QSettings::keys() const
{
    Q_D(const QSettings);
    return d->map.keys();
}

/*!
    Returns the current group.

    \sa beginGroup(), endGroup()
*/
QString QSettings::group() const
{
    Q_D(const QSettings);
    return d->group;
}

/*!
    Sets the group prefix to \a group.

    The current group is automatically prepended to all keys
    specified to QSettings. In addition, query functions such as
    groupKeys() are based on the group. By default, no group is set.

    Call endGroup() to reset the current group to what it was before
    the corresponding beginGroup() call. Groups cannot be nested.

    \sa endGroup(), group()
*/
void QSettings::beginGroup(const QString &group)
{
    Q_D(QSettings);

    if (Q_UNLIKELY(!d->group.isEmpty())) {
        qWarning("QSettings::beginGroup: sub-groups are not supported");
        return;
    }

    d->group = group;
}

/*!
    Resets the group to what it was before the corresponding
    beginGroup() call.

    \sa beginGroup(), group()
*/
void QSettings::endGroup()
{
    Q_D(QSettings);
    if (Q_UNLIKELY(d->group.isEmpty())) {
        qWarning("QSettings::endGroup: No matching beginGroup()");
        return;
    }

    d->group.clear();
}

/*!
    Returns a list of group-level keys without the group prefix
    that can be read using the QSettings object.

    If a group is not set using beginGroup(), empty list is
    returned.

    You can navigate through the entire setting hierarchy using
    keys(), beginGroup(), groupKeys() and endGroup() recursively.

    \sa keys()
*/
QStringList QSettings::groupKeys() const
{
    Q_D(const QSettings);
    if (d->group.isEmpty()) {
        return QStringList();
    }

    QStringList result;
    foreach(const QString &key, keys()) {
        if (!key.startsWith(d->group + QLatin1Char('/'))) {
            continue;
        }
        const int groupsize = d->group.size() + 1;
        QString groupkey = key.mid(groupsize, key.size() - groupsize);
        const int slashindex = groupkey.indexOf("/");
        if (slashindex) {
            groupkey = groupkey.mid(0, slashindex);
        }
        if (result.contains(groupkey)) {
            continue;
        }
        result.append(groupkey);
    }
    return result;
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
    QFileInfo fileinfo(d->filename);
    if (fileinfo.isWritable()) {
        return true;
    }
    // if the file does not exist, check if it can be created
    QStatInfo dirinfo(fileinfo.absolutePath());
    return dirinfo.isWritable();
}

/*!
  
  Sets the value of setting \a key to \a value. If the \a key already
  exists, the previous value is overwritten.

  \sa value(), remove(), contains()
*/
void QSettings::setValue(const QString &key, const QVariant &value)
{
    Q_D(QSettings);
    d->map.insert(d->toGroupKey(key), value);
    d->shouldwrite = true;
}

/*!
    Removes the setting \a key.

    \sa setValue(), value(), contains()
*/
void QSettings::remove(const QString &key)
{
    Q_D(QSettings);
    const QString groupkey = d->toGroupKey(key);
    foreach(const QString &key, d->map.keys()) {
        if (key.startsWith(groupkey)) {
            d->map.remove(key);
            d->shouldwrite = true;
        }
    }
}

/*!
    Returns true if there exists a setting called \a key; returns
    false otherwise.

    \sa value(), setValue()
*/
bool QSettings::contains(const QString &key) const
{
    Q_D(const QSettings);
    return d->map.contains(d->toGroupKey(key));
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
    return d->map.value(d->toGroupKey(key), defaultValue);
}

/*!
    \typedef QSettings::SettingsMap

    Typedef for QMap<QString, QVariant>.
*/

QT_END_NAMESPACE

#endif // QT_NO_SETTINGS
