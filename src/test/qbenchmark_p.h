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

#ifndef QBENCHMARK_P_H
#define QBENCHMARK_P_H

#include <stdlib.h>

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

#include "qbenchmarkmeasurement_p.h"
#include "qmap.h"
#include "qtest_global.h"
#include "qbenchmarkevent_p.h"
#include "qbenchmarkmetric_p.h"

QT_BEGIN_NAMESPACE

struct QBenchmarkContext
{
    // None of the strings below are assumed to contain commas
    QString slotName;
    QString tag; // from _data() function
};

class QBenchmarkResult
{
public:
    QBenchmarkContext context;
    qreal value;
    int iterations;
    QTest::QBenchmarkMetric metric;
    bool setByMacro;
    bool valid;

    QBenchmarkResult()
    : value(-1)
    , iterations(-1)
    , metric(QTest::WalltimeMilliseconds)
    , setByMacro(true)
    , valid(false)
    { }

    QBenchmarkResult(
        const QBenchmarkContext &acontext, const qreal avalue, const int aiterations,
        QTest::QBenchmarkMetric ametric, bool asetByMacro)
        : context(acontext)
        , value(avalue)
        , iterations(aiterations)
        , metric(ametric)
        , setByMacro(asetByMacro)
        , valid(true)
    { }

    bool operator<(const QBenchmarkResult &other) const 
    {
        return (value / iterations) < (other.value / other.iterations);
    }
};

/*
    The QBenchmarkGlobalData class stores global benchmark-related data.
    QBenchmarkGlobalData:current is created at the beginning of qExec()
    and cleared at the end.
*/
class Q_TEST_EXPORT QBenchmarkGlobalData
{
public:
    static QBenchmarkGlobalData *current;

    QBenchmarkGlobalData();
    ~QBenchmarkGlobalData();
    void setMetric(QTest::QBenchmarkMetric metric);
    int adjustMedianIterationCount();

    QBenchmarkMeasurerBase *measurer;
    QBenchmarkContext context;
    int walltimeMinimum;
    int iterationCount;
    int medianIterationCount;
    bool verboseOutput;
};

/*
    The QBenchmarkTestMethodData class stores all benchmark-related data
    for the current test case. QBenchmarkTestMethodData:current is
    created at the beginning of qInvokeTestMethod() and cleared at
    the end.
*/
class Q_TEST_EXPORT QBenchmarkTestMethodData
{
public:
    static QBenchmarkTestMethodData *current;
    QBenchmarkTestMethodData();
    ~QBenchmarkTestMethodData();

    // Called once for each data row created by the _data function,
    // before and after calling the test function itself.
    void beginDataRun();
    void endDataRun();

    bool isBenchmark() const { return result.valid; }
    bool resultsAccepted() const { return resultAccepted; }
    void setResult(qreal value, QTest::QBenchmarkMetric metric, bool setByMacro = true);

    QBenchmarkResult result;
    bool resultAccepted;
    bool runOnce;
    int iterationCount;
};

QT_END_NAMESPACE

#endif // QBENCHMARK_H
