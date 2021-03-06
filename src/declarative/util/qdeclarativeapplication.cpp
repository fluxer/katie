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

#include "qdeclarativeapplication_p.h"
#include "qobject_p.h"
#include <QtGui/QApplication>

QT_BEGIN_NAMESPACE

class QDeclarativeApplicationPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeApplication)
public:
    QDeclarativeApplicationPrivate() : active(QApplication::activeWindow() != 0),
    layoutDirection(QApplication::layoutDirection()) {}
    bool active;
    Qt::LayoutDirection layoutDirection;
};

/*
    This object and its properties are documented as part of the Qt object,
    in qdeclarativengine.cpp
*/

QDeclarativeApplication::QDeclarativeApplication(QObject *parent) : QObject(*new QDeclarativeApplicationPrivate(), parent)
{
    if (qApp)
        qApp->installEventFilter(this);
}

QDeclarativeApplication::~QDeclarativeApplication()
{
}

bool QDeclarativeApplication::active() const
{
    Q_D(const QDeclarativeApplication);
    return d->active;
}

Qt::LayoutDirection QDeclarativeApplication::layoutDirection() const
{
    Q_D(const QDeclarativeApplication);
    return d->layoutDirection;
}

bool QDeclarativeApplication::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj)
    Q_D(QDeclarativeApplication);
    if (event->type() == QEvent::ApplicationActivate
     || event->type() == QEvent::ApplicationDeactivate) {
        bool active = d->active;
        if (event->type() == QEvent::ApplicationActivate)
            active  = true;
        else if (event->type() == QEvent::ApplicationDeactivate)
            active  = false;

        if (d->active != active) {
            d->active = active;
            emit activeChanged();
        }
    }
    if (event->type() == QEvent::LayoutDirectionChange) {
        Qt::LayoutDirection direction = QApplication::layoutDirection();
        if (d->layoutDirection != direction) {
            d->layoutDirection = direction;
            emit layoutDirectionChanged();
        }
    }
    return false;
}

QT_END_NAMESPACE
#include "moc_qdeclarativeapplication_p.h"
