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

#ifndef QDECLARATIVETRANSITION_H
#define QDECLARATIVETRANSITION_H

#include "qdeclarativestate_p.h"

#include "qdeclarative.h"

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE


class QDeclarativeAbstractAnimation;
class QDeclarativeTransitionPrivate;
class QDeclarativeTransitionManager;
class Q_DECLARATIVE_EXPORT QDeclarativeTransition : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeTransition)

    Q_PROPERTY(QString from READ fromState WRITE setFromState NOTIFY fromChanged)
    Q_PROPERTY(QString to READ toState WRITE setToState NOTIFY toChanged)
    Q_PROPERTY(bool reversible READ reversible WRITE setReversible NOTIFY reversibleChanged)
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeAbstractAnimation> animations READ animations)
    Q_CLASSINFO("DefaultProperty", "animations")
    Q_CLASSINFO("DeferredPropertyNames", "animations")

public:
    QDeclarativeTransition(QObject *parent=0);
    ~QDeclarativeTransition();

    QString fromState() const;
    void setFromState(const QString &);

    QString toState() const;
    void setToState(const QString &);

    bool reversible() const;
    void setReversible(bool);

    QDeclarativeListProperty<QDeclarativeAbstractAnimation> animations();

    void prepare(QDeclarativeStateOperation::ActionList &actions,
                 QList<QDeclarativeProperty> &after,
                 QDeclarativeTransitionManager *end);

    void setReversed(bool r);
    void stop();

Q_SIGNALS:
    void fromChanged();
    void toChanged();
    void reversibleChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeTransition)


#endif // QDECLARATIVETRANSITION_H
