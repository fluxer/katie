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

#include "qtconcurrentiteratekernel.h"

#if defined(Q_OS_HURD)
#include <sys/time.h>
#endif
#include <time.h>
#include <unistd.h>

#ifndef QT_NO_CONCURRENT

QT_BEGIN_NAMESPACE

enum {
    MedianSize = 7
};

static qint64 getticks()
{
    clockid_t clockId = CLOCK_REALTIME;

#if defined(_SC_THREAD_CPUTIME) && defined(CLOCK_THREAD_CPUTIME_ID)
    // detect availablility of CLOCK_THREAD_CPUTIME_ID,
    // sysconf() will return either -1 or _POSIX_VERSION (don't care about thread races here)
    static long useThreadCpuTime = sysconf(_SC_THREAD_CPUTIME);
    if (useThreadCpuTime != -1)
        clockId = CLOCK_THREAD_CPUTIME_ID;
#endif

    struct timespec ts;
    if (::clock_gettime(clockId, &ts) == -1)
        return 0;
    return (ts.tv_sec * 1000000000) + ts.tv_nsec;
}

static inline double elapsed(qint64 after, qint64 before)
{
    return double(after - before);
}

namespace QtConcurrent {

/*! \internal

*/
BlockSizeManager::BlockSizeManager(int iterationCount)
: maxBlockSize(iterationCount / (QThreadPool::globalInstance()->maxThreadCount() * 2)),
  beforeUser(0), afterUser(0),
  controlPartElapsed(MedianSize), userPartElapsed(MedianSize),
  m_blockSize(1)
{ }

// Records the time before user code.
void BlockSizeManager::timeBeforeUser()
{
    if (blockSizeMaxed())
        return;

    beforeUser = getticks();
    controlPartElapsed.addValue(elapsed(beforeUser, afterUser));
}

 // Records the time after user code and adjust the block size if we are spending
 // to much time in the for control code compared with the user code.
void BlockSizeManager::timeAfterUser()
{
    if (blockSizeMaxed())
        return;

    afterUser = getticks();
    userPartElapsed.addValue(elapsed(afterUser, beforeUser));

    if (controlPartElapsed.isMedianValid() == false)
        return;

    if (controlPartElapsed.median() * 100.0 < userPartElapsed.median())
        return;

    m_blockSize = qMin(m_blockSize * 2,  maxBlockSize);

#ifdef QTCONCURRENT_FOR_DEBUG
    qDebug() << QThread::currentThread() << "adjusting block size" << controlPartElapsed.median() << userPartElapsed.median() << m_blockSize;
#endif

    // Reset the medians after adjusting the block size so we get
    // new measurements with the new block size.
    controlPartElapsed.reset();
    userPartElapsed.reset();
}

int BlockSizeManager::blockSize()
{
    return m_blockSize;
}

} // namespace QtConcurrent

QT_END_NAMESPACE

#endif // QT_NO_CONCURRENT
