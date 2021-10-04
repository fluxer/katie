/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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

#include <QTest>
#include <qnet_unix_p.h>
#include <qcore_unix_p.h>

QT_USE_NAMESPACE

static const int timeout = 100;

class tst_selectpoll : public QObject
{
    Q_OBJECT
private slots:
    void bench_select();
    void bench_poll();
};

void tst_selectpoll::bench_select()
{
    int posixSocket = qt_safe_socket(AF_INET, SOCK_STREAM, 0);
    struct timeval tm = { 0l, timeout * 1000 };
    QBENCHMARK {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(posixSocket, &fds);
        int ret = qt_safe_select(posixSocket + 1, &fds, 0, 0, &tm);
        Q_UNUSED(ret);
    }
    qt_safe_close(posixSocket);
}

void tst_selectpoll::bench_poll()
{
    int posixSocket = qt_safe_socket(AF_INET, SOCK_STREAM, 0);
    QBENCHMARK {
        struct pollfd fds;
        ::memset(&fds, 0, sizeof(struct pollfd));
        fds.fd = posixSocket;
        fds.events = POLLIN;
        int ret = qt_safe_poll(&fds, 1, timeout);
        Q_UNUSED(ret);
    }
    qt_safe_close(posixSocket);
}

QTEST_MAIN(tst_selectpoll)

#include "moc_main.cpp"
