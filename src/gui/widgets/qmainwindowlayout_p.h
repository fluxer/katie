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

#ifndef QDYNAMICMAINWINDOWLAYOUT_P_H
#define QDYNAMICMAINWINDOWLAYOUT_P_H

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

#include "qmainwindow.h"

#ifndef QT_NO_MAINWINDOW

#include "QtGui/qlayout.h"
#include "QtGui/qtabbar.h"
#include "QtCore/qvector.h"
#include "QtCore/qset.h"
#include "QtCore/qbasictimer.h"
#include "qlayoutengine_p.h"
#include "qwidgetanimator_p.h"

#include "qdockarealayout_p.h"
#include "qtoolbararealayout_p.h"

//#define Q_DEBUG_MAINWINDOW_LAYOUT

#if defined(Q_DEBUG_MAINWINDOW_LAYOUT) && !defined(QT_NO_DOCKWIDGET)
#include "qtextstream.h"
#endif // Q_DEBUG_MAINWINDOW_LAYOUT && !QT_NO_DOCKWIDGET

QT_BEGIN_NAMESPACE

class QToolBar;
class QRubberBand;

/* This data structure represents the state of all the tool-bars and dock-widgets. It's value based
   so it can be easilly copied into a temporary variable. All operations are performed without moving
   any widgets. Only when we are sure we have the desired state, we call apply(), which moves the
   widgets.
*/

class QMainWindowLayoutState
{
public:
    QRect rect;
    QMainWindow *mainWindow;

    QMainWindowLayoutState(QMainWindow *win);

#ifndef QT_NO_TOOLBAR
    QToolBarAreaLayout toolBarAreaLayout;
#endif

#ifndef QT_NO_DOCKWIDGET
    QDockAreaLayout dockAreaLayout;
#else
    QLayoutItem *centralWidgetItem;
    QRect centralWidgetRect;
#endif

    void apply(bool animated);
    void deleteAllLayoutItems();
    void deleteCentralWidgetItem();

    QSize sizeHint() const;
    QSize minimumSize() const;
    void fitLayout();

    QLayoutItem *itemAt(int index, int *x) const;
    QLayoutItem *takeAt(int index, int *x);
    QList<int> indexOf(QWidget *widget) const;
    QLayoutItem *item(const QList<int> &path);
    QRect itemRect(const QList<int> &path) const;
    QRect gapRect(const QList<int> &path) const; // ### get rid of this, use itemRect() instead

    bool contains(QWidget *widget) const;

    void setCentralWidget(QWidget *widget);
    QWidget *centralWidget() const;

    QList<int> gapIndex(QWidget *widget, const QPoint &pos) const;
    bool insertGap(const QList<int> &path, QLayoutItem *item);
    void remove(const QList<int> &path);
    void remove(QLayoutItem *item);
    void clear();
    bool isValid() const;

    QLayoutItem *plug(const QList<int> &path);
    QLayoutItem *unplug(const QList<int> &path, QMainWindowLayoutState *savedState = 0);

    void saveState(QDataStream &stream) const;
    bool checkFormat(QDataStream &stream);
    bool restoreState(QDataStream &stream, const QMainWindowLayoutState &oldState);
};

class Q_AUTOTEST_EXPORT QMainWindowLayout : public QLayout
{
    Q_OBJECT

public:
    QMainWindowLayoutState layoutState, savedState;

    QMainWindowLayout(QMainWindow *mainwindow, QLayout *parentLayout);
    ~QMainWindowLayout();

    QMainWindow::DockOptions dockOptions;
    void setDockOptions(QMainWindow::DockOptions opts);

    void timerEvent(QTimerEvent *e);

    // status bar

    QLayoutItem *statusbar;

#ifndef QT_NO_STATUSBAR
    QStatusBar *statusBar() const;
    void setStatusBar(QStatusBar *sb);
#endif

    // central widget

    QWidget *centralWidget() const;
    void setCentralWidget(QWidget *cw);

    // toolbars

#ifndef QT_NO_TOOLBAR
    void addToolBarBreak(Qt::ToolBarArea area);
    void insertToolBarBreak(QToolBar *before);
    void removeToolBarBreak(QToolBar *before);

