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

#ifndef QIMAGE_P_H
#define QIMAGE_P_H

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

#include <QVector>

QT_BEGIN_NAMESPACE

class QImageWriter;

struct Q_GUI_EXPORT QImageData {        // internal image data
    QImageData();
    ~QImageData();
    static QImageData *create(const QSize &size, QImage::Format format);
    static QImageData *create(uchar *data, int w, int h,  int bpl, QImage::Format format, bool readOnly);

    QAtomicInt ref;

    int width;
    int height;
    int depth;
    int nbytes;               // number of bytes data
    QVector<QRgb> colortable;
    uchar *data;
    QImage::Format format;
    int bytes_per_line;
    int ser_no;               // serial number
    int detach_no;

    qreal  dpmx;                // dots per meter X (or 0)
    qreal  dpmy;                // dots per meter Y (or 0)

    bool own_data;
    bool ro_data;
    bool has_alpha_clut;

    bool checkForAlphaPixels() const;

    bool doImageIO(const QImage *image, QImageWriter* io, int quality) const;

    QPaintEngine *paintEngine;
};

static inline int qt_depthForFormat(QImage::Format format)
{
    switch(format) {
    case QImage::Format_Invalid:
    case QImage::NImageFormats:
        Q_ASSERT(false);
    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
        return 1;
    case QImage::Format_Indexed8:
        return 8;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        return 32;
    case QImage::Format_RGB16:
        return 16;
    }
    return 0;
}

QT_END_NAMESPACE

#endif
