/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#include "qdebug.h"
#include "qiodevice.h"
#include "qbuffer.h"

QT_BEGIN_NAMESPACE

class QDebugPrivate
{
public:
    QDebugPrivate(QIODevice *device);
    QDebugPrivate(QtMsgType t);

    QAtomicInt ref;
    QIODevice *dev;
    QByteArray buffer;
    QtMsgType type;
    bool space;
};

QDebugPrivate::QDebugPrivate(QIODevice *device)
    : ref(1),
    dev(device),
    type(QtDebugMsg),
    space(true)
{
}

QDebugPrivate::QDebugPrivate(QtMsgType t)
    : ref(1),
    dev(nullptr),
    type(t),
    space(true)
{
}


/*!
    \class QDebug

    \brief The QDebug class provides an output stream for debugging information.

    QDebug is used whenever the developer needs to write out debugging or tracing
    information to a device, file, string or console.

    \section1 Basic Use

    In the common case, it is useful to call the qDebug() function to obtain a
    default QDebug object to use for writing debugging information.

    \snippet doc/src/snippets/qdebug/qdebugsnippet.cpp 1

    This constructs a QDebug object using the constructor that accepts a QtMsgType
    value of QtDebugMsg. Similarly, the qWarning(), qCritical() and qFatal()
    functions also return QDebug objects for the corresponding message types.

    The class also provides several constructors for other situations, including
    a constructor that accepts a QFile or any other QIODevice subclass that is
    used to write debugging information to files and other devices. The constructor
    that accepts a QString is used to write to a string for display or serialization.

    \section1 Writing Custom Types to a Stream

    Many standard types can be written to QDebug objects, and Qt provides support for
    most Qt value types. To add support for custom types, you need to implement a
    streaming operator, as in the following example:

    \snippet doc/src/snippets/qdebug/qdebugsnippet.cpp 0

    This is described in the \l{Debugging Techniques} and
    \l{Creating Custom Qt Types#Making the Type Printable}{Creating Custom Qt Types}
    documents.
*/

/*!
    Constructs a debug stream that writes to the given \a device.
*/
QDebug::QDebug(QIODevice *device)
    : d_ptr(new QDebugPrivate(device))
{
}

/*!
    Constructs a debug stream that writes to the handler for the message type specified by \a type.
*/
QDebug::QDebug(QtMsgType type)
    : d_ptr(new QDebugPrivate(type))
{
}

/*!
    Constructs a copy of the \a other debug stream.
*/
QDebug::QDebug(const QDebug &other)
    : d_ptr(other.d_ptr)
{
    d_ptr->ref.ref();
}

/*!
    Assigns the \a other debug stream to this stream and returns a reference to
    this stream.
*/
QDebug &QDebug::operator=(const QDebug &other)
{
    if (d_ptr != other.d_ptr) {
        QDebug copy(other);
        qSwap(d_ptr, copy.d_ptr);
    }
    return *this;
}

/*!
    Flushes any pending data to be written and destroys the debug stream.
*/
QDebug::~QDebug()
{
    if (!d_ptr->ref.deref()) {
        if (!d_ptr->dev) {
            qt_message_output(d_ptr->type, d_ptr->buffer.constData());
        } else {
            d_ptr->dev->write(d_ptr->buffer.constData(), d_ptr->buffer.size());
        }
        delete d_ptr;
    }
}

/*!
    Writes a space character to the debug stream and returns a reference to
    the stream.

    The stream will record that the last character sent to the stream was a
    space.

    \sa nospace(), maybeSpace()
*/
QDebug &QDebug::space()
{
    d_ptr->space = true;
    d_ptr->buffer.append(" ", 1);
    return *this;
}

/*!
    Clears the stream's internal flag that records whether the last character
    was a space and returns a reference to the stream.

    \sa space(), maybeSpace()
*/
QDebug &QDebug::nospace()
{
    d_ptr->space = false;
    return *this;
}

/*!
    Writes a space character to the debug stream, depending on the last
    character sent to the stream, and returns a reference to the stream.

    If the last character was a space character, this function writes a space
    character to the stream; otherwise, no characters are written to the stream.

    \sa space(), nospace()
*/
QDebug &QDebug::maybeSpace()
{
    if (d_ptr->space) {
        d_ptr->buffer.append(" ", 1);
    }
    return *this;
}

/*!
    Writes the character, \a t, to the stream and returns a reference to the
    stream.
*/
QDebug &QDebug::operator<<(QChar t)
{
    const char tc = t.toLatin1();
    d_ptr->buffer.append("\'", 1);
    d_ptr->buffer.append(&tc, 1);
    d_ptr->buffer.append("\'", 1);
    return maybeSpace();
}

