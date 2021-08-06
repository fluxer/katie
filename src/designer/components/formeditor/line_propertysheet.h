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

#ifndef LINE_PROPERTYSHEET_H
#define LINE_PROPERTYSHEET_H

#include "qdesigner_propertysheet_p.h"
#include "qdesigner_widget_p.h"
#include <extensionfactory_p.h>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class LinePropertySheet: public QDesignerPropertySheet
{
    Q_OBJECT
    Q_INTERFACES(QDesignerPropertySheetExtension)
public:
    explicit LinePropertySheet(Line *object, QObject *parent = nullptr);
    virtual ~LinePropertySheet();

    virtual void setProperty(int index, const QVariant &value);
    virtual bool isVisible(int index) const;
    virtual QString propertyGroup(int index) const;
};

typedef QDesignerPropertySheetFactory<Line, LinePropertySheet> LinePropertySheetFactory;
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // LINE_PROPERTYSHEET_H
