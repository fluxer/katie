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
#include <qkeysequence.h>
#include <qapplication_p.h>
#include <qkeysequence_p.h>
#include <qdebug.h>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QKeySequence : public QObject
{
    Q_OBJECT

public:
    tst_QKeySequence();
    virtual ~tst_QKeySequence();

private slots:
    void swap();
    void operatorQString_data();
    void operatorQString();
    void compareConstructors_data();
    void compareConstructors();
    void symetricConstructors_data();
    void symetricConstructors();
    void checkMultipleNames();
    void checkMultipleCodes();
    void mnemonic_data();
    void mnemonic();
    void toString_data();
    void toString();
    void streamOperators_data();
    void streamOperators();
    void fromString_data();
    void fromString();
    void standardKeys_data();
    void standardKeys();
    void keyBindings();
    void i18nKeys_data();
    void i18nKeys();


    void initTestCase();
};

tst_QKeySequence::tst_QKeySequence()
{
}

tst_QKeySequence::~tst_QKeySequence()
{

}

void tst_QKeySequence::initTestCase()
{
}

void tst_QKeySequence::swap()
{
    QKeySequence ks1(Qt::CTRL+Qt::Key_O);
    QKeySequence ks2(Qt::CTRL+Qt::Key_L);
    qSwap(ks1, ks2);
    QCOMPARE(ks1[0], int(Qt::CTRL+Qt::Key_L));
    QCOMPARE(ks2[0], int(Qt::CTRL+Qt::Key_O));
}

void tst_QKeySequence::operatorQString_data()
{
    QTest::addColumn<int>("modifiers");
    QTest::addColumn<int>("keycode");
    QTest::addColumn<QString>("keystring");

    QTest::newRow( "No modifier" ) << 0 << int(Qt::Key_Aring) << QString( "Aring" );

    QTest::newRow( "Ctrl+Left" ) << int(Qt::CTRL) << int(Qt::Key_Left) << QString( "Ctrl+Left" );
    QTest::newRow( "Ctrl+," ) << int(Qt::CTRL) << int(Qt::Key_Comma) << QString( "Ctrl+," );
    QTest::newRow( "Alt+Left" ) << int(Qt::ALT) << int(Qt::Key_Left) << QString( "Alt+Left" );
    QTest::newRow( "Alt+Shift+Left" ) << int(Qt::ALT | Qt::SHIFT) << int(Qt::Key_Left) << QString( "Alt+Shift+Left" );
    QTest::newRow( "Ctrl" ) << int(Qt::CTRL) << int(Qt::Key_Aring) << QString( "Ctrl+Aring" );
    QTest::newRow( "Alt" ) << int(Qt::ALT) << int(Qt::Key_Aring) << QString( "Alt+Aring" );
    QTest::newRow( "Shift" ) << int(Qt::SHIFT) << int(Qt::Key_Aring) << QString( "Shift+Aring" );
    QTest::newRow( "Meta" ) << int(Qt::META) << int(Qt::Key_Aring) << QString( "Meta+Aring" );
}

void tst_QKeySequence::symetricConstructors_data()
{
    QTest::addColumn<int>("modifiers");
    QTest::addColumn<int>("keycode");

    QTest::newRow( "No modifier" ) << 0 << int(Qt::Key_Aring);
    QTest::newRow( "Ctrl" ) << int(Qt::CTRL) << int(Qt::Key_Aring);
    QTest::newRow( "Alt" ) << int(Qt::ALT) << int(Qt::Key_Aring);
    QTest::newRow( "Shift" ) << int(Qt::SHIFT) << int(Qt::Key_Aring);
    QTest::newRow( "Meta" ) << int(Qt::META) << int(Qt::Key_Aring);
}

void tst_QKeySequence::compareConstructors_data()
{
    operatorQString_data();
}

// operator QString()
void tst_QKeySequence::operatorQString()
{
    QKeySequence seq;
    QFETCH( int, modifiers );
    QFETCH( int, keycode );
    QFETCH( QString, keystring );

    seq = QKeySequence( modifiers | keycode );

    QCOMPARE( seq.toString(), keystring );
}

// this verifies that the constructors can handle the same strings in and out
void tst_QKeySequence::symetricConstructors()
{
    QFETCH( int, modifiers );
    QFETCH( int, keycode );

    QKeySequence seq1( modifiers | keycode );
    QKeySequence seq2( seq1.toString() );

    QVERIFY( seq1 == seq2 );
}

