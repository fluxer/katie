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


#include <qfont.h>
#include <qfontdatabase.h>
#include <qfontinfo.h>
#include <qstringlist.h>
#include <qapplication.h>
#include <qwidget.h>
#include <qlist.h>
#include <qbuffer.h>
#include <qdebug.h>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QFont : public QObject
{
Q_OBJECT

public:
    tst_QFont();
    virtual ~tst_QFont();

public slots:
    void init();
    void cleanup();
private slots:
    void getSetCheck();
    void exactMatch();
    void compare();
    void resolve();
    void resetFont();
    void isCopyOf();
    void lastResortFamily();

    void toString_fromString_data();
    void toString_fromString();
};

// Testing get/set functions
void tst_QFont::getSetCheck()
{
    QFont obj1;
    // Style QFont::style()
    // void QFont::setStyle(Style)
    obj1.setStyle(QFont::Style(QFont::StyleNormal));
    QCOMPARE(QFont::Style(QFont::StyleNormal), obj1.style());
    obj1.setStyle(QFont::Style(QFont::StyleItalic));
    QCOMPARE(QFont::Style(QFont::StyleItalic), obj1.style());
    obj1.setStyle(QFont::Style(QFont::StyleOblique));
    QCOMPARE(QFont::Style(QFont::StyleOblique), obj1.style());

    // HintingPreference QFont::hintingPreference()
    // void QFont::setHintingPreference(HintingPreference)
    obj1.setHintingPreference(QFont::HintingPreference(QFont::PreferDefaultHinting));
    QCOMPARE(QFont::HintingPreference(QFont::PreferDefaultHinting), obj1.hintingPreference());
    obj1.setHintingPreference(QFont::HintingPreference(QFont::PreferNoHinting));
    QCOMPARE(QFont::HintingPreference(QFont::PreferNoHinting), obj1.hintingPreference());
}

tst_QFont::tst_QFont()
{
}

tst_QFont::~tst_QFont()
{

}

void tst_QFont::init()
{
    // TODO: Add initialization code here.
    // This will be executed immediately before each test is run.
}

void tst_QFont::cleanup()
{
    // TODO: Add cleanup code here.
    // This will be executed immediately after each test is run.
}

void tst_QFont::exactMatch()
{
    // Check if a non-existing font hasn't an exact match
    QFont font( "BogusFont", 33 );
    QVERIFY(!font.exactMatch() );

    QFontDatabase fdb;
    QVERIFY(!QFont("freesans").exactMatch());
    QVERIFY(!QFont("freeserif").exactMatch());
    QVERIFY(!QFont("freemono").exactMatch());
}

void tst_QFont::compare()
{
    QFont font;
    {
        QFont font2 = font;
        font2.setPointSize( 24 );
        QVERIFY( font != font2 );
        QCOMPARE(font < font2,!(font2 < font));
    }
    {
        QFont font2 = font;
        font2.setPixelSize( 24 );
        QVERIFY( font != font2 );
        QCOMPARE(font < font2,!(font2 < font));
    }

    font.setPointSize(12);
    font.setItalic(false);
    font.setWeight(QFont::Normal);
    font.setUnderline(false);
    font.setStrikeOut(false);
    font.setOverline(false);
    {
        QFont font2 = font;
        font2.setPointSize( 24 );
        QVERIFY( font != font2 );
        QCOMPARE(font < font2,!(font2 < font));
    }
    {
        QFont font2 = font;
        font2.setPixelSize( 24 );
        QVERIFY( font != font2 );
        QCOMPARE(font < font2,!(font2 < font));
    }
    {
        QFont font2 = font;

        font2.setItalic(true);
        QVERIFY( font != font2 );
        QCOMPARE(font < font2,!(font2 < font));
        font2.setItalic(false);
        QVERIFY( font == font2 );
        QVERIFY(!(font < font2));

        font2.setWeight(QFont::Bold);
        QVERIFY( font != font2 );
        QCOMPARE(font < font2,!(font2 < font));
        font2.setWeight(QFont::Normal);
        QVERIFY( font == font2 );
        QVERIFY(!(font < font2));

        font.setUnderline(true);
        QVERIFY( font != font2 );
        QCOMPARE(font < font2,!(font2 < font));
        font.setUnderline(false);
        QVERIFY( font == font2 );
        QVERIFY(!(font < font2));

        font.setStrikeOut(true);
        QVERIFY( font != font2 );
        QCOMPARE(font < font2,!(font2 < font));
        font.setStrikeOut(false);
        QVERIFY( font == font2 );
        QVERIFY(!(font < font2));

        font.setOverline(true);
        QVERIFY( font != font2 );
        QCOMPARE(font < font2,!(font2 < font));
        font.setOverline(false);
        QVERIFY( font == font2 );
        QVERIFY(!(font < font2));
    }
}

