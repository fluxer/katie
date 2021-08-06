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

#ifndef QICONENGINEPLUGIN_H
#define QICONENGINEPLUGIN_H

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>


QT_BEGIN_NAMESPACE

class QIconEngine;

struct Q_GUI_EXPORT QIconEngineFactoryInterface : public QFactoryInterface
{
    virtual QIconEngine *create(const QString &filename= QString()) = 0;
};
typedef QIconEngineFactoryInterface QIconEngineFactoryInterfaceV2;

QT_END_NAMESPACE

#define QIconEngineFactoryInterface_iid \
    "Katie.QIconEngineFactoryInterface"
Q_DECLARE_INTERFACE(QIconEngineFactoryInterface, QIconEngineFactoryInterface_iid)

QT_BEGIN_NAMESPACE

class Q_GUI_EXPORT QIconEnginePlugin : public QObject, public QIconEngineFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QIconEngineFactoryInterface:QFactoryInterface)
public:
    QIconEnginePlugin(QObject *parent = nullptr);
    ~QIconEnginePlugin();

    virtual QStringList keys() const = 0;
    virtual QIconEngine *create(const QString &filename = QString()) = 0;
};
typedef QIconEnginePlugin QIconEnginePluginV2;

QT_END_NAMESPACE


#endif // QICONENGINEPLUGIN_H
