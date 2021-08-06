/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDBus module of the Katie Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the public API.  This header file may
// change from version to version without notice, or even be
// removed.
//
// We mean it.
//
//

#ifndef QDBUSPENDINGCALL_P_H
#define QDBUSPENDINGCALL_P_H

#include "qshareddata.h"
#include "qpointer.h"
#include "qlist.h"
#include "qmutex.h"
#include "qwaitcondition.h"
#include "qdbusmessage.h"

#include <dbus/dbus.h>

QT_BEGIN_NAMESPACE

class QDBusPendingCall;
class QDBusPendingCallWatcher;
class QDBusPendingCallWatcherHelper;
class QDBusConnectionPrivate;

class QDBusPendingCallPrivate: public QSharedData
{
public:
    // {
    //     set only during construction:
    const QDBusMessage sentMessage;
    QDBusConnectionPrivate * const connection;

    // for the callback mechanism (see setReplyCallback and QDBusConnectionPrivate::sendWithReplyAsync)
    QPointer<QObject> receiver;
    QList<int> metaTypes;
    int methodIdx;

    // }

    QMutex mutex;
    QWaitCondition waitForFinishedCondition;

    // {
    //    protected by the mutex above:
    QDBusPendingCallWatcherHelper *watcherHelper;
    QDBusMessage replyMessage;
    DBusPendingCall *pending;
    volatile bool waitingForFinished;

    QString expectedReplySignature;
    int expectedReplyCount;
    // }

    QDBusPendingCallPrivate(const QDBusMessage &sent, QDBusConnectionPrivate *connection)
        : sentMessage(sent), connection(connection), watcherHelper(nullptr), pending(nullptr),
          waitingForFinished(false)
    { }
    ~QDBusPendingCallPrivate();
    bool setReplyCallback(QObject *target, const char *member);
    void waitForFinished();
    void setMetaTypes(int count, const int *types);
    void checkReceivedSignature();

    static QDBusPendingCall fromMessage(const QDBusMessage &msg);
};

class QDBusPendingCallWatcherHelper: public QObject
{
    Q_OBJECT
public:
    void add(QDBusPendingCallWatcher *watcher);

    void emitSignals(const QDBusMessage &replyMessage, const QDBusMessage &sentMessage)
    {
        if (replyMessage.type() == QDBusMessage::ReplyMessage)
            emit reply(replyMessage);
        else
            emit error(replyMessage, sentMessage);
        emit finished();
    }

Q_SIGNALS:
    void finished();
    void reply(const QDBusMessage &msg);
    void error(const QDBusError &error, const QDBusMessage &msg);
};

QT_END_NAMESPACE

#endif
