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

#ifndef QDECLARATIVETIMER_H
#define QDECLARATIVETIMER_H

#include "qdeclarative.h"

#include <QtCore/qobject.h>
#include <QtGui/qabstractanimation.h>

#include "qdeclarativeglobal_p.h"


QT_BEGIN_NAMESPACE


class QDeclarativeTimerPrivate;
class Q_DECLARATIVE_PRIVATE_EXPORT QDeclarativeTimer : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeTimer)
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool repeat READ isRepeating WRITE setRepeating NOTIFY repeatChanged)
    Q_PROPERTY(bool triggeredOnStart READ triggeredOnStart WRITE setTriggeredOnStart NOTIFY triggeredOnStartChanged)
    Q_PROPERTY(QObject *parent READ parent CONSTANT)

public:
    QDeclarativeTimer(QObject *parent=0);

    void setInterval(int interval);
    int interval() const;

    bool isRunning() const;
    void setRunning(bool running);

    bool isRepeating() const;
    void setRepeating(bool repeating);

    bool triggeredOnStart() const;
    void setTriggeredOnStart(bool triggeredOnStart);

protected:
    void classBegin();
    void componentComplete();

public Q_SLOTS:
    void start();
    void stop();
    void restart();

Q_SIGNALS:
    void triggered();
    void runningChanged();
    void intervalChanged();
    void repeatChanged();
    void triggeredOnStartChanged();

private:
    void update();

private Q_SLOTS:
    void ticked();
    void finished();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeTimer)


#endif
