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

#include "qbitmap.h"
#include "qcursor.h"
#include "qcursor_p.h"
#include "qx11info_x11.h"
#include "qt_x11_p.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_CURSOR
// values are from:
// https://www.freedesktop.org/wiki/Specifications/cursor-spec/
// https://tronche.com/gui/x/xlib/appendix/b/
static const struct CursorTblData {
    const int font;
    const char* name;
    const char* alternative;
} CursorTbl[] = {
    { XC_left_ptr, "left_ptr", "default" }, // Qt::ArrowCursor
    { XC_center_ptr, "up_arrow", nullptr }, // Qt::UpArrowCursor
    { XC_crosshair, "cross", "crosshair" }, // Qt::CrossCursor
    { XC_watch, "wait", "watch" }, // Qt::WaitCursor
    { XC_xterm, "ibeam", "text" }, // Qt::IBeamCursor
    { XC_sb_v_double_arrow, "size_ver", "ns-resize" }, // Qt::SizeVerCursor
    { XC_sb_h_double_arrow, "size_hor", "ew-resize" }, // Qt::SizeHorCursor
    { XC_top_right_corner, "size_bdiag", "nwse-resize" }, // Qt::SizeBDiagCursor
    { XC_bottom_right_corner, "size_fdiag", "nesw-resize" }, // Qt::SizeFDiagCursor
    { XC_fleur, "size_all", nullptr }, // Qt::SizeAllCursor
    { -1, "blank", nullptr }, // Qt::BlankCursor
    { XC_sb_v_double_arrow, "split_v", "row-resize" }, // Qt::SplitVCursor
    { XC_sb_h_double_arrow, "split_h", "col-resize" }, // Qt::SplitHCursor
    { XC_hand2, "pointing_hand", "pointer" }, // Qt::PointingHandCursor
    { XC_circle, "forbidden", "not-allowed" }, // Qt::ForbiddenCursor
    { XC_question_arrow, "whats_this", "help" }, // Qt::WhatsThisCursor
    { XC_watch, "left_ptr_watch", "half-busy" }, // Qt::BusyCursor
    { -1, "openhand", "grab" }, // Qt::OpenHandCursor
    { -1, "closedhand", "grabbing" }, // Qt::ClosedHandCursor
    { XC_tcross, "copy", "dnd-copy" }, // Qt::DragCopyCursor
    { XC_top_left_arrow, "move", "dnd-move" }, // Qt::DragMoveCursor
    { XC_center_ptr, "link", "dnd-link" }, // Qt::DragLinkCursor
};

static const char x11_blank_cursor_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*****************************************************************************
  Internal QCursorData class
 *****************************************************************************/

QCursorData::QCursorData(Qt::CursorShape s)
    : ref(1), cshape(s), hx(0), hy(0), hcurs(0), x11px(0), x11bm(0)
#ifndef QT_NO_XRENDER
    , x11pic(0)
#endif
{
}

QCursorData::~QCursorData()
{
    Display *dpy = (qt_x11Data ? qt_x11Data->display : nullptr);

    // Add in checking for the display too as on HP-UX
    // we seem to get a core dump as the cursor data is
    // deleted again from main() on exit...
    if (hcurs && dpy) {
        XFreeCursor(dpy, hcurs);
    }
#ifndef QT_NO_XRENDER
    if (x11pic) {
        XRenderFreePicture(dpy, x11pic);
    }
#endif
    if (x11px && dpy) {
        XFreePixmap(dpy, x11px);
    }
    if (x11bm && dpy) {
        XFreePixmap(dpy, x11bm);
    }
}

QCursorData *QCursorData::setBitmap(const QPixmap &pixmap, const QBitmap &mask, int hotX, int hotY)
{
    if (Q_UNLIKELY(mask.depth() != 1 || pixmap.size() != mask.size())) {
        qWarning("QCursor: Cannot create bitmap cursor; invalid bitmap(s)");
        return nullptr;
    }
    QCursorData *d = new QCursorData(Qt::BitmapCursor);

    d->px = pixmap;
    d->bm = mask;
    d->hx = (hotX >= 0 ? hotX : (pixmap.width() / 2));
    d->hy = (hotY >= 0 ? hotY : (pixmap.height() / 2));
    d->fg.red   = 0x0000;
    d->fg.green = 0x0000;
    d->fg.blue  = 0x0000;
    d->bg.red   = 0xffff;
    d->bg.green = 0xffff;
    d->bg.blue  = 0xffff;
    return d;
}

QCursor::QCursor(Qt::HANDLE cursor)
    : d(new QCursorData(Qt::CustomCursor))
{
    d->hcurs = cursor;
}

