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

class tst_QRand: public QObject
{
    Q_OBJECT
private slots:
    void random_data();
    void random();
};

void tst_QRand::random_data()
{
    QTest::addColumn<bool>("seed");

    QTest::newRow("with_seed") << true;
    QTest::newRow("without_seed") << false;
}

void tst_QRand::random()
{
    QFETCH(bool, seed);

    static int lastvalue = 0;
    static const int numcalls = 10000;
    for (int i = 0; i < numcalls; i++) {
        if (seed) {
            qsrand(i);
        }

        const int value = qrand();
        QVERIFY(lastvalue != value);
        lastvalue = value;
    }
}

QTEST_MAIN(tst_QRand)

#include "moc_tst_qrand.cpp"
