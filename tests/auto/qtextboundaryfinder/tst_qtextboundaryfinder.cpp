/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtTest/QtTest>

#include <qtextboundaryfinder.h>
#include <qfile.h>
#include <qdir.h>
#include <qdebug.h>

//TESTED_CLASS=
//TESTED_FILES=gui/text/qtextlayout.h corelib/tools/qtextboundaryfinder.cpp

class tst_QTextBoundaryFinder : public QObject
{
    Q_OBJECT

public:
    tst_QTextBoundaryFinder();
    virtual ~tst_QTextBoundaryFinder();


public slots:
    void init();
    void cleanup();
private slots:
    void graphemeBoundaries();
    void wordBoundaries();
    void sentenceBoundaries();
    void lineBoundaries();
    void isAtWordStart();
    void fastConstructor();
    void toNextBoundary_data();
    void toNextBoundary();
    void toPreviousBoundary_data();
    void toPreviousBoundary();
};

tst_QTextBoundaryFinder::tst_QTextBoundaryFinder()
{
}

tst_QTextBoundaryFinder::~tst_QTextBoundaryFinder()
{
}

void tst_QTextBoundaryFinder::init()
{
    QDir::setCurrent(SRCDIR);
}

void tst_QTextBoundaryFinder::cleanup()
{
}

void tst_QTextBoundaryFinder::graphemeBoundaries()
{
    QFile file(SRCDIR "/data/GraphemeBreakTest.txt");
    file.open(QFile::ReadOnly);

    int lines = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (line.startsWith('#'))
            continue;

        lines++;
        QString test = QString::fromUtf8(line);
        int hash = test.indexOf('#');
        if (hash > 0)
            test = test.left(hash);

        QList<int> breakPositions;
        QString testString;
        foreach (const QString &part, test.simplified().split(QLatin1Char(' '), QString::SkipEmptyParts)) {
            if (part.size() == 1) {
                if (part.at(0).unicode() == 0xf7)
                    breakPositions.append(testString.size());
                else
                    QVERIFY(part.at(0).unicode() == 0xd7);
                continue;
            }
            bool ok = true;
            uint ucs4 = part.toInt(&ok, 16);
            QVERIFY(ok && ucs4 > 0);
            if (QChar::requiresSurrogates(ucs4)) {
                testString.append(QChar::highSurrogate(ucs4));
                testString.append(QChar::lowSurrogate(ucs4));
            } else {
                testString.append(QChar(ucs4));
            }
        }

        QTextBoundaryFinder finder(QTextBoundaryFinder::Grapheme, testString);
        for (int i = 0; i < breakPositions.size(); ++i) {
            QCOMPARE(finder.position(), breakPositions.at(i));
            finder.toNextBoundary();
        }
        QCOMPARE(finder.toNextBoundary(), -1);

        for (int i = 0; i < testString.length(); ++i) {
            finder.setPosition(i);
            QCOMPARE(finder.isAtBoundary(), breakPositions.contains(i) == true);
        }
    }
    QCOMPARE(lines, 100); // to see it actually found the file and all.
}

void tst_QTextBoundaryFinder::wordBoundaries()
{
    QFile file(SRCDIR "/data/WordBreakTest.txt");
    file.open(QFile::ReadOnly);

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (line.startsWith('#'))
            continue;

        QString test = QString::fromUtf8(line);
        int hash = test.indexOf('#');
        if (hash > 0)
            test = test.left(hash);

        QList<int> breakPositions;
        QString testString;
        foreach (const QString &part, test.simplified().split(QLatin1Char(' '), QString::SkipEmptyParts)) {
            if (part.size() == 1) {
                if (part.at(0).unicode() == 0xf7)
                    breakPositions.append(testString.size());
                else
                    QVERIFY(part.at(0).unicode() == 0xd7);
                continue;
            }
            bool ok = true;
            uint ucs4 = part.toInt(&ok, 16);
            QVERIFY(ok && ucs4 > 0);
            if (QChar::requiresSurrogates(ucs4)) {
                testString.append(QChar::highSurrogate(ucs4));
                testString.append(QChar::lowSurrogate(ucs4));
            } else {
                testString.append(QChar(ucs4));
            }
        }

        QTextBoundaryFinder finder(QTextBoundaryFinder::Word, testString);
        for (int i = 0; i < breakPositions.size(); ++i) {
            QCOMPARE(finder.position(), breakPositions.at(i));
            finder.toNextBoundary();
        }
        QCOMPARE(finder.toNextBoundary(), -1);

        for (int i = 0; i < testString.length(); ++i) {
            finder.setPosition(i);
            QCOMPARE(finder.isAtBoundary(), breakPositions.contains(i) == true);
        }
    }
}

