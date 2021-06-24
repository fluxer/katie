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

#ifndef QBACKINGSTORE_P_H
#define QBACKINGSTORE_P_H

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

#include <QDebug>
#include <QtGui/qwidget.h>
#include "qwidget_p.h"
#include "qwindowsurface_p.h"

QT_BEGIN_NAMESPACE

class QWindowSurface;

struct BeginPaintInfo {
    inline BeginPaintInfo() : wasFlushed(false) {}
    bool wasFlushed;
};

class Q_AUTOTEST_EXPORT QWidgetBackingStore
{
public:
    QWidgetBackingStore(QWidget *t);
    ~QWidgetBackingStore();

    static void showYellowThing(QWidget *widget, const QRegion &rgn, int msec, bool);

    void sync(QWidget *exposedWidget, const QRegion &exposedRegion);
    void sync();
    void flush(QWidget *widget = 0, QWindowSurface *surface = 0);

    inline QPoint topLevelOffset() const { return tlwOffset; }

    QWindowSurface *surface() const { return windowSurface; }

    inline bool isDirty() const
    {
        return !(dirtyWidgets.isEmpty() && dirty.isEmpty());
    }

    // ### Qt 4.6: Merge into a template function (after MSVC isn't supported anymore).
    void markDirty(const QRegion &rgn, QWidget *widget, bool updateImmediately = false,
                   bool invalidateBuffer = false);
    void markDirty(const QRect &rect, QWidget *widget, bool updateImmediately = false,
                   bool invalidateBuffer = false);

private:
    QWidget *tlw;
    QRegion dirtyOnScreen; // needsFlush
    QRegion dirty; // needsRepaint
    QVector<QWidget *> dirtyWidgets;
    QVector<QWidget *> dirtyOnScreenWidgets;
    QList<QWidget *> staticWidgets;
    QWindowSurface *windowSurface;

    QPoint tlwOffset;

    static bool flushPaint(QWidget *widget, const QRegion &rgn);
    static void unflushPaint(QWidget *widget, const QRegion &rgn);

    bool bltRect(const QRect &rect, int dx, int dy, QWidget *widget);

    void beginPaint(QRegion &toClean, QWindowSurface *windowSurface, BeginPaintInfo *returnInfo);
    void endPaint(const QRegion &cleaned, QWindowSurface *windowSurface, BeginPaintInfo *beginPaintInfo);

    QRegion staticContents(QWidget *widget, const QRect &withinClipRect) const;

    void markDirtyOnScreen(const QRegion &dirtyOnScreen, QWidget *widget, const QPoint &topLevelOffset);

    void removeDirtyWidget(QWidget *w);

    void updateLists(QWidget *widget);

    inline void addDirtyWidget(QWidget *widget, const QRegion &rgn)
    {
        if (widget && !widget->d_func()->inDirtyList && !widget->data->in_destructor) {
            QWidgetPrivate *widgetPrivate = widget->d_func();
#ifndef QT_NO_GRAPHICSEFFECT
            if (widgetPrivate->graphicsEffect)
                widgetPrivate->dirty = widgetPrivate->effectiveRectFor(rgn.boundingRect());
            else
#endif //QT_NO_GRAPHICSEFFECT
                widgetPrivate->dirty = rgn;
            dirtyWidgets.append(widget);
            widgetPrivate->inDirtyList = true;
        }
    }

    inline void dirtyWidgetsRemoveAll(QWidget *widget)
    {
        for (int i = 0; i < dirtyWidgets.size(); ++i) {
            if (dirtyWidgets.at(i) == widget)
                dirtyWidgets.remove(i);
        }
    }

    inline void addStaticWidget(QWidget *widget)
    {
        if (!widget)
            return;

        Q_ASSERT(widget->testAttribute(Qt::WA_StaticContents));
        if (!staticWidgets.contains(widget))
            staticWidgets.append(widget);
    }

    inline void removeStaticWidget(QWidget *widget)
    { staticWidgets.removeAll(widget); }

    // Move the reparented widget and all its static children from this backing store
    // to the new backing store if reparented into another top-level / backing store.
    inline void moveStaticWidgets(QWidget *reparented)
    {
        Q_ASSERT(reparented);
        QWidgetBackingStore *newBs = reparented->d_func()->maybeBackingStore();
        if (newBs == this)
            return;

        int i = 0;
        while (i < staticWidgets.size()) {
            QWidget *w = staticWidgets.at(i);
            if (reparented == w || reparented->isAncestorOf(w)) {
                staticWidgets.removeAt(i);
                if (newBs)
                    newBs->addStaticWidget(w);
            } else {
                ++i;
            }
        }
    }

    inline void appendDirtyOnScreenWidget(QWidget *widget)
    {
        if (!widget)
            return;

        if (!dirtyOnScreenWidgets.contains(widget)) {
            dirtyOnScreenWidgets.append(widget);
        }
    }

    inline void dirtyOnScreenWidgetsRemoveAll(QWidget *widget)
    {
        if (!widget)
            return;

        for (int i = 0; i < dirtyOnScreenWidgets.size(); ++i) {
            if (dirtyOnScreenWidgets.at(i) == widget) {
                dirtyOnScreenWidgets.remove(i);
            }
        }
    }

    inline void resetWidget(QWidget *widget)
    {
        if (widget) {
            widget->d_func()->inDirtyList = false;
            widget->d_func()->isScrolled = false;
            widget->d_func()->isMoved = false;
            widget->d_func()->dirty = QRegion();
        }
    }

    inline void updateStaticContentsSize()
    {
        for (int i = 0; i < staticWidgets.size(); ++i) {
            QWidgetPrivate *wd = staticWidgets.at(i)->d_func();
            if (!wd->extra)
                wd->createExtra();
            wd->extra->staticContentsSize = wd->data.crect.size();
        }
    }

    inline bool hasStaticContents() const
    { return !staticWidgets.isEmpty(); }

    friend class QWidgetPrivate;
    friend class QWidget;
    friend class QETWidget;
    friend class QWindowSurface;
};

QT_END_NAMESPACE

#endif // QBACKINGSTORE_P_H
