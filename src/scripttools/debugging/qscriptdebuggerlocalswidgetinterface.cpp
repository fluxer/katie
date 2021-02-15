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

#include "qscriptdebuggerlocalswidgetinterface_p.h"
#include "qscriptdebuggerlocalswidgetinterface_p_p.h"

QT_BEGIN_NAMESPACE

QScriptDebuggerLocalsWidgetInterfacePrivate::QScriptDebuggerLocalsWidgetInterfacePrivate()
{
    completionProvider = 0;
}

QScriptDebuggerLocalsWidgetInterfacePrivate::~QScriptDebuggerLocalsWidgetInterfacePrivate()
{
}

QScriptDebuggerLocalsWidgetInterface::~QScriptDebuggerLocalsWidgetInterface()
{
}

QScriptDebuggerLocalsWidgetInterface::QScriptDebuggerLocalsWidgetInterface(
    QScriptDebuggerLocalsWidgetInterfacePrivate &dd,
    QWidget *parent, Qt::WindowFlags flags)
    : QWidget(dd, parent, flags)
{
}

QScriptCompletionProviderInterface *QScriptDebuggerLocalsWidgetInterface::completionProvider() const
{
    Q_D(const QScriptDebuggerLocalsWidgetInterface);
    return d->completionProvider;
}

void QScriptDebuggerLocalsWidgetInterface::setCompletionProvider(QScriptCompletionProviderInterface *completionProvider)
{
    Q_D(QScriptDebuggerLocalsWidgetInterface);
    d->completionProvider = completionProvider;
}

QT_END_NAMESPACE
#include "moc_qscriptdebuggerlocalswidgetinterface_p.h"
