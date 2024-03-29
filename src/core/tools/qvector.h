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

#ifndef QVECTOR_H
#define QVECTOR_H

#include <QtCore/qlist.h>

QT_BEGIN_NAMESPACE

struct Q_CORE_EXPORT QVectorData
{
    QAtomicInt ref;
    int alloc;
    int size;
    bool capacity;

    static QVectorData shared_null;
    static QVectorData *allocate(int size);
    static QVectorData *reallocate(QVectorData *old, int newsize);
    static void freeData(QVectorData *data);
    static int grow(int sizeofTypedData, int size, int sizeofT, bool excessive);
};

template <typename T>
struct QVectorTypedData : private QVectorData
{ // private inheritance as we must not access QVectorData member thought QVectorTypedData
  // as this would break strict aliasing rules. (in the case of shared_null)
    T array[1];

    static inline void freeTypeData(QVectorTypedData<T> *x) { QVectorData::freeData(static_cast<QVectorData *>(x)); }
};

template <typename T>
class Q_CORE_EXPORT QVector
{
    typedef QVectorTypedData<T> Data;
    union {
        QVectorData *d;
        Data *p;
    };

public:
    inline QVector() : d(&QVectorData::shared_null) { d->ref.ref(); }
    explicit QVector(int size);
    QVector(int size, const T &t);
    inline QVector(const QVector<T> &v) : d(v.d) { d->ref.ref(); }
    inline ~QVector() { if (!d->ref.deref()) freeData(p); }
    QVector<T> &operator=(const QVector<T> &v);
    inline QVector<T> operator=(QVector<T> &&other)
    { qSwap(p, other.p); return *this; }
    inline void swap(QVector<T> &other) { qSwap(d, other.d); }
    inline QVector(std::initializer_list<T> args);

    bool operator==(const QVector<T> &v) const;
    inline bool operator!=(const QVector<T> &v) const { return !(*this == v); }

    inline int size() const { return d->size; }

    inline bool isEmpty() const { return d->size == 0; }

    void resize(int size);

    inline int capacity() const { return d->alloc; }
    void reserve(int size);
    inline void squeeze() { reallocData(d->size, d->size); d->capacity = false; }

    inline T *data() { detach(); return p->array; }
    inline const T *data() const { return p->array; }
    inline const T *constData() const { return p->array; }
    void clear();

    const T &at(int i) const;
    T &operator[](int i);
    const T &operator[](int i) const;
    void append(const T &t);
    void prepend(const T &t);
    void insert(int i, const T &t);
    void insert(int i, int n, const T &t);
    void replace(int i, const T &t);
    void remove(int i);
    void remove(int i, int n);

    QVector<T> &fill(const T &t, int size = -1);

    int indexOf(const T &t, int from = 0) const;
    int lastIndexOf(const T &t, int from = -1) const;
    bool contains(const T &t) const;
    int count(const T &t) const;

#ifdef QT_STRICT_ITERATORS
    class iterator {
    public:
        T *i;
        typedef std::random_access_iterator_tag  iterator_category;
        typedef qptrdiff difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;

        inline iterator() : i(nullptr) {}
        inline iterator(T *n) : i(n) {}
        inline iterator(const iterator &o): i(o.i){}
        inline T &operator*() const { return *i; }
        inline T *operator->() const { return i; }
        inline T &operator[](int j) const { return *(i + j); }
        inline bool operator==(const iterator &o) const { return i == o.i; }
        inline bool operator!=(const iterator &o) const { return i != o.i; }
        inline bool operator<(const iterator& other) const { return i < other.i; }
        inline bool operator<=(const iterator& other) const { return i <= other.i; }
        inline bool operator>(const iterator& other) const { return i > other.i; }
        inline bool operator>=(const iterator& other) const { return i >= other.i; }
        inline iterator &operator++() { ++i; return *this; }
        inline iterator operator++(int) { T *n = i; ++i; return n; }
        inline iterator &operator--() { i--; return *this; }
        inline iterator operator--(int) { T *n = i; i--; return n; }
        inline iterator &operator+=(int j) { i+=j; return *this; }
        inline iterator &operator-=(int j) { i-=j; return *this; }
        inline iterator operator+(int j) const { return iterator(i+j); }
        inline iterator operator-(int j) const { return iterator(i-j); }
        inline int operator-(iterator j) const { return i - j.i; }
    };
    friend class iterator;

