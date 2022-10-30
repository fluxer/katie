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

#include "qdatastream.h"

#if !defined(QT_NO_DATASTREAM)
#include "qbuffer.h"
#include "qstring.h"
#include "qendian.h"
#include "qplatformdefs.h"
#include "qcorecommon_p.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

QT_BEGIN_NAMESPACE

/*!
    \class QDataStream
    \reentrant
    \brief The QDataStream class provides serialization of binary data
    to a QIODevice.

    \ingroup io


    A data stream is a binary stream of encoded information which is
    100% independent of the host computer's operating system, CPU or
    byte order. For example, a data stream that is written by a PC
    under Linux can be read by system running Solaris.

    You can also use a data stream to read/write \l{raw}{raw
    unencoded binary data}. If you want a "parsing" input stream, see
    QTextStream.

    The QDataStream class implements the serialization of C++'s basic
    data types, like \c char, \c short, \c int, \c{char *}, etc.
    Serialization of more complex data is accomplished by breaking up
    the data into primitive units.

    A data stream cooperates closely with a QIODevice. A QIODevice
    represents an input/output medium one can read data from and write
    data to. The QFile class is an example of an I/O device.

    Example (write binary data to a stream):

    \snippet doc/src/snippets/code/src_corelib_io_qdatastream.cpp 0

    Example (read binary data from a stream):

    \snippet doc/src/snippets/code/src_corelib_io_qdatastream.cpp 1

    Each item written to the stream is written in a predefined binary
    format that varies depending on the item's type. Supported Qt
    types include QBrush, QColor, QDateTime, QFont, QPixmap, QString,
    QVariant and many others. For the complete list of all Qt types
    supporting data streaming see \l{Serializing Qt Data Types}.

    For integers it is best to always cast to a Qt integer type for
    writing, and to read back into the same Qt integer type. This
    ensures that you get integers of the size you want and insulates
    you from compiler and platform differences.

    To take one example, a \c{char *} string is written as a 32-bit
    integer equal to the length of the string including the '\\0' byte,
    followed by all the characters of the string including the
    '\\0' byte. When reading a \c{char *} string, 4 bytes are read to
    create the 32-bit length value, then that many characters for the
    \c {char *} string including the '\\0' terminator are read.

    The initial I/O device is usually set in the constructor, but can be
    changed with setDevice(). If you've reached the end of the data
    (or if there is no I/O device set) atEnd() will return true.

    \section1 Compatibility

    QDataStream's binary format has evolved since Qt 1.0, and is
    likely to continue evolving to reflect changes done in Katie.
    If you need both forward and backward compatibility, you would
    write a brief header containing a magic string and a version
    number to give yourself room for future expansion.

    You can select which byte order to use when serializing data. The
    default setting depends on the host endianness. We recommend
    keeping this setting unless you have special requirements.

    \target raw
    \section1 Reading and writing raw binary data

    You may wish to read/write your own raw binary data to/from the
    data stream directly. Data may be read from the stream into a
    preallocated \c{char *} using readRawData(). Similarly data can be
    written to the stream using writeRawData(). Note that any
    encoding/decoding of the data must be done by you.

    A similar pair of functions is readBytes() and writeBytes(). These
    differ from their \e raw counterparts as follows: readBytes()
    reads a quint32 which is taken to be the length of the data to be
    read, then that number of bytes is read into the preallocated
    \c{char *}; writeBytes() writes a quint32 containing the length of the
    data, followed by the data. Note that any encoding/decoding of
    the data (apart from the length quint32) must be done by you.

    \section1 Reading and writing Qt collection classes

    The Qt container classes can also be serialized to a QDataStream.
    These include QList, QVector, QSet, QHash, and QMap.
    The stream operators are declared as non-members of the classes.

    \target Serializing Qt Classes
    \section1 Reading and writing other Qt classes.

    In addition to the overloaded stream operators documented here,
    any Qt classes that you might want to serialize to a QDataStream
    will have appropriate stream operators declared as non-member of
    the class:

    \code
    QDataStream &operator<<(QDataStream &, const QXxx &);
    QDataStream &operator>>(QDataStream &, QXxx &);
    \endcode

    For example, here are the stream operators declared as non-members
    of the QImage class:

    \code
    QDataStream & operator<< (QDataStream& stream, const QImage& image);
    QDataStream & operator>> (QDataStream& stream, QImage& image);
    \endcode

    To see if your favorite Qt class has similar stream operators
    defined, check the \bold {Related Non-Members} section of the
    class's documentation page.

    \sa QTextStream QVariant
*/

