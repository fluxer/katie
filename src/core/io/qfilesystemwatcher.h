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

#ifndef QFILESYSTEMWATCHER_H
#define QFILESYSTEMWATCHER_H

#include <QtCore/qobject.h>

#ifndef QT_NO_FILESYSTEMWATCHER


QT_BEGIN_NAMESPACE


class QFileSystemWatcherPrivate;

class Q_CORE_EXPORT QFileSystemWatcher : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QFileSystemWatcher)

public:
    QFileSystemWatcher(QObject *parent = nullptr);
    QFileSystemWatcher(const QStringList &paths, QObject *parent = nullptr);
    ~QFileSystemWatcher();

    void addPath(const QString &file);
    void addPaths(const QStringList &files);
    void removePath(const QString &file);
    void removePaths(const QStringList &files);

    QStringList files() const;
    QStringList directories() const;

Q_SIGNALS:
    void fileChanged(const QString &path);
    void directoryChanged(const QString &path);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_fileChanged(const QString &path, bool removed))
    Q_PRIVATE_SLOT(d_func(), void _q_directoryChanged(const QString &path, bool removed))
};

QT_END_NAMESPACE


#endif // QT_NO_FILESYSTEMWATCHER
#endif // QFILESYSTEMWATCHER_H
