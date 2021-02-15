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

#include <QtTest/QtTest>

#include <qcoreapplication.h>
#include <qsslconfiguration.h>
#include <qsslsocket.h>


#include "../../../../auto/network-settings.h"

//TESTED_CLASS=
//TESTED_FILES=

class tst_QSslSocket : public QObject
{
    Q_OBJECT

public:
    tst_QSslSocket();
    virtual ~tst_QSslSocket();


public slots:
    void initTestCase_data();
    void init();
    void cleanup();
private slots:
    void rootCertLoading();
    void systemCaCertificates();
};

tst_QSslSocket::tst_QSslSocket()
{
}

tst_QSslSocket::~tst_QSslSocket()
{
}

void tst_QSslSocket::initTestCase_data()
{
}

void tst_QSslSocket::init()
{
}

void tst_QSslSocket::cleanup()
{
}

//----------------------------------------------------------------------------------

void tst_QSslSocket::rootCertLoading()
{
    QBENCHMARK_ONCE {
        QSslSocket socket;
        socket.connectToHostEncrypted(QtNetworkSettings::serverName(), 443);
        socket.waitForEncrypted();
    }
}

void tst_QSslSocket::systemCaCertificates()
{
  // The results of this test change if the benchmarking system changes too much.
  // Therefore this benchmark is only good for manual regression checking between
  // Qt versions.
  QBENCHMARK_ONCE {
     QList<QSslCertificate> list = QSslSocket::systemCaCertificates();
  }
}

QTEST_MAIN(tst_QSslSocket)

#include "moc_tst_qsslsocket.cpp"
