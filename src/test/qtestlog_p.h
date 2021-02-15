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

#ifndef QTESTLOG_P_H
#define QTESTLOG_P_H

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

#include <QtTest/qtest_global.h>

QT_BEGIN_NAMESPACE

class QBenchmarkResult;

class Q_TEST_EXPORT QTestLog
{
public:
    enum LogMode { Plain = 0, XML, LightXML, XunitXML };
    enum FlushMode { NoFlush = 0, FLushOn };

    static void enterTestFunction(const char* function);
    static void leaveTestFunction();

    static void addPass(const char *msg);
    static void addFail(const char *msg, const char *file, int line);
    static void addXFail(const char *msg, const char *file, int line);
    static void addXPass(const char *msg, const char *file, int line);
    static void addSkip(const char *msg, QTest::SkipMode mode,
                        const char *file, int line);
    static void addBenchmarkResult(const QBenchmarkResult &result);
    static void addIgnoreMessage(QtMsgType type, const char *msg);
    static int unhandledIgnoreMessages();
    static void printUnhandledIgnoreMessages();

    static void warn(const char *msg);
    static void info(const char *msg, const char *file, int line);

    static void startLogging();
    static void startLogging(unsigned int randomSeed);
    static void stopLogging();

    static void setLogMode(LogMode mode);
    static LogMode logMode();

    static void setVerboseLevel(int level);
    static int verboseLevel();

    static void redirectOutput(const char *fileName);
    static const char *outputFileName();

    static void setMaxWarnings(int max);

    static void setFlushMode(FlushMode mode);
private:
    QTestLog();
    ~QTestLog();
};

QT_END_NAMESPACE

#endif
