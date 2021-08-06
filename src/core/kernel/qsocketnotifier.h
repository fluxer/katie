/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSOCKETNOTIFIER_H
#define QSOCKETNOTIFIER_H

#include <QtCore/qobject.h>


QT_BEGIN_NAMESPACE


class Q_CORE_EXPORT QSocketNotifier : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QObject)

public:
    enum Type { Read, Write, Exception };

    QSocketNotifier(int socket, Type, QObject *parent = nullptr);
    ~QSocketNotifier();

    inline int socket() const { return sockfd; }
    inline Type type() const { return sntype; }

    inline bool isEnabled() const { return snenabled; }

public Q_SLOTS:
    void setEnabled(bool);

Q_SIGNALS:
    void activated(int socket);

protected:
    bool event(QEvent *);

private:
    Q_DISABLE_COPY(QSocketNotifier)

    int sockfd;
    Type sntype;
    bool snenabled;
};

QT_END_NAMESPACE


#endif // QSOCKETNOTIFIER_H
