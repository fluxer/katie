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

#ifndef QTESSELATOR_P_H
#define QTESSELATOR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qpoint.h"
#include "qrect.h"

QT_BEGIN_NAMESPACE

class QTessellatorPrivate;

typedef int Q27Dot5;
#define Q27Dot5ToDouble(i) ((i)/32.)
#define FloatToQ27Dot5(i) (int)((i) * 32)
#define IntToQ27Dot5(i) ((i) << 5)
#define Q27Dot5ToXFixed(i) ((i) << 11)
#define Q27Dot5Factor 32

class Q_GUI_EXPORT QTessellator {
public:
    QTessellator();
    virtual ~QTessellator();

    QRectF tessellate(const QPointF *points, int nPoints);

    void setWinding(bool w);

    struct Vertex {
        Q27Dot5 x;
        Q27Dot5 y;
    };
    struct Trapezoid {
        Q27Dot5 top;
        Q27Dot5 bottom;
        const Vertex *topLeft;
        const Vertex *bottomLeft;
        const Vertex *topRight;
        const Vertex *bottomRight;
    };
    virtual void addTrap(const Trapezoid &trap) = 0;

private:
    QTessellatorPrivate *d;
};

QT_END_NAMESPACE

#endif
