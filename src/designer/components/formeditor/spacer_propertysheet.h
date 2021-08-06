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

#ifndef SPACER_PROPERTYSHEET_H
#define SPACER_PROPERTYSHEET_H

#include "qdesigner_propertysheet_p.h"
#include <extensionfactory_p.h>
#include <spacer_widget_p.h>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class SpacerPropertySheet: public QDesignerPropertySheet
{
    Q_OBJECT
    Q_INTERFACES(QDesignerPropertySheetExtension)
public:
    explicit SpacerPropertySheet(Spacer *object, QObject *parent = nullptr);
    virtual ~SpacerPropertySheet();

    virtual void setProperty(int index, const QVariant &value);
    virtual bool isVisible(int index) const;

    virtual bool dynamicPropertiesAllowed() const;
};

typedef QDesignerPropertySheetFactory<Spacer, SpacerPropertySheet> SpacerPropertySheetFactory;
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // SPACER_PROPERTYSHEET_H
