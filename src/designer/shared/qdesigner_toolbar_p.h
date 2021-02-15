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

#ifndef QDESIGNER_TOOLBAR_H
#define QDESIGNER_TOOLBAR_H

#include <QtGui/QAction>
#include <QtGui/QToolButton>

#include <QtCore/QList>
#include <QtCore/QPoint>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;
class QToolBar;
class QRect;
class QAction;

namespace qdesigner_internal {

class PromotionTaskMenu;

// Special event filter for  tool bars in designer.
// Handles drag and drop to and from. Ensures that each
// child widget is  WA_TransparentForMouseEvents to enable  drag and drop.

class Q_DESIGNER_EXPORT ToolBarEventFilter : public QObject {
    Q_OBJECT

public:
    static void install(QToolBar *tb);

    // Find action by position. Note that QToolBar::actionAt() will
    // not work as designer sets WA_TransparentForMouseEvents on its tool bar buttons
    // to be able to drag them. This function will return the dummy
    // sentinel action when applied to tool bars created by designer if the position matches.
    static QAction *actionAt(const QToolBar *tb, const QPoint &pos);

    static bool withinHandleArea(const QToolBar *tb, const QPoint &pos);
    static QRect handleArea(const QToolBar *tb);
    static QRect freeArea(const QToolBar *tb);

    // Utility to create an action
    static QAction *createAction(QDesignerFormWindowInterface *fw, const QString &objectName, bool separator);

    virtual bool eventFilter (QObject *watched, QEvent *event);

    // Helper for task menu extension
    QList<QAction *> contextMenuActions(const QPoint &globalPos = QPoint(-1, -1));

    static ToolBarEventFilter *eventFilterOf(const QToolBar *tb);

private slots:
    void slotRemoveSelectedAction();
    void slotRemoveToolBar();
    void slotInsertSeparator();

private:
    explicit ToolBarEventFilter(QToolBar *tb);

    bool handleContextMenuEvent(QContextMenuEvent * event);
    bool handleDragEnterMoveEvent(QDragMoveEvent *event);
    bool handleDragLeaveEvent(QDragLeaveEvent *);
    bool handleDropEvent(QDropEvent *event);
    bool handleMousePressEvent(QMouseEvent *event);
    bool handleMouseReleaseEvent(QMouseEvent *event);
    bool handleMouseMoveEvent(QMouseEvent *event);

    QDesignerFormWindowInterface *formWindow() const;
    void adjustDragIndicator(const QPoint &pos);
    void hideDragIndicator();
    void startDrag(const QPoint &pos, Qt::KeyboardModifiers modifiers);
    bool withinHandleArea(const QPoint &pos) const;

    QToolBar *m_toolBar;
    PromotionTaskMenu *m_promotionTaskMenu;
    QPoint m_startPosition;
};
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QDESIGNER_TOOLBAR_H
