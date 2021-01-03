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

#include "qgraphicssystemfactory_p.h"
#include "qgraphicssystemplugin_p.h"
#include "qfactoryloader_p.h"
#include "qmutex.h"

#include "qapplication.h"
#include "qgraphicssystem_raster_p.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_LIBRARY
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, graphicsloader,
    (QGraphicsSystemFactoryInterface_iid, QLatin1String("/graphicssystems"), Qt::CaseInsensitive))
#endif

QGraphicsSystem *QGraphicsSystemFactory::create(const QString& key)
{
    QGraphicsSystem *ret = Q_NULLPTR;
    QString system = key.toLower();

#if defined (QT_GRAPHICSSYSTEM_RASTER) || defined(Q_WS_X11)
    if (system.isEmpty()) {
        system = QLatin1String("raster");
    }
#endif

    if (system == QLatin1String("raster")) {
        return new QRasterGraphicsSystem;
    } else if (system == QLatin1String("native")) {
        qWarning() << "Attempt to load native graphicssystem";
        return new QRasterGraphicsSystem;
    } else if (system.isEmpty()) {
        return 0;
    }

#ifndef QT_NO_LIBRARY
    if (!ret) {
        if (QGraphicsSystemFactoryInterface *factory = qobject_cast<QGraphicsSystemFactoryInterface*>(graphicsloader()->instance(system)))
            ret = factory->create(system);
    }
#endif

    if (!ret)
        qWarning() << "Unable to load graphicssystem" << system;

    return ret;
}

/*!
    Returns the list of valid keys, i.e. the keys this factory can
    create styles for.

    \sa create()
*/
QStringList QGraphicsSystemFactory::keys()
{
#ifndef QT_NO_LIBRARY
    QStringList list = graphicsloader()->keys();
    if (!list.contains(QLatin1String("raster")))
        list << QLatin1String("raster");
#else
    static QStringList list = QStringList() << QLatin1String("raster");
#endif
    return list;
}

QT_END_NAMESPACE





