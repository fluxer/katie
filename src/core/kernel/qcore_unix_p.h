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

#ifndef QCORE_UNIX_P_H
#define QCORE_UNIX_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Qt code on Unix. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qplatformdefs.h"
#include "qatomic.h"
#include "qlist.h"
#include "qstring.h"
#include "qdatetime.h"

#include <poll.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define Q_EINTR_LOOP(var, cmd)                                \
    do {                                                      \
        var = cmd;                                            \
    } while (var == -1 && errno == EINTR)

// BSD and musl libc implementations do not reset errno and there is no
// reliable way to check if some functions (e.g. strtoll()) errored or returned
// a valid value if they do not reset errno
#ifdef __GLIBC__
#  define Q_RESET_ERRNO
#else
#  define Q_RESET_ERRNO errno = 0;
#endif


QT_BEGIN_NAMESPACE

class Q_CORE_EXPORT QStatInfo {
public:
    QStatInfo(const QString &path, const bool listdir = false);
    QStatInfo(const QStatInfo &other);

    QStatInfo& operator=(const QStatInfo &other);
    bool operator==(const QStatInfo &other) const;
    inline bool operator!=(const QStatInfo &other) const
        { return !operator==(other); }

    inline off_t size() const
        { return m_size; }
    inline uid_t ownerId() const
        { return m_uid; }
    inline gid_t groupId() const
        { return m_gid; }
    inline QDateTime lastModified() const
        { return QDateTime::fromTime_t(m_mtime); }
    inline bool exists() const
        { return (m_mode != 0); }
    inline bool isFile() const
        { return S_ISREG(m_mode); }
    inline bool isDir() const
        { return S_ISDIR(m_mode); }
    bool isReadable() const;
    bool isWritable() const;
    bool isExecutable() const;

    bool dirEquals(const QStatInfo &other) const;
    static QList<QStatInfo> dirInfos(const QByteArray &nativepath, const QString &localpath);

private:
    mode_t m_mode;
    uid_t m_uid;
    gid_t m_gid;
    time_t m_mtime;
    off_t m_size;
    QList<QStatInfo> m_entries;
    QByteArray m_path;
};

// Internal operator functions for timevals
inline timeval &normalizedTimeval(timeval &t)
{
    while (t.tv_usec > 1000000l) {
        ++t.tv_sec;
        t.tv_usec -= 1000000l;
    }
    while (t.tv_usec < 0l) {
        --t.tv_sec;
        t.tv_usec += 1000000l;
    }
    return t;
}
inline bool operator<(const timeval &t1, const timeval &t2)
{ return t1.tv_sec < t2.tv_sec || (t1.tv_sec == t2.tv_sec && t1.tv_usec < t2.tv_usec); }
inline bool operator==(const timeval &t1, const timeval &t2)
{ return t1.tv_sec == t2.tv_sec && t1.tv_usec == t2.tv_usec; }
inline timeval &operator+=(timeval &t1, const timeval &t2)
{
    t1.tv_sec += t2.tv_sec;
    t1.tv_usec += t2.tv_usec;
    return normalizedTimeval(t1);
}
inline timeval operator+(const timeval &t1, const timeval &t2)
{
    timeval tmp;
    tmp.tv_sec = t1.tv_sec + t2.tv_sec;
    tmp.tv_usec = t1.tv_usec + t2.tv_usec;
    return normalizedTimeval(tmp);
}
inline timeval operator-(const timeval &t1, const timeval &t2)
{
    timeval tmp;
    tmp.tv_sec = t1.tv_sec - (t2.tv_sec - 1);
    tmp.tv_usec = t1.tv_usec - (t2.tv_usec + 1000000);
    return normalizedTimeval(tmp);
}
inline timeval operator*(const timeval &t1, int mul)
{
    timeval tmp;
    tmp.tv_sec = t1.tv_sec * mul;
    tmp.tv_usec = t1.tv_usec * mul;
    return normalizedTimeval(tmp);
}

static inline void qt_ignore_sigpipe()
{
    // ignore SIGPIPE once per-thread only
    thread_local bool sigpipe_ignored = false;
    if (!sigpipe_ignored) {
        struct sigaction noaction;
        memset(&noaction, 0, sizeof(noaction));
        noaction.sa_handler = SIG_IGN;
        ::sigaction(SIGPIPE, &noaction, nullptr);
        sigpipe_ignored = true;
    }
}

// don't call QT_OPEN or ::open, call qt_safe_open
static inline int qt_safe_open(const char *pathname, int flags, mode_t mode = 0777)
{
#ifdef O_CLOEXEC
    flags |= O_CLOEXEC;
#endif
    int fd;
    Q_EINTR_LOOP(fd, QT_OPEN(pathname, flags, mode));

#ifndef O_CLOEXEC
    // unknown flags are ignored, so we have no way of verifying if
    // O_CLOEXEC was accepted
    if (fd != -1)
        ::fcntl(fd, F_SETFD, FD_CLOEXEC);
#endif
    return fd;
}

