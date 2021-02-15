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

#ifndef QSCRIPTDEBUGGERCONSOLECOMMANDGROUPDATA_P_H
#define QSCRIPTDEBUGGERCONSOLECOMMANDGROUPDATA_P_H

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

#include "qobjectdefs.h"
#include "qscopedpointer_p.h"
#include "qmap.h"

QT_BEGIN_NAMESPACE

class QString;

class QScriptDebuggerConsoleCommandGroupDataPrivate;
class Q_AUTOTEST_EXPORT QScriptDebuggerConsoleCommandGroupData
{
public:
    QScriptDebuggerConsoleCommandGroupData();
    QScriptDebuggerConsoleCommandGroupData(
        const QString &shortDescription,
        const QString &longDescription);
    QScriptDebuggerConsoleCommandGroupData(
        const QScriptDebuggerConsoleCommandGroupData &other);
    ~QScriptDebuggerConsoleCommandGroupData();

    QString shortDescription() const;
    QString longDescription() const;

    bool isValid() const;

    QScriptDebuggerConsoleCommandGroupData &operator=(
        const QScriptDebuggerConsoleCommandGroupData &other);

private:
    QScopedSharedPointer<QScriptDebuggerConsoleCommandGroupDataPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(QScriptDebuggerConsoleCommandGroupData)
};

typedef QMap<QString, QScriptDebuggerConsoleCommandGroupData> QScriptDebuggerConsoleCommandGroupMap;

QT_END_NAMESPACE

#endif