void tst_QFont::resolve()
{
    QFont font;
    font.setPointSize(font.pointSize() * 2);
    font.setItalic(false);
    font.setWeight(QFont::Normal);
    font.setUnderline(false);
    font.setStrikeOut(false);
    font.setOverline(false);
    font.setStretch(QFont::Unstretched);

    QFont font1;
    font1.setWeight(QFont::Bold);
    QFont font2 = font1.resolve(font);

    QVERIFY(font2.weight() == font1.weight());

    QVERIFY(font2.pointSize() == font.pointSize());
    QVERIFY(font2.italic() == font.italic());
    QVERIFY(font2.underline() == font.underline());
    QVERIFY(font2.overline() == font.overline());
    QVERIFY(font2.strikeOut() == font.strikeOut());
    QVERIFY(font2.stretch() == font.stretch());

    QFont font3;
    font3.setStretch(QFont::UltraCondensed);
    QFont font4 = font3.resolve(font1).resolve(font);

    QVERIFY(font4.stretch() == font3.stretch());

    QVERIFY(font4.weight() == font.weight());
    QVERIFY(font4.pointSize() == font.pointSize());
    QVERIFY(font4.italic() == font.italic());
    QVERIFY(font4.underline() == font.underline());
    QVERIFY(font4.overline() == font.overline());
    QVERIFY(font4.strikeOut() == font.strikeOut());


    QFont f1,f2,f3;
    f2.setPointSize(45);
    f3.setPointSize(55);

    QFont f4 = f1.resolve(f2);
    QCOMPARE(f4.pointSize(), 45);
    f4 = f4.resolve(f3);
    QCOMPARE(f4.pointSize(), 55);
}

void tst_QFont::resetFont()
{
    QWidget parent;
    QFont parentFont = parent.font();
    parentFont.setPointSize(parentFont.pointSize() + 2);
    parent.setFont(parentFont);

    QWidget *child = new QWidget(&parent);

    QFont childFont = child->font();
    childFont.setBold(!childFont.bold());
    child->setFont(childFont);

    QVERIFY(parentFont.resolve() != 0);
    QVERIFY(childFont.resolve() != 0);
    QVERIFY(childFont != parentFont);

    child->setFont(QFont()); // reset font

    QVERIFY(child->font().resolve() == 0);
    QVERIFY(child->font().pointSize() == parent.font().pointSize());
    QVERIFY(parent.font().resolve() != 0);
}

void tst_QFont::isCopyOf()
{
    QFont font;
    QVERIFY(font.isCopyOf(QApplication::font()));

    QFont font2("bogusfont",  23);
    QVERIFY(! font2.isCopyOf(QApplication::font()));

    QFont font3 = font;
    QVERIFY(font3.isCopyOf(font));

    font3.setPointSize(256);
    QVERIFY(!font3.isCopyOf(font));
    font3.setPointSize(font.pointSize());
    QVERIFY(!font3.isCopyOf(font));
}

void tst_QFont::lastResortFamily()
{
    QVERIFY(!QFont::lastResortFamily().isEmpty());
}

void tst_QFont::toString_fromString_data()
{
    QTest::addColumn<QString>("fontstring");
    QTest::addColumn<QString>("expectedstring");

    // actual font families
    QTest::newRow("FreeSans") << QString("FreeSans,10,-1,50,1,0,0,0") << QString("FreeSans,10,-1,50,1,0,0,0");
    QTest::newRow("FreeSans [GNU ]") << QString("FreeSans [GNU ],5,-1,99,0,1,0,0") << QString("FreeSans [GNU ],5,-1,99,0,1,0,0");
    QTest::newRow("FreeMono") << QString("FreeMono,5,-1,999,0,0,1,0") << QString("FreeMono,5,-1,99,0,0,1,0");
    QTest::newRow("FreeSans") << QString("FreeSans,10,-1,50,2,3,4,5") << QString("FreeSans,10,-1,50,2,1,1,1");
    // aliases
    QTest::newRow("Sans Serif") << QString("Sans Serif,123,-1,50,0,0,0,0") << QString("Sans Serif,123,-1,50,0,0,0,0");
    QTest::newRow("Monospace") << QString("Monospace,10,666,50,0,0,0,0") << QString("Monospace,-1,666,50,0,0,0,0");
}

void tst_QFont::toString_fromString()
{
    QFETCH(QString, fontstring);
    QFETCH(QString, expectedstring);

    QFont font;
    font.fromString(fontstring);
    QCOMPARE(expectedstring, font.toString());
}

QTEST_MAIN(tst_QFont)

#include "moc_tst_qfont.cpp"
