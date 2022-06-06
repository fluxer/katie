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
#include "qmap.h"
#include "qdir.h"
#include "qfile.h"
#include "qprocess.h"
#include "qcore_unix_p.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

// for reference:
// https://cgit.freedesktop.org/xdg/xdg-user-dirs/tree/xdg-user-dir-lookup.c
// https://specifications.freedesktop.org/basedir-spec/latest

typedef QMap<QByteArray,QString> XDGDirsMap;

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

static XDGDirsMap getUserDirs()
{
    XDGDirsMap result;

    QString xdgconfig;
    const QString xdgconfighome = getEnvName("XDG_CONFIG_HOME");
    if (!xdgconfighome.isEmpty()) {
        xdgconfig = xdgconfighome + QLatin1String("/user-dirs.dirs");
    } else {
        xdgconfig = QDir::homePath() + QLatin1String("/.config/user-dirs.dirs");
    }
    // qDebug() << Q_FUNC_INFO << xdgconfig;

    QFile xdgconfigfile(xdgconfig);
    if (!xdgconfigfile.open(QFile::ReadOnly)) {
        return result;
    }

    while (!xdgconfigfile.atEnd()) {
        const QByteArray xdgline(xdgconfigfile.readLine());
        const int equalsignindex = xdgline.indexOf('=');
        if (xdgline.startsWith("XDG_") && equalsignindex > 0) {
            QByteArray xdglocation(xdgline.mid(equalsignindex + 1, xdgline.size() - equalsignindex - 1));
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
            const QByteArray xdgdirvar = xdgline.left(equalsignindex);
            // qDebug() << Q_FUNC_INFO << xdgdirvar << xdglocation;
            result.insert(xdgdirvar, QFile::decodeName(xdglocation));
        }
    }

    return result;
}

QString QStandardPaths::writableLocation(StandardLocation type)
{
    foreach (const QString &location, standardLocations(type)) {
        const QStatInfo info(location);
        if (info.isDir()) {
            return location;
        } else {
            if (QDir().mkpath(location)) {
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
    static XDGDirsMap xdguserdirs = getUserDirs();
    switch (type) {
        case StandardLocation::DesktopLocation: {
            const QString location(xdguserdirs.value("XDG_DESKTOP_DIR"));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Desktop"));
            break;
        }
        case StandardLocation::DocumentsLocation: {
            const QString location(xdguserdirs.value("XDG_DOCUMENTS_DIR"));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Documents"));
            break;
        }
        case StandardLocation::DownloadsLocation: {
            const QString location(xdguserdirs.value("XDG_DOWNLOAD_DIR"));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Downloads"));
            break;
        }
        case StandardLocation::VideosLocation: {
            const QString location(xdguserdirs.value("XDG_VIDEOS_DIR"));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Video"));
            break;
        }
        case StandardLocation::MusicLocation: {
            const QString location(xdguserdirs.value("XDG_MUSIC_DIR"));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Music"));
            break;
        }
        case StandardLocation::PicturesLocation: {
            const QString location(xdguserdirs.value("XDG_PICTURES_DIR"));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Pictures"));
            break;
        }
        case StandardLocation::PublicLocation: {
            const QString location(xdguserdirs.value("XDG_PUBLICSHARE_DIR"));
            if (!location.isEmpty()) {
                result.append(location);
            }
            result.append(QDir::homePath() + QLatin1String("/Public"));
            break;
        }
        case StandardLocation::TemplatesLocation: {
            const QString location(xdguserdirs.value("XDG_TEMPLATES_DIR"));
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
