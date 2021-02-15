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

#ifndef WIDGETBOX_DNDITEM_H
#define WIDGETBOX_DNDITEM_H

#include "qdesigner_dnditem_p.h"
#include "widgetbox_global.h"

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class DomUI;

namespace qdesigner_internal {

class QT_WIDGETBOX_EXPORT WidgetBoxDnDItem : public QDesignerDnDItem
{
public:
    WidgetBoxDnDItem(QDesignerFormEditorInterface *core,
                     DomUI *dom_ui,
                     const QPoint &global_mouse_pos);
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // WIDGETBOX_DNDITEM_H
