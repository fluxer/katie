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

#ifndef QDATASTREAM_H
#define QDATASTREAM_H

#include <QtCore/qiodevice.h>


QT_BEGIN_NAMESPACE


class QByteArray;

template <typename T> class QList;
template <typename T> class QLinkedList;
template <typename T> class QVector;
template <typename T> class QSet;
template <class Key, class T> class QHash;
template <class Key, class T> class QMap;

#if !defined(QT_NO_DATASTREAM)
class Q_CORE_EXPORT QDataStream
{
public:
    enum Version {
        Qt_4_6 = 12,
        Qt_4_7 = Qt_4_6,
        Qt_4_8 = Qt_4_7,
        Qt_4_9 = Qt_4_8,
        Qt_4_10 = Qt_4_9,
        Qt_4_11 = Qt_4_10,
#if QT_VERSION > 0x041101
#error Add the datastream version for this version
        Qt_4_12 = Qt_4_11
#endif
        Qt_Default = Qt_4_11
    };

    enum ByteOrder {
        BigEndian = Q_BIG_ENDIAN,
        LittleEndian = Q_LITTLE_ENDIAN,
        HostEndian = Q_BYTE_ORDER
    };

    enum DataStatus {
        Ok,
        ReadPastEnd,
        ReadCorruptData,
        WriteFailed
    };

    enum FloatingPointPrecision {
        SinglePrecision,
        DoublePrecision
    };

    QDataStream();
    explicit QDataStream(QIODevice *);
    QDataStream(QByteArray *, QIODevice::OpenMode flags);
    QDataStream(const QByteArray &);
    ~QDataStream();

    QIODevice *device() const;
    void setDevice(QIODevice *);

    bool atEnd() const;

    DataStatus status() const;
    void setStatus(DataStatus status);
    void resetStatus();

    FloatingPointPrecision floatingPointPrecision() const;
    void setFloatingPointPrecision(FloatingPointPrecision precision);

    ByteOrder byteOrder() const;
    void setByteOrder(ByteOrder);

    Version version() const;
    void setVersion(Version);

    QDataStream &operator>>(qint8 &i);
    QDataStream &operator>>(quint8 &i);
    QDataStream &operator>>(qint16 &i);
    QDataStream &operator>>(quint16 &i);
    QDataStream &operator>>(qint32 &i);
    QDataStream &operator>>(quint32 &i);
    QDataStream &operator>>(qint64 &i);
    QDataStream &operator>>(quint64 &i);

    QDataStream &operator>>(bool &i);
    QDataStream &operator>>(float &f);
    QDataStream &operator>>(double &f);
    QDataStream &operator>>(char *&str);

    QDataStream &operator<<(qint8 i);
    QDataStream &operator<<(quint8 i);
    QDataStream &operator<<(qint16 i);
    QDataStream &operator<<(quint16 i);
    QDataStream &operator<<(qint32 i);
    QDataStream &operator<<(quint32 i);
    QDataStream &operator<<(qint64 i);
    QDataStream &operator<<(quint64 i);
    QDataStream &operator<<(bool i);
    QDataStream &operator<<(float f);
    QDataStream &operator<<(double f);
    QDataStream &operator<<(const char *str);

    QDataStream &readBytes(char *&, uint &len);
    int readRawData(char *, int len);

    QDataStream &writeBytes(const char *, uint len);
    int writeRawData(const char *, int len);

    int skipRawData(int len);

private:
    Q_DISABLE_COPY(QDataStream)

    QIODevice *dev;
    bool owndev;
    bool noswap;
    ByteOrder byteorder;
    Version ver;
    DataStatus q_status;
    FloatingPointPrecision floatingPrecision;
};


/*****************************************************************************
  QDataStream inline functions
 *****************************************************************************/

inline QIODevice *QDataStream::device() const
{ return dev; }

inline QDataStream::ByteOrder QDataStream::byteOrder() const
{ return byteorder; }

inline QDataStream::Version QDataStream::version() const
{ return ver; }

inline void QDataStream::setVersion(QDataStream::Version v)
{ ver = v; }

inline QDataStream &QDataStream::operator>>(quint8 &i)
{ return *this >> reinterpret_cast<qint8&>(i); }

inline QDataStream &QDataStream::operator>>(quint16 &i)
{ return *this >> reinterpret_cast<qint16&>(i); }

inline QDataStream &QDataStream::operator>>(quint32 &i)
{ return *this >> reinterpret_cast<qint32&>(i); }

inline QDataStream &QDataStream::operator>>(quint64 &i)
{ return *this >> reinterpret_cast<qint64&>(i); }

inline QDataStream &QDataStream::operator<<(quint8 i)
{ return *this << qint8(i); }

inline QDataStream &QDataStream::operator<<(quint16 i)
{ return *this << qint16(i); }

inline QDataStream &QDataStream::operator<<(quint32 i)
{ return *this << qint32(i); }

