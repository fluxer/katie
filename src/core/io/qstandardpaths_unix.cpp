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
#include "qfile.h"
#include "qhash.h"
#include "qtextstream.h"
#include "qfilesystemengine_p.h"
#include "qcoreapplication.h"

#include <errno.h>
#include <stdlib.h>


QT_BEGIN_NAMESPACE

static void appendOrganizationAndApp(QString &path)
{
    const QString org = QCoreApplication::organizationName();

    if (!org.isEmpty()) {
        path += QLatin1Char('/') + org;
    }
    const QString appName = QCoreApplication::applicationName();

    if (!appName.isEmpty()) {
        path += QLatin1Char('/') + appName;
    }
}

QString QStandardPaths::writableLocation(StandardLocation type)
{
    switch (type) {
        case DesktopLocation:
            return QDir::homePath() + QLatin1String("/Desktop");
        case DocumentsLocation:
            return QDir::homePath() + QLatin1String("/Documents");
        case PicturesLocation:
            return QDir::homePath() + QLatin1String("/Pictures");
        case FontsLocation:
            return QDir::homePath() + QLatin1String("/.fonts");
        case MusicLocation:
            return QDir::homePath() + QLatin1String("/Music");
        case MoviesLocation:
            return QDir::homePath() + QLatin1String("/Videos");
        case DownloadLocation:
            return QDir::homePath() + QLatin1String("/Downloads");
        case ApplicationsLocation:
            return writableLocation(GenericDataLocation) + QLatin1String("/applications");
        case HomeLocation:
            return QDir::homePath();
        case TempLocation:
            return QDir::tempPath();
        case CacheLocation:
        case GenericCacheLocation: {
            // http://standards.freedesktop.org/basedir-spec/basedir-spec-0.6.html
            static QString xdgCacheHome = QFile::decodeName(qgetenv("XDG_CACHE_HOME"));
            if (xdgCacheHome.isEmpty()) {
                xdgCacheHome = QDir::homePath() + QLatin1String("/.cache");
            }
            if (type == QStandardPaths::CacheLocation) {
                appendOrganizationAndApp(xdgCacheHome);
            }
            return xdgCacheHome;
        }
        case DataLocation:
        case GenericDataLocation: {
            static QString xdgDataHome = QFile::decodeName(qgetenv("XDG_DATA_HOME"));
            if (xdgDataHome.isEmpty()) {
                xdgDataHome = QDir::homePath() + QLatin1String("/.local/share");
            }
            if (type == QStandardPaths::DataLocation) {
                appendOrganizationAndApp(xdgDataHome);
            }
            return xdgDataHome;
        }
        case ConfigLocation:
        case GenericConfigLocation: {
            // http://standards.freedesktop.org/basedir-spec/latest/
            static QString xdgConfigHome = QFile::decodeName(qgetenv("XDG_CONFIG_HOME"));
            if (xdgConfigHome.isEmpty()) {
                xdgConfigHome = QDir::homePath() + QLatin1String("/.config");
            }
            return xdgConfigHome;
        }
        case RuntimeLocation: {
            const uid_t myUid = ::geteuid();
            // http://standards.freedesktop.org/basedir-spec/latest/
            static QString xdgRuntimeDir = QFile::decodeName(qgetenv("XDG_RUNTIME_DIR"));
            if (xdgRuntimeDir.isEmpty()) {
                const QString userName = QFileSystemEngine::resolveUserName(myUid);
                xdgRuntimeDir = QDir::tempPath() + QLatin1String("/runtime-") + userName;
                QDir dir(xdgRuntimeDir);
                if (!dir.exists() && !QDir().mkdir(xdgRuntimeDir)) {
                    qWarning("QStandardPaths: error creating runtime directory %s: %s", qPrintable(xdgRuntimeDir),
                            qPrintable(qt_error_string(errno)));
                    return QString();
                }
                qWarning("QStandardPaths: XDG_RUNTIME_DIR not set, defaulting to '%s'", qPrintable(xdgRuntimeDir));
            }
            // "The directory MUST be owned by the user"
            QFileInfo fileInfo(xdgRuntimeDir);
            if (fileInfo.ownerId() != myUid) {
                qWarning("QStandardPaths: wrong ownership on runtime directory %s, %d instead of %d", qPrintable(xdgRuntimeDir),
                        fileInfo.ownerId(), myUid);
                return QString();
            }
            // "and he MUST be the only one having read and write access to it. Its Unix access mode MUST be 0700."
            QFile file(xdgRuntimeDir);
            static const QFile::Permissions wantedPerms = QFile::ReadUser | QFile::WriteUser | QFile::ExeUser;
            if (file.permissions() != wantedPerms && !file.setPermissions(wantedPerms)) {
                qWarning("QStandardPaths: wrong permissions on runtime directory %s", qPrintable(xdgRuntimeDir));
                return QString();
            }
            return xdgRuntimeDir;
        }
        default:
            break;
    }

    // http://www.freedesktop.org/wiki/Software/xdg-user-dirs
    static QString xdgConfigHome = QFile::decodeName(qgetenv("XDG_CONFIG_HOME"));
    if (xdgConfigHome.isEmpty()) {
        xdgConfigHome = QDir::homePath() + QLatin1String("/.config");
    }
    QFile file(xdgConfigHome + QLatin1String("/user-dirs.dirs"));
    if (file.open(QIODevice::ReadOnly)) {
        QHash<QString, QString> lines;
        QTextStream stream(&file);
        // Only look for lines like: XDG_DESKTOP_DIR="$HOME/Desktop"
        QRegExp exp(QLatin1String("^XDG_(.*)_DIR=(.*)$"));
        while (!stream.atEnd()) {
            const QString &line = stream.readLine();
            if (exp.indexIn(line) != -1) {
                const QStringList lst = exp.capturedTexts();
                const QString key = lst.at(1);
                QString value = lst.at(2);
                if (value.length() > 2
                    && value.startsWith(QLatin1Char('\"'))
                    && value.endsWith(QLatin1Char('\"'))) {
                    value = value.mid(1, value.length() - 2);
                }
                // Store the key and value: "DESKTOP", "$HOME/Desktop"
                lines[key] = value;
            }
        }

        QString value;
        switch (type) {
            case DesktopLocation: {
                value = lines.value(QLatin1String("DESKTOP"));
                break;
            }
            case DocumentsLocation: {
                value = lines.value(QLatin1String("DOCUMENTS"));
                break;
            }
            case PicturesLocation: {
                value = lines.value(QLatin1String("PICTURES"));
                break;
            }
            case MusicLocation: {
                value = lines.value(QLatin1String("MUSIC"));
                break;
            }
            case MoviesLocation: {
                value = lines.value(QLatin1String("VIDEOS"));
                break;
            }
            case DownloadLocation: {
                value = lines.value(QLatin1String("DOWNLOAD"));
                break;
            }
            default:
                break;
        }
        if (!value.isEmpty()) {
            // value can start with $HOME
            if (value.startsWith(QLatin1String("$HOME"))) {
                value = QDir::homePath() + value.mid(5);
            }
            return value;
        }
    }

    return QString();
}