void tst_QTextBoundaryFinder::sentenceBoundaries()
{
    QFile file(SRCDIR "/data/SentenceBreakTest.txt");
    file.open(QFile::ReadOnly);

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (line.startsWith('#'))
            continue;

        QString test = QString::fromUtf8(line);
        int hash = test.indexOf('#');
        if (hash > 0)
            test = test.left(hash);

        QList<int> breakPositions;
        QString testString;
        foreach (const QString &part, test.simplified().split(QLatin1Char(' '), QString::SkipEmptyParts)) {
            if (part.size() == 1) {
                if (part.at(0).unicode() == 0xf7)
                    breakPositions.append(testString.size());
                else
                    QVERIFY(part.at(0).unicode() == 0xd7);
                continue;
            }
            bool ok = true;
            uint ucs4 = part.toInt(&ok, 16);
            QVERIFY(ok && ucs4 > 0);
            if (QChar::requiresSurrogates(ucs4)) {
                testString.append(QChar::highSurrogate(ucs4));
                testString.append(QChar::lowSurrogate(ucs4));
            } else {
                testString.append(QChar(ucs4));
            }
        }

        QTextBoundaryFinder finder(QTextBoundaryFinder::Sentence, testString);
        for (int i = 0; i < breakPositions.size(); ++i) {
            QCOMPARE(finder.position(), breakPositions.at(i));
            finder.toNextBoundary();
        }
        QCOMPARE(finder.toNextBoundary(), -1);

        for (int i = 0; i < testString.length(); ++i) {
            finder.setPosition(i);
            QCOMPARE(finder.isAtBoundary(), breakPositions.contains(i) == true);
        }
    }
}

void tst_QTextBoundaryFinder::lineBoundaries()
{
    QFile file(SRCDIR "/data/LineBreakTest.txt");
    file.open(QFile::ReadOnly);

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (line.startsWith('#'))
            continue;

        QString test = QString::fromUtf8(line);
        int hash = test.indexOf('#');
        if (hash > 0)
            test = test.left(hash);
        test = test.simplified();
        test = test.replace(QLatin1String(" "), QString());

        QList<int> breakPositions;
        QString testString;
        int pos = 0;
        int strPos = 0;
        while (pos < test.length()) {
            if (test.at(pos).unicode() == 0xf7)
                breakPositions.append(strPos);
            else
                QVERIFY(test.at(pos).unicode() == 0xd7);
            ++pos;
            if (pos < test.length()) {
                QVERIFY(pos < test.length() - 4);
                QString hex = test.mid(pos, 4);
                bool ok = true;
                testString.append(QChar(hex.toInt(&ok, 16)));
                QVERIFY(ok);
                pos += 4;
            }
            ++strPos;
        }

        QTextBoundaryFinder finder(QTextBoundaryFinder::Line, testString);
        for (int i = 0; i < breakPositions.size(); ++i) {
            QCOMPARE(finder.position(), breakPositions.at(i));
            finder.toNextBoundary();
        }
        QCOMPARE(finder.toNextBoundary(), -1);

        for (int i = 0; i < testString.length(); ++i) {
            finder.setPosition(i);
            QCOMPARE(finder.isAtBoundary(), breakPositions.contains(i) == true);
        }
    }
}

void tst_QTextBoundaryFinder::isAtWordStart()
{
    QString txt("The quick brown fox jumped over $the lazy. dog  I win!");
    QList<int> start, end;
    start << 0 << 4 << 10 << 16 << 20 << 27 << 32 << 33 << 37 << 41 << 43 << 48 << 50 << 53;
    end << 3 << 9 << 15 << 19 << 26 << 31 << 33 << 36 << 41 << 42 << 46 << 49 << 53 << 54;
    QTextBoundaryFinder finder(QTextBoundaryFinder::Word, txt);
    for(int i=0; i < txt.length(); ++i) {
        finder.setPosition(i);
        QTextBoundaryFinder::BoundaryReasons r = finder.boundaryReasons();
        // qDebug() << i << r;
        QCOMPARE((r & QTextBoundaryFinder::StartWord) != 0, start.contains(i) == true);
        QCOMPARE((r & QTextBoundaryFinder::EndWord) != 0, end.contains(i) == true);
    }
}

