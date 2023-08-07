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
#include "qstandardpaths.h"
#include "qcoreapplication.h"
#include "qcore_unix_p.h"

#ifndef QT_NO_SETTINGS

#include "qsettings_p.h"
#include "qfile.h"
#include "qdir.h"

QT_BEGIN_NAMESPACE

static const QLatin1String s_stringlistdelim = QLatin1String(",");

// ************************************************************************
// INI format
static bool ini_settings_read(QIODevice &device, QSettingsMap &map)
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
        const QString stringvalue(QString::fromAscii(value.constData(), value.size()));
        if (section.isEmpty()) {
            map.insert(key, stringvalue);
        } else {
            map.insert(section + '/' + key, stringvalue);
        }

        parsedsomething = true;
    }

    // qDebug() << "ini_settings_read" << map;
    return parsedsomething;
}

static bool ini_settings_write(QIODevice &device, const QSettingsMap &map)
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

        const QString stringvalue = map.value(key);
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

static inline void createLeadingDir(const QString &filepath)
{
    QDir().mkpath(filepath);
}

static QString getSettingsPath(const QString &filename, const QLatin1String &extension)
{
    QFileInfo info(filename);
    if (info.isAbsolute()) {
        createLeadingDir(info.absolutePath());
        return filename;
    }

    QString nameandext = filename;
    if (!filename.endsWith(extension)) {
        nameandext += extension;
    }

    const QStringList locations = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    foreach (const QString &location, locations) {
        QStatInfo locationinfo(location);
        if (locationinfo.isWritable()) {
            createLeadingDir(location);
            return location + QDir::separator() + nameandext;
        }
    }

    createLeadingDir(locations.first());
    return locations.first() + QDir::separator() + nameandext;
}

QSettingsPrivate::QSettingsPrivate()
    : status(QSettings::NoError), shouldwrite(false)
{
    filename = getSettingsPath(QCoreApplication::applicationName(), QLatin1String(".ini"));
}

