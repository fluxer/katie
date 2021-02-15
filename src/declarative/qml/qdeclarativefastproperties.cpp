/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#include "qdeclarativefastproperties_p.h"

#include "qdeclarativeitem_p.h"

QT_BEGIN_NAMESPACE

// Adding entries to the QDeclarativeFastProperties class allows the QML 
// binding optimizer to bypass Qt's meta system and read and, more 
// importantly, subscribe to properties directly.  Any property that is
// primarily read from bindings is a candidate for inclusion as a fast 
// property.

static void QObject_objectName(QObject *object, void *output, QDeclarativeNotifierEndpoint *endpoint)
{
    if (endpoint) 
        endpoint->connect(QDeclarativeData::get(object, true)->objectNameNotifier());
    *((QString *)output) = object->objectName();
}

QDeclarativeFastProperties::QDeclarativeFastProperties()
{
    add(&QDeclarativeItem::staticMetaObject, QDeclarativeItem::staticMetaObject.indexOfProperty("parent"),
        QDeclarativeItemPrivate::parentProperty);
    add(&QObject::staticMetaObject, QObject::staticMetaObject.indexOfProperty("objectName"),
        QObject_objectName);    
}

int QDeclarativeFastProperties::accessorIndexForProperty(const QMetaObject *metaObject, int propertyIndex)
{
    Q_ASSERT(metaObject);
    Q_ASSERT(propertyIndex >= 0);

    // Find the "real" metaObject
    while (metaObject->propertyOffset() > propertyIndex) 
        metaObject = metaObject->superClass();

    QHash<QPair<const QMetaObject *, int>, int>::Iterator iter = 
        m_index.find(qMakePair(metaObject, propertyIndex));
    if (iter != m_index.end())
        return *iter;
    else
        return -1;
}

void QDeclarativeFastProperties::add(const QMetaObject *metaObject, int propertyIndex, Accessor accessor)
{
    Q_ASSERT(metaObject);
    Q_ASSERT(propertyIndex >= 0);

    // Find the "real" metaObject
    while (metaObject->propertyOffset() > propertyIndex) 
        metaObject = metaObject->superClass();

    QPair<const QMetaObject *, int> data = qMakePair(metaObject, propertyIndex);
    int accessorIndex = m_accessors.count();
    m_accessors.append(accessor);
    m_index.insert(data, accessorIndex);
}

QT_END_NAMESPACE
