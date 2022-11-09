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

#ifndef QPAINTENGINE_X11_P_H
#define QPAINTENGINE_X11_P_H

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

#include "qpaintengine_p.h"

typedef unsigned long Picture;

QT_BEGIN_NAMESPACE

class QX11PaintEnginePrivate;

class QX11PaintEngine : public QPaintEngine
{
    Q_DECLARE_PRIVATE(QX11PaintEngine)
public:
    QX11PaintEngine();
    ~QX11PaintEngine();

    bool begin(QPaintDevice *pdev);
    bool end();

    void updateState(const QPaintEngineState &state);

    void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr, Qt::ImageConversionFlags flags = Qt::AutoColor);
    void drawPath(const QPainterPath &path);

    inline Type type() const { return QPaintEngine::X11; }

protected:
    QX11PaintEngine(QX11PaintEnginePrivate &dptr);

    friend class QPixmap;
    friend class QFontEngineBox;

private:
    Q_DISABLE_COPY(QX11PaintEngine)
};

class QX11PaintEnginePrivate : public QPaintEnginePrivate
{
    Q_DECLARE_PUBLIC(QX11PaintEngine)
public:
    QX11PaintEnginePrivate()
    {
        hd = 0;
        xinfo = 0;
#if !defined (QT_NO_XRENDER)
        picture = 0;
#endif
        gc = 0;
    }

    void init();

    Qt::HANDLE hd;
#if !defined (QT_NO_XRENDER)
    Qt::HANDLE picture;
#endif
    GC gc;
    const QX11Info *xinfo;
};

QT_END_NAMESPACE

#endif // QPAINTENGINE_X11_P_H
