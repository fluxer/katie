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
#include <qthread.h>
#include <qdebug.h>

#include <unistd.h>

//TESTED_CLASS=
//TESTED_FILES=

// this default is somewhat safe
static int s_threadcount = 100;

class QFontDatabaseThread : public QThread
{
public:
    QFontDatabaseThread(QObject *parent, const int threadnum);

protected:
    void run() final;

private:
    const int m_threadnum;
};

QFontDatabaseThread::QFontDatabaseThread(QObject *parent, const int threadnum)
    : QThread(parent),
    m_threadnum(threadnum)
{
}

void QFontDatabaseThread::run()
{
    // qDebug("Thread running: %d", m_threadnum);

    const QString fontfamily("FreeSans");
    const QString fontstyle("Bold");

    (void)QFontDatabase().families();
    (void)QFontDatabase().styles(fontfamily);
    (void)QFontDatabase().isScalable(fontfamily, fontstyle);
    (void)QFontDatabase().isFixedPitch(fontfamily, fontstyle);
    (void)QFontDatabase().pointSizes(fontfamily, fontstyle);
}

class tst_QFontDatabase : public QObject
{
    Q_OBJECT
private slots:
    void styles_data();
    void styles();

    void fixedPitch_data();
    void fixedPitch();

    void widthTwoTimes_data();
    void widthTwoTimes();

    void resolveFamily_data();
    void resolveFamily();

    void styleString();

    void threadSafety();

    void caseInsensitive_data();
    void caseInsensitive();

    void bold_data();
    void bold();
};

void tst_QFontDatabase::styles_data()
{
    QTest::addColumn<QString>("font");

    QTest::newRow("FreeSerif [GNU]") << QString("FreeSerif [GNU]");
}

void tst_QFontDatabase::styles()
{
    QFETCH(QString, font);

    QFontDatabase fdb;
    QStringList styles = fdb.styles(font);
    QStringList::Iterator it = styles.begin();
    while (it != styles.end()) {
        QString style = *it;
        QString trimmed = style.trimmed();
        ++it;

        QCOMPARE(style, trimmed);
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

    QFont f(font);
    QFont fdbfont = fdb.font(f.family(), f.styleName(), f.pointSize());
    QCOMPARE(fdbfont.fixedPitch(), fixedPitch);
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
    QTest::addColumn<QString>("font");

    QTest::newRow("Sans Serif") << QString("Sans Serif") << QString("FreeSans");
    QTest::newRow("Monospace") << QString("Monospace") << QString("FreeMono");
    QTest::newRow("FreeSans [GNU ]") << QString("FreeSans [GNU ]") << QString("FreeSans");
}

void tst_QFontDatabase::resolveFamily()
{
    QFETCH(QString, alias);
    QFETCH(QString, font);

    if (!QFontDatabase::supportsThreadedFontRendering()) {
        QSKIP("Font resolution is not supported", SkipSingle);
    }

    QFontDatabase fdb;
    // qDebug() << fdb.families();
    if (!fdb.hasFamily(font)) {
        QSKIP("Font not installed", SkipSingle);
    }

    QVERIFY(fdb.hasFamily(alias));
}

void tst_QFontDatabase::styleString()
{
    const QString family("DejaVu Sans Mono");

    QFontDatabase fdb;
    // qDebug() << fdb.families();
    if (!fdb.hasFamily(family)) {
        QSKIP("Font not installed", SkipSingle);
    }

    QVERIFY(fdb.styleString(family) != QLatin1String("Regular"));
}

void tst_QFontDatabase::threadSafety()
{
#ifdef _SC_THREAD_THREADS_MAX
    const long threadmax = ::sysconf(_SC_THREAD_THREADS_MAX);
    // should be bellow the limit
    if (threadmax > 10) {
        s_threadcount = (threadmax - 10);
    }
    qDebug("Starting %d threads (max %ld)", s_threadcount, threadmax);
#else
    qDebug("Starting %d threads", s_threadcount);
#endif

    QList<QFontDatabaseThread*> fdbthreads;
    for (int i = 0; i < s_threadcount; i++) {
        QFontDatabaseThread* fdbthread = new QFontDatabaseThread(this, i);
        fdbthreads.append(fdbthread);
        fdbthread->start();
    }

    qDebug("Waiting for threads");
    for (int i = 0; i < s_threadcount; i++) {
        QFontDatabaseThread* fdbthread = fdbthreads.at(i);
        while (!fdbthread->isFinished()) {
            QApplication::processEvents();
        }
    }
    qDebug("Done waiting");
}

void tst_QFontDatabase::caseInsensitive_data()
{
    QTest::addColumn<QString>("font");
    QTest::addColumn<QString>("style");

    // actual font families
    QTest::newRow("FreeSans") << QString("FreeSans") << QString("rEGular");
    QTest::newRow("freeSans [gNu ]") << QString("freeSans [gNu ]") << QString("obLique");
    QTest::newRow("freeMono") << QString("freeMono") << QString("BOLD");
    // aliases
    QTest::newRow("sans serif") << QString("sans serif") << QString("bold");
    QTest::newRow("MoNoSpAcE") << QString("MoNoSpAcE") << QString("BoLd");
}

void tst_QFontDatabase::caseInsensitive()
{
    QFETCH(QString, font);
    QFETCH(QString, style);

    QFontDatabase fdb;
    // qDebug() << fdb.families();
    if (!fdb.hasFamily(font)) {
        QSKIP("Font not installed", SkipSingle);
    }

    QVERIFY(!fdb.pointSizes(font, style).isEmpty());
    QVERIFY(!fdb.styles(font).isEmpty());
}

void tst_QFontDatabase::bold_data()
{
    QTest::addColumn<QString>("font");
    QTest::addColumn<QString>("style");
    QTest::addColumn<bool>("bold");

    // actual font families
    QTest::newRow("FreeSans") << QString("FreeSans") << QString("Regular") << false;
    QTest::newRow("FreeSans [GNU ]") << QString("FreeSans [GNU ]") << QString("Oblique") << false;
    QTest::newRow("FreeMono") << QString("FreeMono") << QString("Bold") << true;
    // aliases
    QTest::newRow("Sans Serif") << QString("Sans Serif") << QString("Bold") << true;
    QTest::newRow("Monospace") << QString("Monospace") << QString("Bold") << true;
}

void tst_QFontDatabase::bold()
{
    QFETCH(QString, font);
    QFETCH(QString, style);
    QFETCH(bool, bold);

    QFontDatabase fdb;
    // qDebug() << fdb.families();
    if (!fdb.hasFamily(font)) {
        QSKIP("Font not installed", SkipSingle);
    }

    QVERIFY(!fdb.pointSizes(font, style).isEmpty());
    QCOMPARE(fdb.bold(font, style), bold);
}

QTEST_MAIN(tst_QFontDatabase)

#include "moc_tst_qfontdatabase.cpp"
