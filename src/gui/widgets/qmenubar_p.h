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

#ifndef QMENUBAR_P_H
#define QMENUBAR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// platformMenuBarementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtGui/qstyleoption.h"
#include "qwidget_p.h"
#include "qbasictimer.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_MENUBAR
class QToolBar;
class QMenuBarExtension;
class QMenuBarPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QMenuBar)
public:
    QMenuBarPrivate() : itemsDirty(false), currentAction(nullptr), mouseDown(false),
                         closePopupMode(false), defaultPopDown(true), popupState(false), keyboardState(false), altPressed(false)
                         , doChildEffects(false)

        { }
    ~QMenuBarPrivate()
        { }

    void init();
    QAction *getNextAction(const int start, const int increment) const;

    //item calculations
    bool itemsDirty;

    QVector<int> shortcutIndexMap;
    mutable QVector<QRect> actionRects;
    void calcActionRects(int max_width, int start) const;
    QRect actionRect(QAction *) const;
    void updateGeometries();

    //selection
    QPointer<QAction> currentAction;
    bool mouseDown;
    bool closePopupMode;
    bool defaultPopDown;
    QAction *actionAt(QPoint p) const;
    void setCurrentAction(QAction *, bool =false, bool =false);
    void popupAction(QAction *, bool);

    //active popup state
    bool popupState;
    QPointer<QMenu> activeMenu;

    //keyboard mode for keyboard navigation
    void focusFirstAction();
    void setKeyboardMode(bool);
    bool keyboardState;
    bool altPressed;
    QPointer<QWidget> keyboardFocusWidget;

    //firing of events
    void activateAction(QAction *, QAction::ActionEvent);

    void _q_actionTriggered();
    void _q_actionHovered();
    void _q_internalShortcutActivated(int);
    void _q_updateLayout();

    //extra widgets in the menubar
    QPointer<QWidget> leftWidget, rightWidget;
    QMenuBarExtension *extension;
    bool isVisible(QAction *action);

    //menu fading/scrolling effects
    bool doChildEffects;

    QRect menuRect(bool) const;

    // reparenting
    void handleReparent();
    QWidget *oldParent;
    QWidget *oldWindow;

    QList<QAction*> hiddenActions;
    //default action
    QPointer<QAction> defaultAction;

    QBasicTimer autoReleaseTimer;

#ifdef Q_WS_X11
    void updateCornerWidgetToolBar();
    QToolBar *cornerWidgetToolBar;
    QWidget *cornerWidgetContainer;
#endif
};

#endif // QT_NO_MENUBAR

QT_END_NAMESPACE

#endif // QMENUBAR_P_H
