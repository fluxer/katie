/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVENULLABLEVALUE_P_H
#define QDECLARATIVENULLABLEVALUE_P_H

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

QT_BEGIN_NAMESPACE

template<typename T>
struct QDeclarativeNullableValue 
{
    QDeclarativeNullableValue()
    : isNull(true), value(T()) {}
    QDeclarativeNullableValue(const QDeclarativeNullableValue<T> &o)
    : isNull(o.isNull), value(o.value) {}
    QDeclarativeNullableValue(const T &t)
    : isNull(false), value(t) {}
    QDeclarativeNullableValue<T> &operator=(const T &t)
    { isNull = false; value = t; return *this; }
    QDeclarativeNullableValue<T> &operator=(const QDeclarativeNullableValue<T> &o)
    { isNull = o.isNull; value = o.value; return *this; }
    operator T() const { return value; }

    void invalidate() { isNull = true; }
    bool isValid() const { return !isNull; }
    bool isNull;
    T value;
};

QT_END_NAMESPACE

#endif // QDECLARATIVENULLABLEVALUE_P_H
