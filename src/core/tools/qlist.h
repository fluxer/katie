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

#include <iterator>
#include <initializer_list>
#include <new>
#include <string.h>

QT_BEGIN_NAMESPACE

template <typename T> class QVector;
template <typename T> class QSet;

struct Q_CORE_EXPORT QListData {
    struct Data {
        QAtomicInt ref;
        int alloc, begin, end;
        void *array[1];
    };
    enum { DataHeaderSize = sizeof(Data) - QT_POINTER_SIZE };

    Data *detach(int alloc);
    static void freeData(Data *);
    void reallocData(int alloc);
    static Data shared_null;
    Data *d;
    void **erase(void **xi);
    void **append(int n);
    void **append();
    void **append(const QListData &l);
    void **prepend();
    void **insert(int i);
    void remove(int i);
    void remove(int i, int n);
    void move(int from, int to);
    inline int size() const { return d->end - d->begin; }
    inline bool isEmpty() const { return d->end  == d->begin; }
    inline void **at(int i) const { return d->array + d->begin + i; }
    inline void **begin() const { return d->array + d->begin; }
    inline void **end() const { return d->array + d->end; }
};

template <typename T>
class Q_CORE_EXPORT QList
{
    struct Node { void *v;
        Q_INLINE_TEMPLATE T &t()
        { return *reinterpret_cast<T*>(QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic
                                       ? v : this); }
    };

    union { QListData p; QListData::Data *d; };

public:
    inline QList() : d(&QListData::shared_null) { d->ref.ref(); }
    inline QList(const QList<T> &l) : d(l.d) { d->ref.ref(); }
    ~QList();
    QList<T> &operator=(const QList<T> &l);
    inline QList &operator=(QList &&other)
    { qSwap(d, other.d); return *this; }
    inline void swap(QList<T> &other) { qSwap(d, other.d); }
    inline QList(std::initializer_list<T> args) : d(&QListData::shared_null)
    { d->ref.ref(); qCopy(args.begin(), args.end(), std::back_inserter(*this)); }
    bool operator==(const QList<T> &l) const;
    inline bool operator!=(const QList<T> &l) const { return !(*this == l); }

    inline int size() const { return p.size(); }
    inline bool isEmpty() const { return p.isEmpty(); }

    void clear();

    const T &at(int i) const;
    const T &operator[](int i) const;
    T &operator[](int i);

    void reserve(int size);
    void append(const T &t);
    void append(const QList<T> &t);
    void prepend(const T &t);
    void insert(int i, const T &t);
    void replace(int i, const T &t);
    void removeAt(int i);
    int removeAll(const T &t);
    bool removeOne(const T &t);
    T takeAt(int i);
    T takeFirst();
    T takeLast();
    void move(int from, int to);
    void swap(int i, int j);
    int indexOf(const T &t, int from = 0) const;
    int lastIndexOf(const T &t, int from = -1) const;
    bool contains(const T &t) const;
    int count(const T &t) const;

    class const_iterator;

    class iterator {
    public:
        Node *i;
        typedef std::random_access_iterator_tag  iterator_category;
        typedef qptrdiff difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;

        inline iterator() : i(nullptr) {}
        inline iterator(Node *n) : i(n) {}
        inline iterator(const iterator &o): i(o.i){}
        inline T &operator*() const { return i->t(); }
        inline T *operator->() const { return &i->t(); }
        inline T &operator[](int j) const { return i[j].t(); }
        inline bool operator==(const iterator &o) const { return i == o.i; }
        inline bool operator!=(const iterator &o) const { return i != o.i; }
        inline bool operator<(const iterator& other) const { return i < other.i; }
        inline bool operator<=(const iterator& other) const { return i <= other.i; }
        inline bool operator>(const iterator& other) const { return i > other.i; }
        inline bool operator>=(const iterator& other) const { return i >= other.i; }
#ifndef QT_STRICT_ITERATORS
        inline bool operator==(const const_iterator &o) const
            { return i == o.i; }
        inline bool operator!=(const const_iterator &o) const
            { return i != o.i; }
        inline bool operator<(const const_iterator& other) const
            { return i < other.i; }
        inline bool operator<=(const const_iterator& other) const
            { return i <= other.i; }
        inline bool operator>(const const_iterator& other) const
            { return i > other.i; }
        inline bool operator>=(const const_iterator& other) const
            { return i >= other.i; }
#endif
        inline iterator &operator++() { ++i; return *this; }
        inline iterator operator++(int) { Node *n = i; ++i; return n; }
        inline iterator &operator--() { i--; return *this; }
        inline iterator operator--(int) { Node *n = i; i--; return n; }
        inline iterator &operator+=(int j) { i+=j; return *this; }
        inline iterator &operator-=(int j) { i-=j; return *this; }
        inline iterator operator+(int j) const { return iterator(i+j); }
        inline iterator operator-(int j) const { return iterator(i-j); }
        inline int operator-(iterator j) const { return int(i - j.i); }
    };
    friend class iterator;

