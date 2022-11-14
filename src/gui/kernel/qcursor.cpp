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

#include "qcursor.h"

#ifndef QT_NO_CURSOR

#include "qimage.h"
#include "qdatastream.h"
#include "qvariant.h"
#include "qcursor_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QCursor

    \brief The QCursor class provides a mouse cursor with an arbitrary
    shape.

    \ingroup appearance
    \ingroup shared


    This class is mainly used to create mouse cursors that are
    associated with particular widgets and to get and set the position
    of the mouse cursor.

    Katie has a number of standard cursor shapes, but you can also make
    custom cursor shapes based on a QPixmap and a hotspot.

    To associate a cursor with a widget, use QWidget::setCursor(). To
    associate a cursor with all widgets (normally for a short period
    of time), use QApplication::setOverrideCursor().

    To set a cursor shape use QCursor::setShape() or use the QCursor
    constructor which takes the shape as argument, or you can use one
    of the predefined cursors defined in the \l Qt::CursorShape enum.

    If you want to create a cursor with your own pixmap, use the
    QCursor constructor which takes a pixmap as arguments.

    To set or get the position of the mouse cursor use the static
    methods QCursor::pos() and QCursor::setPos().

    \bold{Note:} It is possible to create a QCursor before
    QApplication, but it is not useful except as a place-holder for a
    real QCursor created after QApplication. Attempting to use a
    QCursor that was created before QApplication will result in a
    crash.

    \section1 A Note for X11 Users

    On X11, Qt supports the \link
    http://www.xfree86.org/4.3.0/Xcursor.3.html Xcursor\endlink
    library, which allows for full color icon themes. The table below
    shows the cursor name used for each Qt::CursorShape value. If a
    cursor cannot be found using the name shown below, a standard X11
    cursor will be used instead. Note: X11 does not provide
    appropriate cursors for all possible Qt::CursorShape values. It
    is possible that some cursors will be taken from the Xcursor
    theme, while others will use an internal pixmap cursor.

    \table
    \header \o Shape \o Qt::CursorShape Value \o Cursor Name
            \o Shape \o Qt::CursorShape Value \o Cursor Name
    \row \o \inlineimage cursor-arrow.png
         \o Qt::ArrowCursor   \o \c left_ptr
         \o \inlineimage      cursor-sizev.png
         \o Qt::SizeVerCursor \o \c size_ver
    \row \o \inlineimage      cursor-uparrow.png
         \o Qt::UpArrowCursor \o \c up_arrow
         \o \inlineimage      cursor-sizeh.png
         \o Qt::SizeHorCursor \o \c size_hor
    \row \o \inlineimage      cursor-cross.png
         \o Qt::CrossCursor   \o \c cross
         \o \inlineimage      cursor-sizeb.png
         \o Qt::SizeBDiagCursor \o \c size_bdiag
    \row \o \inlineimage      cursor-ibeam.png
         \o Qt::IBeamCursor   \o \c ibeam
         \o \inlineimage      cursor-sizef.png
         \o Qt::SizeFDiagCursor \o \c size_fdiag
    \row \o \inlineimage      cursor-wait.png
         \o Qt::WaitCursor    \o \c wait
         \o \inlineimage      cursor-sizeall.png
         \o Qt::SizeAllCursor \o \c size_all
    \row \o \inlineimage      cursor-busy.png
         \o Qt::BusyCursor    \o \c left_ptr_watch
         \o \inlineimage      cursor-vsplit.png
         \o Qt::SplitVCursor  \o \c split_v
    \row \o \inlineimage      cursor-forbidden.png
         \o Qt::ForbiddenCursor \o \c forbidden
         \o \inlineimage      cursor-hsplit.png
         \o Qt::SplitHCursor  \o \c split_h
    \row \o \inlineimage      cursor-hand.png
         \o Qt::PointingHandCursor \o \c pointing_hand
         \o \inlineimage      cursor-openhand.png
         \o Qt::OpenHandCursor  \o \c openhand
    \row \o \inlineimage      cursor-whatsthis.png
         \o Qt::WhatsThisCursor \o \c whats_this
         \o \inlineimage      cursor-closedhand.png
         \o Qt::ClosedHandCursor \o \c closedhand
    \row \o
         \o Qt::DragMoveCursor      \o \c dnd-move or \c move
         \o
         \o Qt::DragCopyCursor      \o \c dnd-copy or \c copy
    \row \o
         \o Qt::DragLinkCursor      \o \c dnd-link or \c link
    \endtable

    \sa QWidget, {fowler}{GUI Design Handbook: Cursors}
*/

/*!
    \fn Qt::HANDLE QCursor::handle() const

    Returns a platform-specific cursor handle, Qt::HANDLE on X11/

    \warning Using the value returned by this function is not
    portable.
*/

/*!
    \fn QCursor::QCursor(Qt::HANDLE handle)

    Constructs a Qt cursor from the given \a handle.

    \warning This function is only available on X11.

    \sa handle()
*/

/*!
    \fn QPoint QCursor::pos()

    Returns the position of the cursor (hot spot) in global screen
    coordinates.

    You can call QWidget::mapFromGlobal() to translate it to widget
    coordinates.

    \sa setPos(), QWidget::mapFromGlobal(), QWidget::mapToGlobal()
*/

