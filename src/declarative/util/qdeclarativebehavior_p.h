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

#ifndef QDECLARATIVEBEHAVIOR_H
#define QDECLARATIVEBEHAVIOR_H

#include "qdeclarativestate_p.h"

#include "qdeclarativepropertyvaluesource.h"
#include "qdeclarativepropertyvalueinterceptor.h"
#include "qdeclarative.h"
#include <QtGui/QAbstractAnimation>


QT_BEGIN_NAMESPACE


class QDeclarativeAbstractAnimation;
class QDeclarativeBehaviorPrivate;
class Q_DECLARATIVE_PRIVATE_EXPORT QDeclarativeBehavior : public QObject, public QDeclarativePropertyValueInterceptor
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeBehavior)

    Q_INTERFACES(QDeclarativePropertyValueInterceptor)
    Q_CLASSINFO("DefaultProperty", "animation")
    Q_PROPERTY(QDeclarativeAbstractAnimation *animation READ animation WRITE setAnimation)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_CLASSINFO("DeferredPropertyNames", "animation")

public:
    QDeclarativeBehavior(QObject *parent=0);
    ~QDeclarativeBehavior();

    virtual void setTarget(const QDeclarativeProperty &);
    virtual void write(const QVariant &value);

    QDeclarativeAbstractAnimation *animation();
    void setAnimation(QDeclarativeAbstractAnimation *);

    bool enabled() const;
    void setEnabled(bool enabled);

Q_SIGNALS:
    void enabledChanged();

private Q_SLOTS:
    void componentFinalized();
    void qtAnimationStateChanged(QAbstractAnimation::State,QAbstractAnimation::State);
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeBehavior)


#endif // QDECLARATIVEBEHAVIOR_H
