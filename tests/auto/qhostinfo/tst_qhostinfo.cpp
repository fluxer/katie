/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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

#include <QtTest/QtTest>
#include <qcoreapplication.h>
#include <QDebug>
#include <QTcpSocket>
#include <qthread.h>
#include <QTcpServer>
#include <qlibrary.h>
#include <qhostinfo.h>
#include "qhostinfo_p.h"

#ifndef QT_NO_BEARERMANAGEMENT
#include <QtNetwork/qnetworkconfigmanager.h>
#include <QtNetwork/qnetworkconfiguration.h>
#include <QtNetwork/qnetworksession.h>
#endif

#include <time.h>
#include <unistd.h>
#include <signal.h>

#if !defined(QT_NO_GETADDRINFO)
#  include <sys/types.h>
# if defined(Q_OS_UNIX)
#  include <sys/socket.h>
# endif
#  include <netdb.h>
#endif

#include "../network-settings.h"
#include "../../shared/util.h"

//TESTED_CLASS=
//TESTED_FILES=

#define TEST_DOMAIN ".test.qt-project.org"


class tst_QHostInfo : public QObject
{
    Q_OBJECT

public:
    tst_QHostInfo();
    virtual ~tst_QHostInfo();


public slots:
    void init();
    void cleanup();
    void initTestCase();

private slots:
    void getSetCheck();
    void staticInformation();
    void lookupIPv4_data();
    void lookupIPv4();
    void lookupIPv6_data();
    void lookupIPv6();
    void reverseLookup_data();
    void reverseLookup();

    void blockingLookup_data();
    void blockingLookup();

    void raceCondition();
#ifndef QT_NO_THREAD
    void threadSafety();
    void threadSafetyAsynchronousAPI();
#endif

    void multipleSameLookups();
    void multipleDifferentLookups_data();
    void multipleDifferentLookups();

protected slots:
    void resultsReady(const QHostInfo &);

private:
    bool ipv6LookupsAvailable;
    bool ipv6Available;
    bool lookupDone;
    int lookupsDoneCounter;
    QHostInfo lookupResults;
#ifndef QT_NO_BEARERMANAGEMENT
    QNetworkConfigurationManager *netConfMan;
    QNetworkConfiguration networkConfiguration;
    QScopedPointer<QNetworkSession> networkSession;
#endif
};

// Testing get/set functions
void tst_QHostInfo::getSetCheck()
{
    QHostInfo obj1;
    // HostInfoError QHostInfo::error()
    // void QHostInfo::setError(HostInfoError)
    obj1.setError(QHostInfo::HostInfoError(0));
    QCOMPARE(QHostInfo::HostInfoError(0), obj1.error());
    obj1.setError(QHostInfo::HostInfoError(1));
    QCOMPARE(QHostInfo::HostInfoError(1), obj1.error());

    // int QHostInfo::lookupId()
    // void QHostInfo::setLookupId(int)
    obj1.setLookupId(0);
    QCOMPARE(0, obj1.lookupId());
    obj1.setLookupId(INT_MIN);
    QCOMPARE(INT_MIN, obj1.lookupId());
    obj1.setLookupId(INT_MAX);
    QCOMPARE(INT_MAX, obj1.lookupId());
}

void tst_QHostInfo::staticInformation()
{
    qDebug() << "Hostname:" << QHostInfo::localHostName();
    qDebug() << "Domain name:" << QHostInfo::localDomainName();
}

tst_QHostInfo::tst_QHostInfo()
{
    Q_SET_DEFAULT_IAP
}

tst_QHostInfo::~tst_QHostInfo()
{
}

void tst_QHostInfo::initTestCase()
{
#ifndef QT_NO_BEARERMANAGEMENT
    netConfMan = new QNetworkConfigurationManager(this);
    netConfMan->updateConfigurations();
    connect(netConfMan, SIGNAL(updateCompleted()), &QTestEventLoop::instance(), SLOT(exitLoop()));
    QTestEventLoop::instance().enterLoop(10);
    networkConfiguration = netConfMan->defaultConfiguration();
    if (networkConfiguration.isValid()) {
        networkSession.reset(new QNetworkSession(networkConfiguration));
        if (!networkSession->isOpen()) {
            networkSession->open();
            QVERIFY(networkSession->waitForOpened(30000));
        }
    } else {
        QVERIFY(!(netConfMan->capabilities() & QNetworkConfigurationManager::NetworkSessionRequired));
    }
#endif

    ipv6Available = false;
    ipv6LookupsAvailable = false;

    QTcpServer server;
    if (server.listen(QHostAddress("::1"))) {
        // We have IPv6 support
        ipv6Available = true;
    }

#if !defined(QT_NO_GETADDRINFO)
    // check if the system getaddrinfo can do IPv6 lookups
    struct addrinfo hint, *result = 0;
    memset(&hint, 0, sizeof hint);
    hint.ai_family = AF_UNSPEC;
# ifdef AI_ADDRCONFIG
    hint.ai_flags = AI_ADDRCONFIG;
# endif

    int res = getaddrinfo("::1", "80", &hint, &result);
    if (res == 0) {
        // this test worked
        freeaddrinfo(result);
        res = getaddrinfo("aaaa-single" TEST_DOMAIN, "80", &hint, &result);
        if (res == 0 && result != 0 && result->ai_family != AF_INET) {
            freeaddrinfo(result);
            ipv6LookupsAvailable = true;
        }
    }
#endif

    // run each testcase with and without test enabled
    QTest::addColumn<bool>("cache");
    QTest::newRow("WithCache") << true;
    QTest::newRow("WithoutCache") << false;
}