/* Compares QKeySequence constructurs with int or QString arguments
   We don't do this for 3.0 since it doesn't support unicode accelerators */
void tst_QKeySequence::compareConstructors()
{
    QFETCH( int, modifiers );
    QFETCH( int, keycode );
    QFETCH( QString, keystring );

    QKeySequence qstringSeq( keystring );
    QKeySequence intSeq( modifiers | keycode );

    QVERIFY( qstringSeq == intSeq );
}

void tst_QKeySequence::checkMultipleNames()
{
    QKeySequence oldK( "Ctrl+Page Up" );
    QKeySequence newK( "Ctrl+PgUp" );
    QVERIFY( oldK == newK );
}

//TODO: could test third constructor, or test fromString on all constructor-data
void tst_QKeySequence::checkMultipleCodes()
{
    QKeySequence seq1("Alt+d, l");
    QKeySequence seq2 = QKeySequence::fromString("Alt+d, l");
    QVERIFY( seq1 == seq2 );

    QKeySequence seq3("Alt+d,l");
    QKeySequence seq4 = QKeySequence::fromString("Alt+d,l");
    QVERIFY( seq3 == seq4 );
}

void tst_QKeySequence::standardKeys_data()
{
    QTest::addColumn<int>("standardKey");
    QTest::addColumn<QString>("expected");
    QTest::newRow("unknownkey") << (int)QKeySequence::UnknownKey<< QString("");
    QTest::newRow("copy") << (int)QKeySequence::Copy << QString("CTRL+C");
    QTest::newRow("cut") << (int)QKeySequence::Cut << QString("CTRL+X");
    QTest::newRow("paste") << (int)QKeySequence::Paste << QString("CTRL+V");
    QTest::newRow("delete") << (int)QKeySequence::Delete<< QString("DEL");
    QTest::newRow("open") << (int)QKeySequence::Open << QString("CTRL+O");
    QTest::newRow("find") << (int)QKeySequence::Find<< QString("CTRL+F");
    QTest::newRow("bold") << (int)QKeySequence::Bold << QString("CTRL+B");
    QTest::newRow("italic") << (int)QKeySequence::Italic << QString("CTRL+I");
    QTest::newRow("underline") << (int)QKeySequence::Underline << QString("CTRL+U");
    QTest::newRow("selectall") << (int)QKeySequence::SelectAll << QString("CTRL+A");
    QTest::newRow("print") << (int)QKeySequence::Print << QString("CTRL+P");
    QTest::newRow("movenextchar") << (int)QKeySequence::MoveToNextChar<< QString("RIGHT");
    QTest::newRow("zoomIn") << (int)QKeySequence::ZoomIn<< QString("CTRL++");
    QTest::newRow("zoomOut") << (int)QKeySequence::ZoomOut<< QString("CTRL+-");
    QTest::newRow("whatsthis") << (int)QKeySequence::WhatsThis<< QString("SHIFT+F1");

    QTest::newRow("help") << (int)QKeySequence::HelpContents<< QString("F1");
    QTest::newRow("nextChild") << (int)QKeySequence::NextChild<< QString("CTRL+Tab");
    QTest::newRow("previousChild") << (int)QKeySequence::PreviousChild<< QString("CTRL+SHIFT+BACKTAB");
    QTest::newRow("forward") << (int)QKeySequence::Forward << QString("ALT+RIGHT");
    QTest::newRow("backward") << (int)QKeySequence::Back << QString("ALT+LEFT");
    QTest::newRow("MoveToEndOfBlock") << (int)QKeySequence::MoveToEndOfBlock<< QString(""); //mac only
    QTest::newRow("SelectEndOfDocument") << (int)QKeySequence::SelectEndOfDocument<< QString("CTRL+SHIFT+END"); //mac only
}

void tst_QKeySequence::standardKeys()
{
    QFETCH(int, standardKey);
    QFETCH(QString, expected);
    QKeySequence ks((QKeySequence::StandardKey)standardKey);
    QKeySequence ks2(expected);
    QVERIFY(ks == ks2);
}

void tst_QKeySequence::keyBindings()
{
    QList<QKeySequence> bindings = QKeySequence::keyBindings(QKeySequence::Copy);
    QList<QKeySequence> expected;
#if defined Q_WS_X11
    expected  << QKeySequence("CTRL+C") << QKeySequence("F16") << QKeySequence("CTRL+INSERT");
#else
    expected  << QKeySequence("CTRL+C") << QKeySequence("CTRL+INSERT");
#endif
	QVERIFY(bindings == expected);
}



