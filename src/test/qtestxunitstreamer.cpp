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

#include "qtestxunitstreamer.h"
#include "qtestelement.h"
#include "qtestlog_p.h"
#include "qtestresult_p.h"
#include "qxmltestlogger_p.h"

QT_BEGIN_NAMESPACE

QTestXunitStreamer::QTestXunitStreamer()
    :QTestBasicStreamer()
{}

QTestXunitStreamer::~QTestXunitStreamer()
{}

void QTestXunitStreamer::indentForElement(const QTestElement* element, char* buf, int size)
{
    if (size == 0) return;

    buf[0] = 0;

    if (!element) return;

    char* endbuf = buf + size;
    element = element->parentElement();
    while (element && buf+2 < endbuf) {
        *(buf++) = ' ';
        *(buf++) = ' ';
        *buf = 0;
        element = element->parentElement();
    }
}

void QTestXunitStreamer::formatStart(const QTestElement *element, QTestCharBuffer *formatted) const
{
    if(!element || !formatted )
        return;

    char indent[20];
    indentForElement(element, indent, sizeof(indent));

    // Errors are written as CDATA within system-err, comments elsewhere
    if (element->elementType() == QTest::LET_Error) {
        if (element->parentElement()->elementType() == QTest::LET_SystemError) {
            QTest::qt_asprintf(formatted, "<![CDATA[");
        } else {
            QTest::qt_asprintf(formatted, "%s<!--", indent);
        }
        return;
    }

    QTest::qt_asprintf(formatted, "%s<%s", indent, element->elementName());
}

void QTestXunitStreamer::formatEnd(const QTestElement *element, QTestCharBuffer *formatted) const
{
    if (!element || !formatted )
        return;

    if (!element->childElements()){
        formatted->data()[0] = '\0';
        return;
    }

    char indent[20];
    indentForElement(element, indent, sizeof(indent));

    QTest::qt_asprintf(formatted, "%s</%s>\n", indent, element->elementName());
}

void QTestXunitStreamer::formatAttributes(const QTestElement* element, const QTestElementAttribute *attribute, QTestCharBuffer *formatted) const
{
    if(!attribute || !formatted )
        return;

    QTest::AttributeIndex attrindex = attribute->index();

    // For errors within system-err, we only want to output `message'
    if (element && element->elementType() == QTest::LET_Error
        && element->parentElement()->elementType() == QTest::LET_SystemError) {

        if (attrindex != QTest::AI_Description) return;

        QXmlTestLogger::xmlCdata(formatted, attribute->value());
        return;
    }

    char const* key = 0;
    if (attrindex == QTest::AI_Description)
        key = "message";
    else if (attrindex != QTest::AI_File && attrindex != QTest::AI_Line)
        key = attribute->name();

    if (key) {
        QTestCharBuffer quotedValue;
        QXmlTestLogger::xmlQuote(&quotedValue, attribute->value());
        QTest::qt_asprintf(formatted, " %s=\"%s\"", key, quotedValue.constData());
    } else {
        formatted->data()[0] = '\0';
    }
}

void QTestXunitStreamer::formatAfterAttributes(const QTestElement *element, QTestCharBuffer *formatted) const
{
    if(!element || !formatted )
        return;

    // Errors are written as CDATA within system-err, comments elsewhere
    if (element->elementType() == QTest::LET_Error) {
        if (element->parentElement()->elementType() == QTest::LET_SystemError) {
            QTest::qt_asprintf(formatted, "]]>\n");
        } else {
            QTest::qt_asprintf(formatted, " -->\n");
        }
        return;
    }

    if(!element->childElements())
        QTest::qt_asprintf(formatted, "/>\n");
    else
        QTest::qt_asprintf(formatted, ">\n");
}

void QTestXunitStreamer::output(QTestElement *element) const
{
    outputString("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    QTestBasicStreamer::output(element);
}

void QTestXunitStreamer::outputElements(QTestElement *element, bool) const
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

        if(element->elementType() != QTest::LET_Benchmark){
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
        }
        element = element->previousElement();
    }
}

QT_END_NAMESPACE

