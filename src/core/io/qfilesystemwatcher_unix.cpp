/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qfilesystemwatcher.h"
#include "qfilesystemwatcher_unix_p.h"

#if !defined(QT_NO_FILESYSTEMWATCHER) && (defined(QT_HAVE_INOTIFY_INIT1) || defined(QT_HAVE_KEVENT))

#include "qcore_unix_p.h"
#include "qdebug.h"
#include "qfile.h"
#include "qfileinfo.h"
#include "qsocketnotifier.h"
#include "qvarlengtharray.h"

#if defined(QT_HAVE_INOTIFY_INIT1)
#  include <sys/syscall.h>
#  include <sys/ioctl.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <sys/inotify.h>
#elif defined(QT_HAVE_KEVENT)
#  include <sys/types.h>
#  include <sys/event.h>
#  include <sys/stat.h>
#  include <sys/time.h>
#  include <fcntl.h>
#endif

QT_BEGIN_NAMESPACE

QFileSystemWatcherEngineUnix *QFileSystemWatcherEngineUnix::create()
{
#if defined(QT_HAVE_INOTIFY_INIT1)
    int fd = inotify_init1(IN_CLOEXEC);
#elif defined(QT_HAVE_KEVENT)
    int fd = kqueue();
#endif
    if (Q_UNLIKELY(fd == -1)) {
        return Q_NULLPTR;
    }
    return new QFileSystemWatcherEngineUnix(fd);
}

QFileSystemWatcherEngineUnix::QFileSystemWatcherEngineUnix(int fd)
    : sockfd(fd)
    , notifier(fd, QSocketNotifier::Read, this)
{
    connect(&notifier, SIGNAL(activated(int)), SLOT(readFromFd()));

#if defined(QT_HAVE_KEVENT)
    ::fcntl(sockfd, F_SETFD, FD_CLOEXEC);
#endif
}

QFileSystemWatcherEngineUnix::~QFileSystemWatcherEngineUnix()
{
    notifier.setEnabled(false);
#if defined(QT_HAVE_INOTIFY_INIT1)
    foreach (int id, pathToID)
        inotify_rm_watch(sockfd, id < 0 ? -id : id);
    ::close(sockfd);
#elif defined(QT_HAVE_KEVENT)
    foreach (int id, pathToID)
        ::close(id < 0 ? -id : id);
#endif
}

QStringList QFileSystemWatcherEngineUnix::addPaths(const QStringList &paths,
                                                      QStringList *files,
                                                      QStringList *directories)
{
    QStringList p = paths;

#if defined(QT_HAVE_INOTIFY_INIT1)
    foreach (QString path, p) {
        QFileInfo fi(path);
        bool isDir = fi.isDir();
        if (isDir && directories->contains(path)) {
            continue;
        } else if (files->contains(path)) {
            continue;
        }

        int wd = ::inotify_add_watch(sockfd,
                                   QFile::encodeName(path).constData(),
                                   (isDir
                                    ? (0
                                       | IN_ATTRIB
                                       | IN_MOVE
                                       | IN_CREATE
                                       | IN_DELETE
                                       | IN_DELETE_SELF
                                       )
                                    : (0
                                       | IN_ATTRIB
                                       | IN_MODIFY
                                       | IN_MOVE
                                       | IN_MOVE_SELF
                                       | IN_DELETE_SELF
                                       )));
        if (wd < 0) {
            perror("QFileSystemWatcherEngineUnix::addPaths: inotify_add_watch failed");
            continue;
        }

        p.removeAll(path);

        int id = isDir ? -wd : wd;
        if (id < 0) {
            directories->append(path);
        } else {
            files->append(path);
        }

        pathToID.insert(path, id);
        idToPath.insert(id, path);
    }
#elif defined(QT_HAVE_KEVENT)
    foreach (QString path, p) {
#if defined(O_EVTONLY)
        int fd = qt_safe_open(QFile::encodeName(path), O_EVTONLY);
#else
        int fd = qt_safe_open(QFile::encodeName(path), O_RDONLY);
#endif
        if (fd == -1) {
            perror("QKqueueFileSystemWatcherEngine::addPaths: open");
            continue;
        }
        if (fd >= (int)FD_SETSIZE / 2 && fd < (int)FD_SETSIZE) {
            int fddup = ::fcntl(fd, F_DUPFD, FD_SETSIZE);
            if (fddup != -1) {
                ::close(fd);
                fd = fddup;
            }
        }
        ::fcntl(fd, F_SETFD, FD_CLOEXEC);

        QT_STATBUF st;
        if (QT_FSTAT(fd, &st) == -1) {
            perror("QKqueueFileSystemWatcherEngine::addPaths: fstat");
            ::close(fd);
            continue;
        }
        int id = (S_ISDIR(st.st_mode)) ? -fd : fd;
        if (id < 0) {
            if (directories->contains(path)) {
                ::close(fd);
                continue;
            }
        } else {
            if (files->contains(path)) {
                ::close(fd);
                continue;
            }
        }

        struct kevent kev;
        EV_SET(&kev,
               fd,
               EVFILT_VNODE,
               EV_ADD | EV_ENABLE | EV_CLEAR,
               NOTE_DELETE | NOTE_WRITE | NOTE_EXTEND | NOTE_ATTRIB | NOTE_RENAME | NOTE_REVOKE,
               0,
               0);
        if (kevent(sockfd, &kev, 1, 0, 0, 0) == -1) {
            perror("QKqueueFileSystemWatcherEngine::addPaths: kevent");
            ::close(fd);
            continue;
        }

        p.removeAll(path);
        if (id < 0) {
            // qDebug() << "QKqueueFileSystemWatcherEngine: added directory path" << path;
            directories->append(path);
        } else {
            // qDebug() << "QKqueueFileSystemWatcherEngine: added file path" << path;
            files->append(path);
        }

        pathToID.insert(path, id);
        idToPath.insert(id, path);
    }
#endif

    return p;
}

