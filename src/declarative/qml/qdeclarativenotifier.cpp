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

#include "qdeclarativenotifier_p.h"
#include "qdeclarativeproperty_p.h"

QT_BEGIN_NAMESPACE

void QDeclarativeNotifier::emitNotify(QDeclarativeNotifierEndpoint *endpoint)
{
    QDeclarativeNotifierEndpoint::Notifier *n = endpoint->asNotifier();

    QDeclarativeNotifierEndpoint **oldDisconnected = n->disconnected;
    n->disconnected = &endpoint;

    if (n->next)
        emitNotify(n->next);

    if (endpoint) {
        void *args[] = { 0 };

        QMetaObject::metacall(endpoint->target, QMetaObject::InvokeMetaMethod, 
                              endpoint->targetMethod, args);

        if (endpoint)
            endpoint->asNotifier()->disconnected = oldDisconnected;
    } 

    if (oldDisconnected) *oldDisconnected = endpoint;
}

void QDeclarativeNotifierEndpoint::connect(QObject *source, int sourceSignal)
{
    Signal *s = toSignal();
    
    if (s->source == source && s->sourceSignal == sourceSignal) {
        refCount++;
        return;
    }

    disconnect();

    QDeclarativePropertyPrivate::connect(source, sourceSignal, target, targetMethod);

    s->source = source;
    s->sourceSignal = sourceSignal;
    refCount++;
}

void QDeclarativeNotifierEndpoint::copyAndClear(QDeclarativeNotifierEndpoint &other)
{
    other.disconnect();

    other.target = target;
    other.targetMethod = targetMethod;

    if (!isConnected())
        return;

    if (SignalType == type) {
        Signal *other_s = other.toSignal();
        Signal *s = asSignal();

        other_s->source = s->source;
        other_s->sourceSignal = s->sourceSignal;
        s->source = 0;
    } else if(NotifierType == type) {
        Notifier *other_n = other.toNotifier();
        Notifier *n = asNotifier();

        other_n->notifier = n->notifier;
        other_n->disconnected = n->disconnected;
        if (other_n->disconnected) *other_n->disconnected = &other;

        if (n->next) {
            other_n->next = n->next;
            n->next->asNotifier()->prev = &other_n->next;
        }
        other_n->prev = n->prev;
        *other_n->prev = &other;

        n->prev = 0;
        n->next = 0;
        n->disconnected = 0;
        n->notifier = 0;
    } 
}


QT_END_NAMESPACE

