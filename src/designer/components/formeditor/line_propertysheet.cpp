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

#include "line_propertysheet.h"
#include "formwindow.h"

// sdk
#include <QtDesigner/QExtensionManager>

#include <QtGui/QLayout>
#include <QtCore/QMetaObject>
#include <QtCore/qmetaobject.h>
#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

using namespace qdesigner_internal;

LinePropertySheet::LinePropertySheet(Line *object, QObject *parent)
    : QDesignerPropertySheet(object, parent)
{
    clearFakeProperties();
}

LinePropertySheet::~LinePropertySheet()
{
}

bool LinePropertySheet::isVisible(int index) const
{
    const QString name = propertyName(index);

    if (name == QLatin1String("frameShape"))
        return false;
    return QDesignerPropertySheet::isVisible(index);
}

void LinePropertySheet::setProperty(int index, const QVariant &value)
{
    QDesignerPropertySheet::setProperty(index, value);
}

QString LinePropertySheet::propertyGroup(int index) const
{
    return QDesignerPropertySheet::propertyGroup(index);
}

QT_END_NAMESPACE
#include "moc_line_propertysheet.h"
