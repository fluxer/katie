/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtSCriptTools module of the Katie Toolkit.
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

#ifndef QSCRIPTDEBUGGERWIDGETFACTORYINTERFACE_P_H
#define QSCRIPTDEBUGGERWIDGETFACTORYINTERFACE_P_H

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

#include <QtCore/qobjectdefs.h>

QT_BEGIN_NAMESPACE

class QScriptDebugOutputWidgetInterface;
class QScriptDebuggerConsoleWidgetInterface;
class QScriptErrorLogWidgetInterface;
class QScriptDebuggerCodeFinderWidgetInterface;
class QScriptDebuggerStackWidgetInterface;
class QScriptDebuggerScriptsWidgetInterface;
class QScriptDebuggerLocalsWidgetInterface;
class QScriptDebuggerCodeWidgetInterface;
class QScriptBreakpointsWidgetInterface;

class Q_AUTOTEST_EXPORT QScriptDebuggerWidgetFactoryInterface
{
public:
    virtual ~QScriptDebuggerWidgetFactoryInterface() {}

    virtual QScriptDebugOutputWidgetInterface *createDebugOutputWidget() = 0;
    virtual QScriptDebuggerConsoleWidgetInterface *createConsoleWidget() = 0;
    virtual QScriptErrorLogWidgetInterface *createErrorLogWidget() = 0;
    virtual QScriptDebuggerCodeFinderWidgetInterface *createCodeFinderWidget() = 0;
    virtual QScriptDebuggerStackWidgetInterface *createStackWidget() = 0;
    virtual QScriptDebuggerScriptsWidgetInterface *createScriptsWidget() = 0;
    virtual QScriptDebuggerLocalsWidgetInterface *createLocalsWidget() = 0;
    virtual QScriptDebuggerCodeWidgetInterface *createCodeWidget() = 0;
    virtual QScriptBreakpointsWidgetInterface *createBreakpointsWidget() = 0;
};

QT_END_NAMESPACE

#endif
