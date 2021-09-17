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

#include "qevent.h"
#include "qcursor.h"
#include "qapplication.h"
#include "qapplication_p.h"
#include "qkeysequence_p.h"
#include "qwidget.h"
#include "qgraphicsview.h"
#include "qdebug.h"
#include "qmimedata.h"
#include "qdnd_p.h"
#include "qevent_p.h"


QT_BEGIN_NAMESPACE

/*!
    \class QInputEvent
    \ingroup events

    \brief The QInputEvent class is the base class for events that
    describe user input.
*/

/*!
  \internal
*/
QInputEvent::QInputEvent(Type type, Qt::KeyboardModifiers modifiers)
    : QEvent(type), modState(modifiers)
{}

/*!
  \internal
*/
QInputEvent::~QInputEvent()
{
}

/*!
    \fn Qt::KeyboardModifiers QInputEvent::modifiers() const

    Returns the keyboard modifier flags that existed immediately
    before the event occurred.

    \sa QApplication::keyboardModifiers()
*/

/*! \fn void QInputEvent::setModifiers(Qt::KeyboardModifiers modifiers)

    \internal

    Sets the keyboard modifiers flags for this event.
*/

/*!
    \class QMouseEvent
    \ingroup events

    \brief The QMouseEvent class contains parameters that describe a mouse event.

    Mouse events occur when a mouse button is pressed or released
    inside a widget, or when the mouse cursor is moved.

    Mouse move events will occur only when a mouse button is pressed
    down, unless mouse tracking has been enabled with
    QWidget::setMouseTracking().

    Qt automatically grabs the mouse when a mouse button is pressed
    inside a widget; the widget will continue to receive mouse events
    until the last mouse button is released.

    A mouse event contains a special accept flag that indicates
    whether the receiver wants the event. You should call ignore() if
    the mouse event is not handled by your widget. A mouse event is
    propagated up the parent widget chain until a widget accepts it
    with accept(), or an event filter consumes it.

    \note If a mouse event is propagated to a \l{QWidget}{widget} for
    which Qt::WA_NoMousePropagation has been set, that mouse event
    will not be propagated further up the parent widget chain.

    The state of the keyboard modifier keys can be found by calling the
    \l{QInputEvent::modifiers()}{modifiers()} function, inherited from
    QInputEvent.

    The functions pos(), x(), and y() give the cursor position
    relative to the widget that receives the mouse event. If you
    move the widget as a result of the mouse event, use the global
    position returned by globalPos() to avoid a shaking motion.

    The QWidget::setEnabled() function can be used to enable or
    disable mouse and keyboard events for a widget.

    Reimplement the QWidget event handlers, QWidget::mousePressEvent(),
    QWidget::mouseReleaseEvent(), QWidget::mouseDoubleClickEvent(),
    and QWidget::mouseMoveEvent() to receive mouse events in your own
    widgets.

    \sa QWidget::setMouseTracking() QWidget::grabMouse() QCursor::pos()
*/

/*!
    Constructs a mouse event object.

    The \a type parameter must be one of QEvent::MouseButtonPress,
    QEvent::MouseButtonRelease, QEvent::MouseButtonDblClick,
    or QEvent::MouseMove.

    The \a position is the mouse cursor's position relative to the
    receiving widget.
    The \a button that caused the event is given as a value from
    the Qt::MouseButton enum. If the event \a type is
    \l MouseMove, the appropriate button for this event is Qt::NoButton.
    The mouse and keyboard states at the time of the event are specified by
    \a buttons and \a modifiers.

    The globalPos() is initialized to QCursor::pos(), which may not
    be appropriate. Use the other constructor to specify the global
    position explicitly.
*/

