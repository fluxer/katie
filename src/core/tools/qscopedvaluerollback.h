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

#ifndef QSCOPEDVALUEROLLBACK_H
#define QSCOPEDVALUEROLLBACK_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

template <typename T>
class Q_CORE_EXPORT QScopedValueRollback
{
public:
    QScopedValueRollback(T &var) :
        varRef(var)
    {
        oldValue = varRef;
    }

    ~QScopedValueRollback()
    {
        varRef = oldValue;
    }

    void commit()
    {
        oldValue = varRef;
    }

private:
    T& varRef;
    T oldValue;

    Q_DISABLE_COPY(QScopedValueRollback)
};

QT_END_NAMESPACE

#endif // QSCOPEDVALUEROLLBACK_H
