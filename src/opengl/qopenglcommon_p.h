#ifndef QOPENGLCOMMON_P_H
#define QOPENGLCOMMON_P_H

#include "qgl_p.h"
#include "qrect.h"

#if !defined(QT_OPENGL_ES_1)
#include "qpaintengineex_opengl2_p.h"
Q_GLOBAL_STATIC(QGLEngineThreadStorage<QGL2PaintEngineEx>, qt_buffer_2_engine)
#endif

#ifndef QT_OPENGL_ES_2
#include "qpaintengine_opengl_p.h"
Q_GLOBAL_STATIC(QGLEngineThreadStorage<QOpenGLPaintEngine>, qt_buffer_engine)
#endif

static inline const QRectF scaleRect(const QRectF &r, qreal sx, qreal sy)
{
    return QRectF(r.x() * sx, r.y() * sy, r.width() * sx, r.height() * sy);
}

#endif // QOPENGLCOMMON_P_H
