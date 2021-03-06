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

#include "qdeclarativefocuspanel_p.h"

#include "qdeclarativeitem_p.h"

#include <QtGui/qgraphicsscene.h>
#include <QEvent>

QT_BEGIN_NAMESPACE

/*!
   \qmlclass FocusPanel QDeclarativeFocusPanel
    \since 4.7
    \ingroup qml-basic-interaction-elements

   \brief The FocusPanel item explicitly creates a focus panel.
   \inherits Item

    Focus panels assist in keyboard focus handling when building QML
    applications.  All the details are covered in the 
    \l {qmlfocus}{keyboard focus documentation}.
*/

QDeclarativeFocusPanel::QDeclarativeFocusPanel(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    Q_D(QDeclarativeItem);
    d->flags |= QGraphicsItem::ItemIsPanel;
}

QDeclarativeFocusPanel::~QDeclarativeFocusPanel()
{
}

/*!
    \qmlproperty bool FocusPanel::active

    Sets whether the item is the active focus panel.
*/

bool QDeclarativeFocusPanel::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowActivate ||
        event->type() == QEvent::WindowDeactivate)
        emit activeChanged();
    return QDeclarativeItem::sceneEvent(event);
}

QT_END_NAMESPACE
#include "moc_qdeclarativefocuspanel_p.h"
