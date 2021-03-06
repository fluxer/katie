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

#ifndef QWINDOWSURFACE_P_H
#define QWINDOWSURFACE_P_H

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

#include <QtGui/qwidget.h>

QT_BEGIN_NAMESPACE

class QPaintDevice;
class QRegion;
class QRect;
class QPoint;
class QImage;
class QWindowSurfacePrivate;

class Q_GUI_EXPORT QWindowSurface
{
public:
    enum WindowSurfaceFeature {
        PartialUpdates               = 0x00000001, // Supports doing partial updates.
        PreservedContents            = 0x00000002, // Supports doing flush without first doing a repaint.
        StaticContents               = 0x00000004, // Supports having static content regions when being resized.
        AllFeatures                  = 0xffffffff  // For convenience
    };
    Q_DECLARE_FLAGS(WindowSurfaceFeatures, WindowSurfaceFeature)

    QWindowSurface(QWidget *window, bool setDefaultSurface = true);
    virtual ~QWindowSurface();

    QWidget *window() const;

    virtual QPaintDevice *paintDevice() = 0;

    // 'widget' can be a child widget, in which case 'region' is in child widget coordinates and
    // offset is the (child) widget's offset in relation to the window surface. On QWS, 'offset'
    // can be larger than just the offset from the top-level widget as there may also be window
    // decorations which are painted into the window surface.
    virtual void flush(QWidget *widget, const QRegion &region, const QPoint &offset) = 0;
    virtual void setGeometry(const QRect &rect);
    QRect geometry() const;

    virtual bool scroll(const QRegion &area, int dx, int dy);

    virtual void beginPaint(const QRegion &);
    virtual void endPaint(const QRegion &);

    virtual QImage* buffer(const QWidget *widget);
    virtual QPixmap grabWidget(const QWidget *widget) const;

    virtual QPoint offset(const QWidget *widget) const;
    inline QRect rect(const QWidget *widget) const;

    bool hasFeature(WindowSurfaceFeature feature) const;
    virtual WindowSurfaceFeatures features() const;

    void setStaticContents(const QRegion &region);
    QRegion staticContents() const;

protected:
    bool hasStaticContents() const;

private:
    QWindowSurfacePrivate *d_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QWindowSurface::WindowSurfaceFeatures)

inline QRect QWindowSurface::rect(const QWidget *widget) const
{
    return widget->rect().translated(offset(widget));
}

inline bool QWindowSurface::hasFeature(WindowSurfaceFeature feature) const
{
    return (features() & feature) != 0;
}

QT_END_NAMESPACE

#endif // QWINDOWSURFACE_P_H
