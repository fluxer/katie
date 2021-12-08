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

#include "qfilesystemwatcher_unix_p.h"

#if !defined(QT_NO_FILESYSTEMWATCHER)

#include "qdebug.h"
#include "qfile.h"

QT_BEGIN_NAMESPACE

enum { PollingInterval = 1000 };

QFileSystemWatcherEngineUnix::QFileSystemWatcherEngineUnix()
    : timer(this)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start(PollingInterval);
}

QStringList QFileSystemWatcherEngineUnix::addPaths(const QStringList &paths,
                                                      QStringList *files,
                                                      QStringList *directories)
{
    QStringList p = paths;
    foreach (const QString &path, p) {
        QStatInfo fi(path, true);
        if (fi.isDir() || path.endsWith(QLatin1Char('/'))) {
            if (!directories->contains(path))
                directories->append(path);
            if (!path.endsWith(QLatin1Char('/')))
                fi = QStatInfo(path + QLatin1Char('/'), true);
            this->directories.insert(path, fi);
        } else {
            if (!files->contains(path))
                files->append(path);
            this->files.insert(path, fi);
        }
        p.removeAll(path);
    }
    if ((!this->files.isEmpty() ||
         !this->directories.isEmpty()) &&
        !timer.isActive()) {
        timer.start(PollingInterval);
    }
    return p;
}

QStringList QFileSystemWatcherEngineUnix::removePaths(const QStringList &paths,
                                                         QStringList *files,
                                                         QStringList *directories)
{
    QStringList p = paths;
    foreach (const QString &path, p) {
        if (this->directories.remove(path)) {
            directories->removeAll(path);
            p.removeAll(path);
        } else if (this->files.remove(path)) {
            files->removeAll(path);
            p.removeAll(path);
        }
    }
    if (this->files.isEmpty() &&
        this->directories.isEmpty()) {
        timer.stop();
    }
    return p;
}

void QFileSystemWatcherEngineUnix::timeout()
{
    QMutableHashIterator<QString, QStatInfo> fit(files);
    while (fit.hasNext()) {
        QHash<QString, QStatInfo>::iterator x = fit.next();
        QString path = x.key();
        QStatInfo fi(path);
        if (x.value() != fi) {
            if (!fi.exists()) {
                fit.remove();
                emit fileChanged(path, true);
            } else {
                x.value() = fi;
                emit fileChanged(path, false);
            }
        }
    }
    QMutableHashIterator<QString, QStatInfo> dit(directories);
    while (dit.hasNext()) {
        QHash<QString, QStatInfo>::iterator x = dit.next();
        QString path = x.key();
        QStatInfo fi(path, true);
        if (!path.endsWith(QLatin1Char('/')))
            fi = QStatInfo(path + QLatin1Char('/'), true);
        if (!fi.dirEquals(x.value())) {
            if (!fi.exists()) {
                dit.remove();
                emit directoryChanged(path, true);
            } else {
                x.value() = fi;
                emit directoryChanged(path, false);
            }
        }
    }
}

#include "moc_qfilesystemwatcher_unix_p.h"

QT_END_NAMESPACE

#endif // QT_NO_FILESYSTEMWATCHER
