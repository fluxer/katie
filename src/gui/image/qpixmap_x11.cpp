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

#include "qplatformdefs.h"
#include "qdebug.h"
#include "qiodevice.h"
#include "qpixmap_x11_p.h"
#include "qbitmap.h"
#include "qcolormap.h"
#include "qimage.h"
#include "qmatrix.h"
#include "qapplication.h"
#include "qpaintengine_x11_p.h"
#include "qt_x11_p.h"
#include "qx11info_x11.h"
#include "qdrawhelper_p.h"
#include "qimage_p.h"
#include "qcorecommon_p.h"
#include "qguicommon_p.h"

#include <stdlib.h>

QT_BEGIN_NAMESPACE

QPixmap qt_toX11Pixmap(const QImage &image)
{
    QPixmapData *data =
        new QX11PixmapData(image.depth() == 1
                           ? QPixmapData::BitmapType
                           : QPixmapData::PixmapType);

    data->fromImage(image, Qt::AutoColor);

    return QPixmap(data);
}

QPixmap qt_toX11Pixmap(const QPixmap &pixmap)
{
    if (pixmap.isNull())
        return QPixmap();

    if (pixmap.data->classId() == QPixmapData::X11Class)
        return pixmap;

    return qt_toX11Pixmap(pixmap.toImage());
}

// For thread-safety:
//   image->data does not belong to X11, so we must free it ourselves.

inline static void qSafeXDestroyImage(XImage *x)
{
    if (x->data) {
        free(x->data);
        x->data = 0;
    }
    XDestroyImage(x);
}

QBitmap QX11PixmapData::mask_to_bitmap(int screen) const
{
    if (!x11_mask)
        return QBitmap();
    QPixmap::x11SetDefaultScreen(screen);
    QBitmap bm(w, h);
    GC gc = XCreateGC(qt_x11Data->display, bm.handle(), 0, 0);
    XCopyArea(qt_x11Data->display, x11_mask, bm.handle(), gc, 0, 0,
              bm.data->width(), bm.data->height(), 0, 0);
    XFreeGC(qt_x11Data->display, gc);
    return bm;
}

Qt::HANDLE QX11PixmapData::bitmap_to_mask(const QBitmap &bitmap, int screen)
{
    if (bitmap.isNull())
        return 0;
    QBitmap bm = bitmap;
    bm.x11SetScreen(screen);

    Pixmap mask = XCreatePixmap(qt_x11Data->display, RootWindow(qt_x11Data->display, screen),
                                bm.data->width(), bm.data->height(), 1);
    GC gc = XCreateGC(qt_x11Data->display, mask, 0, 0);
    XCopyArea(qt_x11Data->display, bm.handle(), mask, gc, 0, 0,
              bm.data->width(), bm.data->height(), 0, 0);
    XFreeGC(qt_x11Data->display, gc);
    return mask;
}

/*****************************************************************************
  Internal functions
 *****************************************************************************/

// Returns position of highest bit set or -1 if none
static int highest_bit(uint v)
{
    int i;
    uint b = (uint)1 << 31;
    for (i=31; ((b & v) == 0) && i>=0;         i--)
        b >>= 1;
    return i;
}

// Counts the number of bits set in 'v'
static uint n_bits(uint v)
{
    int i = 0;
    while (v) {
        v = v & (v - 1);
        i++;
    }
    return i;
}

static uint *red_scale_table   = 0;
static uint *green_scale_table = 0;
static uint *blue_scale_table  = 0;

static void cleanup_scale_tables()
{
    delete[] red_scale_table;
    delete[] green_scale_table;
    delete[] blue_scale_table;
}

/*
  Could do smart bitshifting, but the "obvious" algorithm only works for
  nBits >= 4. This is more robust.
*/
static void build_scale_table(uint **table, uint nBits)
{
    if (Q_UNLIKELY(nBits > 7)) {
        qWarning("build_scale_table: internal error, nBits = %i", nBits);
        return;
    }
    if (!*table) {
        static bool firstTable = true;
        if (firstTable) {
            qAddPostRoutine(cleanup_scale_tables);
            firstTable = false;
        }
        *table = new uint[256];
    }
    int   maxVal   = (1 << nBits) - 1;
    int   valShift = 8 - nBits;
    int i;
    for(i = 0 ; i < maxVal + 1 ; i++)
        (*table)[i << valShift] = i*255/maxVal;
}

static int defaultScreen = -1;

/*****************************************************************************
  QPixmap member functions
 *****************************************************************************/

QAtomicInt qt_pixmap_serial(0);

QX11PixmapData::QX11PixmapData(PixelType type)
    : QPixmapData(type, X11Class), hd(0),
      flags(NoFlags), x11_mask(0), picture(0), mask_picture(0), hd2(0),
      share_mode(QPixmap::ImplicitlyShared), pengine(0)
{
}

QPixmapData *QX11PixmapData::createCompatiblePixmapData() const
{
    return new QX11PixmapData(pixelType());
}

void QX11PixmapData::resize(int width, int height)
{
    setSerialNumber(qt_pixmap_serial.fetchAndAddRelaxed(1));

    w = width;
    h = height;
    is_null = (w <= 0 || h <= 0);

    if (defaultScreen >= 0 && defaultScreen != xinfo.screen()) {
        QX11InfoData* xd = xinfo.getX11Data();
        xd->screen = defaultScreen;
        xd->depth = QX11Info::appDepth(xd->screen);
        xd->cells = QX11Info::appCells(xd->screen);
        xd->colormap = QX11Info::appColormap(xd->screen);
        xd->defaultColormap = QX11Info::appDefaultColormap(xd->screen);
        xd->visual = (Visual *)QX11Info::appVisual(xd->screen);
        xd->defaultVisual = QX11Info::appDefaultVisual(xd->screen);
        xinfo.setX11Data(xd);
    }

    const int dd = xinfo.depth();

    bool make_null = w <= 0 || h <= 0;                // create null pixmap
    d = (pixelType() == BitmapType ? 1 : dd);
    if (make_null || d == 0) {
        w = 0;
        h = 0;
        is_null = true;
        hd = 0;
        picture = 0;
        d = 0;
        if (Q_UNLIKELY(!make_null))
            qWarning("QPixmap: Invalid pixmap parameters");
        return;
    }
    hd = (Qt::HANDLE)XCreatePixmap(qt_x11Data->display,
                                   RootWindow(qt_x11Data->display, xinfo.screen()),
                                   w, h, d);
#ifndef QT_NO_XRENDER
    if (qt_x11Data->use_xrender) {
        XRenderPictFormat *format = d == 1
                                    ? XRenderFindStandardFormat(qt_x11Data->display, PictStandardA1)
                                    : XRenderFindVisualFormat(qt_x11Data->display, (Visual *)xinfo.visual());
        picture = XRenderCreatePicture(qt_x11Data->display, hd, format, 0, 0);
    }
#endif // QT_NO_XRENDER
}

struct QX11AlphaDetector
{
    bool hasAlpha() const {
        if (checked)
            return has;
        // Will implicitly also check format and return quickly for opaque types...
        checked = true;
        has = image->isNull() ? false : image->d->checkForAlphaPixels();
        return has;
    }

    bool hasXRenderAndAlpha() const {
        if (!qt_x11Data->use_xrender)
            return false;
        return hasAlpha();
    }

