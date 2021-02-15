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

#include "qdeclarativepropertyvaluesource.h"

#include "qdeclarative.h"

QT_BEGIN_NAMESPACE

/*!
    \class QDeclarativePropertyValueSource
    \brief The QDeclarativePropertyValueSource class is an interface for property value sources such as animations and bindings.

    See \l{Property Value Sources} for information on writing custom property
    value sources.
 */

/*!
    Constructs a QDeclarativePropertyValueSource.
*/
QDeclarativePropertyValueSource::QDeclarativePropertyValueSource()
{
}

/*!
    Destroys the value source.
*/
QDeclarativePropertyValueSource::~QDeclarativePropertyValueSource()
{
}

/*!
    \fn void QDeclarativePropertyValueSource::setTarget(const QDeclarativeProperty &property)
    Set the target \a property for the value source.  This method will
    be called by the QML engine when assigning a value source.
*/

QT_END_NAMESPACE
