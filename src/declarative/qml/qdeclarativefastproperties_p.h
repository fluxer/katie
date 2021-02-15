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

#ifndef QDECLARATIVEFASTPROPERTIES_P_H
#define QDECLARATIVEFASTPROPERTIES_P_H

#include <QtCore/qvector.h>
#include <QtCore/qhash.h>


QT_BEGIN_NAMESPACE

class QObject;
class QDeclarativeNotifierEndpoint;
class QDeclarativeFastProperties
{
public:
    typedef void (*Accessor)(QObject *object, void *output, QDeclarativeNotifierEndpoint *endpoint);

    QDeclarativeFastProperties();

    Accessor accessor(int index) const { return m_accessors.at(index); }
    int accessorIndexForProperty(const QMetaObject *, int);

private:
    void add(const QMetaObject *, int, Accessor);

    QHash<QPair<const QMetaObject *, int>, int> m_index;
    QVector<Accessor> m_accessors;
};

QT_END_NAMESPACE


#endif // QDECLARATIVEFASTPROPERTIES_P_H
