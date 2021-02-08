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
#include "qfilesystementry_p.h"

QT_BEGIN_NAMESPACE

class QFileSystemEngine;

class QFileSystemMetaData
{
public:
    QFileSystemMetaData()
        : entryFlags(0),
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
        OwnerReadPermission = QFile::ReadOwner,
        OwnerWritePermission = QFile::WriteOwner,
        OwnerExecutePermission = QFile::ExeOwner,
        UserReadPermission  = QFile::ReadUser,
        UserWritePermission  = QFile::WriteUser,
        UserExecutePermission  = QFile::ExeUser,
        GroupReadPermission = QFile::ReadGroup,
        GroupWritePermission = QFile::WriteGroup,
        GroupExecutePermission = QFile::ExeGroup,
        OtherReadPermission = QFile::ReadOther,
        OtherWritePermission = QFile::WriteOther,
        OtherExecutePermission = QFile::ExeOther,

        OtherPermissions    = OtherReadPermission | OtherWritePermission | OtherExecutePermission,
        GroupPermissions    = GroupReadPermission | GroupWritePermission | GroupExecutePermission,
        UserPermissions     = UserReadPermission  | UserWritePermission  | UserExecutePermission,
        OwnerPermissions    = OwnerReadPermission | OwnerWritePermission | OwnerExecutePermission,

        Permissions         = OtherPermissions | GroupPermissions | UserPermissions | OwnerPermissions,

        // Type
        LinkType            = 0x00010000,
        FileType            = 0x00020000,
        DirectoryType       = 0x00040000,
        SequentialType      = 0x00800000,   // Note: overlaps with QAbstractFileEngine::RootFlag

        // Attributes
        HiddenAttribute     = 0x00100000,
        SizeAttribute       = 0x00200000,   // Note: overlaps with QAbstractFileEngine::LocalDiskFlag
        ExistsAttribute     = 0x00400000,

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

    inline bool hasFlags(MetaDataFlags flags) const
    {
        return ((entryFlags & flags) == flags);
    }

    inline MetaDataFlags missingFlags(MetaDataFlags flags)
    {
        return flags & ~entryFlags;
    }

    inline void clear()
    {
        entryFlags = 0;
    }

    inline void clearFlags(MetaDataFlags flags = AllMetaDataFlags)
    {
        entryFlags &= ~flags;
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
    void fillFromDirEnt(const QT_DIRENT &direntBuffer, const QFileSystemEntry::NativePath &nativePath);

    static const uint nobodyID;

private:
    friend class QFileSystemEngine;

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
