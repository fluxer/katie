/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#ifndef QDBUSMESSAGE_P_H
#define QDBUSMESSAGE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qatomic.h"
#include "qstring.h"
#include "qdbusmessage.h"
#include "qdbusconnection.h"

struct DBusMessage;


QT_BEGIN_NAMESPACE

class QDBusConnectionPrivate;

class QDBusMessagePrivate
{
public:
    QDBusMessagePrivate();
    ~QDBusMessagePrivate();

    QList<QVariant> arguments;

    // the following parameters are "const": they are not changed after the constructors
    // the parametersValidated member below controls whether they've been validated already
    QString service, path, interface, name, message, signature;

    DBusMessage *msg;
    DBusMessage *reply;
    int type;
    int timeout;
    QDBusMessage *localReply;
    QAtomicInt ref;

    bool delayedReply;
    bool localMessage;
    bool autoStartService;
    mutable bool parametersValidated;

    static void setParametersValidated(QDBusMessage &msg, bool enable)
    { msg.d_ptr->parametersValidated = enable; }

    static DBusMessage *toDBusMessage(const QDBusMessage &message, QDBusConnection::ConnectionCapabilities capabilities,
                                      QDBusError *error);
    static QDBusMessage fromDBusMessage(DBusMessage *dmsg, QDBusConnection::ConnectionCapabilities capabilities);

    static bool isLocal(const QDBusMessage &msg);
    static QDBusMessage makeLocal(const QDBusConnectionPrivate &conn,
                                  const QDBusMessage &asSent);
    static QDBusMessage makeLocalReply(const QDBusConnectionPrivate &conn,
                                       const QDBusMessage &asSent);
};

QT_END_NAMESPACE

#endif
