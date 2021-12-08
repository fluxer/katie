/****************************************************************************
**
** Copyright (C) 2021 Ivailo Monev
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
    filesystem, for common tasks such as user-specific directories or
    system-wide configuration directories.
*/

/*!
    \enum QStandardPaths::StandardLocation

    This enum describes the different locations that can be queried using
    methods such as \c QStandardPaths::writableLocation,
    \c QStandardPaths::standardLocations, and \c QStandardPaths::displayName.

    \value DesktopLocation
    \value DocumentsLocation
    \value DownloadsLocation
    \value VideosLocation
    \value MusicLocation
    \value PicturesLocation
    \value PublicLocation
    \value TemplatesLocation
    \value DataLocation
    \value CacheLocation
    \value ConfigLocation
    \value RuntimeLocation
    \value StateLocation

    The following table gives examples of paths:

    \table
    \header \li Path type \li Linux
    \row \li DesktopLocation
         \li "$HOME/Desktop"
    \row \li DocumentsLocation
         \li "$HOME/Documents"
    \row \li DownloadsLocation
         \li "$HOME/Downloads"
    \row \li VideosLocation
         \li "$HOME/Videos"
    \row \li MusicLocation
         \li "$HOME/Music"
    \row \li PicturesLocation
         \li "$HOME/Pictures"
    \row \li PublicLocation
         \li "$HOME/Public"
    \row \li Templates
         \li "$HOME/Templates"
    \row \li DataLocation
         \li "$HOME/.local/share"
    \row \li CacheLocation
         \li "$HOME/.cache"
    \row \li ConfigLocation
         \li "$HOME/.config", "/etc/xdg"
    \row \li RuntimeLocation
         \li "/run/user/<USERID>"
    \row \li StateLocation
         \li "$HOME/.local/state"
    \row \li ExecutableLocation
         \li "$HOME/.local/bin"
    \endtable

    The paths above should not be relied upon, as they may change according to
    user configuration and locale.

    \sa writableLocation(), standardLocations(), displayName(), locate(),
    locateAll()
    \link https://specifications.freedesktop.org/basedir-spec/latest
*/

/*!
    \fn QString QStandardPaths::writableLocation(StandardLocation type)

    Returns the directory where files of \a type should be written to, or an
    empty string if the location cannot be determined.

    \note The storage location returned can be a directory that does not exist,
    an attempt to create directories will be made. If directory exists its
    permissions are not changed.
*/

/*!
    \fn QStringList QStandardPaths::standardLocations(StandardLocation type)

    Returns all the directories where files of \a type belong.

    The list of directories is sorted from high to low priority as per the XDG
    spec.

    \sa writableLocation()
*/

/*!
    \enum QStandardPaths::LocateOption

    This enum describes the different flags that can be used for controlling
    the behavior of \c QStandardPaths::locate and \c QStandardPaths::locateAll.

    \value LocateFile Return only files
    \value LocateDirectory Return only directories
*/

static inline bool existsAsSpecified(const QString &path, QStandardPaths::LocateOption options)
{
    QStatInfo info(path);
    if (options == QStandardPaths::LocateDirectory) {
        return info.isDir();
    }
    return info.isFile();
}

/*!
    Tries to find a file or directory called \a fileName in the standard 
    ocations for \a type. If not found an empty string is returned.
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
    Tries to find all files or directories called \a fileName in the standard
    locations for \a type. If not found an empty list is returned.
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
    Returns a localized display name for the given location \a type.
*/
QString QStandardPaths::displayName(StandardLocation type)
{
    switch (type) {
        case DesktopLocation: {
            return QCoreApplication::translate("QStandardPaths", "Desktop");
        }
        case DocumentsLocation: {
            return QCoreApplication::translate("QStandardPaths", "Documents");
        }
        case DownloadsLocation: {
            return QCoreApplication::translate("QStandardPaths", "Download");
        }
        case VideosLocation: {
            return QCoreApplication::translate("QStandardPaths", "Videos");
        }
        case MusicLocation: {
            return QCoreApplication::translate("QStandardPaths", "Music");
        }
        case PicturesLocation: {
            return QCoreApplication::translate("QStandardPaths", "Pictures");
        }
        case PublicLocation: {
            return QCoreApplication::translate("QStandardPaths", "Public Share");
        }
        case TemplatesLocation: {
            return QCoreApplication::translate("QStandardPaths", "Templates");
        }
        case DataLocation: {
            return QCoreApplication::translate("QStandardPaths", "Application Data");
        }
        case CacheLocation: {
            return QCoreApplication::translate("QStandardPaths", "Cache");
        }
        case ConfigLocation: {
            return QCoreApplication::translate("QStandardPaths", "Configuration");
        }
        case RuntimeLocation: {
            return QCoreApplication::translate("QStandardPaths", "Runtime");
        }
        case StateLocation: {
            return QCoreApplication::translate("QStandardPaths", "State");
        }
        case ExecutableLocation: {
            return QCoreApplication::translate("QStandardPaths", "Executable");
        }
    }
    Q_UNREACHABLE();
}

/*!
    \fn QString QStandardPaths::findExecutable(const QString &executableName, const QStringList &paths)

    Finds the executable named \a executableName in the paths specified by
    \a paths, or the system paths if \a paths is empty. The system path is
    determined by the PATH environment variable.

    Symlinks are not resolved, in order to preserve behavior for the case of
    executables whose behavior depends on the name they are invoked with.

    Returns the absolute file path to the executable or an empty string if not
    found.
*/

/*!
    \fn QString QStandardPaths::openUrl(const QUrl &url)

    Opens the URL \a url with the associated application for it.

    Returns true upon success and false otherwise.
*/

QT_END_NAMESPACE
