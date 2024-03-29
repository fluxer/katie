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
#include "qtestassert.h"
#include "qtestlog_p.h"
#include "qplaintestlogger_p.h"
#include "qbenchmark_p.h"
#include "qbenchmarkmetric_p.h"
#include "qbytearray.h"
#include "qmath.h"
#include "qcorecommon_p.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

QT_BEGIN_NAMESPACE

namespace QTest {

# define COLORED_MSG(prefix, color, msg) colored && QAbstractTestLogger::isTtyOutput() ? "\033["#prefix";"#color"m" msg "\033[0m" : msg

    static const char *incidentType2String(QAbstractTestLogger::IncidentTypes type)
    {
        static bool colored = (!qgetenv("QTEST_COLORED").isEmpty());
        switch (type) {
        case QAbstractTestLogger::Pass:
            return COLORED_MSG(0, 32, "PASS  "); //green
        case QAbstractTestLogger::XFail:
            return COLORED_MSG(1, 32, "XFAIL "); //light green
        case QAbstractTestLogger::Fail:
            return COLORED_MSG(0, 31, "FAIL! "); //red
        case QAbstractTestLogger::XPass:
            return COLORED_MSG(0, 31, "XPASS "); //red, too
        }
        return "??????";
    }

    static const char *benchmarkResult2String()
    {
        static bool colored = (!qgetenv("QTEST_COLORED").isEmpty());
        return COLORED_MSG(0, 36, "RESULT   "); // cyan
    }

    static const char *messageType2String(QAbstractTestLogger::MessageTypes type)
    {
        static bool colored = (!qgetenv("QTEST_COLORED").isEmpty());
        switch (type) {
        case QAbstractTestLogger::Skip:
            return COLORED_MSG(0, 37, "SKIP  "); //white
        case QAbstractTestLogger::Warn:
            return COLORED_MSG(0, 33, "WARN  "); // yellow
        case QAbstractTestLogger::QWarning:
            return COLORED_MSG(1, 33, "QWARN ");
        case QAbstractTestLogger::QDebug:
            return COLORED_MSG(1, 33, "QDEBUG");
        case QAbstractTestLogger::QCritical:
            return COLORED_MSG(1, 33, "QCRIT ");
        case QAbstractTestLogger::QFatal:
            return COLORED_MSG(0, 31, "QFATAL"); // red
        case QAbstractTestLogger::Info:
            return "INFO  "; // no coloring
        }
        return "??????";
    }

    static void printMessage(const char *type, const char *msg, const char *file = 0, int line = 0)
    {
        QTEST_ASSERT(type);
        QTEST_ASSERT(msg);

        QTestCharBuffer buf;

        const char *fn = QTestResult::currentTestFunction() ? QTestResult::currentTestFunction()
            : "UnknownTestFunc";
        const char *tag = QTestResult::currentDataTag() ? QTestResult::currentDataTag() : "";
        const char *gtag = QTestResult::currentGlobalDataTag()
                         ? QTestResult::currentGlobalDataTag()
                         : "";
        const char *filler = (tag[0] && gtag[0]) ? ":" : "";
        if (file) {
            QTest::qt_asprintf(&buf, "%s: %s::%s(%s%s%s)%s%s\n"
                          "   Loc: [%s(%d)]\n"
                          , type, QTestResult::currentTestObjectName(), fn, gtag, filler, tag,
                          msg[0] ? " " : "", msg, file, line);
        } else {
            QTest::qt_asprintf(&buf, "%s: %s::%s(%s%s%s)%s%s\n",
                    type, QTestResult::currentTestObjectName(), fn, gtag, filler, tag,
                    msg[0] ? " " : "", msg);
        }
        // In colored mode, printf above stripped our nonprintable control characters.
        // Put them back.
        memcpy(buf.data(), type, strlen(type));
        QAbstractTestLogger::outputString(buf.data());
    }

    template <typename T>
    static int countSignificantDigits(T num)
    {
        if (num <= 0)
            return 0;

        int digits = 0;
        qreal divisor = 1;

        while (num / divisor >= 1) {
            divisor *= 10;
            ++digits;
        }

        return digits;
    }