    class const_iterator {
    public:
        T *i;
        typedef std::random_access_iterator_tag  iterator_category;
        typedef qptrdiff difference_type;
        typedef T value_type;
        typedef const T *pointer;
        typedef const T &reference;

        inline const_iterator() : i(nullptr) {}
        inline const_iterator(T *n) : i(n) {}
        inline const_iterator(const const_iterator &o): i(o.i) {}
        inline explicit const_iterator(const iterator &o): i(o.i) {}
        inline const T &operator*() const { return *i; }
        inline const T *operator->() const { return i; }
        inline const T &operator[](int j) const { return *(i + j); }
        inline bool operator==(const const_iterator &o) const { return i == o.i; }
        inline bool operator!=(const const_iterator &o) const { return i != o.i; }
        inline bool operator<(const const_iterator& other) const { return i < other.i; }
        inline bool operator<=(const const_iterator& other) const { return i <= other.i; }
        inline bool operator>(const const_iterator& other) const { return i > other.i; }
        inline bool operator>=(const const_iterator& other) const { return i >= other.i; }
        inline const_iterator &operator++() { ++i; return *this; }
        inline const_iterator operator++(int) { T *n = i; ++i; return n; }
        inline const_iterator &operator--() { i--; return *this; }
        inline const_iterator operator--(int) { T *n = i; i--; return n; }
        inline const_iterator &operator+=(int j) { i+=j; return *this; }
        inline const_iterator &operator-=(int j) { i-=j; return *this; }
        inline const_iterator operator+(int j) const { return const_iterator(i+j); }
        inline const_iterator operator-(int j) const { return const_iterator(i-j); }
        inline int operator-(const_iterator j) const { return i - j.i; }
    };
    friend class const_iterator;
#else
    // STL-style
    typedef T* iterator;
    typedef const T* const_iterator;
#endif
    inline iterator begin() { detach(); return p->array; }
    inline const_iterator begin() const { return p->array; }
    inline const_iterator constBegin() const { return p->array; }
    inline iterator end() { detach(); return p->array + d->size; }
    inline const_iterator end() const { return p->array + d->size; }
    inline const_iterator constEnd() const { return p->array + d->size; }
    iterator insert(iterator before, int n, const T &x);
    inline iterator insert(iterator before, const T &x) { return insert(before, 1, x); }
    iterator erase(iterator begin, iterator end);
    inline iterator erase(iterator pos) { return erase(pos, pos+1); }

    // more Qt
    inline int count() const { return d->size; }
    inline T& first() { Q_ASSERT(!isEmpty()); return *begin(); }
    inline const T &first() const { Q_ASSERT(!isEmpty()); return *begin(); }
    inline T& last() { Q_ASSERT(!isEmpty()); return *(end()-1); }
    inline const T &last() const { Q_ASSERT(!isEmpty()); return *(end()-1); }
    inline bool startsWith(const T &t) const { return !isEmpty() && first() == t; }
    inline bool endsWith(const T &t) const { return !isEmpty() && last() == t; }
    QVector<T> mid(int pos, int length = -1) const;

    T value(int i) const;
    T value(int i, const T &defaultValue) const;

    // STL compatibility
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef qptrdiff difference_type;
    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    typedef int size_type;
    inline void push_back(const T &t) { append(t); }
    inline void push_front(const T &t) { prepend(t); }
    void pop_back() { Q_ASSERT(!isEmpty()); erase(end()-1); }
    void pop_front() { Q_ASSERT(!isEmpty()); erase(begin()); }
    inline bool empty() const
    { return d->size == 0; }
    inline T& front() { return first(); }
    inline const_reference front() const { return first(); }
    inline reference back() { return last(); }
    inline const_reference back() const { return last(); }

    // comfort
    QVector<T> &operator+=(const QVector<T> &l);
    inline QVector<T> operator+(const QVector<T> &l) const
    { QVector n = *this; n += l; return n; }
    inline QVector<T> &operator+=(const T &t)
    { append(t); return *this; }
    inline QVector<T> &operator<< (const T &t)
    { append(t); return *this; }
    inline QVector<T> &operator<<(const QVector<T> &l)
    { *this += l; return *this; }

    QList<T> toList() const;

    static QVector<T> fromList(const QList<T> &list);

private:
    inline void detach() { if (d->ref != 1) detach_helper(); }
    void detach_helper();
    QVectorData *mallocData(int alloc);
    void reallocData(int size, int alloc);
    void freeData(Data *d);
    int sizeOfTypedData() {
        // this is more or less the same as sizeof(Data), except that it doesn't
        // count the padding at the end
        return reinterpret_cast<const char *>(&(reinterpret_cast<const Data *>(this))->array[1]) - reinterpret_cast<const char *>(this);
    }
};