void tst_QHostInfo::init()
{
    // delete the cache so inidividual testcase results are independent from each other
    qt_qhostinfo_clear_cache();

    QFETCH_GLOBAL(bool, cache);
    qt_qhostinfo_enable_cache(cache);
}

void tst_QHostInfo::cleanup()
{
}

void tst_QHostInfo::lookupIPv4_data()
{
    QTest::addColumn<QString>("hostname");
    QTest::addColumn<QString>("addresses");
    QTest::addColumn<int>("err");

    // Test server lookup
    QTest::newRow("lookup_01") << QtNetworkSettings::serverName() << QtNetworkSettings::serverIPs() << int(QHostInfo::NoError);
    QTest::newRow("empty") << "" << "" << int(QHostInfo::HostNotFound);

    QTest::newRow("single_ip4") << "a-single" TEST_DOMAIN << "192.0.2.1" << int(QHostInfo::NoError);
    QTest::newRow("multiple_ip4") << "a-multi" TEST_DOMAIN << "192.0.2.1 192.0.2.2 192.0.2.3" << int(QHostInfo::NoError);
    QTest::newRow("literal_ip4") << "192.0.2.1" << "192.0.2.1" << int(QHostInfo::NoError);

    QTest::newRow("notfound") << "invalid" TEST_DOMAIN << "" << int(QHostInfo::HostNotFound);

    QTest::newRow("idn-ace") << "a-single.xn--alqualond-34a" TEST_DOMAIN << "192.0.2.1" << int(QHostInfo::NoError);
    QTest::newRow("idn-unicode") << QString::fromLatin1("a-single.alqualond\353" TEST_DOMAIN) << "192.0.2.1" << int(QHostInfo::NoError);
}

void tst_QHostInfo::lookupIPv4()
{
    QFETCH(QString, hostname);
    QFETCH(int, err);
    QFETCH(QString, addresses);

    lookupDone = false;
    QHostInfo::lookupHost(hostname, this, SLOT(resultsReady(const QHostInfo&)));

    QTestEventLoop::instance().enterLoop(10);
    QVERIFY(!QTestEventLoop::instance().timeout());
    QVERIFY(lookupDone);

    if ((int)lookupResults.error() != (int)err) {
        qWarning() << hostname << "=>" << lookupResults.errorString();
    }
    QCOMPARE((int)lookupResults.error(), (int)err);

    QStringList tmp;
    for (int i = 0; i < lookupResults.addresses().count(); ++i)
        tmp.append(lookupResults.addresses().at(i).toString());
    tmp.sort();

    QStringList expected = addresses.split(' ');
    expected.sort();

    QCOMPARE(tmp.join(" "), expected.join(" "));
}

void tst_QHostInfo::lookupIPv6_data()
{
    QTest::addColumn<QString>("hostname");
    QTest::addColumn<QString>("addresses");
    QTest::addColumn<int>("err");

    QTest::newRow("aaaa-single") << "aaaa-single" TEST_DOMAIN << "2001:db8::1" << int(QHostInfo::NoError);
    QTest::newRow("aaaa-multi") << "aaaa-multi" TEST_DOMAIN << "2001:db8::1 2001:db8::2 2001:db8::3" << int(QHostInfo::NoError);
    QTest::newRow("a-plus-aaaa") << "a-plus-aaaa" TEST_DOMAIN << "198.51.100.1 2001:db8::1:1" << int(QHostInfo::NoError);

    // avoid using real IPv6 addresses here because this will do a DNS query
    // real addresses are between 2000:: and 3fff:ffff:ffff:ffff:ffff:ffff:ffff
    QTest::newRow("literal_ip6") << "f001:6b0:1:ea:202:a5ff:fecd:13a6" << "f001:6b0:1:ea:202:a5ff:fecd:13a6" << int(QHostInfo::NoError);
    QTest::newRow("literal_shortip6") << "f001:618:1401::4" << "f001:618:1401::4" << int(QHostInfo::NoError);
}