/*!
    \enum QDataStream::ByteOrder

    The byte order used for reading/writing the data.

    \value BigEndian Most significant byte first (the default)
    \value LittleEndian Least significant byte first
    \value HostEndian Either BigEndian or LittleEndian, depends on the host endianness
*/

/*!
  \enum QDataStream::FloatingPointPrecision

  The precision of floating point numbers used for reading/writing the data.

  \warning The floating point precision must be set to the same value on the object that writes
  and the object that reads the data stream.

  \value SinglePrecision All floating point numbers in the data stream have 32-bit precision.
  \value DoublePrecision All floating point numbers in the data stream have 64-bit precision.

  \sa setFloatingPointPrecision(), floatingPointPrecision()
*/

/*!
    \enum QDataStream::DataStatus

    This enum describes the current status of the data stream.

    \value Ok               The data stream is operating normally.
    \value ReadPastEnd      The data stream has read past the end of the
                            data in the underlying device.
    \value ReadCorruptData  The data stream has read corrupt data.
    \value WriteFailed      The data stream cannot write to the underlying device.
*/

/*****************************************************************************
  QDataStream member functions
 *****************************************************************************/

#ifndef QT_NO_DEBUG
#define CHECK_STREAM_PRECOND(retVal) \
    if (!dev) { \
        qWarning("QDataStream: No device"); \
        return retVal; \
    }
#else
#define CHECK_STREAM_PRECOND(retVal) \
    if (!dev) { \
        return retVal; \
    }
#endif

#define CHECK_STREAM_WRITE_PRECOND(retVal) \
    CHECK_STREAM_PRECOND(retVal) \
    if (q_status != Ok) \
        return retVal;

// ### 5.0: when streaming invalid QVariants, just the type should
// be written, no "data" after it

/*!
    Constructs a data stream that has no I/O device.

    \sa setDevice()
*/

QDataStream::QDataStream()
    : dev(nullptr),
    owndev(false),
    byteorder(QDataStream::HostEndian),
    q_status(QDataStream::Ok),
    floatingPrecision(QDataStream::DoublePrecision)
{
}

/*!
    Constructs a data stream that uses the I/O device \a d.

    \warning If you use QSocket or QSocketDevice as the I/O device \a d
    for reading data, you must make sure that enough data is available
    on the socket for the operation to successfully proceed;
    QDataStream does not have any means to handle or recover from
    short-reads.

    \sa setDevice(), device()
*/

QDataStream::QDataStream(QIODevice *device)
    : dev(device),
    owndev(false),
    byteorder(QDataStream::HostEndian),
    q_status(QDataStream::Ok),
    floatingPrecision(QDataStream::DoublePrecision)
{
}


/*!
    \fn QDataStream::QDataStream(QByteArray *a, QIODevice::OpenMode mode)

    Constructs a data stream that operates on a byte array, \a a. The
    \a mode describes how the device is to be used.

    Alternatively, you can use QDataStream(const QByteArray &) if you
    just want to read from a byte array.

    Since QByteArray is not a QIODevice subclass, internally a QBuffer
    is created to wrap the byte array.
*/

QDataStream::QDataStream(QByteArray *a, QIODevice::OpenMode flags)
    : dev(nullptr),
    owndev(true),
    byteorder(QDataStream::HostEndian),
    q_status(QDataStream::Ok),
    floatingPrecision(QDataStream::DoublePrecision)
{
    QBuffer *buf = new QBuffer(a);
#ifndef QT_NO_QOBJECT
    buf->blockSignals(true);
#endif
    buf->open(flags);
    dev = buf;
}

/*!
    Constructs a read-only data stream that operates on byte array \a a.
    Use QDataStream(QByteArray*, int) if you want to write to a byte
    array.

    Since QByteArray is not a QIODevice subclass, internally a QBuffer
    is created to wrap the byte array.
*/
QDataStream::QDataStream(const QByteArray &a)
    : dev(nullptr),
    owndev(true),
    byteorder(QDataStream::HostEndian),
    q_status(QDataStream::Ok),
    floatingPrecision(QDataStream::DoublePrecision)
{
    QBuffer *buf = new QBuffer;
#ifndef QT_NO_QOBJECT
    buf->blockSignals(true);
#endif
    buf->setData(a);
    buf->open(QIODevice::ReadOnly);
    dev = buf;
}

