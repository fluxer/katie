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

#include "formwindow_dnditem.h"
#include "formwindow.h"

#include <ui4.h>
#include "qdesigner_resource.h"
#include "qtresourcemodel_p.h"

#include <QtDesigner/abstractformeditor.h>

#include <QtGui/QLabel>
#include <QtGui/QPixmap>

QT_BEGIN_NAMESPACE

using namespace qdesigner_internal;

static QWidget *decorationFromWidget(QWidget *w)
{
    QLabel *label = new QLabel(0, Qt::ToolTip);
    QPixmap pm = QPixmap::grabWidget(w);
    label->setPixmap(pm);
    label->resize(pm.size());

    return label;
}

static DomUI *widgetToDom(QWidget *widget, FormWindow *form)
{
    QDesignerResource builder(form);
    builder.setSaveRelative(false);
    return builder.copy(FormBuilderClipboard(widget));
}

FormWindowDnDItem::FormWindowDnDItem(QDesignerDnDItemInterface::DropType type, FormWindow *form,
                                        QWidget *widget, const QPoint &global_mouse_pos)
    : QDesignerDnDItem(type, form)
{
    QWidget *decoration = decorationFromWidget(widget);
    QPoint pos = widget->mapToGlobal(QPoint(0, 0));
    decoration->move(pos);

    init(0, widget, decoration, global_mouse_pos);
}

DomUI *FormWindowDnDItem::domUi() const
{
    DomUI *result = QDesignerDnDItem::domUi();
    if (result != 0)
        return result;
    FormWindow *form = qobject_cast<FormWindow*>(source());
    if (widget() == 0 || form == 0)
        return 0;

    QtResourceModel *resourceModel = form->core()->resourceModel();
    QtResourceSet *currentResourceSet = resourceModel->currentResourceSet();
    /* Short:
     *   We need to activate the original resourceSet associated with a form
     *   to properly generate the dom resource includes.
     * Long:
     *   widgetToDom() calls copy() on QDesignerResource. It generates the
     *   Dom structure. In order to create DomResources properly we need to
     *   have the associated ResourceSet active (QDesignerResource::saveResources()
     *   queries the resource model for a qrc path for the given resource file:
     *      qrcFile = m_core->resourceModel()->qrcPath(ri->text());
     *   This works only when the resource file comes from the active
     *   resourceSet */
    resourceModel->setCurrentResourceSet(form->resourceSet());

    result = widgetToDom(widget(), form);
    const_cast<FormWindowDnDItem*>(this)->setDomUi(result);
    resourceModel->setCurrentResourceSet(currentResourceSet);
    return result;
}

QT_END_NAMESPACE
