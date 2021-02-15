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
#include <QtCore/QtCore>
#include <QtTest/QtTest>
#include <QtDBus/QtDBus>

#include "./serverobject.h"

static const QString serviceName = QLatin1String("com.trolltech.autotests.performance");
static const int runTime = 500;

class tst_QDBusPerformance: public QObject
{
    Q_OBJECT
    QProcess proc;
    QDBusInterface *target;

    QDBusInterface *remote;
    QDBusInterface *local;

    bool executeTest(const char *funcname, int size, const QVariant &data);

public slots:
    void initTestCase_data();
    void initTestCase();
    void init();

private slots:
    void callSpeed();

    void oneWay_data();
    void oneWay();
    void oneWayVariant_data();
    void oneWayVariant();

    void roundTrip_data();
    void roundTrip();
    void roundTripVariant_data();
    void roundTripVariant();
};
Q_DECLARE_METATYPE(QVariant)

void tst_QDBusPerformance::initTestCase()
{

    QDBusConnection con = QDBusConnection::sessionBus();
    QVERIFY(con.isConnected());

    QDBusServiceWatcher watcher(serviceName, con,
                                QDBusServiceWatcher::WatchForRegistration);
    connect(&watcher, SIGNAL(serviceRegistered(QString)),
            &QTestEventLoop::instance(), SLOT(exitLoop()));

    proc.start(QLatin1String("./performance_server"));
    QVERIFY(proc.waitForStarted());

    QTestEventLoop::instance().enterLoop(5);
    QVERIFY(con.interface()->isServiceRegistered(serviceName));

    remote = new QDBusInterface(serviceName, QLatin1String("/"),
        QLatin1String("com.trolltech.autotests.Performance"), con, this);
    QVERIFY(remote->isValid());

    new ServerObject(QLatin1String("/"), con, this);
    local = new QDBusInterface(con.baseService(), QLatin1String("/"),
        QLatin1String("com.trolltech.autotests.Performance"), con, this);
    QVERIFY(local->isValid());
}

void tst_QDBusPerformance::initTestCase_data()
{
    QTest::addColumn<bool>("loopback");

    QTest::newRow("normal") << false;
    QTest::newRow("loopback") << true;
}

void tst_QDBusPerformance::init()
{
    QFETCH_GLOBAL(bool, loopback);
    if (loopback)
        target = local;
    else
        target = remote;
}

void tst_QDBusPerformance::callSpeed()
{
    QElapsedTimer timer;

    int callCount = 0;
    timer.start();
    while (timer.elapsed() < runTime) {
        QDBusReply<void> reply = target->call(QLatin1String("nothing"));
        QVERIFY(reply.isValid());

        ++callCount;
    }
    qDebug() << callCount << "calls in" << timer.elapsed() << "ms:"
             << (callCount * 1000.0 / timer.elapsed()) << "calls/sec";
}

bool tst_QDBusPerformance::executeTest(const char *funcname, int size, const QVariant &data)
{
    QElapsedTimer timer;

    int callCount = 0;
    qint64 transferred = 0;
    timer.start();
    while (timer.elapsed() < runTime) {
        QDBusMessage reply = target->call(QLatin1String(funcname), data);
        if (reply.type() != QDBusMessage::ReplyMessage)
            return false;

        transferred += size;
        ++callCount;
    }
    qDebug() << transferred << "bytes in" << timer.elapsed() << "ms"
             << "(in" << callCount << "calls):"
             << (transferred * 1000.0 / timer.elapsed() / 1024 / 1024) << "MB/s";

    return true;
}

void tst_QDBusPerformance::oneWay_data()
{
    QTest::addColumn<QVariant>("data");
    QTest::addColumn<int>("size");

    QByteArray ba(256, 'a');
    while (ba.size() < 8193) {
        QTest::newRow(QString("%1-byteArray").arg(ba.size()).toAscii()) << qVariantFromValue(ba) << ba.size();
        ba += ba;
    }

    QString s(256, QLatin1Char('a'));
    while (s.size() < 8193) {
        QTest::newRow(QString("%1-string").arg(s.size()).toAscii()) << qVariantFromValue(s) << s.size();
        s += s;
    }
}

void tst_QDBusPerformance::oneWay()
{
    QFETCH(QVariant, data);
    QFETCH(int, size);

    QVERIFY(executeTest("size", size, data));
}

void tst_QDBusPerformance::oneWayVariant_data()
{
    oneWay_data();
}

void tst_QDBusPerformance::oneWayVariant()
{
    QFETCH(QVariant, data);
    QFETCH(int, size);

    QVERIFY(executeTest("size", size, qVariantFromValue(QDBusVariant(data))));
}

void tst_QDBusPerformance::roundTrip_data()
{
    oneWay_data();
}

void tst_QDBusPerformance::roundTrip()
{
    QFETCH(QVariant, data);
    QFETCH(int, size);

    QVERIFY(executeTest("echo", size, data));
}

void tst_QDBusPerformance::roundTripVariant_data()
{
    oneWay_data();
}

void tst_QDBusPerformance::roundTripVariant()
{
    QFETCH(QVariant, data);
    QFETCH(int, size);

    QVERIFY(executeTest("echo", size, qVariantFromValue(QDBusVariant(data))));
}

QTEST_MAIN(tst_QDBusPerformance)

#include "moc_tst_qdbusperformance.cpp"
#include "moc_serverobject.h"
