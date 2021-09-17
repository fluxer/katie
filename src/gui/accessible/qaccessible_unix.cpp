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

#include "qaccessible.h"
#include "qaccessiblebridge.h"

#ifndef QT_NO_ACCESSIBILITY

#include "qcoreapplication.h"
#include "qvector.h"
#include "qfactoryloader_p.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_LIBRARY
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, bridgeloader,
    (QAccessibleBridgeFactoryInterface_iid, QLatin1String("/accessiblebridge")))
#endif
Q_GLOBAL_STATIC(QVector<QAccessibleBridge *>, bridges)

void QAccessible::initialize()
{
    static const int shouldinit = qgetenv("QT_ACCESSIBILITY").toInt();
    if (!shouldinit)
        return;

#ifndef QT_NO_LIBRARY
    const QStringList l = bridgeloader()->keys();
    for (int i = 0; i < l.count(); ++i) {
        if (QAccessibleBridgeFactoryInterface *factory =
                qobject_cast<QAccessibleBridgeFactoryInterface*>(bridgeloader()->instance(l.at(i)))) {
            QAccessibleBridge * bridge = factory->create(l.at(i));
            if (bridge)
                bridges()->append(bridge);
        }
    }
#endif
}

void QAccessible::cleanup()
{
    qDeleteAll(*bridges());
}

void QAccessible::updateAccessibility(QObject *o, int who, Event reason)
{
    Q_ASSERT(o);

    if (updateHandler) {
        updateHandler(o, who, reason);
        return;
    }

    initialize();
    if (!bridges() || bridges()->isEmpty())
        return;

    QAccessibleInterface *iface = QAccessible::queryAccessibleInterface(o);
    if (!iface)
        return;

    // updates for List/Table/Tree should send child
    if (who) {
        QAccessibleInterface *child;
        iface->navigate(QAccessible::Child, who, &child);
        if (child) {
            delete iface;
            iface = child;
            who = 0;
        }
    }

    for (int i = 0; i < bridges()->count(); ++i)
        bridges()->at(i)->notifyAccessibilityUpdate(reason, iface, who);
    delete iface;
}

void QAccessible::setRootObject(QObject *o)
{
    if (rootObjectHandler) {
        rootObjectHandler(o);
        return;
    }

    initialize();
    if (bridges()->isEmpty() || !o)
        return;

    for (int i = 0; i < bridges()->count(); ++i) {
        QAccessibleInterface *iface = QAccessible::queryAccessibleInterface(o);
        bridges()->at(i)->setRootObject(iface);
    }
}

QT_END_NAMESPACE

#endif // QT_NO_ACCESSIBILITY



