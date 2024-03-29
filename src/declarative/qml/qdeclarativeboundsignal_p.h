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

#ifndef QDECLARATIVEBOUNDSIGNAL_P_H
#define QDECLARATIVEBOUNDSIGNAL_P_H

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

#include "qdeclarativeexpression.h"
#include "qdeclarativeguard_p.h"

#include <QtCore/qmetaobject.h>

#include "qobject_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativeAbstractBoundSignal : public QObject
{
    Q_OBJECT
public:
    QDeclarativeAbstractBoundSignal(QObject *parent = nullptr);
    virtual ~QDeclarativeAbstractBoundSignal() = 0;
    virtual void disconnect() = 0;

protected slots:
    virtual void unregisterScopeObject() = 0;
};

class QDeclarativeBoundSignalParameters;
class QDeclarativeBoundSignal : public QDeclarativeAbstractBoundSignal
{
public:
    QDeclarativeBoundSignal(QObject *scope, const QMetaMethod &signal, QObject *parent);
    QDeclarativeBoundSignal(QDeclarativeContext *ctxt, const QString &val, QObject *scope, 
                   const QMetaMethod &signal, QObject *parent);
    virtual ~QDeclarativeBoundSignal();

    void disconnect();

    int index() const;

    QDeclarativeExpression *expression() const;
    QDeclarativeExpression *setExpression(QDeclarativeExpression *);

    bool isEvaluating() const { return m_isEvaluating; }

    static QDeclarativeBoundSignal *cast(QObject *);

protected:
    void unregisterScopeObject();
    virtual int qt_metacall(QMetaObject::Call c, int id, void **a);

private:
    class ScopeGuard : public QDeclarativeGuard<QObject>
    {
    public:
        ScopeGuard(QObject *object, QDeclarativeBoundSignal *signal)
            : QDeclarativeGuard<QObject>(object), m_signal(signal)
        {
        }

        void objectDestroyed(QObject *obj) {
            Q_UNUSED(obj);
            m_signal->unregisterScopeObject();
        }

    private:
        QDeclarativeBoundSignal *m_signal;
    };

    void init(QObject *parent);

    QDeclarativeExpression *m_expression;
    QMetaMethod m_signal;
    bool m_paramsValid : 1;
    bool m_isEvaluating : 1;
    QDeclarativeBoundSignalParameters *m_params;
    ScopeGuard m_scope;
};

QT_END_NAMESPACE

#endif // QDECLARATIVEBOUNDSIGNAL_P_H
