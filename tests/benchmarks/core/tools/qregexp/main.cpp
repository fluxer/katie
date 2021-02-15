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

#include <QDebug>
#include <QRegExp>
#include <QString>
#include <QFile>
#include <qtest.h>
#include <QtScript>

#define ZLIB_VERSION "1.2.3.4"

QT_USE_NAMESPACE

class tst_qregexp : public QObject
{
    Q_OBJECT
public:
    tst_qregexp();
private slots:
    void escape_old();
    void escape_old_data() { escape_data(); }
    void escape_new1();
    void escape_new1_data() { escape_data(); }
    void escape_new2();
    void escape_new2_data() { escape_data(); }
    void escape_new3();
    void escape_new3_data() { escape_data(); }
    void escape_new4();
    void escape_new4_data() { escape_data(); }
/*
   JSC outperforms everything.
   Boost is less impressive then expected.
 */
    void simpleFind1();
    void rangeReplace1();
    void matchReplace1();

    void simpleFind2();
    void rangeReplace2();
    void matchReplace2();

    void simpleFindJSC();
    void rangeReplaceJSC();
    void matchReplaceJSC();

/* those apply an (incorrect) regexp on entire source
   (this main.cpp). JSC appears to handle this
   (ab)use case best. QRegExp performs extremly bad.
 */
    void horribleWrongReplace1();
    void horribleReplace1();
    void horribleReplace2();
    void horribleWrongReplace2();
    void horribleWrongReplaceJSC();
    void horribleReplaceJSC();
private:
    QString str1;
    QString str2;
    void escape_data();
};

tst_qregexp::tst_qregexp()
    :QObject()
    ,str1("We are all happy monkeys")
{
        QFile f(":/main.cpp");
        f.open(QFile::ReadOnly);
        str2=f.readAll();
}

static void verify(const QString &quoted, const QString &expected)
{
    if (quoted != expected)
        qDebug() << "ERROR:" << quoted << expected;
}

void tst_qregexp::escape_data()
{
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QString>("expected");

    QTest::newRow("escape 0") << "Hello world" << "Hello world";
    QTest::newRow("escape 1") << "(Hello world)" << "\\(Hello world\\)";
    { 
        QString s;
        for (int i = 0; i < 10; ++i)
            s += "(escape)";
        QTest::newRow("escape 10") << s << QRegExp::escape(s);
    }
    { 
        QString s;
        for (int i = 0; i < 100; ++i)
            s += "(escape)";
        QTest::newRow("escape 100") << s << QRegExp::escape(s);
    }
}

void tst_qregexp::escape_old()
{
    QFETCH(QString, pattern);
    QFETCH(QString, expected);

    QBENCHMARK {
        static const char meta[] = "$()*+.?[\\]^{|}";
        QString quoted = pattern;
        int i = 0;

        while (i < quoted.length()) {
            if (strchr(meta, quoted.at(i).toLatin1()) != 0)
                quoted.insert(i++, QLatin1Char('\\'));
            ++i;
        }

        verify(quoted, expected);
    }
}

void tst_qregexp::escape_new1()
{
    QFETCH(QString, pattern);
    QFETCH(QString, expected);

    QBENCHMARK {
        QString quoted;
        const int count = pattern.count();
        quoted.reserve(count * 2);
        const QLatin1Char backslash('\\');
        for (int i = 0; i < count; i++) {
            switch (pattern.at(i).toLatin1()) {
            case '$':
            case '(':
            case ')':
            case '*':
            case '+':
            case '.':
            case '?':
            case '[':
            case '\\':
            case ']':
            case '^':
            case '{':
            case '|':
            case '}':
                quoted.append(backslash);
            }
            quoted.append(pattern.at(i));
        }
        verify(quoted, expected);
    }
}

void tst_qregexp::escape_new2()
{
    QFETCH(QString, pattern);
    QFETCH(QString, expected);

    QBENCHMARK {
        int count = pattern.count();
        const QLatin1Char backslash('\\');
        QString quoted(count * 2, backslash);
        const QChar *patternData = pattern.data();
        QChar *quotedData = quoted.data();
        int escaped = 0;
        for ( ; --count >= 0; ++patternData) {
            const QChar c = *patternData;
            switch (c.unicode()) {
            case '$':
            case '(':
            case ')':
            case '*':
            case '+':
            case '.':
            case '?':
            case '[':
            case '\\':
            case ']':
            case '^':
            case '{':
            case '|':
            case '}':
                ++escaped;
                ++quotedData;
            }
            *quotedData = c;
            ++quotedData;
        }
        quoted.resize(pattern.size() + escaped); 

        verify(quoted, expected);
    }
}

