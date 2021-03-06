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

#ifndef QDECLARATIVEBIND_H
#define QDECLARATIVEBIND_H

#include "qdeclarative.h"

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE


class QDeclarativeBindPrivate;
class Q_AUTOTEST_EXPORT QDeclarativeBind : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeBind)
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(QObject *target READ object WRITE setObject)
    Q_PROPERTY(QString property READ property WRITE setProperty)
    Q_PROPERTY(QVariant value READ value WRITE setValue)
    Q_PROPERTY(bool when READ when WRITE setWhen)

public:
    QDeclarativeBind(QObject *parent=0);
    ~QDeclarativeBind();

    bool when() const;
    void setWhen(bool);

    QObject *object();
    void setObject(QObject *);

    QString property() const;
    void setProperty(const QString &);

    QVariant value() const;
    void setValue(const QVariant &);

protected:
    virtual void classBegin();
    virtual void componentComplete();

private:
    void eval();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeBind)


#endif
