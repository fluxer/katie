/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QT_NO_COLUMNVIEW

#include "qcolumnviewgrip_p.h"
#include "qstyleoption.h"
#include "qpainter.h"
#include "qbrush.h"
#include "qevent.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

/*
    \internal
    class QColumnViewGrip

    QColumnViewGrip is created to go inside QAbstractScrollArea's corner.
    When the mouse it moved it will resize the scroll area and emit's a signal.
 */

/*
    \internal
    \fn void QColumnViewGrip::gripMoved()
    Signal that is emitted when the grip moves the parent widget.
 */

/*!
    Creates a new QColumnViewGrip with the given \a parent to view a model.
    Use setModel() to set the model.
*/
QColumnViewGrip::QColumnViewGrip(QWidget *parent)
:  QWidget(*new QColumnViewGripPrivate, parent, 0)
{
#ifndef QT_NO_CURSOR
    setCursor(Qt::SplitHCursor);
#endif
}

/*!
  \internal
*/
QColumnViewGrip::QColumnViewGrip(QColumnViewGripPrivate & dd, QWidget *parent, Qt::WFlags f)
:  QWidget(dd, parent, f)
{
}

/*!
  Destroys the view.
*/
QColumnViewGrip::~QColumnViewGrip()
{
}

/*!
    Attempt to resize the parent object by \a offset
    returns the amount of offset that it was actually able to resized
*/
int QColumnViewGrip::moveGrip(int offset)
{
    QWidget *parentWidget = (QWidget*)parent();

    // first resize the parent
    int oldWidth = parentWidget->width();
    int newWidth = oldWidth;
    if (isRightToLeft())
       newWidth -= offset;
    else
       newWidth += offset;
    newWidth = qMax(parentWidget->minimumWidth(), newWidth);
    parentWidget->resize(newWidth, parentWidget->height());

    // Then have the view move the widget
    int realOffset = parentWidget->width() - oldWidth;
    int oldX = parentWidget->x();
    if (realOffset != 0)
        emit gripMoved(realOffset);
    if (isRightToLeft())
        realOffset = -1 * (oldX - parentWidget->x());
    return realOffset;
}

/*!
    \reimp
*/
void QColumnViewGrip::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QStyleOption opt;
    opt.initFrom(this);
    style()->drawControl(QStyle::CE_ColumnViewGrip, &opt, &painter, this);
    event->accept();
}

/*!
    \reimp
    Resize the parent window to the sizeHint
*/
void QColumnViewGrip::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget *parentWidget = (QWidget*)parent();
    int offset = parentWidget->sizeHint().width() - parentWidget->width();
    if (isRightToLeft())
        offset *= -1;
    moveGrip(offset);
    event->accept();
}

/*!
    \reimp
    Begin watching for mouse movements
*/
void QColumnViewGrip::mousePressEvent(QMouseEvent *event)
{
    Q_D(QColumnViewGrip);
    d->originalXLocation = event->globalX();
    event->accept();
}

/*!
    \reimp
    Calculate the movement of the grip and moveGrip() and emit gripMoved
*/
void QColumnViewGrip::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QColumnViewGrip);
    int offset = event->globalX() - d->originalXLocation;
    d->originalXLocation = moveGrip(offset) + d->originalXLocation;
    event->accept();
}

/*!
    \reimp
    Stop watching for mouse movements
*/
void QColumnViewGrip::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QColumnViewGrip);
    d->originalXLocation = -1;
    event->accept();
}

/*
 * private object implementation
 */
QColumnViewGripPrivate::QColumnViewGripPrivate()
:  QWidgetPrivate(),
originalXLocation(-1)
{
}

QT_END_NAMESPACE


#include "moc_qcolumnviewgrip_p.h"

#endif // QT_NO_COLUMNVIEW