QStringList QFileSystemWatcherEngineUnix::removePaths(const QStringList &paths,
                                                         QStringList *files,
                                                         QStringList *directories)
{
    QStringList p = paths;

#if defined(QT_HAVE_INOTIFY_INIT1)
    foreach (QString path, p) {
        int id = pathToID.take(path);
        QString x = idToPath.take(id);
        if (x.isEmpty() || x != path)
            continue;

        int wd = id < 0 ? -id : id;
        // qDebug() << "removing watch for path" << path << "wd" << wd;
        ::inotify_rm_watch(sockfd, wd);

        p.removeAll(path);
        if (id < 0) {
            directories->removeAll(path);
        } else {
            files->removeAll(path);
        }
    }
#elif defined(QT_HAVE_KEVENT)
    QMutexLocker locker(&mutex);
    if (pathToID.isEmpty())
        return p;

    foreach (QString path, p) {
        int id = pathToID.take(path);
        QString x = idToPath.take(id);
        if (x.isEmpty() || x != path)
            continue;

        ::close(id < 0 ? -id : id);

        p.removeAll(path);
        if (id < 0)
            directories->removeAll(path);
        else
            files->removeAll(path);
    }
#endif

    return p;
}

void QFileSystemWatcherEngineUnix::readFromFd()
{
#if defined(QT_HAVE_INOTIFY_INIT1)
    // qDebug() << "QFileSystemWatcherEngineUnix::readFromFd";

    int buffSize = 0;
    ::ioctl(sockfd, FIONREAD, (char *) &buffSize);
    char readbuff[buffSize];
    buffSize = ::read(sockfd, readbuff, buffSize);
    char *at = readbuff;
    char * const end = at + buffSize;

    while (at < end) {
        const inotify_event *event = reinterpret_cast<const inotify_event *>(at);
        at += sizeof(inotify_event) + event->len;

        // qDebug() << "inotify event, wd" << event->wd << "mask" << hex << event->mask;

        int id = event->wd;
        QString path = idToPath.value(id);
        if (path.isEmpty()) {
            // perhaps a directory?
            id = -id;
            path = idToPath.value(id);
            if (path.isEmpty())
                continue;
        }

        // qDebug() << "event for path" << path;

        if ((event->mask & (IN_DELETE_SELF | IN_MOVE_SELF | IN_UNMOUNT)) != 0) {
            pathToID.remove(path);
            idToPath.remove(id);
            inotify_rm_watch(sockfd, event->wd);

            if (id < 0)
                emit directoryChanged(path, true);
            else
                emit fileChanged(path, true);
        } else {
            if (id < 0)
                emit directoryChanged(path, false);
            else
                emit fileChanged(path, false);
        }
    }
#elif defined(QT_HAVE_KEVENT)
    forever {
        // qDebug() << "QKqueueFileSystemWatcherEngine: polling for changes";
        int r;
        struct kevent kev;
        struct timespec ts = { 0, 0 }; // 0 ts, because we want to poll
        EINTR_LOOP(r, kevent(sockfd, 0, 0, &kev, 1, &ts));
        if (r < 0) {
            perror("QKqueueFileSystemWatcherEngine: error during kevent wait");
            return;
        } else if (r == 0) {
            // polling returned no events, so stop
            break;
        } else {
            int fd = kev.ident;

            // qDebug() << "QKqueueFileSystemWatcherEngine: processing kevent" << kev.ident << kev.filter;
            QMutexLocker locker(&mutex);

            int id = fd;
            QString path = idToPath.value(id);
            if (path.isEmpty()) {
                // perhaps a directory?
                id = -id;
                path = idToPath.value(id);
                if (path.isEmpty()) {
                    // qDebug() << "QKqueueFileSystemWatcherEngine: received a kevent for a file we're not watching";
                    continue;
                }
            }
            if (kev.filter != EVFILT_VNODE) {
                // qDebug() << "QKqueueFileSystemWatcherEngine: received a kevent with the wrong filter";
                continue;
            }

            if ((kev.fflags & (NOTE_DELETE | NOTE_REVOKE | NOTE_RENAME)) != 0) {
                // qDebug() << path << "removed, removing watch also";

                pathToID.remove(path);
                idToPath.remove(id);
                ::close(fd);

                if (id < 0)
                    emit directoryChanged(path, true);
                else
                    emit fileChanged(path, true);
            } else {
                // qDebug() << path << "changed";

                if (id < 0)
                    emit directoryChanged(path, false);
                else
                    emit fileChanged(path, false);
            }
        }

    }
#endif
}

#include "moc_qfilesystemwatcher_unix_p.h"

QT_END_NAMESPACE

#endif // QT_NO_FILESYSTEMWATCHER
