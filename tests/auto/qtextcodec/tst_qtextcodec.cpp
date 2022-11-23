/****************************************************************************
**
** Copyright (C) 2022 Ivailo Monev
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
#include <QtCore/QtCore>

//TESTED_CLASS=QTextCodec
//TESTED_FILES=qtextcodec.cpp,qtextcodec.h,qtextcodec_p.h

class tst_QTextCodec : public QObject
{
    Q_OBJECT

public slots:
    void init();
    void cleanup();

private slots:
    void mibEnum_data();
    void mibEnum();

    void hasFailure_data();
    void hasFailure();
};

void tst_QTextCodec::init()
{
}

void tst_QTextCodec::cleanup()
{
}

void tst_QTextCodec::mibEnum_data()
{
    QTest::addColumn<int>("mib");
    foreach (const QByteArray &codecname, QTextCodec::availableCodecs()) {
        QTextCodec* codec = QTextCodec::codecForName(codecname);
        QTest::newRow(codecname.constData()) << codec->mibEnum();
    }
}

void tst_QTextCodec::mibEnum()
{
    QFETCH(int, mib);

    const QTextCodec* codec = QTextCodec::codecForMib(mib);
    QVERIFY(codec != nullptr);
    QCOMPARE(codec->mibEnum(), mib);
    QVERIFY(codec->mibEnum() != 2);
}

void tst_QTextCodec::hasFailure_data()
{
    QTest::addColumn<int>("mib");
    QTest::addColumn<bool>("hasfailure");
    QTest::addColumn<QString>("data");

    QTest::newRow("latin1-from-latin1") << int(4) << false << QString("foo");
    QTest::newRow("latin1-from-utf8") << int(4) << true << QString::fromUtf8("бар");
}

void tst_QTextCodec::hasFailure()
{
    QFETCH(int, mib);
    QFETCH(bool, hasfailure);
    QFETCH(QString, data);

    QTextConverter converter(mib);
    const QByteArray result = converter.fromUnicode(data);
    QCOMPARE(converter.hasFailure(), hasfailure);
    if (hasfailure) {
        const QByteArray expected(data.size(), '?');
        QCOMPARE(result, expected);
    }
}

QTEST_MAIN(tst_QTextCodec)

#include "moc_tst_qtextcodec.cpp"