    // Pretty-prints a benchmark result using the given number of digits.
    template <typename T> QString formatResult(T number, int significantDigits)
    {
        if (number < T(0))
            return QLatin1String("NAN");
        if (number == T(0))
            return QLatin1String("0");

        QString beforeDecimalPoint = QString::number(qint64(number), 'f', 0);
        QString afterDecimalPoint = QString::number(number, 'f', 20);
        afterDecimalPoint.remove(0, beforeDecimalPoint.count() + 1);

        int beforeUse = qMin(beforeDecimalPoint.count(), significantDigits);
        int beforeRemove = beforeDecimalPoint.count() - beforeUse;

        // Replace insignificant digits before the decimal point with zeros.
        beforeDecimalPoint.chop(beforeRemove);
        for (int i = 0; i < beforeRemove; ++i) {
            beforeDecimalPoint.append(QLatin1Char('0'));
        }

        int afterUse = significantDigits - beforeUse;

        // leading zeroes after the decimal point does not count towards the digit use.
        if (beforeDecimalPoint == QLatin1String("0") && afterDecimalPoint.isEmpty() == false) {
            ++afterUse;

            int i = 0;
            while (i < afterDecimalPoint.count() && afterDecimalPoint.at(i) == QLatin1Char('0')) {
                ++i;
            }

            afterUse += i;
        }

        int afterRemove = afterDecimalPoint.count() - afterUse;
        afterDecimalPoint.chop(afterRemove);

        QChar separator = QLatin1Char(',');
        QChar decimalPoint = QLatin1Char('.');

        // insert thousands separators
        int length = beforeDecimalPoint.length();
        for (int i = beforeDecimalPoint.length() -1; i >= 1; --i) {
            if ((length - i) % 3 == 0)
                beforeDecimalPoint.insert(i, separator);
        }

        QString print;
        print = beforeDecimalPoint;
        if (afterUse > 0)
            print.append(decimalPoint);

        print += afterDecimalPoint;


        return print;
    }

    template <typename T>
    int formatResult(char * buffer, int bufferSize, T number, int significantDigits)
    {
        QString result = formatResult(number, significantDigits);
        qstrncpy(buffer, result.toAscii().constData(), bufferSize);
        int size = result.count();
        return size;
    }

//    static void printBenchmarkResult(const char *bmtag, int value, int iterations)
    static void printBenchmarkResult(const QBenchmarkResult &result)
    {
        const char *bmtag = QTest::benchmarkResult2String();

        QSTACKARRAY(char, buf1, 1024);
        QTest::qt_snprintf(
            buf1, sizeof(buf1), "%s: %s::%s",
            bmtag,
            QTestResult::currentTestObjectName(),
            result.context.slotName.toAscii().data());

        QSTACKARRAY(char, bufTag, 1024);
        QByteArray tag = result.context.tag.toAscii();
        if (tag.isEmpty() == false) {
            QTest::qt_snprintf(bufTag, sizeof(bufTag), ":\"%s\"", tag.data());
        }


        QSTACKARRAY(char, fillFormat, 8);
        int fillLength = 5;
        QTest::qt_snprintf(
            fillFormat, sizeof(fillFormat), ":\n%%%ds", fillLength);
        QSTACKARRAY(char, fill, 1024);
        QTest::qt_snprintf(fill, sizeof(fill), fillFormat, "");

        const char * unitText = QTest::benchmarkMetricUnit(result.metric);

        qreal valuePerIteration = qreal(result.value) / qreal(result.iterations);
        QSTACKARRAY(char, resultBuffer, 100);
        formatResult(resultBuffer, sizeof(resultBuffer), valuePerIteration, countSignificantDigits(result.value));

        QSTACKARRAY(char, buf2, 1024);
        QTest::qt_snprintf(
            buf2, sizeof(buf2), "%s %s",
            resultBuffer,
            unitText);

        QSTACKARRAY(char, buf2_, 1024);
        QByteArray iterationText = " per iteration";
        Q_ASSERT(result.iterations > 0);
        QTest::qt_snprintf(
            buf2_,
            sizeof(buf2_), "%s",
            iterationText.data());

        QSTACKARRAY(char, buf3, 1024);
        Q_ASSERT(result.iterations > 0);
        formatResult(resultBuffer, sizeof(resultBuffer), result.value, countSignificantDigits(result.value));
        QTest::qt_snprintf(
            buf3, sizeof(buf3), " (total: %s, iterations: %d)",
            resultBuffer,
            result.iterations);

        QSTACKARRAY(char, buf, 1024);
        if (result.setByMacro) {
            QTest::qt_snprintf(
                buf, sizeof(buf), "%s%s%s%s%s%s\n", buf1, bufTag, fill, buf2, buf2_, buf3);
        } else {
            QTest::qt_snprintf(buf, sizeof(buf), "%s%s%s%s\n", buf1, bufTag, fill, buf2);
        }

        memcpy(buf, bmtag, strlen(bmtag));
        QAbstractTestLogger::outputString(buf);
    }
}

