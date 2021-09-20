/****************************************************************************
**
** Copyright (C) 2020 Ivailo Monev
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

#ifndef QSTDCONTAINERS_P_H
#define QSTDCONTAINERS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qiterator.h>
#include <QtCore/qalgorithms.h>

#include <vector>
#include <unordered_set>

QT_BEGIN_NAMESPACE

template <typename T>
class Q_CORE_EXPORT QStdVector : public std::vector<T>
{
    typedef std::vector<T> Data;

public:
    QStdVector() : Data() { }
    explicit QStdVector(int size) : Data(size) { }
    QStdVector(int size, const T &t) : Data() { Data::reserve(size); Data::insert(Data::begin(), size, t);}
#ifdef Q_COMPILER_INITIALIZER_LISTS
    inline QStdVector(std::initializer_list<T> args) : Data(args) { }
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
    QStdVector<T> &fill(const T &t, int size = -1);
    int indexOf(const T &t, int from = 0) const;
    int lastIndexOf(const T &t, int from = -1) const;
    bool contains(const T &t) const;
    int count(const T &t) const;
    T value(int i) const;
    T value(int i, const T &defaultValue) const;
    QStdVector<T> mid(int pos, int length = -1) const;

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
    QStdVector<T> &operator+=(const QStdVector<T> &l);
    inline QStdVector<T> operator+(const QStdVector<T> &l) const { QStdVector n = *this; n += l; return n; }
    inline QStdVector<T> &operator+=(const T &t) { append(t); return *this; }
    inline QStdVector<T> &operator<<(const T &t) { append(t); return *this; }
    inline QStdVector<T> &operator<<(const QStdVector<T> &l) { *this += l; return *this; }

    static inline QStdVector<T> fromStdVector(const std::vector<T> &vector)
    { QStdVector<T> tmp; tmp.reserve(vector.size()); qCopy(vector.cbegin(), vector.cend(), std::back_inserter(tmp)); return tmp; }
    inline std::vector<T> toStdVector() const { return std::vector<T>(*this); }

    typedef typename Data::iterator Iterator;
    typedef typename Data::const_iterator ConstIterator;
    // type deduction for e.g. qMin()/qMax()
    inline int size() const { return Data::size(); }
    inline int capacity() const { return Data::capacity(); }
    // release memory and set capacity to 0
    inline void clear() { Data::clear(); Data::shrink_to_fit(); }

    // QList<T> compat
    inline int length() const { return Data::size(); }
    inline void removeFirst() { Q_ASSERT(!isEmpty()); Data::erase(Data::begin()); }
    inline void removeLast() { Q_ASSERT(!isEmpty()); Data::erase(Data::end()); }

    void append(const QStdVector<T> &l);
    void removeAt(int i);
    int removeAll(const T &t);
    bool removeOne(const T &t);
    T takeAt(int i);
    T takeFirst();
    T takeLast();
    void move(int from, int to);

#if !defined(Q_NO_USING_KEYWORD)
    using Data::insert;
#endif
};

template <typename T>
inline void QStdVector<T>::remove(int i, int n)
{
    Q_ASSERT_X(i >= 0 && n >= 0 && i + n <= Data::size(), "QStdVector<T>::remove", "index out of range");
    Data::erase(Data::begin() + i, Data::begin() + i + n);
}

template <typename T>
inline void QStdVector<T>::remove(int i)
{
    Q_ASSERT_X(i >= 0 && i < Data::size(), "QStdVector<T>::remove", "index out of range");
    Data::erase(Data::begin() + i, Data::begin() + i + 1);
}

template <typename T>
inline void QStdVector<T>::replace(int i, const T &t)
{
    Q_ASSERT_X(i >= 0 && i < Data::size(), "QStdVector<T>::replace", "index out of range");
    Data::data()[i] = t;
}

template<typename T>
inline T QStdVector<T>::value(int i) const
{
    if (i < 0 || i >= Data::size()) {
        return T();
    }
    return Data::at(i);
}
template<typename T>
inline T QStdVector<T>::value(int i, const T &defaultValue) const
{
    return ((i < 0 || i >= Data::size()) ? defaultValue :  Data::at(i));
}

template <typename T>
QStdVector<T> &QStdVector<T>::fill(const T &from, int asize)
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
inline QStdVector<T> &QStdVector<T>::operator+=(const QStdVector &l)
{
    Data::reserve(Data::size() + l.size());
    for (size_t i = 0; i < l.size(); i++) {
        Data::push_back(l.at(i));
    }
    return *this;
}

template <typename T>
int QStdVector<T>::indexOf(const T &t, int from) const
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
int QStdVector<T>::lastIndexOf(const T &t, int from) const
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
bool QStdVector<T>::contains(const T &t) const
{
    for (size_t i = 0; i < Data::size(); i++) {
        if (Data::at(i) == t)
            return true;
    }
    return false;
}

template <typename T>
int QStdVector<T>::count(const T &t) const
{
    int c = 0;
    for (size_t i = 0; i < Data::size(); i++) {
        if (Data::at(i) == t)
            c++;
    }
    return c;
}

template <typename T>
QStdVector<T> QStdVector<T>::mid(int pos, int length) const
{
    if (length < 0)
        length = Data::size() - pos;
    if (pos == 0 && length == Data::size())
        return *this;
    if (pos + length > Data::size())
        length = Data::size() - pos;
    QStdVector<T> copy;
    copy.reserve(length);
    for (size_t i = pos; i < pos + length; i++)
        copy.push_back(Data::at(i));
    return copy;
}

// QList<T> compat
template <typename T>
inline void QStdVector<T>::append(const QStdVector<T> &l)
{
    Data::reserve(Data::size() + l.size());
    for (size_t i = 0; i < l.size(); i++) {
        Data::push_back(l.at(i));
    }
}

template <typename T>
inline void QStdVector<T>::removeAt(int i)
{
    Data::erase(Data::begin() + i);
}

template <typename T>
inline T QStdVector<T>::takeAt(int i)
{
    Q_ASSERT_X(i >= 0 && i < Data::size(), "QStdVector<T>::take", "index out of range");
    T t = Data::at(i); removeAt(i); return t;
}

template <typename T>
inline T QStdVector<T>::takeFirst()
{
    T t = first(); removeFirst(); return t;
}

template <typename T>
inline T QStdVector<T>::takeLast()
{
    T t = last(); removeLast(); return t;
}

template <typename T>
inline void QStdVector<T>::move(int from, int to)
{
    Q_ASSERT_X(from >= 0 && from < Data::size() && to >= 0 && to < Data::size(),
               "QStdVector<T>::move", "index out of range");
    Data::insert(Data::begin() + to, takeAt(from));
}

template <typename T>
int QStdVector<T>::removeAll(const T &t)
{
    int c = 0;
    typename Data::iterator it = Data::begin();
    while (it != Data::end()) {
        if (*it == t) {
            Data::erase(it);
            c++;
            continue;
        }
        it++;
    }
    return c;
}

template <typename T>
bool QStdVector<T>::removeOne(const T &t)
{
    for (size_t i = 0; i < Data::size(); i++) {
        if (Data::at(i) == t) {
            Data::erase(Data::begin() + i);
            return true;
        }
    }
    return false;
}


template<class T>
struct SetHasher {
    inline size_t operator()(const T &t) const { return qHash(t); }
};

template <class T>
class Q_CORE_EXPORT QStdSet : public std::unordered_set<T, SetHasher<T>>
{
    typedef std::unordered_set<T, SetHasher<T>> Data;

public:
    inline QStdSet() : Data() {}
    inline QStdSet(const QStdSet<T> &other) : Data(other) {}

    inline bool isEmpty() const { return Data::empty(); }
    inline void squeeze() { Data::rehash(0); }
    inline bool remove(const T &value) { return Data::erase(value) != 0; }
    inline int count() const { return Data::size(); }
    inline bool contains(const T &value) const { return Data::count(value) != 0; }
    bool contains(const QStdSet<T> &set) const;

    inline typename Data::iterator insert(const T &value)
        { return static_cast<typename Data::iterator>(Data::emplace(value).first); }
    inline typename Data::const_iterator constBegin() const { return Data::cbegin(); }
    inline typename Data::const_iterator constEnd() const { return Data::cend(); }
    inline typename Data::const_iterator constFind(const T &value) const { return Data::find(value); }
    typedef typename Data::iterator Iterator;
    typedef typename Data::const_iterator ConstIterator;

    QStdSet<T> &unite(const QStdSet<T> &other);
    QStdSet<T> &intersect(const QStdSet<T> &other);
    QStdSet<T> &subtract(const QStdSet<T> &other);

    // comfort
    inline QStdSet<T> &operator<<(const T &value) { insert(value); return *this; }
    inline QStdSet<T> &operator|=(const QStdSet<T> &other) { unite(other); return *this; }
    inline QStdSet<T> &operator|=(const T &value) { insert(value); return *this; }
    inline QStdSet<T> &operator&=(const QStdSet<T> &other) { intersect(other); return *this; }
    inline QStdSet<T> &operator&=(const T &value)
        { QStdSet<T> result; if (contains(value)) result.insert(value); return (*this = result); }
    inline QStdSet<T> &operator+=(const QStdSet<T> &other) { unite(other); return *this; }
    inline QStdSet<T> &operator+=(const T &value) { insert(value); return *this; }
    inline QStdSet<T> &operator-=(const QStdSet<T> &other) { subtract(other); return *this; }
    inline QStdSet<T> &operator-=(const T &value) { remove(value); return *this; }
    inline QStdSet<T> operator|(const QStdSet<T> &other) const
        { QStdSet<T> result = *this; result |= other; return result; }
    inline QStdSet<T> operator&(const QStdSet<T> &other) const
        { QStdSet<T> result = *this; result &= other; return result; }
    inline QStdSet<T> operator+(const QStdSet<T> &other) const
        { QStdSet<T> result = *this; result += other; return result; }
    inline QStdSet<T> operator-(const QStdSet<T> &other) const
        { QStdSet<T> result = *this; result -= other; return result; }
};

template <class T>
inline QStdSet<T> &QStdSet<T>::unite(const QStdSet<T> &other)
{
    typename QStdSet<T>::const_iterator i = other.constEnd();
    while (i != other.constEnd()) {
        insert(*i);
        i++;
    }
    return *this;
}

template <class T>
inline QStdSet<T> &QStdSet<T>::intersect(const QStdSet<T> &other)
{
    QStdSet<T> copy(*this);
    typename QStdSet<T>::const_iterator i = copy.constBegin();
    while (i != copy.constEnd()) {
        if (!other.contains(*i))
            remove(*i);
        i++;
    }
    return *this;
}

template <class T>
inline QStdSet<T> &QStdSet<T>::subtract(const QStdSet<T> &other)
{
    QStdSet<T> copy(*this);
    typename QStdSet<T>::const_iterator i = copy.constBegin();
    while (i != copy.constEnd()) {
        if (other.contains(*i))
            remove(*i);
        i++;
    }
    return *this;
}

template <class T>
inline bool QStdSet<T>::contains(const QStdSet<T> &other) const
{
    typename QStdSet<T>::const_iterator i = other.constBegin();
    while (i != other.constEnd()) {
        if (!contains(*i))
            return false;
        ++i;
    }
    return true;
}

QT_END_NAMESPACE

#endif // QSTDCONTAINERS_P_H