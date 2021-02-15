/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEEXPRESSION_H
#define QDECLARATIVEEXPRESSION_H

#include <QtDeclarative/qdeclarativeerror.h>

#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>


QT_BEGIN_NAMESPACE


class QString;
class QDeclarativeRefCount;
class QDeclarativeEngine;
class QDeclarativeContext;
class QDeclarativeExpressionPrivate;
class QDeclarativeContextData;
class QScriptValue;
class Q_DECLARATIVE_EXPORT QDeclarativeExpression : public QObject
{
    Q_OBJECT
public:
    QDeclarativeExpression();
    QDeclarativeExpression(QDeclarativeContext *, QObject *, const QString &, QObject * = 0);
    virtual ~QDeclarativeExpression();

    QDeclarativeEngine *engine() const;
    QDeclarativeContext *context() const;

    QString expression() const;
    void setExpression(const QString &);

    bool notifyOnValueChanged() const;
    void setNotifyOnValueChanged(bool);

    QString sourceFile() const;
    int lineNumber() const;
    void setSourceLocation(const QString &fileName, int line);

    QObject *scopeObject() const;

    bool hasError() const;
    void clearError();
    QDeclarativeError error() const;

    QVariant evaluate(bool *valueIsUndefined = 0);

Q_SIGNALS:
    void valueChanged();

protected:
    QDeclarativeExpression(QDeclarativeContextData *, QObject *, const QString &,
                           QDeclarativeExpressionPrivate &dd);
    QDeclarativeExpression(QDeclarativeContextData *, QObject *, const QScriptValue &,
                           QDeclarativeExpressionPrivate &dd);
    QDeclarativeExpression(QDeclarativeContextData *, void *, QDeclarativeRefCount *rc, 
                           QObject *me, const QString &, int, QDeclarativeExpressionPrivate &dd);

private:
    QDeclarativeExpression(QDeclarativeContextData *, QObject *, const QString &);

    Q_DISABLE_COPY(QDeclarativeExpression)
    Q_DECLARE_PRIVATE(QDeclarativeExpression)
    Q_PRIVATE_SLOT(d_func(), void _q_notify())
    friend class QDeclarativeContext;
    friend class QDeclarativeVME;
};

QT_END_NAMESPACE


#endif // QDECLARATIVEEXPRESSION_H

