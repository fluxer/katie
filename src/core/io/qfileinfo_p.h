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

#ifndef QFILEINFO_P_H
#define QFILEINFO_P_H

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

#include "qshareddata.h"
#include "qfilesystemengine_p.h"

#include "qfilesystementry_p.h"
#include "qfilesystemmetadata_p.h"

QT_BEGIN_NAMESPACE

class QFileInfoPrivate : public QSharedData
{
public:
    enum { CachedFileFlags=0x01, CachedLinkTypeFlag=0x02,
           CachedMTime=0x10, CachedCTime=0x20, CachedATime=0x40,
           CachedSize =0x08, CachedPerms=0x80 };

    inline QFileInfoPrivate()
        : QSharedData(), fileEngine(0),
        isDefaultConstructed(true),
        cache_enabled(true), fileFlags(0), cachedFlags(0), fileSize(0)
    {}
    inline QFileInfoPrivate(const QFileInfoPrivate &copy)
        : QSharedData(copy),
        fileEntry(copy.fileEntry),
        metaData(copy.metaData),
        fileEngine(QAbstractFileEngine::create(fileEntry.filePath())),
#ifndef QT_NO_FSFILEENGINE
        isDefaultConstructed(false),
#else
        isDefaultConstructed(!fileEngine),
#endif
        cache_enabled(copy.cache_enabled), fileFlags(0), cachedFlags(0), fileSize(0)
    {}
    inline QFileInfoPrivate(const QString &file)
        : QSharedData(),
        fileEntry(file),
        fileEngine(QAbstractFileEngine::create(file)),
#ifndef QT_NO_FSFILEENGINE
        isDefaultConstructed(false),
#else
        isDefaultConstructed(!fileEngine),
#endif
        cache_enabled(true), fileFlags(0), cachedFlags(0), fileSize(0)
    {
    }

    inline QFileInfoPrivate(const QFileSystemEntry &file, const QFileSystemMetaData &data)
        : QSharedData(),
        fileEntry(file),
        metaData(data),
        fileEngine(QAbstractFileEngine::create(fileEntry.filePath())),
        isDefaultConstructed(false),
        cache_enabled(true), fileFlags(0), cachedFlags(0), fileSize(0)
    {
        //If the file engine is not null, this maybe a "mount point" for a custom file engine
        //in which case we can't trust the metadata
        if (fileEngine)
            metaData = QFileSystemMetaData();
    }

    inline ~QFileInfoPrivate()
    {
        delete fileEngine;
    }

    inline void clearFlags() const {
        fileFlags = 0;
        cachedFlags = 0;
        if (fileEngine)
            (void)fileEngine->fileFlags(QAbstractFileEngine::Refresh);
    }
    inline void clear() {
        metaData.clear();
        clearFlags();
        for (int i = QAbstractFileEngine::NFileNames - 1 ; i >= 0 ; --i)
            fileNames[i].clear();
        fileOwners[1].clear();
        fileOwners[0].clear();
    }

    uint getFileFlags(QAbstractFileEngine::FileFlags) const;
    QDateTime getFileTime(QAbstractFileEngine::FileTime) const;
    QString getFileName(QAbstractFileEngine::FileName) const;
    QString getFileOwner(QAbstractFileEngine::FileOwner own) const;

    QFileSystemEntry fileEntry;
    mutable QFileSystemMetaData metaData;

    QAbstractFileEngine* const fileEngine;

    mutable QString fileNames[QAbstractFileEngine::NFileNames];
    mutable QString fileOwners[2];

    bool const isDefaultConstructed; // QFileInfo is a default constructed instance
    bool cache_enabled;
    mutable uint fileFlags;
    mutable uint cachedFlags;
    mutable qint64 fileSize;
    mutable QDateTime fileTimes[3];
    inline bool getCachedFlag(uint c) const
    { return cache_enabled ? (cachedFlags & c) : false; }
    inline void setCachedFlag(uint c) const
    { if (cache_enabled) cachedFlags |= c; }

};

QT_END_NAMESPACE

#endif // QFILEINFO_P_H
