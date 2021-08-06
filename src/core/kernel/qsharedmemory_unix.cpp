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

#include "qplatformdefs.h"

#include "qsharedmemory.h"
#include "qsharedmemory_p.h"

#include "qdebug.h"
#include "qfile.h"

#ifndef QT_NO_SHAREDMEMORY
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif // QT_NO_SHAREDMEMORY
#include <errno.h>

#include "qcore_unix_p.h"

#ifndef QT_NO_SHAREDMEMORY

//#define QSHAREDMEMORY_DEBUG

QT_BEGIN_NAMESPACE

QSharedMemoryPrivate::QSharedMemoryPrivate()
    : QObjectPrivate(), memory(0), size(0), error(QSharedMemory::NoError),
#ifndef QT_NO_SYSTEMSEMAPHORE
      systemSemaphore(QString()), lockedByMe(false),
#endif
      hand(0)
{
}

void QSharedMemoryPrivate::setErrorString(const QString &function)
{
    // EINVAL is handled in functions so they can give better error strings
    switch (errno) {
    case EACCES:
    case EPERM:
        errorString = QSharedMemory::tr("%1: permission denied").arg(function);
        error = QSharedMemory::PermissionDenied;
        break;
    case EEXIST:
        errorString = QSharedMemory::tr("%1: already exists").arg(function);
        error = QSharedMemory::AlreadyExists;
        break;
    case ENOENT:
        errorString = QSharedMemory::tr("%1: doesn't exist").arg(function);
        error = QSharedMemory::NotFound;
        break;
    case EAGAIN:
    case EMFILE:
    case ENFILE:
    case ENOMEM:
    case ENOSPC:
        errorString = QSharedMemory::tr("%1: out of resources").arg(function);
        error = QSharedMemory::OutOfResources;
        break;
    case EOVERFLOW:
        errorString = QSharedMemory::tr("%1: invalid size").arg(function);
        error = QSharedMemory::InvalidSize;
        break;
    default:
        errorString = QSharedMemory::tr("%1: unknown error %2").arg(function).arg(errno);
        error = QSharedMemory::UnknownError;
#ifdef QSHAREDMEMORY_DEBUG
        qDebug() << errorString << "key" << key << "errno" << errno << EINVAL;
#endif
        break;
    }
}

/*!
    \internal

    If not already made create the handle used for accessing the shared memory.
*/
int QSharedMemoryPrivate::handle()
{
    // don't allow making handles on empty keys
    QString safeKey = makePlatformSafeKey(key);
    if (safeKey.isEmpty()) {
        errorString = QSharedMemory::tr("%1: key is empty").arg(QLatin1String("QSharedMemory::handle"));
        error = QSharedMemory::KeyError;
        return 0;
    }

    return 1;
}

void QSharedMemoryPrivate::cleanHandle()
{
    qt_safe_close(hand);
    hand = 0;
}

bool QSharedMemoryPrivate::create(int size)
{
    if (!handle())
        return false;

    QByteArray shmName = QFile::encodeName(makePlatformSafeKey(key));

    int fd;
    Q_EINTR_LOOP(fd, ::shm_open(shmName.constData(), O_RDWR | O_CREAT | O_EXCL, 0666));
    if (fd == -1) {
        QString function = QLatin1String("QSharedMemory::create");
        switch (errno) {
        case ENAMETOOLONG:
        case EINVAL:
            errorString = QSharedMemory::tr("%1: bad name").arg(function);
            error = QSharedMemory::KeyError;
            break;
        default:
            setErrorString(function);
        }
        return false;
    }

    // the size may only be set once; ignore errors
    int ret;
    Q_EINTR_LOOP(ret, QT_FTRUNCATE(fd, size));
    if (ret == -1) {
        setErrorString(QLatin1String("QSharedMemory::create (ftruncate)"));
        qt_safe_close(fd);
        return false;
    }

    qt_safe_close(fd);

    return true;
}

bool QSharedMemoryPrivate::attach(QSharedMemory::AccessMode mode)
{
    QByteArray shmName = QFile::encodeName(makePlatformSafeKey(key));

    int oflag = (mode == QSharedMemory::ReadOnly ? O_RDONLY : O_RDWR);
    mode_t omode = (mode == QSharedMemory::ReadOnly ? 0444 : 0660);

    Q_EINTR_LOOP(hand, ::shm_open(shmName.constData(), oflag, omode));
    if (hand == -1) {
        QString function = QLatin1String("QSharedMemory::attach (shm_open)");
        switch (errno) {
        case ENAMETOOLONG:
        case EINVAL:
            errorString = QSharedMemory::tr("%1: bad name").arg(function);
            error = QSharedMemory::KeyError;
            break;
        default:
            setErrorString(function);
        }
        hand = 0;
        return false;
    }

    // grab the size
    QT_STATBUF st;
    if (QT_FSTAT(hand, &st) == -1) {
        setErrorString(QLatin1String("QSharedMemory::attach (fstat)"));
        cleanHandle();
        return false;
    }
    size = st.st_size;

    // grab the memory
    int mprot = (mode == QSharedMemory::ReadOnly ? PROT_READ : PROT_READ | PROT_WRITE);
    memory = QT_MMAP(nullptr, size, mprot, MAP_SHARED, hand, 0);
    if (memory == MAP_FAILED || !memory) {
        setErrorString(QLatin1String("QSharedMemory::attach (mmap)"));
        cleanHandle();
        memory = nullptr;
        size = 0;
        return false;
    }

    return true;
}

bool QSharedMemoryPrivate::detach()
{
    // detach from the memory segment
    if (munmap(memory, size) == -1) {
        setErrorString(QLatin1String("QSharedMemory::detach (munmap)"));
        return false;
    }
    memory = nullptr;
    size = 0;

    // get the number of current attachments
    int shm_nattch = 0;
    QT_STATBUF st;
    if (QT_FSTAT(hand, &st) == 0) {
        // subtract 2 from linkcount: one for our own open and one for the dir entry
        shm_nattch = st.st_nlink - 2;
    }
    cleanHandle();
    // if there are no attachments then unlink the shared memory
    if (shm_nattch == 0) {
        QByteArray shmName = QFile::encodeName(makePlatformSafeKey(key));
        if (::shm_unlink(shmName.constData()) == -1 && errno != ENOENT)
            setErrorString(QLatin1String("QSharedMemory::detach (shm_unlink)"));
    }

    return true;
}

QT_END_NAMESPACE

#endif // QT_NO_SHAREDMEMORY