inline QDataStream &QDataStream::operator<<(quint64 i)
{ return *this << qint64(i); }

template <typename T>
QDataStream& operator>>(QDataStream& s, QList<T>& l)
{
    l.clear();
    quint32 c;
    s >> c;
    l.reserve(c);
    for(quint32 i = 0; i < c; ++i)
    {
        T t;
        s >> t;
        l.append(t);
        if (s.atEnd())
            break;
    }
    return s;
}

template <typename T>
QDataStream& operator<<(QDataStream& s, const QList<T>& l)
{
    s << quint32(l.size());
    for (int i = 0; i < l.size(); ++i)
        s << l.at(i);
    return s;
}

template <typename T>
QDataStream& operator>>(QDataStream& s, QLinkedList<T>& l)
{
    l.clear();
    quint32 c;
    s >> c;
    for(quint32 i = 0; i < c; ++i)
    {
        T t;
        s >> t;
        l.append(t);
        if (s.atEnd())
            break;
    }
    return s;
}

template <typename T>
QDataStream& operator<<(QDataStream& s, const QLinkedList<T>& l)
{
    s << quint32(l.size());
    typename QLinkedList<T>::const_iterator it = l.constBegin();
    for(; it != l.constEnd(); ++it)
        s << *it;
    return s;
}

template<typename T>
QDataStream& operator>>(QDataStream& s, QVector<T>& v)
{
    v.clear();
    quint32 c;
    s >> c;
    v.resize(c);
    for(quint32 i = 0; i < c; ++i) {
        T t;
        s >> t;
        v[i] = t;
    }
    return s;
}

template<typename T>
QDataStream& operator<<(QDataStream& s, const QVector<T>& v)
{
    s << quint32(v.size());
    for (typename QVector<T>::const_iterator it = v.constBegin(); it != v.constEnd(); ++it)
        s << *it;
    return s;
}

template <typename T>
QDataStream &operator>>(QDataStream &in, QSet<T> &set)
{
    set.clear();
    quint32 c;
    in >> c;
    set.reserve(c);
    for (quint32 i = 0; i < c; ++i) {
        T t;
        in >> t;
        set << t;
        if (in.atEnd())
            break;
    }
    return in;
}

template <typename T>
QDataStream& operator<<(QDataStream &out, const QSet<T> &set)
{
    out << quint32(set.size());
    for (typename QSet<T>::const_iterator it = set.constBegin(); it != set.constEnd(); ++it)
        out << *it;
    return out;
}

template <class Key, class T>
Q_OUTOFLINE_TEMPLATE QDataStream &operator>>(QDataStream &in, QHash<Key, T> &hash)
{
    QDataStream::DataStatus oldStatus = in.status();
    in.resetStatus();
    hash.clear();

    quint32 n;
    in >> n;
    hash.reserve(n);

    for (quint32 i = 0; i < n; ++i) {
        if (in.status() != QDataStream::Ok)
            break;

        Key k;
        T t;
        in >> k >> t;
        hash.insertMulti(k, t);
    }

    if (in.status() != QDataStream::Ok)
        hash.clear();
    if (oldStatus != QDataStream::Ok)
        in.setStatus(oldStatus);
    return in;
}

template <class Key, class T>
Q_OUTOFLINE_TEMPLATE QDataStream &operator<<(QDataStream &out, const QHash<Key, T>& hash)
{
    out << quint32(hash.size());
    typename QHash<Key, T>::const_iterator it = hash.constEnd();
    typename QHash<Key, T>::const_iterator begin = hash.constBegin();
    while (it != begin) {
        --it;
        out << it.key() << it.value();
    }
    return out;
}
template <class aKey, class aT>
Q_OUTOFLINE_TEMPLATE QDataStream &operator>>(QDataStream &in, QMap<aKey, aT> &map)
{
    QDataStream::DataStatus oldStatus = in.status();
    in.resetStatus();
    map.clear();

    quint32 n;
    in >> n;

    map.detach();
    map.setInsertInOrder(true);
    for (quint32 i = 0; i < n; ++i) {
        if (in.status() != QDataStream::Ok)
            break;

        aKey key;
        aT value;
        in >> key >> value;
        map.insertMulti(key, value);
    }
    map.setInsertInOrder(false);
    if (in.status() != QDataStream::Ok)
        map.clear();
    if (oldStatus != QDataStream::Ok)
        in.setStatus(oldStatus);
    return in;
}

template <class Key, class T>
Q_OUTOFLINE_TEMPLATE QDataStream &operator<<(QDataStream &out, const QMap<Key, T> &map)
{
    out << quint32(map.size());
    typename QMap<Key, T>::const_iterator it = map.constEnd();
    typename QMap<Key, T>::const_iterator begin = map.constBegin();
    while (it != begin) {
        --it;
        out << it.key() << it.value();
    }
    return out;
}

#endif // QT_NO_DATASTREAM

QT_END_NAMESPACE


#endif // QDATASTREAM_H
