/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#ifndef QEVENTDISPATCHER_GLIB_P_H
#define QEVENTDISPATCHER_GLIB_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QEventDispatcherGlib class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qabstracteventdispatcher.h"
#include "qabstracteventdispatcher_p.h"

#include <glib.h>

QT_BEGIN_NAMESPACE

struct GPostEventSource;
struct GSocketNotifierSource;
struct GTimerSource;
struct GIdleTimerSource;

class Q_CORE_EXPORT QEventDispatcherGlibPrivate : public QAbstractEventDispatcherPrivate
{

public:
    QEventDispatcherGlibPrivate(GMainContext *context = Q_NULLPTR);
    GMainContext *mainContext;
    GPostEventSource *postEventSource;
    GSocketNotifierSource *socketNotifierSource;
    GTimerSource *timerSource;
    GIdleTimerSource *idleTimerSource;

    void runTimersOnceWithNormalPriority();
};

class Q_CORE_EXPORT QEventDispatcherGlib : public QAbstractEventDispatcher
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QEventDispatcherGlib)

public:
    explicit QEventDispatcherGlib(QObject *parent = Q_NULLPTR);
    explicit QEventDispatcherGlib(GMainContext *context, QObject *parent = Q_NULLPTR);
    ~QEventDispatcherGlib();

    bool processEvents(QEventLoop::ProcessEventsFlags flags);
    bool hasPendingEvents();

    void registerSocketNotifier(QSocketNotifier *socketNotifier);
    void unregisterSocketNotifier(QSocketNotifier *socketNotifier);

    void registerTimer(int timerId, int interval, QObject *object);
    bool unregisterTimer(int timerId);
    bool unregisterTimers(QObject *object);
    QList<TimerInfo> registeredTimers(QObject *object) const;

    void wakeUp();
    void interrupt();
    void flush();

protected:
    QEventDispatcherGlib(QEventDispatcherGlibPrivate &dd, QObject *parent);
};

QT_END_NAMESPACE

#endif // QEVENTDISPATCHER_GLIB_P_H
