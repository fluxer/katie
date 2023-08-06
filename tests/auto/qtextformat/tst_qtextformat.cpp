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
#include <qcoreapplication.h>
#include <qdebug.h>
#include <qtextformat.h>
#include <qtextdocument.h>
#include <qtextcursor.h>
#include <qtextobject.h>
#include <qtextlayout.h>
#include <qabstracttextdocumentlayout.h>

#include <limits.h>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QTextFormat : public QObject
{
Q_OBJECT

private slots:
    void getSetCheck();
    void defaultAlignment();
    void testUnderlinePropertyPrecedence();
    void toFormat();
    void resolveFont();
    void testFontStyleSetters();
};

// Testing get/set functions
void tst_QTextFormat::getSetCheck()
{
    QTextFormat obj1;
    // int QTextFormat::objectIndex()
    // void QTextFormat::setObjectIndex(int)
    obj1.setObjectIndex(0);
    QCOMPARE(0, obj1.objectIndex());
    obj1.setObjectIndex(INT_MIN);
    QCOMPARE(INT_MIN, obj1.objectIndex());
    obj1.setObjectIndex(INT_MAX);
    QCOMPARE(INT_MAX, obj1.objectIndex());
}

void tst_QTextFormat::defaultAlignment()
{
    QTextBlockFormat fmt;
    QVERIFY(!fmt.hasProperty(QTextFormat::BlockAlignment));
    QCOMPARE(fmt.intProperty(QTextFormat::BlockAlignment), 0);
    QVERIFY(fmt.alignment() == Qt::AlignLeft);
}

void tst_QTextFormat::testUnderlinePropertyPrecedence()
{
    QTextCharFormat format;
    // use normal accessors and check internal state
    format.setUnderlineStyle(QTextCharFormat::NoUnderline);
    QCOMPARE(format.property(QTextFormat::FontUnderline).isNull(), false);
    QCOMPARE(format.property(QTextFormat::TextUnderlineStyle).isNull(), false);
    QCOMPARE(format.property(QTextFormat::FontUnderline).toBool(), false);
    QCOMPARE(format.property(QTextFormat::TextUnderlineStyle).toInt(), 0);

    format = QTextCharFormat();
    format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    QCOMPARE(format.property(QTextFormat::FontUnderline).isNull(), false);
    QCOMPARE(format.property(QTextFormat::TextUnderlineStyle).isNull(), false);
    QCOMPARE(format.property(QTextFormat::FontUnderline).toBool(), true);
    QCOMPARE(format.property(QTextFormat::TextUnderlineStyle).toInt(), 1);

    format = QTextCharFormat();
    format.setUnderlineStyle(QTextCharFormat::DotLine);
    QCOMPARE(format.property(QTextFormat::FontUnderline).isNull(), false);
    QCOMPARE(format.property(QTextFormat::TextUnderlineStyle).isNull(), false);
    QCOMPARE(format.property(QTextFormat::FontUnderline).toBool(), false);
    QVERIFY(format.property(QTextFormat::TextUnderlineStyle).toInt() > 0);

    // override accessors and use setProperty to create a false state.
    // then check font()
    format = QTextCharFormat();
    format.setProperty(QTextCharFormat::FontUnderline, true);
    QCOMPARE(format.property(QTextFormat::FontUnderline).isNull(), false);
    QCOMPARE(format.property(QTextFormat::TextUnderlineStyle).isNull(), true);
    QCOMPARE(format.fontUnderline(), true);
    QCOMPARE(format.font().underline(), true);

    format = QTextCharFormat();
    // create conflict. Should use the new property
    format.setProperty(QTextCharFormat::TextUnderlineStyle, QTextCharFormat::SingleUnderline);
    format.setProperty(QTextCharFormat::FontUnderline, false);
    QCOMPARE(format.fontUnderline(), true);
    QCOMPARE(format.font().underline(), true);

    format = QTextCharFormat();
    // create conflict. Should use the new property
    format.setProperty(QTextCharFormat::TextUnderlineStyle, QTextCharFormat::NoUnderline);
    format.setProperty(QTextCharFormat::FontUnderline, true);
    QCOMPARE(format.fontUnderline(), false);
    QCOMPARE(format.font().underline(), false);

    // do it again, but reverse the ordering (we use a QVector internally, so test a LOT ;)
    // create conflict. Should use the new property
    format.setProperty(QTextCharFormat::FontUnderline, false);
    format.setProperty(QTextCharFormat::TextUnderlineStyle, QTextCharFormat::SingleUnderline);
    QCOMPARE(format.fontUnderline(), true);
    QCOMPARE(format.font().underline(), true);

    format = QTextCharFormat();
    // create conflict. Should use the new property
    format.setProperty(QTextCharFormat::FontUnderline, true);
    format.setProperty(QTextCharFormat::TextUnderlineStyle, QTextCharFormat::NoUnderline);
    QCOMPARE(format.fontUnderline(), false);
    QCOMPARE(format.font().underline(), false);
}

