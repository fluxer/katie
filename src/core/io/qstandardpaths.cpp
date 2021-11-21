/****************************************************************************
**
** Copyright (c) 2012-2015 Barbara Geller
** Copyright (c) 2012-2015 Ansel Sermersheim
** Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
** Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qstandardpaths.h"

#include "qdir.h"
#include "qfileinfo.h"
#include "qhash.h"
#include "qobject.h"
#include "qcoreapplication.h"
#include "qcore_unix_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QStandardPaths
    \inmodule QtCore
    \brief The QStandardPaths class provides methods for accessing standard paths.
    \since 4.9

    This class contains functions to query standard locations on the local
    filesystem, for common tasks such as user-specific directories or system-wide
    configuration directories.
*/

/*!
    \enum QStandardPaths::StandardLocation

    This enum describes the different locations that can be queried using
    methods such as QStandardPaths::writableLocation, QStandardPaths::standardLocations,
    and QStandardPaths::displayName.

    \value DesktopLocation Returns the user's desktop directory.
    \value DocumentsLocation Returns the user's document.
    \value DownloadLocation Returns a directory for user's downloaded files.
           If no directory specific for downloads exists, a sensible fallback for storing user
           documents is returned.
    \value VideosLocation Returns the user's movies.
    \value MusicLocation Returns the user's music.
    \value PicturesLocation Returns the user's pictures.
    \value PublicLocation Returns the user's public share.
    \value TemplatesLocation Returns the user's templates.
    \value DataLocation Returns a directory location where persistent
           application data can be stored.
    \value CacheLocation Returns a directory location where user-specific
           non-essential (cached) data should be written.

    The following table gives examples of paths on different operating systems.
    The first path is the writable path (unless noted). Other, additional
    paths, if any, represent non-writable locations.

    \table
    \header \li Path type \li Linux (including Android)
    \row \li DesktopLocation
         \li "~/Desktop"
    \row \li DocumentsLocation
         \li "~/Documents"
    \row \li DownloadLocation
         \li "~/Downloads"
    \row \li VideosLocation
         \li "~/Videos"
    \row \li MusicLocation
         \li "~/Music"
    \row \li PicturesLocation
         \li "~/Pictures"
    \row \li PublicLocation
         \li "~/Public"
    \row \li Templates
         \li "~/Templates"
    \row \li DataLocation
         \li "~/.local/share/<APPNAME>", "/usr/local/share/<APPNAME>", "/usr/share/<APPNAME>"
    \row \li CacheLocation
         \li "~/.cache/<APPNAME>"
    \row \li ConfigLocation
         \li "~/.config", "/etc/xdg"
    \row \li RuntimeLocation
         \li "/var/tmp"
         \li "/run/user/<USER>"
    \endtable

    In the table above, \c <APPNAME> is usually the organization name, the
    application name, or both, or a unique name generated at packaging.
    Similarly, <APPROOT> is the location where this application is installed
    (often a sandbox). <APPDIR> is the directory containing the application
    executable.

    The paths above should not be relied upon, as they may change according to
    OS configuration, locale, or they may change in future Qt versions.

    \sa writableLocation(), standardLocations(), displayName(), locate(), locateAll()
*/

/*!
    \fn QString QStandardPaths::writableLocation(StandardLocation type)

    Returns the directory where files of \a type should be written to, or an empty string
    if the location cannot be determined.

    \note The storage location returned can be a directory that does not exist; i.e., it
    may need to be created by the system or the user.
*/

/*!
    \fn QStringList QStandardPaths::standardLocations(StandardLocation type)

    Returns all the directories where files of \a type belong.

    The list of directories is sorted from high to low priority, starting with
    writableLocation() if it can be determined. This list is empty if no locations
    for \a type are defined.

    \sa writableLocation()
 */

/*!
    \enum QStandardPaths::LocateOption

    This enum describes the different flags that can be used for
    controlling the behavior of QStandardPaths::locate and
    QStandardPaths::locateAll.

    \value LocateFile return only files
    \value LocateDirectory return only directories
*/

static inline bool existsAsSpecified(const QString &path, QStandardPaths::LocateOption options)
{
    QStatInfo info(path);
    if (options == QStandardPaths::LocateDirectory) {
        return info.isDir();
    }
    return info.isFile();
}

