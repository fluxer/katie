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

#ifndef QWIDGET_P_H
#define QWIDGET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp and qfiledialog.cpp.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#include "QtGui/qwidget.h"
#include "qobject_p.h"
#include "QtCore/qrect.h"
#include "QtCore/qlocale.h"
#include "QtCore/qset.h"
#include "QtGui/qregion.h"
#include "QtGui/qsizepolicy.h"
#include "QtGui/qstyle.h"
#include "QtGui/qapplication.h"
#include "qgraphicseffect_p.h"
#include "QtGui/qgraphicsproxywidget.h"
#include "QtGui/qgraphicsscene.h"
#include "QtGui/qgraphicsview.h"

#ifdef Q_WS_X11
#include "QtGui/qx11info_x11.h"
#endif

#include <bitset>

QT_BEGIN_NAMESPACE

// Extra QWidget data
//  - to minimize memory usage for members that are seldom used.
//  - top-level widgets have extra extra data to reduce cost further
class QPaintEngine;
class QPixmap;
class QWidgetBackingStore;
class QGraphicsProxyWidget;
class QWidgetItemV2;
class QStyle;

class Q_AUTOTEST_EXPORT QWidgetBackingStoreTracker
{

public:
    QWidgetBackingStoreTracker();
    ~QWidgetBackingStoreTracker();

    void create(QWidget *tlw);
    void destroy();

    void registerWidget(QWidget *w);
    void unregisterWidget(QWidget *w);
    void unregisterWidgetSubtree(QWidget *w);

    inline QWidgetBackingStore* data()
    {
        return m_ptr;
    }

    inline QWidgetBackingStore* operator->()
    {
        return m_ptr;
    }

    inline QWidgetBackingStore& operator*()
    {
        return *m_ptr;
    }

    inline operator bool() const
    {
        return (0 != m_ptr);
    }

private:
    Q_DISABLE_COPY(QWidgetBackingStoreTracker)

private:
    QWidgetBackingStore* m_ptr;
    QSet<QWidget *> m_widgets;
};

struct QTLWExtra {
    // *************************** Cross-platform variables *****************************

    // Regular pointers (keep them together to avoid gaps on 64 bits architectures).
    QIcon *icon; // widget icon
    QPixmap *iconPixmap;
    QWidgetBackingStoreTracker backingStore;
    QWindowSurface *windowSurface;
    QPainter *sharedPainter;

    // Implicit pointers (shared_null).
    QString caption; // widget caption
    QString iconText; // widget icon text
    QString role; // widget role

    // Other variables.
    short incw, inch; // size increments
    short basew, baseh; // base sizes
     // frame strut, don't use these directly, use QWidgetPrivate::frameStrut() instead.
    QRect frameStrut;
    QRect normalGeometry; // used by showMin/maximized/FullScreen
    Qt::WindowFlags savedFlags; // Save widget flags while showing fullscreen

    // *************************** Cross-platform bit fields ****************************
    uint opacity;
    bool posFromMove;
    bool sizeAdjusted;
    bool inTopLevelResize;
    bool inRepaint;
    bool embedded;

    // *************************** Platform specific values (bit fields first) **********
#if defined(Q_WS_X11) // <----------------------------------------------------------- X11
    bool spont_unmapped; // window was spontaneously unmapped
    bool dnd; // DND properties installed
    bool validWMState; // is WM_STATE valid?
    bool waitingForMapNotify; // show() has been called, haven't got the MapNotify yet
    WId parentWinId; // parent window Id (valid after reparenting)
    WId userTimeWindow; // window id that contains user-time timestamp when WM supports a _NET_WM_USER_TIME_WINDOW atom
    QPoint fullScreenOffset;
#ifndef QT_NO_XSYNC
    WId syncUpdateCounter;
    ulong syncRequestTimestamp;
    qint32 newCounterValueHi;
    quint32 newCounterValueLo;
#endif
#endif
};

