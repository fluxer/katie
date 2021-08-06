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

#ifndef QDECLARATIVEAPPLICATION_P_H
#define QDECLARATIVEAPPLICATION_P_H

#include <QtCore/QObject>
#include "qdeclarative.h"
#include "qdeclarativeglobal_p.h"


QT_BEGIN_NAMESPACE


class QDeclarativeApplicationPrivate;
class Q_DECLARATIVE_PRIVATE_EXPORT QDeclarativeApplication : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)
    Q_PROPERTY(Qt::LayoutDirection layoutDirection READ layoutDirection NOTIFY layoutDirectionChanged)

public:
    explicit QDeclarativeApplication(QObject *parent = nullptr);
    virtual ~QDeclarativeApplication();
    bool active() const;
    Qt::LayoutDirection layoutDirection() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event);

Q_SIGNALS:
    void activeChanged();
    void layoutDirectionChanged();

private:
    Q_DISABLE_COPY(QDeclarativeApplication)
    Q_DECLARE_PRIVATE(QDeclarativeApplication)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeApplication)


#endif // QDECLARATIVEAPPLICATION_P_H
