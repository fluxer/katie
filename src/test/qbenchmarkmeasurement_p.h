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

#ifndef QBENCHMARK_MEASUREMENT_P_H
#define QBENCHMARK_MEASUREMENT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qelapsedtimer.h"
#include "qbenchmark.h"

QT_BEGIN_NAMESPACE

class QBenchmarkMeasurerBase
{
public:
    virtual ~QBenchmarkMeasurerBase(){};
    virtual void start() = 0;
    virtual qint64 checkpoint() = 0;
    virtual qint64 stop() = 0;
    virtual bool isMeasurementAccepted(qint64 measurement) = 0;
    virtual bool needsWarmupIteration() { return false; } 
    virtual QTest::QBenchmarkMetric metricType() = 0;
};

class QBenchmarkTimeMeasurer : public QBenchmarkMeasurerBase
{
public:
    void start();
    qint64 checkpoint();
    qint64 stop();
    bool isMeasurementAccepted(qint64 measurement);
    bool needsWarmupIteration();
    QTest::QBenchmarkMetric metricType();
private:
    QElapsedTimer time;
};

class QBenchmarkTickMeasurer : public QBenchmarkMeasurerBase
{
public:
    void start();
    qint64 checkpoint();
    qint64 stop();
    bool isMeasurementAccepted(qint64 measurement);
    bool needsWarmupIteration();
    QTest::QBenchmarkMetric metricType();
private:
    qint64 startTicks;
};

QT_END_NAMESPACE

#endif // QBENCHMARK_MEASUREMENT_P_H
