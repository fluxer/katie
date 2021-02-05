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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeclarativefocusscope_p.h"

#include "qdeclarativeitem_p.h"

QT_BEGIN_NAMESPACE

/*!
    \qmlclass FocusScope QDeclarativeFocusScope
    \since 4.7
    \ingroup qml-basic-interaction-elements

    \brief The FocusScope object explicitly creates a focus scope.
    \inherits Item

    Focus scopes assist in keyboard focus handling when building reusable QML
    components.  All the details are covered in the
    \l {qmlfocus}{keyboard focus documentation}.

    \sa {declarative/keyinteraction/focus}{Keyboard focus example}
*/

QDeclarativeFocusScope::QDeclarativeFocusScope(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    Q_D(QDeclarativeItem);
    d->flags |= QGraphicsItem::ItemIsFocusScope;
}

QDeclarativeFocusScope::~QDeclarativeFocusScope()
{
}
QT_END_NAMESPACE
#include "moc_qdeclarativefocusscope_p.h"