void tst_qregexp::escape_new3()
{
    QFETCH(QString, pattern);
    QFETCH(QString, expected);

    QBENCHMARK {
        QString quoted;
        const int count = pattern.count();
        quoted.reserve(count * 2);
        const QLatin1Char backslash('\\');
        for (int i = 0; i < count; i++) {
            switch (pattern.at(i).toLatin1()) {
            case '$':
            case '(':
            case ')':
            case '*':
            case '+':
            case '.':
            case '?':
            case '[':
            case '\\':
            case ']':
            case '^':
            case '{':
            case '|':
            case '}':
                quoted += backslash;
            }
            quoted += pattern.at(i);
        }

        verify(quoted, expected);
    }
}


static inline bool needsEscaping(int c)
{
    switch (c) {
    case '$':
    case '(':
    case ')':
    case '*':
    case '+':
    case '.':
    case '?':
    case '[':
    case '\\':
    case ']':
    case '^':
    case '{':
    case '|':
    case '}':
        return true;
    }
    return false;
}

void tst_qregexp::escape_new4()
{
    QFETCH(QString, pattern);
    QFETCH(QString, expected);

    QBENCHMARK {
        const int n = pattern.size();
        const QChar *patternData = pattern.data();
        // try to prevent copy if no escape is needed
        int i = 0;
        for (int i = 0; i != n; ++i) {
            const QChar c = patternData[i];
            if (needsEscaping(c.unicode()))
                break;
        }
        if (i == n) {
            verify(pattern, expected);
            // no escaping needed, "return pattern" should be done here.
            return;
        }
        const QLatin1Char backslash('\\');
        QString quoted(n * 2, backslash);
        QChar *quotedData = quoted.data();
        for (int j = 0; j != i; ++j) 
            *quotedData++ = *patternData++;
        int escaped = 0;
        for (; i != n; ++i) {
            const QChar c = *patternData;
            if (needsEscaping(c.unicode())) {
                ++escaped;
                ++quotedData;
            }
            *quotedData = c;
            ++quotedData;
            ++patternData;
        }
        quoted.resize(n + escaped); 
        verify(quoted, expected);
        // "return quoted"
    }
}


void tst_qregexp::simpleFind1()
{
    int roff;
    QRegExp rx("happy");
    rx.setPatternSyntax(QRegExp::RegExp);
    QBENCHMARK{
        roff = rx.indexIn(str1);
    }
    QCOMPARE(roff, 11);
}

void tst_qregexp::rangeReplace1()
{
    QString r;
    QRegExp rx("[a-f]");
    rx.setPatternSyntax(QRegExp::RegExp);
    QBENCHMARK{
        r = QString(str1).replace(rx, "-");
    }
    QCOMPARE(r, QString("W- -r- -ll h-ppy monk-ys"));
}

void tst_qregexp::matchReplace1()
{
    QString r;
    QRegExp rx("[^a-f]*([a-f]+)[^a-f]*");
    rx.setPatternSyntax(QRegExp::RegExp);
    QBENCHMARK{
        r = QString(str1).replace(rx, "\\1");
    }
    QCOMPARE(r, QString("eaeaae"));
}

void tst_qregexp::horribleWrongReplace1()
{
    QString r;
    QRegExp rx(".*#""define ZLIB_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*");
    rx.setPatternSyntax(QRegExp::RegExp);
    QBENCHMARK{
        r = QString(str2).replace(rx, "\\1.\\2.\\3");
    }
    QCOMPARE(r, str2);
}

void tst_qregexp::horribleReplace1()
{
    QString r;
    QRegExp rx(".*#""define ZLIB_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+).*");
    rx.setPatternSyntax(QRegExp::RegExp);
    QBENCHMARK{
        r = QString(str2).replace(rx, "\\1.\\2.\\3");
    }
    QCOMPARE(r, QString("1.2.3"));
}


void tst_qregexp::simpleFind2()
{
    int roff;
    QRegExp rx("happy");
    rx.setPatternSyntax(QRegExp::RegExp2);
    QBENCHMARK{
        roff = rx.indexIn(str1);
    }
    QCOMPARE(roff, 11);
}

