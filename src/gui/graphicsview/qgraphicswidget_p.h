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

#ifndef QGRAPHICSWIDGET_P_H
#define QGRAPHICSWIDGET_P_H

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

#include "qobject_p.h"
#include "qgraphicsitem_p.h"
#include "qgraphicswidget.h"
#include <QtGui/qfont.h>
#include <QtGui/qpalette.h>
#include <QtGui/qsizepolicy.h>
#include <QtGui/qstyle.h>

#include <bitset>

QT_BEGIN_NAMESPACE

class QGraphicsLayout;
class QStyleOptionTitleBar;

#ifndef QT_NO_GRAPHICSVIEW

class QGraphicsWidgetPrivate : public QGraphicsItemPrivate
{
    Q_DECLARE_PUBLIC(QGraphicsWidget)
public:
    QGraphicsWidgetPrivate()
        : margins(nullptr),
          layout(nullptr),
          inheritedPaletteResolveMask(0),
          inheritedFontResolveMask(0),
          polished(false),
          inSetGeometry(false),
          inSetPos(false),
          autoFillBackground(false),
          refCountInvokeRelayout(0),
          focusPolicy(Qt::NoFocus),
          focusNext(nullptr),
          focusPrev(nullptr),
          windowFlags(0),
          windowData(nullptr),
          setWindowFrameMargins(false),
          windowFrameMargins(0)
    { }
    virtual ~QGraphicsWidgetPrivate();

    void init(QGraphicsItem *parentItem, Qt::WindowFlags wFlags);
    qreal titleBarHeight(const QStyleOptionTitleBar &options) const;

    // Margins
    enum {Left, Top, Right, Bottom};
    mutable qreal *margins;
    void ensureMargins() const;

    void fixFocusChainBeforeReparenting(QGraphicsWidget *newParent, QGraphicsScene *oldScene, QGraphicsScene *newScene = 0);
    void setLayout_helper(QGraphicsLayout *l);

    // Layouts
    QGraphicsLayout *layout;
    void setLayoutDirection_helper(Qt::LayoutDirection direction);
    void resolveLayoutDirection();
    void _q_relayout();

    // Style
    QPalette palette;
    uint inheritedPaletteResolveMask;
    void setPalette_helper(const QPalette &palette);
    void resolvePalette(uint inheritedMask);
    void updatePalette(const QPalette &palette);
    QPalette naturalWidgetPalette() const;
    QFont font;
    uint inheritedFontResolveMask;
    void setFont_helper(const QFont &font);
    void resolveFont(uint inheritedMask);
    void updateFont(const QFont &font);
    QFont naturalWidgetFont() const;

    // Window specific
    void initStyleOptionTitleBar(QStyleOptionTitleBar *option);
    void adjustWindowFlags(Qt::WindowFlags *wFlags);
    void windowFrameMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void windowFrameMousePressEvent(QGraphicsSceneMouseEvent *event);
    void windowFrameMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void windowFrameHoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void windowFrameHoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    bool hasDecoration() const;

    // Private Properties
    qreal width() const;
    void setWidth(qreal);
    void resetWidth();

    qreal height() const;
    void setHeight(qreal);
    void resetHeight();
    void setGeometryFromSetPos();

    // State
    inline bool attributeSupported(Qt::WidgetAttribute att) const
    {
        switch (att) {
            case Qt::WA_SetLayoutDirection:
            case Qt::WA_RightToLeft:
            case Qt::WA_SetStyle:
            case Qt::WA_Resized:
            case Qt::WA_DeleteOnClose:
            case Qt::WA_NoSystemBackground:
            case Qt::WA_OpaquePaintEvent:
            case Qt::WA_SetPalette:
            case Qt::WA_SetFont:
            case Qt::WA_WindowPropagation:
                return true;
            default:
                return false;
        }
        return false;
    }

    inline void setAttribute(Qt::WidgetAttribute att, bool value)
    {
        if (!attributeSupported(att)) {
            qWarning("QGraphicsWidget::setAttribute: unsupported attribute %d", int(att));
            return;
        }
        attributes.set(att, value);
    }

    inline bool testAttribute(Qt::WidgetAttribute att) const
    {
        if (!attributeSupported(att))
            return false;
        return attributes.test(att);
    }

    std::bitset<Qt::WA_AttributeCount> attributes;
    bool polished;
    bool inSetGeometry;
    bool inSetPos;
    bool autoFillBackground;
    int refCountInvokeRelayout;

    // Focus
    Qt::FocusPolicy focusPolicy;
    QGraphicsWidget *focusNext;
    QGraphicsWidget *focusPrev;

    // Windows
    Qt::WindowFlags windowFlags;
    struct WindowData {
        QString windowTitle;
        QStyle::SubControl hoveredSubControl;
        Qt::WindowFrameSection grabbedSection;
        bool buttonMouseOver;
        bool buttonSunken;
        QRectF startGeometry;
        QRect buttonRect;
        WindowData()
            : hoveredSubControl(QStyle::SC_None)
            , grabbedSection(Qt::NoSection)
            , buttonMouseOver(false)
            , buttonSunken(false)
        {}
    } *windowData;
    void ensureWindowData();

    bool setWindowFrameMargins;
    mutable qreal *windowFrameMargins;
    void ensureWindowFrameMargins() const;

#ifndef QT_NO_ACTION
    QList<QAction *> actions;
#endif
};

#endif //!defined(QT_NO_GRAPHICSVIEW)

QT_END_NAMESPACE

#endif //QGRAPHICSWIDGET_P_H