struct QWExtra {
    // *************************** Cross-platform variables *****************************

    // Regular pointers (keep them together to avoid gaps on 64 bits architectures).
    QTLWExtra *topextra; // only useful for TLWs
#ifndef QT_NO_GRAPHICSVIEW
    QGraphicsProxyWidget *proxyWidget; // if the widget is embedded
#endif
#ifndef QT_NO_CURSOR
    QCursor *curs;
#endif
    QPointer<QStyle> style;
    QPointer<QWidget> focus_proxy;

    // Implicit pointers (shared_empty/shared_null).
    QRegion mask; // widget mask
    QString styleSheet;

    // Other variables.
    qint32 minw;
    qint32 minh; // minimum size
    qint32 maxw;
    qint32 maxh; // maximum size
    quint16 customDpiX;
    quint16 customDpiY;
    QSize staticContentsSize;

    // *************************** Cross-platform bit fields ****************************
    Qt::Orientations explicitMinSize;
    Qt::Orientations explicitMaxSize;
    bool autoFillBackground;
    bool nativeChildrenForced;
    bool inRenderWithPainter;
    bool hasMask;

    // *************************** Platform specific values (bit fields first) **********
#if defined(Q_WS_X11) // <--------------------------------------------------------- X11
    bool compress_events;
    WId xDndProxy; // XDND forwarding to embedded windows
#endif
};

/*!
    \internal

    Returns true if \a p or any of its parents enable the
    Qt::BypassGraphicsProxyWidget window flag. Used in QWidget::show() and
    QWidget::setParent() to determine whether it's necessary to embed the
    widget into a QGraphicsProxyWidget or not.
*/
static inline bool bypassGraphicsProxyWidget(const QWidget *p)
{
    while (p) {
        if (p->windowFlags() & Qt::BypassGraphicsProxyWidget)
            return true;
        p = p->parentWidget();
    }
    return false;
}

class Q_GUI_EXPORT QWidgetPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QWidget)

public:
    // *************************** Cross-platform ***************************************
    enum DrawWidgetFlags {
        DrawAsRoot = 0x01,
        DrawPaintOnScreen = 0x02,
        DrawRecursive = 0x04,
        DrawInvisible = 0x08,
        DontSubtractOpaqueChildren = 0x10,
        DontDrawOpaqueChildren = 0x20
    };

    enum CloseMode {
        CloseNoEvent,
        CloseWithEvent,
        CloseWithSpontaneousEvent
    };

    QWidgetPrivate();
    ~QWidgetPrivate();

    // Functions.
    QWExtra *extraData() const;
    QTLWExtra *topData() const;
    QTLWExtra *maybeTopData() const;
    QPainter *sharedPainter() const;
    void setSharedPainter(QPainter *painter);
    QWidgetBackingStore *maybeBackingStore() const;
    void init(QWidget *desktopWidget, Qt::WindowFlags f);
    void create_sys(WId window, bool initializeWindow, bool destroyOldWindow);
    void createRecursively();
    void createWinId(WId id = 0);

    void createTLExtra();
    void createExtra();
    void deleteExtra();
    void createSysExtra();
    void createTLSysExtra();
    void deleteTLSysExtra();
    void updateSystemBackground();
    void propagatePaletteChange();

    void setPalette_helper(const QPalette &);
    void resolvePalette();
    QPalette naturalWidgetPalette(uint inheritedMask) const;

    void setMask_sys(const QRegion &);

    void raise_sys();
    void lower_sys();
    void stackUnder_sys(QWidget *);

    void updateFont(const QFont &);
    inline void setFont_helper(const QFont &font) {
        if (data.fnt == font && data.fnt.resolve() == font.resolve())
            return;
        updateFont(font);
    }
    void resolveFont();
    QFont naturalWidgetFont(uint inheritedMask) const;

    void setLayoutDirection_helper(Qt::LayoutDirection);
    void resolveLayoutDirection();

    void setLocale_helper(const QLocale &l, bool forceUpdate = false);
    void resolveLocale();

    void setStyle_helper(QStyle *newStyle, bool propagate);
    void inheritStyle();

    void setUpdatesEnabled_helper(bool );

    void paintBackground(QPainter *, const QRegion &, int flags = DrawAsRoot) const;
    bool isAboutToShow() const;
    QRegion prepareToRender(const QRegion &region, QWidget::RenderFlags renderFlags);
    void render_helper(QPainter *painter, const QPoint &targetOffset, const QRegion &sourceRegion,
                       QWidget::RenderFlags renderFlags);
    void render(QPaintDevice *target, const QPoint &targetOffset, const QRegion &sourceRegion,
                QWidget::RenderFlags renderFlags, bool readyToRender);
    void drawWidget(QPaintDevice *pdev, const QRegion &rgn, const QPoint &offset, int flags,
                    QPainter *sharedPainter = nullptr, QWidgetBackingStore *backingStore = nullptr);


    void paintSiblingsRecursive(QPaintDevice *pdev, const QObjectList& children, int index,
                                const QRegion &rgn, const QPoint &offset, int flags,
                                QPainter *sharedPainter, QWidgetBackingStore *backingStore);

