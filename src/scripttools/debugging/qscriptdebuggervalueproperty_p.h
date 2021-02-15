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

#ifndef QSCRIPTDEBUGGERVALUEPROPERTY_P_H
#define QSCRIPTDEBUGGERVALUEPROPERTY_P_H

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
#include "qlist.h"
#include "qscopedpointer_p.h"
#include "qscriptvalue.h"

QT_BEGIN_NAMESPACE

class QDataStream;
class QString;
class QScriptDebuggerValue;

class QScriptDebuggerValuePropertyPrivate;
class Q_AUTOTEST_EXPORT QScriptDebuggerValueProperty
{
public:
    QScriptDebuggerValueProperty();
    QScriptDebuggerValueProperty(const QString &name,
                                 const QScriptDebuggerValue &value,
                                 const QString &valueAsString,
                                 QScriptValue::PropertyFlags flags);
    QScriptDebuggerValueProperty(const QScriptDebuggerValueProperty &other);
    ~QScriptDebuggerValueProperty();

    QScriptDebuggerValueProperty &operator=(const QScriptDebuggerValueProperty &other);

    QString name() const;
    QScriptDebuggerValue value() const;
    QString valueAsString() const;
    QScriptValue::PropertyFlags flags() const;

    bool isValid() const;

private:
    QScopedSharedPointer<QScriptDebuggerValuePropertyPrivate> d_ptr;

    Q_DECLARE_PRIVATE(QScriptDebuggerValueProperty)
};

typedef QList<QScriptDebuggerValueProperty> QScriptDebuggerValuePropertyList;

Q_AUTOTEST_EXPORT QDataStream &operator<<(QDataStream &, const QScriptDebuggerValueProperty &);
Q_AUTOTEST_EXPORT QDataStream &operator>>(QDataStream &, QScriptDebuggerValueProperty &);

QT_END_NAMESPACE

#endif