QSettingsPrivate::QSettingsPrivate(const QString &fileName)
    : status(QSettings::NoError), shouldwrite(false)
{
    filename = getSettingsPath(fileName, QLatin1String(".ini"));
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

    if (Q_UNLIKELY(!ini_settings_read(readfile, map))) {
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

    QSettingsMap mergemap;
    {
        QFile readfile(filename);
        if (readfile.open(QFile::ReadOnly)) {
            qt_lock_fd(readfile.handle(), true);
            ini_settings_read(readfile, mergemap);
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

    if (Q_UNLIKELY(!ini_settings_write(writefile, mergemap))) {
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

    QSettings's API is based on QString, QStringList and qlonglong. It
    allowing you to save most value-based types.

    \tableofcontents section1

    \section1 Basic Usage

    If you use QSettings from many places in your application, you
    might want to specify the application name using
    QCoreApplication::setApplicationName() and then use the default
    QSettings constructor:

    QSettings stores settings. Each setting consists of a QString
    that specifies the setting's name (the \e key) and a QString,
    QStringList or qlonglong that stores the data associated with the
    key. To write a setting, use setString(), setStringList(),
    setInteger() or setBoolean().

    If there already exists a setting with the same key, the existing
    value is overwritten by the new value. For efficiency, the
    changes may not be saved to permanent storage immediately. (You
    can always call sync() to commit your changes.)

    You can get a setting's value back using string(), stringList(),
    integer() or boolean(). If there is no setting with the specified
    name, QSettings returns a default value. You can specify another
    default value by passing a second argument:

    To test whether a given key exists, call contains(). To remove
    the setting associated with a key, call remove(). To obtain the
    list of all keys, call keys(). To get a map of all settings, call
    map(). To remove all keys, call clear().

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

    On Unix systems, the following files are used:

    \list 1
    \o \c{$HOME/.config/MySoft.ini}
    \o \c{/etc/xdg/MySoft.ini}
    \endlist

    The paths for the files can be changed by the user by setting the
    \c XDG_CONFIG_HOME environment variable.

    \section2 Accessing Files Directly

    Sometimes you do want to access settings stored in a specific file.
    If you want to read an INI file directly, you can use the QSettings
    constructor that takes a file name as first argument. You can then
    use the QSettings object to read and write settingsin the file.

    \sa QSessionManager
*/

/*! \enum QSettings::SettingsStatus

    The following status values are possible:

    \value NoError  No error occurred.
    \value AccessError  An access error occurred (e.g. trying to write to a read-only file).
    \value FormatError  A format error occurred (e.g. loading a malformed INI file).

    \sa status()
*/

/*!
    Constructs a QSettings object for accessing settings of the
    application set previously with a call to
    QCoreApplication::setApplicationName().

    \sa QCoreApplication::setApplicationName()
*/
QSettings::QSettings()
    : d_ptr(new QSettingsPrivate())
{
    Q_D(QSettings);
    d->read();
}

/*!
    Constructs a QSettings object for accessing the settings stored in the
    file called \a fileName. If the file doesn't already exist, it is created.

    The \a fileName will be suffixed with .ini if it is relative.

    \sa fileName()
*/
QSettings::QSettings(const QString &fileName)
    : d_ptr(new QSettingsPrivate(fileName))
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

    \sa string(), remove(), contains()
*/
void QSettings::setString(const QString &key, const QString &value)
{
    Q_D(QSettings);
    d->map.insert(d->toGroupKey(key), value);
    d->shouldwrite = true;
}

/*!
    Returns the value for setting \a key. If the setting doesn't
    exist, returns \a defaultValue.

    If no default value is specified, a default QString is returned.

    \sa setString(), contains(), remove()
*/
QString QSettings::string(const QString &key, const QString &defaultValue) const
{
    Q_D(const QSettings);
    return d->map.value(d->toGroupKey(key), defaultValue);
}

/*!
    Sets the value of setting \a key to \a value. If the \a key already
    exists, the previous value is overwritten.

    \sa stringList(), remove(), contains()
*/
void QSettings::setStringList(const QString &key, const QStringList &value)
{
    setString(key, value.join(s_stringlistdelim));
}

/*!
    Returns the value for setting \a key. If the setting doesn't
    exist, returns \a defaultValue.

    If no default value is specified, a default QStringList is returned.

    \sa setString(), contains(), remove()
*/
QStringList QSettings::stringList(const QString &key, const QStringList &defaultValue) const
{
    return string(key, defaultValue.join(s_stringlistdelim)).split(s_stringlistdelim, QString::SkipEmptyParts);
}

/*!
    Sets the value of setting \a key to \a value. If the \a key already
    exists, the previous value is overwritten.

    \sa integer(), remove(), contains()
*/
void QSettings::setInteger(const QString &key, const qlonglong value)
{
    setString(key, QString::number(value));
}

/*!
    Returns the value for setting \a key. If the setting doesn't
    exist, returns \a defaultValue.

    If no default value is specified, a default qlonglong is returned.

    \sa setInteger(), contains(), remove()
*/
qlonglong QSettings::integer(const QString &key, const qlonglong defaultValue) const
{
    return string(key, QString::number(defaultValue)).toLongLong();
}

/*!
    Sets the value of setting \a key to \a value. If the \a key already
    exists, the previous value is overwritten.

    \sa boolean(), remove(), contains()
*/
void QSettings::setBoolean(const QString &key, const bool value)
{
    setString(key, value ? QString::fromLatin1("true") : QString::fromLatin1("false"));
}

/*!
    Returns the value for setting \a key. If the setting doesn't
    exist, returns \a defaultValue.

    If no default value is specified, a default bool is returned.

    \sa setBoolean(), contains(), remove()
*/
bool QSettings::boolean(const QString &key, const bool defaultValue) const
{
    const QString value = string(key, defaultValue ? QString::fromLatin1("true") : QString::fromLatin1("false"));
    return (value != QLatin1String("0") && value != QLatin1String("false"));
}

/*!
    Removes the setting \a key.

    \sa setString(), string(), contains()
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

    \sa setString(), string()
*/
bool QSettings::contains(const QString &key) const
{
    Q_D(const QSettings);
    return d->map.contains(d->toGroupKey(key));
}

QT_END_NAMESPACE

#endif // QT_NO_SETTINGS