/*!
    Destroys the data stream.

    The destructor will not affect the current I/O device, unless it is
    an internal I/O device (e.g. a QBuffer) processing a QByteArray
    passed in the \e constructor, in which case the internal I/O device
    is destroyed.
*/

QDataStream::~QDataStream()
{
    if (owndev)
        delete dev;
}


/*!
    \fn QIODevice *QDataStream::device() const

    Returns the I/O device currently set, or 0 if no
    device is currently set.

    \sa setDevice()
*/

/*!
    void QDataStream::setDevice(QIODevice *device)

    Sets the I/O device to \a device, which can be 0
    to unset to current I/O device.

    \sa device()
*/

void QDataStream::setDevice(QIODevice *device)
{
    if (owndev) {
        delete dev;
        owndev = false;
    }
    dev = device;
}

/*!
    \fn bool QDataStream::atEnd() const

    Returns true if the I/O device has reached the end position (end of
    the stream or file) or if there is no I/O device set; otherwise
    returns false.

    \sa QIODevice::atEnd()
*/

bool QDataStream::atEnd() const
{
    return dev ? dev->atEnd() : true;
}

/*!
    Returns the floating point precision of the data stream.

    \since 4.6

    \sa FloatingPointPrecision setFloatingPointPrecision()
*/
QDataStream::FloatingPointPrecision QDataStream::floatingPointPrecision() const
{
    return floatingPrecision;
}

/*!
    Sets the floating point precision of the data stream to \a precision. If the floating point
    precision is DoublePrecision, all floating point numbers will be written and read with 64-bit
    precision. If the floating point precision is SinglePrecision, all floating point numbers will
    be written and read with 32-bit precision.

    The default is DoublePrecision.

    \warning This property must be set to the same value on the object that writes and the object
    that reads the data stream.

    \since 4.6
*/
void QDataStream::setFloatingPointPrecision(QDataStream::FloatingPointPrecision precision)
{
    floatingPrecision = precision;
}

/*!
    Returns the status of the data stream.

    \sa Status setStatus() resetStatus()
*/

QDataStream::DataStatus QDataStream::status() const
{
    return q_status;
}

/*!
    Resets the status of the data stream.

    \sa Status status() setStatus()
*/
void QDataStream::resetStatus()
{
    q_status = Ok;
}

/*!
    Sets the status of the data stream to the \a status given.

    Subsequent calls to setStatus() are ignored until resetStatus()
    is called.

    \sa Status status() resetStatus()
*/
void QDataStream::setStatus(DataStatus status)
{
    if (q_status == Ok)
        q_status = status;
}

/*!
    \fn int QDataStream::byteOrder() const

    Returns the current byte order setting -- either BigEndian or
    LittleEndian.

    \sa setByteOrder()
*/

/*!
    Sets the serialization byte order to \a bo.

    The \a bo parameter can be QDataStream::BigEndian,
    QDataStream::LittleEndian or QDataStream::HostEndian.

    The default setting depends on the host endianness. We recommend
    leaving this setting unless you have special requirements.

    \sa byteOrder()
*/

void QDataStream::setByteOrder(ByteOrder bo)
{
    byteorder = bo;
}

/*****************************************************************************
  QDataStream read functions
 *****************************************************************************/

/*!
    \fn QDataStream &QDataStream::operator>>(quint8 &i)
    \overload

    Reads an unsigned byte from the stream into \a i, and returns a
    reference to the stream.
*/

/*!
    Reads a signed byte from the stream into \a i, and returns a
    reference to the stream.
*/

QDataStream &QDataStream::operator>>(qint8 &i)
{
    i = 0;
    CHECK_STREAM_PRECOND(*this)
    char c;
    if (!dev->getChar(&c))
        setStatus(ReadPastEnd);
    else
        i = qint8(c);
    return *this;
}


/*!
    \fn QDataStream &QDataStream::operator>>(quint16 &i)
    \overload

    Reads an unsigned 16-bit integer from the stream into \a i, and
    returns a reference to the stream.
*/

/*!
    \overload

    Reads a signed 16-bit integer from the stream into \a i, and
    returns a reference to the stream.
*/

