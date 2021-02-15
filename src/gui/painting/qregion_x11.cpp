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

#include "qt_x11_p.h"

#include <limits.h>

QT_BEGIN_NAMESPACE

QRegion::QRegionData QRegion::shared_empty = { QAtomicInt(1), 0, 0, 0};

void QRegion::updateX11Region() const
{
    d->rgn = XCreateRegion();
    if (!d->qt_rgn)
        return;

    int n = d->qt_rgn->numRects;
    const QRect *rect = (n == 1 ? &d->qt_rgn->extents : d->qt_rgn->rects.constData());
    while (n--) {
        XRectangle r;
        r.x = qMax(SHRT_MIN, rect->x());
        r.y = qMax(SHRT_MIN, rect->y());
        r.width = qMin((int)USHRT_MAX, rect->width());
        r.height = qMin((int)USHRT_MAX, rect->height());
        XUnionRectWithRegion(&r, d->rgn, d->rgn);
        ++rect;
    }
}

void *QRegion::clipRectangles(int &num) const
{
    if (!d->xrectangles && !(d == &shared_empty || d->qt_rgn->numRects == 0)) {
        XRectangle *r = static_cast<XRectangle*>(malloc(d->qt_rgn->numRects * sizeof(XRectangle)));
        d->xrectangles = r;
        int n = d->qt_rgn->numRects;
        const QRect *rect = (n == 1 ? &d->qt_rgn->extents : d->qt_rgn->rects.constData());
        while (n--) {
            r->x = qMax(SHRT_MIN, rect->x());
            r->y = qMax(SHRT_MIN, rect->y());
            r->width = qMin((int)USHRT_MAX, rect->width());
            r->height = qMin((int)USHRT_MAX, rect->height());
            ++r;
            ++rect;
        }
    }
    if (d == &shared_empty || d->qt_rgn->numRects == 0)
        num = 0;
    else
        num = d->qt_rgn->numRects;
    return d->xrectangles;
}

QT_END_NAMESPACE
