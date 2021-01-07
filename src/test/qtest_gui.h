/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#ifndef QTEST_GUI_H
#define QTEST_GUI_H

// enable GUI features
#ifndef QT_GUI_LIB
#define QT_GUI_LIB
#endif

#include <QtTest/qtestassert.h>
#include <QtTest/qtest.h>
#include <QtTest/qtestevent.h>
#include <QtTest/qtestmouse.h>
#include <QtTest/qtesttouch.h>
#include <QtTest/qtestkeyboard.h>
#include <QtGui/qicon.h>
#include <QtGui/qpixmap.h>


QT_BEGIN_NAMESPACE


namespace QTest
{

template<>
inline bool qCompare(QIcon const &t1, QIcon const &t2, const char *actual, const char *expected,
                    const char *file, int line)
{
    QTEST_ASSERT(sizeof(QIcon) == sizeof(void *));
    return qCompare<void *>(*reinterpret_cast<void * const *>(&t1),
                   *reinterpret_cast<void * const *>(&t2), actual, expected, file, line);
}

template<>
inline bool qCompare(QPixmap const &t1, QPixmap const &t2, const char *actual, const char *expected,
                    const char *file, int line)
{
    return qCompare(t1.toImage(), t2.toImage(), actual, expected, file, line);
}

}

#ifdef Q_WS_X11
extern void qt_x11_wait_for_window_manager(QWidget *w);
#endif

QT_END_NAMESPACE


#endif
