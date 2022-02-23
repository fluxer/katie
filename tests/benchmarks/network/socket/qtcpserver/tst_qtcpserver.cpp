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
#include <qtcpsocket.h>
#include <qtcpserver.h>
#include <qhostaddress.h>
#include <qprocess.h>
#include <qstringlist.h>
#include <qplatformdefs.h>
#include <qhostinfo.h>

#include "../../../../auto/network-settings.h"

class tst_QTcpServer : public QObject
{
    Q_OBJECT

public:
    tst_QTcpServer();
    virtual ~tst_QTcpServer();


public slots:
    void initTestCase_data();
    void init();
    void initTestCase();
    void cleanup();
private slots:
    void ipv4LoopbackPerformanceTest();
    void ipv6LoopbackPerformanceTest();
    void ipv4PerformanceTest();
};

tst_QTcpServer::tst_QTcpServer()
{
}

void tst_QTcpServer::initTestCase()
{
}

tst_QTcpServer::~tst_QTcpServer()
{
}

void tst_QTcpServer::initTestCase_data()
{
}

void tst_QTcpServer::init()
{
}

void tst_QTcpServer::cleanup()
{
}

//----------------------------------------------------------------------------------
void tst_QTcpServer::ipv4LoopbackPerformanceTest()
{
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost));

    QVERIFY(server.isListening());

    QTcpSocket clientA;
    clientA.connectToHost(QHostAddress::LocalHost, server.serverPort());
    QVERIFY(clientA.waitForConnected(5000));
    QVERIFY(clientA.state() == QAbstractSocket::ConnectedState);

    QVERIFY(server.waitForNewConnection());
    QTcpSocket *clientB = server.nextPendingConnection();
    QVERIFY(clientB);

    QByteArray buffer(QT_BUFFSIZE, '@');
    QTime stopWatch;
    stopWatch.start();
    qlonglong totalWritten = 0;
    while (stopWatch.elapsed() < 5000) {
        QVERIFY(clientA.write(buffer.data(), buffer.size()) > 0);
        clientA.flush();
        totalWritten += buffer.size();
        while (clientB->waitForReadyRead(100)) {
            if (clientB->bytesAvailable() == QT_BUFFSIZE)
                break;
        }
        clientB->read(buffer.data(), buffer.size());
        clientB->write(buffer.data(), buffer.size());
        clientB->flush();
        totalWritten += buffer.size();
        while (clientA.waitForReadyRead(100)) {
            if (clientA.bytesAvailable() == QT_BUFFSIZE)
                break;
        }
        clientA.read(buffer.data(), buffer.size());
    }

    qDebug("\t\t%s: %.1fMB/%.1fs: %.1fMB/s",
           server.serverAddress().toString().toLatin1().constData(),
           totalWritten / (1024.0 * 1024.0),
           stopWatch.elapsed() / 1000.0,
           (totalWritten / (stopWatch.elapsed() / 1000.0)) / (1024 * 1024));

    delete clientB;
}

//----------------------------------------------------------------------------------
void tst_QTcpServer::ipv6LoopbackPerformanceTest()
{
    QTcpServer server;
    if (!server.listen(QHostAddress::LocalHostIPv6, 0)) {
        QVERIFY(server.serverError() == QAbstractSocket::UnsupportedSocketOperationError);
    } else {
        QTcpSocket clientA;
        clientA.connectToHost(server.serverAddress(), server.serverPort());
        QVERIFY(clientA.waitForConnected(5000));

        QVERIFY(server.waitForNewConnection(5000));
        QTcpSocket *clientB = server.nextPendingConnection();
        QVERIFY(clientB);

        QByteArray buffer(QT_BUFFSIZE, '@');
        QTime stopWatch;
        stopWatch.start();
        qlonglong totalWritten = 0;
        while (stopWatch.elapsed() < 5000) {
            clientA.write(buffer.data(), buffer.size());
            clientA.flush();
            totalWritten += buffer.size();
            while (clientB->waitForReadyRead(100)) {
                if (clientB->bytesAvailable() == QT_BUFFSIZE)
                    break;
            }
            clientB->read(buffer.data(), buffer.size());
            clientB->write(buffer.data(), buffer.size());
            clientB->flush();
            totalWritten += buffer.size();
            while (clientA.waitForReadyRead(100)) {
                if (clientA.bytesAvailable() == QT_BUFFSIZE)
                   break;
            }
            clientA.read(buffer.data(), buffer.size());
        }

        qDebug("\t\t%s: %.1fMB/%.1fs: %.1fMB/s",
               server.serverAddress().toString().toLatin1().constData(),
               totalWritten / (1024.0 * 1024.0),
               stopWatch.elapsed() / 1000.0,
               (totalWritten / (stopWatch.elapsed() / 1000.0)) / (1024 * 1024));
        delete clientB;
    }
}

//----------------------------------------------------------------------------------
void tst_QTcpServer::ipv4PerformanceTest()
{
    QTcpSocket probeSocket;
    probeSocket.connectToHost(QtNetworkSettings::serverName(), 143);
    QVERIFY(probeSocket.waitForConnected(5000));

    QTcpServer server;
    QVERIFY(server.listen(probeSocket.localAddress(), 0));

    QTcpSocket clientA;
    clientA.connectToHost(server.serverAddress(), server.serverPort());
    QVERIFY(clientA.waitForConnected(5000));

    QVERIFY(server.waitForNewConnection(5000));
    QTcpSocket *clientB = server.nextPendingConnection();
    QVERIFY(clientB);

    QByteArray buffer(QT_BUFFSIZE, '@');
    QTime stopWatch;
    stopWatch.start();
    qlonglong totalWritten = 0;
    while (stopWatch.elapsed() < 5000) {
        qlonglong writtenA = clientA.write(buffer.data(), buffer.size());
        clientA.flush();
        totalWritten += buffer.size();
        while (clientB->waitForReadyRead(100)) {
            if (clientB->bytesAvailable() == writtenA)
                break;
        }
        clientB->read(buffer.data(), buffer.size());
        qlonglong writtenB = clientB->write(buffer.data(), buffer.size());
        clientB->flush();
        totalWritten += buffer.size();
        while (clientA.waitForReadyRead(100)) {
            if (clientA.bytesAvailable() == writtenB)
               break;
        }
        clientA.read(buffer.data(), buffer.size());
    }

    qDebug("\t\t%s: %.1fMB/%.1fs: %.1fMB/s",
           probeSocket.localAddress().toString().toLatin1().constData(),
           totalWritten / (1024.0 * 1024.0),
           stopWatch.elapsed() / 1000.0,
           (totalWritten / (stopWatch.elapsed() / 1000.0)) / (1024 * 1024));

    delete clientB;
}

QTEST_MAIN(tst_QTcpServer)

#include "moc_tst_qtcpserver.cpp"
