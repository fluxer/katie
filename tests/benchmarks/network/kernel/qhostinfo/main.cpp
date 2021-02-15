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


#include <QDebug>
#include <QHostInfo>
#include <QStringList>
#include <QString>

#include <qtest.h>
#include <qtesteventloop.h>

#include "qhostinfo_p.h"

class tst_qhostinfo : public QObject
{
    Q_OBJECT
public slots:
    void init();
private slots:
    void lookupSpeed_data();
    void lookupSpeed();
};

class SignalReceiver : public QObject
{
    Q_OBJECT
public:
    SignalReceiver(int nrc) : receiveCount(0), neededReceiveCount(nrc) {};
    int receiveCount;
    int neededReceiveCount;
public slots:
    void resultsReady(const QHostInfo) {
        receiveCount++;
        if (receiveCount == neededReceiveCount)
            QTestEventLoop::instance().exitLoop();
    }
};

void tst_qhostinfo::init()
{
    // delete the cache so inidividual testcase results are independant from each other
    qt_qhostinfo_clear_cache();
}

void tst_qhostinfo::lookupSpeed_data()
{
    QTest::addColumn<bool>("cache");
    QTest::newRow("WithCache") << true;
    QTest::newRow("WithoutCache") << false;
}

void tst_qhostinfo::lookupSpeed()
{
    QFETCH(bool, cache);
    qt_qhostinfo_enable_cache(cache);

    QStringList hostnameList;
    hostnameList << QLatin1String("www.nokia.com")
                 << QLatin1String("www.trolltech.com")
                 << QLatin1String("www.qtcentre.org")
                 << QLatin1String("www.nokia.no")
                 << QLatin1String("nokia.de")
                 << QLatin1String("127.0.0.1")
                 << QLatin1String("----");
    // also add some duplicates:
    hostnameList << QLatin1String("www.nokia.com")
                 << QLatin1String("127.0.0.1")
                 << QLatin1String("www.trolltech.com");
    // and some more
    hostnameList << hostnameList;

    const int COUNT = hostnameList.size();

    SignalReceiver receiver(COUNT);

    QBENCHMARK {
        for (int i = 0; i < hostnameList.size(); i++)
            QHostInfo::lookupHost(hostnameList.at(i), &receiver, SLOT(resultsReady(const QHostInfo)));
        QTestEventLoop::instance().enterLoop(20);
        QVERIFY(receiver.receiveCount == COUNT);
    }
}


QTEST_MAIN(tst_qhostinfo)

#include "moc_main.cpp"
