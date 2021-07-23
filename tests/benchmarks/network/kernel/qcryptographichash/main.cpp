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

#include <qdebug.h>
#include <qcryptographichash.h>
#include <qtest.h>

QT_USE_NAMESPACE

static const QByteArray lorem = QByteArray("Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.");

class tst_qcryptographichash : public QObject
{
    Q_OBJECT
private slots:
    void append_data();
    void append();
    void append_once();
    void statichash();
    void algorithms_data();
    void algorithms();
};

void tst_qcryptographichash::append_data()
{
    QTest::addColumn<int>("size");
    QTest::newRow("10")  << int(10);
    QTest::newRow("100") << int(100);
    QTest::newRow("250") << int(250);
    QTest::newRow("500") << int(500);
}

void tst_qcryptographichash::append()
{
    QFETCH(int, size);

    const int chunksize = lorem.size() / size;
    QVERIFY(chunksize > 0);
    QList<QByteArray> chunks;
    int chunkposition = 0;
    for (int i = 0; i < chunksize; i++) {
        chunks.append(lorem.mid(chunkposition, chunksize));
        chunkposition += chunksize;
    }

    QBENCHMARK {
        QCryptographicHash hash(QCryptographicHash::Sha512);
        for (int i = 0; i < chunks.size(); i++) {
            hash.addData(chunks.at(i));
        }
        QVERIFY(!hash.result().isEmpty());
    }
}

void tst_qcryptographichash::append_once()
{
    QBENCHMARK {
        QCryptographicHash hash(QCryptographicHash::Sha512);
        hash.addData(lorem);
        QVERIFY(!hash.result().isEmpty());
    }
}

void tst_qcryptographichash::statichash() {
    QBENCHMARK {
        QByteArray hash = QCryptographicHash::hash(lorem, QCryptographicHash::Sha512);
        QVERIFY(!hash.isEmpty());
    }
}

void tst_qcryptographichash::algorithms_data()
{
    QTest::addColumn<QCryptographicHash::Algorithm>("algorithm");
    QTest::newRow("Md4")  << QCryptographicHash::Md4;
    QTest::newRow("Md5")  << QCryptographicHash::Md5;
    QTest::newRow("Sha1")  << QCryptographicHash::Sha1;
    QTest::newRow("Sha256")  << QCryptographicHash::Sha256;
    QTest::newRow("Sha512")  << QCryptographicHash::Sha512;
}

void tst_qcryptographichash::algorithms() {
    QFETCH(QCryptographicHash::Algorithm, algorithm);

    QBENCHMARK {
        QByteArray hash = QCryptographicHash::hash(lorem, algorithm);
        QVERIFY(!hash.isEmpty());
    }
}

QTEST_MAIN(tst_qcryptographichash)

Q_DECLARE_METATYPE(QCryptographicHash::Algorithm)

#include "moc_main.cpp"