void tst_QTextBoundaryFinder::fastConstructor()
{
    QString text("Hello World");
    QTextBoundaryFinder finder(QTextBoundaryFinder::Word, text.constData(), text.length());
    QVERIFY(finder.boundaryReasons() == QTextBoundaryFinder::StartWord);

    finder.toNextBoundary();
    QCOMPARE(finder.position(), 5);
    QVERIFY(finder.boundaryReasons() == QTextBoundaryFinder::EndWord);

    finder.toNextBoundary();
    QCOMPARE(finder.position(), 6);
    QVERIFY(finder.boundaryReasons() == QTextBoundaryFinder::StartWord);

    finder.toNextBoundary();
    QCOMPARE(finder.position(), text.length());
    QVERIFY(finder.boundaryReasons() == QTextBoundaryFinder::EndWord);

    finder.toNextBoundary();
    QVERIFY(finder.boundaryReasons() == QTextBoundaryFinder::NotAtBoundary);
    QCOMPARE(finder.position(), -1);
}

Q_DECLARE_METATYPE(QList<int>)

void tst_QTextBoundaryFinder::toNextBoundary_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("type");
    QTest::addColumn< QList<int> >("boundaries");

    QList<int> boundaries;
    boundaries << 0 << 3 << 4 << 7 << 8 << 11 << 12 << 13 << 16 << 17 << 20 << 21 << 24 << 25;
    QTest::newRow("Line") << QString("Aaa bbb ccc. Ddd eee fff.") << int(QTextBoundaryFinder::Word) \
            << boundaries;

    boundaries.clear();
    boundaries << 0 << 13 << 25;
    QTest::newRow("Line") << QString("Aaa bbb ccc. Ddd eee fff.") << int(QTextBoundaryFinder::Sentence) \
            << boundaries;

    boundaries.clear();
    boundaries << 0 << 4 << 8 << 13 << 17 << 21 << 25;
    QTest::newRow("Line") << QString("Aaa bbb ccc. Ddd eee fff.") << int(QTextBoundaryFinder::Line) \
            << boundaries;

    boundaries.clear();
    boundaries << 0 << 5 << 9 << 15 << 17 << 21 << 28;
    QTest::newRow("Line") << QString::fromUtf8("Diga-nos qualé a sua opinião") << int(QTextBoundaryFinder::Line)
            << boundaries;

}

void tst_QTextBoundaryFinder::toNextBoundary()
{
    QFETCH(QString, text);
    QFETCH(int, type);
    QFETCH(QList<int>, boundaries);

    QList<int> foundBoundaries;
    QTextBoundaryFinder boundaryFinder(QTextBoundaryFinder::BoundaryType(type), text);
    boundaryFinder.toStart();
    for(int next = 0; next != -1; next = boundaryFinder.toNextBoundary())
        foundBoundaries << next;
    QCOMPARE(boundaries, foundBoundaries);
}

void tst_QTextBoundaryFinder::toPreviousBoundary_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("type");
    QTest::addColumn< QList<int> >("boundaries");

    QList<int> boundaries;
    boundaries << 25 << 24 << 21 << 20 << 17 << 16 << 13 << 12 << 11 << 8 << 7 << 4 << 3 << 0;
    QTest::newRow("Line") << QString("Aaa bbb ccc. Ddd eee fff.") << int(QTextBoundaryFinder::Word)
            << boundaries;

    boundaries.clear();
    boundaries << 25 << 13 << 0;
    QTest::newRow("Line") << QString("Aaa bbb ccc. Ddd eee fff.") << int(QTextBoundaryFinder::Sentence)
            << boundaries;

    boundaries.clear();
    boundaries << 25 << 21 << 17 << 13 << 8 << 4 << 0;
    QTest::newRow("Line") << QString("Aaa bbb ccc. Ddd eee fff.") << int(QTextBoundaryFinder::Line)
            << boundaries;

    boundaries.clear();
    boundaries << 28 << 21 << 17 << 15 << 9 << 5 << 0;
    QTest::newRow("Line") << QString::fromUtf8("Diga-nos qualé a sua opinião") << int(QTextBoundaryFinder::Line)
            << boundaries;

}

void tst_QTextBoundaryFinder::toPreviousBoundary()
{
    QFETCH(QString, text);
    QFETCH(int, type);
    QFETCH(QList<int>, boundaries);

    QList<int> foundBoundaries;
    QTextBoundaryFinder boundaryFinder(QTextBoundaryFinder::BoundaryType(type), text);
    boundaryFinder.toEnd();
    for (int previous = boundaryFinder.position();
         previous != -1;
         previous = boundaryFinder.toPreviousBoundary())
    {
        foundBoundaries << previous;
    }
    QCOMPARE(boundaries, foundBoundaries);
}

QTEST_MAIN(tst_QTextBoundaryFinder)

#include "moc_tst_qtextboundaryfinder.cpp"