QPlainTestLogger::QPlainTestLogger()
: randomSeed(9), hasRandomSeed(false)
{
}

QPlainTestLogger::~QPlainTestLogger()
{
}

void QPlainTestLogger::startLogging()
{
    QAbstractTestLogger::startLogging();

    QSTACKARRAY(char, buf, 1024);
    if (QTestLog::verboseLevel() < 0) {
        QTest::qt_snprintf(buf, sizeof(buf), "Testing %s\n",
                           QTestResult::currentTestObjectName());
    } else {
        if (hasRandomSeed) {
            QTest::qt_snprintf(buf, sizeof(buf),
                             "********* Start testing of %s *********\n"
                             "Config: Using QTest library " QT_VERSION_STR
                             ", Katie %s, Random seed %d\n", QTestResult::currentTestObjectName(), qVersion(), randomSeed);
        } else {
            QTest::qt_snprintf(buf, sizeof(buf),
                             "********* Start testing of %s *********\n"
                             "Config: Using QTest library " QT_VERSION_STR
                             ", Katie %s\n", QTestResult::currentTestObjectName(), qVersion());
        }
    }
    QAbstractTestLogger::outputString(buf);
}

void QPlainTestLogger::stopLogging()
{
    QSTACKARRAY(char, buf, 1024);
    if (QTestLog::verboseLevel() < 0) {
        QTest::qt_snprintf(buf, sizeof(buf), "Totals: %d passed, %d failed, %d skipped\n",
                           QTestResult::passCount(), QTestResult::failCount(),
                           QTestResult::skipCount());
    } else {
        QTest::qt_snprintf(buf, sizeof(buf),
                         "Totals: %d passed, %d failed, %d skipped\n"
                         "********* Finished testing of %s *********\n",
                         QTestResult::passCount(), QTestResult::failCount(),
                         QTestResult::skipCount(), QTestResult::currentTestObjectName());
    }
    QAbstractTestLogger::outputString(buf);

    QAbstractTestLogger::stopLogging();
}


void QPlainTestLogger::enterTestFunction(const char * /*function*/)
{
    if (QTestLog::verboseLevel() >= 1)
        QTest::printMessage(QTest::messageType2String(Info), "entering");
}

void QPlainTestLogger::leaveTestFunction()
{
}

void QPlainTestLogger::addIncident(IncidentTypes type, const char *description,
                                   const char *file, int line)
{
    // suppress PASS in silent mode
    if (type == QAbstractTestLogger::Pass && QTestLog::verboseLevel() < 0)
        return;

    QTest::printMessage(QTest::incidentType2String(type), description, file, line);
}

void QPlainTestLogger::addBenchmarkResult(const QBenchmarkResult &result)
{
//    QTest::printBenchmarkResult(QTest::benchmarkResult2String(), value, iterations);
    QTest::printBenchmarkResult(result);
}

void QPlainTestLogger::addMessage(MessageTypes type, const char *message,
                                  const char *file, int line)
{
    // suppress PASS in silent mode
    if ((type == QAbstractTestLogger::Skip || type == QAbstractTestLogger::Info)
       && QTestLog::verboseLevel() < 0)
        return;

    QTest::printMessage(QTest::messageType2String(type), message, file, line);
}

void QPlainTestLogger::registerRandomSeed(unsigned int seed)
{
    randomSeed = seed;
    hasRandomSeed = true;
}

QT_END_NAMESPACE