void tst_QKeySequence::mnemonic_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("key");
    QTest::addColumn<bool>("warning");

    QTest::newRow("1") << QString::fromLatin1("&bonjour") << QString::fromLatin1("ALT+B") << false;
    QTest::newRow("2") << QString::fromLatin1("&&bonjour") << QString() << false;
    QTest::newRow("3") << QString::fromLatin1("&&bon&jour") << QString::fromLatin1("ALT+J") << false;
    QTest::newRow("4") << QString::fromLatin1("&&bon&jo&ur") << QString::fromLatin1("ALT+J") << true;
    QTest::newRow("5") << QString::fromLatin1("b&on&&jour") << QString::fromLatin1("ALT+O") << false;
    QTest::newRow("6") << QString::fromLatin1("bonjour") << QString() << false;
    QTest::newRow("7") << QString::fromLatin1("&&&bonjour") << QString::fromLatin1("ALT+B") << false;
    QTest::newRow("8") << QString::fromLatin1("bonjour&&&") << QString() << false;
    QTest::newRow("9") << QString::fromLatin1("bo&&nj&o&&u&r") << QString::fromLatin1("ALT+O") << true;
    QTest::newRow("10") << QString::fromLatin1("BON&JOUR") << QString::fromLatin1("ALT+J") << false;
    QTest::newRow("11") << QString::fromUtf8("bonjour") << QString() << false;
}

void tst_QKeySequence::mnemonic()
{
    QFETCH(QString, string);
    QFETCH(QString, key);
    QFETCH(bool, warning);

#ifndef QT_NO_DEBUG
    if (warning) {
        QString str = QString::fromLatin1("QKeySequence::mnemonic: \"%1\" contains multiple occurrences of '&'").arg(string);
        QTest::ignoreMessage(QtWarningMsg, qPrintable(str));
    //    qWarning(qPrintable(str));
    }
#endif
    QKeySequence seq = QKeySequence::mnemonic(string);
    QKeySequence res = QKeySequence(key);

    QCOMPARE(seq, res);
}



void tst_QKeySequence::toString_data()
{
    QTest::addColumn<QString>("strSequence");
    QTest::addColumn<QString>("neutralString");
    QTest::addColumn<QString>("platformString");


    QTest::newRow("Ctrl+Left") << QString("Ctrl+Left") << QString("Ctrl+Left") << QString("Ctrl+Left");
    QTest::newRow("Alt+Left") << QString("Alt+Left") << QString("Alt+Left") << QString("Alt+Left");
    QTest::newRow("Alt+Shift+Left") << QString("Alt+Shift+Left") << QString("Alt+Shift+Left") << QString("Alt+Shift+Left");
    QTest::newRow("Ctrl") << QString("Ctrl+Aring") << QString("Ctrl+Aring") << QString("Ctrl+Aring");
    QTest::newRow("Alt") << QString("Alt+Aring") << QString("Alt+Aring") << QString("Alt+Aring");
    QTest::newRow("Shift") << QString("Shift+Aring") << QString("Shift+Aring") << QString("Shift+Aring");
    QTest::newRow("Meta") << QString("Meta+Aring") << QString("Meta+Aring") << QString("Meta+Aring");
    QTest::newRow("Ctrl+Plus") << QString("Ctrl++") << QString("Ctrl++") << QString("Ctrl++");
    QTest::newRow("Ctrl+,") << QString("Ctrl+,") << QString("Ctrl+,") << QString("Ctrl+,");
    QTest::newRow("Ctrl+,,Ctrl+,") << QString("Ctrl+,,Ctrl+,") << QString("Ctrl+,, Ctrl+,") << QString("Ctrl+,, Ctrl+,");
    QTest::newRow("MultiKey") << QString("Alt+X, Ctrl+Y") << QString("Alt+X, Ctrl+Y")
                           << QString("Alt+X, Ctrl+Y");

    QTest::newRow("Invalid") << QString("Ctrly") << QString("") << QString("");
}

