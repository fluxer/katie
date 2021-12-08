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

#include "qbenchmark.h"
#include "qbenchmark_p.h"
#include "qbenchmarkmetric_p.h"

#ifdef QT_GUI_LIB
#include "qapplication.h"
#endif

#include "qprocess.h"
#include "qdir.h"
#include "qset.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

QBenchmarkGlobalData *QBenchmarkGlobalData::current;

QBenchmarkGlobalData::QBenchmarkGlobalData()
    : measurer(nullptr)
    , walltimeMinimum(-1)
    , iterationCount(-1)
    , medianIterationCount(1)
    , verboseOutput(false)
{
    setMetric(QTest::QBenchmarkMetric::WalltimeMilliseconds);
}

QBenchmarkGlobalData::~QBenchmarkGlobalData()
{
    delete measurer;
    QBenchmarkGlobalData::current = 0;
}

void QBenchmarkGlobalData::setMetric(QTest::QBenchmarkMetric metric)
{
    if (measurer)
        delete measurer;

    if (metric == QTest::QBenchmarkMetric::CPUTicks) {
        measurer = new QBenchmarkTickMeasurer;
    } else if (metric == QTest::QBenchmarkMetric::Events) {
        measurer = new QBenchmarkEvent;
    } else {
        measurer = new QBenchmarkTimeMeasurer;
    }
}

int QBenchmarkGlobalData::adjustMedianIterationCount()
{
    return medianIterationCount;
}


QBenchmarkTestMethodData *QBenchmarkTestMethodData::current;

QBenchmarkTestMethodData::QBenchmarkTestMethodData()
:resultAccepted(false), runOnce(false), iterationCount(-1)
{
   
}

QBenchmarkTestMethodData::~QBenchmarkTestMethodData()
{
    QBenchmarkTestMethodData::current = 0;
}

void QBenchmarkTestMethodData::beginDataRun()
{
    // Let the -iterations option override the measurer.
    if (QBenchmarkGlobalData::current->iterationCount != -1) {
        iterationCount = QBenchmarkGlobalData::current->iterationCount;
    } else {
        iterationCount = 1;
    }
}

void QBenchmarkTestMethodData::endDataRun()
{

}

void QBenchmarkTestMethodData::setResult(
    qreal value, QTest::QBenchmarkMetric metric, bool setByMacro)
{
    bool accepted = false;

    // Always accept the result if the iteration count has been
    // specified on the command line with -iterations.
    if (QBenchmarkGlobalData::current->iterationCount != -1)
        accepted = true;

    else if (QBenchmarkTestMethodData::current->runOnce || !setByMacro) {
        iterationCount = 1;
        accepted = true;
    }

    // Test the result directly without calling the measurer if the minimum time 
    // has been specified on the command line with -minimumvalue.
    else if (QBenchmarkGlobalData::current->walltimeMinimum != -1)
        accepted = (value > QBenchmarkGlobalData::current->walltimeMinimum);
    else
        accepted = QBenchmarkGlobalData::current->measurer->isMeasurementAccepted(value);

    // Accept the result or double the number of iterations.
    if (accepted)
        resultAccepted = true;
    else
        iterationCount *= 2;

    this->result = QBenchmarkResult(
        QBenchmarkGlobalData::current->context, value, iterationCount, metric, setByMacro);
}

/*!
    \class QTest::QBenchmarkIterationController
    \internal

    The QBenchmarkIterationController class is used by the QBENCHMARK macro to
    drive the benchmarking loop. It is repsonsible for starting and stopping
    the timing measurements as well as calling the result reporting functions.
*/

/*! \internal
*/
QTest::QBenchmarkIterationController::QBenchmarkIterationController(RunMode runMode)
{
    i = 0;
    if (runMode == RunOnce)
        QBenchmarkTestMethodData::current->runOnce = true;
    QBenchmarkGlobalData::current->measurer->start();
    // the clock is ticking after the line above, don't add code here.
}

QTest::QBenchmarkIterationController::QBenchmarkIterationController()
{
    i = 0;
    QBenchmarkGlobalData::current->measurer->start();
    // the clock is ticking after the line above, don't add code here.
}

/*! \internal
*/
QTest::QBenchmarkIterationController::~QBenchmarkIterationController()
{
    // the clock is ticking before the line below, don't add code here.
    const qreal result = QBenchmarkGlobalData::current->measurer->stop();
    QBenchmarkTestMethodData::current->setResult(result, QBenchmarkGlobalData::current->measurer->metricType());
}

/*! \internal
*/
bool QTest::QBenchmarkIterationController::isDone()
{
    if (QBenchmarkTestMethodData::current->runOnce)
        return i > 0;
    return i >= QBenchmarkTestMethodData::current->iterationCount;
}

/*! \internal
*/
void QTest::QBenchmarkIterationController::next()
{
    ++i;
}

/*!
    Sets the benchmark result for this test function to \a result.
 
    Use this function if you want to report benchmark results without
    using the QBENCHMARK macro. Use \a metric to specify how QTestLib
    should interpret the results.
 
    The context for the result will be the test function name and any
    data tag from the _data function. This function can only be called
    once in each test function, subsequent calls will replace the
    earlier reported results.

    Note that the -iterations command line argument has no effect
    on test functions without the QBENCHMARK macro.

    \since 4.7
*/
void QTest::setBenchmarkResult(qreal result, QTest::QBenchmarkMetric metric)
{
    QBenchmarkTestMethodData::current->setResult(result, metric, false);
}

QT_END_NAMESPACE