/*!
    \fn void QCursor::setPos(int x, int y)

    Moves the cursor (hot spot) to the global screen position (\a x,
    \a y).

    You can call QWidget::mapToGlobal() to translate widget
    coordinates to global screen coordinates.

    \sa pos(), QWidget::mapFromGlobal(), QWidget::mapToGlobal()
*/

/*!
    \fn void QCursor::setPos (const QPoint &p)

    \overload

    Moves the cursor (hot spot) to the global screen position at point
    \a p.
*/

/*****************************************************************************
  QCursor stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
/*!
    \fn QDataStream &operator<<(QDataStream &stream, const QCursor &cursor)
    \relates QCursor

    Writes the \a cursor to the \a stream.

    \sa {Serializing Qt Data Types}
*/
QDataStream &operator<<(QDataStream &s, const QCursor &c)
{
    s << (qint16)c.shape();
    if (c.shape() == Qt::BitmapCursor) {
        s << c.pixmap();
        s << c.hotSpot();
    }
    return s;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, QCursor &cursor)
    \relates QCursor

    Reads the \a cursor from the \a stream.

    \sa {Serializing Qt Data Types}
*/
QDataStream &operator>>(QDataStream &s, QCursor &c)
{
    qint16 shape = Qt::ArrowCursor;
    s >> shape;
    if (shape == Qt::BitmapCursor) {
        QPixmap pm;
        QPoint hot;
        s >> pm >> hot;
        c = QCursor(pm, hot.x(), hot.y());
    } else {
        c.setShape(static_cast<Qt::CursorShape>(shape));                // create cursor with shape
    }
    return s;
}
#endif // QT_NO_DATASTREAM

/*!
    Constructs a custom pixmap cursor.

    \a pixmap is the image. It is usual to give it a mask (set using
    QPixmap::setMask()). \a hotX and \a hotY define the cursor's hot
    spot.

    If \a hotX is negative, it is set to the \c{pixmap().width()/2}.
    If \a hotY is negative, it is set to the \c{pixmap().height()/2}.

    Valid cursor sizes depend on the display hardware (or the
    underlying window system). We recommend using 32 x 32 cursors,
    because this size is supported on all platforms. Some platforms
    also support 16 x 16, 48 x 48, and 64 x 64 cursors.

    \sa QPixmap::QPixmap(), QPixmap::setMask()
*/
QCursor::QCursor(const QPixmap &pixmap, int hotX, int hotY)
    : d(nullptr)
{
    const QBitmap mask = pixmap.mask();
    if (Q_UNLIKELY(mask.depth() != 1 || pixmap.size() != mask.size())) {
        qWarning("QCursor: Cannot create bitmap cursor; invalid bitmap(s)");
        return;
    }

    d = new QCursorData(Qt::BitmapCursor);
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
}

/*!
    Constructs a cursor with the default arrow shape.
*/
QCursor::QCursor()
    : d(nullptr)
{
    setShape(Qt::ArrowCursor);
}

/*!
    Constructs a cursor with the specified \a shape.

    See \l Qt::CursorShape for a list of shapes.

    \sa setShape()
*/
QCursor::QCursor(Qt::CursorShape shape)
    : d(nullptr)
{
    setShape(shape);
}

/*!
    Constructs a copy of the cursor \a other.
*/
QCursor::QCursor(const QCursor &other)
    : d(other.d)
{
    if (d) {
        d->ref.ref();
    }
}

/*!
    Destroys the cursor.
*/
QCursor::~QCursor()
{
    if (d && !d->ref.deref()) {
        delete d;
    }
}

/*!
    Assigns \a other to this cursor and returns a reference to this
    cursor.
*/
QCursor &QCursor::operator=(const QCursor &other)
{
    qAtomicAssign(d, other.d);
    return *this;
}

/*!
    Returns the cursor shape identifier. The return value is one of
    the \l Qt::CursorShape enum values (cast to an int).

    \sa setShape()
*/
Qt::CursorShape QCursor::shape() const
{
    if (!d) {
        return Qt::ArrowCursor;
    }
    return d->cshape;
}

/*!
    Sets the cursor to the shape identified by \a shape.

    See \l Qt::CursorShape for the list of cursor shapes.

    \sa shape()
*/
void QCursor::setShape(Qt::CursorShape shape)
{
    if (d && !d->ref.deref()) {
        delete d;
        d = nullptr;
    }
    if (shape <= Qt::LastCursor) {
        d = new QCursorData(shape);
    }
}

/*!
    Returns the cursor pixmap. This is only valid if the cursor is a
    pixmap cursor.
*/

QPixmap QCursor::pixmap() const
{
    if (!d) {
        return QPixmap();
    }
    return d->px;
}

/*!
    Returns the cursor hot spot, or (0, 0) if it is one of the
    standard cursors.
*/

QPoint QCursor::hotSpot() const
{
    if (!d) {
        return QPoint(0, 0);
    }
    return QPoint(d->hx, d->hy);
}

/*!
   Returns the cursor as a QVariant.
*/
QCursor::operator QVariant() const
{
    return QVariant(QVariant::Cursor, this);
}

QT_END_NAMESPACE

#endif // QT_NO_CURSOR
