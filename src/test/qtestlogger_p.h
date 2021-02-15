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

#ifndef QTESTLOGGER_P_H
#define QTESTLOGGER_P_H

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

#include "qabstracttestlogger_p.h"

QT_BEGIN_NAMESPACE

class QTestBasicStreamer;
class QTestElement;
class QTestFileLogger;

class QTestLogger : public QAbstractTestLogger
{
    public:
        enum TestLoggerFormat
        {
            TLF_XML = 0,
            TLF_LightXml = 1,
            TLF_XunitXml = 2
        };


        QTestLogger(TestLoggerFormat fm = TestLoggerFormat::TLF_XML);
        ~QTestLogger();

        void startLogging();
        void stopLogging();

        void enterTestFunction(const char *function);
        void leaveTestFunction();

        void addIncident(IncidentTypes type, const char *description,
                     const char *file = 0, int line = 0);
        void addBenchmarkResult(const QBenchmarkResult &result);
        void addTag(QTestElement* element);

        void addMessage(MessageTypes type, const char *message,
                    const char *file = 0, int line = 0);

        void registerRandomSeed(unsigned int seed);
        unsigned int randomSeed() const;
        bool hasRandomSeed() const;

    private:
        QTestElement *listOfTestcases;
        QTestElement *currentLogElement;
        QTestElement *errorLogElement;
        QTestBasicStreamer *logFormatter;
        TestLoggerFormat format;
        QTestFileLogger *filelogger;

        int testCounter;
        int passCounter;
        int failureCounter;
        int errorCounter;
        int skipCounter;
        unsigned int randomSeed_;
        bool hasRandomSeed_;
};

QT_END_NAMESPACE

#endif // QTESTLOGGER_P_H
