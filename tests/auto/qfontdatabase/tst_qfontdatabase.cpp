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

#include <qfontdatabase.h>
#include <qdir.h>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QFontDatabase : public QObject
{
Q_OBJECT

public:
    tst_QFontDatabase();
    virtual ~tst_QFontDatabase();

public slots:
    void init();
    void cleanup();
private slots:
    void styles_data();
    void styles();

    void fixedPitch_data();
    void fixedPitch();

    void widthTwoTimes_data();
    void widthTwoTimes();

    void addAppFont_data();
    void addAppFont();
};

tst_QFontDatabase::tst_QFontDatabase()
{
    QDir::setCurrent(SRCDIR);
}

tst_QFontDatabase::~tst_QFontDatabase()
{

}

void tst_QFontDatabase::init()
{
// TODO: Add initialization code here.
// This will be executed immediately before each test is run.
}

void tst_QFontDatabase::cleanup()
{
// TODO: Add cleanup code here.
// This will be executed immediately after each test is run.
}

void tst_QFontDatabase::styles_data()
{
    QTest::addColumn<QString>("font");

    QTest::newRow( "data0" ) << QString( "Times New Roman" );
}

void tst_QFontDatabase::styles()
{
    QFETCH( QString, font );

    QFontDatabase fdb;
    QStringList styles = fdb.styles( font );
    QStringList::Iterator it = styles.begin();
    while ( it != styles.end() ) {
        QString style = *it;
        QString trimmed = style.trimmed();
        ++it;

        QCOMPARE( style, trimmed );
    }
}

void tst_QFontDatabase::fixedPitch_data()
{
    QTest::addColumn<QString>("font");
    QTest::addColumn<bool>("fixedPitch");

    QTest::newRow( "Times New Roman" ) << QString( "Times New Roman" ) << false;
    QTest::newRow( "Arial" ) << QString( "Arial" ) << false;
    QTest::newRow( "Andale Mono" ) << QString( "Andale Mono" ) << true;
    QTest::newRow( "Courier" ) << QString( "Courier" ) << true;
    QTest::newRow( "Courier New" ) << QString( "Courier New" ) << true;
    QTest::newRow( "Script" ) << QString( "Script" ) << false;
    QTest::newRow( "Lucida Console" ) << QString( "Lucida Console" ) << true;
}

void tst_QFontDatabase::fixedPitch()
{
    QFETCH(QString, font);
    QFETCH(bool, fixedPitch);

    QFontDatabase fdb;
    // qDebug() << fdb.families();
    if (!fdb.families().contains(font))
        QSKIP( "Font not installed", SkipSingle);

    QCOMPARE(fdb.isFixedPitch(font), fixedPitch);

    QFont qfont(font);
    QFontInfo fi(qfont);
    QCOMPARE(fi.fixedPitch(), fixedPitch);
}

void tst_QFontDatabase::widthTwoTimes_data()
{
    QTest::addColumn<QString>("font");
    QTest::addColumn<int>("pixelSize");
    QTest::addColumn<QString>("text");

    QTest::newRow("Arial") << QString("Arial") << 1000 << QString("Some text");
}

void tst_QFontDatabase::widthTwoTimes()
{
    QFETCH(QString, font);
    QFETCH(int, pixelSize);
    QFETCH(QString, text);

    QFont f;
    f.setFamily(font);
    f.setPixelSize(pixelSize);

    QFontMetrics fm(f);
    int w1 = fm.charWidth(text, 0);
    int w2 = fm.charWidth(text, 0);

    QCOMPARE(w1, w2);
}

void tst_QFontDatabase::addAppFont_data()
{
    QTest::addColumn<bool>("useMemoryFont");
    QTest::newRow("font file") << false;
    QTest::newRow("memory font") << true;
}

void tst_QFontDatabase::addAppFont()
{
    QFETCH(bool, useMemoryFont);
    QSignalSpy fontDbChangedSpy(QApplication::instance(), SIGNAL(fontDatabaseChanged()));

    QFontDatabase db;

    const QStringList oldFamilies = db.families();
    QVERIFY(!oldFamilies.isEmpty());

    fontDbChangedSpy.clear();

    int id;
    if (useMemoryFont) {
        QFile fontfile("FreeMono.ttf");
        fontfile.open(QIODevice::ReadOnly);
        QByteArray fontdata = fontfile.readAll();
        QVERIFY(!fontdata.isEmpty());
        id = QFontDatabase::addApplicationFontFromData(fontdata);
    } else {
        id = QFontDatabase::addApplicationFont("FreeMono.ttf");
    }
    QCOMPARE(fontDbChangedSpy.count(), 1);
    if (id == -1) {
        QSKIP("Skip the test since app fonts are not supported on this system", SkipSingle);
        return;
    }

    const QStringList addedFamilies = QFontDatabase::applicationFontFamilies(id);
    QVERIFY(!addedFamilies.isEmpty());

    const QStringList newFamilies = db.families();
    QVERIFY(!newFamilies.isEmpty());
    QVERIFY(newFamilies.count() >= oldFamilies.count());

    for (int i = 0; i < addedFamilies.count(); ++i)
        QVERIFY(newFamilies.contains(addedFamilies.at(i)));

    QVERIFY(QFontDatabase::removeApplicationFont(id));
    QCOMPARE(fontDbChangedSpy.count(), 2);

    QVERIFY(db.families() == oldFamilies);
}

QTEST_MAIN(tst_QFontDatabase)

#include "moc_tst_qfontdatabase.cpp"
