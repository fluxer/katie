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

    ximage->data = static_cast<char*>(::malloc(size_t(ximage->bytes_per_line) * image.height()));
    Q_CHECK_PTR(ximage->data);

#if 0
    for (int h = 0; h < image.height(); h++) {
        for (int w = 0; w < image.width(); w++) {
            const QRgb pixel = image.pixel(w, h);
            XPutPixel(ximage, w, h, pixel);
        }
    }
#else

    const int w = image.width();
    const int h = image.height();
    switch(image.format()) {
    case QImage::Format_Indexed8: {
        QVector<QRgb> colorTable = image.colorTable();
        uint *xidata = (uint *)ximage->data;
        for (int y = 0; y < h; ++y) {
            const uchar *p = image.constScanLine(y);
            for (int x = 0; x < w; ++x) {
                const QRgb rgb = colorTable[p[x]];
                const int a = qAlpha(rgb);
                if (a == 0xff)
                    *xidata = rgb;
                else
                    // RENDER expects premultiplied alpha
                    *xidata = qRgba(qt_div_255(qRed(rgb) * a),
                                    qt_div_255(qGreen(rgb) * a),
                                    qt_div_255(qBlue(rgb) * a),
                                    a);
                ++xidata;
            }
        }
        break;
    }
    case QImage::Format_RGB32: {
        uint *xidata = (uint *)ximage->data;
        for (int y = 0; y < h; ++y) {
            const QRgb *p = (const QRgb *) image.constScanLine(y);
            for (int x = 0; x < w; ++x)
                *xidata++ = p[x] | 0xff000000;
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
                if (a == 0xff)
                    *xidata = rgb;
                else
                    // RENDER expects premultiplied alpha
                    *xidata = qRgba(qt_div_255(qRed(rgb) * a),
                                    qt_div_255(qGreen(rgb) * a),
                                    qt_div_255(qBlue(rgb) * a),
                                    a);
                ++xidata;
            }
        }
        break;
    }
    case QImage::Format_ARGB32_Premultiplied: {
        uint *xidata = (uint *)ximage->data;
        for (int y = 0; y < h; ++y) {
            const QRgb *p = (const QRgb *) image.constScanLine(y);
            memcpy(xidata, p, w*sizeof(QRgb));
            xidata += w;
        }
        break;
    }
    default:
        Q_ASSERT(false);
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
#endif
}

void QX11Data::copyXImageToQImage(XImage *ximage, QImage &image)
{
    Q_ASSERT(ximage);

    QImage qimage(ximage->height, ximage->height, QImage::systemFormat());
    for (int h = 0; h < ximage->height; h++) {
        for (int w = 0; w < ximage->width; w++) {
            const unsigned long xpixel = XGetPixel(ximage, w, h);
            qimage.setPixel(w, h, xpixel);
        }
    }
    image = qimage;
}

QT_END_NAMESPACE
