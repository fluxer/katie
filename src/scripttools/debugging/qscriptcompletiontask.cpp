/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#include "qscriptcompletiontask_p.h"
#include "qscriptcompletiontaskinterface_p_p.h"
#include "qscriptdebuggerconsole_p.h"
#include "qscriptdebuggerconsolecommand_p.h"
#include "qscriptdebuggerconsolecommandmanager_p.h"
#include "qscriptdebuggercommandschedulerjob_p.h"
#include "qscriptdebuggercommandschedulerfrontend_p.h"
#include "qscriptdebuggerjobschedulerinterface_p.h"
#include "qscriptdebuggerresponse_p.h"
#include "qscripttoolscommon_p.h"

#include "qobject_p.h"

#include <QtCore/qset.h>
#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

class QScriptCompletionTaskPrivate
    : public QScriptCompletionTaskInterfacePrivate
{
    Q_DECLARE_PUBLIC(QScriptCompletionTask)
public:
    QScriptCompletionTaskPrivate();
    ~QScriptCompletionTaskPrivate();

    void completeScriptExpression();
    void emitFinished();

    QString contents;
    int cursorPosition;
    int frameIndex;
    QScriptDebuggerCommandSchedulerInterface *commandScheduler;
    QScriptDebuggerJobSchedulerInterface *jobScheduler;
    QScriptDebuggerConsole *console;
};

QScriptCompletionTaskPrivate::QScriptCompletionTaskPrivate()
    : cursorPosition(0), frameIndex(0), commandScheduler(0),
      jobScheduler(0), console(0)
{
}

QScriptCompletionTaskPrivate::~QScriptCompletionTaskPrivate()
{
}

class QScriptCompleteExpressionJob : public QScriptDebuggerCommandSchedulerJob
{
public:
    QScriptCompleteExpressionJob(int frameIndex, const QStringList &path,
                                 QScriptCompletionTaskPrivate *task,
                                 QScriptDebuggerCommandSchedulerInterface *scheduler)
        : QScriptDebuggerCommandSchedulerJob(scheduler),
          m_frameIndex(frameIndex), m_path(path), m_task(task)
        {}

    void start()
    {
        QScriptDebuggerCommandSchedulerFrontend frontend(commandScheduler(), this);
        frontend.scheduleGetCompletions(m_frameIndex, m_path);
    }
    void handleResponse(const QScriptDebuggerResponse &response, int /*commandId*/)
    {
        m_task->results = response.result().toStringList();
        m_task->emitFinished();
        finish();
    }

private:
    int m_frameIndex;
    QStringList m_path;
    QScriptCompletionTaskPrivate *m_task;
};

class QScriptCompleteScriptsJob : public QScriptDebuggerCommandSchedulerJob
{
public:
    QScriptCompleteScriptsJob(const QString &prefix, QScriptCompletionTaskPrivate *task,
                              QScriptDebuggerCommandSchedulerInterface *scheduler)
        : QScriptDebuggerCommandSchedulerJob(scheduler),
          m_prefix(prefix), m_task(task)
        {}

    void start()
    {
        QScriptDebuggerCommandSchedulerFrontend frontend(commandScheduler(), this);
        frontend.scheduleGetScripts();
    }
    void handleResponse(const QScriptDebuggerResponse &response, int /*commandId*/)
    {
        QScriptScriptMap scripts = response.resultAsScripts();
        QScriptScriptMap::const_iterator it;
        for (it = scripts.constBegin(); it != scripts.constEnd(); ++it) {
            QString fileName = it.value().fileName();
            if (isPrefixOf(m_prefix, fileName))
                m_task->results.append(fileName);
        }
        m_task->emitFinished();
        finish();
    }
private:
    QString m_prefix;
    QScriptCompletionTaskPrivate *m_task;
};

void QScriptCompletionTaskPrivate::completeScriptExpression()
{
    int pos = cursorPosition;
    if ((pos > 0) && contents.at(pos-1).isNumber()) {
        // completion of numbers is pointless
        emitFinished();
        return;
    }

    while ((pos > 0) && isAlmostIdentChar(contents.at(pos-1)))
        --pos;
    int pos2 = cursorPosition - 1;
    while ((pos2+1 < contents.size()) && isAlmostIdentChar(contents.at(pos2+1)))
        ++pos2;
    QString ident = contents.mid(pos, pos2 - pos + 1);
    position = pos;

    QStringList path;
    path.append(ident);
    while ((pos > 0) && (contents.at(pos-1) == QLatin1Char('.'))) {
        --pos;
        pos2 = pos;
        while ((pos > 0) && isAlmostIdentChar(contents.at(pos-1)))
            --pos;
        path.prepend(contents.mid(pos, pos2 - pos));
    }

    length = path.last().length();
    type = QScriptCompletionTask::ScriptIdentifierCompletion;

    QScriptDebuggerJob *job = new QScriptCompleteExpressionJob(frameIndex, path, this, commandScheduler);
    jobScheduler->scheduleJob(job);
}

