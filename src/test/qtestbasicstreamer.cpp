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

#include "qtestbasicstreamer.h"
#include "qtestlogger_p.h"
#include "qtestelement.h"
#include "qtestelementattribute.h"
#include "qtestlog_p.h"
#include "qtestassert.h"
#include "qtestcommon_p.h"
#include "qplatformdefs.h"

#include <unistd.h>

QT_BEGIN_NAMESPACE

QTestBasicStreamer::QTestBasicStreamer()
    :testLogger(0)
{
}

QTestBasicStreamer::~QTestBasicStreamer()
{}

void QTestBasicStreamer::formatStart(const QTestElement *element, QTestCharBuffer *formatted) const
{
    if(!element || !formatted )
        return;
    formatted->data()[0] = '\0';
}

void QTestBasicStreamer::formatEnd(const QTestElement *element, QTestCharBuffer *formatted) const
{
    if(!element || !formatted )
        return;
    formatted->data()[0] = '\0';
}

void QTestBasicStreamer::formatBeforeAttributes(const QTestElement *element, QTestCharBuffer *formatted) const
{
    if(!element || !formatted )
        return;
    formatted->data()[0] = '\0';
}

void QTestBasicStreamer::formatAfterAttributes(const QTestElement *element, QTestCharBuffer *formatted) const
{
    if(!element || !formatted )
        return;
    formatted->data()[0] = '\0';
}

void QTestBasicStreamer::formatAttributes(const QTestElement *, const QTestElementAttribute *attribute, QTestCharBuffer *formatted) const
{
    if(!attribute || !formatted )
        return;
    formatted->data()[0] = '\0';
}

void QTestBasicStreamer::output(QTestElement *element) const
{
    if(!element)
        return;

    outputElements(element);
}

void QTestBasicStreamer::outputElements(QTestElement *element, bool) const
{
    QTestCharBuffer buf;
    bool hasChildren;
    /*
        Elements are in reverse order of occurrence, so start from the end and work
        our way backwards.
    */
    while (element && element->nextElement()) {
        element = element->nextElement();
    }
    while (element) {
        hasChildren = element->childElements();

        formatStart(element, &buf);
        outputString(buf.data());

        formatBeforeAttributes(element, &buf);
        outputString(buf.data());

        outputElementAttributes(element, element->attributes());

        formatAfterAttributes(element, &buf);
        outputString(buf.data());

        if(hasChildren)
            outputElements(element->childElements(), true);

        formatEnd(element, &buf);
        outputString(buf.data());

        element = element->previousElement();
    }
}

void QTestBasicStreamer::outputElementAttributes(const QTestElement* element, QTestElementAttribute *attribute) const
{
    QTestCharBuffer buf;
    while(attribute){
        formatAttributes(element, attribute, &buf);
        outputString(buf.data());
        attribute = attribute->nextElement();
    }
}

void QTestBasicStreamer::outputString(const char *msg) const
{
    QTEST_ASSERT(QTest::stream);

    ::fputs(msg, QTest::stream);
    ::fflush(QTest::stream);
}

void QTestBasicStreamer::startStreaming()
{
    QTEST_ASSERT(!QTest::stream);

    const char *out = QTestLog::outputFileName();
    if (!out) {
        QTest::stream = stdout;
        return;
    }
    QTest::stream = QT_FOPEN(out, "wt");
    if (!QTest::stream) {
        printf("Unable to open file for logging: %s", out);
        ::exit(1);
    }
}

bool QTestBasicStreamer::isTtyOutput()
{
    QTEST_ASSERT(QTest::stream);

    static bool ttyoutput = ::isatty(QT_FILENO(QTest::stream));
    return ttyoutput;
}

void QTestBasicStreamer::stopStreaming()
{
    QTEST_ASSERT(QTest::stream);
    if (QTest::stream != stdout)
        ::fclose(QTest::stream);

    QTest::stream = 0;
}

void QTestBasicStreamer::setLogger(const QTestLogger *tstLogger)
{
    testLogger = tstLogger;
}

const QTestLogger *QTestBasicStreamer::logger() const
{
    return testLogger;
}

QT_END_NAMESPACE

