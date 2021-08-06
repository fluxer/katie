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

#ifndef LAYOUT_PROPERTYSHEET_H
#define LAYOUT_PROPERTYSHEET_H

#include "qdesigner_propertysheet_p.h"
#include <extensionfactory_p.h>

#include <QtGui/QLayout>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class DomLayout;

namespace qdesigner_internal {

class LayoutPropertySheet: public QDesignerPropertySheet
{
    Q_OBJECT
    Q_INTERFACES(QDesignerPropertySheetExtension)
public:
    explicit LayoutPropertySheet(QLayout *object, QObject *parent = nullptr);
    virtual ~LayoutPropertySheet();

    virtual void setProperty(int index, const QVariant &value);
    virtual QVariant property(int index) const;
    virtual bool reset(int index);
    void setChanged(int index, bool changed);

    static void stretchAttributesToDom(QDesignerFormEditorInterface *core, QLayout *lt, DomLayout *domLayout);
    static void markChangedStretchProperties(QDesignerFormEditorInterface *core, QLayout *lt, const DomLayout *domLayout);

private:
    QLayout *m_layout;
};

typedef QDesignerPropertySheetFactory<QLayout, LayoutPropertySheet>  LayoutPropertySheetFactory;
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // LAYOUT_PROPERTYSHEET_H
