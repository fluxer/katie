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

#ifndef QDYNAMICDOCKWIDGET_P_H
#define QDYNAMICDOCKWIDGET_P_H

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

#include "QtGui/qstyleoption.h"
#include "qwidget_p.h"
#include "QtGui/qboxlayout.h"
#include "QtGui/qdockwidget.h"

#ifndef QT_NO_DOCKWIDGET

QT_BEGIN_NAMESPACE

class QGridLayout;
class QWidgetResizeHandler;
class QRubberBand;
class QDockWidgetTitleButton;
class QSpacerItem;
class QDockWidgetItem;

class QDockWidgetPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QDockWidget)

    struct DragState {
        QPoint pressPos;
        bool dragging;
        QLayoutItem *widgetItem;
        bool ownWidgetItem;
        bool nca;
        bool ctrlDrag;
    };

public:
    inline QDockWidgetPrivate()
	: QWidgetPrivate(), state(0),
          features(QDockWidget::DockWidgetClosable
                   | QDockWidget::DockWidgetMovable
                   | QDockWidget::DockWidgetFloatable),
          allowedAreas(Qt::AllDockWidgetAreas)
    { }

    void init();
    void _q_toggleView(bool); // private slot
    void _q_toggleTopLevel(); // private slot

    void updateButtons();
    DragState *state;

    QDockWidget::DockWidgetFeatures features;
    Qt::DockWidgetAreas allowedAreas;

    QWidgetResizeHandler *resizer;

#ifndef QT_NO_ACTION
    QAction *toggleViewAction;
#endif

//    QMainWindow *findMainWindow(QWidget *widget) const;
    QRect undockedGeometry;
    QString fixedWindowTitle;

    bool mousePressEvent(QMouseEvent *event);
    bool mouseDoubleClickEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void setWindowState(bool floating, bool unplug = false, const QRect &rect = QRect());
    void initDrag(const QPoint &pos, bool nca);
    void startDrag();
    void endDrag(bool abort = false);
    void moveEvent(QMoveEvent *event);

    void unplug(const QRect &rect);
    void plug(const QRect &rect);

    bool isAnimating() const;
};

class Q_GUI_EXPORT QDockWidgetLayout : public QLayout
{
    Q_OBJECT
public:
    QDockWidgetLayout(QWidget *parent = nullptr);
    ~QDockWidgetLayout();
    void addItem(QLayoutItem *item);
    QLayoutItem *itemAt(int index) const;
    QLayoutItem *takeAt(int index);
    int count() const;

    QSize maximumSize() const;
    QSize minimumSize() const;
    QSize sizeHint() const;

    QSize sizeFromContent(const QSize &content, bool floating) const;

    void setGeometry(const QRect &r);

    enum Role { Content, CloseButton, FloatButton, TitleBar, RoleCount };
    QWidget *widgetForRole(Role r) const;
    void setWidgetForRole(Role r, QWidget *w);
    QLayoutItem *itemForRole(Role r) const;

    QRect titleArea() const { return _titleArea; }

    int minimumTitleWidth() const;
    int titleHeight() const;
    void updateMaxSize();
    bool nativeWindowDeco() const;
    bool nativeWindowDeco(bool floating) const;

    void setVerticalTitleBar(bool b);

    bool verticalTitleBar;

private:
    QVector<QLayoutItem*> item_list;
    QRect _titleArea;
};

/* The size hints of a QDockWidget will depend on whether it is docked or not.
   This layout item always returns the size hints as if the dock widget was docked. */

class QDockWidgetItem : public QWidgetItem
{
public:
    QDockWidgetItem(QDockWidget *dockWidget);
    QSize minimumSize() const;
    QSize maximumSize() const;
    QSize sizeHint() const;

private:
    inline QLayoutItem *dockWidgetChildItem() const;
    inline QDockWidgetLayout *dockWidgetLayout() const;
};

inline QLayoutItem *QDockWidgetItem::dockWidgetChildItem() const
{
    if (QDockWidgetLayout *layout = dockWidgetLayout())
        return layout->itemForRole(QDockWidgetLayout::Content);
    return 0;
}

inline QDockWidgetLayout *QDockWidgetItem::dockWidgetLayout() const
{
    QWidget *w = const_cast<QDockWidgetItem*>(this)->widget();
    if (w != 0)
        return qobject_cast<QDockWidgetLayout*>(w->layout());
    return 0;
}

QT_END_NAMESPACE

#endif // QT_NO_DOCKWIDGET

#endif // QDYNAMICDOCKWIDGET_P_H
