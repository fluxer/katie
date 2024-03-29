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

#include "qtestresult_p.h"
#include "qtestlog_p.h"
#include "qtestdata.h"
#include "qtestassert.h"
#include "qcorecommon_p.h"

#include <stdio.h>
#include <string.h>

QT_BEGIN_NAMESPACE

namespace QTest
{
    static QTestData *currentTestData = 0;
    static QTestData *currentGlobalTestData = 0;
    static const char *currentTestFunc = 0;
    static const char *currentTestObjectName = 0;
    static bool failed = false;
    static bool dataFailed = false;
    static bool skipCurrentTest = false;
    static QTestResult::TestLocation location = QTestResult::NoWhere;

    static int fails = 0;
    static int passes = 0;
    static int skips = 0;

    static const char *expectFailComment = 0;
    static int expectFailMode = 0;

    static const char *currentApplicationName = 0;
}

void QTestResult::reset()
{
    QTest::currentTestData = 0;
    QTest::currentGlobalTestData = 0;
    QTest::currentTestFunc = 0;
    QTest::currentTestObjectName = 0;
    QTest::failed = false;
    QTest::dataFailed = false;
    QTest::location = QTestResult::NoWhere;

    QTest::fails = 0;
    QTest::passes = 0;
    QTest::skips = 0;

    QTest::expectFailComment = 0;
    QTest::expectFailMode = 0;
}

bool QTestResult::allDataPassed()
{
    return !QTest::failed;
}

bool QTestResult::currentTestFailed()
{
    return QTest::dataFailed;
}

QTestData *QTestResult::currentGlobalTestData()
{
    return QTest::currentGlobalTestData;
}

QTestData *QTestResult::currentTestData()
{
    return QTest::currentTestData;
}

void QTestResult::setCurrentGlobalTestData(QTestData *data)
{
    QTest::currentGlobalTestData = data;
}

void QTestResult::setCurrentTestData(QTestData *data)
{
    QTest::currentTestData = data;
    QTest::dataFailed = false;
}

void QTestResult::setCurrentTestFunction(const char *func)
{
    QTest::currentTestFunc = func;
    QTest::failed = false;
    if (!func)
        QTest::location = NoWhere;
    if (func)
        QTestLog::enterTestFunction(func);
}

static void clearExpectFail()
{
    QTest::expectFailMode = 0;
    delete [] const_cast<char *>(QTest::expectFailComment);
    QTest::expectFailComment = 0;
}

void QTestResult::finishedCurrentTestFunction()
{
    if (!QTest::failed && QTestLog::unhandledIgnoreMessages()) {
        QTestLog::printUnhandledIgnoreMessages();
        addFailure("Not all expected messages were received", 0, 0);
    }

    if (!QTest::failed && !QTest::skipCurrentTest) {
        QTestLog::addPass("");
        ++QTest::passes;
    }
    QTest::currentTestFunc = 0;
    QTest::failed = false;
    QTest::dataFailed = false;
    QTest::location = NoWhere;

    QTestLog::leaveTestFunction();

    clearExpectFail();
}

const char *QTestResult::currentTestFunction()
{
    return QTest::currentTestFunc;
}

const char *QTestResult::currentDataTag()
{
    return QTest::currentTestData ? QTest::currentTestData->dataTag()
                                   : static_cast<const char *>(0);
}

const char *QTestResult::currentGlobalDataTag()
{
    return QTest::currentGlobalTestData ? QTest::currentGlobalTestData->dataTag()
                                         : static_cast<const char *>(0);
}

static bool isExpectFailData(const char *dataIndex)
{
    if (!dataIndex || dataIndex[0] == '\0')
        return true;
    if (!QTest::currentTestData)
        return false;
    if (strcmp(dataIndex, QTest::currentTestData->dataTag()) == 0)
        return true;
    return false;
}

bool QTestResult::expectFail(const char *dataIndex, const char *comment,
                             QTest::TestFailMode mode, const char *file, int line)
{
    QTEST_ASSERT(comment);
    QTEST_ASSERT(mode > 0);

    if (!isExpectFailData(dataIndex)) {
        delete[] comment;
        return true; // we don't care
    }

    if (QTest::expectFailMode) {
        delete[] comment;
        clearExpectFail();
        addFailure("Already expecting a fail", file, line);
        return false;
    }

    QTest::expectFailMode = mode;
    QTest::expectFailComment = comment;
    return true;
}

