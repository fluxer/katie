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
#include "qbitmap.h"
#include "qdrawhelper_p.h"
#include "qt_x11_p.h"
#include "qx11info_x11.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_XRENDER
XRenderColor QX11Data::preMultiply(const QColor &c)
{
    XRenderColor color;
    const uint A = c.alpha(),
               R = c.red(),
               G = c.green(),
               B = c.blue();
    color.alpha = (A | A << 8);
    color.red   = (R | R << 8) * color.alpha / 0x10000;
    color.green = (G | G << 8) * color.alpha / 0x10000;
    color.blue  = (B | B << 8) * color.alpha / 0x10000;
    return color;
}

Picture QX11Data::getSolidFill(int screen, const QColor &c)
{
    if (!qt_x11Data->use_xrender)
        return XNone;

    XRenderColor color = preMultiply(c);
    for (int i = 0; i < QX11Data::solid_fill_count; ++i) {
        if (qt_x11Data->solid_fills[i].screen == screen
            && qt_x11Data->solid_fills[i].color.alpha == color.alpha
            && qt_x11Data->solid_fills[i].color.red == color.red
            && qt_x11Data->solid_fills[i].color.green == color.green
            && qt_x11Data->solid_fills[i].color.blue == color.blue)
            return qt_x11Data->solid_fills[i].picture;
    }
    // none found, replace one
    int i = qrand() % QX11Data::solid_fill_count;

    if (qt_x11Data->solid_fills[i].screen != screen && qt_x11Data->solid_fills[i].picture) {
        XRenderFreePicture (qt_x11Data->display, qt_x11Data->solid_fills[i].picture);
        qt_x11Data->solid_fills[i].picture = 0;
    }

    if (!qt_x11Data->solid_fills[i].picture) {
        Pixmap pixmap = XCreatePixmap (qt_x11Data->display, RootWindow (qt_x11Data->display, screen), 1, 1, 32);
        XRenderPictureAttributes attrs;
        attrs.repeat = True;
        qt_x11Data->solid_fills[i].picture = XRenderCreatePicture (qt_x11Data->display, pixmap,
                                                            XRenderFindStandardFormat(qt_x11Data->display, PictStandardARGB32),
                                                            CPRepeat, &attrs);
        XFreePixmap (qt_x11Data->display, pixmap);
    }

    qt_x11Data->solid_fills[i].color = color;
    qt_x11Data->solid_fills[i].screen = screen;
    XRenderFillRectangle (qt_x11Data->display, PictOpSrc, qt_x11Data->solid_fills[i].picture, &color, 0, 0, 1, 1);
    return qt_x11Data->solid_fills[i].picture;
}
#endif

void QX11Data::copyQImageToXImage(const QImage &image, XImage *ximage)
{
    Q_ASSERT(ximage);
    Q_ASSERT(image.depth() == 32);

    ximage->data = static_cast<char*>(::malloc(size_t(ximage->bytes_per_line) * image.height()));
    Q_CHECK_PTR(ximage->data);

    const int w = image.width();
    const int h = image.height();

    if (ximage->bits_per_pixel == image.depth()) {
        switch(image.format()) {
            case QImage::Format_RGB32: {
                uint *xidata = (uint *)ximage->data;
                for (int y = 0; y < h; ++y) {
                    const QRgb *p = (const QRgb *) image.constScanLine(y);
                    for (int x = 0; x < w; ++x) {
                        *xidata++ = p[x] | 0xff000000;
                    }
                }
                break;
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
                break;
            }
            case QImage::Format_ARGB32_Premultiplied: {
                ::memcpy(ximage->data, image.constBits(), image.byteCount());
                break;
            }
            default: {
                Q_ASSERT(false);
                break;
            }
        }

        if ((ximage->byte_order == MSBFirst) != (Q_BYTE_ORDER == Q_BIG_ENDIAN)) {
            uint *xidata = (uint *)ximage->data;
            uint *xiend = xidata + w*h;
            while (xidata < xiend) {
                *xidata = (*xidata >> 24)
                            | ((*xidata >> 8) & 0xff00)
                            | ((*xidata << 8) & 0xff0000)
                            | (*xidata << 24);
                ++xidata;
            }
        }

        return;
    }

    for (int h = 0; h < image.height(); h++) {
        for (int w = 0; w < image.width(); w++) {
            const QRgb pixel = image.pixel(w, h);
            XPutPixel(ximage, w, h, pixel);
        }
    }
}

void QX11Data::copyXImageToQImage(XImage *ximage, QImage &image)
{
    Q_ASSERT(ximage);
    Q_ASSERT(ximage->width == image.width());
    Q_ASSERT(ximage->height == image.height());

    if (ximage->bits_per_pixel == image.depth()) {
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
            case QImage::Format_ARGB32_Premultiplied: {
                uchar *imagedata = image.bits();
                const int imagebpl = image.bytesPerLine();
                for (int h = 0; h < ximage->height; h++) {
                    uchar* imageline = QFAST_SCAN_LINE(imagedata, imagebpl, h);
                    for (int w = 0; w < ximage->width; w++) {
                        const uint xpixel = XGetPixel(ximage, w, h);
                        ((uint *)imageline)[w] = xpixel;
                    }
                }
                return;
            }
            case QImage::Format_RGB16: {
                uchar *imagedata = image.bits();
                const int imagebpl = image.bytesPerLine();
                for (int h = 0; h < ximage->height; h++) {
                    uchar* imageline = QFAST_SCAN_LINE(imagedata, imagebpl, h);
                    for (int w = 0; w < ximage->width; w++) {
                        const quint16 xpixel = XGetPixel(ximage, w, h);
                        ((quint16 *)imageline)[w] = xpixel;
                    }
                }
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
