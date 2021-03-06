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

#include "qdeclarativepropertyvalueinterceptor.h"

#include "qdeclarative.h"

QT_BEGIN_NAMESPACE

/*!
    \class QDeclarativePropertyValueInterceptor
    \brief The QDeclarativePropertyValueInterceptor class is inherited by property interceptors such as Behavior.
    \internal

    This class intercepts property writes, allowing for custom handling. For example, Behavior uses this
    interception to provide a default animation for all changes to a property's value.
 */

/*!
    Constructs a QDeclarativePropertyValueInterceptor.
*/
QDeclarativePropertyValueInterceptor::QDeclarativePropertyValueInterceptor()
{
}

QDeclarativePropertyValueInterceptor::~QDeclarativePropertyValueInterceptor()
{
}

/*!
    \fn void QDeclarativePropertyValueInterceptor::setTarget(const QDeclarativeProperty &property)
    Set the target \a property for the value interceptor.  This method will
    be called by the QML engine when assigning a value interceptor.
*/

/*!
    \fn void QDeclarativePropertyValueInterceptor::write(const QVariant &value)
    This method will be called when a new \a value is assigned to the property being intercepted.
*/

QT_END_NAMESPACE
