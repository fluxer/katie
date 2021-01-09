/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGRAPHICSSYSTEMPLUGIN_H
#define QGRAPHICSSYSTEMPLUGIN_H

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

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>


QT_BEGIN_NAMESPACE

class QGraphicsSystem;

struct QGraphicsSystemFactoryInterface : public QFactoryInterface
{
    virtual QGraphicsSystem *create(const QString &key) = 0;
};

QT_END_NAMESPACE

#define QGraphicsSystemFactoryInterface_iid "Katie.QGraphicsSystemFactoryInterface"
Q_DECLARE_INTERFACE(QGraphicsSystemFactoryInterface, QGraphicsSystemFactoryInterface_iid)

QT_BEGIN_NAMESPACE

class Q_GUI_EXPORT QGraphicsSystemPlugin : public QObject, public QGraphicsSystemFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsSystemFactoryInterface:QFactoryInterface)
public:
    explicit QGraphicsSystemPlugin(QObject *parent = Q_NULLPTR);
    ~QGraphicsSystemPlugin();

    virtual QStringList keys() const = 0;
    virtual QGraphicsSystem *create(const QString &key) = 0;
};

QT_END_NAMESPACE


#endif // QGRAPHICSSYSTEMEPLUGIN_H
