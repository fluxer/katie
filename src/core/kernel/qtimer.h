/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QTIMER_H
#define QTIMER_H

#ifndef QT_NO_QOBJECT

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE

class Q_CORE_EXPORT QTimer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool singleShot READ isSingleShot WRITE setSingleShot)
    Q_PROPERTY(int interval READ interval WRITE setInterval)
    Q_PROPERTY(bool active READ isActive)
public:
    explicit QTimer(QObject *parent = Q_NULLPTR);
    ~QTimer();

    inline bool isActive() const { return id >= 0; }
    inline int timerId() const { return id; }

    void setInterval(const int msec);
    inline int interval() const { return inter; }

    inline void setSingleShot(bool singleShot) { single = singleShot; }
    inline bool isSingleShot() const { return single; }

    static void singleShot(const int msec, QObject *receiver, const char *member);

public Q_SLOTS:
    void start(int msec);

    void start();
    void stop();

Q_SIGNALS:
    void timeout();

protected:
    void timerEvent(QTimerEvent *);

private:
    Q_DISABLE_COPY(QTimer)

    inline int startTimer(int){ return -1;}
    inline void killTimer(int){}

    int id;
    int inter;
    bool single;
};

QT_END_NAMESPACE


#endif // QT_NO_QOBJECT

#endif // QTIMER_H
