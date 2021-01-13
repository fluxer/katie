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

#ifndef QSET_H
#define QSET_H

#include <QtCore/qhash.h>

#include <unordered_set>

QT_BEGIN_NAMESPACE

template<class T>
struct SetHasher {
    size_t operator()(const T& k) const { return qHash(k); }
};

template <class T>
class Q_CORE_EXPORT QSet : public std::unordered_set<T, SetHasher<T>>
{
    typedef std::unordered_set<T, SetHasher<T>> Data;

public:
    inline QSet() : Data() {}
    inline QSet(const QSet<T> &other) : Data(other) {}

    inline bool isEmpty() const { return Data::empty(); }
    inline void squeeze() { Data::rehash(); }
    inline bool remove(const T &value) { return Data::erase(value) != 0; }
    inline int count() const { return Data::size(); }
    inline bool contains(const T &value) const { return Data::count(value) != 0; }
    bool contains(const QSet<T> &set) const;

    inline typename Data::iterator insert(const T &value)
        { return static_cast<typename Data::iterator>(Data::emplace(value).first); }
    inline typename Data::const_iterator constBegin() const { return Data::cbegin(); }
    inline typename Data::const_iterator constEnd() const { return Data::cend(); }
    inline typename Data::const_iterator constFind(const T &value) const { return Data::find(value); }
    typedef typename Data::iterator Iterator;
    typedef typename Data::const_iterator ConstIterator;

    QSet<T> &unite(const QSet<T> &other);
    QSet<T> &intersect(const QSet<T> &other);
    QSet<T> &subtract(const QSet<T> &other);

    // comfort
    inline QSet<T> &operator<<(const T &value) { insert(value); return *this; }
    inline QSet<T> &operator|=(const QSet<T> &other) { unite(other); return *this; }
    inline QSet<T> &operator|=(const T &value) { insert(value); return *this; }
    inline QSet<T> &operator&=(const QSet<T> &other) { intersect(other); return *this; }
    inline QSet<T> &operator&=(const T &value)
        { QSet<T> result; if (contains(value)) result.insert(value); return (*this = result); }
    inline QSet<T> &operator+=(const QSet<T> &other) { unite(other); return *this; }
    inline QSet<T> &operator+=(const T &value) { insert(value); return *this; }
    inline QSet<T> &operator-=(const QSet<T> &other) { subtract(other); return *this; }
    inline QSet<T> &operator-=(const T &value) { remove(value); return *this; }
    inline QSet<T> operator|(const QSet<T> &other) const
        { QSet<T> result = *this; result |= other; return result; }
    inline QSet<T> operator&(const QSet<T> &other) const
        { QSet<T> result = *this; result &= other; return result; }
    inline QSet<T> operator+(const QSet<T> &other) const
        { QSet<T> result = *this; result += other; return result; }
    inline QSet<T> operator-(const QSet<T> &other) const
        { QSet<T> result = *this; result -= other; return result; }

    QList<T> toList() const;
    inline QList<T> values() const { return toList(); }

    static QSet<T> fromList(const QList<T> &list);
};

template <class T>
inline QSet<T> &QSet<T>::unite(const QSet<T> &other)
{
    typename QSet<T>::const_iterator i = other.constEnd();
    while (i != other.constEnd()) {
        insert(*i);
        i++;
    }
    return *this;
}

template <class T>
inline QSet<T> &QSet<T>::intersect(const QSet<T> &other)
{
    QSet<T> copy(*this);
    typename QSet<T>::const_iterator i = copy.constBegin();
    while (i != copy.constEnd()) {
        if (!other.contains(*i))
            remove(*i);
        i++;
    }
    return *this;
}

template <class T>
inline QSet<T> &QSet<T>::subtract(const QSet<T> &other)
{
    QSet<T> copy(*this);
    typename QSet<T>::const_iterator i = copy.constBegin();
    while (i != copy.constEnd()) {
        if (other.contains(*i))
            remove(*i);
        i++;
    }
    return *this;
}

template <class T>
inline bool QSet<T>::contains(const QSet<T> &other) const
{
    typename QSet<T>::const_iterator i = other.constBegin();
    while (i != other.constEnd()) {
        if (!contains(*i))
            return false;
        ++i;
    }
    return true;
}

template <typename T>
QList<T> QSet<T>::toList() const
{
    QList<T> result;
    result.reserve(Data::size());
    typename QSet<T>::const_iterator i = constBegin();
    while (i != constEnd()) {
        result.append(*i);
        ++i;
    }
    return result;
}

template <typename T>
QSet<T> QSet<T>::fromList(const QList<T> &list)
{
    return list.toSet();
}

Q_DECLARE_SEQUENTIAL_ITERATOR(Set)

template <typename T>
class QMutableSetIterator
{
    typedef typename QSet<T>::iterator iterator;
    QSet<T> *c;
    iterator i, n;
    inline bool item_exists() const { return c->constEnd() != n; }

public:
    inline QMutableSetIterator(QSet<T> &container)
        : c(&container)
    { i = c->begin(); n = c->end(); }
    inline ~QMutableSetIterator() { }
    inline QMutableSetIterator &operator=(QSet<T> &container)
    { c = &container; i = c->begin(); n = c->end(); return *this; }
    inline void toFront() { i = c->begin(); n = c->end(); }
    inline void toBack() { i = c->end(); n = i; }
    inline bool hasNext() const { return c->constEnd() != i; }
    inline const T &next() { n = i++; return *n; }
    inline const T &peekNext() const { return *i; }
    inline bool hasPrevious() const { return c->constBegin() != i; }
    inline const T &previous() { n = --i; return *n; }
    inline const T &peekPrevious() const { iterator p = i; return *--p; }
    inline void remove()
    { if (c->constEnd() != n) { i = c->erase(n); n = c->end(); } }
    inline const T &value() const { Q_ASSERT(item_exists()); return *n; }
    inline bool findNext(const T &t)
    { while (c->constEnd() != (n = i)) if (*i++ == t) return true; return false; }
    inline bool findPrevious(const T &t)
    { while (c->constBegin() != i) if (*(n = --i) == t) return true;
      n = c->end(); return false;  }
};

QT_END_NAMESPACE


#endif // QSET_H
