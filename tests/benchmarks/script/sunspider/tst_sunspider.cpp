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

#include <qtest.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include <QtScript/qscriptengine.h>
#include <QtScript/qscriptvalue.h>

static QString readFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
        return QString();
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    return stream.readAll();
}

class tst_SunSpider : public QObject
{
    Q_OBJECT

public:
    tst_SunSpider();
    virtual ~tst_SunSpider();

public slots:
    void init();
    void cleanup();

private slots:
    void benchmark_data();
    void benchmark();

private:
    QDir testsDir;
};

tst_SunSpider::tst_SunSpider()
{
    testsDir = QDir(QLatin1String(":/tests"));
    if (!testsDir.exists())
        qWarning("*** no tests/ dir!");
}

tst_SunSpider::~tst_SunSpider()
{
}

void tst_SunSpider::init()
{
}

void tst_SunSpider::cleanup()
{
}

void tst_SunSpider::benchmark_data()
{
    QTest::addColumn<QString>("testName");
    QFileInfoList testFileInfos = testsDir.entryInfoList(QStringList() << QLatin1String("*.js"), QDir::Files);
    foreach (QFileInfo tfi, testFileInfos) {
        QString name = tfi.baseName();
        QTest::newRow(name.toLatin1().constData()) << name;
    }
}

void tst_SunSpider::benchmark()
{
    QFETCH(QString, testName);
    QString testContents = readFile(testsDir.filePath(testName + QLatin1String(".js")));
    QVERIFY(!testContents.isEmpty());

    QScriptEngine engine;
    QBENCHMARK {
        engine.evaluate(testContents);
    }
    QVERIFY(!engine.hasUncaughtException());
}

QTEST_MAIN(tst_SunSpider)

#include "moc_tst_sunspider.cpp"
#include "qrc_sunspider.cpp"
