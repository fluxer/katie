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

private slots:
    void styles_data();
    void styles();

    void fixedPitch_data();
    void fixedPitch();

    void widthTwoTimes_data();
    void widthTwoTimes();

    void resolveFamily_data();
    void resolveFamily();
};

tst_QFontDatabase::tst_QFontDatabase()
{
    QDir::setCurrent(SRCDIR);
}

tst_QFontDatabase::~tst_QFontDatabase()
{

}
void tst_QFontDatabase::styles_data()
{
    QTest::addColumn<QString>("font");

    QTest::newRow("data0") << QString("FreeSerif [GNU]");
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

    QTest::newRow("FreeSans") << QString("FreeSans") << false;
    QTest::newRow("FreeMono") << QString("FreeMono") << true;
}

void tst_QFontDatabase::fixedPitch()
{
    QFETCH(QString, font);
    QFETCH(bool, fixedPitch);

    QFontDatabase fdb;
    // qDebug() << fdb.families();
    if (!fdb.hasFamily(font)) {
        QSKIP("Font not installed", SkipSingle);
    }

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

    QTest::newRow("FreeSans") << QString("FreeSans") << 1000 << QString("Some text");
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
    int w1 = fm.width(text.at(0));
    int w2 = fm.width(text.at(0));

    QCOMPARE(w1, w2);
}

void tst_QFontDatabase::resolveFamily_data()
{
    QTest::addColumn<QString>("alias");
    QTest::addColumn<QString>("resolved");

    QTest::newRow("Sans Serif") << QString("Sans Serif") << QString("FreeSans");
    QTest::newRow("Monospace") << QString("Monospace") << QString("FreeMono");
}

void tst_QFontDatabase::resolveFamily()
{
    QFETCH(QString, alias);
    QFETCH(QString, resolved);

    if (!QFontDatabase::supportsThreadedFontRendering()) {
        QSKIP("Font resolution is not supported", SkipSingle);
    }

    QFontDatabase fdb;
    // qDebug() << fdb.families();
    if (!fdb.hasFamily(resolved)) {
        QSKIP("Font not installed", SkipSingle);
    }

    QVERIFY(fdb.hasFamily(alias));
}

QTEST_MAIN(tst_QFontDatabase)

#include "moc_tst_qfontdatabase.cpp"
