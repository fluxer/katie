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
#include "qdir.h"
#include "qfilesystemmetadata_p.h"
#include "qdebug.h"

#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>

QT_BEGIN_NAMESPACE

QStatInfo::QStatInfo()
    : m_mode(0),
    m_uid(QFileSystemMetaData::nobodyID),
    m_gid(QFileSystemMetaData::nobodyID),
    m_mtime(0),
    m_size(0)
{
}

QStatInfo::QStatInfo(const QString &path, const bool listdir)
    : m_mode(0),
    m_uid(QFileSystemMetaData::nobodyID),
    m_gid(QFileSystemMetaData::nobodyID),
    m_mtime(0),
    m_size(0),
    m_path(path),
    m_native(QFile::encodeName(path))
{
    QT_STATBUF statbuf;
    if (QT_STAT(m_native.constData(), &statbuf) == 0) {
        m_mode = statbuf.st_mode;
        m_uid = statbuf.st_uid;
        m_gid = statbuf.st_gid;
        m_mtime = statbuf.st_mtime;
        m_size = statbuf.st_size;
        if (listdir && S_ISDIR(statbuf.st_mode)) {
            m_entries = dirInfos(m_native, path);
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
    m_path(other.m_path),
    m_native(other.m_native)
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
    m_native = other.m_native;
    return *this;
}

bool QStatInfo::operator==(const QStatInfo &other) const
{
    if (m_mode != other.m_mode || m_uid != other.m_uid
        || m_gid != other.m_gid || m_mtime != other.m_mtime
        || m_size != other.m_size || m_entries != other.m_entries) {
        return false;
    }
    if (m_path == other.m_path) {
        return true;
    }
    return (QDir::cleanPath(m_path) == QDir::cleanPath(other.m_path));
}

bool QStatInfo::isReadable() const
{
    return (QT_ACCESS(m_native.constData(), R_OK) == 0);
}

bool QStatInfo::isWritable() const
{
    return (QT_ACCESS(m_native.constData(), W_OK) == 0);
}

bool QStatInfo::isExecutable() const
{
    return (QT_ACCESS(m_native.constData(), X_OK) == 0);
}

bool QStatInfo::dirEquals(const QStatInfo &other) const
{
    if (*this != other) {
        return false;
    }
    if (isDir() && other.isDir()) {
        if (m_entries != dirInfos(other.m_native, other.m_path)) {
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
            const QString dirlocal = QFile::decodeName(dirent->d_name);
            const QString fulllocal = QString::fromLatin1("%1/%2").arg(localpath, dirlocal);
#ifdef QT_HAVE_DIRENT_D_TYPE
            switch (dirent->d_type) {
                case DT_BLK:
                case DT_CHR:
                case DT_FIFO:
                case DT_SOCK:
                case DT_LNK:
                case DT_REG: {
                    result.append(QStatInfo(fulllocal, false));
                    break;
                }
                case DT_DIR:
                case DT_UNKNOWN:
                default: {
                    break;
                }
            }
#else
            const QByteArray fullnative = QFile::encodeName(fulllocal);
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
