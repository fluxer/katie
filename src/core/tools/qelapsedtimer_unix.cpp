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

#include "qelapsedtimer.h"

#include <sys/time.h>
#include <time.h>
#include <unistd.h>

QT_BEGIN_NAMESPACE

#if defined(_SC_MONOTONIC_CLOCK)
static const bool monotonicClockAvailable = (sysconf(_SC_MONOTONIC_CLOCK) >= 200112L);
#else
static const bool monotonicClockAvailable = (_POSIX_MONOTONIC_CLOCK > 0);
#endif

bool QElapsedTimer::isMonotonic()
{
    return monotonicClockAvailable;
}

QElapsedTimer::ClockType QElapsedTimer::clockType()
{
    if (Q_LIKELY(monotonicClockAvailable)) {
        return QElapsedTimer::MonotonicClock;
    }
    return QElapsedTimer::SystemTime;
}

static inline void do_gettime(qint64 *sec, qint64 *frac)
{
    struct timespec ts;
    if (Q_LIKELY(monotonicClockAvailable)) {
        ::clock_gettime(CLOCK_MONOTONIC, &ts);
    } else {
        ::clock_gettime(CLOCK_REALTIME, &ts);
    }
    *sec = ts.tv_sec;
    *frac = ts.tv_nsec;
}

// used in qcore_unix.cpp and qeventdispatcher_unix.cpp
timeval qt_gettime()
{
    qint64 sec, frac;
    do_gettime(&sec, &frac);

    timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = frac / 1000;
    return tv;
}

static inline qint64 elapsedAndRestart(qint64 sec, qint64 frac,
                                qint64 *nowsec, qint64 *nowfrac)
{
    do_gettime(nowsec, nowfrac);
    sec = *nowsec - sec;
    frac = *nowfrac - frac;
    return (sec * Q_INT64_C(1000000000) + frac) / Q_INT64_C(1000000);
}

void QElapsedTimer::start()
{
    do_gettime(&t1, &t2);
}

qint64 QElapsedTimer::restart()
{
    return elapsedAndRestart(t1, t2, &t1, &t2);
}

qint64 QElapsedTimer::nsecsElapsed() const
{
    qint64 sec, frac;
    do_gettime(&sec, &frac);
    sec = sec - t1;
    frac = frac - t2;
    return (sec * Q_INT64_C(1000000000) + frac);
}

qint64 QElapsedTimer::elapsed() const
{
    return (nsecsElapsed() / Q_INT64_C(1000000));
}

qint64 QElapsedTimer::msecsSinceReference() const
{
    return (t1 * Q_INT64_C(1000) + t2 / Q_INT64_C(1000000));
}

qint64 QElapsedTimer::msecsTo(const QElapsedTimer &other) const
{
    qint64 secs = other.t1 - t1;
    qint64 fraction = other.t2 - t2;
    return (secs * Q_INT64_C(1000000000) + fraction) / Q_INT64_C(1000000);;
}

qint64 QElapsedTimer::secsTo(const QElapsedTimer &other) const
{
    return other.t1 - t1;
}

bool operator<(const QElapsedTimer &v1, const QElapsedTimer &v2)
{
    return v1.t1 < v2.t1 || (v1.t1 == v2.t1 && v1.t2 < v2.t2);
}

QT_END_NAMESPACE
