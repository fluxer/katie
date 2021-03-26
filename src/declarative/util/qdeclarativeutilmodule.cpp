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

#include "qdeclarativeutilmodule_p.h"
#include "qdeclarativeanimation_p.h"
#include "qdeclarativeanimation_p_p.h"
#include "qdeclarativebehavior_p.h"
#include "qdeclarativebind_p.h"
#include "qdeclarativeconnections_p.h"
#include "qdeclarativesmoothedanimation_p.h"
#include "qdeclarativefontloader_p.h"
#include "qdeclarativelistaccessor_p.h"
#include "qdeclarativelistmodel_p.h"
#include "qdeclarativenullablevalue_p_p.h"
#include "qdeclarativeopenmetaobject_p.h"
#include "qdeclarativepackage_p.h"
#include "qdeclarativepixmapcache_p.h"
#include "qdeclarativepropertychanges_p.h"
#include "qdeclarativepropertymap.h"
#include "qdeclarativespringanimation_p.h"
#include "qdeclarativestategroup_p.h"
#include "qdeclarativestateoperations_p.h"
#include "qdeclarativestate_p.h"
#include "qdeclarativestate_p_p.h"
#include "qdeclarativestyledtext_p.h"
#include "qdeclarativesystempalette_p.h"
#include "qdeclarativetimeline_p_p.h"
#include "qdeclarativetimer_p.h"
#include "qdeclarativetransitionmanager_p_p.h"
#include "qdeclarativetransition_p.h"
#include "qdeclarativeapplication_p.h"
#include "qdeclarativeview.h"
#include "qdeclarativeinfo.h"
#include "qdeclarativetypenotavailable_p.h"

QT_BEGIN_NAMESPACE

void QDeclarativeUtilModule::defineModule()
{
    if (QApplication::type() != QApplication::Tty) {
        qmlRegisterUncreatableType<QDeclarativeApplication>("QtQuick",1,1,"Application", QDeclarativeApplication::tr("Application is an abstract class"));

        qmlRegisterType<QDeclarativeAnchorAnimation>("QtQuick",1,0,"AnchorAnimation");
        qmlRegisterType<QDeclarativeAnchorChanges>("QtQuick",1,0,"AnchorChanges");
        qmlRegisterType<QDeclarativeBehavior>("QtQuick",1,0,"Behavior");
        qmlRegisterType<QDeclarativeColorAnimation>("QtQuick",1,0,"ColorAnimation");
        qmlRegisterType<QDeclarativeSmoothedAnimation>("QtQuick",1,0,"SmoothedAnimation");
        qmlRegisterType<QDeclarativeFontLoader>("QtQuick",1,0,"FontLoader");
        qmlRegisterType<QDeclarativeNumberAnimation>("QtQuick",1,0,"NumberAnimation");
        qmlRegisterType<QDeclarativePackage>("QtQuick",1,0,"Package");
        qmlRegisterType<QDeclarativeParallelAnimation>("QtQuick",1,0,"ParallelAnimation");
        qmlRegisterType<QDeclarativeParentAnimation>("QtQuick",1,0,"ParentAnimation");
        qmlRegisterType<QDeclarativeParentChange>("QtQuick",1,0,"ParentChange");
        qmlRegisterType<QDeclarativePauseAnimation>("QtQuick",1,0,"PauseAnimation");
        qmlRegisterType<QDeclarativePropertyAction>("QtQuick",1,0,"PropertyAction");
        qmlRegisterType<QDeclarativePropertyAnimation>("QtQuick",1,0,"PropertyAnimation");
        qmlRegisterType<QDeclarativeRotationAnimation>("QtQuick",1,0,"RotationAnimation");
        qmlRegisterType<QDeclarativeScriptAction>("QtQuick",1,0,"ScriptAction");
        qmlRegisterType<QDeclarativeSequentialAnimation>("QtQuick",1,0,"SequentialAnimation");
        qmlRegisterType<QDeclarativeSpringAnimation>("QtQuick",1,0,"SpringAnimation");
        qmlRegisterType<QDeclarativeSystemPalette>("QtQuick",1,0,"SystemPalette");
        qmlRegisterType<QDeclarativeTransition>("QtQuick",1,0,"Transition");
        qmlRegisterType<QDeclarativeVector3dAnimation>("QtQuick",1,0,"Vector3dAnimation");

        qmlRegisterType<QDeclarativeAnchors>();
        qmlRegisterType<QDeclarativeStateOperation>();
        qmlRegisterType<QDeclarativeAnchorSet>();

        qmlRegisterUncreatableType<QDeclarativeAbstractAnimation>("QtQuick",1,0,"Animation",QDeclarativeAbstractAnimation::tr("Animation is an abstract class"));
    }

    qmlRegisterType<QDeclarativeBind>("QtQuick",1,0,"Binding");
    qmlRegisterType<QDeclarativeConnections>("QtQuick",1,0,"Connections");
    qmlRegisterType<QDeclarativeTimer>("QtQuick",1,0,"Timer");
    qmlRegisterType<QDeclarativeStateGroup>("QtQuick",1,0,"StateGroup");
    qmlRegisterType<QDeclarativeState>("QtQuick",1,0,"State");
    qmlRegisterType<QDeclarativeStateChangeScript>("QtQuick",1,0,"StateChangeScript");
    qmlRegisterType<QDeclarativeListElement>("QtQuick",1,0,"ListElement");
    qmlRegisterCustomType<QDeclarativeConnections>("QtQuick",1,0,"Connections", new QDeclarativeConnectionsParser);
    qmlRegisterCustomType<QDeclarativePropertyChanges>("QtQuick",1,0,"PropertyChanges", new QDeclarativePropertyChangesParser);
    qmlRegisterCustomType<QDeclarativeListModel>("QtQuick",1,0,"ListModel", new QDeclarativeListModelParser);
}

QT_END_NAMESPACE
