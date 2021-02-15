/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the plugins of the Katie Toolkit.
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

#include <QtDeclarative/qdeclarativeextensionplugin.h>
#include <QtDeclarative/qdeclarative.h>

#include "qdeclarativegesturearea_p.h"

QT_BEGIN_NAMESPACE

class GestureAreaQmlPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Qt.labs.gestures"));
#ifndef QT_NO_GESTURES
        qmlRegisterCustomType<QDeclarativeGestureArea>(uri,1,0, "GestureArea", new QDeclarativeGestureAreaParser);

        qmlRegisterUncreatableType<QGesture>(uri, 1, 0, "Gesture", QLatin1String("Do not create objects of this type."));
        qmlRegisterUncreatableType<QPanGesture>(uri, 1, 0, "PanGesture", QLatin1String("Do not create objects of this type."));
        qmlRegisterUncreatableType<QTapGesture>(uri, 1, 0, "TapGesture", QLatin1String("Do not create objects of this type."));
        qmlRegisterUncreatableType<QTapAndHoldGesture>(uri, 1, 0, "TapAndHoldGesture", QLatin1String("Do not create objects of this type."));
        qmlRegisterUncreatableType<QPinchGesture>(uri, 1, 0, "PinchGesture", QLatin1String("Do not create objects of this type."));
        qmlRegisterUncreatableType<QSwipeGesture>(uri, 1, 0, "SwipeGesture", QLatin1String("Do not create objects of this type."));
#endif
    }
};

QT_END_NAMESPACE

#include "moc_gestures.cpp"

Q_EXPORT_PLUGIN2(qmlgesturesplugin, QT_PREPEND_NAMESPACE(GestureAreaQmlPlugin))
