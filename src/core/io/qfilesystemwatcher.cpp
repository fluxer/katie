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

#include "qfilesystemwatcher.h"
#include "qfilesystemwatcher_p.h"

#ifndef QT_NO_FILESYSTEMWATCHER

#include "qdebug.h"

QT_BEGIN_NAMESPACE


enum { PollingInterval = 1000 };

QFileSystemWatcherPrivate::QFileSystemWatcherPrivate()
    : timer(q_ptr)
{
}

QStringList QFileSystemWatcherPrivate::addPaths(const QStringList &paths)
{
    QStringList p = paths;
    foreach (const QString &path, paths) {
        QStatInfo fi(path, true);
        if (fi.isDir() || path.endsWith(QLatin1Char('/'))) {
            if (!path.endsWith(QLatin1Char('/')))
                fi = QStatInfo(path + QLatin1Char('/'), true);
            directories.insert(path, fi);
        } else {
            files.insert(path, fi);
        }
        p.removeAll(path);
    }
    if ((!files.isEmpty() || !directories.isEmpty()) && !timer.isActive()) {
        timer.start(PollingInterval);
    }
    return p;
}

QStringList QFileSystemWatcherPrivate::removePaths(const QStringList &paths)
{
    QStringList p = paths;
    foreach (const QString &path, paths) {
        if (directories.remove(path)) {
            p.removeAll(path);
        } else if (files.remove(path)) {
            p.removeAll(path);
        }
    }
    if (files.isEmpty() && directories.isEmpty()) {
        timer.stop();
    }
    return p;
}

void QFileSystemWatcherPrivate::_q_timeout()
{
    Q_Q(QFileSystemWatcher);

    QMutableHashIterator<QString, QStatInfo> fit(files);
    while (fit.hasNext()) {
        QHash<QString, QStatInfo>::iterator x = fit.next();
        QString path = x.key();
        QStatInfo fi(path);
        if (x.value() != fi) {
            if (!fi.exists()) {
                fit.remove();
            } else {
                x.value() = fi;
            }
            emit q->fileChanged(path);
        }
    }
    QMutableHashIterator<QString, QStatInfo> dit(directories);
    while (dit.hasNext()) {
        QHash<QString, QStatInfo>::iterator x = dit.next();
        QString path = x.key();
        QStatInfo fi;
        if (!path.endsWith(QLatin1Char('/'))) {
            fi = QStatInfo(path + QLatin1Char('/'), true);
        } else {
            fi = QStatInfo(path, true);
        }
        if (!fi.dirEquals(x.value())) {
            if (!fi.exists()) {
                dit.remove();
            } else {
                x.value() = fi;
            }
            emit q->directoryChanged(path);
        }
    }
}

/*!
    \class QFileSystemWatcher
    \brief The QFileSystemWatcher class provides an interface for monitoring files and directories for modifications.
    \ingroup io
    \since 4.2
    \reentrant

    QFileSystemWatcher monitors the file system for changes to files
    and directories by watching a list of specified paths.

    Call addPath() to watch a particular file or directory. Multiple
    paths can be added using the addPaths() function. Existing paths can
    be removed by using the removePath() and removePaths() functions.

    QFileSystemWatcher examines each path added to it. Files that have
    been added to the QFileSystemWatcher can be accessed using the
    files() function, and directories using the directories() function.

    The fileChanged() signal is emitted when a file has been modified,
    renamed or removed from disk. Similarly, the directoryChanged()
    signal is emitted when a directory or its contents is modified or
    removed.  Note that QFileSystemWatcher stops monitoring files once
    they have been renamed or removed from disk, and directories once
    they have been removed from disk.

    \sa QFile, QDir
*/


/*!
    Constructs a new file system watcher object with the given \a parent.
*/
QFileSystemWatcher::QFileSystemWatcher(QObject *parent)
    : QObject(*new QFileSystemWatcherPrivate, parent)
{
    Q_D(QFileSystemWatcher);
    connect(&d->timer, SIGNAL(timeout()), this, SLOT(_q_timeout()));
}

