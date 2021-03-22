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

#ifndef QMATH_H
#define QMATH_H

#include <QtCore/qglobal.h>

#ifndef _USE_MATH_DEFINES
#  define _USE_MATH_DEFINES
#endif

#include <cmath>


QT_BEGIN_NAMESPACE

inline int qRound(qreal d)
{
    return int(std::round(d));
}

inline qint64 qRound64(qreal d)
{
    return qint64(std::round(d));
}

inline int qCeil(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return int(ceilf(v));
#else
    return int(ceil(v));
#endif
}

inline int qFloor(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return int(floorf(v));
#else
    return int(floor(v));
#endif
}

inline qreal qFabs(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return fabsf(v);
#else
    return fabs(v);
#endif
}

inline qreal qSin(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return sinf(v);
#else
    return sin(v);
#endif
}

inline qreal qCos(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return cosf(v);
#else
    return cos(v);
#endif
}

inline qreal qTan(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return tanf(v);
#else
    return tan(v);
#endif
}

inline qreal qAcos(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return acosf(v);
#else
    return acos(v);
#endif
}

inline qreal qAsin(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return asinf(v);
#else
    return asin(v);
#endif
}

inline qreal qAtan(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return atanf(v);
#else
    return atan(v);
#endif
}

inline qreal qAtan2(qreal x, qreal y)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return atan2f(x, y);
#else
    return atan2(x, y);
#endif
}

inline qreal qSqrt(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return sqrtf(v);
#else
    return sqrt(v);
#endif
}

inline qreal qLn(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return logf(v);
#else
    return log(v);
#endif
}

inline qreal qExp(qreal v)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return expf(v);
#else
    return exp(v);
#endif
}

inline qreal qPow(qreal x, qreal y)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return powf(x, y);
#else
    return pow(x, y);
#endif
}


inline qreal qFmod(qreal x, qreal y)
{
#if defined(QT_USE_MATH_H_FLOATS) && defined(QT_NO_FPU)
    return fmodf(x, y);
#else
    return fmod(x, y);
#endif
}

QT_END_NAMESPACE

#endif // QMATH_H
