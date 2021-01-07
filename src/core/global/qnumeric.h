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

#ifndef QNUMERIC_H
#define QNUMERIC_H

#include <QtCore/qglobal.h>

#include <cmath>
#include <limits>


QT_BEGIN_NAMESPACE

inline bool qIsInf(double d) { return std::isinf(d); }
inline bool qIsNaN(double d) { return std::isnan(d); }
inline bool qIsFinite(double d) { return std::isfinite(d); }
inline bool qIsInf(float f) { return std::isinf(f); }
inline bool qIsNaN(float f) { return std::isnan(f); }
inline bool qIsFinite(float f) { return std::isfinite(f); }

inline double qSNaN() { return std::numeric_limits<double>::signaling_NaN(); }
inline double qQNaN() { return std::numeric_limits<double>::quiet_NaN(); }
inline double qInf() { return std::numeric_limits<double>::infinity(); }

QT_END_NAMESPACE


#endif // QNUMERIC_H
