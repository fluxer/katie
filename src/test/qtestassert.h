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

#ifndef QTESTASSERT_H
#define QTESTASSERT_H

#include <QtCore/qglobal.h>


QT_BEGIN_NAMESPACE


#define QTEST_ASSERT(cond) do {if(!(cond))qt_assert(#cond,__FILE__,__LINE__);} while (0)

#define QTEST_ASSERT_X(cond, where, what) do {if(!(cond))qt_assert_x(where, what,__FILE__,__LINE__);} while (0)

QT_END_NAMESPACE


#endif
