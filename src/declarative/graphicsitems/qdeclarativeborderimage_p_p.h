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

#ifndef QDECLARATIVEBORDERIMAGE_P_H
#define QDECLARATIVEBORDERIMAGE_P_H

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

#include "qdeclarativeimagebase_p_p.h"
#include "qdeclarativescalegrid_p_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativeBorderImagePrivate : public QDeclarativeImageBasePrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeBorderImage)

public:
    QDeclarativeBorderImagePrivate()
      : border(0),
        horizontalTileMode(QDeclarativeBorderImage::Stretch),
        verticalTileMode(QDeclarativeBorderImage::Stretch)
    {
    }

    ~QDeclarativeBorderImagePrivate()
    {
    }


    QDeclarativeScaleGrid *getScaleGrid()
    {
        Q_Q(QDeclarativeBorderImage);
        if (!border) {
            border = new QDeclarativeScaleGrid(q);
            static int borderChangedSignalIdx = -1;
            static int doUpdateSlotIdx = -1;
            if (borderChangedSignalIdx < 0)
                borderChangedSignalIdx = QDeclarativeScaleGrid::staticMetaObject.indexOfSignal("borderChanged()");
            if (doUpdateSlotIdx < 0)
                doUpdateSlotIdx = QDeclarativeBorderImage::staticMetaObject.indexOfSlot("doUpdate()");
            QMetaObject::connect(border, borderChangedSignalIdx, q, doUpdateSlotIdx);
        }
        return border;
    }

    QDeclarativeScaleGrid *border;
    QDeclarativeBorderImage::TileMode horizontalTileMode;
    QDeclarativeBorderImage::TileMode verticalTileMode;
};

QT_END_NAMESPACE

#endif // QDECLARATIVEBORDERIMAGE_P_H
