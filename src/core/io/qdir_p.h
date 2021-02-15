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

#ifndef QDIR_PRIVATE_H
#define QDIR_PRIVATE_H

#include "qfilesystementry_p.h"
#include "qfilesystemmetadata_p.h"

QT_BEGIN_NAMESPACE

class QDirPrivate : public QSharedData
{
public:
    QDirPrivate(const QString &path, const QStringList &nameFilters_ = QStringList(),
            QDir::SortFlags sort_ = QDir::SortFlags(QDir::Name | QDir::IgnoreCase),
            QDir::Filters filters_ = QDir::AllEntries);

    QDirPrivate(const QDirPrivate &copy);
    ~QDirPrivate();

    bool exists() const;

    void initFileEngine();
    void initFileLists(const QDir &dir) const;

    static void sortFileList(QDir::SortFlags, QFileInfoList &, QStringList *, QFileInfoList *);

    inline void setPath(const QString &path);

    inline void clearFileLists();

    inline void resolveAbsoluteEntry() const;

    QStringList nameFilters;
    QDir::SortFlags sort;
    QDir::Filters filters;


    QAbstractFileEngine* fileEngine;

    mutable bool fileListsInitialized;
    mutable QStringList files;
    mutable QFileInfoList fileInfos;

    QFileSystemEntry dirEntry;
    mutable QFileSystemEntry absoluteDirEntry;
    mutable QFileSystemMetaData metaData;
};

QT_END_NAMESPACE

#endif