static inline QString checkExecutable(const QString &path)
{
    const QStatInfo info(path);
    if (info.isFile() && info.isExecutable()) {
        return QDir::cleanPath(path);
    }
    return QString();
}

static inline QString searchExecutable(const QStringList &searchPaths,
                                       const QString &executableName)
{
    const QDir currentDir = QDir::current();
    foreach (const QString &searchPath, searchPaths) {
        const QString candidate = currentDir.absoluteFilePath(searchPath + QLatin1Char('/') + executableName);
        const QString absPath = checkExecutable(QDir::cleanPath(candidate));
        if (!absPath.isEmpty()) {
            return absPath;
        }
    }
    return QString();
}

/*!
    Tries to find a file or directory called \a fileName in the standard locations
    for \a type.

    The full path to the first file or directory (depending on \a options) found is returned.
    If no such file or directory can be found, an empty string is returned.
 */
QString QStandardPaths::locate(StandardLocation type, const QString &fileName, LocateOption options)
{
    foreach (const QString &dir, standardLocations(type)) {
        const QString path = dir + QLatin1Char('/') + fileName;
        if (existsAsSpecified(path, options)) {
            return path;
        }
    }
    return QString();
}

/*!
    Tries to find all files or directories called \a fileName in the standard locations
    for \a type.

    The \a options flag allows to specify whether to look for files or directories.

    Returns the list of all the files that were found.
 */
QStringList QStandardPaths::locateAll(StandardLocation type, const QString &fileName, LocateOption options)
{
    QStringList result;
    foreach (const QString &dir, standardLocations(type)) {
        const QString path = dir + QLatin1Char('/') + fileName;
        if (existsAsSpecified(path, options)) {
            result.append(path);
        }
    }
    return result;
}

/*!
    Finds the executable named \a executableName in the paths specified by \a paths,
    or the system paths if \a paths is empty.

    On most operating systems the system path is determined by the PATH environment variable.

    The directories where to search for the executable can be set in the \a paths argument.
    To search in both your own paths and the system paths, call findExecutable twice, once with
    \a paths set and once with \a paths empty.

    Symlinks are not resolved, in order to preserve behavior for the case of executables
    whose behavior depends on the name they are invoked with.

    Returns the absolute file path to the executable, or an empty string if not found.
 */
QString QStandardPaths::findExecutable(const QString &executableName, const QStringList &paths)
{
    if (QFileInfo(executableName).isAbsolute()) {
        return checkExecutable(executableName);
    }

    if (paths.isEmpty()) {
        const QStringList envPaths = QString::fromLocal8Bit(::getenv("PATH")).split(QLatin1Char(':'), QString::SkipEmptyParts);
        return searchExecutable(envPaths, executableName);
    }

    return searchExecutable(paths, executableName);
}

/*!
    \fn QString QStandardPaths::displayName(StandardLocation type)

    Returns a localized display name for the given location \a type or
    an empty QString if no relevant location can be found.
*/

QString QStandardPaths::displayName(StandardLocation type)
{
    switch (type) {
        case DesktopLocation:
            return QCoreApplication::translate("QStandardPaths", "Desktop");
        case DocumentsLocation:
            return QCoreApplication::translate("QStandardPaths", "Documents");
        case DownloadLocation:
            return QCoreApplication::translate("QStandardPaths", "Download");
        case VideosLocation:
            return QCoreApplication::translate("QStandardPaths", "Videos");
        case MusicLocation:
            return QCoreApplication::translate("QStandardPaths", "Music");
        case PicturesLocation:
            return QCoreApplication::translate("QStandardPaths", "Pictures");
        case PublicLocation:
            return QCoreApplication::translate("QStandardPaths", "Public Share");
        case TemplatesLocation:
            return QCoreApplication::translate("QStandardPaths", "Templates");
        case DataLocation:
            return QCoreApplication::translate("QStandardPaths", "Application Data");
        case CacheLocation:
            return QCoreApplication::translate("QStandardPaths", "Cache");
        case ConfigLocation:
            return QCoreApplication::translate("QStandardPaths", "Configuration");
        case RuntimeLocation:
            return QCoreApplication::translate("QStandardPaths", "Runtime");
    }
    Q_UNREACHABLE();
}

QT_END_NAMESPACE