template <typename T>
void QVector<T>::detach_helper()
{ reallocData(d->size, d->alloc); }
template <typename T>
void QVector<T>::reserve(int asize)
{ if (asize > d->alloc) reallocData(d->size, asize); if (d->ref == 1) d->capacity = true; }
template <typename T>
void QVector<T>::resize(int asize)
{ reallocData(asize, (asize > d->alloc || (!d->capacity && asize < d->size && asize < (d->alloc >> 1))) ?
          QVectorData::grow(sizeOfTypedData(), asize, sizeof(T), QTypeInfo<T>::isStatic)
          : d->alloc); }
template <typename T>
inline void QVector<T>::clear()
{ *this = QVector<T>(); }
template <typename T>
inline const T &QVector<T>::at(int i) const
{ Q_ASSERT_X(i >= 0 && i < d->size, "QVector<T>::at", "index out of range");
  return p->array[i]; }
template <typename T>
inline const T &QVector<T>::operator[](int i) const
{ Q_ASSERT_X(i >= 0 && i < d->size, "QVector<T>::operator[]", "index out of range");
  return p->array[i]; }
template <typename T>
inline T &QVector<T>::operator[](int i)
{ Q_ASSERT_X(i >= 0 && i < d->size, "QVector<T>::operator[]", "index out of range");
  return data()[i]; }
template <typename T>
inline void QVector<T>::insert(int i, const T &t)
{ Q_ASSERT_X(i >= 0 && i <= d->size, "QVector<T>::insert", "index out of range");
  insert(begin() + i, 1, t); }
template <typename T>
inline void QVector<T>::insert(int i, int n, const T &t)
{ Q_ASSERT_X(i >= 0 && i <= d->size, "QVector<T>::insert", "index out of range");
  insert(begin() + i, n, t); }
template <typename T>
inline void QVector<T>::remove(int i, int n)
{ Q_ASSERT_X(i >= 0 && n >= 0 && i + n <= d->size, "QVector<T>::remove", "index out of range");
  erase(begin() + i, begin() + i + n); }
template <typename T>
inline void QVector<T>::remove(int i)
{ Q_ASSERT_X(i >= 0 && i < d->size, "QVector<T>::remove", "index out of range");
  erase(begin() + i, begin() + i + 1); }
template <typename T>
inline void QVector<T>::prepend(const T &t)
{ insert(begin(), 1, t); }

template <typename T>
inline void QVector<T>::replace(int i, const T &t)
{
    Q_ASSERT_X(i >= 0 && i < d->size, "QVector<T>::replace", "index out of range");
    data()[i] = t;
}

template <typename T>
QVector<T> &QVector<T>::operator=(const QVector<T> &v)
{
    QVectorData *o = v.d;
    o->ref.ref();
    if (!d->ref.deref())
        freeData(p);
    d = o;
    return *this;
}

template <typename T>
inline QVectorData *QVector<T>::mallocData(int aalloc)
{
    QVectorData *vectordata = QVectorData::allocate(sizeOfTypedData() + (aalloc - 1) * sizeof(T));
    Q_CHECK_PTR(vectordata);
    return vectordata;
}

template <typename T>
QVector<T>::QVector(int asize)
{
    d = mallocData(asize);
    d->ref = 1;
    d->alloc = d->size = asize;
    d->capacity = false;
    if (QTypeInfo<T>::isComplex) {
        T* b = p->array;
        T* i = p->array + d->size;
        while (i != b)
            new (--i) T();
    } else {
        memset(p->array, 0, asize * sizeof(T));
    }
}

template <typename T>
QVector<T>::QVector(int asize, const T &t)
{
    d = mallocData(asize);
    d->ref = 1;
    d->alloc = d->size = asize;
    d->capacity = false;
    T* i = p->array + d->size;
    while (i != p->array)
        new (--i) T(t);
}

template <typename T>
QVector<T>::QVector(std::initializer_list<T> args)
{
    d = mallocData(int(args.size()));
    d->ref = 1;
    d->alloc = d->size = int(args.size());
    d->capacity = false;
    T* i = p->array + d->size;
    auto it = args.end();
    while (i != p->array)
        new (--i) T(*(--it));
}

