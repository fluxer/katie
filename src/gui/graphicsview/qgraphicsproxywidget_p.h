/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGRAPHICSPROXYWIDGET_P_H
#define QGRAPHICSPROXYWIDGET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qgraphicsproxywidget.h"
#include "qgraphicswidget_p.h"

#ifndef QT_NO_GRAPHICSVIEW

QT_BEGIN_NAMESPACE

class QGraphicsProxyWidgetPrivate : public QGraphicsWidgetPrivate
{
    Q_DECLARE_PUBLIC(QGraphicsProxyWidget)
public:
    QGraphicsProxyWidgetPrivate()
        : dragDropWidget(Q_NULLPTR),
          posChangeMode(NoMode),
          sizeChangeMode(NoMode),
          visibleChangeMode(NoMode),
          enabledChangeMode(NoMode),
          styleChangeMode(NoMode),
          paletteChangeMode(NoMode),
          tooltipChangeMode(NoMode),
          focusFromWidgetToProxy(false),
          proxyIsGivingFocus(false)
    { }
    void init();
    void sendWidgetMouseEvent(QGraphicsSceneMouseEvent *event);
    void sendWidgetMouseEvent(QGraphicsSceneHoverEvent *event);
    void sendWidgetKeyEvent(QKeyEvent *event);
    void setWidget_helper(QWidget *widget, bool autoShow);

    QWidget *findFocusChild(QWidget *child, bool next) const;
    void removeSubFocusHelper(QWidget *widget, Qt::FocusReason reason);

    void _q_removeWidgetSlot();

    void embedSubWindow(QWidget *);
    void unembedSubWindow(QWidget *);

    bool isProxyWidget() const;

    QPointer<QWidget> widget;
    QPointer<QWidget> lastWidgetUnderMouse;
    QPointer<QWidget> embeddedMouseGrabber;
    QWidget *dragDropWidget;
    Qt::DropAction lastDropAction;

    void updateWidgetGeometryFromProxy();
    void updateProxyGeometryFromWidget();

    QPointF mapToReceiver(const QPointF &pos, const QWidget *receiver) const;

    enum ChangeMode {
        NoMode,
        ProxyToWidgetMode,
        WidgetToProxyMode
    };
    ChangeMode posChangeMode;
    ChangeMode sizeChangeMode;
    ChangeMode visibleChangeMode;
    ChangeMode enabledChangeMode;
    ChangeMode styleChangeMode;
    ChangeMode paletteChangeMode;
    ChangeMode tooltipChangeMode;
    bool focusFromWidgetToProxy;
    bool proxyIsGivingFocus;
};

QT_END_NAMESPACE

#endif

#endif