void tst_qregexp::rangeReplace2()
{
    QString r;
    QRegExp rx("[a-f]");
    rx.setPatternSyntax(QRegExp::RegExp2);
    QBENCHMARK{
        r = QString(str1).replace(rx, "-");
    }
    QCOMPARE(r, QString("W- -r- -ll h-ppy monk-ys"));
}

void tst_qregexp::matchReplace2()
{
    QString r;
    QRegExp rx("[^a-f]*([a-f]+)[^a-f]*");
    rx.setPatternSyntax(QRegExp::RegExp2);
    QBENCHMARK{
        r = QString(str1).replace(rx, "\\1");
    }
    QCOMPARE(r, QString("eaeaae"));
}

void tst_qregexp::horribleWrongReplace2()
{
    QString r;
    QRegExp rx(".*#""define ZLIB_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*");
    rx.setPatternSyntax(QRegExp::RegExp2);
    QBENCHMARK{
        r = QString(str2).replace(rx, "\\1.\\2.\\3");
    }
    QCOMPARE(r, str2);
}

void tst_qregexp::horribleReplace2()
{
    QString r;
    QRegExp rx(".*#""define ZLIB_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+).*");
    rx.setPatternSyntax(QRegExp::RegExp2);
    QBENCHMARK{
        r = QString(str2).replace(rx, "\\1.\\2.\\3");
    }
    QCOMPARE(r, QString("1.2.3"));
}


void tst_qregexp::simpleFindJSC()
{
    QScriptValue r;
    QScriptEngine engine;
    engine.globalObject().setProperty("s", "happy");
    QScriptValue findFunc = engine.evaluate("(function() { return s.search('pp')  } )");
    QVERIFY(findFunc.isFunction());
    QBENCHMARK{
        r = findFunc.call(QScriptValue());
    }
    QCOMPARE(r.toInt32(), 2);
}

void tst_qregexp::rangeReplaceJSC()
{
    QScriptValue r;
    QScriptEngine engine;
    engine.globalObject().setProperty("s", str1);
    QScriptValue replaceFunc = engine.evaluate("(function() { return s.replace(/[a-f]/g, '-')  } )");
    QVERIFY(replaceFunc.isFunction());
    QBENCHMARK{
        r = replaceFunc.call(QScriptValue());
    }
    QCOMPARE(r.toString(), QString("W- -r- -ll h-ppy monk-ys"));
}

void tst_qregexp::matchReplaceJSC()
{
    QScriptValue r;
    QScriptEngine engine;
    engine.globalObject().setProperty("s", str1);
    QScriptValue replaceFunc = engine.evaluate("(function() { return s.replace(/[^a-f]*([a-f]+)[^a-f]*/g, '$1')  } )");
    QVERIFY(replaceFunc.isFunction());
    QBENCHMARK{
        r = replaceFunc.call(QScriptValue());
    }
    QCOMPARE(r.toString(), QString("eaeaae"));
}

void tst_qregexp::horribleWrongReplaceJSC()
{
    QScriptValue r;
    QScriptEngine engine;
    engine.globalObject().setProperty("s", str2);
    QScriptValue replaceFunc = engine.evaluate("(function() { return s.replace(/.*#""define ZLIB_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*/gm, '$1.$2.$3')  } )");
    QVERIFY(replaceFunc.isFunction());
    QBENCHMARK{
        r = replaceFunc.call(QScriptValue());
    }
    QCOMPARE(r.toString(), str2);
}

void tst_qregexp::horribleReplaceJSC()
{
    QScriptValue r;
    QScriptEngine engine;
    // the m flag doesnt actually work here; dunno
    engine.globalObject().setProperty("s", str2.replace('\n', ' '));
    QScriptValue replaceFunc = engine.evaluate("(function() { return s.replace(/.*#""define ZLIB_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+).*/gm, '$1.$2.$3')  } )");
    QVERIFY(replaceFunc.isFunction());
    QBENCHMARK{
        r = replaceFunc.call(QScriptValue());
    }
    QCOMPARE(r.toString(), QString("1.2.3"));
}

QTEST_MAIN(tst_qregexp)

#include "moc_main.cpp"
#include "qrc_qregexp.cpp"