#ifndef QT_NO_GRAPHICSVIEW
    static QGraphicsProxyWidget * nearestGraphicsProxyWidget(const QWidget *origin);
#endif
    QWindowSurface *createDefaultWindowSurface();
    void repaint_sys(const QRegion &rgn);

    QRect clipRect() const;
    QRegion clipRegion() const;
    void subtractOpaqueChildren(QRegion &rgn, const QRect &clipRect) const;
    void subtractOpaqueSiblings(QRegion &source, bool *hasDirtySiblingsAbove = nullptr,
                                bool alsoNonOpaque = false) const;
    void clipToEffectiveMask(QRegion &region) const;
    void updateIsOpaque();
    void setOpaque(bool opaque);
    void updateIsTranslucent();
    bool paintOnScreen() const;
#ifndef QT_NO_GRAPHICSEFFECT
    void invalidateGraphicsEffectsRecursively();
#endif //QT_NO_GRAPHICSEFFECT

    const QRegion &getOpaqueChildren() const;
    void setDirtyOpaqueRegion();

    bool close_helper(CloseMode mode);

    void setWindowIcon_helper();
    void setWindowIcon_sys(bool forceReset = false);
    void setWindowOpacity_sys(qreal opacity);
    void adjustQuitOnCloseAttribute();

    void scrollChildren(int dx, int dy);
    void moveRect(const QRect &, int dx, int dy);
    void scrollRect(const QRect &, int dx, int dy);
    void invalidateBuffer_resizeHelper(const QPoint &oldPos, const QSize &oldSize);
    // ### Qt 4.6: Merge into a template function (after MSVC isn't supported anymore).
    void invalidateBuffer(const QRegion &);
    void invalidateBuffer(const QRect &);
    bool isOverlapped(const QRect&) const;
    void syncBackingStore();
    void syncBackingStore(const QRegion &region);

    void reparentFocusWidgets(QWidget *oldtlw);

    static int pointToRect(const QPoint &p, const QRect &r);

    void setWinId(WId);
    void showChildren(bool spontaneous);
    void hideChildren(bool spontaneous);
    void setParent_sys(QWidget *parent, Qt::WindowFlags);
    void scroll_sys(int dx, int dy);
    void scroll_sys(int dx, int dy, const QRect &r);
    void deactivateWidgetCleanup();
    void setGeometry_sys(int, int, int, int, bool);
    void sendPendingMoveAndResizeEvents(bool recursive = false, bool disableUpdates = false);
    void activateChildLayoutsRecursively();
    void show_recursive();
    void show_helper();
    void show_sys();
    void hide_sys();
    void hide_helper();
    void _q_showIfNotHidden();

    void setEnabled_helper(bool);
    static void adjustFlags(Qt::WindowFlags &flags, QWidget *w = nullptr);

    void updateFrameStrut();
    QRect frameStrut() const;


    void setWindowIconText_sys(const QString &cap);
    void setWindowIconText_helper(const QString &cap);
    void setWindowTitle_sys(const QString &cap);