/*!
    Writes the boolean value, \a t, to the stream and returns a reference to the
    stream.
*/
QDebug &QDebug::operator<<(bool t)
{
    if (t) {
        d_ptr->buffer.append("true", 4);
    } else {
        d_ptr->buffer.append("false", 5);
    }
    return maybeSpace();
}

/*!
    Writes the character, \a t, to the stream and returns a reference to the
    stream.
*/
QDebug &QDebug::operator<<(char t)
{
    d_ptr->buffer.append(&t, 1);
    return maybeSpace();
}

/*!
    Writes the signed short integer, \a t, to the stream and returns a reference
    to the stream.
*/
QDebug &QDebug::operator<<(qint8 t)
{
    return QDebug::operator<<(qint64(t));
}

/*!
    Writes then unsigned short integer, \a t, to the stream and returns a
    reference to the stream.
*/
QDebug &QDebug::operator<<(quint8 t)
{
    return QDebug::operator<<(quint64(t));
}

/*!
    Writes the signed integer, \a t, to the stream and returns a reference
    to the stream.
*/
QDebug &QDebug::operator<<(qint16 t)
{
    return QDebug::operator<<(qint64(t));
}

/*!
    Writes then unsigned integer, \a t, to the stream and returns a reference to
    the stream.
*/
QDebug &QDebug::operator<<(quint16 t)
{
    return QDebug::operator<<(quint64(t));
}

/*!
    Writes the signed integer, \a t, to the stream and returns a reference
    to the stream.
*/
QDebug &QDebug::operator<<(qint32 t)
{
    return QDebug::operator<<(qint64(t));
}

/*!
    Writes then unsigned integer, \a t, to the stream and returns a reference to
    the stream.
*/
QDebug &QDebug::operator<<(quint32 t)
{
    return QDebug::operator<<(quint64(t));
}

/*!
    Writes the signed long integer, \a t, to the stream and returns a reference
    to the stream.
*/
QDebug &QDebug::operator<<(long int t)
{
    return QDebug::operator<<(qint64(t));
}

/*!
    Writes then unsigned long integer, \a t, to the stream and returns a reference
    to the stream.
*/
QDebug &QDebug::operator<<(unsigned long int t)
{
    return QDebug::operator<<(quint64(t));
}

/*!
    Writes the signed 64-bit integer, \a t, to the stream and returns a reference
    to the stream.
*/
QDebug &QDebug::operator<<(qint64 t)
{
    d_ptr->buffer.append(QByteArray::number(t));
    return maybeSpace();
}

/*!
    Writes then unsigned 64-bit integer, \a t, to the stream and returns a
    reference to the stream.
*/
QDebug &QDebug::operator<<(quint64 t)
{
    d_ptr->buffer.append(QByteArray::number(t));
    return maybeSpace();
}

/*!
    Writes the 32-bit floating point number, \a t, to the stream and returns a
    reference to the stream.
*/
QDebug &QDebug::operator<<(float t)
{
    d_ptr->buffer.append(QByteArray::number(t));
    return maybeSpace();
}

/*!
    Writes the 64-bit floating point number, \a t, to the stream and returns a
    reference to the stream.
*/
QDebug &QDebug::operator<<(double t)
{
    d_ptr->buffer.append(QByteArray::number(t));
    return maybeSpace();
}

/*!
    Writes the '\0'-terminated string, \a t, to the stream and returns a
    reference to the stream.
*/
QDebug &QDebug::operator<<(const char* t)
{
    d_ptr->buffer.append(t);
    return maybeSpace();
}

/*!
    Writes the string, \a t, to the stream and returns a reference to the stream.
*/
QDebug &QDebug::operator<<(const QString &t)
{
    return QDebug::operator<<(t.toLocal8Bit());
}

/*!
    Writes the string reference, \a t, to the stream and returns a reference to
    the stream.
*/
QDebug &QDebug::operator<<(const QStringRef &t)
{
    return QDebug::operator<<(t.toString());
}

/*!
    Writes the Latin1-encoded string, \a s, to the stream and returns a reference
    to the stream.
*/
QDebug &QDebug::operator<<(const QLatin1String &t)
{
    return QDebug::operator<<(t.latin1());
}

/*!
    Writes the byte array, \a t, to the stream and returns a reference to the
    stream.
*/
QDebug &QDebug::operator<<(const QByteArray &t)
{
    d_ptr->buffer.append('\"');
    d_ptr->buffer.append(t.constData(), t.size());
    d_ptr->buffer.append('\"');
    return maybeSpace();
}

/*!
    Writes a pointer, \a t, to the stream and returns a reference to the stream.
*/
QDebug &QDebug::operator<<(const void* t)
{
    d_ptr->buffer.append(QByteArray::number(qulonglong(quintptr(t))));
    return maybeSpace();
}

QT_END_NAMESPACE
