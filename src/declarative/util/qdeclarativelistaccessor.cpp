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

#include "qdeclarativelistaccessor_p.h"
#include "qdeclarativemetatype_p.h"
#include "qstringlist.h"
#include "qdebug.h"
#include "qdeclarativeengine_p.h"

QT_BEGIN_NAMESPACE

QDeclarativeListAccessor::QDeclarativeListAccessor()
: m_type(Invalid)
{
}

QDeclarativeListAccessor::~QDeclarativeListAccessor()
{
}

QVariant QDeclarativeListAccessor::list() const
{
    return d;
}

void QDeclarativeListAccessor::setList(const QVariant &v, QDeclarativeEngine *engine)
{
    d = v;

    QDeclarativeEnginePrivate *enginePrivate = engine?QDeclarativeEnginePrivate::get(engine):0;

    if (!d.isValid()) {
        m_type = Invalid;
    } else if (d.userType() == QVariant::StringList) {
        m_type = StringList;
    } else if (d.userType() == QMetaType::QVariantList) {
        m_type = VariantList;
    } else if (d.canConvert(QVariant::Int)) {
        m_type = Integer;
    } else if ((!enginePrivate && QDeclarativeMetaType::isQObject(d.userType())) ||
               (enginePrivate && enginePrivate->isQObject(d.userType()))) {
        QObject *data = enginePrivate?enginePrivate->toQObject(v):QDeclarativeMetaType::toQObject(v);
        d = QVariant::fromValue(data);
        m_type = Instance;
    } else if (d.userType() == qMetaTypeId<QDeclarativeListReference>()) {
        m_type = ListProperty;
    } else {
        m_type = Instance;
    }
}

int QDeclarativeListAccessor::count() const
{
    switch(m_type) {
    case StringList:
        return qvariant_cast<QStringList>(d).count();
    case VariantList:
        return qvariant_cast<QVariantList>(d).count();
    case ListProperty:
        return ((QDeclarativeListReference *)d.constData())->count();
    case Instance:
        return 1;
    case Integer:
        return d.toInt();
    default:
    case Invalid:
        return 0;
    }
}

QVariant QDeclarativeListAccessor::at(int idx) const
{
    Q_ASSERT(idx >= 0 && idx < count());
    switch(m_type) {
    case StringList:
        return QVariant::fromValue(qvariant_cast<QStringList>(d).at(idx));
    case VariantList:
        return qvariant_cast<QVariantList>(d).at(idx);
    case ListProperty:
        return QVariant::fromValue(((QDeclarativeListReference *)d.constData())->at(idx));
    case Instance:
        return d;
    case Integer:
        return QVariant(idx);
    default:
    case Invalid:
        return QVariant();
    }
}

bool QDeclarativeListAccessor::isValid() const
{
    return m_type != Invalid;
}

QT_END_NAMESPACE