    class const_iterator {
    public:
        Node *i;
        typedef std::random_access_iterator_tag  iterator_category;
        typedef qptrdiff difference_type;
        typedef T value_type;
        typedef const T *pointer;
        typedef const T &reference;

        inline const_iterator() : i(nullptr) {}
        inline const_iterator(Node *n) : i(n) {}
        inline const_iterator(const const_iterator &o): i(o.i) {}
#ifdef QT_STRICT_ITERATORS
        inline explicit const_iterator(const iterator &o): i(o.i) {}
#else
        inline const_iterator(const iterator &o): i(o.i) {}
#endif
        inline const T &operator*() const { return i->t(); }
        inline const T *operator->() const { return &i->t(); }
        inline const T &operator[](int j) const { return i[j].t(); }
        inline bool operator==(const const_iterator &o) const { return i == o.i; }
        inline bool operator!=(const const_iterator &o) const { return i != o.i; }
        inline bool operator<(const const_iterator& other) const { return i < other.i; }
        inline bool operator<=(const const_iterator& other) const { return i <= other.i; }
        inline bool operator>(const const_iterator& other) const { return i > other.i; }
        inline bool operator>=(const const_iterator& other) const { return i >= other.i; }
        inline const_iterator &operator++() { ++i; return *this; }
        inline const_iterator operator++(int) { Node *n = i; ++i; return n; }
        inline const_iterator &operator--() { i--; return *this; }
        inline const_iterator operator--(int) { Node *n = i; i--; return n; }
        inline const_iterator &operator+=(int j) { i+=j; return *this; }
        inline const_iterator &operator-=(int j) { i-=j; return *this; }
        inline const_iterator operator+(int j) const { return const_iterator(i+j); }
        inline const_iterator operator-(int j) const { return const_iterator(i-j); }
        inline int operator-(const_iterator j) const { return i - j.i; }
    };
    friend class const_iterator;

    // stl style
    inline iterator begin() { detach(); return reinterpret_cast<Node *>(p.begin()); }
    inline const_iterator begin() const { return reinterpret_cast<Node *>(p.begin()); }
    inline const_iterator constBegin() const { return reinterpret_cast<Node *>(p.begin()); }
    inline iterator end() { detach(); return reinterpret_cast<Node *>(p.end()); }
    inline const_iterator end() const { return reinterpret_cast<Node *>(p.end()); }
    inline const_iterator constEnd() const { return reinterpret_cast<Node *>(p.end()); }
    iterator insert(iterator before, const T &t);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    // more Qt
    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    inline int count() const { return p.size(); }
    inline int length() const { return p.size(); } // Same as count()
    inline T& first() { Q_ASSERT(!isEmpty()); return *begin(); }
    inline const T& first() const { Q_ASSERT(!isEmpty()); return at(0); }
    T& last() { Q_ASSERT(!isEmpty()); return *(--end()); }
    const T& last() const { Q_ASSERT(!isEmpty()); return at(count() - 1); }
    inline void removeFirst() { Q_ASSERT(!isEmpty()); erase(begin()); }
    inline void removeLast() { Q_ASSERT(!isEmpty()); erase(--end()); }
    inline bool startsWith(const T &t) const { return !isEmpty() && first() == t; }
    inline bool endsWith(const T &t) const { return !isEmpty() && last() == t; }
    QList<T> mid(int pos, int length = -1) const;

    T value(int i) const;
    T value(int i, const T &defaultValue) const;

    // stl compatibility
    inline void push_back(const T &t) { append(t); }
    inline void push_front(const T &t) { prepend(t); }
    inline T& front() { return first(); }
    inline const T& front() const { return first(); }
    inline T& back() { return last(); }
    inline const T& back() const { return last(); }
    inline void pop_front() { removeFirst(); }
    inline void pop_back() { removeLast(); }
    inline bool empty() const { return isEmpty(); }
    typedef int size_type;
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef qptrdiff difference_type;


