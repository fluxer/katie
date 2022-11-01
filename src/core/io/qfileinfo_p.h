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
    enum FileName {
        AbsoluteName,
        AbsolutePathName,
        LinkName,
        CanonicalName,
        CanonicalPathName,
        NFileNames
    };

    inline QFileInfoPrivate()
        : QSharedData(),
        isDefaultConstructed(true),
        cache_enabled(true)
    {}
    inline QFileInfoPrivate(const QFileInfoPrivate &copy)
        : QSharedData(copy),
        fileEntry(copy.fileEntry),
        metaData(copy.metaData),
        isDefaultConstructed(false),
        cache_enabled(copy.cache_enabled)
    {}
    inline QFileInfoPrivate(const QString &file)
        : QSharedData(),
        fileEntry(file),
        isDefaultConstructed(false),
        cache_enabled(true)
    {
    }

    inline QFileInfoPrivate(const QFileSystemEntry &file, const QFileSystemMetaData &data)
        : QSharedData(),
        fileEntry(file),
        metaData(data),
        isDefaultConstructed(false),
        cache_enabled(true)
    {
    }

    inline void clear() const {
        metaData.clear();
        for (int i = 0; i < FileName::NFileNames; i++)
            fileNames[i].clear();
    }

    QString getFileName(const FileName name) const;

    QFileSystemEntry fileEntry;
    mutable QFileSystemMetaData metaData;

    mutable QString fileNames[FileName::NFileNames];

    bool const isDefaultConstructed; // QFileInfo is a default constructed instance
    bool cache_enabled;
};

QT_END_NAMESPACE

#endif // QFILEINFO_P_H