template <typename T>
void QVector<T>::freeData(Data *x)
{
    if (QTypeInfo<T>::isComplex) {
        T* b = x->array;
        union { QVectorData *d; Data *p; } u;
        u.p = x;
        T* i = b + u.d->size;
        while (i-- != b)
             i->~T();
    }
    x->freeTypeData(x);
}

template <typename T>
void QVector<T>::reallocData(int asize, int aalloc)
{
    Q_ASSERT(asize <= aalloc);
    T *pOld;
    T *pNew;
    union { QVectorData *d; Data *p; } x;
    x.d = d;

    if (QTypeInfo<T>::isComplex && asize < d->size && d->ref == 1 ) {
        // call the destructor on all objects that need to be
        // destroyed when shrinking
        pOld = p->array + d->size;
        pNew = p->array + asize;
        while (asize < d->size) {
            (--pOld)->~T();
            d->size--;
        }
    }

    if (aalloc != d->alloc || d->ref != 1) {
        // (re)allocate memory
        if (QTypeInfo<T>::isStatic) {
            x.d = mallocData(aalloc);
            Q_CHECK_PTR(x.p);
            x.d->size = 0;
        } else if (d->ref != 1) {
            x.d = mallocData(aalloc);
            Q_CHECK_PTR(x.p);
            if (QTypeInfo<T>::isComplex) {
                x.d->size = 0;
            } else {
                ::memcpy(x.p, p, sizeOfTypedData() + (qMin(aalloc, d->alloc) - 1) * sizeof(T));
                x.d->size = d->size;
            }
        } else {
            QVectorData *mem = QVectorData::reallocate(d, sizeOfTypedData() + (aalloc - 1) * sizeof(T));
            Q_CHECK_PTR(mem);
            x.d = d = mem;
            x.d->size = d->size;
        }
        x.d->ref = 1;
        x.d->alloc = aalloc;
        x.d->capacity = d->capacity;
    }

    if (QTypeInfo<T>::isComplex) {
        pOld = p->array + x.d->size;
        pNew = x.p->array + x.d->size;
        // copy objects from the old array into the new array
        const int toMove = qMin(asize, d->size);
        while (x.d->size < toMove) {
            new (pNew++) T(*pOld++);
            x.d->size++;
        }
        // construct all new objects when growing
        while (x.d->size < asize) {
            new (pNew++) T;
            x.d->size++;
        }

    } else if (asize > x.d->size) {
        // initialize newly allocated memory to 0
        memset(x.p->array + x.d->size, 0, (asize - x.d->size) * sizeof(T));
    }
    x.d->size = asize;

    if (d != x.d) {
        if (!d->ref.deref())
            freeData(p);
        d = x.d;
    }
}

template<typename T>
Q_OUTOFLINE_TEMPLATE T QVector<T>::value(int i) const
{
    if (i < 0 || i >= d->size) {
        return T();
    }
    return p->array[i];
}
template<typename T>
Q_OUTOFLINE_TEMPLATE T QVector<T>::value(int i, const T &defaultValue) const
{
    return ((i < 0 || i >= d->size) ? defaultValue : p->array[i]);
}

template <typename T>
void QVector<T>::append(const T &t)
{
    if (d->ref != 1 || d->size + 1 > d->alloc) {
        reallocData(d->size, QVectorData::grow(sizeOfTypedData(), d->size + 1, sizeof(T),
                                           QTypeInfo<T>::isStatic));
        if (QTypeInfo<T>::isComplex)
            new (p->array + d->size) T(t);
        else
            p->array[d->size] = t;
    } else {
        if (QTypeInfo<T>::isComplex)
            new (p->array + d->size) T(t);
        else
            p->array[d->size] = t;
    }
    ++d->size;
}

template <typename T>
typename QVector<T>::iterator QVector<T>::insert(iterator before, size_type n, const T &t)
{
    int offset = int(before - p->array);
    if (n != 0) {
        if (d->ref != 1 || d->size + n > d->alloc)
            reallocData(d->size, QVectorData::grow(sizeOfTypedData(), d->size + n, sizeof(T),
                                               QTypeInfo<T>::isStatic));
        if (QTypeInfo<T>::isStatic) {
            T *b = p->array + d->size;
            T *i = p->array + d->size + n;
            while (i != b)
                new (--i) T();
            i = p->array + d->size;
            T *j = i + n;
            b = p->array + offset;
            while (i != b)
                *--j = *--i;
            i = b+n;
            while (i != b)
                *--i = t;
        } else {
            T *b = p->array + offset;
            T *i = b + n;
            memmove(i, b, (d->size - offset) * sizeof(T));
            while (i != b)
                new (--i) T(t);
        }
        d->size += n;
    }
    return p->array + offset;
}

