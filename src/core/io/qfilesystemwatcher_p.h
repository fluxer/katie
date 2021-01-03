/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#ifndef QFILESYSTEMWATCHER_P_H
#define QFILESYSTEMWATCHER_P_H

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

#include "qfilesystemwatcher.h"

#ifndef QT_NO_FILESYSTEMWATCHER

#include "qobject_p.h"

#include <QtCore/qstringlist.h>

QT_BEGIN_NAMESPACE

class QFileSystemWatcherEngine : public QObject
{
    Q_OBJECT

protected:
    inline QFileSystemWatcherEngine()
    {
    }

public:
    // fills \a files and \a directories with the \a paths it could
    // watch, and returns a list of paths this engine could not watch
    virtual QStringList addPaths(const QStringList &paths,
                                 QStringList *files,
                                 QStringList *directories) = 0;
    // removes \a paths from \a files and \a directories, and returns
    // a list of paths this engine does not know about (either addPath
    // failed or wasn't called)
    virtual QStringList removePaths(const QStringList &paths,
                                    QStringList *files,
                                    QStringList *directories) = 0;

Q_SIGNALS:
    void fileChanged(const QString &path, bool removed);
    void directoryChanged(const QString &path, bool removed);
};

class QFileSystemWatcherPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QFileSystemWatcher)

public:
    QFileSystemWatcherPrivate();
    void init();
    void initNativeEngine();
    void initPollerEngine();

    QFileSystemWatcherEngine *watcher;
    QStringList files, directories;

    // private slots
    void _q_fileChanged(const QString &path, bool removed);
    void _q_directoryChanged(const QString &path, bool removed);
};

QT_END_NAMESPACE

#endif // QT_NO_FILESYSTEMWATCHER

#endif // QFILESYSTEMWATCHER_P_H
