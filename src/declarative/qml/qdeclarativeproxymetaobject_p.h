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

#ifndef QDECLARATIVEPROXYMETAOBJECT_P_H
#define QDECLARATIVEPROXYMETAOBJECT_P_H

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

#include "qmetaobjectbuilder_p.h"
#include "qdeclarative.h"

#include <QtCore/QMetaObject>
#include <QtCore/QObject>

#include "qobject_p.h"


QT_BEGIN_NAMESPACE


class QDeclarativeProxyMetaObject : public QAbstractDynamicMetaObject
{
public:
    struct ProxyData {
        typedef QObject *(*CreateFunc)(QObject *);
        QMetaObject *metaObject;
        CreateFunc createFunc;
        int propertyOffset;
        int methodOffset;
    };

    QDeclarativeProxyMetaObject(QObject *, QList<ProxyData> *);
    virtual ~QDeclarativeProxyMetaObject();

protected:
    virtual int metaCall(QMetaObject::Call _c, int _id, void **_a);

private:
    QList<ProxyData> *metaObjects;
    QObject **proxies;

    QAbstractDynamicMetaObject *parent;
    QObject *object;
};

QT_END_NAMESPACE


#endif // QDECLARATIVEPROXYMETAOBJECT_P_H

