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

#ifndef QBITARRAY_H
#define QBITARRAY_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>

QT_BEGIN_NAMESPACE

class QDebug;
class QBitRef;

class Q_CORE_EXPORT QBitArray
{
    friend Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QBitArray &);
    friend Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QBitArray &);
    friend Q_CORE_EXPORT uint qHash(const QBitArray &key);
    QByteArray d;

public:
    inline QBitArray() {}
    explicit QBitArray(int size, bool val = false);
    QBitArray(const QBitArray &other) : d(other.d) {}
    inline QBitArray &operator=(const QBitArray &other) { d = other.d; return *this; }
    inline QBitArray &operator=(QBitArray &&other)
    { qSwap(d, other.d); return *this; }

    inline void swap(QBitArray &other) { qSwap(d, other.d); }

    inline int size() const { return (d.size() << 3) - *d.constData(); }
    inline int count() const { return (d.size() << 3) - *d.constData(); }
    int count(bool on) const;
    // ### Qt 5: Store the number of set bits separately

    inline bool isEmpty() const { return d.isEmpty(); }
    inline bool isNull() const { return d.isNull(); }

    void resize(int size);

    inline void clear() { d.clear(); }

    bool testBit(int i) const;
    void setBit(int i);
    void setBit(int i, bool val);
    void clearBit(int i);
    bool toggleBit(int i);

    bool at(int i) const;
    QBitRef operator[](int i);
    bool operator[](int i) const;
    QBitRef operator[](uint i);
    bool operator[](uint i) const;

    QBitArray& operator&=(const QBitArray &);
    QBitArray& operator|=(const QBitArray &);
    QBitArray& operator^=(const QBitArray &);
    QBitArray  operator~() const;

    inline bool operator==(const QBitArray& a) const { return d == a.d; }
    inline bool operator!=(const QBitArray& a) const { return d != a.d; }

    inline void fill(bool val, int size = -1);
    void fill(bool val, int first, int last);

    inline void truncate(int pos) { if (pos < size()) resize(pos); }
};

inline void QBitArray::fill(bool aval, int asize)
{ *this = QBitArray((asize < 0 ? this->size() : asize), aval); }

Q_CORE_EXPORT QBitArray operator&(const QBitArray &, const QBitArray &);
Q_CORE_EXPORT QBitArray operator|(const QBitArray &, const QBitArray &);
Q_CORE_EXPORT QBitArray operator^(const QBitArray &, const QBitArray &);

inline bool QBitArray::testBit(int i) const
{ Q_ASSERT(i < size());
 return (*(reinterpret_cast<const uchar*>(d.constData())+1+(i>>3)) & (1 << (i & 7))) != 0; }

inline void QBitArray::setBit(int i)
{ Q_ASSERT(i < size());
 *(reinterpret_cast<uchar*>(d.data())+1+(i>>3)) |= uchar(1 << (i & 7)); }

inline void QBitArray::clearBit(int i)
{ Q_ASSERT(i < size());
 *(reinterpret_cast<uchar*>(d.data())+1+(i>>3)) &= ~uchar(1 << (i & 7)); }

inline void QBitArray::setBit(int i, bool val)
{ if (val) setBit(i); else clearBit(i); }

inline bool QBitArray::toggleBit(int i)
{ Q_ASSERT(i < size());
 uchar b = uchar(1<<(i&7)); uchar* p = reinterpret_cast<uchar*>(d.data())+1+(i>>3);
 uchar c = uchar(*p&b); *p^=b; return c!=0; }

inline bool QBitArray::operator[](int i) const { return testBit(i); }
inline bool QBitArray::operator[](uint i) const { return testBit(i); }
inline bool QBitArray::at(int i) const { return testBit(i); }

class Q_CORE_EXPORT QBitRef
{
private:
    QBitArray& a;
    int i;
    inline QBitRef(QBitArray& array, int idx) : a(array), i(idx) {}
    friend class QBitArray;
public:
    inline operator bool() const { return a.testBit(i); }
    inline bool operator!() const { return !a.testBit(i); }
    QBitRef& operator=(const QBitRef& val) { a.setBit(i, val); return *this; }
    QBitRef& operator=(bool val) { a.setBit(i, val); return *this; }
};

inline QBitRef QBitArray::operator[](int i)
{ Q_ASSERT(i >= 0); return QBitRef(*this, i); }
inline QBitRef QBitArray::operator[](uint i)
{ return QBitRef(*this, i); }


#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QBitArray &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QBitArray &);
#endif

#ifndef QT_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug, const QBitArray &);
#endif

Q_DECLARE_TYPEINFO(QBitArray, Q_MOVABLE_TYPE);

QT_END_NAMESPACE


#endif // QBITARRAY_H
