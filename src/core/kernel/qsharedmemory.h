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

#ifndef QSHAREDMEMORY_H
#define QSHAREDMEMORY_H

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_SHAREDMEMORY

class QSharedMemoryPrivate;

class Q_CORE_EXPORT QSharedMemory : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSharedMemory)

public:
    enum AccessMode {
        ReadOnly,
        ReadWrite
    };

    enum SharedMemoryError {
        NoError,
        PermissionDenied,
        InvalidSize,
        KeyError,
        AlreadyExists,
        NotFound,
        LockError,
        OutOfResources,
        UnknownError
    };

    QSharedMemory(QObject *parent = nullptr);
    QSharedMemory(const QString &key, QObject *parent = nullptr);
    ~QSharedMemory();

    void setKey(const QString &key);
    QString key() const;
    void setNativeKey(const QString &key);
    QString nativeKey() const;

    bool create(int size, AccessMode mode = ReadWrite);
    int size() const;

    bool attach(AccessMode mode = ReadWrite);
    bool isAttached() const;
    bool detach();

    void *data();
    const void* constData() const;
    const void *data() const;

#ifndef QT_NO_SYSTEMSEMAPHORE
    bool lock();
    bool unlock();
#endif

    SharedMemoryError error() const;
    QString errorString() const;

private:
    Q_DISABLE_COPY(QSharedMemory)
};

#endif // QT_NO_SHAREDMEMORY

QT_END_NAMESPACE


#endif // QSHAREDMEMORY_H

