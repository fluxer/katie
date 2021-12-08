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
#include "qfile.h"
#include "qprocess.h"
#include "qcore_unix_p.h"

QT_BEGIN_NAMESPACE

// for reference:
// https://cgit.freedesktop.org/xdg/xdg-user-dirs/tree/xdg-user-dir-lookup.c
// https://specifications.freedesktop.org/basedir-spec/latest

static inline QString getEnvName(const char* const name)
{
    return QFile::decodeName(qgetenv(name));
}

static QStringList getEnvListName(const char* const name)
{
    QStringList result;
    const QByteArray location(qgetenv(name));
    foreach (const QByteArray &path, location.split(':')) {
        if (path.isEmpty()) {
            continue;
        }
        result.append(QFile::decodeName(path));
    }
    return result;
}

static QString getUserDirName(const char* const name, const int namesize)
{
    QString xdgconfig;
    const QByteArray xdgconfighome(qgetenv("XDG_CONFIG_HOME"));
    if (!xdgconfighome.isEmpty()) {
        xdgconfig = QFile::decodeName(xdgconfighome) + QLatin1String("/user-dirs.dirs");
    } else {
        xdgconfig = QDir::homePath() + QLatin1String("/.config/user-dirs.dirs");
    }

    QFile xdgconfigfile(xdgconfig);
    if (!xdgconfigfile.open(QFile::ReadOnly)) {
        return QString();
    }

    while (!xdgconfigfile.atEnd()) {
        const QByteArray xdgline(xdgconfigfile.readLine());
        if (xdgline.startsWith(name)) {
            QByteArray xdglocation(xdgline.mid(namesize, xdgline.size() - namesize));
            xdglocation = xdglocation.trimmed();
            if (xdglocation.contains("$HOME")) {
                xdglocation.replace("$HOME", QFile::encodeName(QDir::homePath()));
            }
            if (xdglocation.startsWith('"')) {
                xdglocation = xdglocation.mid(1, xdglocation.size() - 1);
            }
            if (xdglocation.endsWith('"')) {
                xdglocation.chop(1);
            }
            return QFile::decodeName(xdglocation);
        }
    }

    return QString();
}

static inline QString checkExecutable(const QString &path)
{
    const QStatInfo info(path);
    if (info.isFile() && info.isExecutable()) {
        return QDir::cleanPath(path);
    }
    return QString();
}

static QString searchExecutable(const QStringList &searchPaths, const QString &executableName)
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

QString QStandardPaths::writableLocation(StandardLocation type)
{
    foreach (const QString &location, standardLocations(type)) {
        const QStatInfo info(location);
        if (info.isDir()) {
            return location;
        } else {
            QDir locationdir(location);
            const QString locationdirname(locationdir.dirName());
            if (locationdir.cdUp() && locationdir.mkdir(locationdirname)) {
                QFile locationfile(location);
                if (locationfile.setPermissions(QFile::ReadUser | QFile::WriteUser | QFile::ExeUser)) {
                    return location;
                }
            }
        }
    }
    return QString();
}

QStringList QStandardPaths::standardLocations(StandardLocation type)
{
    QStringList result;
    switch (type) {
        case StandardLocation::DesktopLocation: {
            const QString location(getUserDirName("XDG_DESKTOP_DIR=", 16));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Desktop"));
            break;
        }
        case StandardLocation::DocumentsLocation: {
            const QString location(getUserDirName("XDG_DOCUMENTS_DIR=", 18));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Documents"));
            break;
        }
        case StandardLocation::DownloadsLocation: {
            const QString location(getUserDirName("XDG_DOWNLOAD_DIR=", 17));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Downloads"));
            break;
        }
        case StandardLocation::VideosLocation: {
            const QString location(getUserDirName("XDG_VIDEOS_DIR=", 15));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Video"));
            break;
        }
        case StandardLocation::MusicLocation: {
            const QString location(getUserDirName("XDG_MUSIC_DIR=", 14));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Music"));
            break;
        }
        case StandardLocation::PicturesLocation: {
            const QString location(getUserDirName("XDG_PICTURES_DIR=", 17));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Pictures"));
            break;
        }
        case StandardLocation::PublicLocation: {
            const QString location(getUserDirName("XDG_PUBLICSHARE_DIR=", 20));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Public"));
            break;
        }
        case StandardLocation::TemplatesLocation: {
            const QString location(getUserDirName("XDG_TEMPLATES_DIR=", 18));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Templates"));
            break;
        }

        case StandardLocation::DataLocation: {
            const QString location(getEnvName("XDG_DATA_HOME"));
            if (!location.isEmpty()) {
                result.append(location);
            } else {
                result.append(QDir::homePath() + QLatin1String("/.local/share"));
            }

            const QStringList locations(getEnvListName("XDG_DATA_DIRS"));
            if (!locations.isEmpty()) {
                result.append(locations);
            } else {
                result.append(QLatin1String("/usr/local/share/"));
                result.append(QLatin1String("/usr/share/"));
            }
            break;
        }
        case StandardLocation::CacheLocation: {
            const QString location(getEnvName("XDG_CACHE_HOME"));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/.cache"));
            break;
        }
        case StandardLocation::ConfigLocation: {
            const QString location(getEnvName("XDG_CONFIG_HOME"));
            if (!location.isEmpty()) {
                result.append(location);
            } else {
                result.append(QDir::homePath() + QLatin1String("/.config"));
            }

            const QStringList locations(getEnvListName("XDG_CONFIG_DIRS"));
            if (!locations.isEmpty()) {
                result.append(locations);
            } else {
                result.append(QDir::homePath() + QLatin1String("/etc/xdg"));
            }
            break;
        }
        case StandardLocation::RuntimeLocation: {
            const QString location(getEnvName("XDG_RUNTIME_DIR"));
            if (Q_UNLIKELY(!location.isEmpty())) {
                result.append(location);
            } else {
                qWarning("QStandardPaths: runtime directory is not set, using fallback");
                result.append(QDir::tempPath());
            }
            break;
        }
        case StandardLocation::StateLocation: {
            const QString location(getEnvName("XDG_STATE_HOME"));
            if (!location.isEmpty()) {
                result.append(location);
            } else {
                result.append(QDir::homePath() + QLatin1String("/.local/state"));
            }
            break;
        }
        case StandardLocation::ExecutableLocation: {
            result.append(QDir::homePath() + QLatin1String("/.local/bin"));
            break;
        }
    }
    return result;
}

QString QStandardPaths::findExecutable(const QString &executableName, const QStringList &paths)
{
    const QStatInfo info(executableName);
    if (info.isFile() && info.isExecutable()) {
        return QDir::cleanPath(executableName);
    }

    if (paths.isEmpty()) {
        const QStringList envPaths = QFile::decodeName(qgetenv("PATH")).split(QLatin1Char(':'), QString::SkipEmptyParts);
        return searchExecutable(envPaths, executableName);
    }

    return searchExecutable(paths, executableName);
}

bool QStandardPaths::openUrl(const QUrl &url)
{
    if (!url.isValid()) {
        return false;
    }

    const QString command = QLatin1String("xdg-open ") + url.toEncoded();
    return QProcess::startDetached(command);
}

QT_END_NAMESPACE
