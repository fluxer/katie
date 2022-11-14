/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QCURSOR_P_H
#define QCURSOR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of a number of Qt sources files.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/qatomic.h"
#include "QtCore/qnamespace.h"
#include "QtGui/qpixmap.h"
#include "QtGui/qbitmap.h"

#if defined(Q_WS_X11)
#  include "qt_x11_p.h"
#endif

#ifndef QT_NO_CURSOR

QT_BEGIN_NAMESPACE

class QCursorData {
public:
    QCursorData(Qt::CursorShape s);
    ~QCursorData();

    QAtomicInt ref;
    Qt::CursorShape cshape;
    QPixmap px;
    QBitmap bm;
    short hx, hy;
#if defined(Q_WS_X11)
    XColor fg, bg;
    Cursor hcurs;
    Qt::HANDLE x11px;
    Qt::HANDLE x11bm;
#ifndef QT_NO_XRENDER
    Picture x11pic;
#endif
#endif
};

QT_END_NAMESPACE

#endif // QT_NO_CURSOR

#endif // QCURSOR_P_H