    // comfort
    QList<T> &operator+=(const QList<T> &l);
    inline QList<T> operator+(const QList<T> &l) const
    { QList n = *this; n += l; return n; }
    inline QList<T> &operator+=(const T &t)
    { append(t); return *this; }
    inline QList<T> &operator<< (const T &t)
    { append(t); return *this; }
    inline QList<T> &operator<<(const QList<T> &l)
    { *this += l; return *this; }

    QVector<T> toVector() const;
    QSet<T> toSet() const;

    static QList<T> fromVector(const QVector<T> &vector);
    static QList<T> fromSet(const QSet<T> &set);

private:
    inline void detach() { if (d->ref != 1) detach_helper(d->alloc); }
    void detach_helper(int alloc);

    void node_construct(Node *n, const T &t);
    void node_destruct(Node *n);
    void node_copy(Node *from, Node *to, Node *src);
    void node_destruct(Node *from, Node *to);
};

template <typename T>
Q_INLINE_TEMPLATE void QList<T>::node_construct(Node *n, const T &t)
{
    if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) n->v = new T(t);
    else if (QTypeInfo<T>::isComplex) new (n) T(t);
    // This is always safe, but penaltizes unoptimized builds a lot.
    else ::memcpy(n, &t, sizeof(T));
}

template <typename T>
Q_INLINE_TEMPLATE void QList<T>::node_destruct(Node *n)
{
    if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) delete reinterpret_cast<T*>(n->v);
    else if (QTypeInfo<T>::isComplex) reinterpret_cast<T*>(n)->~T();
}

template <typename T>
Q_INLINE_TEMPLATE void QList<T>::node_copy(Node *from, Node *to, Node *src)
{
    Node *current = from;
    if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) {
        while(current != to) {
            current->v = new T(*reinterpret_cast<T*>(src->v));
            ++current;
            ++src;
        }
    } else if (QTypeInfo<T>::isComplex) {
        while(current != to) {
            new (current) T(*reinterpret_cast<T*>(src));
            ++current;
            ++src;
        }
    } else {
        if (src != from && to - from > 0)
            ::memcpy(from, src, (to - from) * sizeof(Node *));
    }
}

template <typename T>
Q_INLINE_TEMPLATE void QList<T>::node_destruct(Node *from, Node *to)
{
    if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic)
        while(from != to) --to, delete reinterpret_cast<T*>(to->v);
    else if (QTypeInfo<T>::isComplex)
        while (from != to) --to, reinterpret_cast<T*>(to)->~T();
}

template <typename T>
Q_INLINE_TEMPLATE QList<T> &QList<T>::operator=(const QList<T> &l)
{
    if (d != l.d) {
        QListData::Data *o = l.d;
        o->ref.ref();
        if (!d->ref.deref()) {
            node_destruct(reinterpret_cast<Node *>(d->array + d->begin),
                          reinterpret_cast<Node *>(d->array + d->end));
            QListData::freeData(d);
        }
        d = o;
    }
    return *this;
}
template <typename T>
inline typename QList<T>::iterator QList<T>::insert(iterator before, const T &t)
{
    int iBefore = int(before.i - reinterpret_cast<Node *>(p.begin()));
    Node *n = reinterpret_cast<Node *>(p.insert(iBefore));
    node_construct(n, t);
    return n;
}
template <typename T>
inline typename QList<T>::iterator QList<T>::erase(iterator it)
{ node_destruct(it.i);
 return reinterpret_cast<Node *>(p.erase(reinterpret_cast<void**>(it.i))); }
template <typename T>
inline const T &QList<T>::at(int i) const
{ Q_ASSERT_X(i >= 0 && i < p.size(), "QList<T>::at", "index out of range");
 return reinterpret_cast<Node *>(p.at(i))->t(); }
template <typename T>
inline const T &QList<T>::operator[](int i) const
{ Q_ASSERT_X(i >= 0 && i < p.size(), "QList<T>::operator[]", "index out of range");
 return reinterpret_cast<Node *>(p.at(i))->t(); }
template <typename T>
inline T &QList<T>::operator[](int i)
{ Q_ASSERT_X(i >= 0 && i < p.size(), "QList<T>::operator[]", "index out of range");
  detach(); return reinterpret_cast<Node *>(p.at(i))->t(); }
template <typename T>
inline void QList<T>::removeAt(int i)
{ if(i >= 0 && i < p.size()) { detach();
 node_destruct(reinterpret_cast<Node *>(p.at(i))); p.remove(i); } }
template <typename T>
inline T QList<T>::takeAt(int i)
{ Q_ASSERT_X(i >= 0 && i < p.size(), "QList<T>::take", "index out of range");
 detach(); Node *n = reinterpret_cast<Node *>(p.at(i)); T t = n->t(); node_destruct(n);
 p.remove(i); return t; }
