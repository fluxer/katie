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

#include <QtCore/qhash.h>
#include <QtCore/qsocketnotifier.h>

QT_BEGIN_NAMESPACE

class QFileSystemWatcherEngineUnix : public QFileSystemWatcherEngine
{
    Q_OBJECT

public:
    ~QFileSystemWatcherEngineUnix();

    static QFileSystemWatcherEngineUnix *create();

    QStringList addPaths(const QStringList &paths, QStringList *files, QStringList *directories);
    QStringList removePaths(const QStringList &paths, QStringList *files, QStringList *directories);

private Q_SLOTS:
    void readFromFd();

private:
    QFileSystemWatcherEngineUnix(int fd);

    int sockfd;
    QHash<QString, int> pathToID;
    QHash<int, QString> idToPath;
    QSocketNotifier notifier;
};


QT_END_NAMESPACE

#endif // QT_NO_FILESYSTEMWATCHER

#endif // QFILESYSTEMWATCHER_UNIX_P_H
