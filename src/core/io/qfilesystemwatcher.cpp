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

#include "qdatetime.h"
#include "qdebug.h"
#include "qdir.h"
#include "qfileinfo.h"
#include "qset.h"
#include "qtimer.h"
#include "qfilesystemwatcher_unix_p.h"

QT_BEGIN_NAMESPACE

enum { PollingInterval = 1000 };

class QPollingFileSystemWatcherEngine : public QFileSystemWatcherEngine
{
    Q_OBJECT

    class FileInfo
    {
        uint ownerId;
        uint groupId;
        QFile::Permissions permissions;
        QDateTime lastModified;
        QStringList entries;

    public:
        FileInfo(const QFileInfo &fileInfo)
            : ownerId(fileInfo.ownerId()),
              groupId(fileInfo.groupId()),
              permissions(fileInfo.permissions()),
              lastModified(fileInfo.lastModified())
        { 
            if (fileInfo.isDir()) {
                entries = fileInfo.absoluteDir().entryList(QDir::AllEntries);
            }
        }
        FileInfo &operator=(const QFileInfo &fileInfo)
        {
            *this = FileInfo(fileInfo);
            return *this;
        }

        bool operator!=(const QFileInfo &fileInfo) const
        {
            if (fileInfo.isDir() && entries != fileInfo.absoluteDir().entryList(QDir::AllEntries))
                return true;
            return (ownerId != fileInfo.ownerId()
                    || groupId != fileInfo.groupId()
                    || permissions != fileInfo.permissions()
                    || lastModified != fileInfo.lastModified());
        }
    };

    QHash<QString, FileInfo> files, directories;

public:
    QPollingFileSystemWatcherEngine();

    QStringList addPaths(const QStringList &paths, QStringList *files, QStringList *directories);
    QStringList removePaths(const QStringList &paths, QStringList *files, QStringList *directories);

private Q_SLOTS:
    void timeout();

private:
    QTimer timer;
};

QPollingFileSystemWatcherEngine::QPollingFileSystemWatcherEngine()
    : timer(this)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start(PollingInterval);
}