Qt::HANDLE QCursor::handle() const
{
    if (!d) {
        return 0;
    }

    Display *dpy = qt_x11Data->display;
    if (!dpy) {
        return 0;
    }

    if (!d->hcurs) {
        // Non-standard X11 cursors are created from bitmaps
        if (d->cshape == Qt::BitmapCursor) {
#ifndef QT_NO_XRENDER
            if (!d->px.isNull() && qt_x11Data->use_xrender) {
                d->x11px = d->px.toX11Pixmap();
                XRenderPictFormat *format = (d->px.depth() == 1)
                                        ? XRenderFindStandardFormat(dpy, PictStandardA1)
                                        : XRenderFindStandardFormat(dpy, PictStandardARGB32);
                d->x11pic = XRenderCreatePicture(dpy, d->x11px, format, 0, 0);
                if (d->x11pic) {
                    d->hcurs = XRenderCreateCursor(dpy, d->x11pic, d->hx, d->hy);
                }
                return d->hcurs;
            }
#endif
            if (!d->px.isNull()) {
                d->x11px = d->px.toX11Pixmap();
                d->x11bm = d->bm.toX11Pixmap();
                d->hcurs = XCreatePixmapCursor(dpy, d->x11px, d->x11bm, &d->fg, &d->bg, d->hx, d->hy);
            }
            return d->hcurs;
        }

#ifndef QT_NO_XCURSOR
        if (!d->hcurs) {
            d->hcurs = XcursorLibraryLoadCursor(dpy, CursorTbl[d->cshape].name);
        }
        if (!d->hcurs && CursorTbl[d->cshape].alternative) {
            d->hcurs = XcursorLibraryLoadCursor(dpy, CursorTbl[d->cshape].alternative);
        }
        if (d->hcurs) {
            return d->hcurs;
        }
#endif // QT_NO_XCURSOR

        // Q cursor to X cursor
        if (d->cshape == Qt::BlankCursor) {
            XColor bg, fg;
            bg.red   = 255 << 8;
            bg.green = 255 << 8;
            bg.blue  = 255 << 8;
            fg.red   = 0;
            fg.green = 0;
            fg.blue  = 0;
            d->x11bm = XCreateBitmapFromData(dpy, QX11Info::appRootWindow(), x11_blank_cursor_bits, 16, 16);
            // reusing the pixmap as mask to create invisible cursor
            d->hcurs = XCreatePixmapCursor(dpy, d->x11bm, d->x11bm, &fg, &bg, 8, 8);
        } else {
            d->hcurs = XCreateFontCursor(dpy, CursorTbl[d->cshape].font);
            if (Q_UNLIKELY(!d->hcurs)) {
                qWarning("QCursor::update: Invalid cursor shape %d", d->cshape);
#ifndef QT_NO_XFIXES
            } else if (qt_x11Data->use_xfixes) {
                XFixesSetCursorName(dpy, d->hcurs, CursorTbl[d->cshape].name);
#endif
            }
        }
    }
    return d->hcurs;
}

/*! \internal
*/
int QCursor::x11Screen()
{
    Window root;
    Window child;
    int root_x, root_y, win_x, win_y;
    uint buttons;
    Display* dpy = qt_x11Data->display;
    for (int i = 0; i < ScreenCount(dpy); ++i) {
        if (XQueryPointer(dpy, QX11Info::appRootWindow(i), &root, &child, &root_x, &root_y,
                          &win_x, &win_y, &buttons)) {
            return i;
        }
    }
    return -1;
}
#endif // QT_NO_CURSOR

QPoint QCursor::pos()
{
    Window root;
    Window child;
    int root_x, root_y, win_x, win_y;
    uint buttons;
    Display* dpy = qt_x11Data->display;
    for (int i = 0; i < ScreenCount(dpy); ++i) {
        if (XQueryPointer(dpy, QX11Info::appRootWindow(i), &root, &child, &root_x, &root_y,
                          &win_x, &win_y, &buttons)) {
            return QPoint(root_x, root_y);
        }
    }
    return QPoint();
}

void QCursor::setPos(int x, int y)
{
    QPoint current;
    QPoint target(x, y);

    // this is copied from pos(), since we need the screen number for the correct
    // root window in the XWarpPointer call
    Window root;
    Window child;
    int root_x, root_y, win_x, win_y;
    uint buttons;
    Display* dpy = qt_x11Data->display;
    int screen;
    for (screen = 0; screen < ScreenCount(dpy); ++screen) {
        if (XQueryPointer(dpy, QX11Info::appRootWindow(screen), &root, &child, &root_x, &root_y,
                          &win_x, &win_y, &buttons)) {
            current = QPoint(root_x, root_y);
            break;
        }
    }

    if (screen >= ScreenCount(dpy)) {
        return;
    }

    // Need to check, since some X servers generate null mouse move
    // events, causing looping in applications which call setPos() on
    // every mouse move event.
    if (current == target) {
        return;
    }

    XWarpPointer(qt_x11Data->display, XNone, QX11Info::appRootWindow(screen), 0, 0, 0, 0, x, y);
}

QT_END_NAMESPACE

