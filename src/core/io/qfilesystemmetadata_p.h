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

#ifndef QFILESYSTEMMETADATA_P_H_INCLUDED
#define QFILESYSTEMMETADATA_P_H_INCLUDED

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qplatformdefs.h"
#include <QtCore/qdatetime.h>
#include <QtCore/qabstractfileengine.h>

QT_BEGIN_NAMESPACE

class QFileSystemEngine;

class QFileSystemMetaData
{
public:
    QFileSystemMetaData()
        : knownFlagsMask(0),
        entryFlags(0),
        size_(0),
        creationTime_(0),
        modificationTime_(0),
        accessTime_(0),
        userId_(-2),
        groupId_(-2)
    {
    }

    enum MetaDataFlag {
        // Permissions, overlaps with QFile::Permissions
        OtherReadPermission = 0x00000004,   OtherWritePermission = 0x00000002,  OtherExecutePermission = 0x00000001,
        GroupReadPermission = 0x00000040,   GroupWritePermission = 0x00000020,  GroupExecutePermission = 0x00000010,
        UserReadPermission  = 0x00000400,   UserWritePermission  = 0x00000200,  UserExecutePermission  = 0x00000100,
        OwnerReadPermission = 0x00004000,   OwnerWritePermission = 0x00002000,  OwnerExecutePermission = 0x00001000,

        OtherPermissions    = OtherReadPermission | OtherWritePermission | OtherExecutePermission,
        GroupPermissions    = GroupReadPermission | GroupWritePermission | GroupExecutePermission,
        UserPermissions     = UserReadPermission  | UserWritePermission  | UserExecutePermission,
        OwnerPermissions    = OwnerReadPermission | OwnerWritePermission | OwnerExecutePermission,

        ReadPermissions     = OtherReadPermission | GroupReadPermission | UserReadPermission | OwnerReadPermission,
        WritePermissions    = OtherWritePermission | GroupWritePermission | UserWritePermission | OwnerWritePermission,
        ExecutePermissions  = OtherExecutePermission | GroupExecutePermission | UserExecutePermission | OwnerExecutePermission,

        Permissions         = OtherPermissions | GroupPermissions | UserPermissions | OwnerPermissions,

        // Type
        LinkType            = 0x00010000,
        FileType            = 0x00020000,
        DirectoryType       = 0x00040000,
        SequentialType      = 0x00800000,   // Note: overlaps with QAbstractFileEngine::RootFlag

        Type                = LinkType | FileType | DirectoryType | SequentialType,

        // Attributes
        HiddenAttribute     = 0x00100000,
        SizeAttribute       = 0x00200000,   // Note: overlaps with QAbstractFileEngine::LocalDiskFlag
        ExistsAttribute     = 0x00400000,

        Attributes          = HiddenAttribute | SizeAttribute | ExistsAttribute,

        // Times
        CreationTime        = 0x01000000,   // Note: overlaps with QAbstractFileEngine::Refresh
        ModificationTime    = 0x02000000,
        AccessTime          = 0x04000000,

        Times               = CreationTime | ModificationTime | AccessTime,

        // Owner IDs
        UserId              = 0x10000000,
        GroupId             = 0x20000000,

        OwnerIds            = UserId | GroupId,

        PosixStatFlags      = QFileSystemMetaData::OtherPermissions
                            | QFileSystemMetaData::GroupPermissions
                            | QFileSystemMetaData::OwnerPermissions
                            | QFileSystemMetaData::FileType
                            | QFileSystemMetaData::DirectoryType
                            | QFileSystemMetaData::SequentialType
                            | QFileSystemMetaData::SizeAttribute
                            | QFileSystemMetaData::Times
                            | QFileSystemMetaData::OwnerIds,

        AllMetaDataFlags    = 0xFFFFFFFF

    };
    Q_DECLARE_FLAGS(MetaDataFlags, MetaDataFlag)

    bool hasFlags(MetaDataFlags flags) const
    {
        return ((knownFlagsMask & flags) == flags);
    }

    MetaDataFlags missingFlags(MetaDataFlags flags)
    {
        return flags & ~knownFlagsMask;
    }

    void clear()
    {
        knownFlagsMask = 0;
    }

    void clearFlags(MetaDataFlags flags = AllMetaDataFlags)
    {
        knownFlagsMask &= ~flags;
    }

    inline bool exists() const                    { return (entryFlags & ExistsAttribute); }

    inline bool isLink() const                    { return  (entryFlags & LinkType); }
    inline bool isFile() const                    { return (entryFlags & FileType); }
    inline bool isDirectory() const               { return (entryFlags & DirectoryType); }
    inline bool isSequential() const              { return (entryFlags & SequentialType); }
    inline bool isHidden() const                  { return (entryFlags & HiddenAttribute); }

    inline qint64 size() const                    { return size_; }

    inline QFile::Permissions permissions() const { return QFile::Permissions(Permissions & entryFlags); }

    inline QDateTime creationTime() const         { return QDateTime::fromTime_t(creationTime_); }
    inline QDateTime modificationTime() const     { return QDateTime::fromTime_t(modificationTime_); }
    inline QDateTime accessTime() const           { return QDateTime::fromTime_t(accessTime_); }

    inline uint userId() const                    { return userId_; }
    inline uint groupId() const                   { return groupId_; }

    void fillFromStatBuf(const QT_STATBUF &statBuffer);
    void fillFromDirEnt(const QT_DIRENT &statBuffer);

private:
    friend class QFileSystemEngine;

    MetaDataFlags knownFlagsMask;
    MetaDataFlags entryFlags;

    qint64 size_;

    // Platform-specific data goes here:
    time_t creationTime_;
    time_t modificationTime_;
    time_t accessTime_;

    uint userId_;
    uint groupId_;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QFileSystemMetaData::MetaDataFlags)

QT_END_NAMESPACE

#endif // include guard