#ifndef QT_NO_CURSOR
    void setCursor_sys(const QCursor &cursor);
    void unsetCursor_sys();
#endif

    void setWindowTitle_helper(const QString &cap);

    bool setMinimumSize_helper(int &minw, int &minh);
    bool setMaximumSize_helper(int &maxw, int &maxh);
    virtual bool hasHeightForWidth() const;
    void setConstraints_sys();
    bool pointInsideRectAndMask(const QPoint &) const;
    QWidget *childAt_helper(const QPoint &, bool) const;
    QWidget *childAtRecursiveHelper(const QPoint &p, bool) const;
    void updateGeometry_helper(bool forceUpdate);

    void getLayoutItemMargins(int *left, int *top, int *right, int *bottom) const;
    void setLayoutItemMargins(int left, int top, int right, int bottom);
    void setLayoutItemMargins(QStyle::SubElement element, const QStyleOption *opt = nullptr);

    inline QWidget *effectiveFocusWidget() {
        QWidget *w = q_func();
        while (w->focusProxy())
            w = w->focusProxy();
        return w;
    }

    // This is an helper function that return the available geometry for
    // a widget and takes care is this one is in QGraphicsView.
    // If the widget is not embed in a scene then the geometry available is
    // null, we let QDesktopWidget decide for us.
    static QRect screenGeometry(const QWidget *widget)
    {
        QRect screen;
#ifndef QT_NO_GRAPHICSVIEW
        QGraphicsProxyWidget *ancestorProxy = widget->d_func()->nearestGraphicsProxyWidget(widget);
        //It's embedded if it has an ancestor
        if (ancestorProxy) {
            if (!bypassGraphicsProxyWidget(widget) && ancestorProxy->scene() != 0) {
                // One view, let be smart and return the viewport rect then the popup is aligned
                if (ancestorProxy->scene()->views().size() == 1) {
                    QGraphicsView *view = ancestorProxy->scene()->views().at(0);
                    screen = view->mapToScene(view->viewport()->rect()).boundingRect().toRect();
                } else {
                    screen = ancestorProxy->scene()->sceneRect().toRect();
                }
            }
        }
#endif
        return screen;
    }

    inline void setRedirected(QPaintDevice *replacement, const QPoint &offset)
    {
        Q_ASSERT(q_func()->testAttribute(Qt::WA_WState_InPaintEvent));
        redirectDev = replacement;
        redirectOffset = offset;
    }

    inline QPaintDevice *redirected(QPoint *offset) const
    {
        if (offset)
            *offset = redirectDev ? redirectOffset : QPoint();
        return redirectDev;
    }

    inline void restoreRedirected()
    { redirectDev = nullptr; }

    inline void enforceNativeChildren()
    {
        if (!extra)
            createExtra();

        if (extra->nativeChildrenForced)
            return;
        extra->nativeChildrenForced = true;

        for (int i = 0; i < children.size(); ++i) {
            if (QWidget *child = qobject_cast<QWidget *>(children.at(i)))
                child->setAttribute(Qt::WA_NativeWindow);
        }
    }

    inline bool nativeChildrenForced() const
    {
        return extra ? extra->nativeChildrenForced : false;
    }

    inline QRect effectiveRectFor(const QRect &rect) const
    {
#ifndef QT_NO_GRAPHICSEFFECT
        if (graphicsEffect && graphicsEffect->isEnabled())
            return graphicsEffect->boundingRectFor(rect).toAlignedRect();
#endif //QT_NO_GRAPHICSEFFECT
        return rect;
    }

    QSize adjustedSize() const;

    inline void handleSoftwareInputPanel(Qt::MouseButton button, bool clickCausedFocus)
    {
        Q_Q(QWidget);
        if (button == Qt::LeftButton && qApp->autoSipEnabled()) {
            QStyle::RequestSoftwareInputPanel behavior = QStyle::RequestSoftwareInputPanel(
                    q->style()->styleHint(QStyle::SH_RequestSoftwareInputPanel));
            if (!clickCausedFocus || behavior == QStyle::RSIP_OnMouseClick) {
                QEvent event(QEvent::RequestSoftwareInputPanel);
                QApplication::sendEvent(q, &event);
            }
        }
    }

    // Almost cross-platform :-)
    void setWSGeometry(bool dontShow=false);

    inline QPoint mapToWS(const QPoint &p) const
    { return p - data.wrect.topLeft(); }

    inline QPoint mapFromWS(const QPoint &p) const
    { return p + data.wrect.topLeft(); }

    inline QRect mapToWS(const QRect &r) const
    { return r.translated(-data.wrect.topLeft()); }

    inline QRect mapFromWS(const QRect &r) const
    { return r.translated(data.wrect.topLeft()); }

    // Variables.
    // Regular pointers (keep them together to avoid gaps on 64 bit architectures).
    QWExtra *extra;
    QWidget *focus_next;
    QWidget *focus_prev;
    QWidget *focus_child;
    QLayout *layout;
    QRegion *needsFlush;
    QPaintDevice *redirectDev;
    QWidgetItemV2 *widgetItem;
    QPaintEngine *extraPaintEngine;
    mutable const QMetaObject *polished;
    QGraphicsEffect *graphicsEffect;
    // All widgets are added into the allWidgets set. Once
    // they receive a window id they are also added to the mapper.
    // This should just ensure that all widgets are deleted by QApplication
    static QWidgetMapper *mapper;
    static QWidgetSet *allWidgets;

    // Implicit pointers (shared_null/shared_empty).
    QRegion opaqueChildren;
    QRegion dirty;
