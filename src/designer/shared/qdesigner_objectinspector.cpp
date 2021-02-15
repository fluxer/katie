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

#include "qdesigner_objectinspector_p.h"

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

QDesignerObjectInspector::QDesignerObjectInspector(QWidget *parent, Qt::WindowFlags flags) :
    QDesignerObjectInspectorInterface(parent, flags)
{
}

void QDesignerObjectInspector::mainContainerChanged()
{
}

void Selection::clear()
{
    managed.clear();
    unmanaged.clear();
    objects.clear();
}

bool Selection::empty() const
{
    return managed.empty() && unmanaged.empty() && objects.empty();
}

QObjectList Selection::selection() const
{
    QObjectList rc(objects);
    foreach (QObject* o, managed)
        rc.push_back(o);
    foreach (QObject* o, unmanaged)
        rc.push_back(o);
    return rc;
}
}

QT_END_NAMESPACE
#include "moc_qdesigner_objectinspector_p.h"
