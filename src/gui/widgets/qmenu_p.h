/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QMENU_P_H
#define QMENU_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtGui/qmenubar.h"
#include "QtGui/qstyleoption.h"
#include "QtCore/qdatetime.h"
#include "QtCore/qmap.h"
#include "QtCore/qhash.h"
#include "QtCore/qbasictimer.h"
#include "qwidget_p.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_MENU

class QTornOffMenu;
class QEventLoop;

class QMenuPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QMenu)
public:
    QMenuPrivate() : itemsDirty(0), maxIconWidth(0), tabWidth(0), ncols(0),
                      collapsibleSeparators(true), activationRecursionGuard(false),
                      hasHadMouse(false), aboutToHide(false), motions(0),
                      currentAction(0),
                      scroll(0), eventLoop(0), tearoff(false), tornoff(false), tearoffHighlighted(false),
                      hasCheckableItems(false), sloppyDelayTimer(0), sloppyAction(0), doChildEffects(false)
    { }
    ~QMenuPrivate()
    {
        delete scroll;

    }
    void init();

    static QMenuPrivate *get(QMenu *m) { return m->d_func(); }
    int scrollerHeight() const;

    //item calculations
    mutable bool itemsDirty;
    mutable uint maxIconWidth;
    mutable uint tabWidth;
    QRect actionRect(QAction *) const;

    mutable QVector<QRect> actionRects;
    QHash<QAction *, QWidget *> widgetItems;
    void updateActionRects() const;
    void updateActionRects(const QRect &screen) const;
    QRect popupGeometry(const QWidget *widget) const;
    QRect popupGeometry(int screen = -1) const;
    mutable int ncols;
    bool collapsibleSeparators;
    QSize adjustMenuSizeForScreen(const QRect & screen);
    int getLastVisibleAction() const;

    bool activationRecursionGuard;

    //selection
    static QMenu *mouseDown;
    QPoint mousePopupPos;
    bool hasHadMouse;
    bool aboutToHide;
    int motions;
    QAction *currentAction;
    QBasicTimer menuDelayTimer;
    enum SelectionReason {
        SelectedFromKeyboard,
        SelectedFromElsewhere
    };
    QWidget *topCausedWidget() const;
    QAction *actionAt(QPoint p) const;
    void setFirstActionActive();
    void setCurrentAction(QAction *, int popup = -1, SelectionReason reason = SelectedFromElsewhere, bool activateFirst = false);
    void popupAction(QAction *, int, bool);
    void setSyncAction();

    //scrolling support
    struct QMenuScroller {
        enum ScrollLocation { ScrollStay, ScrollBottom, ScrollTop, ScrollCenter };
        enum ScrollDirection { ScrollNone=0, ScrollUp=0x01, ScrollDown=0x02 };
        uint scrollFlags : 2, scrollDirection : 2;
        int scrollOffset;
        QBasicTimer scrollTimer;

        QMenuScroller() : scrollFlags(ScrollNone), scrollDirection(ScrollNone), scrollOffset(0) { }
        ~QMenuScroller() { }
    } *scroll;
    void scrollMenu(QMenuScroller::ScrollLocation location, bool active=false);
    void scrollMenu(QMenuScroller::ScrollDirection direction, bool page=false, bool active=false);
    void scrollMenu(QAction *action, QMenuScroller::ScrollLocation location, bool active=false);

    //synchronous operation (ie exec())
    QEventLoop *eventLoop;
    QPointer<QAction> syncAction;

    //search buffer
    QString searchBuffer;
    QBasicTimer searchBufferTimer;

    //passing of mouse events up the parent hierarchy
    QPointer<QMenu> activeMenu;
    bool mouseEventTaken(QMouseEvent *);

    //used to walk up the popup list
    struct QMenuCaused {
        QPointer<QWidget> widget;
        QPointer<QAction> action;
    };
    virtual QList<QPointer<QWidget> > calcCausedStack() const;
    QMenuCaused causedPopup;
    void hideUpToMenuBar();
    void hideMenu(QMenu *menu, bool justRegister = false);

    //index mappings
    inline QAction *actionAt(int i) const { return q_func()->actions().at(i); }
    inline int indexOf(QAction *act) const { return q_func()->actions().indexOf(act); }

    //tear off support
    bool tearoff;
    bool tornoff;
    bool tearoffHighlighted;
    QPointer<QTornOffMenu> tornPopup;

    mutable bool hasCheckableItems;

    //sloppy selection
    int sloppyDelayTimer;
    mutable QAction *sloppyAction;
    QRegion sloppyRegion;

    //default action
    QPointer<QAction> defaultAction;

    QAction *menuAction;
    QAction *defaultMenuAction;

    void setOverrideMenuAction(QAction *);
    void _q_overrideMenuActionDestroyed();

    //firing of events
    void activateAction(QAction *, QAction::ActionEvent, bool self=true);
    void activateCausedStack(const QList<QPointer<QWidget> > &, QAction *, QAction::ActionEvent, bool);

    void _q_actionTriggered();
    void _q_actionHovered();

    bool hasMouseMoved(const QPoint &globalPos);

    void updateLayoutDirection();

    //menu fading/scrolling effects
    bool doChildEffects;

    QPointer<QAction> actionAboutToTrigger;

    QPointer<QWidget> noReplayFor;
};

#endif // QT_NO_MENU

QT_END_NAMESPACE

#endif // QMENU_P_H
