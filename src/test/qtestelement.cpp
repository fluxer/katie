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

#include "qtestelement.h"

QT_BEGIN_NAMESPACE

QTestElement::QTestElement(int type)
    :QTestCoreElement<QTestElement>(type),
    listOfChildren(0),
    parent(0)
{
}

QTestElement::~QTestElement()
{
    delete listOfChildren;
}

bool QTestElement::addLogElement(QTestElement *element)
{
    if(!element)
        return false;

    if(element->elementType() != QTest::LET_Undefined){
        element->addToList(&listOfChildren);
        element->setParent(this);
        return true;
    }

    return false;
}

QTestElement *QTestElement::childElements() const
{
    return listOfChildren;
}

const QTestElement *QTestElement::parentElement() const
{
    return parent;
}

void QTestElement::setParent(const QTestElement *p)
{
    parent = p;
}

QT_END_NAMESPACE