QDataStream &QDataStream::operator>>(qint16 &i)
{
    i = 0;
    CHECK_STREAM_PRECOND(*this)
    if (dev->read((char *)&i, sizeof(qint16)) != sizeof(qint16)) {
        i = 0;
        setStatus(ReadPastEnd);
    } else if (byteorder != QDataStream::HostEndian) {
        i = qbswap(i);
    }
    return *this;
}


/*!
    \fn QDataStream &QDataStream::operator>>(quint32 &i)
    \overload

    Reads an unsigned 32-bit integer from the stream into \a i, and
    returns a reference to the stream.
*/

/*!
    \overload

    Reads a signed 32-bit integer from the stream into \a i, and
    returns a reference to the stream.
*/

QDataStream &QDataStream::operator>>(qint32 &i)
{
    i = 0;
    CHECK_STREAM_PRECOND(*this)
    if (dev->read((char *)&i, sizeof(qint32)) != sizeof(qint32)) {
        i = 0;
        setStatus(ReadPastEnd);
    } else if (byteorder != QDataStream::HostEndian) {
        i = qbswap(i);
    }
    return *this;
}

/*!
    \fn QDataStream &QDataStream::operator>>(quint64 &i)
    \overload

    Reads an unsigned 64-bit integer from the stream, into \a i, and
    returns a reference to the stream.
*/

/*!
    \overload

    Reads a signed 64-bit integer from the stream into \a i, and
    returns a reference to the stream.
*/

QDataStream &QDataStream::operator>>(qint64 &i)
{
    i = qint64(0);
    CHECK_STREAM_PRECOND(*this)
    if (dev->read((char *)&i, sizeof(qint64)) != sizeof(qint64)) {
        i = qint64(0);
        setStatus(ReadPastEnd);
    } else if (byteorder != QDataStream::HostEndian) {
        i = qbswap(i);
    }
    return *this;
}

/*!
    Reads a boolean value from the stream into \a i. Returns a
    reference to the stream.
*/
QDataStream &QDataStream::operator>>(bool &i)
{
    qint8 v;
    *this >> v;
    i = !!v;
    return *this;
}

/*!
    \overload

    Reads a floating point number from the stream into \a f,
    using the standard IEEE 754 format. Returns a reference to the
    stream.

    \sa setFloatingPointPrecision()
*/

QDataStream &QDataStream::operator>>(float &f)
{
    if (floatingPointPrecision() == QDataStream::DoublePrecision) {
        double dbl;
        *this >> dbl;
        f = dbl;
        return *this;
    }

    f = 0.0f;
    CHECK_STREAM_PRECOND(*this)
    if (dev->read((char *)&f, sizeof(float)) != sizeof(float)) {
        f = 0.0f;
        setStatus(ReadPastEnd);
    } else if (byteorder != QDataStream::HostEndian) {
        union {
            float val1;
            quint32 val2;
        } x;
        x.val2 = qbswap(*reinterpret_cast<quint32 *>(&f));
        f = x.val1;
    }
    return *this;
}

/*!
    \overload

    Reads a floating point number from the stream into \a f,
    using the standard IEEE 754 format. Returns a reference to the
    stream.

    \sa setFloatingPointPrecision()
*/

QDataStream &QDataStream::operator>>(double &f)
{
    if (floatingPointPrecision() == QDataStream::SinglePrecision) {
        float flt;
        *this >> flt;
        f = flt;
        return *this;
    }

    f = 0.0;
    CHECK_STREAM_PRECOND(*this)
    if (dev->read((char *)&f, sizeof(double)) != sizeof(double)) {
        f = 0.0;
        setStatus(ReadPastEnd);
    } else if (byteorder != QDataStream::HostEndian) {
        union {
            double val1;
            quint64 val2;
        } x;
        x.val2 = qbswap(*reinterpret_cast<quint64 *>(&f));
        f = x.val1;
    }
    return *this;
}


/*!
    \overload

    Reads the '\0'-terminated string \a s from the stream and returns
    a reference to the stream.

    Space for the string is allocated using \c new -- the caller must
    destroy it with \c{delete[]}.
*/

QDataStream &QDataStream::operator>>(char *&s)
{
    uint len = 0;
    return readBytes(s, len);
}


