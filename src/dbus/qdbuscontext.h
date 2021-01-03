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

#ifndef QDBUSCONTEXT_H
#define QDBUSCONTEXT_H

#include <QtCore/qstring.h>
#include <QtDBus/qdbuserror.h>


QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QDBusConnection;
class QDBusMessage;

class QDBusContextPrivate;
class Q_DBUS_EXPORT QDBusContext
{
public:
    QDBusContext();
    ~QDBusContext();

    bool calledFromDBus() const;
    QDBusConnection connection() const;
    const QDBusMessage &message() const;

    // convenience methods
    bool isDelayedReply() const;
    void setDelayedReply(bool enable);
    void sendErrorReply(const QString &name, const QString &msg = QString());
    void sendErrorReply(QDBusError::ErrorType type, const QString &msg = QString());

private:
    QDBusContextPrivate *d_ptr;
    friend class QDBusContextPrivate;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif
