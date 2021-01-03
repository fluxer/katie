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

#include "qsystemsemaphore.h"
#include "qsystemsemaphore_p.h"

#include "qcoreapplication.h"
#include "qdebug.h"
#include "qfile.h"

#ifndef QT_NO_SYSTEMSEMAPHORE

#include <sys/types.h>
#include <sys/ipc.h>
#ifndef QT_HAVE_SEMAPHORE_H
#include <sys/sem.h>
#endif
#include <fcntl.h>
#include <errno.h>

#include "qcore_unix_p.h"

// OpenBSD 4.2 doesn't define EIDRM, see BUGS section:
// https://man.openbsd.org/OpenBSD-4.2/semop.2
#if defined(Q_OS_OPENBSD) && !defined(EIDRM)
#define EIDRM EINVAL
#endif

// Hurd has SEM_FAILED defined as void pointer which triggers invalid
// type conversion compiler error
#if defined(Q_OS_HURD)
#  define QT_SEM_FAILED static_cast<sem_t*>(SEM_FAILED)
#else
#  define QT_SEM_FAILED SEM_FAILED
#endif

//#define QSYSTEMSEMAPHORE_DEBUG

QT_BEGIN_NAMESPACE

QSystemSemaphorePrivate::QSystemSemaphorePrivate() :
#ifndef QT_HAVE_SEMAPHORE_H
    unix_key(-1), semaphore(-1), createdFile(false),
#else
    semaphore(QT_SEM_FAILED),
#endif
    createdSemaphore(false), error(QSystemSemaphore::NoError)
{
}

void QSystemSemaphorePrivate::setErrorString(const QString &function)
{
    // EINVAL is handled in functions so they can give better error strings
    switch (errno) {
    case EPERM:
    case EACCES:
        errorString = QCoreApplication::translate("QSystemSemaphore", "%1: permission denied").arg(function);
        error = QSystemSemaphore::PermissionDenied;
        break;
    case EEXIST:
        errorString = QCoreApplication::translate("QSystemSemaphore", "%1: already exists").arg(function);
        error = QSystemSemaphore::AlreadyExists;
        break;
    case ENOENT:
        errorString = QCoreApplication::translate("QSystemSemaphore", "%1: does not exist").arg(function);
        error = QSystemSemaphore::NotFound;
        break;
    case ERANGE:
    case ENOMEM:
    case ENOSPC:
    case EMFILE:
    case ENFILE:
    case EOVERFLOW:
        errorString = QCoreApplication::translate("QSystemSemaphore", "%1: out of resources").arg(function);
        error = QSystemSemaphore::OutOfResources;
        break;
    case ENAMETOOLONG:
        errorString = QCoreApplication::translate("QSystemSemaphore", "%1: name error").arg(function);
        error = QSystemSemaphore::KeyError;
        break;
    default:
        errorString = QCoreApplication::translate("QSystemSemaphore", "%1: unknown error %2").arg(function).arg(errno);
        error = QSystemSemaphore::UnknownError;
#ifdef QSYSTEMSEMAPHORE_DEBUG
        qDebug() << errorString << "key" << key << "errno" << errno << EINVAL;
#endif
        break;
    }
}

/*!
    \internal

    Initialise the semaphore
*/
#ifndef QT_HAVE_SEMAPHORE_H
key_t QSystemSemaphorePrivate::handle(QSystemSemaphore::AccessMode mode)
{
    if (-1 != unix_key)
        return unix_key;

    if (key.isEmpty()) {
        errorString = QCoreApplication::tr("%1: key is empty").arg(QLatin1String("QSystemSemaphore::handle"));
        error = QSystemSemaphore::KeyError;
        return -1;
    }

    // ftok requires that an actual file exists somewhere
    int built = QSharedMemoryPrivate::createUnixKeyFile(fileName);
    if (-1 == built) {
        errorString = QCoreApplication::tr("%1: unable to make key").arg(QLatin1String("QSystemSemaphore::handle"));
        error = QSystemSemaphore::KeyError;
        return -1;
    }
    createdFile = (1 == built);

    // Get the unix key for the created file
    unix_key = ftok(QFile::encodeName(fileName).constData(), 'Q');
    if (-1 == unix_key) {
        errorString = QCoreApplication::tr("%1: ftok failed").arg(QLatin1String("QSystemSemaphore::handle"));
        error = QSystemSemaphore::KeyError;
        return -1;
    }

    // Get semaphore
    semaphore = semget(unix_key, 1, 0600 | IPC_CREAT | IPC_EXCL);
    if (-1 == semaphore) {
        if (errno == EEXIST)
            semaphore = semget(unix_key, 1, 0600 | IPC_CREAT);
        if (-1 == semaphore) {
            setErrorString(QLatin1String("QSystemSemaphore::handle"));
            cleanHandle();
            return -1;
        }
        if (mode == QSystemSemaphore::Create) {
            createdSemaphore = true;
            createdFile = true;
        }
    } else {
        createdSemaphore = true;
        // Force cleanup of file, it is possible that it can be left over from a crash
        createdFile = true;
    }

    // Created semaphore so initialize its value.
    if (createdSemaphore && initialValue >= 0) {
        qt_semun init_op;
        init_op.val = initialValue;
        if (-1 == semctl(semaphore, 0, SETVAL, init_op)) {
            setErrorString(QLatin1String("QSystemSemaphore::handle"));
            cleanHandle();
            return -1;
        }
    }

    return unix_key;
}
#else
bool QSystemSemaphorePrivate::handle(QSystemSemaphore::AccessMode mode)
{
    if (semaphore != QT_SEM_FAILED)
        return true;  // we already have a semaphore

    if (fileName.isEmpty()) {
        errorString = QCoreApplication::tr("%1: key is empty").arg(QLatin1String("QSystemSemaphore::handle"));
        error = QSystemSemaphore::KeyError;
        return false;
    }

    QByteArray semName = QFile::encodeName(fileName);

    // Always try with O_EXCL so we know whether we created the semaphore.
    int oflag = O_CREAT | O_EXCL;
    for (int tryNum = 0, maxTries = 1; tryNum < maxTries; ++tryNum) {
        do {
            semaphore = sem_open(semName.constData(), oflag, 0666, initialValue);
        } while (semaphore == QT_SEM_FAILED && errno == EINTR);
        if (semaphore == QT_SEM_FAILED && errno == EEXIST) {
            if (mode == QSystemSemaphore::Create) {
                if (sem_unlink(semName.constData()) == -1 && errno != ENOENT) {
                    setErrorString(QLatin1String("QSystemSemaphore::handle (sem_unlink)"));
                    return false;
                }
                // Race condition: the semaphore might be recreated before
                // we call sem_open again, so we'll retry several times.
                maxTries = 3;
            } else {
                // Race condition: if it no longer exists at the next sem_open
                // call, we won't realize we created it, so we'll leak it later.
                oflag &= ~O_EXCL;
                maxTries = 2;
            }
        } else {
            break;
        }
    }
    if (semaphore == QT_SEM_FAILED) {
        setErrorString(QLatin1String("QSystemSemaphore::handle"));
        return false;
    }

    createdSemaphore = (oflag & O_EXCL) != 0;
    return true;
}
#endif // QT_HAVE_SEMAPHORE_H

/*!
    \internal

    Clean up the semaphore
*/
void QSystemSemaphorePrivate::cleanHandle()
{
#ifndef QT_HAVE_SEMAPHORE_H
    unix_key = -1;

    // remove the file if we made it
    if (createdFile) {
        QFile::remove(fileName);
        createdFile = false;
    }

    if (createdSemaphore) {
        if (-1 != semaphore) {
            if (-1 == semctl(semaphore, 0, IPC_RMID, 0)) {
                setErrorString(QLatin1String("QSystemSemaphore::cleanHandle"));
#ifdef QSYSTEMSEMAPHORE_DEBUG
                qDebug("QSystemSemaphore::cleanHandle semctl failed.");
#endif
            }
            semaphore = -1;
        }
        createdSemaphore = false;
    }
#else
    if (semaphore != QT_SEM_FAILED) {
        if (sem_close(semaphore) == -1) {
            setErrorString(QLatin1String("QSystemSemaphore::cleanHandle (sem_close)"));
#ifdef QSYSTEMSEMAPHORE_DEBUG
            qDebug() << QLatin1String("QSystemSemaphore::cleanHandle sem_close failed.");
#endif
        }
        semaphore = QT_SEM_FAILED;
    }

    if (createdSemaphore) {
        if (sem_unlink(QFile::encodeName(fileName).constData()) == -1 && errno != ENOENT) {
            setErrorString(QLatin1String("QSystemSemaphore::cleanHandle (sem_unlink)"));
#ifdef QSYSTEMSEMAPHORE_DEBUG
            qDebug() << QLatin1String("QSystemSemaphore::cleanHandle sem_unlink failed.");
#endif
        }
        createdSemaphore = false;
    }
#endif // QT_HAVE_SEMAPHORE_H
}

/*!
    \internal
*/
bool QSystemSemaphorePrivate::modifySemaphore(int count)
{
#ifndef QT_HAVE_SEMAPHORE_H
    if (-1 == handle())
        return false;

    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = count;
    operation.sem_flg = SEM_UNDO;

    int res;
    EINTR_LOOP(res, semop(semaphore, &operation, 1));
    if (-1 == res) {
        // If the semaphore was removed be nice and create it and then modifySemaphore again
        if (errno == EINVAL || errno == EIDRM) {
            semaphore = -1;
            cleanHandle();
            handle();
            return modifySemaphore(count);
        }
        setErrorString(QLatin1String("QSystemSemaphore::modifySemaphore"));
#ifdef QSYSTEMSEMAPHORE_DEBUG
        qDebug() << QLatin1String("QSystemSemaphore::modify failed") << count << semctl(semaphore, 0, GETVAL) << errno << EIDRM << EINVAL;
#endif
        return false;
    }
#else
    if (!handle())
        return false;

    if (count > 0) {
        int cnt = count;
        do {
            if (sem_post(semaphore) == -1) {
                setErrorString(QLatin1String("QSystemSemaphore::modifySemaphore (sem_post)"));
#ifdef QSYSTEMSEMAPHORE_DEBUG
                qDebug() << QLatin1String("QSystemSemaphore::modify sem_post failed") << count << errno;
#endif
                // rollback changes to preserve the SysV semaphore behavior
                for ( ; cnt < count; ++cnt) {
                    int res;
                    EINTR_LOOP(res, sem_wait(semaphore));
                }
                return false;
            }
            --cnt;
        } while (cnt > 0);
    } else {
        int res;
        EINTR_LOOP(res, sem_wait(semaphore));
        if (res == -1) {
            // If the semaphore was removed be nice and create it and then modifySemaphore again
            if (errno == EINVAL || errno == EIDRM) {
                semaphore = QT_SEM_FAILED;
                return modifySemaphore(count);
            }
            setErrorString(QLatin1String("QSystemSemaphore::modifySemaphore (sem_wait)"));
#ifdef QSYSTEMSEMAPHORE_DEBUG
            qDebug() << QLatin1String("QSystemSemaphore::modify sem_wait failed") << count << errno;
#endif
            return false;
        }
    }
#endif // QT_HAVE_SEMAPHORE_H

    return true;
}

QT_END_NAMESPACE

#endif // QT_NO_SYSTEMSEMAPHORE
