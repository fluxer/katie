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

#ifndef QSCRIPTDEBUGGERVALUE_P_H
#define QSCRIPTDEBUGGERVALUE_P_H

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
#include <QtCore/qscopedpointer_p.h>
#include <QtCore/qlist.h>

QT_BEGIN_NAMESPACE

class QScriptValue;
class QScriptEngine;
class QDataStream;

class QScriptDebuggerValuePrivate;
class Q_AUTOTEST_EXPORT QScriptDebuggerValue
{
public:
    enum ValueType {
        NoValue,
        UndefinedValue,
        NullValue,
        BooleanValue,
        StringValue,
        NumberValue,
        ObjectValue
    };

    QScriptDebuggerValue();
    QScriptDebuggerValue(const QScriptValue &value);
    QScriptDebuggerValue(double value);
    QScriptDebuggerValue(bool value);
    QScriptDebuggerValue(const QString &value);
    QScriptDebuggerValue(qint64 objectId);
    QScriptDebuggerValue(ValueType type);
    QScriptDebuggerValue(const QScriptDebuggerValue &other);
    ~QScriptDebuggerValue();

    QScriptDebuggerValue &operator=(const QScriptDebuggerValue &other);

    ValueType type() const;

    double numberValue() const;
    bool booleanValue() const;
    QString stringValue() const;
    qint64 objectId() const;

    QScriptValue toScriptValue(QScriptEngine *engine) const;
    QString toString() const;

    bool operator==(const QScriptDebuggerValue &other) const;
    bool operator!=(const QScriptDebuggerValue &other) const;

private:
    QScopedSharedPointer<QScriptDebuggerValuePrivate> d_ptr;

    Q_DECLARE_PRIVATE(QScriptDebuggerValue)
};

typedef QList<QScriptDebuggerValue> QScriptDebuggerValueList;

Q_AUTOTEST_EXPORT QDataStream &operator<<(QDataStream &, const QScriptDebuggerValue &);
Q_AUTOTEST_EXPORT QDataStream &operator>>(QDataStream &, QScriptDebuggerValue &);

QT_END_NAMESPACE

#endif
