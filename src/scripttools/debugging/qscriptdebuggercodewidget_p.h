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

#ifndef QSCRIPTDEBUGGERCODEWIDGET_P_H
#define QSCRIPTDEBUGGERCODEWIDGET_P_H

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

#include "qscriptdebuggercodewidgetinterface_p.h"

QT_BEGIN_NAMESPACE

class QScriptDebuggerCodeWidgetPrivate;
class Q_AUTOTEST_EXPORT QScriptDebuggerCodeWidget:
    public QScriptDebuggerCodeWidgetInterface
{
    Q_OBJECT
public:
    QScriptDebuggerCodeWidget(QWidget *parent = nullptr);
    ~QScriptDebuggerCodeWidget();

    QScriptDebuggerScriptsModel *scriptsModel() const;
    void setScriptsModel(QScriptDebuggerScriptsModel *model);

    void setToolTipProvider(QScriptToolTipProviderInterface *toolTipProvider);

    qint64 currentScriptId() const;
    void setCurrentScript(qint64 scriptId);

    void invalidateExecutionLineNumbers();

    QScriptBreakpointsModel *breakpointsModel() const;
    void setBreakpointsModel(QScriptBreakpointsModel *model);

    QScriptDebuggerCodeViewInterface *currentView() const;

private:
    Q_DECLARE_PRIVATE(QScriptDebuggerCodeWidget)
    Q_DISABLE_COPY(QScriptDebuggerCodeWidget)

    Q_PRIVATE_SLOT(d_func(), void _q_onBreakpointToggleRequest(int,bool))
    Q_PRIVATE_SLOT(d_func(), void _q_onBreakpointEnableRequest(int,bool))
    Q_PRIVATE_SLOT(d_func(), void _q_onBreakpointsAboutToBeRemoved(const QModelIndex &,int,int))
    Q_PRIVATE_SLOT(d_func(), void _q_onBreakpointsInserted(const QModelIndex &,int,int))
    Q_PRIVATE_SLOT(d_func(), void _q_onBreakpointsDataChanged(const QModelIndex &, const QModelIndex &))
    Q_PRIVATE_SLOT(d_func(), void _q_onScriptsChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_onToolTipRequest(const QPoint &, int, const QStringList &))
};

QT_END_NAMESPACE

#endif