/*!
    Reads the buffer \a s from the stream and returns a reference to
    the stream.

    The buffer \a s is allocated using \c new. Destroy it with the \c
    delete[] operator.

    The \a l parameter is set to the length of the buffer. If the
    string read is empty, \a l is set to 0 and \a s is set to
    a null pointer.

    The serialization format is a quint32 length specifier first,
    then \a l bytes of data.

    \sa readRawData(), writeBytes()
*/

QDataStream &QDataStream::readBytes(char *&s, uint &l)
{
    s = 0;
    l = 0;
    CHECK_STREAM_PRECOND(*this)

    quint32 len;
    *this >> len;
    if (len == 0)
        return *this;

    char *sbuf = new char[len + 1];
    if (dev->read(sbuf, len) != len) {
        delete [] sbuf;
        setStatus(ReadPastEnd);
        return *this;
    }

    s = sbuf;
    s[len] = '\0';
    l = (uint)len;
    return *this;
}

/*!
    Reads at most \a len bytes from the stream into \a s and returns the number of
    bytes read. If an error occurs, this function returns -1.

    The buffer \a s must be preallocated. The data is \e not encoded.

    \sa readBytes(), QIODevice::read(), writeRawData()
*/

int QDataStream::readRawData(char *s, int len)
{
    CHECK_STREAM_PRECOND(-1)
    return dev->read(s, len);
}


/*****************************************************************************
  QDataStream write functions
 *****************************************************************************/


/*!
    \fn QDataStream &QDataStream::operator<<(quint8 i)
    \overload

    Writes an unsigned byte, \a i, to the stream and returns a
    reference to the stream.
*/

/*!
    Writes a signed byte, \a i, to the stream and returns a reference
    to the stream.
*/

QDataStream &QDataStream::operator<<(qint8 i)
{
    Q_ASSERT(sizeof(char) == sizeof(qint8));
    CHECK_STREAM_WRITE_PRECOND(*this)
    if (!dev->putChar(i))
        q_status = WriteFailed;
    return *this;
}


/*!
    \fn QDataStream &QDataStream::operator<<(quint16 i)
    \overload

    Writes an unsigned 16-bit integer, \a i, to the stream and returns
    a reference to the stream.
*/

/*!
    \overload

    Writes a signed 16-bit integer, \a i, to the stream and returns a
    reference to the stream.
*/

QDataStream &QDataStream::operator<<(qint16 i)
{
    CHECK_STREAM_WRITE_PRECOND(*this)
    if (byteorder != QDataStream::HostEndian) {
        i = qbswap(i);
    }
    if (dev->write((char *)&i, sizeof(qint16)) != sizeof(qint16))
        q_status = WriteFailed;
    return *this;
}

/*!
    \overload

    Writes a signed 32-bit integer, \a i, to the stream and returns a
    reference to the stream.
*/

QDataStream &QDataStream::operator<<(qint32 i)
{
    CHECK_STREAM_WRITE_PRECOND(*this)
    if (byteorder != QDataStream::HostEndian) {
        i = qbswap(i);
    }
    if (dev->write((char *)&i, sizeof(qint32)) != sizeof(qint32))
        q_status = WriteFailed;
    return *this;
}

/*!
    \fn QDataStream &QDataStream::operator<<(quint64 i)
    \overload

    Writes an unsigned 64-bit integer, \a i, to the stream and returns a
    reference to the stream.
*/

/*!
    \overload

    Writes a signed 64-bit integer, \a i, to the stream and returns a
    reference to the stream.
*/

QDataStream &QDataStream::operator<<(qint64 i)
{
    CHECK_STREAM_WRITE_PRECOND(*this)
    if (byteorder != QDataStream::HostEndian) {
        i = qbswap(i);
    }
    if (dev->write((char *)&i, sizeof(qint64)) != sizeof(qint64))
        q_status = WriteFailed;
    return *this;
}

/*!
    \fn QDataStream &QDataStream::operator<<(quint32 i)
    \overload

    Writes an unsigned integer, \a i, to the stream as a 32-bit
    unsigned integer (quint32). Returns a reference to the stream.
*/

/*!
    Writes a boolean value, \a i, to the stream. Returns a reference
    to the stream.
*/

QDataStream &QDataStream::operator<<(bool i)
{
    CHECK_STREAM_WRITE_PRECOND(*this)
    if (!dev->putChar(qint8(i)))
        q_status = WriteFailed;
    return *this;
}

