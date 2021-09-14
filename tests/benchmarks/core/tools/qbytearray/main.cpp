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
#include <QString>
#include <qtest.h>

QT_USE_NAMESPACE

static const QByteArray lorem = QByteArray("Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.");

class tst_qbytearray : public QObject
{
    Q_OBJECT
private slots:
    void append();
    void append_data();
    void compress_uncompress_data();
    void compress_uncompress();
    void to_from_base64();
    void to_from_hex();
    void bench_qstrcmp();
    void bench_qstrncmp();
};


void tst_qbytearray::append_data()
{
    QTest::addColumn<int>("size");
    QTest::newRow("1")         << int(1);
    QTest::newRow("10")        << int(10);
    QTest::newRow("100")       << int(100);
    QTest::newRow("1000")      << int(1000);
    QTest::newRow("10000")     << int(10000);
    QTest::newRow("100000")    << int(100000);
    QTest::newRow("1000000")   << int(1000000);
    QTest::newRow("10000000")  << int(10000000);
    QTest::newRow("100000000") << int(100000000);
}

void tst_qbytearray::append()
{
    QFETCH(int, size);

    QByteArray ba;
    QBENCHMARK {
        QByteArray ba2(size, 'x');
        ba.append(ba2);
        ba.clear();
    }
}

void tst_qbytearray::compress_uncompress_data()
{
    QTest::addColumn<int>("level");
    QTest::newRow("-1") << int(-1);
    QTest::newRow("0")  << int(0);
    QTest::newRow("1")  << int(1);
    QTest::newRow("2")  << int(2);
    QTest::newRow("3")  << int(3);
    QTest::newRow("4")  << int(4);
    QTest::newRow("5")  << int(5);
    QTest::newRow("6")  << int(6);
    QTest::newRow("7")  << int(7);
    QTest::newRow("8")  << int(8);
    QTest::newRow("9")  << int(9);
}

void tst_qbytearray::compress_uncompress()
{
    QFETCH(int, level);

    QBENCHMARK {
        QByteArray compressed = qCompress(lorem, level);
        QVERIFY(!compressed.isEmpty());
        QByteArray decompressed = qUncompress(compressed);
        QVERIFY(decompressed == lorem);
    }
}

void tst_qbytearray::to_from_base64() {
    QBENCHMARK {
        QByteArray base64 = lorem.toBase64();
        QVERIFY(!base64.isEmpty());
        QByteArray original = QByteArray::fromBase64(base64);
        QVERIFY(original == lorem);
    }
}

void tst_qbytearray::to_from_hex() {
    QBENCHMARK {
        QByteArray hex = lorem.toHex();
        QVERIFY(!hex.isEmpty());
        QByteArray original = QByteArray::fromHex(hex);
        QVERIFY(original == lorem);
    }
}

void tst_qbytearray::bench_qstrcmp() {
    QBENCHMARK {
        const int result = qstrcmp(lorem.constData(), lorem.constData());
        Q_UNUSED(result);
    }
}

void tst_qbytearray::bench_qstrncmp() {
    static const int loremsize = lorem.size();
    QBENCHMARK {
        const int result = qstrncmp(lorem.constData(), lorem.constData(), loremsize);
        Q_UNUSED(result);
    }
}

QTEST_MAIN(tst_qbytearray)

#include "moc_main.cpp"
