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

#ifndef QFILESYSTEMWATCHER_UNIX_P_H
#define QFILESYSTEMWATCHER_UNIX_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qfilesystemwatcher_p.h"

#ifndef QT_NO_FILESYSTEMWATCHER

#include "qhash.h"
#include "qfile.h"
#include "qdir.h"
#include "qfileinfo.h"
#include "qdatetime.h"
#include "qtimer.h"

QT_BEGIN_NAMESPACE


class QFileSystemWatcherEngineUnix : public QFileSystemWatcherEngine
{
    Q_OBJECT

    class FileInfo
    {
        uint ownerId;
        uint groupId;
        QFile::Permissions permissions;
        QDateTime lastModified;
        QStringList entries;

    public:
        FileInfo(const QFileInfo &fileInfo)
            : ownerId(fileInfo.ownerId()),
              groupId(fileInfo.groupId()),
              permissions(fileInfo.permissions()),
              lastModified(fileInfo.lastModified())
        { 
            if (fileInfo.isDir()) {
                entries = fileInfo.absoluteDir().entryList(QDir::AllEntries);
            }
        }
        FileInfo &operator=(const QFileInfo &fileInfo)
        {
            *this = FileInfo(fileInfo);
            return *this;
        }

        bool operator!=(const QFileInfo &fileInfo) const
        {
            if (ownerId != fileInfo.ownerId()
                    || groupId != fileInfo.groupId()
                    || permissions != fileInfo.permissions()
                    || lastModified != fileInfo.lastModified()) {
                return true;
            }
            if (fileInfo.isDir() && entries != fileInfo.absoluteDir().entryList(QDir::AllEntries))
                return true;
            return false;
        }
    };

    QHash<QString, FileInfo> files, directories;

public:
    QFileSystemWatcherEngineUnix();

    QStringList addPaths(const QStringList &paths, QStringList *files, QStringList *directories);
    QStringList removePaths(const QStringList &paths, QStringList *files, QStringList *directories);

private Q_SLOTS:
    void timeout();

private:
    QTimer timer;
};


QT_END_NAMESPACE

#endif // QT_NO_FILESYSTEMWATCHER

#endif // QFILESYSTEMWATCHER_UNIX_P_H
