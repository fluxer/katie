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

#include "qplatformdefs.h"
#include "qabstractfileengine.h"
#include "qfsfileengine_p.h"
#include "qcore_unix_p.h"
#include "qfilesystementry_p.h"
#include "qfilesystemengine_p.h"

#ifndef QT_NO_FSFILEENGINE

#include "qfile.h"
#include "qdir.h"
#include "qdatetime.h"

#include <sys/mman.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

QT_BEGIN_NAMESPACE

bool QFSFileEnginePrivate::doStat(QFileSystemMetaData::MetaDataFlags flags) const
{
    if (!metaData.hasFlags(flags)) {
        if (fd != -1)
            QFileSystemEngine::fillMetaData(fd, metaData);

        if (metaData.missingFlags(flags) && !fileEntry.isEmpty())
            QFileSystemEngine::fillMetaData(fileEntry, metaData, metaData.missingFlags(flags));
    }

    return metaData.exists();
}

uchar *QFSFileEnginePrivate::map(qint64 offset, qint64 size)
{
    Q_Q(QFSFileEngine);
    if (openMode == QIODevice::NotOpen) {
        q->setError(QFile::PermissionsError, qt_error_string(EACCES));
        return 0;
    }

    if (offset < 0 || offset != qint64(QT_OFF_T(offset))
            || size < 0 || quint64(size) > quint64(size_t(-1))) {
        q->setError(QFile::UnspecifiedError, qt_error_string(EINVAL));
        return 0;
    }

    // If we know the mapping will extend beyond EOF, fail early to avoid
    // undefined behavior. Otherwise, let mmap have its say.
    if (doStat(QFileSystemMetaData::SizeAttribute)
            && (QT_OFF_T(size) > metaData.size() - QT_OFF_T(offset)))
        qWarning("QFSFileEngine::map: Mapping a file beyond its size is not portable");

    int access = 0;
    if (openMode & QIODevice::ReadOnly) access |= PROT_READ;
    if (openMode & QIODevice::WriteOnly) access |= PROT_WRITE;

    static const int pageSize = ::getpagesize();
    const int extra = offset % pageSize;

    if (quint64(size + extra) > quint64((size_t)-1)) {
        q->setError(QFile::UnspecifiedError, qt_error_string(EINVAL));
        return 0;
    }

    size_t realSize = (size_t)size + extra;
    QT_OFF_T realOffset = QT_OFF_T(offset);
    realOffset &= ~(QT_OFF_T(pageSize - 1));

    void *mapAddress = QT_MMAP(nullptr, realSize,
                   access, MAP_SHARED, fd, realOffset);
    if (mapAddress != MAP_FAILED) {
        uchar *address = extra + static_cast<uchar*>(mapAddress);
        maps[address] = QPair<int,size_t>(extra, realSize);
        return address;
    }

    switch(errno) {
    case EBADF:
        q->setError(QFile::PermissionsError, qt_error_string(EACCES));
        break;
    case ENFILE:
    case ENOMEM:
        q->setError(QFile::ResourceError, qt_error_string(errno));
        break;
    case EINVAL:
        // size are out of bounds
    default:
        q->setError(QFile::UnspecifiedError, qt_error_string(errno));
        break;
    }
    return 0;
}

bool QFSFileEnginePrivate::unmap(uchar *ptr)
{
    Q_Q(QFSFileEngine);
    if (!maps.contains(ptr)) {
        q->setError(QFile::PermissionsError, qt_error_string(EACCES));
        return false;
    }

    uchar *start = ptr - maps[ptr].first;
    size_t len = maps[ptr].second;
    if (::munmap(start, len) == -1) {
        q->setError(QFile::UnspecifiedError, qt_error_string(errno));
        return false;
    }
    maps.remove(ptr);
    return true;
}

bool QFSFileEngine::remove()
{
    Q_D(QFSFileEngine);
    int error;
    bool ret = QFileSystemEngine::removeFile(d->fileEntry, &error);
    d->metaData.clear();
    if (!ret) {
        setError(QFile::RemoveError, qt_error_string(error));
    }
    return ret;
}

bool QFSFileEngine::copy(const QString &newName)
{
    Q_D(QFSFileEngine);
    int error;
    bool ret = QFileSystemEngine::copyFile(d->fileEntry, QFileSystemEntry(newName), &error);
    if (!ret) {
        setError(QFile::CopyError, qt_error_string(error));
    }
    return ret;
}

bool QFSFileEngine::rename(const QString &newName)
{
    Q_D(QFSFileEngine);
    int error;
    bool ret = QFileSystemEngine::renameFile(d->fileEntry, QFileSystemEntry(newName), &error);
    d->metaData.clear();
    if (!ret) {
        setError(QFile::RenameError, qt_error_string(error));
    }

    return ret;
}

bool QFSFileEngine::link(const QString &newName)
{
    Q_D(QFSFileEngine);
    int error;
    bool ret = QFileSystemEngine::createLink(d->fileEntry, QFileSystemEntry(newName), &error);
    if (!ret) {
        setError(QFile::RenameError, qt_error_string(error));
    }
    return ret;
}

bool QFSFileEngine::mkdir(const QString &name, bool createParentDirectories) const
{
    return QFileSystemEngine::createDirectory(QFileSystemEntry(name), createParentDirectories);
}

bool QFSFileEngine::rmdir(const QString &name, bool recurseParentDirectories) const
{
    return QFileSystemEngine::removeDirectory(QFileSystemEntry(name), recurseParentDirectories);
}

bool QFSFileEngine::caseSensitive() const
{
    return true;
}

bool QFSFileEngine::setCurrentPath(const QString &path)
{
    return QFileSystemEngine::setCurrentPath(QFileSystemEntry(path));
}

