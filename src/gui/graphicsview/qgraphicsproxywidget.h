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

#ifndef QGRAPHICSPROXYWIDGET_H
#define QGRAPHICSPROXYWIDGET_H

#include <QtGui/qgraphicswidget.h>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_GRAPHICSVIEW

class QGraphicsProxyWidgetPrivate;

class Q_GUI_EXPORT QGraphicsProxyWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    QGraphicsProxyWidget(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    ~QGraphicsProxyWidget();

    void setWidget(QWidget *widget);
    QWidget *widget() const;

    QRectF subWidgetRect(const QWidget *widget) const;

    void setGeometry(const QRectF &rect);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    enum {
        Type = 12
    };
    int type() const;

    QGraphicsProxyWidget *createProxyForChildWidget(QWidget *child);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    bool event(QEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
#endif

#ifndef QT_NO_DRAGANDDROP
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
#endif

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void grabMouseEvent(QEvent *event);
    void ungrabMouseEvent(QEvent *event);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QGraphicsSceneWheelEvent *event);
#endif

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    bool focusNextPrevChild(bool next);

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    void resizeEvent(QGraphicsSceneResizeEvent *event);

protected Q_SLOTS:
    QGraphicsProxyWidget *newProxyWidget(const QWidget *);

private:
    Q_DISABLE_COPY(QGraphicsProxyWidget)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QGraphicsProxyWidget)
    Q_PRIVATE_SLOT(d_func(), void _q_removeWidgetSlot())

    friend class QWidget;
    friend class QWidgetPrivate;
    friend class QGraphicsItem;
};

#endif

QT_END_NAMESPACE


#endif

