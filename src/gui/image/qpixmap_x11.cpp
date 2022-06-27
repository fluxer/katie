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

static int defaultScreen = -1;

/*****************************************************************************
  QPixmap member functions
 *****************************************************************************/

QAtomicInt qt_pixmap_serial(0);

QX11PixmapData::QX11PixmapData(PixelType type)
    : QPixmapData(type, X11Class), hd(0),
      flags(NoFlags), x11_mask(0), picture(0), mask_picture(0),
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

class QX11AlphaDetector
{
public:
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

    QImage image(img);

#ifndef QT_NO_XRENDER
    if (alphaCheck.hasXRenderAndAlpha()) {
        if (d != 32) {
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
        QX11Data::copyQImageToXImage(image, xi);

        GC gc = XCreateGC(dpy, hd, 0, 0);
        XPutImage(dpy, hd, gc, xi, 0, 0, 0, 0, w, h);
        XFreeGC(dpy, gc);

        qSafeXDestroyImage(xi);

        return;
    }
#endif // QT_NO_XRENDER

    if (!xi) { // X image not created
        if (image.format() != QImage::Format_RGB32) {
            image = image.convertToFormat(QImage::Format_RGB32, flags);
        }

        xi = XCreateImage(dpy, visual, dd, ZPixmap, 0, 0, w, h, 32, 0);
        Q_CHECK_PTR(xi);
        QX11Data::copyQImageToXImage(image, xi);
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
        setMask(QBitmap::fromImage(image.createAlphaMask(flags)));
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
                       QX11Data::XColorPixel(xinfo.screen(), fillColor));
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
    if (!xi) {
        return QImage();
    }

    if (!x11_mask && canTakeQImageFromXImage(xi)) {
        return takeQImageFromXImage(xi);
    }

    const int d = depth();

    QImage::Format format = QImage::systemFormat();
    if (d == 1 && xi->bitmap_bit_order == LSBFirst) {
        format = QImage::Format_MonoLSB;
    } else if (d == 1) {
        format = QImage::Format_Mono;
    } else if (x11_mask || qt_x11Data->use_xrender) {
        format = QImage::Format_ARGB32;
    }

    QImage image(xi->width, xi->height, format);
    if (image.isNull()) {
        // could not create image
        return image;
    }

    if (image.depth() == 1) {
        image.setColorTable(monoColorTable());
    }

    QX11Data::copyXImageToQImage(xi, image);
    if (x11_mask) {
        QImage alpha;
        if (rect.contains(QRect(0, 0, w, h))) {
            alpha = mask().toImage();
        } else {
            alpha = mask().toImage().copy(rect);
        }
        image = qt_mask_image(image, alpha);
    }

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

void QX11PixmapData::scroll(int dx, int dy, const QRect &rect)
{
    GC gc = XCreateGC(qt_x11Data->display, hd, 0, 0);
    XCopyArea(qt_x11Data->display, hd, hd, gc,
              rect.left(), rect.top(), rect.width(), rect.height(),
              rect.left() + dx, rect.top() + dy);
    XFreeGC(qt_x11Data->display, gc);
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





