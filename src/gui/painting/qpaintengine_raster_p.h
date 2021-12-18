/****************************************************************************
**
** Copyright (C) 2021 Ivailo Monev
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

#ifndef QPAINTENGINE_RASTER_P_H
#define QPAINTENGINE_RASTER_P_H

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

#include "qpaintengine.h"
#include "qpaintengine_p.h"

#include <cairo/cairo.h>

QT_BEGIN_NAMESPACE

class QRasterPaintEnginePrivate;

class QRasterPaintEngine : public QPaintEngine
{
    Q_DECLARE_PRIVATE(QRasterPaintEngine)
public:
    QRasterPaintEngine();
    ~QRasterPaintEngine();

    bool begin(QPaintDevice *pdev) final;
    bool end() final;

    void updateState(const QPaintEngineState &state) final;

    void drawPath(const QPainterPath &path) final;
    void drawPolygon(const QPointF *points, int pointCount, QPaintEngine::PolygonDrawMode mode) final;
    void drawPixmap(const QRectF &r, const QPixmap &pixmap, const QRectF &sr) final;
    void drawImage(const QRectF &r, const QImage &image, const QRectF &sr,
                   Qt::ImageConversionFlags flags = Qt::AutoColor) final;

    QPaintEngine::Type type() const final { return QPaintEngine::Raster; }

protected:
    QRasterPaintEngine(QRasterPaintEnginePrivate &dptr);

private:
    void updateFont(const QFont &font);

    cairo_pattern_t* backgroundPattern(const QColor &color);
    cairo_pattern_t* imagePattern(const QImage &image, const Qt::ImageConversionFlags flags = Qt::AutoColor);
    cairo_pattern_t* penPattern(const QPen &pen);
    cairo_pattern_t* brushPattern(const QBrush &brush);
    void pushPattern(cairo_pattern_t* cairopattern);
    void popPattern(cairo_pattern_t* cairopattern);

    void strokeAndFill(const cairo_fill_rule_t cairorule);

    Q_DISABLE_COPY(QRasterPaintEngine)
};

class QRasterPaintEnginePrivate : public QPaintEnginePrivate
{
    Q_DECLARE_PUBLIC(QRasterPaintEngine)
public:
    QRasterPaintEnginePrivate();
    ~QRasterPaintEnginePrivate();

    cairo_t* m_cairo;
    cairo_surface_t* m_cairosurface;
    cairo_pattern_t* m_cairobackground;
    uchar* m_imagebits;
    cairo_filter_t m_cairofilter;
};

QT_END_NAMESPACE

#endif // QPAINTENGINE_RASTER_P_H
