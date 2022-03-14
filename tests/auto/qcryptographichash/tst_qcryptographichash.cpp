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
#include <QCryptographicHash>
#include <QDebug>

Q_DECLARE_METATYPE(QCryptographicHash::Algorithm)

class tst_QCryptographicHash : public QObject
{
    Q_OBJECT
private slots:
    void repeated_result_data();
    void repeated_result();
    void intermediary_result_data();
    void intermediary_result();
    void static_vs_incremental_result_data();
    void static_vs_incremental_result();
};

void tst_QCryptographicHash::repeated_result_data()
{
    intermediary_result_data();
}

void tst_QCryptographicHash::repeated_result()
{
    QFETCH(QCryptographicHash::Algorithm, algo);

    QCryptographicHash hash(algo);

    QFETCH(QByteArray, first);
    hash.addData(first);

    QFETCH(QByteArray, hash_first);
    QByteArray result = hash.result();
    QCOMPARE(result, hash_first);
    QCOMPARE(result, hash.result());

    hash.reset();
    hash.addData(first);
    result = hash.result();
    QCOMPARE(result, hash_first);
    QCOMPARE(result, hash.result());
}

void tst_QCryptographicHash::intermediary_result_data()
{
    QTest::addColumn<QCryptographicHash::Algorithm>("algo");
    QTest::addColumn<QByteArray>("first");
    QTest::addColumn<QByteArray>("second");
    QTest::addColumn<QByteArray>("hash_first");
    QTest::addColumn<QByteArray>("hash_firstsecond");

    QTest::newRow("md5") << QCryptographicHash::Md5
                         << QByteArray("abc") << QByteArray("abc")
                         << QByteArray::fromHex("900150983CD24FB0D6963F7D28E17F72")
                         << QByteArray::fromHex("440AC85892CA43AD26D44C7AD9D47D3E");
    QTest::newRow("sha1") << QCryptographicHash::Sha1
                         << QByteArray("abc") << QByteArray("abc")
                         << QByteArray::fromHex("A9993E364706816ABA3E25717850C26C9CD0D89D")
                         << QByteArray::fromHex("F8C1D87006FBF7E5CC4B026C3138BC046883DC71");

    QTest::newRow("sha256") << QCryptographicHash::Sha256
                         << QByteArray("abc") << QByteArray("abc")
                         << QByteArray::fromHex("BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD")
                         << QByteArray::fromHex("BBB59DA3AF939F7AF5F360F2CEB80A496E3BAE1CD87DDE426DB0AE40677E1C2C");

    QTest::newRow("sha512") << QCryptographicHash::Sha512
                         << QByteArray("abc") << QByteArray("abc")
                         << QByteArray::fromHex("DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A2192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F")
                         << QByteArray::fromHex("F3C41E7B63EE869596FC28BAD64120612C520F65928AB4D126C72C6998B551B8FF1CEDDFED4373E6717554DC89D1EEE6F0AB22FD3675E561ABA9AE26A3EEC53B");
    QTest::newRow("KAT") << QCryptographicHash::KAT
                         << QByteArray("abc") << QByteArray("abc")
                         << QByteArray::fromHex("a96faf705af16834e6c632b61e964e1f4b2212e31ac97fd4575a0b1c44d8843f")
                         << QByteArray::fromHex("b9fe94d346d39b20369242a646a19333db1de4b3da6c7871b776d5cb968aa5a4");
}

void tst_QCryptographicHash::intermediary_result()
{
    QFETCH(QCryptographicHash::Algorithm, algo);

    QCryptographicHash hash(algo);

    QFETCH(QByteArray, first);
    hash.addData(first);

    QFETCH(QByteArray, hash_first);
    QByteArray result = hash.result();
    // qDebug() << result.toHex();
    QCOMPARE(result, hash_first);

    // don't reset
    QFETCH(QByteArray, second);
    QFETCH(QByteArray, hash_firstsecond);
    hash.addData(second);

    result = hash.result();
    // qDebug() << result.toHex();
    QCOMPARE(result, hash_firstsecond);
}

