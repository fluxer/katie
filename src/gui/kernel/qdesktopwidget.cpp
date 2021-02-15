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

#include "qglobal.h"
#include "qdesktopwidget.h"
#include "qwidget_p.h"

QT_BEGIN_NAMESPACE

const QRect QDesktopWidget::screenGeometry(const QWidget *widget) const
{
    if (!widget) {
        qWarning("QDesktopWidget::screenGeometry(): Attempt "
                 "to get the screen geometry of a null widget");
        return QRect();
    }
    const QRect rect = QWidgetPrivate::screenGeometry(widget);
    if (rect.isNull())
        return screenGeometry(screenNumber(widget));
    return rect;
}

const QRect QDesktopWidget::availableGeometry(const QWidget *widget) const
{
    if (!widget) {
        qWarning("QDesktopWidget::availableGeometry(): Attempt "
                 "to get the available geometry of a null widget");
        return QRect();
    }
    const QRect rect = QWidgetPrivate::screenGeometry(widget);
    if (rect.isNull())
        return availableGeometry(screenNumber(widget));
    return rect;
}

QT_END_NAMESPACE


#include "moc_qdesktopwidget.h"