/*!
    \overload

    Writes a floating point number, \a f, to the stream using
    the standard IEEE 754 format. Returns a reference to the stream.

    \sa setFloatingPointPrecision()
*/

QDataStream &QDataStream::operator<<(float f)
{
    if (floatingPointPrecision() == QDataStream::DoublePrecision) {
        *this << double(f);
        return *this;
    }

    CHECK_STREAM_WRITE_PRECOND(*this)
    float g = f;                                // fixes float-on-stack problem
    if (byteorder != QDataStream::HostEndian) {
        union {
            float val1;
            quint32 val2;
        } x;
        x.val1 = g;
        x.val2 = qbswap(x.val2);

        if (dev->write((char *)&x.val2, sizeof(float)) != sizeof(float))
            q_status = WriteFailed;
        return *this;
    }

    if (dev->write((char *)&g, sizeof(float)) != sizeof(float))
        q_status = WriteFailed;
    return *this;
}


/*!
    \overload

    Writes a floating point number, \a f, to the stream using
    the standard IEEE 754 format. Returns a reference to the stream.

    \sa setFloatingPointPrecision()
*/

QDataStream &QDataStream::operator<<(double f)
{
    if (floatingPointPrecision() == QDataStream::SinglePrecision) {
        *this << float(f);
        return *this;
    }

    CHECK_STREAM_WRITE_PRECOND(*this)
    if (byteorder == QDataStream::HostEndian) {
        if (dev->write((char *)&f, sizeof(double)) != sizeof(double))
            q_status = WriteFailed;
    } else {
        union {
            double val1;
            quint64 val2;
        } x;
        x.val1 = f;
        x.val2 = qbswap(x.val2);
        if (dev->write((char *)&x.val2, sizeof(double)) != sizeof(double))
            q_status = WriteFailed;
    }
    return *this;
}


/*!
    \overload

    Writes the '\0'-terminated string \a s to the stream and returns a
    reference to the stream.

    The string is serialized using writeBytes().
*/

QDataStream &QDataStream::operator<<(const char *s)
{
    if (!s) {
        *this << (quint32)0;
        return *this;
    }
    uint len = qstrlen(s) + 1;                    // also write null terminator
    *this << (quint32)len;                        // write length specifier
    writeRawData(s, len);
    return *this;
}


/*!
    Writes the length specifier \a len and the buffer \a s to the
    stream and returns a reference to the stream.

    The \a len is serialized as a quint32, followed by \a len bytes
    from \a s. Note that the data is \e not encoded.

    \sa writeRawData(), readBytes()
*/

QDataStream &QDataStream::writeBytes(const char *s, uint len)
{
    CHECK_STREAM_WRITE_PRECOND(*this)
    *this << (quint32)len;                        // write length specifier
    if (len)
        writeRawData(s, len);
    return *this;
}


/*!
    Writes \a len bytes from \a s to the stream. Returns the
    number of bytes actually written, or -1 on error.
    The data is \e not encoded.

    \sa writeBytes(), QIODevice::write(), readRawData()
*/

int QDataStream::writeRawData(const char *s, int len)
{
    CHECK_STREAM_WRITE_PRECOND(-1)
    int ret = dev->write(s, len);
    if (ret != len)
        q_status = WriteFailed;
    return ret;
}

/*!
    \since 4.1

    Skips \a len bytes from the device. Returns the number of bytes
    actually skipped, or -1 on error.

    This is equivalent to calling readRawData() on a buffer of length
    \a len and ignoring the buffer.

    \sa QIODevice::seek()
*/
int QDataStream::skipRawData(int len)
{
    CHECK_STREAM_PRECOND(-1)

    if (dev->isSequential()) {
        QSTACKARRAY(char, buf, QT_BUFFSIZE);
        int sumRead = 0;

        while (len > 0) {
            int blockSize = qMin(len, (int)sizeof(buf));
            int n = dev->read(buf, blockSize);
            if (n == -1)
                return -1;
            if (n == 0)
                return sumRead;

            sumRead += n;
            len -= blockSize;
        }
        return sumRead;
    } else {
        qint64 pos = dev->pos();
        qint64 size = dev->size();
        if (pos + len > size)
            len = size - pos;
        if (!dev->seek(pos + len))
            return -1;
        return len;
    }
}


QT_END_NAMESPACE

#endif // QT_NO_DATASTREAM
