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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Katie Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef DYNAMICPROPERTYSHEET_H
#define DYNAMICPROPERTYSHEET_H

#include <QtDesigner/extension.h>


QT_BEGIN_NAMESPACE

class QString;

class QDesignerDynamicPropertySheetExtension
{
public:
    virtual ~QDesignerDynamicPropertySheetExtension() {}

    virtual bool dynamicPropertiesAllowed() const = 0;
    virtual int addDynamicProperty(const QString &propertyName, const QVariant &value) = 0;
    virtual bool removeDynamicProperty(int index) = 0;
    virtual bool isDynamicProperty(int index) const = 0;
    virtual bool canAddDynamicProperty(const QString &propertyName) const = 0;
};

QT_END_NAMESPACE

Q_DECLARE_EXTENSION_INTERFACE(QDesignerDynamicPropertySheetExtension, "Katie.Designer.DynamicPropertySheet")


#endif // DYNAMICPROPERTYSHEET_H