template <typename T>
typename QVector<T>::iterator QVector<T>::erase(iterator abegin, iterator aend)
{
    int f = int(abegin - p->array);
    int l = int(aend - p->array);
    int n = l - f;
    detach();
    if (QTypeInfo<T>::isComplex) {
        qCopy(p->array+l, p->array+d->size, p->array+f);
        T *i = p->array+d->size;
        T* b = p->array+d->size-n;
        while (i != b) {
            --i;
            i->~T();
        }
    } else {
        memmove(p->array + f, p->array + l, (d->size-l)*sizeof(T));
    }
    d->size -= n;
    return p->array + f;
}

template <typename T>
bool QVector<T>::operator==(const QVector<T> &v) const
{
    if (d->size != v.d->size)
        return false;
    if (d == v.d)
        return true;
    T* b = p->array;
    T* i = b + d->size;
    T* j = v.p->array + d->size;
    while (i != b)
        if (!(*--i == *--j))
            return false;
    return true;
}

template <typename T>
QVector<T> &QVector<T>::fill(const T &from, int asize)
{
    resize(asize < 0 ? d->size : asize);
    if (d->size) {
        T *i = p->array + d->size;
        T *b = p->array;
        while (i != b)
            *--i = from;
    }
    return *this;
}

template <typename T>
QVector<T> &QVector<T>::operator+=(const QVector &l)
{
    int newSize = d->size + l.d->size;
    reallocData(d->size, newSize);

    T *w = p->array + newSize;
    T *i = l.p->array + l.d->size;
    T *b = l.p->array;
    while (i != b) {
        if (QTypeInfo<T>::isComplex)
            new (--w) T(*--i);
        else
            *--w = *--i;
    }
    d->size = newSize;
    return *this;
}

template <typename T>
int QVector<T>::indexOf(const T &t, int from) const
{
    if (from < 0)
        from = qMax(from + d->size, 0);
    if (from < d->size) {
        T* n = p->array + from - 1;
        T* e = p->array + d->size;
        while (++n != e)
            if (*n == t)
                return n - p->array;
    }
    return -1;
}

template <typename T>
int QVector<T>::lastIndexOf(const T &t, int from) const
{
    if (from < 0)
        from += d->size;
    else if (from >= d->size)
        from = d->size-1;
    if (from >= 0) {
        T* b = p->array;
        T* n = p->array + from + 1;
        while (n != b) {
            if (*--n == t)
                return n - b;
        }
    }
    return -1;
}

template <typename T>
bool QVector<T>::contains(const T &t) const
{
    T* b = p->array;
    T* i = p->array + d->size;
    while (i != b)
        if (*--i == t)
            return true;
    return false;
}

template <typename T>
int QVector<T>::count(const T &t) const
{
    int c = 0;
    T* b = p->array;
    T* i = p->array + d->size;
    while (i != b)
        if (*--i == t)
            ++c;
    return c;
}

template <typename T>
Q_OUTOFLINE_TEMPLATE QVector<T> QVector<T>::mid(int pos, int length) const
{
    if (length < 0)
        length = size() - pos;
    if (pos == 0 && length == size())
        return *this;
    if (pos + length > size())
        length = size() - pos;
    QVector<T> copy;
    copy.reserve(length);
    for (int i = pos; i < pos + length; ++i)
        copy += at(i);
    return copy;
}

template <typename T>
Q_OUTOFLINE_TEMPLATE QList<T> QVector<T>::toList() const
{
    QList<T> result;
    result.reserve(size());
    for (int i = 0; i < size(); ++i)
        result.append(at(i));
    return result;
}

template <typename T>
Q_OUTOFLINE_TEMPLATE QVector<T> QList<T>::toVector() const
{
    QVector<T> result(size());
    for (int i = 0; i < size(); ++i)
        result[i] = at(i);
    return result;
}

template <typename T>
QVector<T> QVector<T>::fromList(const QList<T> &list)
{
    return list.toVector();
}

template <typename T>
QList<T> QList<T>::fromVector(const QVector<T> &vector)
{
    return vector.toList();
}

Q_DECLARE_SEQUENTIAL_ITERATOR(Vector)
Q_DECLARE_MUTABLE_SEQUENTIAL_ITERATOR(Vector)

QT_END_NAMESPACE


#endif // QVECTOR_H
