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

#ifndef QSCRIPTDEBUGGERSCRIPTEDCONSOLECOMMAND_P_H
#define QSCRIPTDEBUGGERSCRIPTEDCONSOLECOMMAND_P_H

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

#include "qscriptdebuggerconsolecommand_p.h"

QT_BEGIN_NAMESPACE

class QScriptEngine;
class QScriptValue;

class QScriptDebuggerScriptedConsoleCommandPrivate;
class Q_AUTOTEST_EXPORT QScriptDebuggerScriptedConsoleCommand
    : public QScriptDebuggerConsoleCommand
{
protected:
    QScriptDebuggerScriptedConsoleCommand(const QString &name,
                                          const QString &group,
                                          const QString &shortDescription,
                                          const QString &longDescription,
                                          const QStringList &aliases,
                                          const QStringList &seeAlso,
                                          const QStringList &argumentTypes,
                                          const QStringList &subCommands,
                                          const QScriptValue &globalObject,
                                          const QScriptValue &execFunction,
                                          const QScriptValue &responseFunction);
public:
    ~QScriptDebuggerScriptedConsoleCommand();

    static QScriptDebuggerScriptedConsoleCommand *parse(
        const QString &program, const QString &fileName,
        QScriptEngine *engine, QScriptMessageHandlerInterface *messageHandler);

    QString name() const;
    QString group() const;
    QString shortDescription() const;
    QString longDescription() const;
    QStringList aliases() const;
    QStringList seeAlso() const;
    QStringList argumentTypes() const;
    QStringList subCommands() const;

    QScriptDebuggerConsoleCommandJob *createJob(
        const QStringList &arguments,
        QScriptDebuggerConsole *console,
        QScriptMessageHandlerInterface *messageHandler,
        QScriptDebuggerCommandSchedulerInterface *commandScheduler);

private:
    Q_DECLARE_PRIVATE(QScriptDebuggerScriptedConsoleCommand)
    Q_DISABLE_COPY(QScriptDebuggerScriptedConsoleCommand)
};

QT_END_NAMESPACE

#endif
