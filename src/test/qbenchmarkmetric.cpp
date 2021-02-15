/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qbenchmarkmetric_p.h"

QT_BEGIN_NAMESPACE

/*!
  \enum QTest::QBenchmarkMetric
  \since 4.7

  This enum lists all the things that can be benchmarked.
  
  \value WalltimeMilliseconds   Clock time in milliseconds
  \value CPUTicks               CPU time
  \value InstructionReads       Instruction reads
  \value Events                 Event count

  \sa QTest::benchmarkMetricName(), QTest::benchmarkMetricUnit()

 */

/*!
  \relates QTest
  \since 4.7
  Returns the enum value \a metric as a character string.
 */
const char * QTest::benchmarkMetricName(QBenchmarkMetric metric)
{
    switch (metric) {
        case WalltimeMilliseconds:
            return "WalltimeMilliseconds";
        case CPUTicks:
            return "CPUTicks";
        case InstructionReads:
            return "InstructionReads";
        case Events:
            return "Events";
    }
    return "";
}

/*!
  \relates QTest
  \since 4.7
  Retuns the units of measure for the specified \a metric.
 */
const char * QTest::benchmarkMetricUnit(QBenchmarkMetric metric)
{
    switch (metric) {
        case WalltimeMilliseconds:
            return "msecs";
        case CPUTicks:
            return "CPU ticks";
        case InstructionReads:
            return "instruction reads";
        case Events:
            return "events";
    }
    return "";
}

QT_END_NAMESPACE
