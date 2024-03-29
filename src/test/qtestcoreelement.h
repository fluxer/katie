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

#ifndef QTESTCOREELEMENT_H
#define QTESTCOREELEMENT_H

#include <QtTest/qtestcorelist.h>
#include <QtTest/qtestelementattribute.h>


QT_BEGIN_NAMESPACE

template <class ElementType>
class Q_TEST_EXPORT QTestCoreElement: public QTestCoreList<ElementType>
{
public:
    QTestCoreElement( int type = -1 );
    virtual ~QTestCoreElement();

    void addAttribute(const QTest::AttributeIndex index, const char *value);
    QTestElementAttribute *attributes() const;
    const char *attributeValue(QTest::AttributeIndex index) const;
    const char *attributeName(QTest::AttributeIndex index) const;
    const QTestElementAttribute *attribute(QTest::AttributeIndex index) const;

    const char *elementName() const;
    QTest::LogElementType elementType() const;

private:
    QTestElementAttribute *listOfAttributes;
    QTest::LogElementType type;
};

template<class ElementType>
QTestCoreElement<ElementType>::QTestCoreElement(int t)
    : listOfAttributes(0), type(QTest::LogElementType(t))
{
}

template<class ElementType>
QTestCoreElement<ElementType>::~QTestCoreElement()
{
    delete listOfAttributes;
}

template <class ElementType>
void QTestCoreElement<ElementType>::addAttribute(const QTest::AttributeIndex attributeIndex, const char *value)
{
    if(attributeIndex == -1)
        return;

    if (attribute(attributeIndex))
        return;

    QTestElementAttribute *testAttribute = new QTestElementAttribute;
    testAttribute->setPair(attributeIndex, value);
    testAttribute->addToList(&listOfAttributes);
}

template <class ElementType>
QTestElementAttribute *QTestCoreElement<ElementType>::attributes() const
{
    return listOfAttributes;
}

template <class ElementType>
const char *QTestCoreElement<ElementType>::attributeValue(QTest::AttributeIndex index) const
{
    const QTestElementAttribute *attrb = attribute(index);
    if(attrb)
        return attrb->value();

    return 0;
}

template <class ElementType>
const char *QTestCoreElement<ElementType>::attributeName(QTest::AttributeIndex index) const
{
    const QTestElementAttribute *attrb = attribute(index);
    if(attrb)
        return attrb->name();

    return 0;
}

template <class ElementType>
const char *QTestCoreElement<ElementType>::elementName() const
{
    const char *xmlElementNames[] =
    {
        "property",
        "properties",
        "failure",
        "error",
        "testcase",
        "testsuite",
        "benchmark",
        "system-err"
    };

    if(type != QTest::LET_Undefined)
        return xmlElementNames[type];

    return 0;
}

template <class ElementType>
QTest::LogElementType QTestCoreElement<ElementType>::elementType() const
{
    return type;
}

template <class ElementType>
const QTestElementAttribute *QTestCoreElement<ElementType>::attribute(QTest::AttributeIndex index) const
{
    QTestElementAttribute *iterator = listOfAttributes;
    while(iterator){
        if(iterator->index() == index)
            return iterator;

        iterator = iterator->nextElement();
    }

    return 0;
}

QT_END_NAMESPACE


#endif