template <typename T>
inline T QList<T>::takeFirst()
{ T t = first(); removeFirst(); return t; }
template <typename T>
inline T QList<T>::takeLast()
{ T t = last(); removeLast(); return t; }

template <typename T>
Q_OUTOFLINE_TEMPLATE void QList<T>::reserve(int alloc)
{
    if (d->alloc < alloc) {
        detach_helper(alloc);
    }
}

template <typename T>
Q_OUTOFLINE_TEMPLATE void QList<T>::append(const T &t)
{
    detach();
    if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) {
        Node *n = reinterpret_cast<Node *>(p.append());
        node_construct(n, t);
    } else {
        Node copy;
        node_construct(&copy, t); // t might be a reference to an object in the array
        Node *n = reinterpret_cast<Node *>(p.append());;
        *n = copy;
    }
}

template <typename T>
inline void QList<T>::prepend(const T &t)
{
    detach();
    if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) {
        Node *n = reinterpret_cast<Node *>(p.prepend());
        node_construct(n, t);
    } else {
        Node copy;
        node_construct(&copy, t); // t might be a reference to an object in the array
        Node *n = reinterpret_cast<Node *>(p.prepend());;
        *n = copy;
    }
}

template <typename T>
inline void QList<T>::insert(int i, const T &t)
{
    detach();
    if (QTypeInfo<T>::isLarge || QTypeInfo<T>::isStatic) {
        Node *n = reinterpret_cast<Node *>(p.insert(i));
        node_construct(n, t);
    } else {
        Node copy;
        node_construct(&copy, t); // t might be a reference to an object in the array
        Node *n = reinterpret_cast<Node *>(p.insert(i));;
        *n = copy;
    }
}

template <typename T>
inline void QList<T>::replace(int i, const T &t)
{
    Q_ASSERT_X(i >= 0 && i < p.size(), "QList<T>::replace", "index out of range");
    detach();
    reinterpret_cast<Node *>(p.at(i))->t() = t;
}

template <typename T>
inline void QList<T>::swap(int i, int j)
{
    Q_ASSERT_X(i >= 0 && i < p.size() && j >= 0 && j < p.size(),
                "QList<T>::swap", "index out of range");
    detach();
    void *t = d->array[d->begin + i];
    d->array[d->begin + i] = d->array[d->begin + j];
    d->array[d->begin + j] = t;
}

template <typename T>
inline void QList<T>::move(int from, int to)
{
    Q_ASSERT_X(from >= 0 && from < p.size() && to >= 0 && to < p.size(),
               "QList<T>::move", "index out of range");
    detach();
    p.move(from, to);
}

template<typename T>
Q_OUTOFLINE_TEMPLATE QList<T> QList<T>::mid(int pos, int alength) const
{
    if (alength < 0 || pos + alength > size())
        alength = size() - pos;
    if (pos == 0 && alength == size())
        return *this;
    QList<T> cpy;
    if (alength <= 0)
        return cpy;
    cpy.reserve(alength);
    cpy.d->end = alength;
    cpy.node_copy(reinterpret_cast<Node *>(cpy.p.begin()),
                  reinterpret_cast<Node *>(cpy.p.end()),
                  reinterpret_cast<Node *>(p.begin() + pos));
    return cpy;
}

template<typename T>
Q_OUTOFLINE_TEMPLATE T QList<T>::value(int i) const
{
    if (i < 0 || i >= p.size()) {
        return T();
    }
    return reinterpret_cast<Node *>(p.at(i))->t();
}

template<typename T>
Q_OUTOFLINE_TEMPLATE T QList<T>::value(int i, const T& defaultValue) const
{
    return ((i < 0 || i >= p.size()) ? defaultValue : reinterpret_cast<Node *>(p.at(i))->t());
}

template <typename T>
Q_OUTOFLINE_TEMPLATE void QList<T>::detach_helper(int alloc)
{
    Node *n = reinterpret_cast<Node *>(p.begin());
    QListData::Data *x = p.detach(alloc);
    node_copy(reinterpret_cast<Node *>(p.begin()), reinterpret_cast<Node *>(p.end()), n);

    if (!x->ref.deref())
        QListData::freeData(x);
}

template <typename T>
Q_OUTOFLINE_TEMPLATE QList<T>::~QList()
{
    if (!d->ref.deref()) {
        node_destruct(reinterpret_cast<Node *>(d->array + d->begin),
                      reinterpret_cast<Node *>(d->array + d->end));
        QListData::freeData(d);
    }
}

