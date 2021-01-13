/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QVECTOR_H
#define QVECTOR_H

#include <QtCore/qiterator.h>
#include <QtCore/qatomic.h>
#include <QtCore/qalgorithms.h>

#include <vector>


QT_BEGIN_NAMESPACE

template <typename T> class QList;

template <typename T>
class Q_CORE_EXPORT QVector : public std::vector<T>
{
    typedef std::vector<T> Data;

public:
    QVector() : Data() { }
    explicit QVector(int size) : Data(size) { }
    QVector(int size, const T &t) : Data() { Data::reserve(size); Data::insert(Data::begin(), size, t);}
#ifdef Q_COMPILER_INITIALIZER_LISTS
    inline QVector(std::initializer_list<T> args) : Data(args) { }
#endif

    inline bool isEmpty() const { return Data::empty(); }
    inline void squeeze() { Data::shrink_to_fit(); }
    inline const T *constData() const { return Data::data(); }
    inline void append(const T &t) { Data::push_back(t); }
    inline void prepend(const T &t) { Data::insert(Data::begin(), t); }
    inline void insert(int i, const T &t) { Data::insert(Data::begin() + i, t); }
    inline void insert(int i, int n, const T &t) { Data::insert(Data::begin() + i, n, t); }
    inline void push_front(const T &t) { prepend(t); }

    void replace(int i, const T &t);
    void remove(int i);
    void remove(int i, int n);
    QVector<T> &fill(const T &t, int size = -1);
    int indexOf(const T &t, int from = 0) const;
    int lastIndexOf(const T &t, int from = -1) const;
    bool contains(const T &t) const;
    int count(const T &t) const;
    T value(int i) const;
    T value(int i, const T &defaultValue) const;
    QVector<T> mid(int pos, int length = -1) const;

    inline int count() const { return Data::size(); }
    inline T& first() { Q_ASSERT(!isEmpty()); return Data::front(); }
    inline const T &first() const { Q_ASSERT(!isEmpty()); return Data::front(); }
    inline T& last() { Q_ASSERT(!isEmpty()); return Data::back(); }
    inline const T &last() const { Q_ASSERT(!isEmpty()); return Data::back(); }
    inline bool startsWith(const T &t) const { return !isEmpty() && first() == t; }
    inline bool endsWith(const T &t) const { return !isEmpty() && last() == t; }
    inline void pop_front() { Q_ASSERT(!isEmpty()); Data::erase(Data::begin()); }
    inline typename Data::const_iterator constBegin() const { return Data::cbegin(); }
    inline typename Data::const_iterator constEnd() const { return Data::cend(); }

    // comfort
    QVector<T> &operator+=(const QVector<T> &l);
    inline QVector<T> operator+(const QVector<T> &l) const { QVector n = *this; n += l; return n; }
    inline QVector<T> &operator+=(const T &t) { append(t); return *this; }
    inline QVector<T> &operator<<(const T &t) { append(t); return *this; }
    inline QVector<T> &operator<<(const QVector<T> &l) { *this += l; return *this; }

    QList<T> toList() const;
    static QVector<T> fromList(const QList<T> &list);

    static inline QVector<T> fromStdVector(const std::vector<T> &vector)
    { QVector<T> tmp; tmp.reserve(vector.size()); qCopy(vector.cbegin(), vector.cend(), std::back_inserter(tmp)); return tmp; }
    inline std::vector<T> toStdVector() const { return std::vector<T>(*this); }

    typedef typename Data::iterator Iterator;
    typedef typename Data::const_iterator ConstIterator;
     // type deduction for e.g. qMin()/qMax()
    inline int size() const { return Data::size(); }
    inline int capacity() const { return Data::capacity(); }
     // release memory and set capacity to 0
    inline void clear() { Data::clear(); Data::shrink_to_fit(); }

#if !defined(Q_NO_USING_KEYWORD)
    using Data::insert;
#endif
};

template <typename T>
inline void QVector<T>::remove(int i, int n)
{
    Q_ASSERT_X(i >= 0 && n >= 0 && i + n <= Data::size(), "QVector<T>::remove", "index out of range");
    Data::erase(Data::begin() + i, Data::begin() + i + n);
}

template <typename T>
inline void QVector<T>::remove(int i)
{
    Q_ASSERT_X(i >= 0 && i < Data::size(), "QVector<T>::remove", "index out of range");
    Data::erase(Data::begin() + i, Data::begin() + i + 1);
}

template <typename T>
inline void QVector<T>::replace(int i, const T &t)
{
    Q_ASSERT_X(i >= 0 && i < Data::size(), "QVector<T>::replace", "index out of range");
    Data::data()[i] = t;
}

template<typename T>
inline T QVector<T>::value(int i) const
{
    if (i < 0 || i >= Data::size()) {
        return T();
    }
    return Data::at(i);
}
template<typename T>
inline T QVector<T>::value(int i, const T &defaultValue) const
{
    return ((i < 0 || i >= Data::size()) ? defaultValue :  Data::at(i));
}

template <typename T>
QVector<T> &QVector<T>::fill(const T &from, int asize)
{
    if (asize < 0)
        asize = Data::size();
    Data::clear();
    if (asize > 0) {
        Data::reserve(asize);
        Data::insert(Data::begin(), asize, from);
    }
    Data::shrink_to_fit();
    return *this;
}

template <typename T>
inline QVector<T> &QVector<T>::operator+=(const QVector &l)
{
    Data::reserve(Data::size() + l.size());
    for (size_t i = 0; i < l.size(); i++) {
        Data::push_back(l.at(i));
    }
    return *this;
}

template <typename T>
int QVector<T>::indexOf(const T &t, int from) const
{
    if (from < 0)
        from = qMax(from + int(Data::size()), 0);
    if (from < Data::size()) {
        for (size_t i = from; i < Data::size(); i++) {
            if (Data::at(i) == t)
                return i;
        }
    }
    return -1;
}

template <typename T>
int QVector<T>::lastIndexOf(const T &t, int from) const
{
    if (from < 0)
        from += Data::size();
    else if (from >= Data::size())
        from = Data::size() - 1;
    while(from >= 0) {
        if (Data::at(from) == t)
            return from;
        from--;
    }
    return -1;
}

template <typename T>
bool QVector<T>::contains(const T &t) const
{
    for (size_t i = 0; i < Data::size(); i++) {
        if (Data::at(i) == t)
            return true;
    }
    return false;
}

template <typename T>
int QVector<T>::count(const T &t) const
{
    int c = 0;
    for (size_t i = 0; i < Data::size(); i++) {
        if (Data::at(i) == t)
            c++;
    }
    return c;
}

template <typename T>
QVector<T> QVector<T>::mid(int pos, int length) const
{
    if (length < 0)
        length = Data::size() - pos;
    if (pos == 0 && length == Data::size())
        return *this;
    if (pos + length > Data::size())
        length = Data::size() - pos;
    QVector<T> copy;
    copy.reserve(length);
    for (size_t i = pos; i < pos + length; i++)
        copy.push_back(Data::at(i));
    return copy;
}

template <typename T>
QList<T> QVector<T>::toList() const
{
    QList<T> result;
    result.reserve(Data::size());
    for (size_t i = 0; i < Data::size(); i++)
        result.append(Data::at(i));
    return result;
}

template <typename T>
inline QVector<T> QVector<T>::fromList(const QList<T> &list)
{
    return list.toVector();
}

Q_DECLARE_SEQUENTIAL_ITERATOR(Vector)
Q_DECLARE_MUTABLE_SEQUENTIAL_ITERATOR(Vector)

QT_END_NAMESPACE


#endif // QVECTOR_H
