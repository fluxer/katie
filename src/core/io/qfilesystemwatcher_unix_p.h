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

#include "qglobal.h"

#ifndef QT_NO_FILESYSTEMWATCHER

#include "qhash.h"
#include "qfile.h"
#include "qdir.h"
#include "qfileinfo.h"
#include "qdatetime.h"
#include "qtimer.h"
#include "qcore_unix_p.h"

QT_BEGIN_NAMESPACE


class QFileSystemWatcherEngineUnix : public QObject
{
    Q_OBJECT

    QHash<QString, QStatInfo> files, directories;

public:
    QFileSystemWatcherEngineUnix();

    QStringList addPaths(const QStringList &paths, QStringList *files, QStringList *directories);
    QStringList removePaths(const QStringList &paths, QStringList *files, QStringList *directories);

Q_SIGNALS:
    void fileChanged(const QString &path, bool removed);
    void directoryChanged(const QString &path, bool removed);

private Q_SLOTS:
    void timeout();

private:
    QTimer timer;
};


QT_END_NAMESPACE

#endif // QT_NO_FILESYSTEMWATCHER

#endif // QFILESYSTEMWATCHER_UNIX_P_H
