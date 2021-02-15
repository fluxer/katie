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

#ifndef QSCRIPTDEBUGGERAGENT_P_P_H
#define QSCRIPTDEBUGGERAGENT_P_P_H

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

#include <QtScript/qscriptvalue.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qhash.h>
#include <QtCore/qmap.h>
#include <QtCore/qlist.h>

#include "qscriptscriptdata_p.h"
#include "qscriptbreakpointdata_p.h"

QT_BEGIN_NAMESPACE

class QScriptDebuggerAgent;
class QScriptDebuggerAgentPrivate
{
public:
    enum State {
        NoState,
        SteppingIntoState,
        SteppedIntoState,
        SteppingOverState,
        SteppedOverState,
        SteppingOutState,
        SteppedOutState,
        RunningToLocationState,
        ReachedLocationState,
        InterruptingState,
        InterruptedState,
        BreakpointState,
        ReturningByForceState,
        ReturnedByForceState
    };

    QScriptDebuggerAgentPrivate();
    ~QScriptDebuggerAgentPrivate();

    static QScriptDebuggerAgentPrivate *get(QScriptDebuggerAgent *);

    State state;
    int stepDepth;
    int stepCount;
    int targetScriptId;
    QString targetFileName;
    int targetLineNumber;
    QScriptValue stepResult;
    int returnCounter;
    QScriptValue returnValue;

    int nextBreakpointId;
    QHash<qint64, QList<int> > resolvedBreakpoints;
    QHash<QString, QList<int> > unresolvedBreakpoints;
    QScriptBreakpointMap breakpoints;
    int hitBreakpointId;

    QScriptScriptMap scripts;
    QScriptScriptMap checkpointScripts;
    QScriptScriptMap previousCheckpointScripts;
    QList<QList<qint64> > scriptIdStack;

    QList<qint64> contextIdStack;
    QList<qint64> checkpointContextIdStack;
    qint64 nextContextId;

    QTime processEventsTimer;
    int statementCounter;

    QScriptDebuggerBackendPrivate *backend;
};

QT_END_NAMESPACE

#endif