void tst_QKeySequence::toString()
{
    QFETCH(QString, strSequence);
    QFETCH(QString, neutralString);
    QFETCH(QString, platformString);

    QKeySequence ks1(strSequence);

    QCOMPARE(ks1.toString(QKeySequence::NativeText), platformString);
    QCOMPARE(ks1.toString(QKeySequence::PortableText), neutralString);

}

void tst_QKeySequence::streamOperators_data()
{
    operatorQString_data();
}

void tst_QKeySequence::streamOperators()
{
    QFETCH( int, modifiers );
    QFETCH( int, keycode );

    QByteArray data;
    QKeySequence refK( modifiers | keycode );
    QKeySequence orgK( "Ctrl+A" );
    QKeySequence copyOrgK = orgK;
    QVERIFY( copyOrgK == orgK );

    QDataStream in(&data, QIODevice::WriteOnly);
    in << refK;
    QDataStream out(&data, QIODevice::ReadOnly);
    out >> orgK;

    QVERIFY( orgK == refK );

    // check if detached
    QVERIFY( orgK != copyOrgK );
}

void tst_QKeySequence::fromString_data()
{
    toString_data();
}

void tst_QKeySequence::fromString()
{
    QFETCH(QString, strSequence);
    QFETCH(QString, neutralString);
    QFETCH(QString, platformString);

    QKeySequence ks1(strSequence);
    QKeySequence ks2 = QKeySequence::fromString(ks1.toString());
    QKeySequence ks3 = QKeySequence::fromString(neutralString, QKeySequence::PortableText);
    QKeySequence ks4 = QKeySequence::fromString(platformString, QKeySequence::NativeText);


    // assume the transitive property exists here.
    QCOMPARE(ks2, ks1);
    QCOMPARE(ks3, ks1);
    QCOMPARE(ks4, ks1);
}

void tst_QKeySequence::i18nKeys_data()
{
    QTest::addColumn<int>("keycode");
    QTest::addColumn<QString>("keystring");

    // Japanese keyboard support
    QTest::newRow("Kanji") << (int)Qt::Key_Kanji << QString("Kanji");
    QTest::newRow("Muhenkan") << (int)Qt::Key_Muhenkan << QString("Muhenkan");
    QTest::newRow("Henkan") << (int)Qt::Key_Henkan << QString("Henkan");
    QTest::newRow("Romaji") << (int)Qt::Key_Romaji << QString("Romaji");
    QTest::newRow("Hiragana") << (int)Qt::Key_Hiragana << QString("Hiragana");
    QTest::newRow("Katakana") << (int)Qt::Key_Katakana << QString("Katakana");
    QTest::newRow("Hiragana Katakana") << (int)Qt::Key_Hiragana_Katakana << QString("Hiragana Katakana");
    QTest::newRow("Zenkaku") << (int)Qt::Key_Zenkaku << QString("Zenkaku");
    QTest::newRow("Hankaku") << (int)Qt::Key_Hankaku << QString("Hankaku");
    QTest::newRow("Zenkaku Hankaku") << (int)Qt::Key_Zenkaku_Hankaku << QString("Zenkaku Hankaku");
    QTest::newRow("Touroku") << (int)Qt::Key_Touroku << QString("Touroku");
    QTest::newRow("Massyo") << (int)Qt::Key_Massyo << QString("Massyo");
    QTest::newRow("Kana Lock") << (int)Qt::Key_Kana_Lock << QString("Kana Lock");
    QTest::newRow("Kana Shift") << (int)Qt::Key_Kana_Shift << QString("Kana Shift");
    QTest::newRow("Eisu Shift") << (int)Qt::Key_Eisu_Shift << QString("Eisu Shift");
    QTest::newRow("Eisu_toggle") << (int)Qt::Key_Eisu_toggle << QString("Eisu toggle");
    QTest::newRow("Code input") << (int)Qt::Key_Codeinput << QString("Code input");
    QTest::newRow("Multiple Candidate") << (int)Qt::Key_MultipleCandidate << QString("Multiple Candidate");
    QTest::newRow("Previous Candidate") << (int)Qt::Key_PreviousCandidate << QString("Previous Candidate");
}

void tst_QKeySequence::i18nKeys()
{
    QFETCH(int, keycode);
    QFETCH(QString, keystring);
    QKeySequence seq(keycode);

    QCOMPARE(seq, QKeySequence(keystring));
    QCOMPARE(seq.toString(), keystring);
}

QTEST_MAIN(tst_QKeySequence)

#include "moc_tst_qkeysequence.cpp"
