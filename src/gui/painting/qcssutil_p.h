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

#ifndef QCSSUTIL_P_H
#define QCSSUTIL_P_H

#include "QtCore/qglobal.h"

#ifndef QT_NO_CSSPARSER

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

#include "qcssparser_p.h"
#include "QtCore/qsize.h"

QT_BEGIN_NAMESPACE

class QPainter;

extern void qDrawEdge(QPainter *p, qreal x1, qreal y1, qreal x2, qreal y2, qreal dw1, qreal dw2,
                      QCss::Edge edge, QCss::BorderStyle style, QBrush c);

extern void qDrawRoundedCorners(QPainter *p, qreal x1, qreal y1, qreal x2, qreal y2,
                                const QSizeF& r1, const QSizeF& r2,
                                QCss::Edge edge, QCss::BorderStyle s, QBrush c);

extern void qDrawBorder(QPainter *p, const QRect &rect, const QCss::BorderStyle *styles,
                        const int *borders, const QBrush *colors, const QSize *radii);

extern void qNormalizeRadii(const QRect &br, const QSize *radii,
                            QSize *tlr, QSize *trr, QSize *blr, QSize *brr);

QT_END_NAMESPACE

#endif //QT_NO_CSSPARSER

#endif // QCSSUTIL_P_H
