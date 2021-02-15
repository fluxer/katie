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

#ifndef QDECLARATIVEPATH_P_H
#define QDECLARATIVEPATH_P_H

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

#include "qdeclarativepath_p.h"

#include "qdeclarative.h"

#include "qobject_p.h"

QT_BEGIN_NAMESPACE
class QDeclarativePathPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativePath)

public:
    QDeclarativePathPrivate() : startX(0), startY(0), closed(false), componentComplete(true) { }

    QPainterPath _path;
    QList<QDeclarativePathElement*> _pathElements;
    mutable QVector<QPointF> _pointCache;
    QList<QDeclarativePath::AttributePoint> _attributePoints;
    QStringList _attributes;
    int startX;
    int startY;
    bool closed;
    bool componentComplete;
};

QT_END_NAMESPACE
#endif