void tst_QCryptographicHash::static_vs_incremental_result_data()
{
    QTest::addColumn<QCryptographicHash::Algorithm>("algo");
    QTest::addColumn<QByteArray>("first");
    QTest::addColumn<QByteArray>("second");
    QTest::addColumn<QByteArray>("third");

    QTest::newRow("123 (Md5)")          << QCryptographicHash::Md5 << QByteArray("1") << QByteArray("2") << QByteArray("3");
    QTest::newRow("1bar3 (Md5)")        << QCryptographicHash::Md5 << QByteArray("1") << QByteArray("bar") << QByteArray("3");
    QTest::newRow("12baz (Md5)")        << QCryptographicHash::Md5 << QByteArray("1") << QByteArray("2") << QByteArray("baz");
    QTest::newRow("foobarbaz (Md5)")    << QCryptographicHash::Md5 << QByteArray("foo") << QByteArray("bar") << QByteArray("baz");

    QTest::newRow("123 (Sha1)")         << QCryptographicHash::Sha1 << QByteArray("1") << QByteArray("2") << QByteArray("3");
    QTest::newRow("1bar3 (Sha1)")       << QCryptographicHash::Sha1 << QByteArray("1") << QByteArray("bar") << QByteArray("3");
    QTest::newRow("12baz (Sha1)")       << QCryptographicHash::Sha1 << QByteArray("1") << QByteArray("2") << QByteArray("baz");
    QTest::newRow("foobarbaz (Sha1)")   << QCryptographicHash::Sha1 << QByteArray("foo") << QByteArray("bar") << QByteArray("baz");

    QTest::newRow("123 (Sha256)")       << QCryptographicHash::Sha256 << QByteArray("1") << QByteArray("2") << QByteArray("3");
    QTest::newRow("1bar3 (Sha256)")     << QCryptographicHash::Sha256 << QByteArray("1") << QByteArray("bar") << QByteArray("3");
    QTest::newRow("12baz (Sha256)")     << QCryptographicHash::Sha256 << QByteArray("1") << QByteArray("2") << QByteArray("baz");
    QTest::newRow("foobarbaz (Sha256)") << QCryptographicHash::Sha256 << QByteArray("foo") << QByteArray("bar") << QByteArray("baz");

    QTest::newRow("123 (Sha512)")       << QCryptographicHash::Sha512 << QByteArray("1") << QByteArray("2") << QByteArray("3");
    QTest::newRow("1bar3 (Sha512)")     << QCryptographicHash::Sha512 << QByteArray("1") << QByteArray("bar") << QByteArray("3");
    QTest::newRow("12baz (Sha512)")     << QCryptographicHash::Sha512 << QByteArray("1") << QByteArray("2") << QByteArray("baz");
    QTest::newRow("foobarbaz (Sha512)") << QCryptographicHash::Sha512 << QByteArray("foo") << QByteArray("bar") << QByteArray("baz");

    QTest::newRow("123 (KAT)")          << QCryptographicHash::KAT << QByteArray("1") << QByteArray("2") << QByteArray("3");
    QTest::newRow("1bar3 (KAT)")        << QCryptographicHash::KAT << QByteArray("1") << QByteArray("bar") << QByteArray("3");
    QTest::newRow("12baz (KAT)")        << QCryptographicHash::KAT << QByteArray("1") << QByteArray("2") << QByteArray("baz");
    QTest::newRow("foobarbaz (KAT)")    << QCryptographicHash::KAT << QByteArray("foo") << QByteArray("bar") << QByteArray("baz");
}

void tst_QCryptographicHash::static_vs_incremental_result()
{
    QFETCH(QCryptographicHash::Algorithm, algo);
    QFETCH(QByteArray, first);
    QFETCH(QByteArray, second);
    QFETCH(QByteArray, third);

    QCryptographicHash incrementalhash(algo);
    incrementalhash.addData(first);
    incrementalhash.addData(second);
    incrementalhash.addData(third);
    const QByteArray incrementalresult = incrementalhash.result();

    const QByteArray staticresult = QCryptographicHash::hash(first + second + third, algo);

    // qDebug() << incrementalresult.toHex() << staticresult.toHex();
    if (algo == QCryptographicHash::KAT) {
        QEXPECT_FAIL("", "This is expected behaviour currently", Continue);
    }
    QCOMPARE(incrementalresult, staticresult);
}

QTEST_MAIN(tst_QCryptographicHash)

#include "moc_tst_qcryptographichash.cpp"
