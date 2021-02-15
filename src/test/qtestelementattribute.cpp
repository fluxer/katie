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

#include "qtestelementattribute.h"
#include <QtCore/qbytearray.h>
#include <string.h>
#include <stdlib.h>

QT_BEGIN_NAMESPACE

/*! \enum QTest::AttributeIndex
  This enum numbers the different tests.

  \value AI_Undefined
        
  \value AI_Name
        
  \value AI_Result
        
  \value AI_Tests
        
  \value AI_Failures
        
  \value AI_Errors
        
  \value AI_Type
        
  \value AI_Description
        
  \value AI_PropertyValue
        
  \value AI_QTestVersion
        
  \value AI_QtVersion
        
  \value AI_File
        
  \value AI_Line
        
  \value AI_Metric
        
  \value AI_Tag
        
  \value AI_Value
        
  \value AI_Iterations
*/

/*! \enum QTest::LogElementType
  The enum specifies the kinds of test log messages.

  \value LET_Undefined

  \value LET_Property

  \value LET_Properties

  \value LET_Failure

  \value LET_Error

  \value LET_TestCase

  \value LET_TestSuite

  \value LET_Benchmark

  \value LET_SystemError
*/

QTestElementAttribute::QTestElementAttribute()
    :attributeValue(0),
    attributeIndex(QTest::AI_Undefined)
{
}

QTestElementAttribute::~QTestElementAttribute()
{
    delete[] attributeValue;
}

const char *QTestElementAttribute::value() const
{
    return attributeValue;
}

const char *QTestElementAttribute::name() const
{
    const char *AttributeNames[] =
    {
        "name",
        "result",
        "tests",
        "failures",
        "errors",
        "type",
        "description",
        "value",
        "qtestversion",
        "qtversion",
        "file",
        "line",
        "metric",
        "tag",
        "value",
        "iterations"
    };

    if(attributeIndex != QTest::AI_Undefined)
        return AttributeNames[attributeIndex];

    return 0;
}

QTest::AttributeIndex QTestElementAttribute::index() const
{
    return attributeIndex;
}

bool QTestElementAttribute::isNull() const
{
    return attributeIndex == QTest::AI_Undefined;
}

bool QTestElementAttribute::setPair(QTest::AttributeIndex index, const char *value)
{
    if(!value)
        return false;

    delete[] attributeValue;

    attributeIndex = index;
    attributeValue = qstrdup(value);

    return (attributeValue != 0);
}

QT_END_NAMESPACE

