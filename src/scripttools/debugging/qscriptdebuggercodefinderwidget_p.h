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

#ifndef QSCRIPTDEBUGGERCODEFINDERWIDGET_P_H
#define QSCRIPTDEBUGGERCODEFINDERWIDGET_P_H

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

#include "qscriptdebuggercodefinderwidgetinterface_p.h"

QT_BEGIN_NAMESPACE

class QScriptDebuggerCodeFinderWidgetPrivate;
class Q_AUTOTEST_EXPORT QScriptDebuggerCodeFinderWidget:
    public QScriptDebuggerCodeFinderWidgetInterface
{
    Q_OBJECT
public:
    QScriptDebuggerCodeFinderWidget(QWidget *parent = nullptr);
    ~QScriptDebuggerCodeFinderWidget();

    int findOptions() const;

    QString text() const;
    void setText(const QString &text);

    void setOK(bool ok);
    void setWrapped(bool wrapped);

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    Q_DECLARE_PRIVATE(QScriptDebuggerCodeFinderWidget)
    Q_DISABLE_COPY(QScriptDebuggerCodeFinderWidget)

    Q_PRIVATE_SLOT(d_func(), void _q_updateButtons())
    Q_PRIVATE_SLOT(d_func(), void _q_onTextChanged(const QString &))
    Q_PRIVATE_SLOT(d_func(), void _q_next())
    Q_PRIVATE_SLOT(d_func(), void _q_previous())
};

QT_END_NAMESPACE

#endif
