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

#include "qcore_unix_p.h"
#include "qelapsedtimer.h"
#include "qbytearray.h"
#include "qdebug.h"

#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>

QT_BEGIN_NAMESPACE

QStatInfo::QStatInfo(const QString &path, const bool listdir)
    : m_mode(0),
    m_uid(-2),
    m_gid(-2),
    m_mtime(0),
    m_size(0),
    m_path(path.toLocal8Bit())
{
    QT_STATBUF statbuf;
    if (QT_STAT(m_path.constData(), &statbuf) == 0) {
        m_mode = statbuf.st_mode;
        m_uid = statbuf.st_uid;
        m_gid = statbuf.st_gid;
        m_mtime = statbuf.st_mtime;
        m_size = statbuf.st_size;
        if (listdir && S_ISDIR(statbuf.st_mode)) {
            m_entries = dirInfos(m_path, path);
        }
    }
}

QStatInfo::QStatInfo(const QStatInfo &other)
    : m_mode(other.m_mode),
    m_uid(other.m_uid),
    m_gid(other.m_gid),
    m_mtime(other.m_mtime),
    m_size(other.m_size),
    m_entries(other.m_entries),
    m_path(other.m_path)
{
}

QStatInfo& QStatInfo::operator=(const QStatInfo &other)
{
    m_mode = other.m_mode;
    m_uid = other.m_uid;
    m_gid = other.m_gid;
    m_mtime = other.m_mtime;
    m_size = other.m_size;
    m_entries = other.m_entries;
    m_path = other.m_path;
    return *this;
}

bool QStatInfo::operator==(const QStatInfo &other) const
{
    if (m_mode != other.m_mode || m_uid != other.m_uid
        || m_gid != other.m_gid || m_mtime != other.m_mtime
        || m_size != other.m_size) {
        return false;
    }
    return (m_entries == other.m_entries && m_path == other.m_path);
}

bool QStatInfo::isReadable() const
{
    return (QT_ACCESS(m_path.constData(), R_OK) == 0);
}

bool QStatInfo::isWritable() const
{
    return (QT_ACCESS(m_path.constData(), W_OK) == 0);
}

bool QStatInfo::isExecutable() const
{
    return (QT_ACCESS(m_path.constData(), X_OK) == 0);
}

bool QStatInfo::dirEquals(const QStatInfo &other) const
{
    if (*this != other) {
        return false;
    }
    if (isDir() && other.isDir()) {
        const QString localpath = QString::fromLocal8Bit(other.m_path);
        if (m_entries != dirInfos(other.m_path, localpath)) {
            return false;
        }
    }
    return true;
}

QList<QStatInfo> QStatInfo::dirInfos(const QByteArray &nativepath, const QString &localpath)
{
    QList<QStatInfo> result;
    QT_DIR *dir = QT_OPENDIR(nativepath.constData());
    if (dir) {
        QT_DIRENT *dirent = QT_READDIR(dir);
        while (dirent) {
            if (qstrcmp(".", dirent->d_name) == 0 || qstrcmp("..", dirent->d_name) == 0) {
                dirent = QT_READDIR(dir);
                continue;
            }
            const QString dirlocal = QString::fromLocal8Bit(dirent->d_name);
            const QString fulllocal = QString::fromLatin1("%1/%2").arg(localpath, dirlocal);
#ifdef QT_HAVE_DIRENT_D_TYPE
            switch (dirent->d_type) {
                case DT_BLK:
                case DT_CHR:
                case DT_FIFO:
                case DT_SOCK:
                case DT_LNK:
                case DT_REG:
                case DT_DIR: {
                    result.append(QStatInfo(fulllocal, false));
                    break;
                }
                case DT_UNKNOWN:
                default: {
                    break;
                }
            }
#else
            const QByteArray fullnative = fulllocal.toLocal8Bit();
            QT_STATBUF statbuf;
            if (QT_STAT(fullnative.constData(), &statbuf) == 0) {
                result.append(QStatInfo(fulllocal, false));
            }
#endif
            dirent = QT_READDIR(dir);
        }
        QT_CLOSEDIR(dir);
    }
    return result;
}


static inline bool time_update(struct timeval *tv, const struct timeval &start,
                               const struct timeval &timeout)
{
    // clock source is (hopefully) monotonic, so we can recalculate how much timeout is left;
    // if it isn't monotonic, we'll simply hope that it hasn't jumped, because we have no alternative
    struct timeval now = qt_gettime();
    *tv = timeout + start - now;
    return tv->tv_sec >= 0;
}

int qt_safe_select(int nfds, fd_set *fdread, fd_set *fdwrite, fd_set *fdexcept,
                   const struct timeval *orig_timeout)
{
    if (!orig_timeout) {
        // no timeout -> block forever
        int ret;
        do {
            ret = ::select(nfds, fdread, fdwrite, fdexcept, 0);
        } while (ret == -1 && (errno == EINTR || errno == EAGAIN));
        return ret;
    }

    timeval start = qt_gettime();
    timeval timeout = *orig_timeout;

    // loop and recalculate the timeout as needed
    int ret;
    forever {
        ret = ::select(nfds, fdread, fdwrite, fdexcept, &timeout);
        if (ret != -1 || errno != EINTR)
            return ret;

        // recalculate the timeout
        if (!time_update(&timeout, start, *orig_timeout)) {
            // timeout during update
            // or clock reset, fake timeout error
            return 0;
        }
    }
}

QT_END_NAMESPACE
