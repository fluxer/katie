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

#include "qcolor.h"
#include "qimage.h"
#include "qdrawhelper_p.h"
#include "qt_x11_p.h"
#include "qx11info_x11.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

void QX11Data::copyQImageToXImage(const QImage &image, XImage *ximage, bool *freedata)
{
    Q_ASSERT(ximage);
    Q_ASSERT(freedata);

    const bool samebpl = (ximage->bytes_per_line == image.bytesPerLine());
    const bool samebyteorder = ((ximage->byte_order == MSBFirst) == (Q_BYTE_ORDER == Q_BIG_ENDIAN));
    // the XRender case (depth == 24 or 32)
    if (image.format() == QImage::Format_ARGB32_Premultiplied) {
        ximage->data = (char*)image.constBits();
        *freedata = false;
        return;
    }

    const bool samedepth = (ximage->depth == image.depth());
    // depth == 16
    if (samedepth && samebyteorder && samebpl
        && image.format() == QImage::Format_RGB16) {
        ximage->data = (char*)image.constBits();
        *freedata = false;
        return;
    }

    *freedata = true;
    ximage->data = static_cast<char*>(::malloc(size_t(ximage->bytes_per_line) * image.height()));
    Q_CHECK_PTR(ximage->data);
    const int w = image.width();
    const int h = image.height();
    // same bytes per line, same byte order
    if (samedepth && samebyteorder) {
        switch(image.format()) {
            case QImage::Format_RGB16: {
                quint16 *xidata = (quint16 *)ximage->data;
                for (int y = 0; y < h; ++y) {
                    const quint16 *p = (const quint16 *) image.constScanLine(y);
                    for (int x = 0; x < w; ++x) {
                        *xidata++ = p[x];
                    }
                }
                return;
            }
            case QImage::Format_RGB32: {
                uint *xidata = (uint *)ximage->data;
                for (int y = 0; y < h; ++y) {
                    const QRgb *p = (const QRgb *) image.constScanLine(y);
                    for (int x = 0; x < w; ++x) {
                        *xidata++ = p[x] | 0xff000000;
                    }
                }
                return;
            }
            case QImage::Format_ARGB32: {
                uint *xidata = (uint *)ximage->data;
                for (int y = 0; y < h; ++y) {
                    const QRgb *p = (const QRgb *) image.constScanLine(y);
                    for (int x = 0; x < w; ++x) {
                        const QRgb rgb = p[x];
                        const int a = qAlpha(rgb);
                        if (a == 0xff) {
                            *xidata = rgb;
                        } else {
                            // RENDER expects premultiplied alpha
                            *xidata = qRgba(qt_div_255(qRed(rgb) * a),
                                            qt_div_255(qGreen(rgb) * a),
                                            qt_div_255(qBlue(rgb) * a),
                                            a);
                        }
                        ++xidata;
                    }
                }
                return;
            }
            default: {
                break;
            }
        }
    }

    // any other case, pretty much not tested and likely to produce incorrect results
    for (int h = 0; h < image.height(); h++) {
        for (int w = 0; w < image.width(); w++) {
            const QRgb pixel = image.pixel(w, h);
            XPutPixel(ximage, w, h, pixel);
        }
    }
}

void QX11Data::destroyXImage(XImage *ximage, const bool freedata)
{
    if (!freedata) {
        ximage->data = nullptr;
    }
    XDestroyImage(ximage);
}

void QX11Data::copyXImageToQImage(XImage *ximage, QImage &image)
{
    Q_ASSERT(ximage);
    Q_ASSERT(ximage->width == image.width());
    Q_ASSERT(ximage->height == image.height());

    const bool samedepth = (ximage->depth == image.depth());
    const bool samebyteorder = ((ximage->byte_order == MSBFirst) == (Q_BYTE_ORDER == Q_BIG_ENDIAN));
    if (samedepth && samebyteorder) {
        switch (image.format()) {
            case QImage::Format_RGB32: {
                uchar *imagedata = image.bits();
                const int imagebpl = image.bytesPerLine();
                for (int h = 0; h < ximage->height; h++) {
                    uchar* imageline = QFAST_SCAN_LINE(imagedata, imagebpl, h);
                    for (int w = 0; w < ximage->width; w++) {
                        const uint xpixel = XGetPixel(ximage, w, h);
                        //make sure alpha is 255, we depend on it in qdrawhelper for cases
                        // when image is set as a texture pattern on a qbrush
                        ((uint *)imageline)[w] = (xpixel | 0xff000000);
                    }
                }
                return;
            }
            case QImage::Format_ARGB32:
            case QImage::Format_ARGB32_Premultiplied:
            case QImage::Format_RGB16: {
                ::memcpy(image.bits(), ximage->data, image.byteCount());
                return;
            }
            default: {
                break;
            }
        }
    }

    if (image.depth() == 1) {
        for (int h = 0; h < ximage->height; h++) {
            for (int w = 0; w < ximage->width; w++) {
                const uint xpixel = XGetPixel(ximage, w, h);
                image.setPixel(w, h, image.color(xpixel));
            }
        }
    } else {
        for (int h = 0; h < ximage->height; h++) {
            for (int w = 0; w < ximage->width; w++) {
                const uint xpixel = XGetPixel(ximage, w, h);
                image.setPixel(w, h, xpixel);
            }
        }
    }
}

uint QX11Data::XColorPixel(const int screen, const QColor &color)
{
    Display* x11display = QX11Info::display();
    if (!x11display) {
        // qWarning("QX11Data::XColorPixel: no display");
        return 0;
    }

    Qt::HANDLE x11colormap = QX11Info::appColormap(screen);
    if (!x11colormap) {
        // qWarning("QX11Data::XColorPixel: no color map");
        return 0;
    }

    int qr = 0;
    int qg = 0;
    int qb = 0;
    color.getRgb(
        &qr,
        &qg,
        &qb
    );

    XColor x11color;
    ::memset(&x11color, 0, sizeof(XColor));
    x11color.flags = DoRed | DoGreen | DoBlue; 
    x11color.red = qr << 8;
    x11color.green = qg << 8;
    x11color.blue = qb << 8;

    XAllocColor(x11display, x11colormap, &x11color);

    // qDebug() << Q_FUNC_INFO << color << x11color.pixel;
    return x11color.pixel;
}

QT_END_NAMESPACE
