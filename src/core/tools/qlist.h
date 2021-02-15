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

#ifndef QLIST_H
#define QLIST_H

#include <QtCore/qiterator.h>
#include <QtCore/qatomic.h>
#include <QtCore/qalgorithms.h>

#include <vector>
#include <list>


QT_BEGIN_NAMESPACE

template <typename T> class QSet;
template <typename T> class QVector;

template <typename T>
class Q_CORE_EXPORT QList : public std::vector<T>
{
    typedef std::vector<T> Data;

public:
    QList() : Data() { }
    QList(const QList &other) : Data() {
        Data::reserve(other.size());
        for (size_t i = 0;  i < other.size(); i++)
            Data::push_back(other.at(i));
    }
    explicit QList(int size) : Data(size) { }
#ifdef Q_COMPILER_INITIALIZER_LISTS
    inline QList(std::initializer_list<T> args) : Data(args) { }
#endif

    inline int length() const { return Data::size(); }
    inline void removeFirst() { Q_ASSERT(!isEmpty()); Data::erase(Data::begin()); }
    inline void removeLast() { Q_ASSERT(!isEmpty()); Data::erase(Data::end()); }

    void append(const QList<T> &l);
    void removeAt(int i);
    int removeAll(const T &t);
    bool removeOne(const T &t);
    T takeAt(int i);
    T takeFirst();
    T takeLast();
    void move(int from, int to);

    QVector<T> toVector() const;
    QSet<T> toSet() const;

    static QList<T> fromVector(const QVector<T> &vector);
    static QList<T> fromSet(const QSet<T> &set);

    static inline QList<T> fromStdList(const std::list<T> &list)
    { QList<T> tmp; qCopy(list.begin(), list.end(), std::back_inserter(tmp)); return tmp; }
    inline std::list<T> toStdList() const
    { std::list<T> tmp; qCopy(Data::cbegin(), Data::cend(), std::back_inserter(tmp)); return tmp; }

    // almost the same as QVector<T>
    inline bool isEmpty() const { return Data::empty(); }
    inline void append(const T &t) { Data::push_back(t); }
    inline void prepend(const T &t) { Data::insert(Data::begin(), t); }
    inline void insert(int i, const T &t) { Data::insert(Data::begin() + i, t); }
    inline void insert(int i, int n, const T &t) { Data::insert(Data::begin() + i, n, t); }
    inline void push_front(const T &t) { prepend(t); }

    void replace(int i, const T &t);
    void remove(int i);
    void remove(int i, int n);
    int indexOf(const T &t, int from = 0) const;
    int lastIndexOf(const T &t, int from = -1) const;
    bool contains(const T &t) const;
    int count(const T &t) const;
    T value(int i) const;
    T value(int i, const T &defaultValue) const;
    QList<T> mid(int pos, int length = -1) const;

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
    QList<T> &operator+=(const QList<T> &l);
    inline QList<T> operator+(const QList<T> &l) const { QList n = *this; n += l; return n; }
    inline QList<T> &operator+=(const T &t) { append(t); return *this; }
    inline QList<T> &operator<<(const T &t) { append(t); return *this; }
    inline QList<T> &operator<<(const QList<T> &l) { *this += l; return *this; }

    typedef typename Data::iterator Iterator;
    typedef typename Data::const_iterator ConstIterator;
     // type deduction for e.g. qMin()/qMax()
    inline int size() const { return Data::size(); }
     // release memory and set capacity to 0
    inline void clear() { Data::clear(); Data::shrink_to_fit(); }

#if !defined(Q_NO_USING_KEYWORD)
    using Data::insert;
#endif
};

template <typename T>
inline void QList<T>::append(const QList<T> &l)
{
    Data::reserve(Data::size() + l.size());
    for (size_t i = 0; i < l.size(); i++) {
        Data::push_back(l.at(i));
    }
}

template <typename T>
inline void QList<T>::removeAt(int i)
{
    Data::erase(Data::begin() + i);
}

template <typename T>
inline T QList<T>::takeAt(int i)
{
    Q_ASSERT_X(i >= 0 && i < Data::size(), "QList<T>::take", "index out of range");
    T t = Data::at(i); removeAt(i); return t;
}

