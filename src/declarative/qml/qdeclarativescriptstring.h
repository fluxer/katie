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

#ifndef QDECLARATIVESCRIPTSTRING_H
#define QDECLARATIVESCRIPTSTRING_H

#include <QtCore/qstring.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>


QT_BEGIN_NAMESPACE


class QObject;
class QDeclarativeContext;
class QDeclarativeScriptStringPrivate;
class Q_DECLARATIVE_EXPORT QDeclarativeScriptString 
{
public:
    QDeclarativeScriptString();
    QDeclarativeScriptString(const QDeclarativeScriptString &);
    ~QDeclarativeScriptString();

    QDeclarativeScriptString &operator=(const QDeclarativeScriptString &);

    QDeclarativeContext *context() const;
    void setContext(QDeclarativeContext *);

    QObject *scopeObject() const;
    void setScopeObject(QObject *);

    QString script() const;
    void setScript(const QString &);

private:
    QSharedDataPointer<QDeclarativeScriptStringPrivate> d;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QDeclarativeScriptString)


#endif // QDECLARATIVESCRIPTSTRING_H