void QScriptCompletionTaskPrivate::emitFinished()
{
    emit q_func()->finished();
}

QScriptCompletionTask::QScriptCompletionTask(
    const QString &contents, int cursorPosition, int frameIndex,
    QScriptDebuggerCommandSchedulerInterface *commandScheduler,
    QScriptDebuggerJobSchedulerInterface *jobScheduler,
    QScriptDebuggerConsole *console,
    QObject *parent)
    : QScriptCompletionTaskInterface(
        *new QScriptCompletionTaskPrivate, parent)
{
    Q_D(QScriptCompletionTask);
    d->contents = contents;
    d->cursorPosition = cursorPosition;
    if ((frameIndex == -1) && console)
        d->frameIndex = console->currentFrameIndex();
    else
        d->frameIndex = frameIndex;
    d->commandScheduler = commandScheduler;
    d->jobScheduler = jobScheduler;
    d->console = console;
}

QScriptCompletionTask::~QScriptCompletionTask()
{
}

void QScriptCompletionTask::start()
{
    Q_D(QScriptCompletionTask);
    d->type = NoCompletion;
    // see if we're typing a command
    // ### don't hardcode the command prefix
    QRegExp cmdRx(QString::fromLatin1("^\\s*\\.([a-zA-Z]*)"));
    int cmdIndex = cmdRx.indexIn(d->contents);
    if ((cmdIndex != -1) && d->console) {
        int len = cmdRx.matchedLength();
        QString prefix = cmdRx.capturedTexts().at(1);
        if ((d->cursorPosition >= cmdIndex) && (d->cursorPosition <= (cmdIndex+len))) {
            // editing command --> get command completions
            d->results = d->console->commandManager()->completions(prefix);
            d->position = cmdRx.pos(1);
            d->length = prefix.length();
            d->type = CommandNameCompletion;
            d->appendix = QString::fromLatin1(" ");
            emit finished();
        } else {
            QScriptDebuggerConsoleCommand *cmd = d->console->commandManager()->findCommand(prefix);
            if (!cmd) {
                emit finished();
                return;
            }
            // editing an argument
            int argNum = 0;
            QString arg;
            int pos = cmdIndex + len;
            while (pos < d->contents.size()) {
                while ((pos < d->contents.size()) && d->contents.at(pos).isSpace())
                    ++pos;
                if (pos < d->contents.size()) {
                    int pos2 = pos + 1;
                    while ((pos2 < d->contents.size()) && !d->contents.at(pos2).isSpace())
                        ++pos2;
                    if ((d->cursorPosition >= pos) && (d->cursorPosition <= pos2)) {
                        arg = d->contents.mid(pos, pos2 - pos);
                        break;
                    }
                    pos = pos2;
                    ++argNum;
                }
            }
            QString argType = cmd->argumentTypes().value(argNum);
            if (!argType.isEmpty()) {
                if (argType == QLatin1String("command-or-group-name")) {
                    d->results = d->console->commandManager()->completions(arg);
                } else if (argType == QLatin1String("script-filename")) {
                    d->position = pos;
                    d->length = arg.length();
                    d->type = CommandArgumentCompletion;
                    QScriptDebuggerJob *job = new QScriptCompleteScriptsJob(arg, d, d->commandScheduler);
                    d->jobScheduler->scheduleJob(job);
                } else if (argType == QLatin1String("subcommand-name")) {
                    for (int i = 0; i < cmd->subCommands().size(); ++i) {
                        QString name = cmd->subCommands().at(i);
                        if (isPrefixOf(arg, name))
                            d->results.append(name);
                    }
                    qStableSort(d->results);
                } else if (argType == QLatin1String("script")) {
                    d->completeScriptExpression();
                } else {
                    emit finished();
                }
                if ((d->type == NoCompletion) && !d->results.isEmpty()) {
                    d->position = pos;
                    d->length = arg.length();
                    d->type = CommandArgumentCompletion;
                    emit finished();
                }
            }
        }
    } else {
        // assume it's an eval expression
        d->completeScriptExpression();
    }
}

QT_END_NAMESPACE
#include <moc_qscriptcompletiontask_p.h>
