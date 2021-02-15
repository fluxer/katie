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

#ifndef QDECLARATIVEPROPERTYVALUESOURCE_H
#define QDECLARATIVEPROPERTYVALUESOURCE_H

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE

class QDeclarativeProperty;

class Q_DECLARATIVE_EXPORT QDeclarativePropertyValueSource
{
public:
    QDeclarativePropertyValueSource();
    virtual ~QDeclarativePropertyValueSource();
    virtual void setTarget(const QDeclarativeProperty &) = 0;
};

QT_END_NAMESPACE

Q_DECLARE_INTERFACE(QDeclarativePropertyValueSource, "Katie.qml.QDeclarativePropertyValueSource")


#endif // QDECLARATIVEPROPERTYVALUESOURCE_H
