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

#include "spacer_propertysheet.h"
#include "qdesigner_widget_p.h"
#include "formwindow.h"
#include "spacer_widget_p.h"

#include <QtDesigner/QExtensionManager>

#include <QtGui/QLayout>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal
{
SpacerPropertySheet::SpacerPropertySheet(Spacer *object, QObject *parent)
    : QDesignerPropertySheet(object, parent)
{
    clearFakeProperties();
}

SpacerPropertySheet::~SpacerPropertySheet()
{
}

bool SpacerPropertySheet::isVisible(int index) const
{
    static const QString spacerGroup = QLatin1String("Spacer");
    return propertyGroup(index) == spacerGroup;
}

void SpacerPropertySheet::setProperty(int index, const QVariant &value)
{
    QDesignerPropertySheet::setProperty(index, value);
}

bool SpacerPropertySheet::dynamicPropertiesAllowed() const
{
    return false;
}
}

QT_END_NAMESPACE
#include "moc_spacer_propertysheet.h"
