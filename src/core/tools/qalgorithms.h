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

#ifndef QALGORITHMS_H
#define QALGORITHMS_H

#include <QtCore/qglobal.h>

#include <algorithm>


QT_BEGIN_NAMESPACE

template <typename InputIterator, typename OutputIterator>
inline OutputIterator qCopy(InputIterator begin, InputIterator end, OutputIterator dest)
{
    return std::copy(begin, end, dest);
}

template <typename BiIterator1, typename BiIterator2>
inline BiIterator2 qCopyBackward(BiIterator1 begin, BiIterator1 end, BiIterator2 dest)
{
    return std::copy_backward(begin, end, dest);
}

template <typename InputIterator1, typename InputIterator2>
inline bool qEqual(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
    return std::equal(first1, last1, first2);
}

template <typename ForwardIterator, typename T>
inline void qFill(ForwardIterator first, ForwardIterator last, const T &val)
{
    return std::fill(first, last, val);
}

template <typename Container, typename T>
inline void qFill(Container &container, const T &val)
{
    return std::fill_n(container, val);
}

template <typename InputIterator, typename T>
inline InputIterator qFind(InputIterator first, InputIterator last, const T &val)
{
    return std::find(first, last, val);
}

template <typename Container, typename T>
inline typename Container::const_iterator qFind(const Container &container, const T &val)
{
    return std::find(container.constBegin(), container.constEnd(), val);
}

template <typename InputIterator, typename T, typename Size>
inline void qCount(InputIterator first, InputIterator last, const T &value, Size &n)
{
    n = std::count(first, last, value);
}

template <typename Container, typename T, typename Size>
inline void qCount(const Container &container, const T &value, Size &n)
{
    qCount(container.constBegin(), container.constEnd(), value, n);
}

template <typename T>
class qLess
{
public:
    inline bool operator()(const T &t1, const T &t2) const
    {
        return (t1 < t2);
    }
};

template <typename T>
class qGreater
{
public:
    inline bool operator()(const T &t1, const T &t2) const
    {
        return (t2 < t1);
    }
};

template <typename RandomAccessIterator>
inline void qSort(RandomAccessIterator start, RandomAccessIterator end)
{
    if (start != end)
        std::sort(start, end);
}

template <typename RandomAccessIterator, typename LessThan>
inline void qSort(RandomAccessIterator start, RandomAccessIterator end, LessThan lessThan)
{
    if (start != end)
        std::sort(start, end, lessThan);
}

template<typename Container>
inline void qSort(Container &c)
{
    if (!c.empty())
        std::sort(c.begin(), c.end());
}

template <typename RandomAccessIterator>
inline void qStableSort(RandomAccessIterator start, RandomAccessIterator end)
{
    if (start != end)
        std::stable_sort(start, end);
}

template <typename RandomAccessIterator, typename LessThan>
inline void qStableSort(RandomAccessIterator start, RandomAccessIterator end, LessThan lessThan)
{
    if (start != end)
        std::stable_sort(start, end, lessThan);
}

template<typename Container>
inline void qStableSort(Container &c)
{
    if (!c.empty())
        std::stable_sort(c.begin(), c.end());
}

template <typename RandomAccessIterator, typename T>
inline RandomAccessIterator qLowerBound(RandomAccessIterator begin, RandomAccessIterator end, const T &value)
{
    return std::lower_bound(begin, end, value);
}

template <typename RandomAccessIterator, typename T, typename LessThan>
inline RandomAccessIterator qLowerBound(RandomAccessIterator begin, RandomAccessIterator end, const T &value, LessThan lessThan)
{
    return std::lower_bound(begin, end, value, lessThan);
}

template <typename Container, typename T>
inline typename Container::const_iterator qLowerBound(const Container &container, const T &value)
{
    return std::lower_bound(container.constBegin(), container.constEnd(), value, qLess<T>());
}

template <typename RandomAccessIterator, typename T>
inline RandomAccessIterator qUpperBound(RandomAccessIterator begin, RandomAccessIterator end, const T &value)
{
    return std::upper_bound(begin, end, value);
}

template <typename RandomAccessIterator, typename T, typename LessThan>
inline RandomAccessIterator qUpperBound(RandomAccessIterator begin, RandomAccessIterator end, const T &value, LessThan lessThan)
{
    return std::upper_bound(begin, end, value, lessThan);
}

template <typename Container, typename T>
inline typename Container::const_iterator qUpperBound(const Container &container, const T &value)
{
    return std::upper_bound(container.constBegin(), container.constEnd(), value, qLess<T>());
}

template <typename RandomAccessIterator, typename T>
inline RandomAccessIterator qBinaryFind(RandomAccessIterator begin, RandomAccessIterator end, const T &value)
{
    RandomAccessIterator it = qLowerBound(begin, end, value);

    if (it == end || value < *it)
        return end;

    return it;
}

template <typename RandomAccessIterator, typename T, typename LessThan>
inline RandomAccessIterator qBinaryFind(RandomAccessIterator begin, RandomAccessIterator end, const T &value, LessThan lessThan)
{
    RandomAccessIterator it = qLowerBound(begin, end, value, lessThan);

    if (it == end || lessThan(value, *it))
        return end;

    return it;
}

template <typename Container, typename T>
inline typename Container::const_iterator qBinaryFind(const Container &container, const T &value)
{
    return qBinaryFind(container.constBegin(), container.constEnd(), value, qLess<T>());
}

template <typename ForwardIterator>
inline void qDeleteAll(ForwardIterator begin, ForwardIterator end)
{
    while (begin != end) {
        delete *begin;
        ++begin;
    }
}

template <typename Container>
inline void qDeleteAll(const Container &c)
{
    qDeleteAll(c.begin(), c.end());
}

QT_END_NAMESPACE


#endif // QALGORITHMS_H
