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

#ifndef QIODEVICE_P_H
#define QIODEVICE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of QIODevice. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/qiodevice.h"
#include "QtCore/qbytearray.h"
#include "QtCore/qobjectdefs.h"
#include "QtCore/qstring.h"
#include "QtCore/qplatformdefs.h"
#ifndef QT_NO_QOBJECT
#include "qobject_p.h"
#endif

QT_BEGIN_NAMESPACE

#define QIODEVICE_BUFFERSIZE qint64(QT_BUFFSIZE)

// This is QIODevice's read buffer, optimized for read(), isEmpty() and getChar()
class QIODevicePrivateLinearBuffer
{
public:
    QIODevicePrivateLinearBuffer(int) : len(0), first(0), buf(0), capacity(0) {
    }
    ~QIODevicePrivateLinearBuffer() {
        delete [] buf;
    }
    void clear() {
        first = buf;
        len = 0;
    }
    qint64 size() const {
        return len;
    }
    bool isEmpty() const {
        return len == 0;
    }
    void skip(const qint64 n) {
        if (n >= len) {
            clear();
        } else {
            len -= n;
            first += n;
        }
    }
    int getChar() {
        if (len == 0)
            return -1;
        const int ch = uchar(*first);
        len--;
        first++;
        return ch;
    }
    qint64 read(char* target, const qint64 size) {
        const qint64 r = qMin(size, len);
        memcpy(target, first, r);
        len -= r;
        first += r;
        return r;
    }
    qint64 peek(char* target, const qint64 size) {
        const qint64 r = qMin(size, len);
        memcpy(target, first, r);
        return r;
    }
    char* reserve(const qint64 size) {
        makeSpace(size + len, freeSpaceAtEnd);
        char* writePtr = first + len;
        len += size;
        return writePtr;
    }
    void chop(const qint64 size) {
        if (size >= len) {
            clear();
        } else {
            len -= size;
        }
    }
    QByteArray readAll() {
        char* f = first;
        const int l = len;
        clear();
        return QByteArray(f, l);
    }
    qint64 readLine(char* target, qint64 size) {
        qint64 r = qMin(size, len);
        char* eol = static_cast<char*>(memchr(first, '\n', r));
        if (eol)
            r = 1+(eol-first);
        memcpy(target, first, r);
        len -= r;
        first += r;
        return r;
    }
    bool canReadLine() const {
        return memchr(first, '\n', len);
    }
    void ungetChar(char c) {
        if (first == buf) {
            // underflow, the existing valid data needs to move to the end of the (potentially bigger) buffer
            makeSpace(len+1, freeSpaceAtStart);
        }
        first--;
        len++;
        *first = c;
    }
    void ungetBlock(const char* block, qint64 size) {
        if ((first - buf) < size) {
            // underflow, the existing valid data needs to move to the end of the (potentially bigger) buffer
            makeSpace(len + size, freeSpaceAtStart);
        }
        first -= size;
        len += size;
        memcpy(first, block, size);
    }

private:
    enum FreeSpacePos {freeSpaceAtStart, freeSpaceAtEnd};
    void makeSpace(qint64 required, FreeSpacePos where) {
        qint64 newCapacity = qMax(capacity, QIODEVICE_BUFFERSIZE);
        while (newCapacity < required)
            newCapacity *= 2;
        const int moveOffset = (where == freeSpaceAtEnd) ? 0 : newCapacity - len;
        if (newCapacity > capacity) {
            // allocate more space
            char* newBuf = new char[newCapacity];
            memmove(newBuf + moveOffset, first, len);
            delete [] buf;
            buf = newBuf;
            capacity = newCapacity;
        } else {
            // shift any existing data to make space
            memmove(buf + moveOffset, first, len);
        }
        first = buf + moveOffset;
    }

    // length of the unread data
    qint64 len;
    // start of the unread data
    char* first;
    // the allocated buffer
    char* buf;
    // allocated buffer size
    qint64 capacity;
};

class Q_CORE_EXPORT QIODevicePrivate
#ifndef QT_NO_QOBJECT
    : public QObjectPrivate
#endif
{
    Q_DECLARE_PUBLIC(QIODevice)

public:
    QIODevicePrivate();
    virtual ~QIODevicePrivate();

    QIODevice::OpenMode openMode;
    QString errorString;

    QIODevicePrivateLinearBuffer buffer;
    qint64 pos;
    qint64 devicePos;
    bool baseReadLineDataCalled;

    virtual qint64 peek(char *data, qint64 maxSize);
    virtual QByteArray peek(qint64 maxSize);

#ifdef QT_NO_QOBJECT
    QIODevice *q_ptr;
#endif
};

QT_END_NAMESPACE

#endif // QIODEVICE_P_H