void tst_QTextFormat::toFormat()
{
    {
        QTextFormat fmt = QTextFrameFormat();
        QCOMPARE(fmt.toFrameFormat().type(), int(QTextFormat::FrameFormat));
    }

    {
        QTextFormat fmt = QTextTableFormat();
        QCOMPARE(fmt.toTableFormat().type(), int(QTextFormat::FrameFormat));
        QCOMPARE(fmt.toTableFormat().objectType(), int(QTextFormat::TableObject));
    }

    {
        QTextFormat fmt = QTextBlockFormat();
        QCOMPARE(fmt.toBlockFormat().type(), int(QTextFormat::BlockFormat));
    }

    {
        QTextFormat fmt = QTextCharFormat();
        QCOMPARE(fmt.toCharFormat().type(), int(QTextFormat::CharFormat));
    }

    {
        QTextFormat fmt = QTextListFormat();
        QCOMPARE(fmt.toListFormat().type(), int(QTextFormat::ListFormat));
    }
}

void tst_QTextFormat::resolveFont()
{
    QTextDocument doc;

    QTextCharFormat fmt;
    fmt.setProperty(QTextFormat::ForegroundBrush, Qt::blue);
    fmt.setProperty(QTextFormat::FontItalic, true);
    QTextCursor(&doc).insertText("Test", fmt);

    QVector<QTextFormat> formats = doc.allFormats();
    QCOMPARE(formats.count(), 3);

    QVERIFY(formats.at(2).type() == QTextFormat::CharFormat);
    fmt = formats.at(2).toCharFormat();

    QVERIFY(!fmt.font().underline());
    QVERIFY(fmt.hasProperty(QTextFormat::ForegroundBrush));

    QFont f;
    f.setUnderline(true);
    doc.setDefaultFont(f);
    formats = doc.allFormats();
    fmt = formats.at(2).toCharFormat();

    QVERIFY(fmt.font().underline());
    QVERIFY(!fmt.hasProperty(QTextFormat::FontUnderline));

    // verify that deleting a non-existent property does not affect the font resolving

    QVERIFY(!fmt.hasProperty(QTextFormat::BackgroundBrush));
    fmt.clearProperty(QTextFormat::BackgroundBrush);
    QVERIFY(!fmt.hasProperty(QTextFormat::BackgroundBrush));

    QVERIFY(!fmt.hasProperty(QTextFormat::FontUnderline));
    QVERIFY(fmt.font().underline());

    // verify that deleting an existent but font _unrelated_ property does not affect the font resolving

    QVERIFY(fmt.hasProperty(QTextFormat::ForegroundBrush));
    fmt.clearProperty(QTextFormat::ForegroundBrush);
    QVERIFY(!fmt.hasProperty(QTextFormat::ForegroundBrush));

    QVERIFY(!fmt.hasProperty(QTextFormat::FontUnderline));
    QVERIFY(fmt.font().underline());

    // verify that removing a font property _does_ clear the resolving

    QVERIFY(fmt.hasProperty(QTextFormat::FontItalic));
    fmt.clearProperty(QTextFormat::FontItalic);
    QVERIFY(!fmt.hasProperty(QTextFormat::FontItalic));

    QVERIFY(!fmt.hasProperty(QTextFormat::FontUnderline));
    QVERIFY(!fmt.font().underline());
    QVERIFY(!fmt.font().italic());

    // reset
    fmt = formats.at(2).toCharFormat();

    QVERIFY(fmt.font().underline());
    QVERIFY(!fmt.hasProperty(QTextFormat::FontUnderline));

    // verify that _setting_ an unrelated property does _not_ affect the resolving

    QVERIFY(!fmt.hasProperty(QTextFormat::IsAnchor));
    fmt.setProperty(QTextFormat::IsAnchor, true);
    QVERIFY(fmt.hasProperty(QTextFormat::IsAnchor));

    QVERIFY(fmt.font().underline());
    QVERIFY(!fmt.hasProperty(QTextFormat::FontUnderline));

    // verify that setting a _related_ font property does affect the resolving
    //
    QVERIFY(!fmt.hasProperty(QTextFormat::FontStrikeOut));
    fmt.setProperty(QTextFormat::FontStrikeOut, true);
    QVERIFY(fmt.hasProperty(QTextFormat::FontStrikeOut));

    QVERIFY(!fmt.font().underline());
    QVERIFY(!fmt.hasProperty(QTextFormat::FontUnderline));
    QVERIFY(fmt.font().strikeOut());
}

void tst_QTextFormat::testFontStyleSetters()
{
    QTextCharFormat format;

    // test the setters
    QCOMPARE(format.font().hintingPreference(), QFont::PreferDefaultHinting);
    format.setFontHintingPreference(QFont::PreferFullHinting);
    QCOMPARE(format.font().hintingPreference(), QFont::PreferFullHinting);

    // test setting properties through setFont()
    QFont font;
    font.setHintingPreference(QFont::PreferVerticalHinting);
    format.setFont(font);
    QCOMPARE(format.font().hintingPreference(), QFont::PreferVerticalHinting);

    // test kerning
    format.setFontKerning(false);
    QCOMPARE(format.font().kerning(), false);
    format.setFontKerning(true);
    QCOMPARE(format.font().kerning(), true);
    font.setKerning(false);
    format.setFont(font);
    QCOMPARE(format.font().kerning(), false);
}

QTEST_MAIN(tst_QTextFormat)

#include "moc_tst_qtextformat.cpp"
