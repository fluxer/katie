/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#include <QtDesigner/abstractwidgetfactory.h>
#include "abstractformeditor.h"
#include "abstractwidgetdatabase.h"

QT_BEGIN_NAMESPACE

/*!
    \class QDesignerWidgetFactoryInterface
    \brief The QDesignerWidgetFactoryInterface class provides an interface that is used to control
    the widget factory used by Katie Designer.
    \inmodule QtDesigner
    \internal
*/

/*!
    \fn QDesignerWidgetFactoryInterface::QDesignerWidgetFactoryInterface(QObject *parent)

    Constructs an interface to a widget factory with the given \a parent.
*/
QDesignerWidgetFactoryInterface::QDesignerWidgetFactoryInterface(QObject *parent)
    : QObject(parent)
{
}

/*!
    \fn virtual QDesignerWidgetFactoryInterface::~QDesignerWidgetFactoryInterface()
*/
QDesignerWidgetFactoryInterface::~QDesignerWidgetFactoryInterface()
{
}

/*!
    \fn virtual QDesignerFormEditorInterface *QDesignerWidgetFactoryInterface::core() const = 0

    Returns the core form editor interface associated with this interface.
*/

/*!
    \fn virtual QWidget* QDesignerWidgetFactoryInterface::containerOfWidget(QWidget *child) const = 0

    Returns the widget that contains the specified \a child widget.
*/

/*!
    \fn virtual QWidget* QDesignerWidgetFactoryInterface::widgetOfContainer(QWidget *container) const = 0


*/

/*!
    \fn virtual QWidget *QDesignerWidgetFactoryInterface::createWidget(const QString &name, QWidget *parent) const = 0

    Returns a new widget with the given \a name and \a parent widget. If no parent is specified,
    the widget created will be a top-level widget.
*/

/*!
    \fn virtual QLayout *QDesignerWidgetFactoryInterface::createLayout(QWidget *widget, QLayout *layout, int type) const = 0

    Returns a new layout of the specified \a type for the given \a widget or \a layout.
*/

/*!
    \fn virtual bool QDesignerWidgetFactoryInterface::isPassiveInteractor(QWidget *widget) = 0
*/

/*!
    \fn virtual void QDesignerWidgetFactoryInterface::initialize(QObject *object) const = 0
*/

QT_END_NAMESPACE
#include "moc_abstractwidgetfactory.h"