template <typename T>
inline T QList<T>::takeFirst()
{
    T t = first(); removeFirst(); return t;
}

template <typename T>
inline T QList<T>::takeLast()
{
    T t = last(); removeLast(); return t;
}

template <typename T>
inline void QList<T>::move(int from, int to)
{
    Q_ASSERT_X(from >= 0 && from < Data::size() && to >= 0 && to < Data::size(),
               "QList<T>::move", "index out of range");
    Data::insert(Data::begin() + to, takeAt(from));
}

template <typename T>
int QList<T>::removeAll(const T &t)
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
bool QList<T>::removeOne(const T &t)
{
    for (size_t i = 0; i < Data::size(); i++) {
        if (Data::at(i) == t) {
            Data::erase(Data::begin() + i);
            return true;
        }
    }
    return false;
}

template <typename T>
QVector<T> QList<T>::toVector() const
{
    QVector<T> result;
    result.reserve(Data::size());
    for (size_t i = 0; i < Data::size(); i++)
        result.push_back(Data::at(i));
    return result;
}

template <typename T>
QList<T> QList<T>::fromVector(const QVector<T> &vector)
{
    QList<T> result;
    result.reserve(vector.size());
    for (size_t i = 0; i < vector.size(); i++)
        result.push_back(vector.at(i));
    return result;
}

template <typename T>
QList<T> QList<T>::fromSet(const QSet<T> &set)
{
    return set.toList();
}

template <typename T>
QSet<T> QList<T>::toSet() const
{
    QSet<T> result;
    result.reserve(Data::size());
    for (int i = 0; i < Data::size(); ++i)
        result.insert(Data::at(i));
    return result;
}

// copy from QVector<T>
template <typename T>
inline void QList<T>::remove(int i, int n)
{
    Q_ASSERT_X(i >= 0 && n >= 0 && i + n <= Data::size(), "QList<T>::remove", "index out of range");
    Data::erase(Data::begin() + i, Data::begin() + i + n);
}

template <typename T>
inline void QList<T>::remove(int i)
{
    Q_ASSERT_X(i >= 0 && i < Data::size(), "QList<T>::remove", "index out of range");
    Data::erase(Data::begin() + i, Data::begin() + i + 1);
}

template <typename T>
inline void QList<T>::replace(int i, const T &t)
{
    Q_ASSERT_X(i >= 0 && i < Data::size(), "QList<T>::replace", "index out of range");
    Data::data()[i] = t;
}

template<typename T>
inline T QList<T>::value(int i) const
{
    if (i < 0 || i >= Data::size()) {
        return T();
    }
    return Data::at(i);
}
template<typename T>
inline T QList<T>::value(int i, const T &defaultValue) const
{
    return ((i < 0 || i >= Data::size()) ? defaultValue :  Data::at(i));
}

template <typename T>
inline QList<T> &QList<T>::operator+=(const QList &l)
{
    append(l);
    return *this;
}

template <typename T>
int QList<T>::indexOf(const T &t, int from) const
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
int QList<T>::lastIndexOf(const T &t, int from) const
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
bool QList<T>::contains(const T &t) const
{
    for (size_t i = 0; i < Data::size(); i++) {
        if (Data::at(i) == t)
            return true;
    }
    return false;
}

template <typename T>
int QList<T>::count(const T &t) const
{
    int c = 0;
    for (size_t i = 0; i < Data::size(); i++) {
        if (Data::at(i) == t)
            c++;
    }
    return c;
}

template <typename T>
QList<T> QList<T>::mid(int pos, int length) const
{
    if (length < 0)
        length = Data::size() - pos;
    if (pos == 0 && length == Data::size())
        return *this;
    if (pos + length > Data::size())
        length = Data::size() - pos;
    QList<T> copy;
    copy.reserve(length);
    for (size_t i = pos; i < pos + length; i++)
        copy.push_back(Data::at(i));
    return copy;
}

Q_DECLARE_SEQUENTIAL_ITERATOR(List)
Q_DECLARE_MUTABLE_SEQUENTIAL_ITERATOR(List)

QT_END_NAMESPACE


#endif // QLIST_H