    QX11AlphaDetector(const QImage *i, Qt::ImageConversionFlags flags)
        : image(i), checked(false), has(false)
    {
        if (flags & Qt::NoOpaqueDetection) {
            checked = true;
            has = image->hasAlphaChannel();
        }
    }

    const QImage *image;
    mutable bool checked;
    mutable bool has;
};

void QX11PixmapData::fromImage(const QImage &img,
                               Qt::ImageConversionFlags flags)
{
    setSerialNumber(qt_pixmap_serial.fetchAndAddRelaxed(1));

    w = img.width();
    h = img.height();
    d = img.depth();
    is_null = (w <= 0 || h <= 0);

    if (is_null) {
        w = h = 0;
        return;
    }

    if (defaultScreen >= 0 && defaultScreen != xinfo.screen()) {
        QX11InfoData* xd = xinfo.getX11Data();
        xd->screen = defaultScreen;
        xd->depth = QX11Info::appDepth(xd->screen);
        xd->cells = QX11Info::appCells(xd->screen);
        xd->colormap = QX11Info::appColormap(xd->screen);
        xd->defaultColormap = QX11Info::appDefaultColormap(xd->screen);
        xd->visual = (Visual *)QX11Info::appVisual(xd->screen);
        xd->defaultVisual = QX11Info::appDefaultVisual(xd->screen);
        xinfo.setX11Data(xd);
    }

    if (pixelType() == BitmapType || d == 1) {
        bitmapFromImage(img);
        return;
    }

    if (w >= 32768 || h >= 32768) {
        w = h = 0;
        is_null = true;
        return;
    }

    QX11AlphaDetector alphaCheck(&img, flags);
    const int dd = alphaCheck.hasXRenderAndAlpha() ? 32 : xinfo.depth();

    Display *dpy   = qt_x11Data->display;
    Visual *visual = (Visual *)xinfo.visual();
    XImage *xi = nullptr;
    uchar  *newbits = nullptr;

    QImage image(img);

#ifndef QT_NO_XRENDER
    if (alphaCheck.hasXRenderAndAlpha()) {
        if (d == 16) {
            image = image.convertToFormat(QImage::Format_RGB32, flags);
        }

        d = 32;

        if (QX11Info::appDepth() != d) {
            if (xinfo.x11data) {
                xinfo.x11data->depth = d;
            } else {
                QX11InfoData *xd = xinfo.getX11Data();
                xd->screen = QX11Info::appScreen();
                xd->depth = d;
                xd->cells = QX11Info::appCells();
                xd->colormap = QX11Info::appColormap();
                xd->defaultColormap = QX11Info::appDefaultColormap();
                xd->visual = (Visual *)QX11Info::appVisual();
                xd->defaultVisual = QX11Info::appDefaultVisual();
                xinfo.setX11Data(xd);
            }
        }

        hd = (Qt::HANDLE)XCreatePixmap(dpy, RootWindow(dpy, xinfo.screen()),
                                       w, h, d);
        picture = XRenderCreatePicture(qt_x11Data->display, hd,
                                       XRenderFindStandardFormat(qt_x11Data->display, PictStandardARGB32), 0, 0);

        xi = XCreateImage(dpy, visual, d, ZPixmap, 0, 0, w, h, 32, 0);
        Q_CHECK_PTR(xi);
        newbits = (uchar *)::malloc(size_t(xi->bytes_per_line) * h);
        Q_CHECK_PTR(newbits);
        xi->data = (char *)newbits;

        switch(image.format()) {
        case QImage::Format_Indexed8: {
            QVector<QRgb> colorTable = image.colorTable();
            uint *xidata = (uint *)xi->data;
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
            uint *xidata = (uint *)xi->data;
            for (int y = 0; y < h; ++y) {
                const QRgb *p = (const QRgb *) image.constScanLine(y);
                for (int x = 0; x < w; ++x)
                    *xidata++ = p[x] | 0xff000000;
            }
            break;
        }
        case QImage::Format_ARGB32: {
            uint *xidata = (uint *)xi->data;
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
            uint *xidata = (uint *)xi->data;
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

        if ((xi->byte_order == MSBFirst) != (Q_BYTE_ORDER == Q_BIG_ENDIAN)) {
            uint *xidata = (uint *)xi->data;
            uint *xiend = xidata + w*h;
            while (xidata < xiend) {
                *xidata = (*xidata >> 24)
                          | ((*xidata >> 8) & 0xff00)
                          | ((*xidata << 8) & 0xff0000)
                          | (*xidata << 24);
                ++xidata;
            }
        }

        GC gc = XCreateGC(dpy, hd, 0, 0);
        XPutImage(dpy, hd, gc, xi, 0, 0, 0, 0, w, h);
        XFreeGC(dpy, gc);

        qSafeXDestroyImage(xi);

        return;
    }
#endif // QT_NO_XRENDER

    if (!xi) {                                      // X image not created
        xi = XCreateImage(dpy, visual, dd, ZPixmap, 0, 0, w, h, 32, 0);
        Q_CHECK_PTR(xi);
        newbits = (uchar *)::malloc(size_t(xi->bytes_per_line) * h);
        Q_CHECK_PTR(newbits);
        xi->data = (char *)newbits;

        if (image.format() != QImage::Format_RGB32) {
            image = image.convertToFormat(QImage::Format_RGB32, flags);
        }

        uint *xidata = (uint *)xi->data;
        for (int y = 0; y < h; ++y) {
            const QRgb *p = (const QRgb *) image.constScanLine(y);
            for (int x = 0; x < w; ++x)
                *xidata++ = p[x] | 0xff000000;
        }
    }

    hd = (Qt::HANDLE)XCreatePixmap(qt_x11Data->display,
                                   RootWindow(qt_x11Data->display, xinfo.screen()),
                                   w, h, dd);

    GC gc = XCreateGC(dpy, hd, 0, 0);
    XPutImage(dpy, hd, gc, xi, 0, 0, 0, 0, w, h);
    XFreeGC(dpy, gc);

    qSafeXDestroyImage(xi);
    d = dd;

#ifndef QT_NO_XRENDER
    if (qt_x11Data->use_xrender) {
        XRenderPictFormat *format = d == 1
                                    ? XRenderFindStandardFormat(qt_x11Data->display, PictStandardA1)
                                    : XRenderFindVisualFormat(qt_x11Data->display, (Visual *)xinfo.visual());
        picture = XRenderCreatePicture(qt_x11Data->display, hd, format, 0, 0);
    }
#endif

    if (alphaCheck.hasAlpha()) {
        QBitmap m = QBitmap::fromImage(image.createAlphaMask(flags));
        setMask(m);
    }
}

Qt::HANDLE QX11PixmapData::createBitmapFromImage(const QImage &image)
{
    QImage img = image.convertToFormat(QImage::Format_MonoLSB);
    if (img.color(0) == qt_blackrgb && img.color(1) == qt_whitergb) {
        img.invertPixels();
        img.setColor(0, qt_whitergb);
        img.setColor(1, qt_blackrgb);
    }

    char  *bits;
    uchar *tmp_bits = nullptr;
    int w = img.width();
    int h = img.height();
    int bpl = (w + 7) / 8;
    int ibpl = img.bytesPerLine();
    if (bpl != ibpl) {
        tmp_bits = new uchar[bpl*h];
        bits = (char *)tmp_bits;
        uchar *b = tmp_bits;
        const uchar *p = img.constScanLine(0);
        for (int y = 0; y < h; y++) {
            memcpy(b, p, bpl);
            b += bpl;
            p += ibpl;
        }
    } else {
        bits = (char *)img.bits();
    }
    Qt::HANDLE hd = (Qt::HANDLE)XCreateBitmapFromData(qt_x11Data->display,
                                           QX11Info::appRootWindow(),
                                           bits, w, h);
    if (tmp_bits)                                // Avoid purify complaint
        delete [] tmp_bits;
    return hd;
}

void QX11PixmapData::bitmapFromImage(const QImage &image)
{
    w = image.width();
    h = image.height();
    d = 1;
    is_null = (w <= 0 || h <= 0);
    hd = createBitmapFromImage(image);
#ifndef QT_NO_XRENDER
    if (qt_x11Data->use_xrender)
        picture = XRenderCreatePicture(qt_x11Data->display, hd,
                                       XRenderFindStandardFormat(qt_x11Data->display, PictStandardA1), 0, 0);
#endif // QT_NO_XRENDER
}

void QX11PixmapData::fill(const QColor &fillColor)
{
    if (fillColor.alpha() != 255) {
#ifndef QT_NO_XRENDER
        if (qt_x11Data->use_xrender) {
            if (!picture || d != 32)
                convertToARGB32(/*preserveContents = */false);

            ::Picture src  = qt_x11Data->getSolidFill(xinfo.screen(), fillColor);
            XRenderComposite(qt_x11Data->display, PictOpSrc, src, 0, picture,
                             0, 0, width(), height(),
                             0, 0, width(), height());
        } else
#endif
        {
            QImage im(width(), height(), QImage::Format_ARGB32_Premultiplied);
            im.fill(PREMUL(fillColor.rgba()));
            release();
            fromImage(im, Qt::AutoColor | Qt::OrderedAlphaDither);
        }
        return;
    }

    GC gc = XCreateGC(qt_x11Data->display, hd, 0, 0);
    if (depth() == 1) {
        XSetForeground(qt_x11Data->display, gc, qGray(fillColor.rgb()) > 127 ? 0 : 1);
    } else if (qt_x11Data->use_xrender && d >= 24) {
        XSetForeground(qt_x11Data->display, gc, fillColor.rgba());
    } else {
        XSetForeground(qt_x11Data->display, gc,
                       QColormap::instance(xinfo.screen()).pixel(fillColor));
    }
    XFillRectangle(qt_x11Data->display, hd, gc, 0, 0, width(), height());
    XFreeGC(qt_x11Data->display, gc);
}

QX11PixmapData::~QX11PixmapData()
{
    release();
}

void QX11PixmapData::release()
{
    delete pengine;
    pengine = 0;

    if (!qt_x11Data) {
        // At this point, the X server will already have freed our resources,
        // so there is nothing to do.
        return;
    }

    if (x11_mask) {
#ifndef QT_NO_XRENDER
        if (mask_picture)
            XRenderFreePicture(qt_x11Data->display, mask_picture);
        mask_picture = 0;
#endif
        XFreePixmap(qt_x11Data->display, x11_mask);
        x11_mask = 0;
    }

    if (hd) {
#ifndef QT_NO_XRENDER
        if (picture) {
            XRenderFreePicture(qt_x11Data->display, picture);
            picture = 0;
        }
#endif // QT_NO_XRENDER

        if (hd2) {
            XFreePixmap(xinfo.display(), hd2);
            hd2 = 0;
        }
        if (!(flags & Readonly))
            XFreePixmap(xinfo.display(), hd);
        hd = 0;
    }
}

QPixmap QX11PixmapData::alphaChannel() const
{
    if (!hasAlphaChannel()) {
        QPixmap pm(w, h);
        pm.fill(Qt::white);
        return pm;
    }
    QImage im(toImage());
    return QPixmap::fromImage(im.alphaChannel(), Qt::OrderedDither);
}

void QX11PixmapData::setAlphaChannel(const QPixmap &alpha)
{
    QImage image(toImage());
    image.setAlphaChannel(alpha.toImage());
    release();
    fromImage(image, Qt::OrderedDither | Qt::OrderedAlphaDither);
}


QBitmap QX11PixmapData::mask() const
{
    QBitmap mask;
#ifndef QT_NO_XRENDER
    if (picture && d == 32) {
        // #### slow - there must be a better way..
        mask = QBitmap::fromImage(toImage().createAlphaMask());
    } else
#endif
    if (d == 1) {
        QX11PixmapData *that = const_cast<QX11PixmapData*>(this);
        mask = QPixmap(that);
    } else {
        mask = mask_to_bitmap(xinfo.screen());
    }
    return mask;
}

/*!
    Sets a mask bitmap.

    The \a newmask bitmap defines the clip mask for this pixmap. Every
    pixel in \a newmask corresponds to a pixel in this pixmap. Pixel
    value 1 means opaque and pixel value 0 means transparent. The mask
    must have the same size as this pixmap.

    \warning Setting the mask on a pixmap will cause any alpha channel
    data to be cleared. For example:
    \snippet doc/src/snippets/image/image.cpp 2
    Now, alpha and alphacopy are visually different.

    Setting a null mask resets the mask.

    The effect of this function is undefined when the pixmap is being
    painted on.

    \sa mask(), {QPixmap#Pixmap Transformations}{Pixmap
    Transformations}, QBitmap
*/
void QX11PixmapData::setMask(const QBitmap &newmask)
{
    if (newmask.isNull()) { // clear mask
#ifndef QT_NO_XRENDER
        if (picture && d == 32) {
            QX11PixmapData newData(pixelType());
            newData.resize(w, h);
            newData.fill(Qt::black);
            XRenderComposite(qt_x11Data->display, PictOpOver,
                             picture, 0, newData.picture,
                             0, 0, 0, 0, 0, 0, w, h);
            release();
            *this = newData;
            // the new QX11PixmapData object isn't referenced yet, so
            // ref it
            ref.ref();

            // the below is to make sure the QX11PixmapData destructor
            // doesn't delete our newly created render picture
            newData.hd = 0;
            newData.x11_mask = 0;
            newData.picture = 0;
            newData.mask_picture = 0;
            newData.hd2 = 0;
        } else
#endif
            if (x11_mask) {
#ifndef QT_NO_XRENDER
                if (picture) {
                    XRenderPictureAttributes attrs;
                    attrs.alpha_map = 0;
                    XRenderChangePicture(qt_x11Data->display, picture, CPAlphaMap,
                                         &attrs);
                }
                if (mask_picture)
                    XRenderFreePicture(qt_x11Data->display, mask_picture);
                mask_picture = 0;
#endif
                XFreePixmap(qt_x11Data->display, x11_mask);
                x11_mask = 0;
            }
        return;
    }

#ifndef QT_NO_XRENDER
    if (picture && d == 32) {
        XRenderComposite(qt_x11Data->display, PictOpSrc,
                         picture, newmask.x11PictureHandle(),
                         picture, 0, 0, 0, 0, 0, 0, w, h);
    } else
#endif
        if (depth() == 1) {
            XGCValues vals;
            vals.function = GXand;
            GC gc = XCreateGC(qt_x11Data->display, hd, GCFunction, &vals);
            XCopyArea(qt_x11Data->display, newmask.handle(), hd, gc, 0, 0,
                      width(), height(), 0, 0);
            XFreeGC(qt_x11Data->display, gc);
        } else {
            // ##### should or the masks together
            if (x11_mask) {
                XFreePixmap(qt_x11Data->display, x11_mask);
#ifndef QT_NO_XRENDER
                if (mask_picture)
                    XRenderFreePicture(qt_x11Data->display, mask_picture);
#endif
            }
            x11_mask = QX11PixmapData::bitmap_to_mask(newmask, xinfo.screen());
#ifndef QT_NO_XRENDER
            if (picture) {
                mask_picture = XRenderCreatePicture(qt_x11Data->display, x11_mask,
                                                    XRenderFindStandardFormat(qt_x11Data->display, PictStandardA1), 0, 0);
                XRenderPictureAttributes attrs;
                attrs.alpha_map = mask_picture;
                XRenderChangePicture(qt_x11Data->display, picture, CPAlphaMap, &attrs);
            }
#endif
        }
}

int QX11PixmapData::metric(QPaintDevice::PaintDeviceMetric metric) const
{
    switch (metric) {
    case QPaintDevice::PdmWidth:
        return w;
    case QPaintDevice::PdmHeight:
        return h;
    case QPaintDevice::PdmNumColors:
        return 1 << d;
    case QPaintDevice::PdmDepth:
        return d;
    case QPaintDevice::PdmWidthMM: {
        const int screen = xinfo.screen();
        const int mm = DisplayWidthMM(qt_x11Data->display, screen) * w
                       / DisplayWidth(qt_x11Data->display, screen);
        return mm;
    }
    case QPaintDevice::PdmHeightMM: {
        const int screen = xinfo.screen();
        const int mm = (DisplayHeightMM(qt_x11Data->display, screen) * h)
                       / DisplayHeight(qt_x11Data->display, screen);
        return mm;
    }
    case QPaintDevice::PdmDpiX:
    case QPaintDevice::PdmPhysicalDpiX:
        return QX11Info::appDpiX(xinfo.screen());
    case QPaintDevice::PdmDpiY:
    case QPaintDevice::PdmPhysicalDpiY:
        return QX11Info::appDpiY(xinfo.screen());
    }

    qWarning("QX11PixmapData::metric(): Invalid metric");
    return 0;
}

bool QX11PixmapData::canTakeQImageFromXImage(const XImage *xi) const
{
    // ARGB32_Premultiplied
    if (picture && depth() == 32)
        return true;

    Visual *visual = (Visual *)xinfo.visual();

    // RGB32
    if (depth() == 24 && xi->bits_per_pixel == 32 && visual->red_mask == 0xff0000
        && visual->green_mask == 0xff00 && visual->blue_mask == 0xff)
        return true;

    // RGB16
    if (depth() == 16 && xi->bits_per_pixel == 16 && visual->red_mask == 0xf800
        && visual->green_mask == 0x7e0 && visual->blue_mask == 0x1f)
        return true;

    return false;
}

QImage QX11PixmapData::takeQImageFromXImage(XImage *xi) const
{
    QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    if (depth() == 24)
        format = QImage::Format_RGB32;
    else if (depth() == 16)
        format = QImage::Format_RGB16;

    QImage image((uchar *)xi->data, xi->width, xi->height, xi->bytes_per_line, format);
    // take ownership
    image.d->own_data = true;
    xi->data = 0;

    // we may have to swap the byte order
    if ((Q_BYTE_ORDER == Q_LITTLE_ENDIAN && xi->byte_order == MSBFirst)
        || (Q_BYTE_ORDER == Q_BIG_ENDIAN && xi->byte_order == LSBFirst))
    {
        const int bpl = image.bytesPerLine();
        uchar* dest = image.bits();
        for (int i=0; i < image.height(); i++) {
            if (depth() == 16) {
                ushort *p = reinterpret_cast<ushort*>(QFAST_SCAN_LINE(dest, bpl, i));
                const ushort *end = p + image.width();
                while (p < end) {
                    *p = ((*p << 8) & 0xff00) | ((*p >> 8) & 0x00ff);
                    p++;
                }
            } else {
                uint *p = reinterpret_cast<uint*>(QFAST_SCAN_LINE(dest, bpl, i));
                const uint *end = p + image.width();
                while (p < end) {
                    *p = ((*p << 24) & 0xff000000) | ((*p << 8) & 0x00ff0000)
                         | ((*p >> 8) & 0x0000ff00) | ((*p >> 24) & 0x000000ff);
                    p++;
                }
            }
        }
    }

    // fix-up alpha channel
    if (format == QImage::Format_RGB32) {
        QRgb *p = (QRgb *)image.bits();
        for (int y = 0; y < xi->height; ++y) {
            for (int x = 0; x < xi->width; ++x)
                p[x] |= 0xff000000;
            p += xi->bytes_per_line / 4;
        }
    }

    XDestroyImage(xi);
    return image;
}

QImage QX11PixmapData::toImage(const QRect &rect) const
{
    XImage *xi = XGetImage(qt_x11Data->display, hd, rect.x(), rect.y(), rect.width(), rect.height(),
                           AllPlanes, (depth() == 1) ? XYPixmap : ZPixmap);

    Q_CHECK_PTR(xi);
    if (!xi)
        return QImage();

    if (!x11_mask && canTakeQImageFromXImage(xi))
        return takeQImageFromXImage(xi);

    QImage image = toImage(xi, rect);
    qSafeXDestroyImage(xi);
    return image;
}

/*!
    Converts the pixmap to a QImage. Returns a null image if the
    conversion fails.

    If the pixmap has 1-bit depth, the returned image will also be 1
    bit deep. If the pixmap has 2- to 8-bit depth, the returned image
    has 8-bit depth. If the pixmap has greater than 8-bit depth, the
    returned image has 32-bit depth.

    Note that for the moment, alpha masks on monochrome images are
    ignored.

    \sa fromImage(), {QImage#Image Formats}{Image Formats}
*/

QImage QX11PixmapData::toImage() const
{
    return toImage(QRect(0, 0, w, h));
}

QImage QX11PixmapData::toImage(const XImage *xi, const QRect &rect) const
{
    int d = depth();
    Visual *visual = (Visual *)xinfo.visual();
    bool trucol = (visual->c_class >= TrueColor) && d > 1;

    QImage::Format format = QImage::Format_Mono;
    if (d > 1 && d <= 8) {
        d = 8;
        format = QImage::Format_Indexed8;
    }
    // we could run into the situation where d == 8 AND trucol is true, which can
    // cause problems when converting to and from images.  in this case, always treat
    // the depth as 32...
    if (d > 8 || trucol) {
        d = 32;
        format = QImage::Format_RGB32;
    }

    if (d == 1 && xi->bitmap_bit_order == LSBFirst)
        format = QImage::Format_MonoLSB;
    if (x11_mask && format == QImage::Format_RGB32)
        format = QImage::Format_ARGB32;

    QImage image(xi->width, xi->height, format);
    if (image.isNull())                        // could not create image
        return image;

    QImage alpha;
    if (x11_mask) {
        if (rect.contains(QRect(0, 0, w, h)))
            alpha = mask().toImage();
        else
            alpha = mask().toImage().copy(rect);
    }
    bool ale = alpha.format() == QImage::Format_MonoLSB;

    if (trucol) {                                // truecolor
        const uint red_mask = (uint)visual->red_mask;
        const uint green_mask = (uint)visual->green_mask;
        const uint blue_mask = (uint)visual->blue_mask;
        const int  red_shift = highest_bit(red_mask) - 7;
        const int  green_shift = highest_bit(green_mask) - 7;
        const int  blue_shift = highest_bit(blue_mask) - 7;

        const uint red_bits = n_bits(red_mask);
        const uint green_bits = n_bits(green_mask);
        const uint blue_bits = n_bits(blue_mask);

        static uint red_table_bits = 0;
        static uint green_table_bits = 0;
        static uint blue_table_bits = 0;

        if (red_bits < 8 && red_table_bits != red_bits) {
            build_scale_table(&red_scale_table, red_bits);
            red_table_bits = red_bits;
        }
        if (blue_bits < 8 && blue_table_bits != blue_bits) {
            build_scale_table(&blue_scale_table, blue_bits);
            blue_table_bits = blue_bits;
        }
        if (green_bits < 8 && green_table_bits != green_bits) {
            build_scale_table(&green_scale_table, green_bits);
            green_table_bits = green_bits;
        }

        int  r, g, b;

        int bppc = xi->bits_per_pixel;

        if (bppc > 8 && xi->byte_order == LSBFirst)
            bppc++;

        for (int y = 0; y < xi->height; ++y) {
            uint pixel;
            const uchar* asrc = x11_mask ? alpha.constScanLine(y) : 0;
            QRgb *dst = (QRgb *)image.scanLine(y);
            uchar *src = (uchar *)xi->data + xi->bytes_per_line*y;
            for (int x = 0; x < xi->width; x++) {
                switch (bppc) {
                case 8:
                    pixel = *src++;
                    break;
                case 16:                        // 16 bit MSB
                    pixel = src[1] | (uint)src[0] << 8;
                    src += 2;
                    break;
                case 17:                        // 16 bit LSB
                    pixel = src[0] | (uint)src[1] << 8;
                    src += 2;
                    break;
                case 24:                        // 24 bit MSB
                    pixel = src[2] | (uint)src[1] << 8 | (uint)src[0] << 16;
                    src += 3;
                    break;
                case 25:                        // 24 bit LSB
                    pixel = src[0] | (uint)src[1] << 8 | (uint)src[2] << 16;
                    src += 3;
                    break;
                case 32:                        // 32 bit MSB
                    pixel = src[3] | (uint)src[2] << 8 | (uint)src[1] << 16 | (uint)src[0] << 24;
                    src += 4;
                    break;
                case 33:                        // 32 bit LSB
                    pixel = src[0] | (uint)src[1] << 8 | (uint)src[2] << 16 | (uint)src[3] << 24;
                    src += 4;
                    break;
                default:                        // should not really happen
                    x = xi->width;                        // leave loop
                    y = xi->height;
                    pixel = 0;                // eliminate compiler warning
                    qWarning("QPixmap::convertToImage: Invalid depth %d", bppc);
                }
                if (red_shift > 0)
                    r = (pixel & red_mask) >> red_shift;
                else
                    r = (pixel & red_mask) << -red_shift;
                if (green_shift > 0)
                    g = (pixel & green_mask) >> green_shift;
                else
                    g = (pixel & green_mask) << -green_shift;
                if (blue_shift > 0)
                    b = (pixel & blue_mask) >> blue_shift;
                else
                    b = (pixel & blue_mask) << -blue_shift;

                if (red_bits < 8)
                    r = red_scale_table[r];
                if (green_bits < 8)
                    g = green_scale_table[g];
                if (blue_bits < 8)
                    b = blue_scale_table[b];

                if (x11_mask) {
                    if (ale) {
                        *dst++ = (asrc[x >> 3] & (1 << (x & 7))) ? qRgba(r, g, b, 0xff) : 0;
                    } else {
                        *dst++ = (asrc[x >> 3] & (0x80 >> (x & 7))) ? qRgba(r, g, b, 0xff) : 0;
                    }
                } else {
                    *dst++ = qRgb(r, g, b);
                }
            }
        }
    } else if (xi->bits_per_pixel == d) {        // compatible depth
        char *xidata = xi->data;                // copy each scanline
        const int qbpl = image.bytesPerLine();
        const int bpl = qMin(qbpl, xi->bytes_per_line);
        uchar* qdata = image.bits();
        for (int y=0; y<xi->height; y++) {
            uchar* tscan = QFAST_SCAN_LINE(qdata, qbpl, y);
            memcpy(tscan, xidata, bpl);
            xidata += xi->bytes_per_line;
        }
    } else {
        /* Typically 2 or 4 bits display depth */
        qWarning("QPixmap::convertToImage: Display not supported (bpp=%d)",
                 xi->bits_per_pixel);
        return QImage();
    }

    if (d == 1) {                                // bitmap
        image.setColorTable(monoColorTable());
    } else if (!trucol) {                        // pixmap with colormap
        QSTACKARRAY(uchar, use, 256);            // pixel-in-use table
        QSTACKARRAY(uchar, pix, 256);            // pixel translation table
        int ncols = 0;
        int bpl = image.bytesPerLine();

        if (x11_mask) {                         // which pixels are used?
            for (int i = 0; i < xi->height; i++) {
                const uchar* asrc = alpha.constScanLine(i);
                const uchar *p = image.constScanLine(i);
                if (ale) {
                    for (int x = 0; x < xi->width; x++) {
                        if (asrc[x >> 3] & (1 << (x & 7)))
                            use[*p] = 1;
                        ++p;
                    }
                } else {
                    for (int x = 0; x < xi->width; x++) {
                        if (asrc[x >> 3] & (0x80 >> (x & 7)))
                            use[*p] = 1;
                        ++p;
                    }
                }
            }
        } else {
            for (int i = 0; i < xi->height; i++) {
                const uchar *p = image.constScanLine(i);
                const uchar *end = p + bpl;
                while (p < end)
                    use[*p++] = 1;
            }
        }
        for (int i = 0; i < 256; i++) {                // build translation table
            if (use[i])
                pix[i] = ncols++;
        }
        for (int i = 0; i < xi->height; i++) {                        // translate pixels
            uchar *p = image.scanLine(i);
            const uchar *end = p + bpl;
            while (p < end) {
                *p = pix[*p];
                p++;
            }
        }
        if (x11_mask) {
            int trans;
            if (ncols < 256) {
                trans = ncols++;
                image.setColorCount(ncols);        // create color table
                image.setColor(trans, 0x00000000);
            } else {
                image.setColorCount(ncols);        // create color table
                // oh dear... no spare "transparent" pixel.
                // use first pixel in image (as good as any).
                trans = image.scanLine(0)[0];
            }
            for (int i = 0; i < xi->height; i++) {
                const uchar* asrc = alpha.scanLine(i);
                uchar *p = image.scanLine(i);
                if (ale) {
                    for (int x = 0; x < xi->width; x++) {
                        if (!(asrc[x >> 3] & (1 << (x & 7))))
                            *p = trans;
                        ++p;
                    }
                } else {
                    for (int x = 0; x < xi->width; x++) {
                        if (!(asrc[x >> 3] & (1 << (7 -(x & 7)))))
                            *p = trans;
                        ++p;
                    }
                }
            }
        } else {
            image.setColorCount(ncols);        // create color table
        }
        QVector<QColor> colors = QColormap::instance(xinfo.screen()).colormap();
        int j = 0;
        for (int i=0; i<colors.size(); i++) {                // translate pixels
            if (use[i])
                image.setColor(j++, 0xff000000 | colors.at(i).rgb());
        }
    }

    return image;
}

/*!
    Returns a copy of the pixmap that is transformed using the given
    transformation \a matrix and transformation \a mode. The original
    pixmap is not changed.

    The transformation \a matrix is internally adjusted to compensate
    for unwanted translation; i.e. the pixmap produced is the smallest
    pixmap that contains all the transformed points of the original
    pixmap. Use the trueMatrix() function to retrieve the actual
    matrix used for transforming the pixmap.

    This function is slow because it involves transformation to a
    QImage, non-trivial computations and a transformation back to a
    QPixmap.

    \sa trueMatrix(), {QPixmap#Pixmap Transformations}{Pixmap
    Transformations}
*/
QPixmap QX11PixmapData::transformed(const QTransform &transform,
                                    Qt::TransformationMode mode ) const
{
    if (mode == Qt::SmoothTransformation || transform.type() >= QTransform::TxProject) {
        QImage image = toImage();
        return QPixmap::fromImage(image.transformed(transform, mode));
    }

    uint   w = 0;
    uint   h = 0;                               // size of target pixmap
    uint   ws, hs;                              // size of source pixmap
    uchar *dptr;                                // data in target pixmap
    uint   dbpl, dbytes;                        // bytes per line/bytes total
    uchar *sptr;                                // data in original pixmap
    int    sbpl;                                // bytes per line in original
    int    bpp;                                 // bits per pixel
    bool   depth1 = depth() == 1;
    Display *dpy = qt_x11Data->display;

    ws = width();
    hs = height();

    QTransform mat(transform.m11(), transform.m12(), transform.m13(),
                   transform.m21(), transform.m22(), transform.m23(),
                   0., 0., 1);
    bool complex_xform = false;
    qreal scaledWidth;
    qreal scaledHeight;

    if (mat.type() <= QTransform::TxScale) {
        scaledHeight = qAbs(mat.m22()) * hs + 0.9999;
        scaledWidth = qAbs(mat.m11()) * ws + 0.9999;
        h = qAbs(int(scaledHeight));
        w = qAbs(int(scaledWidth));
    } else {                                        // rotation or shearing
        QPolygonF a(QRectF(0, 0, ws, hs));
        a = mat.map(a);
        QRect r = a.boundingRect().toAlignedRect();
        w = r.width();
        h = r.height();
        scaledWidth = w;
        scaledHeight = h;
        complex_xform = true;
    }
    mat = QPixmap::trueMatrix(mat, ws, hs); // true matrix

    bool invertible;
    mat = mat.inverted(&invertible);  // invert matrix

    if (h == 0 || w == 0 || !invertible
        || qAbs(scaledWidth) >= 32768 || qAbs(scaledHeight) >= 32768 ) {
        // error, return null pixmap
        return QPixmap();
    }

    XImage *xi = XGetImage(qt_x11Data->display, handle(), 0, 0, ws, hs, AllPlanes,
                           depth1 ? XYPixmap : ZPixmap);

    if (!xi)
        return QPixmap();

    sbpl = xi->bytes_per_line;
    sptr = (uchar *)xi->data;
    bpp  = xi->bits_per_pixel;

    if (depth1)
        dbpl = (w+7)/8;
    else
        dbpl = ((w*bpp+31)/32)*4;
    dbytes = dbpl*h;

    dptr = (uchar *)::malloc(dbytes);        // create buffer for bits
    Q_CHECK_PTR(dptr);
    if (depth1)                                // fill with zeros
        ::memset(dptr, 0, dbytes);
    else if (bpp == 8)                        // fill with background color
        ::memset(dptr, WhitePixel(qt_x11Data->display, xinfo.screen()), dbytes);
    else
        ::memset(dptr, 0, dbytes);

    // #define QT_DEBUG_XIMAGE
#if defined(QT_DEBUG_XIMAGE)
    qDebug("----IMAGE--INFO--------------");
    qDebug("width............. %d", xi->width);
    qDebug("height............ %d", xi->height);
    qDebug("xoffset........... %d", xi->xoffset);
    qDebug("format............ %d", xi->format);
    qDebug("byte order........ %d", xi->byte_order);
    qDebug("bitmap unit....... %d", xi->bitmap_unit);
    qDebug("bitmap bit order.. %d", xi->bitmap_bit_order);
    qDebug("depth............. %d", xi->depth);
    qDebug("bytes per line.... %d", xi->bytes_per_line);
    qDebug("bits per pixel.... %d", xi->bits_per_pixel);
#endif

    int type;
    if (xi->bitmap_bit_order == MSBFirst)
        type = QT_XFORM_TYPE_MSBFIRST;
    else
        type = QT_XFORM_TYPE_LSBFIRST;
    int xbpl, p_inc;
    if (depth1) {
        xbpl  = (w+7)/8;
        p_inc = dbpl - xbpl;
    } else {
        xbpl  = (w*bpp)/8;
        p_inc = dbpl - xbpl;
    }

    if (!qt_xForm_helper(mat, xi->xoffset, type, bpp, dptr, xbpl, p_inc, h, sptr, sbpl, ws, hs)){
        qWarning("QPixmap::transform: display not supported (bpp=%d)",bpp);
        QPixmap pm;
        return pm;
    }

    qSafeXDestroyImage(xi);

    if (depth1) {                                // mono bitmap
        QBitmap bm = QBitmap::fromData(QSize(w, h), dptr,
                                       BitmapBitOrder(qt_x11Data->display) == MSBFirst
                                       ? QImage::Format_Mono
                                       : QImage::Format_MonoLSB);
        free(dptr);
        return bm;
    } else {                                        // color pixmap
        QX11PixmapData *x11Data = new QX11PixmapData(QPixmapData::PixmapType);
        QPixmap pm(x11Data);
        x11Data->xinfo = xinfo;
        x11Data->d = d;
        x11Data->w = w;
        x11Data->h = h;
        x11Data->is_null = (w <= 0 || h <= 0);
        x11Data->hd = (Qt::HANDLE)XCreatePixmap(qt_x11Data->display,
                                                RootWindow(qt_x11Data->display, xinfo.screen()),
                                                w, h, d);
        x11Data->setSerialNumber(qt_pixmap_serial.fetchAndAddRelaxed(1));

#ifndef QT_NO_XRENDER
        if (qt_x11Data->use_xrender) {
            XRenderPictFormat *format = x11Data->d == 32
                                        ? XRenderFindStandardFormat(qt_x11Data->display, PictStandardARGB32)
                                        : XRenderFindVisualFormat(qt_x11Data->display, (Visual *) x11Data->xinfo.visual());
            x11Data->picture = XRenderCreatePicture(qt_x11Data->display, x11Data->hd, format, 0, 0);
        }
#endif // QT_NO_XRENDER

        GC gc = XCreateGC(qt_x11Data->display, x11Data->hd, 0, 0);
        xi = XCreateImage(dpy, (Visual*)x11Data->xinfo.visual(),
                            x11Data->d,
                            ZPixmap, 0, (char *)dptr, w, h, 32, 0);
        XPutImage(dpy, pm.handle(), gc, xi, 0, 0, 0, 0, w, h);
        qSafeXDestroyImage(xi);
        XFreeGC(qt_x11Data->display, gc);

        if (x11_mask) { // xform mask, too
            pm.setMask(mask_to_bitmap(xinfo.screen()).transformed(transform));
        } else if (d != 32 && complex_xform) { // need a mask!
            QBitmap mask(ws, hs);
            mask.fill(Qt::color1);
            pm.setMask(mask.transformed(transform));
        }
        return pm;
    }
}

int QPixmap::x11SetDefaultScreen(int screen)
{
    int old = defaultScreen;
    defaultScreen = screen;
    return old;
}

void QPixmap::x11SetScreen(int screen)
{
    if (Q_UNLIKELY(paintingActive())) {
        qWarning("QPixmap::x11SetScreen(): Cannot change screens during painting");
        return;
    }

    if (isNull())
        return;

    if (data->classId() != QPixmapData::X11Class)
        return;

    if (screen < 0)
        screen = QX11Info::appScreen();

    QX11PixmapData *x11Data = static_cast<QX11PixmapData*>(data.data());
    if (screen == x11Data->xinfo.screen())
        return; // nothing to do

    if (isNull()) {
        QX11InfoData* xd = x11Data->xinfo.getX11Data();
        xd->screen = screen;
        xd->depth = QX11Info::appDepth(screen);
        xd->cells = QX11Info::appCells(screen);
        xd->colormap = QX11Info::appColormap(screen);
        xd->defaultColormap = QX11Info::appDefaultColormap(screen);
        xd->visual = (Visual *)QX11Info::appVisual(screen);
        xd->defaultVisual = QX11Info::appDefaultVisual(screen);
        x11Data->xinfo.setX11Data(xd);
        return;
    }
#if 0
    qDebug("QPixmap::x11SetScreen for %p from %d to %d. Size is %d/%d", x11Data, x11Data->xinfo.screen(), screen, width(), height());
#endif

    x11SetDefaultScreen(screen);
    *this = qt_toX11Pixmap(toImage());
}

QPixmap QPixmap::grabWindow(WId window, int x, int y, int w, int h)
{
    if (w == 0 || h == 0)
        return QPixmap();

    Display *dpy = qt_x11Data->display;
    XWindowAttributes window_attr;
    if (!XGetWindowAttributes(dpy, window, &window_attr))
        return QPixmap();

    if (w < 0)
        w = window_attr.width - x;
    if (h < 0)
        h = window_attr.height - y;

    // determine the screen
    int scr;
    for (scr = 0; scr < ScreenCount(dpy); ++scr) {
        if (window_attr.root == RootWindow(dpy, scr))        // found it
            break;
    }
    if (scr >= ScreenCount(dpy))                // sanity check
        return QPixmap();


    // get the depth of the root window
    XWindowAttributes root_attr;
    if (!XGetWindowAttributes(dpy, window_attr.root, &root_attr))
        return QPixmap();

    if (window_attr.depth == root_attr.depth) {
        // if the depth of the specified window and the root window are the
        // same, grab pixels from the root window (so that we get the any
        // overlapping windows and window manager frames)

        // map x and y to the root window
        WId unused;
        if (!XTranslateCoordinates(dpy, window, window_attr.root, x, y,
                                   &x, &y, &unused))
            return QPixmap();

        window = window_attr.root;
        window_attr = root_attr;
    }

    QX11PixmapData *data = new QX11PixmapData(QPixmapData::PixmapType);

    extern void qt_x11_getX11InfoForWindow(QX11Info *xinfo, const void *a);
    qt_x11_getX11InfoForWindow(&data->xinfo, static_cast<const void*>(&window_attr));

    data->resize(w, h);

    QPixmap pm(data);
    pm.x11SetScreen(scr);

    GC gc = XCreateGC(dpy, pm.handle(), 0, 0);
    XSetSubwindowMode(dpy, gc, IncludeInferiors);
    XCopyArea(dpy, window, pm.handle(), gc, x, y, w, h, 0, 0);
    XFreeGC(dpy, gc);

    return pm;
}

bool QX11PixmapData::hasAlphaChannel() const
{
    return d == 32;
}

const QX11Info &QPixmap::x11Info() const
{
    if (data && data->classId() == QPixmapData::X11Class) {
        return static_cast<QX11PixmapData*>(data.data())->xinfo;
    } else {
        static QX11Info nullX11Info;
        return nullX11Info;
    }
}

#if !defined(QT_NO_XRENDER)
static XRenderPictFormat *qt_renderformat_for_depth(const QX11Info &xinfo, int depth)
{
    if (depth == 1)
        return XRenderFindStandardFormat(qt_x11Data->display, PictStandardA1);
    else if (depth == 32)
        return XRenderFindStandardFormat(qt_x11Data->display, PictStandardARGB32);
    else
        return XRenderFindVisualFormat(qt_x11Data->display, (Visual *)xinfo.visual());
}
#endif

QPaintEngine* QX11PixmapData::paintEngine() const
{
    QX11PixmapData *that = const_cast<QX11PixmapData*>(this);

    if ((flags & Readonly) && share_mode == QPixmap::ImplicitlyShared) {
        // if someone wants to draw onto us, copy the shared contents
        // and turn it into a fully fledged QPixmap
        ::Pixmap hd_copy = XCreatePixmap(qt_x11Data->display, RootWindow(qt_x11Data->display, xinfo.screen()),
                                         w, h, d);
#if !defined(QT_NO_XRENDER)
        if (picture && d == 32) {
            XRenderPictFormat *format = qt_renderformat_for_depth(xinfo, d);
            ::Picture picture_copy = XRenderCreatePicture(qt_x11Data->display,
                                                          hd_copy, format,
                                                          0, 0);

            XRenderComposite(qt_x11Data->display, PictOpSrc, picture, 0, picture_copy,
                             0, 0, 0, 0, 0, 0, w, h);
            XRenderFreePicture(qt_x11Data->display, picture);
            that->picture = picture_copy;
        } else
#endif
        {
            GC gc = XCreateGC(qt_x11Data->display, hd_copy, 0, 0);
            XCopyArea(qt_x11Data->display, hd, hd_copy, gc, 0, 0, w, h, 0, 0);
            XFreeGC(qt_x11Data->display, gc);
        }
        that->hd = hd_copy;
        that->flags &= ~QX11PixmapData::Readonly;
    }

    if (!that->pengine)
        that->pengine = new QX11PaintEngine;
    return that->pengine;
}

Qt::HANDLE QPixmap::x11PictureHandle() const
{
#ifndef QT_NO_XRENDER
    if (data && data->classId() == QPixmapData::X11Class)
        return static_cast<const QX11PixmapData*>(data.data())->picture;
    else
        return 0;
#else
    return 0;
#endif // QT_NO_XRENDER
}

Qt::HANDLE QX11PixmapData::x11ConvertToDefaultDepth()
{
#ifndef QT_NO_XRENDER
    if (d == QX11Info::appDepth() || !qt_x11Data->use_xrender)
        return hd;
    if (!hd2) {
        hd2 = XCreatePixmap(xinfo.display(), hd, w, h, QX11Info::appDepth());
        XRenderPictFormat *format = XRenderFindVisualFormat(xinfo.display(),
                                                            (Visual*) xinfo.visual());
        Picture pic = XRenderCreatePicture(xinfo.display(), hd2, format, 0, 0);
        XRenderComposite(xinfo.display(), PictOpSrc, picture,
                         XNone, pic, 0, 0, 0, 0, 0, 0, w, h);
        XRenderFreePicture(xinfo.display(), pic);
    }
    return hd2;
#else
    return hd;
#endif
}

void QX11PixmapData::copy(const QPixmapData *data, const QRect &rect)
{
    if (data->pixelType() == BitmapType) {
        fromImage(data->toImage(rect), Qt::AutoColor);
        return;
    }

    const QX11PixmapData *x11Data = static_cast<const QX11PixmapData*>(data);

    setSerialNumber(qt_pixmap_serial.fetchAndAddRelaxed(1));

    xinfo = x11Data->xinfo;
    d = x11Data->d;
    w = rect.width();
    h = rect.height();
    is_null = (w <= 0 || h <= 0);
    hd = (Qt::HANDLE)XCreatePixmap(qt_x11Data->display,
                                   RootWindow(qt_x11Data->display, x11Data->xinfo.screen()),
                                   w, h, d);
#ifndef QT_NO_XRENDER
    if (qt_x11Data->use_xrender) {
        XRenderPictFormat *format = d == 32
                                    ? XRenderFindStandardFormat(qt_x11Data->display, PictStandardARGB32)
                                    : XRenderFindVisualFormat(qt_x11Data->display, (Visual *)xinfo.visual());
        picture = XRenderCreatePicture(qt_x11Data->display, hd, format, 0, 0);
    }
#endif // QT_NO_XRENDER
    if (x11Data->x11_mask) {
        x11_mask = XCreatePixmap(qt_x11Data->display, hd, w, h, 1);
#ifndef QT_NO_XRENDER
        if (qt_x11Data->use_xrender) {
            mask_picture = XRenderCreatePicture(qt_x11Data->display, x11_mask,
                                                XRenderFindStandardFormat(qt_x11Data->display, PictStandardA1), 0, 0);
            XRenderPictureAttributes attrs;
            attrs.alpha_map = x11Data->mask_picture;
            XRenderChangePicture(qt_x11Data->display, x11Data->picture, CPAlphaMap, &attrs);
        }
#endif
    }

#if !defined(QT_NO_XRENDER)
    if (x11Data->picture && x11Data->d == 32) {
        XRenderComposite(qt_x11Data->display, PictOpSrc,
                         x11Data->picture, 0, picture,
                         rect.x(), rect.y(), 0, 0, 0, 0, w, h);
    } else
#endif
    {
        GC gc = XCreateGC(qt_x11Data->display, hd, 0, 0);
        XCopyArea(qt_x11Data->display, x11Data->hd, hd, gc,
                  rect.x(), rect.y(), w, h, 0, 0);
        if (x11Data->x11_mask) {
            GC monogc = XCreateGC(qt_x11Data->display, x11_mask, 0, 0);
            XCopyArea(qt_x11Data->display, x11Data->x11_mask, x11_mask, monogc,
                      rect.x(), rect.y(), w, h, 0, 0);
            XFreeGC(qt_x11Data->display, monogc);
        }
        XFreeGC(qt_x11Data->display, gc);
    }
}

bool QX11PixmapData::scroll(int dx, int dy, const QRect &rect)
{
    GC gc = XCreateGC(qt_x11Data->display, hd, 0, 0);
    XCopyArea(qt_x11Data->display, hd, hd, gc,
              rect.left(), rect.top(), rect.width(), rect.height(),
              rect.left() + dx, rect.top() + dy);
    XFreeGC(qt_x11Data->display, gc);
    return true;
}

#if !defined(QT_NO_XRENDER)
void QX11PixmapData::convertToARGB32(bool preserveContents)
{
    if (!qt_x11Data->use_xrender)
        return;

    // Q_ASSERT(count == 1);
    if ((flags & Readonly) && share_mode == QPixmap::ExplicitlyShared)
        return;

    Pixmap pm = XCreatePixmap(qt_x11Data->display, RootWindow(qt_x11Data->display, xinfo.screen()),
                              w, h, 32);
    Picture p = XRenderCreatePicture(qt_x11Data->display, pm,
                                     XRenderFindStandardFormat(qt_x11Data->display, PictStandardARGB32), 0, 0);
    if (picture) {
        if (preserveContents)
            XRenderComposite(qt_x11Data->display, PictOpSrc, picture, 0, p, 0, 0, 0, 0, 0, 0, w, h);
        if (!(flags & Readonly))
            XRenderFreePicture(qt_x11Data->display, picture);
    }
    if (hd && !(flags & Readonly))
        XFreePixmap(qt_x11Data->display, hd);
    if (x11_mask) {
        XFreePixmap(qt_x11Data->display, x11_mask);
        if (mask_picture)
            XRenderFreePicture(qt_x11Data->display, mask_picture);
        x11_mask = 0;
        mask_picture = 0;
    }
    hd = pm;
    picture = p;
    d = 32;
}
#endif

QPixmap QPixmap::fromX11Pixmap(Qt::HANDLE pixmap, QPixmap::ShareMode mode)
{
    Window root;
    int x;
    int y;
    uint width;
    uint height;
    uint border_width;
    uint depth;
    XWindowAttributes win_attribs;
    int num_screens = ScreenCount(qt_x11Data->display);
    int screen = 0;

    XGetGeometry(qt_x11Data->display, pixmap, &root, &x, &y, &width, &height, &border_width, &depth);
    XGetWindowAttributes(qt_x11Data->display, root, &win_attribs);

    for (; screen < num_screens; ++screen) {
        if (win_attribs.screen == ScreenOfDisplay(qt_x11Data->display, screen))
            break;
    }

    QX11PixmapData *data = new QX11PixmapData(depth == 1 ? QPixmapData::BitmapType : QPixmapData::PixmapType);
    data->setSerialNumber(qt_pixmap_serial.fetchAndAddRelaxed(1));
    data->flags = QX11PixmapData::Readonly;
    data->share_mode = mode;
    data->w = width;
    data->h = height;
    data->is_null = (width <= 0 || height <= 0);
    data->d = depth;
    data->hd = pixmap;

    if (defaultScreen >= 0 && defaultScreen != screen) {
        QX11InfoData* xd = data->xinfo.getX11Data();
        xd->screen = defaultScreen;
        xd->depth = QX11Info::appDepth(xd->screen);
        xd->cells = QX11Info::appCells(xd->screen);
        xd->colormap = QX11Info::appColormap(xd->screen);
        xd->defaultColormap = QX11Info::appDefaultColormap(xd->screen);
        xd->visual = (Visual *)QX11Info::appVisual(xd->screen);
        xd->defaultVisual = QX11Info::appDefaultVisual(xd->screen);
        data->xinfo.setX11Data(xd);
    }

#ifndef QT_NO_XRENDER
    if (qt_x11Data->use_xrender) {
        XRenderPictFormat *format = qt_renderformat_for_depth(data->xinfo, depth);
        data->picture = XRenderCreatePicture(qt_x11Data->display, data->hd, format, 0, 0);
    }
#endif // QT_NO_XRENDER

    return QPixmap(data);
}


QT_END_NAMESPACE