#ifndef QT_NO_TOOLTIP
    QString toolTip;
#endif
#ifndef QT_NO_STATUSTIP
    QString statusTip;
#endif
#ifndef QT_NO_WHATSTHIS
    QString whatsThis;
#endif
#ifndef QT_NO_ACCESSIBILITY
    QString accessibleName;
    QString accessibleDescription;
#endif

    // Other variables.
    uint inheritedFontResolveMask;
    uint inheritedPaletteResolveMask;
    short leftmargin;
    short topmargin;
    short rightmargin;
    short bottommargin;
    int leftLayoutItemMargin;
    int topLayoutItemMargin;
    int rightLayoutItemMargin;
    int bottomLayoutItemMargin;
    Qt::HANDLE hd;
    QWidgetData data;
    QSizePolicy size_policy;
    QLocale locale;
    QPoint redirectOffset;
#ifndef QT_NO_ACTION
    QList<QAction*> actions;
#endif

    // Bit fields.
    std::bitset<Qt::WA_AttributeCount> widget_attributes;
    QPalette::ColorRole fg_role;
    QPalette::ColorRole bg_role;
    bool dirtyOpaqueChildren;
    bool isOpaque;
    bool inDirtyList;
    bool isScrolled;
    bool isMoved;
    bool inSetParent;

    // *************************** Platform specific ************************************
#if defined(Q_WS_X11) // <----------------------------------------------------------- X11
    QX11Info xinfo;
    Qt::HANDLE picture;
    static QWidget *mouseGrabber;
    static QWidget *keyboardGrabber;

    void setWindowRole();
    void sendStartupMessage() const;
    void setNetWmWindowTypes();
    void x11UpdateIsOpaque();
    bool isBackgroundInherited() const;
    void updateX11AcceptFocus();
    QPoint mapToGlobal(const QPoint &pos) const;
    QPoint mapFromGlobal(const QPoint &pos) const;
