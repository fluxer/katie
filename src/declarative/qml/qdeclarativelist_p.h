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

#ifndef QDECLARATIVELIST_P_H
#define QDECLARATIVELIST_P_H

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

#include "qdeclarativelist.h"
#include "qdeclarativeguard_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativeListReferencePrivate
{
public:
    QDeclarativeListReferencePrivate();

    static QDeclarativeListReference init(const QDeclarativeListProperty<QObject> &, int, QDeclarativeEngine *);

    QDeclarativeGuard<QObject> object;
    const QMetaObject *elementType;
    QDeclarativeListProperty<QObject> property;
    int propertyType;

    void addref();
    void release();
    int refCount;

    static inline QDeclarativeListReferencePrivate *get(QDeclarativeListReference *ref) {
        return ref->d;
    }
};


QT_END_NAMESPACE

#endif // QDECLARATIVELIST_P_H
