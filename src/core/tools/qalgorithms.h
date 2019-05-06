/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#ifndef QALGORITHMS_H
#define QALGORITHMS_H

#include <QtCore/qglobal.h>

QT_BEGIN_HEADER

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
Q_OUTOFLINE_TEMPLATE RandomAccessIterator qLowerBound(RandomAccessIterator begin, RandomAccessIterator end, const T &value)
{
    // Implementation is duplicated from QAlgorithmsPrivate to keep existing code
    // compiling. We have to allow using *begin and value with different types,
    // and then implementing operator< for those types.
    RandomAccessIterator middle;
    int n = end - begin;
    int half;

    while (n > 0) {
        half = n >> 1;
        middle = begin + half;
        if (*middle < value) {
            begin = middle + 1;
            n -= half + 1;
        } else {
            n = half;
        }
    }
    return begin;
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
Q_OUTOFLINE_TEMPLATE RandomAccessIterator qUpperBound(RandomAccessIterator begin, RandomAccessIterator end, const T &value)
{
    // Implementation is duplicated from QAlgorithmsPrivate.
    RandomAccessIterator middle;
    int n = end - begin;
    int half;

    while (n > 0) {
        half = n >> 1;
        middle = begin + half;
        if (value < *middle) {
            n = half;
        } else {
            begin = middle + 1;
            n -= half + 1;
        }
    }
    return begin;
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

template <typename RandomAccessIterator, typename T, typename LessThan>
inline RandomAccessIterator qBinaryFindHelper(RandomAccessIterator begin, RandomAccessIterator end, const T &value, LessThan lessThan)
{
    RandomAccessIterator it = qLowerBound(begin, end, value, lessThan);

    if (it == end || lessThan(value, *it))
        return end;

    return it;
}

template <typename RandomAccessIterator, typename T>
inline RandomAccessIterator qBinaryFind(RandomAccessIterator begin, RandomAccessIterator end, const T &value)
{
    // Implementation is duplicated from QAlgorithmsPrivate.
    RandomAccessIterator it = qLowerBound(begin, end, value);

    if (it == end || value < *it)
        return end;

    return it;
}

template <typename RandomAccessIterator, typename T, typename LessThan>
inline RandomAccessIterator qBinaryFind(RandomAccessIterator begin, RandomAccessIterator end, const T &value, LessThan lessThan)
{
    return qBinaryFindHelper(begin, end, value, lessThan);
}

template <typename Container, typename T>
inline typename Container::const_iterator qBinaryFind(const Container &container, const T &value)
{
    return qBinaryFindHelper(container.constBegin(), container.constEnd(), value, qLess<T>());
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

/*
    Warning: The contents of QAlgorithmsPrivate is not a part of the public Qt API
    and may be changed from version to version or even be completely removed.
*/
namespace QAlgorithmsPrivate {

template <typename RandomAccessIterator>
inline void qReverse(RandomAccessIterator begin, RandomAccessIterator end)
{
    --end;
    while (begin < end)
        qSwap(*begin++, *end--);
}

template <typename RandomAccessIterator>
inline void qRotate(RandomAccessIterator begin, RandomAccessIterator middle, RandomAccessIterator end)
{
    qReverse(begin, middle);
    qReverse(middle, end);
    qReverse(begin, end);
}

template <typename RandomAccessIterator, typename T, typename LessThan>
Q_OUTOFLINE_TEMPLATE void qMerge(RandomAccessIterator begin, RandomAccessIterator pivot, RandomAccessIterator end, T &t, LessThan lessThan)
{
    const int len1 = pivot - begin;
    const int len2 = end - pivot;

    if (len1 == 0 || len2 == 0)
        return;

    if (len1 + len2 == 2) {
        if (lessThan(*(begin + 1), *(begin)))
            qSwap(*begin, *(begin + 1));
        return;
    }

    RandomAccessIterator firstCut;
    RandomAccessIterator secondCut;
    int len2Half;
    if (len1 > len2) {
        const int len1Half = len1 / 2;
        firstCut = begin + len1Half;
        secondCut = qLowerBound(pivot, end, *firstCut, lessThan);
        len2Half = secondCut - pivot;
    } else {
        len2Half = len2 / 2;
        secondCut = pivot + len2Half;
        firstCut = qUpperBound(begin, pivot, *secondCut, lessThan);
    }

    qRotate(firstCut, pivot, secondCut);
    const RandomAccessIterator newPivot = firstCut + len2Half;
    qMerge(begin, firstCut, newPivot, t, lessThan);
    qMerge(newPivot, secondCut, end, t, lessThan);
}

} //namespace QAlgorithmsPrivate

QT_END_NAMESPACE

QT_END_HEADER

#endif // QALGORITHMS_H
