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
#include "qpixmap_x11_p.h"
#include "qt_x11_p.h"

QT_BEGIN_NAMESPACE

extern QPixmap qt_toX11Pixmap(const QPixmap &pixmap); // qpixmap_x11.cpp

/*****************************************************************************
  Internal QCursorData class
 *****************************************************************************/

QCursorData::QCursorData(Qt::CursorShape s)
    : ref(1), cshape(s), bm(nullptr), bmm(nullptr), hx(0), hy(0), hcurs(0), pm(0)
{
}

QCursorData::~QCursorData()
{
    Display *dpy = qt_x11Data ? qt_x11Data->display : (Display*)0;

    // Add in checking for the display too as on HP-UX
    // we seem to get a core dump as the cursor data is
    // deleted again from main() on exit...
    if (hcurs && dpy)
        XFreeCursor(dpy, hcurs);
    if (pm && dpy)
        XFreePixmap(dpy, pm);
    delete bm;
    delete bmm;
}

#ifndef QT_NO_CURSOR
QCursor::QCursor(Qt::HANDLE cursor)
{
    if (!QCursorData::initialized)
        QCursorData::initialize();
    d = new QCursorData(Qt::CustomCursor);
    d->hcurs = cursor;
}

#endif

QCursorData *QCursorData::setBitmap(const QBitmap &bitmap, const QBitmap &mask, int hotX, int hotY)
{
    if (!QCursorData::initialized)
        QCursorData::initialize();
    if (Q_UNLIKELY(bitmap.depth() != 1 || mask.depth() != 1 || bitmap.size() != mask.size())) {
        qWarning("QCursor: Cannot create bitmap cursor; invalid bitmap(s)");
        QCursorData *c = qt_cursorTable[0];
        c->ref.ref();
        return c;
    }
    QCursorData *d = new QCursorData(Qt::BitmapCursor);

    d->bm  = new QBitmap(qt_toX11Pixmap(bitmap));
    d->bmm = new QBitmap(qt_toX11Pixmap(mask));
    d->hx = hotX >= 0 ? hotX : bitmap.width() / 2;
    d->hy = hotY >= 0 ? hotY : bitmap.height() / 2;
    d->fg.red   = 0x0000;
    d->fg.green = 0x0000;
    d->fg.blue  = 0x0000;
    d->bg.red   = 0xffff;
    d->bg.green = 0xffff;
    d->bg.blue  = 0xffff;
    return d;
}



#ifndef QT_NO_CURSOR
Qt::HANDLE QCursor::handle() const
{
    if (!QCursorData::initialized)
        QCursorData::initialize();
    if (!d->hcurs)
        d->update();
    return d->hcurs;
}
#endif

QPoint QCursor::pos()
{
    Window root;
    Window child;
    int root_x, root_y, win_x, win_y;
    uint buttons;
    Display* dpy = qt_x11Data->display;
    for (int i = 0; i < ScreenCount(dpy); ++i) {
        if (XQueryPointer(dpy, QX11Info::appRootWindow(i), &root, &child, &root_x, &root_y,
                          &win_x, &win_y, &buttons))

            return QPoint(root_x, root_y);
    }
    return QPoint();
}

/*! \internal
*/
#ifndef QT_NO_CURSOR
int QCursor::x11Screen()
{
    Window root;
    Window child;
    int root_x, root_y, win_x, win_y;
    uint buttons;
    Display* dpy = qt_x11Data->display;
    for (int i = 0; i < ScreenCount(dpy); ++i) {
        if (XQueryPointer(dpy, QX11Info::appRootWindow(i), &root, &child, &root_x, &root_y,
                          &win_x, &win_y, &buttons))
            return i;
    }
    return -1;
}
#endif

void QCursor::setPos(int x, int y)
{
    QPoint current, target(x, y);

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

    if (screen >= ScreenCount(dpy))
        return;

    // Need to check, since some X servers generate null mouse move
    // events, causing looping in applications which call setPos() on
    // every mouse move event.
    if (current == target)
        return;

    XWarpPointer(qt_x11Data->display, XNone, QX11Info::appRootWindow(screen), 0, 0, 0, 0, x, y);
}

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

/*!
    \internal

    Creates the cursor.
*/
void QCursorData::update()
{
    if (!QCursorData::initialized)
        QCursorData::initialize();
    if (hcurs)
        return;

    Display *dpy = qt_x11Data->display;

    // Non-standard X11 cursors are created from bitmaps
    if (cshape == Qt::BitmapCursor) {
#ifndef QT_NO_XRENDER
        if (!pixmap.isNull() && qt_x11Data->use_xrender) {
            pixmap = qt_toX11Pixmap(pixmap);
            hcurs = XRenderCreateCursor(qt_x11Data->display, pixmap.x11PictureHandle(), hx, hy);
        } else
#endif
        {
            hcurs = XCreatePixmapCursor(dpy, bm->handle(), bmm->handle(), &fg, &bg, hx, hy);
        }
        return;
    }

#ifndef QT_NO_XCURSOR
    if (!hcurs)
        hcurs = XcursorLibraryLoadCursor(dpy, CursorTbl[cshape].name);
    if (!hcurs && CursorTbl[cshape].alternative)
        hcurs = XcursorLibraryLoadCursor(dpy, CursorTbl[cshape].alternative);
    if (hcurs)
        return;
#endif // QT_NO_XCURSOR

    // Q cursor to X cursor
    if (cshape == Qt::BlankCursor) {
        XColor bg, fg;
        bg.red   = 255 << 8;
        bg.green = 255 << 8;
        bg.blue  = 255 << 8;
        fg.red   = 0;
        fg.green = 0;
        fg.blue  = 0;
        pm  = XCreateBitmapFromData(dpy, QX11Info::appRootWindow(), x11_blank_cursor_bits, 16, 16);
        // reusing the pixmap as mask to create invisible cursor
        hcurs = XCreatePixmapCursor(dpy, pm, pm, &fg, &bg, 8, 8);
    } else {
        hcurs = XCreateFontCursor(dpy, CursorTbl[cshape].font);
        if (Q_UNLIKELY(!hcurs)) {
            qWarning("QCursor::update: Invalid cursor shape %d", cshape);
#ifndef QT_NO_XFIXES
        } else if (qt_x11Data->use_xfixes) {
            XFixesSetCursorName(dpy, hcurs, CursorTbl[cshape].name);
#endif
        }
    }
}

QT_END_NAMESPACE


