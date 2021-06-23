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

#ifndef QWINDOWSURFACE_RASTER_P_H
#define QWINDOWSURFACE_RASTER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qwindowsurface_p.h"

QT_BEGIN_NAMESPACE

class QPaintDevice;
class QPoint;
class QRegion;
class QRegion;
class QSize;
class QWidget;
class QRasterWindowSurfacePrivate;

class Q_GUI_EXPORT QRasterWindowSurface : public QWindowSurface
{
public:
    QRasterWindowSurface(QWidget *widget);
    ~QRasterWindowSurface();

    QPaintDevice *paintDevice();
    void flush(QWidget *widget, const QRegion &region, const QPoint &offset);
    void beginPaint(const QRegion &rgn);
    void setGeometry(const QRect &rect);
    bool scroll(const QRegion &area, int dx, int dy);

private:
    void prepareBuffer(QImage::Format format);
    Q_DECLARE_PRIVATE(QRasterWindowSurface)
    QScopedPointer<QRasterWindowSurfacePrivate> d_ptr;
};

QT_END_NAMESPACE

#endif // QWINDOWSURFACE_RASTER_P_H
