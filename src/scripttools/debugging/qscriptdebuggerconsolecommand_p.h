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

#ifndef QSCRIPTDEBUGGERCONSOLECOMMAND_P_H
#define QSCRIPTDEBUGGERCONSOLECOMMAND_P_H

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
#include <QtCore/qlist.h>

QT_BEGIN_NAMESPACE

class QString;
class QStringList;
class QScriptDebuggerConsole;
class QScriptDebuggerConsoleCommandJob;
class QScriptMessageHandlerInterface;
class QScriptDebuggerCommandSchedulerInterface;

class QScriptDebuggerConsoleCommandPrivate;
class Q_AUTOTEST_EXPORT QScriptDebuggerConsoleCommand
{
public:
    QScriptDebuggerConsoleCommand();
    virtual ~QScriptDebuggerConsoleCommand();

    virtual QString name() const = 0;
    virtual QString group() const = 0;
    virtual QString shortDescription() const = 0;
    virtual QString longDescription() const = 0;
    virtual QStringList seeAlso() const;
    virtual QStringList aliases() const;

    virtual QStringList argumentTypes() const;

    virtual QStringList subCommands() const;
    virtual QScriptDebuggerConsoleCommandJob *createJob(
        const QStringList &arguments,
        QScriptDebuggerConsole *console,
        QScriptMessageHandlerInterface *messageHandler,
        QScriptDebuggerCommandSchedulerInterface *scheduler) = 0;

protected:
    QScriptDebuggerConsoleCommand(QScriptDebuggerConsoleCommandPrivate &dd);
    QScopedPointer<QScriptDebuggerConsoleCommandPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(QScriptDebuggerConsoleCommand)
    Q_DISABLE_COPY(QScriptDebuggerConsoleCommand)
};

typedef QList<QScriptDebuggerConsoleCommand*> QScriptDebuggerConsoleCommandList;

QT_END_NAMESPACE

#endif