static bool checkStatement(bool statement, const char *msg, const char *file, int line)
{
    if (statement) {
        if (QTest::expectFailMode) {
            QTestLog::addXPass(msg, file, line);
            bool doContinue = (QTest::expectFailMode == QTest::Continue);
            clearExpectFail();
            QTest::failed = true;
            ++QTest::fails;
            return doContinue;
        }
        return true;
    }

    if (QTest::expectFailMode) {
        QTestLog::addXFail(QTest::expectFailComment, file, line);
        bool doContinue = (QTest::expectFailMode == QTest::Continue);
        clearExpectFail();
        return doContinue;
    }

    QTestResult::addFailure(msg, file, line);
    return false;
}

bool QTestResult::verify(bool statement, const char *statementStr,
                         const char *description, const char *file, int line)
{
    QSTACKARRAY(char, msg, 1024);

    if (QTestLog::verboseLevel() >= 2) {
        QTest::qt_snprintf(msg, sizeof(msg), "QVERIFY(%s)", statementStr);
        QTestLog::info(msg, file, line);
    }

    QTest::qt_snprintf(msg, sizeof(msg), "'%s' returned FALSE. (%s)", statementStr, description);

    return checkStatement(statement, msg, file, line);
}

bool QTestResult::compare(bool success, const char *msg, const char *file, int line)
{
    if (QTestLog::verboseLevel() >= 2) {
        QTestLog::info(msg, file, line);
    }

    return checkStatement(success, msg, file, line);
}

bool QTestResult::compare(bool success, const char *msg, char *val1, char *val2,
                          const char *actual, const char *expected, const char *file, int line)
{
    QTEST_ASSERT(expected);
    QTEST_ASSERT(actual);

    if (!val1 && !val2)
        return compare(success, msg, file, line);

    QSTACKARRAY(char, buf, 1024);
    QTest::qt_snprintf(buf, sizeof(buf), "%s\n   Actual (%s): %s\n   Expected (%s): %s", msg,
                       actual, val1 ? val1 : "<null>",
                       expected, val2 ? val2 : "<null>");
    delete [] val1;
    delete [] val2;
    return compare(success, buf, file, line);
}

void QTestResult::addFailure(const char *message, const char *file, int line)
{
    clearExpectFail();

    QTestLog::addFail(message, file, line);
    QTest::failed = true;
    QTest::dataFailed = true;
    ++QTest::fails;
}

void QTestResult::addSkip(const char *message, QTest::SkipMode mode,
                          const char *file, int line)
{
    clearExpectFail();

    QTestLog::addSkip(message, mode, file, line);
    ++QTest::skips;
}

QTestResult::TestLocation QTestResult::currentTestLocation()
{
    return QTest::location;
}

void QTestResult::setCurrentTestLocation(TestLocation loc)
{
    QTest::location = loc;
}

void QTestResult::setCurrentTestObject(const char *name)
{
    QTest::currentTestObjectName = name;
}

const char *QTestResult::currentTestObjectName()
{
    return QTest::currentTestObjectName ? QTest::currentTestObjectName : "";
}

int QTestResult::passCount()
{
    return QTest::passes;
}

int QTestResult::failCount()
{
    return QTest::fails;
}

int QTestResult::skipCount()
{
    return QTest::skips;
}

void QTestResult::ignoreMessage(QtMsgType type, const char *msg)
{
    QTestLog::addIgnoreMessage(type, msg);
}

bool QTestResult::testFailed()
{
    return QTest::failed;
}

void QTestResult::setSkipCurrentTest(bool value)
{
    QTest::skipCurrentTest = value;
}

bool QTestResult::skipCurrentTest()
{
    return QTest::skipCurrentTest;
}

void QTestResult::setCurrentApplicationName(const char *appName)
{
    QTest::currentApplicationName = appName;
}
const char *QTestResult::currentApplicationName()
{
    return QTest::currentApplicationName;
}

QT_END_NAMESPACE
