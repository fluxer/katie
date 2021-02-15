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
#include "bytearrayclass.h"

static QString readFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
        return QString();
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    return stream.readAll();
}

class tst_QScriptClass_ByteArray : public QObject
{
    Q_OBJECT

public:
    tst_QScriptClass_ByteArray();

private slots:
    void benchmark_data();
    void benchmark();

private:
    QDir testsDir;
};

tst_QScriptClass_ByteArray::tst_QScriptClass_ByteArray()
{
    testsDir = QDir(QLatin1String(":/tests"));
    if (!testsDir.exists())
        qWarning("*** no tests/ dir!");
}

void tst_QScriptClass_ByteArray::benchmark_data()
{
    QTest::addColumn<QString>("testName");
    QFileInfoList testFileInfos = testsDir.entryInfoList(QStringList() << QLatin1String("*.js"), QDir::Files);
    foreach (QFileInfo tfi, testFileInfos) {
        QString name = tfi.baseName();
        QTest::newRow(name.toLatin1().constData()) << name;
    }
}

void tst_QScriptClass_ByteArray::benchmark()
{
    QFETCH(QString, testName);
    QString testContents = readFile(testsDir.absoluteFilePath(testName + QLatin1String(".js")));
    QVERIFY(!testContents.isEmpty());

    QScriptEngine eng;
    ByteArrayClass *baClass = new ByteArrayClass(&eng);
    eng.globalObject().setProperty(QLatin1String("ByteArray"), baClass->constructor());

    QBENCHMARK {
        eng.evaluate(testContents);
    }
    QVERIFY(!eng.hasUncaughtException());
}

QTEST_MAIN(tst_QScriptClass_ByteArray)

#include "moc_tst_qscriptclass_bytearray.cpp"
#include "qrc_qscriptclass_bytearray.cpp"
