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

#ifndef QSCRIPTDEBUGGERCONSOLEWIDGETINTERFACE_P_H
#define QSCRIPTDEBUGGERCONSOLEWIDGETINTERFACE_P_H

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

#include <QtGui/qwidget.h>

#include "qscriptmessagehandlerinterface_p.h"

QT_BEGIN_NAMESPACE

class QScriptDebuggerConsoleHistorianInterface;
class QScriptCompletionProviderInterface;

class QScriptDebuggerConsoleWidgetInterfacePrivate;
class Q_AUTOTEST_EXPORT QScriptDebuggerConsoleWidgetInterface:
    public QWidget,
    public QScriptMessageHandlerInterface
{
    Q_OBJECT
public:
    enum InputMode {
        NormalInputMode,
        PartialInputMode
    };

    ~QScriptDebuggerConsoleWidgetInterface();

    QScriptDebuggerConsoleHistorianInterface *commandHistorian() const;
    void setCommandHistorian(QScriptDebuggerConsoleHistorianInterface *historian);

    QScriptCompletionProviderInterface *completionProvider() const;
    void setCompletionProvider(QScriptCompletionProviderInterface *completionProvider);

    virtual void setLineContinuationMode(bool enabled) = 0;

    virtual void clear() = 0;

Q_SIGNALS:
    void lineEntered(const QString &contents);

protected:
    QScriptDebuggerConsoleWidgetInterface(QScriptDebuggerConsoleWidgetInterfacePrivate &dd,
                                          QWidget *parent, Qt::WindowFlags flags);

private:
    Q_DECLARE_PRIVATE(QScriptDebuggerConsoleWidgetInterface)
    Q_DISABLE_COPY(QScriptDebuggerConsoleWidgetInterface)
};

QT_END_NAMESPACE

#endif
