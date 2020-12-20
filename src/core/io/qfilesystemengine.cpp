/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#include "qfilesystemengine_p.h"
#include "qdir.h"
#include "qset.h"
#include "qabstractfileengine_p.h"

QT_BEGIN_NAMESPACE

//these unix functions are in this file, because they were shared by symbian port
//for open C file handles.
//static
bool QFileSystemEngine::fillMetaData(int fd, QFileSystemMetaData &data)
{
    data.entryFlags &= ~QFileSystemMetaData::PosixStatFlags;

    QT_STATBUF statBuffer;
    if (QT_FSTAT(fd, &statBuffer) == 0) {
        data.fillFromStatBuf(statBuffer);
        return true;
    }

    return false;
}

void QFileSystemMetaData::fillFromStatBuf(const QT_STATBUF &statBuffer)
{
    // Permissions
    if (statBuffer.st_mode & S_IRUSR)
        entryFlags |= QFileSystemMetaData::OwnerReadPermission;
    if (statBuffer.st_mode & S_IWUSR)
        entryFlags |= QFileSystemMetaData::OwnerWritePermission;
    if (statBuffer.st_mode & S_IXUSR)
        entryFlags |= QFileSystemMetaData::OwnerExecutePermission;

    if (statBuffer.st_mode & S_IRGRP)
        entryFlags |= QFileSystemMetaData::GroupReadPermission;
    if (statBuffer.st_mode & S_IWGRP)
        entryFlags |= QFileSystemMetaData::GroupWritePermission;
    if (statBuffer.st_mode & S_IXGRP)
        entryFlags |= QFileSystemMetaData::GroupExecutePermission;

    if (statBuffer.st_mode & S_IROTH)
        entryFlags |= QFileSystemMetaData::OtherReadPermission;
    if (statBuffer.st_mode & S_IWOTH)
        entryFlags |= QFileSystemMetaData::OtherWritePermission;
    if (statBuffer.st_mode & S_IXOTH)
        entryFlags |= QFileSystemMetaData::OtherExecutePermission;

    // Type
    if ((statBuffer.st_mode & S_IFMT) == S_IFREG)
        entryFlags |= QFileSystemMetaData::FileType;
    else if ((statBuffer.st_mode & S_IFMT) == S_IFDIR)
        entryFlags |= QFileSystemMetaData::DirectoryType;
    else
        entryFlags |= QFileSystemMetaData::SequentialType;

    // Attributes
    entryFlags |= QFileSystemMetaData::ExistsAttribute;
    entryFlags |= QFileSystemMetaData::SizeAttribute;
    size_ = statBuffer.st_size;

    // Times
    entryFlags |= QFileSystemMetaData::Times;
    creationTime_ = statBuffer.st_ctime;
    modificationTime_ = statBuffer.st_mtime;
    accessTime_ = statBuffer.st_atime;
    userId_ = statBuffer.st_uid;
    groupId_ = statBuffer.st_gid;
}

void QFileSystemMetaData::fillFromDirEnt(const QT_DIRENT &entry)
{
#ifdef QT_HAVE_DIRENT_D_TYPE
    // ### This will clear all entry flags
    switch (entry.d_type)
    {
    case DT_DIR:
        entryFlags = QFileSystemMetaData::DirectoryType
            | QFileSystemMetaData::ExistsAttribute;

        break;

    case DT_BLK:
    case DT_CHR:
    case DT_FIFO:
    case DT_SOCK:
        entryFlags = QFileSystemMetaData::SequentialType
            | QFileSystemMetaData::ExistsAttribute;

        break;

    case DT_LNK:
        entryFlags = QFileSystemMetaData::LinkType;
        break;

    case DT_REG:
        entryFlags = QFileSystemMetaData::FileType
            | QFileSystemMetaData::ExistsAttribute;

        break;

    case DT_UNKNOWN:
    default:
        clear();
    }
#else
    Q_UNUSED(entry)
#endif
}

//static
QString QFileSystemEngine::resolveUserName(const QFileSystemEntry &entry, QFileSystemMetaData &metaData)
{
    if (!metaData.hasFlags(QFileSystemMetaData::UserId))
        QFileSystemEngine::fillMetaData(entry, metaData, QFileSystemMetaData::UserId);
    return resolveUserName(metaData.userId());
}

//static
QString QFileSystemEngine::resolveGroupName(const QFileSystemEntry &entry, QFileSystemMetaData &metaData)
{
    if (!metaData.hasFlags(QFileSystemMetaData::GroupId))
        QFileSystemEngine::fillMetaData(entry, metaData, QFileSystemMetaData::GroupId);
    return resolveGroupName(metaData.groupId());
}

QT_END_NAMESPACE
