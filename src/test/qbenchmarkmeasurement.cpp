/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtTest module of the Katie Toolkit.
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

#include "qbenchmarkmeasurement_p.h"
#include "qbenchmark_p.h"
#include "qbenchmarkmetric_p.h"
#include "qbenchmark.h"
#include "qmath.h"
#include "qdebug.h"

#ifdef QT_HAVE_CLOCK_GETTIME
#  include <time.h>
#endif

QT_BEGIN_NAMESPACE

// QBenchmarkTimeMeasurer implementation

void QBenchmarkTimeMeasurer::start()
{
    time.start();
}

qint64 QBenchmarkTimeMeasurer::checkpoint()
{
    return time.elapsed();
}

qint64 QBenchmarkTimeMeasurer::stop()
{
    return time.elapsed();
}

bool QBenchmarkTimeMeasurer::isMeasurementAccepted(qint64 measurement)
{
    return (measurement > 50);
}

bool QBenchmarkTimeMeasurer::needsWarmupIteration()
{
    return true;
}

QTest::QBenchmarkMetric QBenchmarkTimeMeasurer::metricType()
{
    return QTest::WalltimeMilliseconds;
}

#ifdef QT_HAVE_CLOCK_GETTIME
static qint64 getticks()
{
    struct timespec ts;
    if (::clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts) == -1)
        return 0;
    return (ts.tv_sec * 1000000000) + ts.tv_nsec;
}

void QBenchmarkTickMeasurer::start()
{
    startTicks = getticks();
}

qint64 QBenchmarkTickMeasurer::checkpoint()
{
    return (getticks() - startTicks);
}

qint64 QBenchmarkTickMeasurer::stop()
{
    return (getticks() - startTicks);
}

bool QBenchmarkTickMeasurer::isMeasurementAccepted(qint64)
{
    return true;
}

bool QBenchmarkTickMeasurer::needsWarmupIteration()
{ 
    return true; 
}

QTest::QBenchmarkMetric QBenchmarkTickMeasurer::metricType()
{
    return QTest::CPUTicks;
}
#endif


QT_END_NAMESPACE
