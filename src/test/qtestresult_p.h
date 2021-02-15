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

#ifndef QTESTRESULT_P_H
#define QTESTRESULT_P_H

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

#include "qtest_global.h"

QT_BEGIN_NAMESPACE

class QTestResultPrivate;
class QTestData;

class Q_TEST_EXPORT QTestResult
{
public:
    enum TestLocation { NoWhere = 0, DataFunc = 1, InitFunc = 2, Func = 3, CleanupFunc = 4 };

    static const char *currentTestObjectName();
    static bool currentTestFailed();
    static bool allDataPassed();
    static QTestData *currentTestData();
    static QTestData *currentGlobalTestData();
    static const char *currentTestFunction();
    static TestLocation currentTestLocation();
    static const char *currentDataTag();
    static const char *currentGlobalDataTag();
    static void finishedCurrentTestFunction();
    static void reset();

    static int passCount();
    static int failCount();
    static int skipCount();

    static void ignoreMessage(QtMsgType type, const char *msg);

    static void addFailure(const char *message, const char *file, int line);
    static bool compare(bool success, const char *msg, const char *file, int line);
    static bool compare(bool success, const char *msg, char *val1, char *val2,
                        const char *actual, const char *expected, const char *file, int line);

    static void setCurrentGlobalTestData(QTestData *data);
    static void setCurrentTestData(QTestData *data);
    static void setCurrentTestFunction(const char *func);
    static void setCurrentTestLocation(TestLocation loc);
    static void setCurrentTestObject(const char *name);
    static void addSkip(const char *message, QTest::SkipMode mode,
                        const char *file, int line);
    static bool expectFail(const char *dataIndex, const char *comment,
                           QTest::TestFailMode mode, const char *file, int line);
    static bool verify(bool statement, const char *statementStr, const char *extraInfo,
                       const char *file, int line);
    static bool testFailed();
    static void setSkipCurrentTest(bool value);
    static bool skipCurrentTest();

    static void setCurrentApplicationName(const char *appName);
    static const char *currentApplicationName();

private:
    Q_DISABLE_COPY(QTestResult)
};

QT_END_NAMESPACE

#endif