QStringList QPollingFileSystemWatcherEngine::addPaths(const QStringList &paths,
                                                      QStringList *files,
                                                      QStringList *directories)
{
    QStringList p = paths;
    foreach (const QString &path, p) {
        QFileInfo fi(path);
        if (!fi.exists())
            continue;
        if (fi.isDir()) {
            if (!directories->contains(path))
                directories->append(path);
            if (!path.endsWith(QLatin1Char('/')))
                fi = QFileInfo(path + QLatin1Char('/'));
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

QStringList QPollingFileSystemWatcherEngine::removePaths(const QStringList &paths,
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

void QPollingFileSystemWatcherEngine::timeout()
{
    QMutableHashIterator<QString, FileInfo> fit(files);
    while (fit.hasNext()) {
        QHash<QString, FileInfo>::iterator x = fit.next();
        QString path = x.key();
        QFileInfo fi(path);
        if (!fi.exists()) {
            fit.remove();
            emit fileChanged(path, true);
        } else if (x.value() != fi) {
            x.value() = fi;
            emit fileChanged(path, false);
        }
    }
    QMutableHashIterator<QString, FileInfo> dit(directories);
    while (dit.hasNext()) {
        QHash<QString, FileInfo>::iterator x = dit.next();
        QString path = x.key();
        QFileInfo fi(path);
        if (!path.endsWith(QLatin1Char('/')))
            fi = QFileInfo(path + QLatin1Char('/'));
        if (!fi.exists()) {
            dit.remove();
            emit directoryChanged(path, true);
        } else if (x.value() != fi) {
            fi.refresh();
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


void QFileSystemWatcherPrivate::initNativeEngine()
{
    Q_ASSERT(!watcher);
    Q_Q(const QFileSystemWatcher);

#if defined(QT_HAVE_INOTIFY_INIT1) || defined(QT_HAVE_KEVENT)
    watcher = QFileSystemWatcherEngineUnix::create();
#endif

    if (watcher) {
        QObject::connect(watcher,
                         SIGNAL(fileChanged(QString,bool)),
                         q,
                         SLOT(_q_fileChanged(QString,bool)));
        QObject::connect(watcher,
                         SIGNAL(directoryChanged(QString,bool)),
                         q,
                         SLOT(_q_directoryChanged(QString,bool)));
    }
}

QFileSystemWatcherPrivate::QFileSystemWatcherPrivate()
    : watcher(Q_NULLPTR)
{
}

void QFileSystemWatcherPrivate::init()
{
#ifdef QT_BUILD_INTERNAL
    Q_Q(const QFileSystemWatcher);

    const QObject *parent = q->parent();
    if (parent && parent->objectName().startsWith(QLatin1String("_qt_autotest_force_engine_"))) {
        // Autotest override case - use the explicitly selected engine only
        QString forceName = parent->objectName().mid(26);
        if (forceName == QLatin1String("poller")) {
            qDebug() << "QFileSystemWatcher: skipping native engine, using only polling engine";
            initPollerEngine();
        } else if (forceName == QLatin1String("native")) {
            qDebug() << "QFileSystemWatcher: skipping polling engine, using only native engine";
            initNativeEngine();
        } else {
            qDebug() << "QFileSystemWatcher: unknown engine, using native engine";
            initNativeEngine();
        }
    } else {
#endif // QT_BUILD_INTERNAL
        // Normal runtime case - search intelligently for best engine
        initNativeEngine();
        if (!watcher) {
            initPollerEngine();
        }
#ifdef QT_BUILD_INTERNAL
    }
#endif // QT_BUILD_INTERNAL
}

void QFileSystemWatcherPrivate::initPollerEngine()
{
    Q_ASSERT(!watcher);
    Q_Q(const QFileSystemWatcher);
    watcher = new QPollingFileSystemWatcherEngine; // that was a mouthful
    QObject::connect(watcher,
                     SIGNAL(fileChanged(QString,bool)),
                     q,
                     SLOT(_q_fileChanged(QString,bool)));
    QObject::connect(watcher,
                     SIGNAL(directoryChanged(QString,bool)),
                     q,
                     SLOT(_q_directoryChanged(QString,bool)));
}

void QFileSystemWatcherPrivate::_q_fileChanged(const QString &path, bool removed)
{
    Q_Q(QFileSystemWatcher);
    if (!files.contains(path)) {
        // the path was removed after a change was detected, but before we delivered the signal
        return;
    }
    if (removed)
        files.removeAll(path);
    emit q->fileChanged(path);
}

void QFileSystemWatcherPrivate::_q_directoryChanged(const QString &path, bool removed)
{
    Q_Q(QFileSystemWatcher);
    if (!directories.contains(path)) {
        // perhaps the path was removed after a change was detected, but before we delivered the signal
        return;
    }
    if (removed)
        directories.removeAll(path);
    emit q->directoryChanged(path);
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

    \note On systems running a Linux kernel without inotify support,
    file systems that contain watched paths cannot be unmounted.

    \note The act of monitoring files and directories for
    modifications consumes system resources. This implies there is a
    limit to the number of files and directories your process can
    monitor simultaneously. On all BSD variants, for example, an open
    file descriptor is required for each monitored file. Some system
    limits the number of open file descriptors to 256 by default.
    This means that addPath() and addPaths() will fail if your process
    tries to add more than 256 files or directories to the file system
    monitor. Also note that your process may have other file
    descriptors open in addition to the ones for files being monitored,
    and these other open descriptors also count in the total.


    \sa QFile, QDir
*/


/*!
    Constructs a new file system watcher object with the given \a parent.
*/
QFileSystemWatcher::QFileSystemWatcher(QObject *parent)
    : QObject(*new QFileSystemWatcherPrivate, parent)
{
    d_func()->init();
}

/*!
    Constructs a new file system watcher object with the given \a parent
    which monitors the specified \a paths list.
*/
QFileSystemWatcher::QFileSystemWatcher(const QStringList &paths, QObject *parent)
    : QObject(*new QFileSystemWatcherPrivate, parent)
{
    d_func()->init();
    addPaths(paths);
}

/*!
    Destroys the file system watcher.
*/
QFileSystemWatcher::~QFileSystemWatcher()
{
    Q_D(QFileSystemWatcher);
    if (d->watcher) {
        delete d->watcher;
        d->watcher = Q_NULLPTR;
    }
}

/*!
    Adds \a path to the file system watcher if \a path exists. The
    path is not added if it does not exist, or if it is already being
    monitored by the file system watcher.

    If \a path specifies a directory, the directoryChanged() signal
    will be emitted when \a path is modified or removed from disk;
    otherwise the fileChanged() signal is emitted when \a path is
    modified, renamed or removed.

    \note There is a system dependent limit to the number of files and
    directories that can be monitored simultaneously. If this limit
    has been reached, \a path will not be added to the file system
    watcher, and a warning message will be printed to \e{stderr}.

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
    not added if they not exist, or if they are already being
    monitored by the file system watcher.

    If a path specifies a directory, the directoryChanged() signal
    will be emitted when the path is modified or removed from disk;
    otherwise the fileChanged() signal is emitted when the path is
    modified, renamed, or removed.

    \note There is a system dependent limit to the number of files and
    directories that can be monitored simultaneously. If this limit
    has been reached, the excess \a paths will not be added to the
    file system watcher, and a warning message will be printed to
    \e{stderr} for each path that could not be added.

    \sa addPath(), removePaths()
*/
void QFileSystemWatcher::addPaths(const QStringList &paths)
{
    Q_D(QFileSystemWatcher);
    if (Q_UNLIKELY(paths.isEmpty())) {
        qWarning("QFileSystemWatcher::addPaths: list is empty");
        return;
    }

    QStringList p = paths;
    if (Q_LIKELY(d->watcher))
        p = d->watcher->addPaths(paths, &d->files, &d->directories);

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
    if (Q_LIKELY(d->watcher))
        d->watcher->removePaths(paths, &d->files, &d->directories);
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

/*!
    \fn QStringList QFileSystemWatcher::directories() const

    Returns a list of paths to directories that are being watched.

    \sa files()
*/

/*!
    \fn QStringList QFileSystemWatcher::files() const

    Returns a list of paths to files that are being watched.

    \sa directories()
*/

QStringList QFileSystemWatcher::directories() const
{
    Q_D(const QFileSystemWatcher);
    return d->directories;
}

QStringList QFileSystemWatcher::files() const
{
    Q_D(const QFileSystemWatcher);
    return d->files;
}

QT_END_NAMESPACE

#include "moc_qfilesystemwatcher.cpp"
#include "moc_qfilesystemwatcher.h"
#include "moc_qfilesystemwatcher_p.h"

#endif // QT_NO_FILESYSTEMWATCHER