static QStringList xdgDataDirs()
{
    // http://standards.freedesktop.org/basedir-spec/latest/
    static const QString xdgDataDirsEnv = QFile::decodeName(qgetenv("XDG_DATA_DIRS"));
    if (xdgDataDirsEnv.isEmpty()) {
        QStringList dirs;
        dirs.append(QString::fromLatin1("/usr/local/share"));
        dirs.append(QString::fromLatin1("/usr/share"));
        return dirs;
    }
    return xdgDataDirsEnv.split(QLatin1Char(':'));
}

QStringList QStandardPaths::standardLocations(StandardLocation type)
{
    QStringList dirs;
    switch (type) {
        case ConfigLocation:
        case GenericConfigLocation: {
            // http://standards.freedesktop.org/basedir-spec/latest/
            static const QString xdgConfigDirs = QFile::decodeName(qgetenv("XDG_CONFIG_DIRS"));
            if (xdgConfigDirs.isEmpty()) {
                dirs.append(QString::fromLatin1("/etc/xdg"));
            } else {
                dirs = xdgConfigDirs.split(QLatin1Char(':'));
            }
            break;
        }
        case GenericDataLocation: {
            dirs = xdgDataDirs();
            break;
        }
        case ApplicationsLocation: {
            dirs = xdgDataDirs();
            for (int i = 0; i < dirs.count(); ++i) {
                dirs[i].append(QLatin1String("/applications"));
            }
            break;
        }
        case DataLocation: {
            dirs = xdgDataDirs();
            for (int i = 0; i < dirs.count(); ++i) {
                appendOrganizationAndApp(dirs[i]);
            }
            break;
        }
        default:
            break;
    }
    dirs.prepend(writableLocation(type));
    return dirs;
}

QT_END_NAMESPACE

