/****************************************************************************
**
** Copyright (C) 2019 Ivailo Monev
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

#include <qdebug.h>
#include <qcryptographichash.h>
#include <qtest.h>

QT_USE_NAMESPACE

static const QByteArray lorem = QByteArray("Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.");

Q_DECLARE_METATYPE(QCryptographicHash::Algorithm)

class tst_qcryptographichash : public QObject
{
    Q_OBJECT
private slots:
    void append_data();
    void append();
    void append_once_data();
    void append_once();
    void statichash_data();
    void statichash();
};

void tst_qcryptographichash::append_data()
{
    QTest::addColumn<int>("size");
    QTest::addColumn<QCryptographicHash::Algorithm>("algorithm");

    QTest::newRow("10 (Md5)")      << int(10)   << QCryptographicHash::Md5;
    QTest::newRow("10 (Sha1)")     << int(10)   << QCryptographicHash::Sha1;
    QTest::newRow("10 (Sha256)")   << int(10)   << QCryptographicHash::Sha256;
    QTest::newRow("10 (Sha512)")   << int(10)   << QCryptographicHash::Sha512;
    QTest::newRow("10 (BLAKE3)")  << int(10)   << QCryptographicHash::BLAKE3;

    QTest::newRow("100 (Md5)")     << int(100)  << QCryptographicHash::Md5;
    QTest::newRow("100 (Sha1)")    << int(100)  << QCryptographicHash::Sha1;
    QTest::newRow("100 (Sha256)")  << int(100)  << QCryptographicHash::Sha256;
    QTest::newRow("100 (Sha512)")  << int(100)  << QCryptographicHash::Sha512;
    QTest::newRow("100 (BLAKE3)") << int(100)  << QCryptographicHash::BLAKE3;

    QTest::newRow("250 (Md5)")     << int(250)  << QCryptographicHash::Md5;
    QTest::newRow("250 (Sha1)")    << int(250)  << QCryptographicHash::Sha1;
    QTest::newRow("250 (Sha256)")  << int(250)  << QCryptographicHash::Sha256;
    QTest::newRow("250 (Sha512)")  << int(250)  << QCryptographicHash::Sha512;
    QTest::newRow("250 (BLAKE3)") << int(250)  << QCryptographicHash::BLAKE3;

    QTest::newRow("500 (Md5)")     << int(500)  << QCryptographicHash::Md5;
    QTest::newRow("500 (Sha1)")    << int(500)  << QCryptographicHash::Sha1;
    QTest::newRow("500 (Sha256)")  << int(500)  << QCryptographicHash::Sha256;
    QTest::newRow("500 (Sha512)")  << int(500)  << QCryptographicHash::Sha512;
    QTest::newRow("500 (BLAKE3)") << int(500)  << QCryptographicHash::BLAKE3;
}

void tst_qcryptographichash::append()
{
    QFETCH(int, size);
    QFETCH(QCryptographicHash::Algorithm, algorithm);

    const int chunksize = lorem.size() / size;
    QVERIFY(chunksize > 0);
    QList<QByteArray> chunks;
    int chunkposition = 0;
    for (int i = 0; i < chunksize; i++) {
        chunks.append(lorem.mid(chunkposition, chunksize));
        chunkposition += chunksize;
    }

    QBENCHMARK {
        QCryptographicHash hash(algorithm);
        for (int i = 0; i < chunks.size(); i++) {
            hash.addData(chunks.at(i));
        }
        QVERIFY(!hash.result().isEmpty());
    }
}

void tst_qcryptographichash::append_once_data()
{
    QTest::addColumn<QCryptographicHash::Algorithm>("algorithm");

    QTest::newRow("Md5")    << QCryptographicHash::Md5;
    QTest::newRow("Sha1")   << QCryptographicHash::Sha1;
    QTest::newRow("Sha256") << QCryptographicHash::Sha256;
    QTest::newRow("Sha512") << QCryptographicHash::Sha512;
    QTest::newRow("BLAKE3") << QCryptographicHash::BLAKE3;
}

void tst_qcryptographichash::append_once()
{
    QFETCH(QCryptographicHash::Algorithm, algorithm);

    QBENCHMARK {
        QCryptographicHash hash(algorithm);
        hash.addData(lorem);
        QVERIFY(!hash.result().isEmpty());
    }
}

void tst_qcryptographichash::statichash_data()
{
    append_once_data();
}

void tst_qcryptographichash::statichash()
{
    QFETCH(QCryptographicHash::Algorithm, algorithm);

    QBENCHMARK {
        QByteArray hash = QCryptographicHash::hash(lorem, algorithm);
        QVERIFY(!hash.isEmpty());
    }
}

QTEST_MAIN(tst_qcryptographichash)

#include "moc_main.cpp"
