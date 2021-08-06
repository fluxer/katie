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

#ifndef QACCESSIBLEBRIDGE_H
#define QACCESSIBLEBRIDGE_H

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_ACCESSIBILITY

class QAccessibleInterface;

class QAccessibleBridge
{
public:
    virtual ~QAccessibleBridge() {}
    virtual void setRootObject(QAccessibleInterface *) = 0;
    virtual void notifyAccessibilityUpdate(int, QAccessibleInterface*, int) = 0;
};

struct Q_GUI_EXPORT QAccessibleBridgeFactoryInterface : public QFactoryInterface
{
    virtual QAccessibleBridge *create(const QString& name) = 0;
};

QT_END_NAMESPACE

#define QAccessibleBridgeFactoryInterface_iid "Katie.QAccessibleBridgeFactoryInterface"
Q_DECLARE_INTERFACE(QAccessibleBridgeFactoryInterface, QAccessibleBridgeFactoryInterface_iid)

QT_BEGIN_NAMESPACE

class Q_GUI_EXPORT QAccessibleBridgePlugin : public QObject, public QAccessibleBridgeFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QAccessibleBridgeFactoryInterface:QFactoryInterface)
public:
    explicit QAccessibleBridgePlugin(QObject *parent = nullptr);
    ~QAccessibleBridgePlugin();

    virtual QStringList keys() const = 0;
    virtual QAccessibleBridge *create(const QString &key) = 0;
};

#endif // QT_NO_ACCESSIBILITY

QT_END_NAMESPACE


#endif // QACCESSIBLEBRIDGE_H
