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

#ifndef QPAINTENGINE_P_H
#define QPAINTENGINE_P_H

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

#include "QtGui/qpainter.h"
#include "QtGui/qpaintengine.h"
#include "QtGui/qregion.h"

QT_BEGIN_NAMESPACE

class QTextItemInt;
class QPaintDevice;

class QPaintEnginePrivate
{
    Q_DECLARE_PUBLIC(QPaintEngine)
public:
    QPaintEnginePrivate() : pdev(nullptr), q_ptr(nullptr), currentClipWidget(nullptr),
                            hasSystemTransform(false), hasSystemViewport(false) {}
    virtual ~QPaintEnginePrivate() { }
    QPaintDevice *pdev;
    QPaintEngine *q_ptr;
    QRegion systemClip;
    QRect systemRect;
    QRegion systemViewport;
    QTransform systemTransform;
    QWidget *currentClipWidget;
    bool hasSystemTransform;
    bool hasSystemViewport;

    inline void transformSystemClip()
    {
        if (systemClip.isEmpty())
            return;

        if (hasSystemTransform) {
            if (systemTransform.type() <= QTransform::TxTranslate)
                systemClip.translate(qRound(systemTransform.dx()), qRound(systemTransform.dy()));
            else
                systemClip = systemTransform.map(systemClip);
        }

        // Make sure we're inside the viewport.
        if (hasSystemViewport) {
            systemClip &= systemViewport;
            if (systemClip.isEmpty()) {
                // We don't want to paint without system clip, so set it to 1 pixel :)
                systemClip = QRect(systemViewport.boundingRect().topLeft(), QSize(1, 1));
            }
        }
    }

    inline void setSystemTransform(const QTransform &xform)
    {
        systemTransform = xform;
        if ((hasSystemTransform = !xform.isIdentity()) || hasSystemViewport)
            transformSystemClip();
        systemStateChanged();
    }

    inline void setSystemViewport(const QRegion &region)
    {
        systemViewport = region;
        hasSystemViewport = !systemViewport.isEmpty();
    }

    virtual void systemStateChanged() { }

    void drawBoxTextItem(const QPointF &p, const QTextItemInt &ti);
};

QT_END_NAMESPACE

#endif // QPAINTENGINE_P_H