#endif

};

struct QWidgetPaintContext
{
    inline QWidgetPaintContext(QPaintDevice *d, const QRegion &r, const QPoint &o, int f,
                               QPainter *p, QWidgetBackingStore *b)
        : pdev(d), rgn(r), offset(o), flags(f), sharedPainter(p), backingStore(b), painter(0) {}

    QPaintDevice *pdev;
    QRegion rgn;
    QPoint offset;
    int flags;
    QPainter *sharedPainter;
    QWidgetBackingStore *backingStore;
    QPainter *painter;
};

#ifndef QT_NO_GRAPHICSEFFECT
class QWidgetEffectSourcePrivate : public QGraphicsEffectSourcePrivate
{
public:
    QWidgetEffectSourcePrivate(QWidget *widget)
        : QGraphicsEffectSourcePrivate(), m_widget(widget), context(0), updateDueToGraphicsEffect(false)
    {}

    inline void detach()
    { m_widget->d_func()->graphicsEffect = 0; }

    inline const QGraphicsItem *graphicsItem() const
    { return nullptr; }

    inline const QWidget *widget() const
    { return m_widget; }

    inline void update()
    {
        updateDueToGraphicsEffect = true;
        m_widget->update();
        updateDueToGraphicsEffect = false;
    }

    inline bool isPixmap() const
    { return false; }

    inline void effectBoundingRectChanged()
    {
        // ### This function should take a rect parameter; then we can avoid
        // updating too much on the parent widget.
        if (QWidget *parent = m_widget->parentWidget())
            parent->update();
        else
            update();
    }

    inline const QStyleOption *styleOption() const
    { return nullptr; }

    inline QRect deviceRect() const
    { return m_widget->window()->rect(); }

    QRectF boundingRect(Qt::CoordinateSystem system) const;
    void draw(QPainter *p);
    QPixmap pixmap(Qt::CoordinateSystem system, QPoint *offset,
                   QGraphicsEffect::PixmapPadMode mode) const;

    QWidget *m_widget;
    QWidgetPaintContext *context;
    QTransform lastEffectTransform;
    bool updateDueToGraphicsEffect;
};
#endif //QT_NO_GRAPHICSEFFECT

inline QWExtra *QWidgetPrivate::extraData() const
{
    return extra;
}

inline QTLWExtra *QWidgetPrivate::topData() const
{
    const_cast<QWidgetPrivate *>(this)->createTLExtra();
    return extra->topextra;
}

inline QTLWExtra *QWidgetPrivate::maybeTopData() const
{
    return extra ? extra->topextra : nullptr;
}

inline QPainter *QWidgetPrivate::sharedPainter() const
{
    Q_Q(const QWidget);
    QTLWExtra *x = q->window()->d_func()->maybeTopData();
    return x ? x->sharedPainter : nullptr;
}

inline void QWidgetPrivate::setSharedPainter(QPainter *painter)
{
    Q_Q(QWidget);
    QTLWExtra *x = q->window()->d_func()->topData();
    x->sharedPainter = painter;
}

inline bool QWidgetPrivate::pointInsideRectAndMask(const QPoint &p) const
{
    Q_Q(const QWidget);
    return q->rect().contains(p) && (!extra || !extra->hasMask || q->testAttribute(Qt::WA_MouseNoMask)
                                     || extra->mask.contains(p));
}

inline QWidgetBackingStore *QWidgetPrivate::maybeBackingStore() const
{
    Q_Q(const QWidget);
    QTLWExtra *x = q->window()->d_func()->maybeTopData();
    return x ? x->backingStore.data() : nullptr;
}

QT_END_NAMESPACE

#endif // QWIDGET_P_H
