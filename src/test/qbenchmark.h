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

#ifndef QBENCHMARK_H
#define QBENCHMARK_H

#include <QtTest/qtest_global.h>
#include <QtTest/qbenchmarkmetric.h>


QT_BEGIN_NAMESPACE


namespace QTest
{

//
//  W A R N I N G
//  -------------
//
// The QBenchmarkIterationController class is not a part of the
// QTestlib API. It exists purely as an implementation detail.
//
//
class Q_TEST_EXPORT QBenchmarkIterationController
{
public:
    enum RunMode { RepeatUntilValidMeasurement, RunOnce };
    QBenchmarkIterationController();
    QBenchmarkIterationController(RunMode runMode);
    ~QBenchmarkIterationController();
    bool isDone();
    void next();
    int i;
};

}

// --- BEGIN public API ---

#define QBENCHMARK \
    for (QTest::QBenchmarkIterationController __iteration_controller; \
            __iteration_controller.isDone() == false; __iteration_controller.next())

#define QBENCHMARK_ONCE \
    for (QTest::QBenchmarkIterationController __iteration_controller(QTest::QBenchmarkIterationController::RunOnce); \
            __iteration_controller.isDone() == false; __iteration_controller.next())

namespace QTest
{
    void Q_TEST_EXPORT setBenchmarkResult(qreal result, QBenchmarkMetric metric);
}

// --- END public API ---

QT_END_NAMESPACE


#endif // QBENCHMARK_H