QString QFSFileEngine::currentPath(const QString &)
{
    return QFileSystemEngine::currentPath().filePath();
}

QString QFSFileEngine::homePath()
{
    return QFileSystemEngine::homePath();
}

QString QFSFileEngine::rootPath()
{
    return QFileSystemEngine::rootPath();
}

QString QFSFileEngine::tempPath()
{
    return QFileSystemEngine::tempPath();
}

/*!
    \reimp
*/
QAbstractFileEngine::FileFlags QFSFileEngine::fileFlags(FileFlags type) const
{
    Q_D(const QFSFileEngine);

    if (type & Refresh)
        d->metaData.clear();

    QAbstractFileEngine::FileFlags ret = 0;

    if (type & FlagsMask)
        ret |= LocalDiskFlag;

    QFileSystemMetaData::MetaDataFlags queryFlags =
            QFileSystemMetaData::MetaDataFlags(uint(type))
            & QFileSystemMetaData::Permissions;
    queryFlags |= QFileSystemMetaData::LinkType;

    if (type & TypesMask)
        queryFlags |= QFileSystemMetaData::LinkType
                | QFileSystemMetaData::FileType
                | QFileSystemMetaData::DirectoryType;

    if (type & FlagsMask)
        queryFlags |= QFileSystemMetaData::HiddenAttribute
                | QFileSystemMetaData::ExistsAttribute;

    bool exists = d->doStat(queryFlags);

    if (!exists && !d->metaData.isLink())
        return ret;

    if (exists && (type & PermsMask))
        ret |= FileFlags(uint(d->metaData.permissions()));

    if (type & TypesMask) {
        if ((type & LinkType) && d->metaData.isLink())
            ret |= LinkType;
        if (exists) {
            if (d->metaData.isFile()) {
                ret |= FileType;
            } else if (d->metaData.isDirectory()) {
                ret |= DirectoryType;
            }
        }
    }

    if (type & FlagsMask) {
        if (exists)
            ret |= ExistsFlag;
        if (d->fileEntry.isRoot())
            ret |= RootFlag;
        else if (d->metaData.isHidden())
            ret |= HiddenFlag;
    }

    return ret;
}

QString QFSFileEngine::fileName(FileName file) const
{
    Q_D(const QFSFileEngine);
    if (file == BaseName) {
        return d->fileEntry.fileName();
    } else if (file == PathName) {
        return d->fileEntry.path();
    } else if (file == AbsoluteName || file == AbsolutePathName) {
        QFileSystemEntry entry(QFileSystemEngine::absoluteName(d->fileEntry));
        if (file == AbsolutePathName) {
            return entry.path();
        }
        return entry.filePath();
    } else if (file == CanonicalName || file == CanonicalPathName) {
        QFileSystemEntry entry(QFileSystemEngine::canonicalName(d->fileEntry, d->metaData));
        if (file == CanonicalPathName)
            return entry.path();
        return entry.filePath();
    } else if (file == LinkName) {
        if (!d->metaData.hasFlags(QFileSystemMetaData::LinkType))
            QFileSystemEngine::fillMetaData(d->fileEntry, d->metaData, QFileSystemMetaData::LinkType);
        if (d->metaData.isLink()) {
            QFileSystemEntry entry = QFileSystemEngine::getLinkTarget(d->fileEntry, d->metaData);
            return entry.filePath();
        }
        return QString();
    }
    return d->fileEntry.filePath();
}

bool QFSFileEngine::isRelativePath() const
{
    Q_D(const QFSFileEngine);
    return d->fileEntry.filePath().length() ? d->fileEntry.filePath()[0] != QLatin1Char('/') : true;
}

uint QFSFileEngine::ownerId(FileOwner own) const
{
    Q_D(const QFSFileEngine);

    if (d->doStat(QFileSystemMetaData::OwnerIds)) {
        if (own == QAbstractFileEngine::OwnerUser)
            return d->metaData.userId();
        return d->metaData.groupId();
    }

    return QFileSystemMetaData::nobodyID;
}

QString QFSFileEngine::owner(FileOwner own) const
{
    if (own == QAbstractFileEngine::OwnerUser)
        return QFileSystemEngine::resolveUserName(ownerId(own));
    return QFileSystemEngine::resolveGroupName(ownerId(own));
}

bool QFSFileEngine::setPermissions(uint perms)
{
    Q_D(QFSFileEngine);
    int error;
    if (!QFileSystemEngine::setPermissions(d->fileEntry, QFile::Permissions(perms), &error)) {
        setError(QFile::PermissionsError, qt_error_string(error));
        return false;
    }
    return true;
}

bool QFSFileEngine::setSize(qint64 size)
{
    Q_D(QFSFileEngine);
    int ret = 0;
    if (d->fd != -1) {
        Q_EINTR_LOOP(ret, QT_FTRUNCATE(d->fd, size));
    } else {
        Q_EINTR_LOOP(ret, QT_TRUNCATE(d->fileEntry.nativeFilePath().constData(), size));
    }
    d->metaData.clearFlags(QFileSystemMetaData::SizeAttribute);
    if (ret == -1) {
        setError(QFile::ResizeError, qt_error_string(errno));
        return false;
    }
    return true;
}

QDateTime QFSFileEngine::fileTime(FileTime time) const
{
    Q_D(const QFSFileEngine);

    if (d->doStat(QFileSystemMetaData::Times)) {
        switch (time) {
        case QAbstractFileEngine::ModificationTime:
            return d->metaData.modificationTime();
        case QAbstractFileEngine::AccessTime:
            return d->metaData.accessTime();
        case QAbstractFileEngine::CreationTime:
            return d->metaData.creationTime();
        }
    }

    return QDateTime();
}

QT_END_NAMESPACE

#endif // QT_NO_FSFILEENGINE
