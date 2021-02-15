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
    void italicOblique();
    void insertAndRemoveSubstitutions();
    void serializeSpacing();
    void lastResortFont();
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

    // StyleStrategy QFont::styleStrategy()
    // void QFont::setStyleStrategy(StyleStrategy)
    obj1.setStyleStrategy(QFont::StyleStrategy(QFont::PreferDefault));
    QCOMPARE(QFont::StyleStrategy(QFont::PreferDefault), obj1.styleStrategy());
    obj1.setStyleStrategy(QFont::StyleStrategy(QFont::PreferBitmap));
    QCOMPARE(QFont::StyleStrategy(QFont::PreferBitmap), obj1.styleStrategy());
    obj1.setStyleStrategy(QFont::StyleStrategy(QFont::PreferDevice));
    QCOMPARE(QFont::StyleStrategy(QFont::PreferDevice), obj1.styleStrategy());
    obj1.setStyleStrategy(QFont::StyleStrategy(QFont::PreferOutline));
    QCOMPARE(QFont::StyleStrategy(QFont::PreferOutline), obj1.styleStrategy());
    obj1.setStyleStrategy(QFont::StyleStrategy(QFont::ForceOutline));
    QCOMPARE(QFont::StyleStrategy(QFont::ForceOutline), obj1.styleStrategy());
    obj1.setStyleStrategy(QFont::StyleStrategy(QFont::PreferMatch));
    QCOMPARE(QFont::StyleStrategy(QFont::PreferMatch), obj1.styleStrategy());
    obj1.setStyleStrategy(QFont::StyleStrategy(QFont::PreferQuality));
    QCOMPARE(QFont::StyleStrategy(QFont::PreferQuality), obj1.styleStrategy());
    obj1.setStyleStrategy(QFont::StyleStrategy(QFont::PreferAntialias));
    QCOMPARE(QFont::StyleStrategy(QFont::PreferAntialias), obj1.styleStrategy());
    obj1.setStyleStrategy(QFont::StyleStrategy(QFont::NoAntialias));
    QCOMPARE(QFont::StyleStrategy(QFont::NoAntialias), obj1.styleStrategy());
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

#ifdef Q_WS_X11
    QFontDatabase fdb;
    QVERIFY(!QFont("sans-serif").exactMatch());
    QVERIFY(!QFont("sans").exactMatch());
    QVERIFY(!QFont("serif").exactMatch());
    QVERIFY(!QFont("monospace").exactMatch());
#endif
}

void tst_QFont::italicOblique()
{
    QFontDatabase fdb;

    QStringList families = fdb.families();
    if (families.isEmpty())
        return;

    QStringList::ConstIterator f_it, f_end = families.end();
    for (f_it = families.begin(); f_it != f_end; ++f_it) {

        QString family = *f_it;
        QStringList styles = fdb.styles(family);
        QVERIFY(!styles.isEmpty());
        QStringList::ConstIterator s_it, s_end = styles.end();
        for (s_it = styles.begin(); s_it != s_end; ++s_it) {
            QString style = *s_it;

            if (fdb.isSmoothlyScalable(family, style)) {
                if (style.contains("Oblique")) {
                    style.replace("Oblique", "Italic");
                } else if (style.contains("Italic")) {
                    style.replace("Italic", "Oblique");
                } else {
                    continue;
                }
                QFont f = fdb.font(family, style, 12);
                QVERIFY(f.italic());
            }
        }
    }
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

        font.setCapitalization(QFont::SmallCaps);
        QVERIFY( font != font2 );
        QCOMPARE(font < font2,!(font2 < font));
        font.setCapitalization(QFont::MixedCase);
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

void tst_QFont::insertAndRemoveSubstitutions()
{
    QFont::removeSubstitution("BogusFontFamily");
    // make sure it is empty before we start
    QVERIFY(QFont::substitutes("BogusFontFamily").isEmpty());
    QVERIFY(QFont::substitutes("bogusfontfamily").isEmpty());

    // inserting Foo
    QFont::insertSubstitution("BogusFontFamily", "Foo");
    QCOMPARE(QFont::substitutes("BogusFontFamily").count(), 1);
    QCOMPARE(QFont::substitutes("bogusfontfamily").count(), 1);

    // inserting Bar and Baz
    QStringList moreFonts;
    moreFonts << "Bar" << "Baz";
    QFont::insertSubstitutions("BogusFontFamily", moreFonts);
    QCOMPARE(QFont::substitutes("BogusFontFamily").count(), 3);
    QCOMPARE(QFont::substitutes("bogusfontfamily").count(), 3);

    QFont::removeSubstitution("BogusFontFamily");
    // make sure it is empty again
    QVERIFY(QFont::substitutes("BogusFontFamily").isEmpty());
    QVERIFY(QFont::substitutes("bogusfontfamily").isEmpty());
}


static QFont copyFont(const QFont &font1) // copy using a QDataStream
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QDataStream ds(&buffer);
    ds << font1;
    buffer.close();

    buffer.open(QIODevice::ReadOnly);
    QFont font2;
    ds >> font2;
    return font2;
}

void tst_QFont::serializeSpacing()
{
    QFont font;
    QCOMPARE(font.letterSpacing(), 0.);
    QCOMPARE(font.wordSpacing(), 0.);

    font.setLetterSpacing(QFont::AbsoluteSpacing, 105);
    QCOMPARE(font.letterSpacing(), 105.);
    QCOMPARE(font.letterSpacingType(), QFont::AbsoluteSpacing);
    QCOMPARE(font.wordSpacing(), 0.);
    QFont font2 = copyFont(font);
    QCOMPARE(font2.letterSpacing(), 105.);
    QCOMPARE(font2.letterSpacingType(), QFont::AbsoluteSpacing);
    QCOMPARE(font2.wordSpacing(), 0.);

    font.setWordSpacing(50.0);
    QCOMPARE(font.letterSpacing(), 105.);
    QCOMPARE(font.wordSpacing(), 50.);

    QFont font3 = copyFont(font);
    QCOMPARE(font3.letterSpacing(), 105.);
    QCOMPARE(font3.letterSpacingType(), QFont::AbsoluteSpacing);
    QCOMPARE(font3.wordSpacing(), 50.);
}

void tst_QFont::lastResortFont()
{
    QFont font;
    QVERIFY(!font.lastResortFont().isEmpty());
}

QTEST_MAIN(tst_QFont)

#include "moc_tst_qfont.cpp"