    void addToolBar(Qt::ToolBarArea area, QToolBar *toolbar, bool needAddChildWidget = true);
    void insertToolBar(QToolBar *before, QToolBar *toolbar);
    Qt::ToolBarArea toolBarArea(QToolBar *toolbar) const;
    bool toolBarBreak(QToolBar *toolBar) const;
    void getStyleOptionInfo(QStyleOptionToolBar *option, QToolBar *toolBar) const;
    void removeToolBar(QToolBar *toolbar);
    void moveToolBar(QToolBar *toolbar, int pos);
#endif

    // dock widgets

#ifndef QT_NO_DOCKWIDGET
    void setCorner(Qt::Corner corner, Qt::DockWidgetArea area);
    Qt::DockWidgetArea corner(Qt::Corner corner) const;
    void addDockWidget(Qt::DockWidgetArea area,
                       QDockWidget *dockwidget,
                       Qt::Orientation orientation);
    void splitDockWidget(QDockWidget *after,
                         QDockWidget *dockwidget,
                         Qt::Orientation orientation);
    void tabifyDockWidget(QDockWidget *first, QDockWidget *second);
    Qt::DockWidgetArea dockWidgetArea(QDockWidget *dockwidget) const;
    void raise(QDockWidget *widget);
    void setVerticalTabsEnabled(bool enabled);
    bool restoreDockWidget(QDockWidget *dockwidget);

#ifndef QT_NO_TABBAR
    bool _documentMode;
    bool documentMode() const;
    void setDocumentMode(bool enabled);

    QTabBar *getTabBar();
    QSet<QTabBar*> usedTabBars;
    QList<QTabBar*> unusedTabBars;
    bool verticalTabsEnabled;

    QWidget *getSeparatorWidget();
    QSet<QWidget*> usedSeparatorWidgets;
    QList<QWidget*> unusedSeparatorWidgets;
    int sep; // separator extent

#ifndef QT_NO_TABWIDGET
    QTabWidget::TabPosition tabPositions[4];
    QTabWidget::TabShape _tabShape;

    QTabWidget::TabShape tabShape() const;
    void setTabShape(QTabWidget::TabShape tabShape);
    QTabWidget::TabPosition tabPosition(Qt::DockWidgetArea area) const;
    void setTabPosition(Qt::DockWidgetAreas areas, QTabWidget::TabPosition tabPosition);
#endif // QT_NO_TABWIDGET
#endif // QT_NO_TABBAR

    // separators

    QList<int> movingSeparator;
    QPoint movingSeparatorOrigin, movingSeparatorPos;
    QBasicTimer separatorMoveTimer;

    bool startSeparatorMove(const QPoint &pos);
    bool separatorMove(const QPoint &pos);
    bool endSeparatorMove(const QPoint &pos);
    void keepSize(QDockWidget *w);
#endif // QT_NO_DOCKWIDGET

    // save/restore

    enum { // sentinel values used to validate state data
        VersionMarker = 0xff
    };
    void saveState(QDataStream &stream) const;
    bool restoreState(QDataStream &stream);

    // QLayout interface

    void addItem(QLayoutItem *item);
    void setGeometry(const QRect &r);
    QLayoutItem *itemAt(int index) const;
    QLayoutItem *takeAt(int index);
    int count() const;

    QSize sizeHint() const;
    QSize minimumSize() const;
    mutable QSize szHint;
    mutable QSize minSize;
    void invalidate();

    // animations

    QWidgetAnimator widgetAnimator;
    QList<int> currentGapPos;
    QRect currentGapRect;
    QWidget *pluggingWidget;
#ifndef QT_NO_RUBBERBAND
    QRubberBand *gapIndicator;
#endif

    QList<int> hover(QLayoutItem *widgetItem, const QPoint &mousePos);
    bool plug(QLayoutItem *widgetItem);
    QLayoutItem *unplug(QWidget *widget);
    void revert(QLayoutItem *widgetItem);
    void updateGapIndicator();
    void paintDropIndicator(QPainter *p, QWidget *widget, const QRegion &clip);
    void applyState(QMainWindowLayoutState &newState, bool animate = true);
    void restore(bool keepSavedState = false);
    void updateHIToolBarStatus();
    void animationFinished(QWidget *widget);

private Q_SLOTS:
#ifndef QT_NO_DOCKWIDGET
#ifndef QT_NO_TABBAR
    void tabChanged();
#endif
#endif
private:
#ifndef QT_NO_TABBAR
    void updateTabBarShapes();
#endif
};
QT_END_NAMESPACE

#endif // QT_NO_MAINWINDOW

#endif // QDYNAMICMAINWINDOWLAYOUT_P_H
