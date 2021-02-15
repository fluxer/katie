/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtTest module of the Katie Toolkit.
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

#include <QVector>
#include <vector>
#include "qrawvector.h"

QVector<double> qvector_fill_and_return_helper()
{
    QVector<double> v(N);
    for (int i = 0; i != N; ++i)
        v[i] = i;
    return v;
}

QVector<double> qrawvector_fill_and_return_helper()
{
    QRawVector<double> v(N);
    for (int i = 0; i != N; ++i)
        v[i] = i;
    return v.mutateToVector();
}

QVector<double> mixedvector_fill_and_return_helper()
{
    std::vector<double> v(N);
    for (int i = 0; i != N; ++i)
        v[i] = i;
    return QVector<double>::fromStdVector(v);
}


std::vector<double> stdvector_fill_and_return_helper()
{
    std::vector<double> v(N);
    for (int i = 0; i != N; ++i)
        v[i] = i;
    return v;
}