template <typename T>
Q_OUTOFLINE_TEMPLATE bool QList<T>::operator==(const QList<T> &l) const
{
    if (p.size() != l.p.size())
        return false;
    if (d == l.d)
        return true;
    Node *i = reinterpret_cast<Node *>(p.end());
    Node *b = reinterpret_cast<Node *>(p.begin());
    Node *li = reinterpret_cast<Node *>(l.p.end());
    while (i != b) {
        --i; --li;
        if (!(i->t() == li->t()))
            return false;
    }
    return true;
}

template <typename T>
Q_OUTOFLINE_TEMPLATE void QList<T>::clear()
{
    *this = QList<T>();
}

template <typename T>
Q_OUTOFLINE_TEMPLATE int QList<T>::removeAll(const T &_t)
{
    int index = indexOf(_t);
    if (index == -1)
        return 0;

    const T t = _t;
    detach();

    Node *i = reinterpret_cast<Node *>(p.at(index));
    Node *e = reinterpret_cast<Node *>(p.end());
    Node *n = i;
    node_destruct(i);
    while (++i != e) {
        if (i->t() == t)
            node_destruct(i);
        else
            *n++ = *i;
    }

    int removedCount = e - n;
    d->end -= removedCount;
    return removedCount;
}

template <typename T>
Q_OUTOFLINE_TEMPLATE bool QList<T>::removeOne(const T &_t)
{
    int index = indexOf(_t);
    if (index != -1) {
        removeAt(index);
        return true;
    }
    return false;
}

template <typename T>
Q_OUTOFLINE_TEMPLATE typename QList<T>::iterator QList<T>::erase(typename QList<T>::iterator afirst,
                                                                 typename QList<T>::iterator alast)
{
    for (Node *n = afirst.i; n < alast.i; ++n)
        node_destruct(n);
    int idx = afirst - begin();
    p.remove(idx, alast - afirst);
    return begin() + idx;
}

template <typename T>
Q_OUTOFLINE_TEMPLATE QList<T> &QList<T>::operator+=(const QList<T> &l)
{
    if (!l.isEmpty()) {
        if (isEmpty()) {
            *this = l;
        } else {
            detach();
            Node *n = reinterpret_cast<Node *>(p.append(l.p));
            node_copy(n, reinterpret_cast<Node *>(p.end()),
                         reinterpret_cast<Node *>(l.p.begin()));
        }
    }
    return *this;
}

template <typename T>
inline void QList<T>::append(const QList<T> &t)
{
    *this += t;
}

template <typename T>
Q_OUTOFLINE_TEMPLATE int QList<T>::indexOf(const T &t, int from) const
{
    if (from < 0)
        from = qMax(from + p.size(), 0);
    if (from < p.size()) {
        Node *n = reinterpret_cast<Node *>(p.at(from -1));
        Node *e = reinterpret_cast<Node *>(p.end());
        while (++n != e)
            if (n->t() == t)
                return int(n - reinterpret_cast<Node *>(p.begin()));
    }
    return -1;
}

template <typename T>
Q_OUTOFLINE_TEMPLATE int QList<T>::lastIndexOf(const T &t, int from) const
{
    if (from < 0)
        from += p.size();
    else if (from >= p.size())
        from = p.size()-1;
    if (from >= 0) {
        Node *b = reinterpret_cast<Node *>(p.begin());
        Node *n = reinterpret_cast<Node *>(p.at(from + 1));
        while (n-- != b) {
            if (n->t() == t)
                return n - b;
        }
    }
    return -1;
}

template <typename T>
Q_OUTOFLINE_TEMPLATE bool QList<T>::contains(const T &t) const
{
    Node *b = reinterpret_cast<Node *>(p.begin());
    Node *i = reinterpret_cast<Node *>(p.end());
    while (i-- != b)
        if (i->t() == t)
            return true;
    return false;
}

template <typename T>
Q_OUTOFLINE_TEMPLATE int QList<T>::count(const T &t) const
{
    int c = 0;
    Node *b = reinterpret_cast<Node *>(p.begin());
    Node *i = reinterpret_cast<Node *>(p.end());
    while (i-- != b)
        if (i->t() == t)
            ++c;
    return c;
}

Q_DECLARE_SEQUENTIAL_ITERATOR(List)
Q_DECLARE_MUTABLE_SEQUENTIAL_ITERATOR(List)

QT_END_NAMESPACE

#endif // QLIST_H