QMouseEvent::QMouseEvent(Type type, const QPoint &position, Qt::MouseButton button,
                         Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
    : QInputEvent(type, modifiers), p(position), b(button), mouseState(buttons)
{
    g = QCursor::pos();
}

QMouseEvent::QMouseEvent(Type type, const QPointF &position, Qt::MouseButton button,
                         Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
    : QInputEvent(type, modifiers), p(position), b(button), mouseState(buttons)
{
    g = QCursor::pos();
}

/*!
    \internal
*/
QMouseEvent::~QMouseEvent()
{
}



/*!
    Constructs a mouse event object.

    The \a type parameter must be QEvent::MouseButtonPress,
    QEvent::MouseButtonRelease, QEvent::MouseButtonDblClick,
    or QEvent::MouseMove.

    The \a pos is the mouse cursor's position relative to the
    receiving widget. The cursor's position in global coordinates is
    specified by \a globalPos.  The \a button that caused the event is
    given as a value from the \l Qt::MouseButton enum. If the event \a
    type is \l MouseMove, the appropriate button for this event is
    Qt::NoButton. \a buttons is the state of all buttons at the
    time of the event, \a modifiers the state of all keyboard
    modifiers.

*/
QMouseEvent::QMouseEvent(Type type, const QPoint &pos, const QPoint &globalPos,
                         Qt::MouseButton button, Qt::MouseButtons buttons,
                         Qt::KeyboardModifiers modifiers)
    : QInputEvent(type, modifiers), p(pos), g(globalPos), b(button), mouseState(buttons)
{}

QMouseEvent::QMouseEvent(Type type, const QPointF &pos, const QPointF &globalPos,
                         Qt::MouseButton button, Qt::MouseButtons buttons,
                         Qt::KeyboardModifiers modifiers)
    : QInputEvent(type, modifiers), p(pos), g(globalPos), b(button), mouseState(buttons)
{}

/*!
    \fn const QPoint &QMouseEvent::pos() const

    Returns the position of the mouse cursor, relative to the widget
    that received the event.

    If you move the widget as a result of the mouse event, use the
    global position returned by globalPos() to avoid a shaking
    motion.

    \sa x() y() globalPos()
*/

/*!
    \fn const QPointF &QMouseEvent::posF() const
    \since 4.4

    Returns the position of the mouse cursor as a QPointF, relative to the
    widget that received the event.

    If you move the widget as a result of the mouse event, use the
    global position returned by globalPos() to avoid a shaking
    motion.

    \sa x() y() pos() globalPos()
*/

/*!
    \fn const QPoint &QMouseEvent::globalPos() const

    Returns the global position of the mouse cursor \e{at the time
    of the event}. This is important on asynchronous window systems
    like X11. Whenever you move your widgets around in response to
    mouse events, globalPos() may differ a lot from the current
    pointer position QCursor::pos(), and from
    QWidget::mapToGlobal(pos()).

    \sa globalX() globalY()
*/

/*!
    \fn const QPointF &QMouseEvent::globalPosF() const

    Returns the global position of the mouse cursor \e{at the time
    of the event}. This is important on asynchronous window systems
    like X11. Whenever you move your widgets around in response to
    mouse events, globalPos() may differ a lot from the current
    pointer position QCursor::pos(), and from
    QWidget::mapToGlobal(pos()).

    \sa globalX() globalY()
*/

/*!
    \fn int QMouseEvent::x() const

    Returns the x position of the mouse cursor, relative to the
    widget that received the event.

    \sa y() pos()
*/

/*!
    \fn int QMouseEvent::y() const

    Returns the y position of the mouse cursor, relative to the
    widget that received the event.

    \sa x() pos()
*/

/*!
    \fn int QMouseEvent::globalX() const

    Returns the global x position of the mouse cursor at the time of
    the event.

    \sa globalY() globalPos()
*/

/*!
    \fn int QMouseEvent::globalY() const

    Returns the global y position of the mouse cursor at the time of
    the event.

    \sa globalX() globalPos()
*/

/*!
    \fn Qt::MouseButton QMouseEvent::button() const

    Returns the button that caused the event.

    Note that the returned value is always Qt::NoButton for mouse
    move events.

    \sa buttons() Qt::MouseButton
*/

/*!
    \fn Qt::MouseButton QMouseEvent::buttons() const

    Returns the button state when the event was generated. The button
    state is a combination of Qt::LeftButton, Qt::RightButton,
    Qt::MiddleButton using the OR operator. For mouse move events,
    this is all buttons that are pressed down. For mouse press and
    double click events this includes the button that caused the
    event. For mouse release events this excludes the button that
    caused the event.

    \sa button() Qt::MouseButton
*/


/*!
    \fn Qt::ButtonState QMouseEvent::state() const

    Returns the button state immediately before the event was
    generated. The button state is a combination of mouse buttons
    (see Qt::ButtonState) and keyboard modifiers (Qt::MouseButtons).

    Use buttons() and/or modifiers() instead. Be aware that buttons()
    return the state immediately \e after the event was generated.
*/

/*!
    \fn Qt::ButtonState QMouseEvent::stateAfter() const

    Returns the button state immediately after the event was
    generated. The button state is a combination of mouse buttons
    (see Qt::ButtonState) and keyboard modifiers (Qt::MouseButtons).

    Use buttons() and/or modifiers() instead.
*/

/*!
    \class QHoverEvent
    \ingroup events

    \brief The QHoverEvent class contains parameters that describe a mouse event.

    Mouse events occur when a mouse cursor is moved into, out of, or within a
    widget, and if the widget has the Qt::WA_Hover attribute.

    The function pos() gives the current cursor position, while oldPos() gives
    the old mouse position.

    There are a few similarities between the events QEvent::HoverEnter
    and QEvent::HoverLeave, and the events QEvent::Enter and QEvent::Leave.
    However, they are slightly different because we do an update() in the event
    handler of HoverEnter and HoverLeave.

    QEvent::HoverMove is also slightly different from QEvent::MouseMove. Let us
    consider a top-level window A containing a child B which in turn contains a
    child C (all with mouse tracking enabled):

    \image hoverevents.png

    Now, if you move the cursor from the top to the bottom in the middle of A,
    you will get the following QEvent::MouseMove events:

    \list 1
        \o A::MouseMove
        \o B::MouseMove
        \o C::MouseMove
    \endlist

    You will get the same events for QEvent::HoverMove, except that the event
    always propagates to the top-level regardless whether the event is accepted
    or not. It will only stop propagating with the Qt::WA_NoMousePropagation
    attribute.

    In this case the events will occur in the following way:

    \list 1
        \o A::HoverMove
        \o A::HoverMove, B::HoverMove
        \o A::HoverMove, B::HoverMove, C::HoverMove
    \endlist

*/

/*!
    \fn const QPoint &QHoverEvent::pos() const

    Returns the position of the mouse cursor, relative to the widget
    that received the event.

    On QEvent::HoverLeave events, this position will always be
    QPoint(-1, -1).

    \sa oldPos()
*/

/*!
    \fn const QPoint &QHoverEvent::oldPos() const

    Returns the previous position of the mouse cursor, relative to the widget
    that received the event. If there is no previous position, oldPos() will
    return the same position as pos().

    On QEvent::HoverEnter events, this position will always be
    QPoint(-1, -1).

    \sa pos()
*/

/*!
    Constructs a hover event object.

    The \a type parameter must be QEvent::HoverEnter,
    QEvent::HoverLeave, or QEvent::HoverMove.

    The \a pos is the current mouse cursor's position relative to the
    receiving widget, while \a oldPos is the previous mouse cursor's
    position relative to the receiving widget.
*/
QHoverEvent::QHoverEvent(Type type, const QPoint &pos, const QPoint &oldPos)
    : QEvent(type), p(pos), op(oldPos)
{
}

QHoverEvent::QHoverEvent(Type type, const QPointF &pos, const QPointF &oldPos)
    : QEvent(type), p(pos), op(oldPos)
{
}


/*!
    \internal
*/
QHoverEvent::~QHoverEvent()
{
}


/*!
    \class QWheelEvent
    \brief The QWheelEvent class contains parameters that describe a wheel event.

    \ingroup events

    Wheel events are sent to the widget under the mouse cursor, but
    if that widget does not handle the event they are sent to the
    focus widget. The rotation distance is provided by delta().
    The functions pos() and globalPos() return the mouse cursor's
    location at the time of the event.

    A wheel event contains a special accept flag that indicates
    whether the receiver wants the event. You should call ignore() if
    you do not handle the wheel event; this ensures that it will be
    sent to the parent widget.

    The QWidget::setEnabled() function can be used to enable or
    disable mouse and keyboard events for a widget.

    The event handler QWidget::wheelEvent() receives wheel events.

    \sa QMouseEvent QWidget::grabMouse()
*/

/*!
    \fn Qt::MouseButtons QWheelEvent::buttons() const

    Returns the mouse state when the event occurred.
*/

/*!
    \fn Qt::Orientation QWheelEvent::orientation() const

    Returns the wheel's orientation.
*/

/*!
    Constructs a wheel event object.

    The position, \a pos, is the location of the mouse cursor within
    the widget. The globalPos() is initialized to QCursor::pos()
    which is usually, but not always, correct.
    Use the other constructor if you need to specify the global
    position explicitly.

    The \a buttons describe the state of the mouse buttons at the time
    of the event, \a delta contains the rotation distance,
    \a modifiers holds the keyboard modifier flags at the time of the
    event, and \a orient holds the wheel's orientation.

    \sa pos() delta() state()
*/
#ifndef QT_NO_WHEELEVENT
QWheelEvent::QWheelEvent(const QPoint &pos, int delta,
                         Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers,
                         Qt::Orientation orient)
    : QInputEvent(Wheel, modifiers), p(pos), d(delta), mouseState(buttons), o(orient)
{
    g = QCursor::pos();
}

QWheelEvent::QWheelEvent(const QPointF &pos, int delta,
                         Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers,
                         Qt::Orientation orient)
    : QInputEvent(Wheel, modifiers), p(pos), d(delta), mouseState(buttons), o(orient)
{
    g = QCursor::pos();
}

/*!
  \internal
*/
QWheelEvent::~QWheelEvent()
{
}


/*!
    Constructs a wheel event object.

    The \a pos provides the location of the mouse cursor
    within the widget. The position in global coordinates is specified
    by \a globalPos. \a delta contains the rotation distance, \a modifiers
    holds the keyboard modifier flags at the time of the event, and
    \a orient holds the wheel's orientation.

    \sa pos() globalPos() delta() state()
*/
QWheelEvent::QWheelEvent(const QPoint &pos, const QPoint& globalPos, int delta,
                         Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers,
                         Qt::Orientation orient)
    : QInputEvent(Wheel, modifiers), p(pos), g(globalPos), d(delta), mouseState(buttons), o(orient)
{}

QWheelEvent::QWheelEvent(const QPointF &pos, const QPointF& globalPos, int delta,
                         Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers,
                         Qt::Orientation orient)
    : QInputEvent(Wheel, modifiers), p(pos), g(globalPos), d(delta), mouseState(buttons), o(orient)
{}

#endif // QT_NO_WHEELEVENT

/*!
    \fn int QWheelEvent::delta() const

    Returns the distance that the wheel is rotated, in eighths of a
    degree. A positive value indicates that the wheel was rotated
    forwards away from the user; a negative value indicates that the
    wheel was rotated backwards toward the user.

    Most mouse types work in steps of 15 degrees, in which case the
    delta value is a multiple of 120; i.e., 120 units * 1/8 = 15 degrees.

    However, some mice have finer-resolution wheels and send delta values
    that are less than 120 units (less than 15 degrees). To support this
    possibility, you can either cumulatively add the delta values from events
    until the value of 120 is reached, then scroll the widget, or you can
    partially scroll the widget in response to each wheel event.

    Example:

    \snippet doc/src/snippets/code/src_gui_kernel_qevent.cpp 0
*/

/*!
    \fn const QPoint &QWheelEvent::pos() const

    Returns the position of the mouse cursor relative to the widget
    that received the event.

    If you move your widgets around in response to mouse events,
    use globalPos() instead of this function.

    \sa x() y() globalPos()
*/

/*!
    \fn int QWheelEvent::x() const

    Returns the x position of the mouse cursor, relative to the
    widget that received the event.

    \sa y() pos()
*/

/*!
    \fn int QWheelEvent::y() const

    Returns the y position of the mouse cursor, relative to the
    widget that received the event.

    \sa x() pos()
*/


/*!
    \fn const QPoint &QWheelEvent::globalPos() const

    Returns the global position of the mouse pointer \e{at the time
    of the event}. This is important on asynchronous window systems
    such as X11; whenever you move your widgets around in response to
    mouse events, globalPos() can differ a lot from the current
    cursor position returned by QCursor::pos().

    \sa globalX() globalY()
*/

/*!
    \fn int QWheelEvent::globalX() const

    Returns the global x position of the mouse cursor at the time of
    the event.

    \sa globalY() globalPos()
*/

/*!
    \fn int QWheelEvent::globalY() const

    Returns the global y position of the mouse cursor at the time of
    the event.

    \sa globalX() globalPos()
*/


/*! \obsolete
    \fn Qt::ButtonState QWheelEvent::state() const

    Returns the keyboard modifier flags at the time of the event.

    The returned value is a selection of the following values,
    combined using the OR operator: Qt::ShiftButton,
    Qt::ControlButton, and Qt::AltButton.
*/


/*!
    \class QKeyEvent
    \brief The QKeyEvent class describes a key event.

    \ingroup events

    Key events are sent to the widget with keyboard input focus
    when keys are pressed or released.

    A key event contains a special accept flag that indicates whether
    the receiver will handle the key event. You should call ignore()
    if the key press or release event is not handled by your widget.
    A key event is propagated up the parent widget chain until a
    widget accepts it with accept() or an event filter consumes it.
    Key events for multimedia keys are ignored by default. You should
    call accept() if your widget handles those events.

    The QWidget::setEnable() function can be used to enable or disable
    mouse and keyboard events for a widget.

    The event handlers QWidget::keyPressEvent(), QWidget::keyReleaseEvent(),
    QGraphicsItem::keyPressEvent() and QGraphicsItem::keyReleaseEvent()
    receive key events.

    \sa QFocusEvent, QWidget::grabKeyboard()
*/

/*!
    Constructs a key event object.

    The \a type parameter must be QEvent::KeyPress, QEvent::KeyRelease,
    or QEvent::ShortcutOverride.

    Int \a key is the code for the Qt::Key that the event loop should listen
    for. If \a key is 0, the event is not a result of a known key; for
    example, it may be the result of a compose sequence or keyboard macro.
    The \a modifiers holds the keyboard modifiers, and the given \a text
    is the Unicode text that the key generated. If \a autorep is true,
    isAutoRepeat() will be true. \a count is the number of keys involved
    in the event.
*/
QKeyEvent::QKeyEvent(Type type, int key, Qt::KeyboardModifiers modifiers, const QString& text,
                     bool autorep, int count)
    : QInputEvent(type, modifiers), txt(text), k(key), c(count), autor(autorep)
{
}

/*!
    Constructs a key event object.

    The \a type parameter must be QEvent::KeyPress, QEvent::KeyRelease,
    or QEvent::ShortcutOverride.

    Int \a key is the code for the Qt::Key that the event loop should listen
    for. If \a key is 0, the event is not a result of a known key; for
    example, it may be the result of a compose sequence or keyboard macro.
    The \a modifiers holds the keyboard modifiers, and the given \a text
    is the Unicode text that the key generated. If \a autorep is true,
    isAutoRepeat() will be true. \a count is the number of keys involved
    in the event.

    In addition to the normal key event data, also contains \a nativeScanCode,
    \a nativeVirtualKey and \a nativeModifiers. This extra data is used by the
    shortcut system, to determine which shortcuts to trigger.
*/
QKeyEvent::QKeyEvent(Type type, int key, Qt::KeyboardModifiers modifiers,
                     quint32 nativeScanCode, quint32 nativeVirtualKey, quint32 nativeModifiers,
                     const QString &text, bool autorep, int count)
    : QInputEvent(type, modifiers), txt(text), k(key), c(count),
      nScanCode(nativeScanCode), nVirtualKey(nativeVirtualKey), nModifiers(nativeModifiers),
      autor(autorep)
{
}

/*!
  \internal
*/
QKeyEvent::~QKeyEvent()
{
}

/*!
    \fn int QKeyEvent::key() const

    Returns the code of the key that was pressed or released.

    See \l Qt::Key for the list of keyboard codes. These codes are
    independent of the underlying window system. Note that this
    function does not distinguish between capital and non-capital
    letters, use the text() function (returning the Unicode text the
    key generated) for this purpose.

    A value of either 0 or Qt::Key_unknown means that the event is not
    the result of a known key; for example, it may be the result of
    a compose sequence, or a keyboard macro.
*/

/*!
    \fn QString QKeyEvent::text() const

    Returns the Unicode text that this key generated. The text
    returned can be an empty string in cases
    where modifier keys, such as Shift, Control, Alt, and Meta,
    are being pressed or released. In such cases key() will contain
    a valid value.
*/

/*!
    Returns the keyboard modifier flags that existed immediately
    after the event occurred.

    \warning This function cannot always be trusted. The user can
    confuse it by pressing both \key{Shift} keys simultaneously and
    releasing one of them, for example.

    \sa QApplication::keyboardModifiers()
*/
//###### We must check with XGetModifierMapping
Qt::KeyboardModifiers QKeyEvent::modifiers() const
{
    if (key() == Qt::Key_Shift)
        return Qt::KeyboardModifiers(QInputEvent::modifiers()^Qt::ShiftModifier);
    if (key() == Qt::Key_Control)
        return Qt::KeyboardModifiers(QInputEvent::modifiers()^Qt::ControlModifier);
    if (key() == Qt::Key_Alt)
        return Qt::KeyboardModifiers(QInputEvent::modifiers()^Qt::AltModifier);
    if (key() == Qt::Key_Meta)
        return Qt::KeyboardModifiers(QInputEvent::modifiers()^Qt::MetaModifier);
    return QInputEvent::modifiers();
}

#ifndef QT_NO_SHORTCUT
/*!
    \fn bool QKeyEvent::matches(QKeySequence::StandardKey key) const
    \since 4.2

    Returns true if the key event matches the given standard \a key;
    otherwise returns false.
*/
bool QKeyEvent::matches(QKeySequence::StandardKey matchKey) const
{
    uint searchkey = (modifiers() | key()) & ~(Qt::KeypadModifier); //The keypad modifier should not make a difference

    for (ushort i = 0; i < QKeySequencePrivate::numberOfKeyBindings; i++) {
        if (QKeySequencePrivate::keyBindings[i].standardKey == matchKey
            && QKeySequencePrivate::keyBindings[i].shortcut == searchkey) {
            return true;
        }
    }
    return false;
}
#endif // QT_NO_SHORTCUT


/*!
    \fn bool QKeyEvent::isAutoRepeat() const

    Returns true if this event comes from an auto-repeating key;
    returns false if it comes from an initial key press.

    Note that if the event is a multiple-key compressed event that is
    partly due to auto-repeat, this function could return either true
    or false indeterminately.
*/

/*!
    \fn int QKeyEvent::count() const

    Returns the number of keys involved in this event. If text()
    is not empty, this is simply the length of the string.
*/


/*!
    \class QFocusEvent
    \brief The QFocusEvent class contains event parameters for widget focus
    events.

    \ingroup events

    Focus events are sent to widgets when the keyboard input focus
    changes. Focus events occur due to mouse actions, key presses
    (such as \gui{Tab} or \gui{Backtab}), the window system, popup
    menus, keyboard shortcuts, or other application-specific reasons.
    The reason for a particular focus event is returned by reason()
    in the appropriate event handler.

    The event handlers QWidget::focusInEvent(),
    QWidget::focusOutEvent(), QGraphicsItem::focusInEvent and
    QGraphicsItem::focusOutEvent() receive focus events.

    \sa QWidget::setFocus(), QWidget::setFocusPolicy(), {Keyboard Focus}
*/

/*!
    Constructs a focus event object.

    The \a type parameter must be either QEvent::FocusIn or
    QEvent::FocusOut. The \a reason describes the cause of the change
    in focus.
*/
QFocusEvent::QFocusEvent(Type type, Qt::FocusReason reason)
    : QEvent(type), m_reason(reason)
{}

/*!
    \internal
*/
QFocusEvent::~QFocusEvent()
{
}

/*!
    \fn bool QFocusEvent::reason() const

    Returns the reason for this focus event.
 */

/*!
    \fn bool QFocusEvent::gotFocus() const

    Returns true if type() is QEvent::FocusIn; otherwise returns
    false.
*/

/*!
    \fn bool QFocusEvent::lostFocus() const

    Returns true if type() is QEvent::FocusOut; otherwise returns
    false.
*/


/*!
    \class QPaintEvent
    \brief The QPaintEvent class contains event parameters for paint events.

    \ingroup events

    Paint events are sent to widgets that need to update themselves,
    for instance when part of a widget is exposed because a covering
    widget was moved.

    The event contains a region() that needs to be updated, and a
    rect() that is the bounding rectangle of that region. Both are
    provided because many widgets can't make much use of region(),
    and rect() can be much faster than region().boundingRect().

    \section1 Automatic Clipping

    Painting is clipped to region() during the processing of a paint
    event. This clipping is performed by Qt's paint system and is
    independent of any clipping that may be applied to a QPainter used to
    draw on the paint device.

    As a result, the value returned by QPainter::clipRegion() on
    a newly-constructed QPainter will not reflect the clip region that is
    used by the paint system.

    \sa QPainter, QWidget::update(), QWidget::repaint(),
        QWidget::paintEvent()
*/

/*!
    Constructs a paint event object with the region that needs to
    be updated. The region is specified by \a paintRegion.
*/
QPaintEvent::QPaintEvent(const QRegion& paintRegion)
    : QEvent(Paint), m_rect(paintRegion.boundingRect()), m_region(paintRegion)
{}

/*!
    Constructs a paint event object with the rectangle that needs
    to be updated. The region is specified by \a paintRect.
*/
QPaintEvent::QPaintEvent(const QRect &paintRect)
    : QEvent(Paint), m_rect(paintRect),m_region(paintRect)
{}



/*!
  \internal
*/
QPaintEvent::~QPaintEvent()
{
}

/*!
    \fn const QRect &QPaintEvent::rect() const

    Returns the rectangle that needs to be updated.

    \sa region() QPainter::setClipRect()
*/

/*!
    \fn const QRegion &QPaintEvent::region() const

    Returns the region that needs to be updated.

    \sa rect() QPainter::setClipRegion()
*/


QUpdateLaterEvent::QUpdateLaterEvent(const QRegion& paintRegion)
    : QEvent(UpdateLater), m_region(paintRegion)
{
}

QUpdateLaterEvent::~QUpdateLaterEvent()
{
}

/*!
    \class QMoveEvent
    \brief The QMoveEvent class contains event parameters for move events.

    \ingroup events

    Move events are sent to widgets that have been moved to a new
    position relative to their parent.

    The event handler QWidget::moveEvent() receives move events.

    \sa QWidget::move(), QWidget::setGeometry()
*/

/*!
    Constructs a move event with the new and old widget positions,
    \a pos and \a oldPos respectively.
*/
QMoveEvent::QMoveEvent(const QPoint &pos, const QPoint &oldPos)
    : QEvent(Move), p(pos), oldp(oldPos)
{}

/*!
  \internal
*/
QMoveEvent::~QMoveEvent()
{
}

/*!
    \fn const QPoint &QMoveEvent::pos() const

    Returns the new position of the widget. This excludes the window
    frame for top level widgets.
*/

/*!
    \fn const QPoint &QMoveEvent::oldPos() const

    Returns the old position of the widget.
*/


/*!
    \class QResizeEvent
    \brief The QResizeEvent class contains event parameters for resize events.

    \ingroup events

    Resize events are sent to widgets that have been resized.

    The event handler QWidget::resizeEvent() receives resize events.

    \sa QWidget::resize() QWidget::setGeometry()
*/

/*!
    Constructs a resize event with the new and old widget sizes, \a
    size and \a oldSize respectively.
*/
QResizeEvent::QResizeEvent(const QSize &size, const QSize &oldSize)
    : QEvent(Resize), s(size), olds(oldSize)
{}

/*!
  \internal
*/
QResizeEvent::~QResizeEvent()
{
}

/*!
    \fn const QSize &QResizeEvent::size() const

    Returns the new size of the widget. This is the same as
    QWidget::size().
*/

/*!
    \fn const QSize &QResizeEvent::oldSize() const

    Returns the old size of the widget.
*/


/*!
    \class QCloseEvent
    \brief The QCloseEvent class contains parameters that describe a close event.

    \ingroup events

    Close events are sent to widgets that the user wants to close,
    usually by choosing "Close" from the window menu, or by clicking
    the \gui{X} title bar button. They are also sent when you call
    QWidget::close() to close a widget programmatically.

    Close events contain a flag that indicates whether the receiver
    wants the widget to be closed or not. When a widget accepts the
    close event, it is hidden (and destroyed if it was created with
    the Qt::WA_DeleteOnClose flag). If it refuses to accept the close
    event nothing happens. (Under X11 it is possible that the window
    manager will forcibly close the window; but at the time of writing
    we are not aware of any window manager that does this.)

    The event handler QWidget::closeEvent() receives close events. The
    default implementation of this event handler accepts the close
    event. If you do not want your widget to be hidden, or want some
    special handing, you should reimplement the event handler and
    ignore() the event.

    The \l{mainwindows/application#close event handler}{closeEvent() in the
    Application example} shows a close event handler that
    asks whether to save a document before closing.

    If you want the widget to be deleted when it is closed, create it
    with the Qt::WA_DeleteOnClose flag. This is very useful for
    independent top-level windows in a multi-window application.

    \l{QObject}s emits the \l{QObject::destroyed()}{destroyed()}
    signal when they are deleted.

    If the last top-level window is closed, the
    QApplication::lastWindowClosed() signal is emitted.

    The isAccepted() function returns true if the event's receiver has
    agreed to close the widget; call accept() to agree to close the
    widget and call ignore() if the receiver of this event does not
    want the widget to be closed.

    \sa QWidget::close(), QWidget::hide(), QObject::destroyed(),
        QCoreApplication::exec(), QCoreApplication::quit(),
        QApplication::lastWindowClosed()
*/

/*!
    Constructs a close event object.

    \sa accept()
*/
QCloseEvent::QCloseEvent()
    : QEvent(Close)
{}

/*! \internal
*/
QCloseEvent::~QCloseEvent()
{
}

/*!
    \class QContextMenuEvent
    \brief The QContextMenuEvent class contains parameters that describe a context menu event.

    \ingroup events

    Context menu events are sent to widgets when a user performs
    an action associated with opening a context menu.
    The actions required to open context menus vary between platforms;
    for example, on Windows, pressing the menu button or clicking the
    right mouse button will cause this event to be sent.

    When this event occurs it is customary to show a QMenu with a
    context menu, if this is relevant to the context.

    Context menu events contain a special accept flag that indicates
    whether the receiver accepted the event. If the event handler does
    not accept the event then, if possible, whatever triggered the event will be
    handled as a regular input event.
*/

#ifndef QT_NO_CONTEXTMENU
/*!
    Constructs a context menu event object with the accept parameter
    flag set to false.

    The \a reason parameter must be QContextMenuEvent::Mouse or
    QContextMenuEvent::Keyboard.

    The \a pos parameter specifies the mouse position relative to the
    receiving widget. \a globalPos is the mouse position in absolute
    coordinates.
*/
QContextMenuEvent::QContextMenuEvent(Reason reason, const QPoint &pos, const QPoint &globalPos)
    : QInputEvent(ContextMenu), p(pos), gp(globalPos), reas(reason)
{}

/*!
    Constructs a context menu event object with the accept parameter
    flag set to false.

    The \a reason parameter must be QContextMenuEvent::Mouse or
    QContextMenuEvent::Keyboard.

    The \a pos parameter specifies the mouse position relative to the
    receiving widget. \a globalPos is the mouse position in absolute
    coordinates. The \a modifiers holds the keyboard modifiers.
*/
QContextMenuEvent::QContextMenuEvent(Reason reason, const QPoint &pos, const QPoint &globalPos,
                                     Qt::KeyboardModifiers modifiers)
    : QInputEvent(ContextMenu, modifiers), p(pos), gp(globalPos), reas(reason)
{}


/*! \internal */
QContextMenuEvent::~QContextMenuEvent()
{
}
/*!
    Constructs a context menu event object with the accept parameter
    flag set to false.

    The \a reason parameter must be QContextMenuEvent::Mouse or
    QContextMenuEvent::Keyboard.

    The \a pos parameter specifies the mouse position relative to the
    receiving widget.

    The globalPos() is initialized to QCursor::pos(), which may not be
    appropriate. Use the other constructor to specify the global
    position explicitly.
*/
QContextMenuEvent::QContextMenuEvent(Reason reason, const QPoint &pos)
    : QInputEvent(ContextMenu), p(pos), reas(reason)
{
    gp = QCursor::pos();
}


/*!
    \fn const QPoint &QContextMenuEvent::pos() const

    Returns the position of the mouse pointer relative to the widget
    that received the event.

    \sa x(), y(), globalPos()
*/

/*!
    \fn int QContextMenuEvent::x() const

    Returns the x position of the mouse pointer, relative to the
    widget that received the event.

    \sa y(), pos()
*/

/*!
    \fn int QContextMenuEvent::y() const

    Returns the y position of the mouse pointer, relative to the
    widget that received the event.

    \sa x(), pos()
*/

/*!
    \fn const QPoint &QContextMenuEvent::globalPos() const

    Returns the global position of the mouse pointer at the time of
    the event.

    \sa x(), y(), pos()
*/

/*!
    \fn int QContextMenuEvent::globalX() const

    Returns the global x position of the mouse pointer at the time of
    the event.

    \sa globalY(), globalPos()
*/

/*!
    \fn int QContextMenuEvent::globalY() const

    Returns the global y position of the mouse pointer at the time of
    the event.

    \sa globalX(), globalPos()
*/
#endif // QT_NO_CONTEXTMENU

/*!
    \fn Qt::ButtonState QContextMenuEvent::state() const

    Returns the button state (a combination of mouse buttons
    and keyboard modifiers) immediately before the event was
    generated.

    The returned value is a selection of the following values,
    combined with the OR operator:
    Qt::LeftButton, Qt::RightButton, Qt::MiddleButton,
    Qt::ShiftButton, Qt::ControlButton, and Qt::AltButton.
*/

/*!
    \enum QContextMenuEvent::Reason

    This enum describes the reason why the event was sent.

    \value Mouse The mouse caused the event to be sent. Normally this
    means the right mouse button was clicked, but this is platform
    dependent.

    \value Keyboard The keyboard caused this event to be sent. On
    Windows, this means the menu button was pressed.

    \value Other The event was sent by some other means (i.e. not by
    the mouse or keyboard).
*/


/*!
    \fn QContextMenuEvent::Reason QContextMenuEvent::reason() const

    Returns the reason for this context event.
*/

#ifndef QT_NO_DRAGANDDROP
/*!
    Creates a QDragMoveEvent of the required \a type indicating
    that the mouse is at position \a pos given within a widget.

    The mouse and keyboard states are specified by \a buttons and
    \a modifiers, and the \a actions describe the types of drag
    and drop operation that are possible.
    The drag data is passed as MIME-encoded information in \a data.

    \warning Do not attempt to create a QDragMoveEvent yourself.
    These objects rely on Qt's internal state.
*/
QDragMoveEvent::QDragMoveEvent(const QPoint& pos, Qt::DropActions actions, const QMimeData *data,
                               Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Type type)
    : QDropEvent(pos, actions, data, buttons, modifiers, type)
    , rect(pos, QSize(1, 1))
{}

/*!
    Destroys the event.
*/
QDragMoveEvent::~QDragMoveEvent()
{
}

/*!
    \fn void QDragMoveEvent::accept(bool y)

    Calls setAccepted(\a y) instead.
*/

/*!
    \fn void QDragMoveEvent::accept(const QRect &rectangle)

    The same as accept(), but also notifies that future moves will
    also be acceptable if they remain within the \a rectangle
    given on the widget. This can improve performance, but may
    also be ignored by the underlying system.

    If the rectangle is empty, drag move events will be sent
    continuously. This is useful if the source is scrolling in a
    timer event.
*/

/*!
    \fn void QDragMoveEvent::accept()

    \overload

    Calls QDropEvent::accept().
*/

/*!
    \fn void QDragMoveEvent::ignore()

    \overload

    Calls QDropEvent::ignore().
*/

/*!
    \fn void QDragMoveEvent::ignore(const QRect &rectangle)

    The opposite of the accept(const QRect&) function.
    Moves within the \a rectangle are not acceptable, and will be
    ignored.
*/

/*!
    \fn QRect QDragMoveEvent::answerRect() const

    Returns the rectangle in the widget where the drop will occur if accepted.
    You can use this information to restrict drops to certain places on the
    widget.
*/


/*!
    \class QDropEvent
    \ingroup events
    \ingroup draganddrop

    \brief The QDropEvent class provides an event which is sent when a
    drag and drop action is completed.

    When a widget \l{QWidget::setAcceptDrops()}{accepts drop events}, it will
    receive this event if it has accepted the most recent QDragEnterEvent or
    QDragMoveEvent sent to it.

    The drop event contains a proposed action, available from proposedAction(), for
    the widget to either accept or ignore. If the action can be handled by the
    widget, you should call the acceptProposedAction() function. Since the
    proposed action can be a combination of \l Qt::DropAction values, it may be
    useful to either select one of these values as a default action or ask
    the user to select their preferred action.

    If the proposed drop action is not suitable, perhaps because your custom
    widget does not support that action, you can replace it with any of the
    \l{possibleActions()}{possible drop actions} by calling setDropAction()
    with your preferred action. If you set a value that is not present in the
    bitwise OR combination of values returned by possibleActions(), the default
    copy action will be used. Once a replacement drop action has been set, call
    accept() instead of acceptProposedAction() to complete the drop operation.

    The mimeData() function provides the data dropped on the widget in a QMimeData
    object. This contains information about the MIME type of the data in addition to
    the data itself.

    \sa QMimeData, QDrag, {Drag and Drop}
*/

/*!
    \fn const QMimeData *QDropEvent::mimeData() const

    Returns the data that was dropped on the widget and its associated MIME
    type information.
*/

/*!
    Constructs a drop event of a certain \a type corresponding to a
    drop at the point specified by \a pos in the destination widget's
    coordinate system.

    The \a actions indicate which types of drag and drop operation can
    be performed, and the drag data is stored as MIME-encoded data in \a data.

    The states of the mouse buttons and keyboard modifiers at the time of
    the drop are specified by \a buttons and \a modifiers.
*/ // ### pos is in which coordinate system?
QDropEvent::QDropEvent(const QPoint& pos, Qt::DropActions actions, const QMimeData *data,
                       Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Type type)
    : QEvent(type), p(pos), mouseState(buttons),
      modState(modifiers), act(actions),
      mdata(data)
{
    default_action = QDragManager::self()->defaultAction(act, modifiers);
    drop_action = default_action;
    ignore();
}

/*! \internal */
QDropEvent::~QDropEvent()
{
}

/*!
    If the source of the drag operation is a widget in this
    application, this function returns that source; otherwise it
    returns 0. The source of the operation is the first parameter to
    the QDrag object used instantiate the drag.

    This is useful if your widget needs special behavior when dragging
    to itself.

    \sa QDrag::QDrag()
*/
QWidget* QDropEvent::source() const
{
    QDragManager *manager = QDragManager::self();
    return manager ? manager->source() : 0;
}


void QDropEvent::setDropAction(Qt::DropAction action)
{
    if (!(action & act) && action != Qt::IgnoreAction)
        action = default_action;
    drop_action = action;
}

/*!
    \fn const QPoint& QDropEvent::pos() const

    Returns the position where the drop was made.
*/

/*!
    \fn Qt::MouseButtons QDropEvent::mouseButtons() const

    Returns the mouse buttons that are pressed..
*/

/*!
    \fn Qt::KeyboardModifiers QDropEvent::keyboardModifiers() const

    Returns the modifier keys that are pressed.
*/

/*!
    \fn void QDropEvent::accept()
    \internal
*/

/*!
    \fn void QDropEvent::accept(bool accept)

    Call setAccepted(\a accept) instead.
*/

/*!
    \fn void QDropEvent::acceptAction(bool accept = true)

    Call this to indicate that the action described by action() is
    accepted (i.e. if \a accept is true, which is the default), not merely
    the default copy action. If you call acceptAction(true), there is
    no need to also call accept(true).
*/

/*!
    \enum QDropEvent::Action
    \compat

    When a drag and drop action is completed, the target is expected
    to perform an action on the data provided by the source. This
    will be one of the following:

    \value Copy The default action. The source simply uses the data
                provided in the operation.
    \value Link The source should somehow create a link to the
                location specified by the data.
    \value Move The source should somehow move the object from the
                location specified by the data to a new location.
    \value Private  The target has special knowledge of the MIME type,
                which the source should respond to in a similar way to
                a Copy.
    \value UserAction  The source and target can co-operate using
                special actions. This feature is not currently
                supported.

    The Link and Move actions only makes sense if the data is a
    reference, for example, text/uri-list file lists (see QUriDrag).
*/

/*!
    \fn void QDropEvent::setDropAction(Qt::DropAction action)

    Sets the \a action to be performed on the data by the target.
    Use this to override the \l{proposedAction()}{proposed action}
    with one of the \l{possibleActions()}{possible actions}.

    If you set a drop action that is not one of the possible actions, the
    drag and drop operation will default to a copy operation.

    Once you have supplied a replacement drop action, call accept()
    instead of acceptProposedAction().

    \sa dropAction()
*/

/*!
    \fn Qt::DropAction QDropEvent::dropAction() const

    Returns the action to be performed on the data by the target. This may be
    different from the action supplied in proposedAction() if you have called
    setDropAction() to explicitly choose a drop action.

    \sa setDropAction()
*/

/*!
    \fn Qt::DropActions QDropEvent::possibleActions() const

    Returns an OR-combination of possible drop actions.

    \sa dropAction()
*/

/*!
    \fn Qt::DropAction QDropEvent::proposedAction() const

    Returns the proposed drop action.

    \sa dropAction()
*/

/*!
    \fn void QDropEvent::acceptProposedAction()

    Sets the drop action to be the proposed action.

    \sa setDropAction(), proposedAction(), {QEvent::accept()}{accept()}
*/


/*!
    \fn void QDropEvent::setPoint(const QPoint &point)
    \compat

    Sets the drop to happen at the given \a point. You do not normally
    need to use this as it will be set internally before your widget
    receives the drop event.
*/ // ### here too - what coordinate system?


/*!
    \class QDragEnterEvent
    \brief The QDragEnterEvent class provides an event which is sent
    to a widget when a drag and drop action enters it.

    \ingroup events
    \ingroup draganddrop

    A widget must accept this event in order to receive the \l
    {QDragMoveEvent}{drag move events} that are sent while the drag
    and drop action is in progress. The drag enter event is always
    immediately followed by a drag move event.

    QDragEnterEvent inherits most of its functionality from
    QDragMoveEvent, which in turn inherits most of its functionality
    from QDropEvent.

    \sa QDragLeaveEvent, QDragMoveEvent, QDropEvent
*/

/*!
    Constructs a QDragEnterEvent that represents a drag entering a
    widget at the given \a point with mouse and keyboard states specified by
    \a buttons and \a modifiers.

    The drag data is passed as MIME-encoded information in \a data, and the
    specified \a actions describe the possible types of drag and drop
    operation that can be performed.

    \warning Do not create a QDragEnterEvent yourself since these
    objects rely on Qt's internal state.
*/
QDragEnterEvent::QDragEnterEvent(const QPoint& point, Qt::DropActions actions, const QMimeData *data,
                                 Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
    : QDragMoveEvent(point, actions, data, buttons, modifiers, DragEnter)
{}

/*! \internal
*/
QDragEnterEvent::~QDragEnterEvent()
{
}

/*!
    \class QDragMoveEvent
    \brief The QDragMoveEvent class provides an event which is sent while a drag and drop action is in progress.

    \ingroup events
    \ingroup draganddrop

    A widget will receive drag move events repeatedly while the drag
    is within its boundaries, if it accepts
    \l{QWidget::setAcceptDrops()}{drop events} and \l
    {QWidget::dragEnterEvent()}{enter events}. The widget should
    examine the event to see what kind of data it
    \l{QDragMoveEvent::provides()}{provides}, and call the accept()
    function to accept the drop if appropriate.

    The rectangle supplied by the answerRect() function can be used to restrict
    drops to certain parts of the widget. For example, we can check whether the
    rectangle intersects with the geometry of a certain child widget and only
    call \l{QDropEvent::acceptProposedAction()}{acceptProposedAction()} if that
    is the case.

    Note that this class inherits most of its functionality from
    QDropEvent.

    \sa QDragEnterEvent, QDragLeaveEvent, QDropEvent
*/

/*!
    \class QDragLeaveEvent
    \brief The QDragLeaveEvent class provides an event that is sent to a widget when a drag and drop action leaves it.

    \ingroup events
    \ingroup draganddrop

    This event is always preceded by a QDragEnterEvent and a series
    of \l{QDragMoveEvent}s. It is not sent if a QDropEvent is sent
    instead.

    \sa QDragEnterEvent, QDragMoveEvent, QDropEvent
*/

/*!
    Constructs a QDragLeaveEvent.

    \warning Do not create a QDragLeaveEvent yourself since these
    objects rely on Qt's internal state.
*/
QDragLeaveEvent::QDragLeaveEvent()
    : QEvent(DragLeave)
{}

/*! \internal
*/
QDragLeaveEvent::~QDragLeaveEvent()
{
}
#endif // QT_NO_DRAGANDDROP

/*!
    \class QHelpEvent
    \brief The QHelpEvent class provides an event that is used to request helpful information
    about a particular point in a widget.

    \ingroup events
    \ingroup helpsystem

    This event can be intercepted in applications to provide tooltips
    or "What's This?" help for custom widgets. The type() can be
    either QEvent::ToolTip or QEvent::WhatsThis.

    \sa QToolTip, QWhatsThis, QStatusTipEvent, QWhatsThisClickedEvent
*/

/*!
    Constructs a help event with the given \a type corresponding to the
    widget-relative position specified by \a pos and the global position
    specified by \a globalPos.

    \a type must be either QEvent::ToolTip or QEvent::WhatsThis.

    \sa pos(), globalPos()
*/
QHelpEvent::QHelpEvent(Type type, const QPoint &pos, const QPoint &globalPos)
    : QEvent(type), p(pos), gp(globalPos)
{}

/*!
    \fn int QHelpEvent::x() const

    Same as pos().x().

    \sa y(), pos(), globalPos()
*/

/*!
    \fn int QHelpEvent::y() const

    Same as pos().y().

    \sa x(), pos(), globalPos()
*/

/*!
    \fn int QHelpEvent::globalX() const

    Same as globalPos().x().

    \sa x(), globalY(), globalPos()
*/

/*!
    \fn int QHelpEvent::globalY() const

    Same as globalPos().y().

    \sa y(), globalX(), globalPos()
*/

/*!
    \fn const QPoint &QHelpEvent::pos()  const

    Returns the mouse cursor position when the event was generated,
    relative to the widget to which the event is dispatched.

    \sa globalPos(), x(), y()
*/

/*!
    \fn const QPoint &QHelpEvent::globalPos() const

    Returns the mouse cursor position when the event was generated
    in global coordinates.

    \sa pos(), globalX(), globalY()
*/

/*! \internal
*/
QHelpEvent::~QHelpEvent()
{
}

#ifndef QT_NO_STATUSTIP

/*!
    \class QStatusTipEvent
    \brief The QStatusTipEvent class provides an event that is used to show messages in a status bar.

    \ingroup events
    \ingroup helpsystem

    Status tips can be set on a widget using the
    QWidget::setStatusTip() function.  They are shown in the status
    bar when the mouse cursor enters the widget. For example:

    \table 100%
    \row
    \o
    \snippet doc/src/snippets/qstatustipevent/main.cpp 1
    \dots
    \snippet doc/src/snippets/qstatustipevent/main.cpp 3
    \o
    \image qstatustipevent-widget.png Widget with status tip.
    \endtable

    Status tips can also be set on actions using the
    QAction::setStatusTip() function:

    \table 100%
    \row
    \o
    \snippet doc/src/snippets/qstatustipevent/main.cpp 0
    \snippet doc/src/snippets/qstatustipevent/main.cpp 2
    \dots
    \snippet doc/src/snippets/qstatustipevent/main.cpp 3
    \o
    \image qstatustipevent-action.png Action with status tip.
    \endtable

    Finally, status tips are supported for the item view classes
    through the Qt::StatusTipRole enum value.

    \sa QStatusBar, QHelpEvent, QWhatsThisClickedEvent
*/

/*!
    Constructs a status tip event with the text specified by \a tip.

    \sa tip()
*/
QStatusTipEvent::QStatusTipEvent(const QString &tip)
    : QEvent(StatusTip), s(tip)
{}

/*! \internal
*/
QStatusTipEvent::~QStatusTipEvent()
{
}

/*!
    \fn QString QStatusTipEvent::tip() const

    Returns the message to show in the status bar.

    \sa QStatusBar::showMessage()
*/

#endif // QT_NO_STATUSTIP

#ifndef QT_NO_WHATSTHIS

/*!
    \class QWhatsThisClickedEvent
    \brief The QWhatsThisClickedEvent class provides an event that
    can be used to handle hyperlinks in a "What's This?" text.

    \ingroup events
    \ingroup helpsystem

    \sa QWhatsThis, QHelpEvent, QStatusTipEvent
*/

/*!
    Constructs an event containing a URL specified by \a href when a link
    is clicked in a "What's This?" message.

    \sa href()
*/
QWhatsThisClickedEvent::QWhatsThisClickedEvent(const QString &href)
    : QEvent(WhatsThisClicked), s(href)
{}

/*! \internal
*/
QWhatsThisClickedEvent::~QWhatsThisClickedEvent()
{
}

/*!
    \fn QString QWhatsThisClickedEvent::href() const

    Returns the URL that was clicked by the user in the "What's
    This?" text.
*/

#endif // QT_NO_WHATSTHIS

#ifndef QT_NO_ACTION

/*!
    \class QActionEvent
    \brief The QActionEvent class provides an event that is generated
    when a QAction is added, removed, or changed.

    \ingroup events

    Actions can be added to widgets using QWidget::addAction(). This
    generates an \l ActionAdded event, which you can handle to provide
    custom behavior. For example, QToolBar reimplements
    QWidget::actionEvent() to create \l{QToolButton}s for the
    actions.

    \sa QAction, QWidget::addAction(), QWidget::removeAction(), QWidget::actions()
*/

/*!
    Constructs an action event. The \a type can be \l ActionChanged,
    \l ActionAdded, or \l ActionRemoved.

    \a action is the action that is changed, added, or removed. If \a
    type is ActionAdded, the action is to be inserted before the
    action \a before. If \a before is 0, the action is appended.
*/
QActionEvent::QActionEvent(QEvent::Type type, QAction *action, QAction *before)
    : QEvent(type), act(action), bef(before)
{}

/*! \internal
*/
QActionEvent::~QActionEvent()
{
}

/*!
    \fn QAction *QActionEvent::action() const

    Returns the action that is changed, added, or removed.

    \sa before()
*/

/*!
    \fn QAction *QActionEvent::before() const

    If type() is \l ActionAdded, returns the action that should
    appear before action(). If this function returns 0, the action
    should be appended to already existing actions on the same
    widget.

    \sa action(), QWidget::actions()
*/

#endif // QT_NO_ACTION

/*!
    \class QHideEvent
    \brief The QHideEvent class provides an event which is sent after a widget is hidden.

    \ingroup events

    This event is sent just before QWidget::hide() returns, and also
    when a top-level window has been hidden (iconified) by the user.

    If spontaneous() is true, the event originated outside the
    application. In this case, the user hid the window using the
    window manager controls, either by iconifying the window or by
    switching to another virtual desktop where the window isn't
    visible. The window will become hidden but not withdrawn. If the
    window was iconified, QWidget::isMinimized() returns true.

    \sa QShowEvent
*/

/*!
    Constructs a QHideEvent.
*/
QHideEvent::QHideEvent()
    : QEvent(Hide)
{}

/*! \internal
*/
QHideEvent::~QHideEvent()
{
}

/*!
    \class QShowEvent
    \brief The QShowEvent class provides an event that is sent when a widget is shown.

    \ingroup events

    There are two kinds of show events: show events caused by the
    window system (spontaneous), and internal show events. Spontaneous (QEvent::spontaneous())
    show events are sent just after the window system shows the
    window; they are also sent when a top-level window is redisplayed
    after being iconified. Internal show events are delivered just
    before the widget becomes visible.

    \sa QHideEvent
*/

/*!
    Constructs a QShowEvent.
*/
QShowEvent::QShowEvent()
    : QEvent(Show)
{}

/*! \internal
*/
QShowEvent::~QShowEvent()
{
}

/*!
  \fn QByteArray QDropEvent::data(const char* f) const

  \obsolete

  The encoded data is in \a f.
  Use QDropEvent::encodedData().
*/

#ifndef QT_NO_SHORTCUT

/*!
    Constructs a shortcut event for the given \a key press,
    associated with the QShortcut ID \a id.

    \a ambiguous specifies whether there is more than one QShortcut
    for the same key sequence.
*/
QShortcutEvent::QShortcutEvent(const QKeySequence &key, int id, bool ambiguous)
    : QEvent(Shortcut), sequence(key), ambig(ambiguous), sid(id)
{
}

/*!
    Destroys the event object.
*/
QShortcutEvent::~QShortcutEvent()
{
}

#endif // QT_NO_SHORTCUT

#ifndef QT_NO_DEBUG_STREAM

static const char *eventTypeName(QEvent::Type t)
{
    static const int enumIdx = QEvent::staticMetaObject.indexOfEnumerator("Type");
    return t <= QEvent::User
        ? QEvent::staticMetaObject.enumerator(enumIdx).valueToKey(t)
        : "User";
}

static const char *eventClassName(QEvent::Type t)
{
    switch (t) {
    case QEvent::ActionAdded:
    case QEvent::ActionRemoved:
    case QEvent::ActionChanged:
        return "QActionEvent";
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseMove:
    case QEvent::NonClientAreaMouseMove:
    case QEvent::NonClientAreaMouseButtonPress:
    case QEvent::NonClientAreaMouseButtonRelease:
    case QEvent::NonClientAreaMouseButtonDblClick:
        return "QMouseEvent";
    case QEvent::DragEnter:
        return "QDragEnterEvent";
    case QEvent::DragMove:
        return "QDragMoveEvent";
    case QEvent::Drop:
        return "QDropEvent";
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::ShortcutOverride:
        return "QKeyEvent";
    case QEvent::FocusIn:
    case QEvent::FocusOut:
        return "QFocusEvent";
    case QEvent::ChildAdded:
    case QEvent::ChildPolished:
    case QEvent::ChildRemoved:
        return "QChildEvent";
    case QEvent::Paint:
        return "QPaintEvent";
    case QEvent::Move:
        return "QMoveEvent";
    case QEvent::Resize:
        return "QResizeEvent";
    case QEvent::Show:
        return "QShowEvent";
    case QEvent::Hide:
        return "QHideEvent";
    case QEvent::Enter:
        return "QEnterEvent";
    case QEvent::Close:
        return "QCloseEvent";
    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
    case QEvent::HoverMove:
        return "QHoverEvent";;
    case QEvent::StatusTip:
        return "QStatusTipEvent";
    case QEvent::ToolTip:
        return "QHelpEvent";
    case QEvent::WindowStateChange:
        return "QWindowStateChangeEvent";
    case QEvent::Wheel:
        return "QWheelEvent";
    case QEvent::Shortcut:
        return "QShortcutEvent";
    case QEvent::GraphicsSceneMouseMove:
    case QEvent::GraphicsSceneMousePress:
    case QEvent::GraphicsSceneMouseRelease:
    case QEvent::GraphicsSceneMouseDoubleClick:
        return "QGraphicsSceneMouseEvent";
    case QEvent::GraphicsSceneContextMenu:
    case QEvent::GraphicsSceneHoverEnter:
    case QEvent::GraphicsSceneHoverMove:
    case QEvent::GraphicsSceneHoverLeave:
    case QEvent::GraphicsSceneLeave:
    case QEvent::GraphicsSceneHelp:
    case QEvent::GraphicsSceneDragEnter:
    case QEvent::GraphicsSceneDragMove:
    case QEvent::GraphicsSceneDragLeave:
    case QEvent::GraphicsSceneDrop:
    case QEvent::GraphicsSceneWheel:
        return "QGraphicsSceneEvent";
    case QEvent::Timer:
        return "QTimerEvent";
    default:
        break;
    }
    return "QEvent";
}

static const char *mouseButtonToString(Qt::MouseButton button)
{
    switch (button) {
        case Qt::NoButton:
            return "Qt::NoButton";
        case Qt::LeftButton:
            return "Qt::LeftButton";
        case Qt::RightButton:
            return "Qt::RightButton";
        case Qt::MiddleButton:
            return "Qt::MiddleButton";
        default:
            Q_ASSERT_X(false, "mouseButtonToString", "internal error");
    }
    Q_UNREACHABLE();
}

static QByteArray mouseButtonsToString(Qt::MouseButtons buttons)
{
    QByteArray result;
    for (int i = 0; (uint)(1 << i) <= Qt::MouseButtonMask; ++i) {
        const Qt::MouseButton button = static_cast<Qt::MouseButton>(1 << i);
        if (buttons.testFlag(button)) {
            if (!result.isEmpty())
                result.append('|');
            result.append(mouseButtonToString(button));
        }
    }
    if (result.isEmpty())
        result.append("NoButton");
    return result;
}

#  ifndef QT_NO_DRAGANDDROP

static void formatDropEvent(QDebug d, const QDropEvent *e)
{
    const QEvent::Type type = e->type();
    d << eventClassName(type) << "(dropAction=" << e->dropAction() << ", proposedAction="
        << e->proposedAction() << ", possibleActions=" << e->possibleActions();
    if (type == QEvent::DragMove || type == QEvent::DragEnter)
        d << ", answerRect=" << static_cast<const QDragMoveEvent *>(e)->answerRect();
    d << ", formats=" << e->mimeData()->formats();
    if (const Qt::KeyboardModifiers mods = e->keyboardModifiers())
        d << ", keyboardModifiers=" << mods;
    d << ", " << mouseButtonsToString(e->mouseButtons()).constData();
}

#  endif // !QT_NO_DRAGANDDROP

QDebug operator<<(QDebug dbg, const QEvent *e) {
#ifndef Q_BROKEN_DEBUG_STREAM
    dbg.nospace();
    if (!e) {
        dbg << "QEvent(this = 0x0)";
        dbg.space();
        return dbg;
    }
    // More useful event output could be added here
    const QEvent::Type type = e->type();
    switch (type) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseMove:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::NonClientAreaMouseButtonPress:
    case QEvent::NonClientAreaMouseMove:
    case QEvent::NonClientAreaMouseButtonRelease:
    case QEvent::NonClientAreaMouseButtonDblClick:
    {
        const QMouseEvent *me = static_cast<const QMouseEvent*>(e);
        const Qt::MouseButton button = me->button();
        const Qt::MouseButtons buttons = me->buttons();
        dbg << "QMouseEvent(" << eventTypeName(type);
        if (type != QEvent::MouseMove && type != QEvent::NonClientAreaMouseMove)
            dbg << ", " << mouseButtonToString(button);
        if (buttons && button != buttons)
            dbg << ", buttons=" << mouseButtonsToString(buttons).constData();
        if (const int mods = int(me->modifiers()))
            dbg << ", modifiers=0x" << hex << mods << dec;
        dbg << ')';
    }
        break;
#  ifndef QT_NO_WHEELEVENT
    case QEvent::Wheel: {
        const QWheelEvent *we = static_cast<const QWheelEvent *>(e);
        dbg << "QWheelEvent(" << "delta=" << we->delta() << ", pos=" << we->pos()
            << ", orientation=" << we->orientation() << ')';
    }
        break;
#  endif // !QT_NO_WHEELEVENT
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::ShortcutOverride:
    {
        const QKeyEvent *ke = static_cast<const QKeyEvent *>(e);
        dbg << "QKeyEvent("  << eventTypeName(type)
            << ", key=0x" << hex << ke->key() << dec;
        if (const int mods = ke->modifiers())
            dbg << ", modifiers=0x" << hex << mods << dec;
        if (!ke->text().isEmpty())
            dbg << ", text=" << ke->text();
        if (ke->isAutoRepeat())
            dbg << ", autorepeat, count=" << ke->count();
        dbg << ')';
    }
        break;
    case QEvent::Shortcut: {
        const QShortcutEvent *se = static_cast<const QShortcutEvent *>(e);
        dbg << "QShortcutEvent(" << se->key().toString() << ", id=" << se->shortcutId();
        if (se->isAmbiguous())
            dbg << ", ambiguous";
        dbg << ')';
    }
        break;
    case QEvent::FocusIn:
    case QEvent::FocusOut:
        dbg << "QFocusEvent(" << eventTypeName(type) << ", "
            << static_cast<const QFocusEvent *>(e)->reason() << ')';
        break;
    case QEvent::Move: {
        const QMoveEvent *me = static_cast<const QMoveEvent *>(e);
        dbg << "QMoveEvent(" << me->pos();
        if (!me->spontaneous())
            dbg << ", non-spontaneous";
        dbg << ')';
    }
         break;
    case QEvent::Resize: {
        const QResizeEvent *re = static_cast<const QResizeEvent *>(e);
        dbg << "QResizeEvent(" << re->size();
        if (!re->spontaneous())
            dbg << ", non-spontaneous";
        dbg << ')';
    }
        break;
#  ifndef QT_NO_DRAGANDDROP
    case QEvent::DragEnter:
    case QEvent::DragMove:
    case QEvent::Drop:
        formatDropEvent(dbg, static_cast<const QDropEvent *>(e));
        break;
#  endif // !QT_NO_DRAGANDDROP
    case QEvent::ChildAdded:
    case QEvent::ChildPolished:
    case QEvent::ChildRemoved:
        dbg << "QChildEvent(" << eventTypeName(type) << ", " << (static_cast<const QChildEvent*>(e))->child() << ')';
        break;
    case QEvent::ContextMenu:
        dbg << "QContextMenuEvent(" << static_cast<const QContextMenuEvent *>(e)->pos() << ')';
        break;
    case QEvent::Timer:
        dbg << "QTimerEvent(id=" << static_cast<const QTimerEvent *>(e)->timerId() << ')';
        break;
    default:
        dbg << eventClassName(type) << '(' << eventTypeName(type) << ", "
            << (const void *)e << ", type = " << e->type() << ')';
        break;
    }
    dbg.maybeSpace();
    return dbg;
#else // !Q_BROKEN_DEBUG_STREAM
    qWarning("This compiler doesn't support streaming QEvent to QDebug");
    return dbg;
    Q_UNUSED(e);
#endif
}
#endif

#ifndef QT_NO_CLIPBOARD
/*!
    \class QClipboardEvent
    \ingroup events
    \internal

    \brief The QClipboardEvent class provides the parameters used in a clipboard event.

    This class is for internal use only, and exists to aid the clipboard on various
    platforms to get all the information it needs. Use QEvent::Clipboard instead.

    \sa QClipboard
*/

QClipboardEvent::QClipboardEvent(XEvent *event)
    : QEvent(QEvent::Clipboard), m_event(event)
{
}

QClipboardEvent::~QClipboardEvent()
{
}
#endif // QT_NO_CLIPBOARD

/*!
    \class QShortcutEvent
    \brief The QShortcutEvent class provides an event which is generated when
    the user presses a key combination.

    \ingroup events

    Normally you don't need to use this class directly; QShortcut
    provides a higher-level interface to handle shortcut keys.

    \sa QShortcut
*/

/*!
    \fn const QKeySequence &QShortcutEvent::key() const

    Returns the key sequence that triggered the event.
*/

/*!
    \fn int QShortcutEvent::shortcutId() const

    Returns the ID of the QShortcut object for which this event was
    generated.

    \sa QShortcut::id()
*/

/*!
    \fn bool QShortcutEvent::isAmbiguous() const

    Returns true if the key sequence that triggered the event is
    ambiguous.

    \sa QShortcut::activatedAmbiguously()
*/

/*!
    \class QWindowStateChangeEvent
    \ingroup events

    \brief The QWindowStateChangeEvent class provides the window state before a
    window state change.
*/

/*! \fn Qt::WindowStates QWindowStateChangeEvent::oldState() const

    Returns the state of the window before the change.
*/

/*! \internal
 */
QWindowStateChangeEvent::QWindowStateChangeEvent(Qt::WindowStates s)
    : QEvent(WindowStateChange), ostate(s)
{
}

/*! \internal
 */
QWindowStateChangeEvent::QWindowStateChangeEvent(Qt::WindowStates s, bool isOverride)
    : QEvent(WindowStateChange), ostate(s)
{
    m_override = isOverride;
}

/*! \internal
*/
QWindowStateChangeEvent::~QWindowStateChangeEvent()
{
}

QT_END_NAMESPACE
