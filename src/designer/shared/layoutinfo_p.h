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

#ifndef LAYOUTINFO_H
#define LAYOUTINFO_H

#include "qglobal.h"

QT_BEGIN_NAMESPACE

class QWidget;
class QLayout;
class QLayoutItem;
class QDesignerFormEditorInterface;
class QFormLayout;
class QRect;
class QString;

namespace qdesigner_internal {

class Q_DESIGNER_EXPORT LayoutInfo
{
public:
    enum Type
    {
        NoLayout,
        HSplitter,
        VSplitter,
        HBox,
        VBox,
        Grid,
        Form,
        UnknownLayout // QDockWindow inside QMainWindow is inside QMainWindowLayout - it doesn't mean there is no layout
    };

    static void deleteLayout(const QDesignerFormEditorInterface *core, QWidget *widget);

    // Examines the immediate layout of the widget (will fail for Q3Group Box).
    static Type layoutType(const QDesignerFormEditorInterface *core, const QWidget *w);
    // Examines the managed layout of the widget
    static Type managedLayoutType(const QDesignerFormEditorInterface *core, const QWidget *w, QLayout **layout = 0);
    static Type layoutType(const QDesignerFormEditorInterface *core, const QLayout *layout);
    static Type layoutType(const QString &typeName);
    static QString layoutName(Type t);

    static QWidget *layoutParent(const QDesignerFormEditorInterface *core, QLayout *layout);

    static Type laidoutWidgetType(const QDesignerFormEditorInterface *core, QWidget *widget, bool *isManaged = 0, QLayout **layout = 0);
    static bool inline isWidgetLaidout(const QDesignerFormEditorInterface *core, QWidget *widget) { return laidoutWidgetType(core, widget) != NoLayout; }

    static QLayout *managedLayout(const QDesignerFormEditorInterface *core, const QWidget *widget);
    static QLayout *managedLayout(const QDesignerFormEditorInterface *core, QLayout *layout);

    // Is it a a dummy grid placeholder created by Designer?
    static bool isEmptyItem(QLayoutItem *item);
};

Q_DESIGNER_EXPORT void getFormLayoutItemPosition(const QFormLayout *formLayout, int index, int *rowPtr, int *columnPtr = 0, int *rowspanPtr = 0, int *colspanPtr = 0);
Q_DESIGNER_EXPORT void formLayoutAddWidget(QFormLayout *formLayout, QWidget *w, const QRect &r, bool insert);
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // LAYOUTINFO_H