/*!
    Constructs a new file system watcher object with the given \a parent
    which monitors the specified \a paths list.
*/
QFileSystemWatcher::QFileSystemWatcher(const QStringList &paths, QObject *parent)
    : QObject(*new QFileSystemWatcherPrivate, parent)
{
    Q_D(QFileSystemWatcher);
    addPaths(paths);
    connect(&d->timer, SIGNAL(timeout()), this, SLOT(_q_timeout()));
}

/*!
    Destroys the file system watcher.
*/
QFileSystemWatcher::~QFileSystemWatcher()
{
    Q_D(QFileSystemWatcher);
    d->timer.stop();
}

/*!
    Adds \a path to the file system watcher. The path is not added
    if it is already being monitored by the file system watcher.

    If \a path specifies a directory, the directoryChanged() signal
    will be emitted when \a path is modified or removed from disk;
    otherwise the fileChanged() signal is emitted when \a path is
    modified, renamed or removed.

    \note To watch for the creation of non-existing directories you
    must ensure that the directory path to watch for ends with
    trainling slash, otherwise it cannot be distinguished from file
    path.

    \sa addPaths(), removePath()
*/
void QFileSystemWatcher::addPath(const QString &path)
{
    if (Q_UNLIKELY(path.isEmpty())) {
        qWarning("QFileSystemWatcher::addPath: path is empty");
        return;
    }
    addPaths(QStringList(path));
}

/*!
    Adds each path in \a paths to the file system watcher. Paths are
    not added if they are already being monitored by the file system
    watcher.

    If a path specifies a directory, the directoryChanged() signal
    will be emitted when the path is modified or removed from disk;
    otherwise the fileChanged() signal is emitted when the path is
    modified, renamed, or removed.

    \sa addPath(), removePaths()
*/
void QFileSystemWatcher::addPaths(const QStringList &paths)
{
    Q_D(QFileSystemWatcher);
    if (Q_UNLIKELY(paths.isEmpty())) {
        qWarning("QFileSystemWatcher::addPaths: list is empty");
        return;
    }

    QStringList p = d->addPaths(paths);
    if (Q_UNLIKELY(!p.isEmpty())) {
        qWarning("QFileSystemWatcher: failed to add paths: %s",
                 qPrintable(p.join(QLatin1String(", "))));
    }
}

/*!
    Removes the specified \a path from the file system watcher.

    \sa removePaths(), addPath()
*/
void QFileSystemWatcher::removePath(const QString &path)
{
    if (Q_UNLIKELY(path.isEmpty())) {
        qWarning("QFileSystemWatcher::removePath: path is empty");
        return;
    }
    removePaths(QStringList(path));
}

/*!
    Removes the specified \a paths from the file system watcher.

    \sa removePath(), addPaths()
*/
void QFileSystemWatcher::removePaths(const QStringList &paths)
{
    if (Q_UNLIKELY(paths.isEmpty())) {
        qWarning("QFileSystemWatcher::removePaths: list is empty");
        return;
    }
    Q_D(QFileSystemWatcher);
    d->removePaths(paths);
}

/*!
    Returns a list of paths to directories that are being watched.

    \sa files()
*/
QStringList QFileSystemWatcher::directories() const
{
    Q_D(const QFileSystemWatcher);
    return d->directories.keys();
}

/*!
    Returns a list of paths to files that are being watched.

    \sa directories()
*/
QStringList QFileSystemWatcher::files() const
{
    Q_D(const QFileSystemWatcher);
    return d->files.keys();
}

/*!
    \fn void QFileSystemWatcher::fileChanged(const QString &path)

    This signal is emitted when the file at the specified \a path is
    modified, renamed or removed from disk.

    \sa directoryChanged()
*/

/*!
    \fn void QFileSystemWatcher::directoryChanged(const QString &path)

    This signal is emitted when the directory at a specified \a path,
    is modified (e.g., when a file is added, modified or deleted) or
    removed from disk. Note that if there are several changes during a
    short period of time, some of the changes might not emit this
    signal. However, the last change in the sequence of changes will
    always generate this signal.

    \sa fileChanged()
*/

QT_END_NAMESPACE

#include "moc_qfilesystemwatcher.h"

#endif // QT_NO_FILESYSTEMWATCHER