// don't call ::pipe or ::pipe2, call qt_safe_pipe
static inline int qt_safe_pipe(int pipefd[2], int flags = 0)
{
#ifdef O_CLOEXEC
    Q_ASSERT((flags & ~(O_CLOEXEC | O_NONBLOCK)) == 0);
#else
    Q_ASSERT((flags & ~O_NONBLOCK) == 0);
#endif

#if defined(QT_HAVE_PIPE2) && defined(O_CLOEXEC)
    // since Linux 2.6.24 and glibc 2.9, FreeBSD also supports it
    return ::pipe2(pipefd, flags | O_CLOEXEC);
#else
    int ret = ::pipe(pipefd);
    if (ret == -1)
        return -1;

    ::fcntl(pipefd[0], F_SETFD, FD_CLOEXEC);
    ::fcntl(pipefd[1], F_SETFD, FD_CLOEXEC);

    // set non-block too?
    if (flags & O_NONBLOCK) {
        ::fcntl(pipefd[0], F_SETFL, ::fcntl(pipefd[0], F_GETFL) | O_NONBLOCK);
        ::fcntl(pipefd[1], F_SETFL, ::fcntl(pipefd[1], F_GETFL) | O_NONBLOCK);
    }

    return 0;
#endif
}

// don't call dup or fcntl(F_DUPFD), call qt_safe_dup
static inline int qt_safe_dup(int oldfd)
{
#ifdef F_DUPFD_CLOEXEC
    // since Linux 2.6.24
    return ::fcntl(oldfd, F_DUPFD_CLOEXEC, 0);
#else
    int ret = ::fcntl(oldfd, F_DUPFD, 0);
    if (ret != -1)
        ::fcntl(ret, F_SETFD, FD_CLOEXEC);
    return ret;
#endif
}

// don't call dup2, call qt_safe_dup2
static inline int qt_safe_dup2(int oldfd, int newfd)
{
    int ret;
    Q_EINTR_LOOP(ret, ::dup2(oldfd, newfd));
    return ret;
}

// don't call QT_READ or ::read, call qt_safe_read
static inline qint64 qt_safe_read(int fd, void *data, qint64 maxlen)
{
    qint64 ret = 0;
    Q_EINTR_LOOP(ret, QT_READ(fd, data, maxlen));
    return ret;
}

// don't call QT_WRITE or ::write, call qt_safe_write
static inline qint64 qt_safe_write(int fd, const void *data, qint64 len)
{
    qint64 ret = 0;
    Q_EINTR_LOOP(ret, QT_WRITE(fd, data, len));
    return ret;
}

static inline qint64 qt_safe_write_nosignal(int fd, const void *data, qint64 len)
{
    qt_ignore_sigpipe();
    return qt_safe_write(fd, data, len);
}

// don't call QT_CREAT or ::creat, call qt_safe_creat
static inline int qt_safe_creat(const char* path, mode_t flags)
{
    int ret;
    Q_EINTR_LOOP(ret, QT_CREAT(path, flags));
    return ret;
}

// don't call QT_CLOSE or ::close, call qt_safe_close
static inline int qt_safe_close(int fd)
{
    int ret;
    Q_EINTR_LOOP(ret, QT_CLOSE(fd));
    return ret;
}

// don't call ::execve, call qt_safe_execve
static inline int qt_safe_execve(const char *filename, char *const argv[],
                                 char *const envp[])
{
    int ret;
    Q_EINTR_LOOP(ret, ::execve(filename, argv, envp));
    return ret;
}

// don't call ::execv, call qt_safe_execv
static inline int qt_safe_execv(const char *path, char *const argv[])
{
    int ret;
    Q_EINTR_LOOP(ret, ::execv(path, argv));
    return ret;
}

// don't call ::execvp, call qt_safe_execvp
static inline int qt_safe_execvp(const char *file, char *const argv[])
{
    int ret;
    Q_EINTR_LOOP(ret, ::execvp(file, argv));
    return ret;
}

// don't call ::waitpid, call qt_safe_waitpid
static inline pid_t qt_safe_waitpid(pid_t pid, int *status, int options)
{
    pid_t ret;
    Q_EINTR_LOOP(ret, ::waitpid(pid, status, options));
    return ret;
}

// don't call ::poll, call qt_safe_poll
static inline int qt_safe_poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    int ret;
    Q_EINTR_LOOP(ret, ::poll(fds, nfds, timeout));
    for (nfds_t i = 0; i < nfds; i++) {
        if ((fds[i].revents & POLLERR) != 0 || (fds[i].revents & POLLHUP) != 0 || (fds[i].revents & POLLNVAL) != 0) {
            // select() compat
            return -1;
        }
    }
    return ret;
}

timeval qt_gettime(); // in qelapsedtimer_unix.cpp

// don't call ::select, call qt_safe_select
Q_CORE_EXPORT int qt_safe_select(int nfds, fd_set *fdread, fd_set *fdwrite, fd_set *fdexcept,
                                 const struct timeval *tv);

/*
   Returns the difference between msecs and elapsed. If msecs is -1,
   however, -1 is returned.
*/
inline static qint64 qt_timeout_value(qint64 msecs, qint64 elapsed)
{
    if (msecs == -1)
        return -1;

    qint64 timeout = msecs - elapsed;
    return timeout < 0 ? 0 : timeout;
}

QT_END_NAMESPACE

#endif
