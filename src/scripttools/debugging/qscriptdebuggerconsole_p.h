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

#ifndef QSCRIPTDEBUGGERCONSOLE_P_H
#define QSCRIPTDEBUGGERCONSOLE_P_H

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
#include <QtCore/qscopedpointer.h>

#include "qscriptdebuggerconsolehistorianinterface_p.h"

QT_BEGIN_NAMESPACE

class QString;
class QScriptDebuggerConsoleCommandJob;
class QScriptMessageHandlerInterface;
class QScriptDebuggerCommandSchedulerInterface;
class QScriptDebuggerConsoleCommandManager;

class QScriptDebuggerConsolePrivate;
class Q_AUTOTEST_EXPORT QScriptDebuggerConsole
    : public QScriptDebuggerConsoleHistorianInterface
{
public:
    QScriptDebuggerConsole();
    ~QScriptDebuggerConsole();

    void loadScriptedCommands(const QString &scriptsPath,
                              QScriptMessageHandlerInterface *messageHandler);

    void showDebuggerInfoMessage(QScriptMessageHandlerInterface *messageHandler);

    QScriptDebuggerConsoleCommandManager *commandManager() const;

    QScriptDebuggerConsoleCommandJob *consumeInput(
        const QString &input,
        QScriptMessageHandlerInterface *messageHandler,
        QScriptDebuggerCommandSchedulerInterface *commandScheduler);
    bool hasIncompleteInput() const;
    QString incompleteInput() const;
    void setIncompleteInput(const QString &input);
    QString commandPrefix() const;

    int historyCount() const;
    QString historyAt(int index) const;
    void changeHistoryAt(int index, const QString &newHistory);

    int currentFrameIndex() const;
    void setCurrentFrameIndex(int index);

    qint64 currentScriptId() const;
    void setCurrentScriptId(qint64 id);

    int currentLineNumber() const;
    void setCurrentLineNumber(int lineNumber);

    int evaluateAction() const;
    void setEvaluateAction(int action);

    qint64 sessionId() const;
    void bumpSessionId();

private:
    QScopedPointer<QScriptDebuggerConsolePrivate> d_ptr;

    Q_DECLARE_PRIVATE(QScriptDebuggerConsole)
    Q_DISABLE_COPY(QScriptDebuggerConsole)
};

QT_END_NAMESPACE

#endif
