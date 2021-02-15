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

#ifndef TOOL_WIDGETEDITOR_H
#define TOOL_WIDGETEDITOR_H

#include <QtDesigner/abstractformwindowtool.h>

#include <QtGui/qevent.h>
#include <QtCore/QPointer>

QT_BEGIN_NAMESPACE

class QAction;
class QMainWindow;

namespace qdesigner_internal {

class FormWindow;
class QDesignerMimeData;

class WidgetEditorTool: public QDesignerFormWindowToolInterface
{
    Q_OBJECT
public:
    explicit WidgetEditorTool(FormWindow *formWindow);
    virtual ~WidgetEditorTool();

    virtual QDesignerFormEditorInterface *core() const;
    virtual QDesignerFormWindowInterface *formWindow() const;
    virtual QWidget *editor() const;
    virtual QAction *action() const;

    virtual void activated();
    virtual void deactivated();

    virtual bool handleEvent(QWidget *widget, QWidget *managedWidget, QEvent *event);

    bool handleContextMenu(QWidget *widget, QWidget *managedWidget, QContextMenuEvent *e);
    bool handleMouseButtonDblClickEvent(QWidget *widget, QWidget *managedWidget, QMouseEvent *e);
    bool handleMousePressEvent(QWidget *widget, QWidget *managedWidget, QMouseEvent *e);
    bool handleMouseMoveEvent(QWidget *widget, QWidget *managedWidget, QMouseEvent *e);
    bool handleMouseReleaseEvent(QWidget *widget, QWidget *managedWidget, QMouseEvent *e);
    bool handleKeyPressEvent(QWidget *widget, QWidget *managedWidget, QKeyEvent *e);
    bool handleKeyReleaseEvent(QWidget *widget, QWidget *managedWidget, QKeyEvent *e);
    bool handlePaintEvent(QWidget *widget, QWidget *managedWidget, QPaintEvent *e);

    bool handleDragEnterMoveEvent(QWidget *widget, QWidget *managedWidget, QDragMoveEvent *e, bool isEnter);
    bool handleDragLeaveEvent(QWidget *widget, QWidget *managedWidget, QDragLeaveEvent *e);
    bool handleDropEvent(QWidget *widget, QWidget *managedWidget, QDropEvent *e);

private:
    bool restoreDropHighlighting();
    void detectDockDrag(const QDesignerMimeData *mimeData);

    FormWindow *m_formWindow;
    QAction *m_action;

    bool mainWindowSeparatorEvent(QWidget *widget, QEvent *event);
    QPointer<QMainWindow> m_separator_drag_mw;
    QPointer<QWidget> m_lastDropTarget;
    bool m_specialDockDrag;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // TOOL_WIDGETEDITOR_H