void tst_QHostInfo::lookupIPv6()
{
    QFETCH(QString, hostname);
    QFETCH(int, err);
    QFETCH(QString, addresses);

    if (!ipv6LookupsAvailable)
        QSKIP("This platform does not support IPv6 lookups", SkipAll);

    lookupDone = false;
    QHostInfo::lookupHost(hostname, this, SLOT(resultsReady(const QHostInfo&)));

    QTestEventLoop::instance().enterLoop(10);
    QVERIFY(!QTestEventLoop::instance().timeout());
    QVERIFY(lookupDone);

    QCOMPARE((int)lookupResults.error(), (int)err);

    QStringList tmp;
    for (int i = 0; i < lookupResults.addresses().count(); ++i)
        tmp.append(lookupResults.addresses().at(i).toString());
    tmp.sort();

    QStringList expected = addresses.split(' ');
    expected.sort();

    QCOMPARE(tmp.join(" ").toLower(), expected.join(" ").toLower());
}

void tst_QHostInfo::reverseLookup_data()
{
    QTest::addColumn<QString>("address");
    QTest::addColumn<QStringList>("hostNames");
    QTest::addColumn<int>("err");

    QTest::newRow("qt-project.org") << QString("87.238.53.172") << QStringList(QString("tufsla.qtproject.c.bitbit.net")) << 0;

    // ### Use internal DNS instead. Discussed with Andreas.
    //QTest::newRow("classical.hexago.com") << QString("2001:5c0:0:2::24") << QStringList(QString("classical.hexago.com")) << 0;
    QTest::newRow("gitorious.org") << QString("87.238.52.168") << QStringList(QString("gitorious.org")) << 0;
    QTest::newRow("bogus-name") << QString("1::2::3::4") << QStringList() << 1;
}

void tst_QHostInfo::reverseLookup()
{
    QFETCH(QString, address);
    QFETCH(QStringList, hostNames);
    QFETCH(int, err);

    if (!ipv6LookupsAvailable && hostNames.contains("classical.hexago.com")) {
        QSKIP("IPv6 lookups are not supported on this platform", SkipSingle);
    }
#if defined(Q_OS_HPUX) && defined(QT_ARCH_IA64)
    if (hostNames.contains("classical.hexago.com"))
        QSKIP("HP-UX 11i does not support IPv6 reverse lookups.", SkipSingle);
#endif

    QHostInfo info = QHostInfo::fromName(address);

    if (err == 0) {
        QVERIFY(hostNames.contains(info.hostName()));
        QCOMPARE(info.addresses().first(), QHostAddress(address));
    } else {
        QCOMPARE(info.hostName(), address);
        QCOMPARE(info.error(), QHostInfo::HostNotFound);
    }

}

void tst_QHostInfo::blockingLookup_data()
{
    lookupIPv4_data();
    if (ipv6LookupsAvailable)
        lookupIPv6_data();
}

void tst_QHostInfo::blockingLookup()
{
    QFETCH(QString, hostname);
    QFETCH(int, err);
    QFETCH(QString, addresses);

    QHostInfo hostInfo = QHostInfo::fromName(hostname);
    QStringList tmp;
    for (int i = 0; i < hostInfo.addresses().count(); ++i)
        tmp.append(hostInfo.addresses().at(i).toString());
    tmp.sort();

    if ((int)hostInfo.error() != (int)err) {
        qWarning() << hostname << "=>" << lookupResults.errorString();
    }
    QCOMPARE((int)hostInfo.error(), (int)err);

    QStringList expected = addresses.split(' ');
    expected.sort();

    QCOMPARE(tmp.join(" ").toUpper(), expected.join(" ").toUpper());
}

void tst_QHostInfo::raceCondition()
{
    for (int i = 0; i < 1000; ++i) {
        QTcpSocket socket;
        socket.connectToHost("invalid" TEST_DOMAIN, 80);
    }
}

#ifndef QT_NO_THREAD
class LookupThread : public QThread
{
protected:
    inline void run()
    {
         QHostInfo info = QHostInfo::fromName("a-single" TEST_DOMAIN);
         QCOMPARE(info.error(), QHostInfo::NoError);
         QVERIFY(info.addresses().count() > 0);
         QCOMPARE(info.addresses().at(0).toString(), QString("192.0.2.1"));
    }
};

void tst_QHostInfo::threadSafety()
{
    const int nattempts = 5;
    const int runs = 100;
    LookupThread thr[nattempts];
    for (int j = 0; j < runs; ++j) {
        for (int i = 0; i < nattempts; ++i)
            thr[i].start();
        for (int k = nattempts - 1; k >= 0; --k)
            thr[k].wait();
    }
}

class LookupReceiver : public QObject
{
    Q_OBJECT
public slots:
    void start();
    void resultsReady(const QHostInfo&);
public:
    QHostInfo result;
    int numrequests;
};

void LookupReceiver::start()
{
    for (int i=0;i<numrequests;i++)
        QHostInfo::lookupHost(QString("a-single" TEST_DOMAIN), this, SLOT(resultsReady(QHostInfo)));
}

void LookupReceiver::resultsReady(const QHostInfo &info)
{
    result = info;
    numrequests--;
    if (numrequests == 0 || info.error() != QHostInfo::NoError)
        QThread::currentThread()->quit();
}

void tst_QHostInfo::threadSafetyAsynchronousAPI()
{
    const int nattempts = 10;
    const int lookupsperthread = 10;
    QList<QThread*> threads;
    QList<LookupReceiver*> receivers;
    for (int i = 0; i < nattempts; ++i) {
        QThread* thread = new QThread;
        LookupReceiver* receiver = new LookupReceiver;
        receiver->numrequests = lookupsperthread;
        receivers.append(receiver);
        receiver->moveToThread(thread);
        connect(thread, SIGNAL(started()), receiver, SLOT(start()));
        thread->start();
        threads.append(thread);
    }
    for (int k = threads.count() - 1; k >= 0; --k)
        QVERIFY(threads.at(k)->wait(60000));
    foreach (LookupReceiver* receiver, receivers) {
        QCOMPARE(receiver->result.error(), QHostInfo::NoError);
        QCOMPARE(receiver->result.addresses().at(0).toString(), QString("192.0.2.1"));
        QCOMPARE(receiver->numrequests, 0);
    }
}
#endif // QT_NO_THREAD

// this test is for the multi-threaded QHostInfo rewrite. It is about getting results at all,
// not about getting correct IPs
void tst_QHostInfo::multipleSameLookups()
{
    const int COUNT = 10;
    lookupsDoneCounter = 0;

    for (int i = 0; i < COUNT; i++)
        QHostInfo::lookupHost("localhost", this, SLOT(resultsReady(const QHostInfo)));

    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < 10000 && lookupsDoneCounter < COUNT) {
        QTestEventLoop::instance().enterLoop(2);
    }
    QCOMPARE(lookupsDoneCounter, COUNT);
}

// this test is for the multi-threaded QHostInfo rewrite. It is about getting results at all,
// not about getting correct IPs
void tst_QHostInfo::multipleDifferentLookups_data()
{
    QTest::addColumn<int>("repeats");
    QTest::newRow("1") << 1;
    QTest::newRow("2") << 2;
    QTest::newRow("5") << 5;
    QTest::newRow("10") << 10;
}

void tst_QHostInfo::multipleDifferentLookups()
{
    QStringList hostnameList;
    hostnameList << "a-single" TEST_DOMAIN
                 << "a-multi" TEST_DOMAIN
                 << "aaaa-single" TEST_DOMAIN
                 << "aaaa-multi" TEST_DOMAIN
                 << "a-plus-aaaa" TEST_DOMAIN
                 << "multi" TEST_DOMAIN
                 << "localhost" TEST_DOMAIN
                 << "cname" TEST_DOMAIN
                 << "127.0.0.1" << "----";

    QFETCH(int, repeats);
    const int COUNT = hostnameList.size();
    lookupsDoneCounter = 0;

    for (int i = 0; i < hostnameList.size(); i++)
        for (int j = 0; j < repeats; ++j)
            QHostInfo::lookupHost(hostnameList.at(i), this, SLOT(resultsReady(const QHostInfo)));

    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < 60000 && lookupsDoneCounter < repeats*COUNT) {
        QTestEventLoop::instance().enterLoop(2);
        //qDebug() << "t:" << timer.elapsed();
    }
    QCOMPARE(lookupsDoneCounter, repeats*COUNT);
}

void tst_QHostInfo::resultsReady(const QHostInfo &hi)
{
    lookupDone = true;
    lookupResults = hi;
    lookupsDoneCounter++;
    QMetaObject::invokeMethod(&QTestEventLoop::instance(), "exitLoop", Qt::QueuedConnection);
}

QTEST_MAIN(tst_QHostInfo)

#include "moc_tst_qhostinfo.cpp"
