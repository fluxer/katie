/****************************************************************************
**
** Copyright (C) 2021 Ivailo Monev
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
#include <QtCore/QtCore>

//TESTED_CLASS=QJsonDocument
//TESTED_FILES=qjsondocument.cpp,qjsondocument.h

static QString testinvalidfile = QString::fromLatin1(SRCDIR "/invalidjson.json");
static QString testjsonfile = QString::fromLatin1(SRCDIR "/qjsondocument.json");
static QVariantMap testjsondata;

class tst_QJsonDocument : public QObject
{
    Q_OBJECT

public slots:
    void init();
    void cleanup();

private slots:
    void read();
    void write();
    void error();
    void eq_not_eq();
};

void tst_QJsonDocument::init()
{
    testjsondata.insert("Qt/GUIEffects", QStringList() << "none");
    testjsondata.insert("Qt/doubleClickInterval", 400);
    testjsondata.insert("Qt/embedFonts", true);
    testjsondata.insert("Qt/style", "Cleanlooks");

    testjsondata.insert("MixedArray", QVariantList() << 1 << "a");
    QVariantMap NestedObject;
    NestedObject.insert("MixedArray", QVariantList() << 2 << "b");
    testjsondata.insert("NestedObject", NestedObject);

    testjsondata.insert("UTF8", QString("УТФ"));
}

void tst_QJsonDocument::cleanup()
{
    testjsondata.clear();
}

void tst_QJsonDocument::read()
{
    QJsonDocument jsondoc = QJsonDocument::fromVariant(testjsondata);
    QVERIFY(jsondoc.errorString().isEmpty());
    QVariantMap jsonmap = jsondoc.toVariant().toMap();

    QStringList GUIEffects = jsonmap.value("Qt/GUIEffects").toStringList();
    QCOMPARE(GUIEffects, QStringList() << "none");

    int doubleClickInterval = jsonmap.value("Qt/doubleClickInterval").toInt();
    QCOMPARE(doubleClickInterval, 400);

    bool embedFonts = jsonmap.value("Qt/embedFonts").toBool();
    QCOMPARE(embedFonts, true);

    QString style = jsonmap.value("Qt/style").toString();
    QCOMPARE(style, QLatin1String("Cleanlooks"));

    QVariantList MixedArray = jsonmap.value("MixedArray").toList();
    QCOMPARE(MixedArray, QVariantList() << 1 << "a");

    QVariantList NestedObject = jsonmap.value("NestedObject").toMap().value("MixedArray").toList();
    QCOMPARE(NestedObject, QVariantList() << 2 << "b");

    QString UTF8 = jsonmap.value("UTF8").toString();
    QCOMPARE(UTF8, QString("УТФ"));

    QFile jsonfile(testjsonfile);
    QVERIFY(jsonfile.open(QIODevice::ReadOnly));

    jsondoc = QJsonDocument::fromJson(jsonfile.readAll());
    QVERIFY(jsondoc.errorString().isEmpty());
    jsonmap = jsondoc.toVariant().toMap();

    GUIEffects = jsonmap.value("Qt/GUIEffects").toStringList();
    QCOMPARE(GUIEffects, QStringList() << "none");

    doubleClickInterval = jsonmap.value("Qt/doubleClickInterval").toInt();
    QCOMPARE(doubleClickInterval, 400);

    embedFonts = jsonmap.value("Qt/embedFonts").toBool();
    QCOMPARE(embedFonts, true);

    style = jsonmap.value("Qt/style").toString();
    QCOMPARE(style, QLatin1String("Cleanlooks"));

    MixedArray = jsonmap.value("MixedArray").toList();
    QCOMPARE(MixedArray, QVariantList() << 1 << "a");

    NestedObject = jsonmap.value("NestedObject").toMap().value("MixedArray").toList();
    QCOMPARE(NestedObject, QVariantList() << 2 << "b");

    UTF8 = jsonmap.value("UTF8").toString();
    QCOMPARE(UTF8, QString("УТФ"));

    // TODO: test other types too
}

void tst_QJsonDocument::write()
{
    QJsonDocument jsondoc = QJsonDocument::fromVariant(testjsondata);
    // TODO: compare to desired output
    QVERIFY(!jsondoc.toJson().isEmpty());

    QFile jsonfile(testjsonfile);
    QVERIFY(jsonfile.open(QIODevice::ReadOnly));
    QByteArray jsoncontent = jsonfile.readAll();
    jsondoc = QJsonDocument::fromJson(jsoncontent);
    QCOMPARE(jsoncontent, jsondoc.toJson());
}

void tst_QJsonDocument::error()
{
    QFile jsonfile(testinvalidfile);
    QVERIFY(jsonfile.open(QIODevice::ReadOnly));
    QJsonDocument jsondoc = QJsonDocument::fromJson(jsonfile.readAll());
    // not testing for specific string on purpose
    QVERIFY(!jsondoc.errorString().isEmpty());
    QVERIFY(jsondoc.isNull());

    jsondoc = QJsonDocument::fromVariant(QVariantMap());
    QCOMPARE(jsondoc.errorString(), QLatin1String("Data map is empty"));
    QVERIFY(jsondoc.isNull());

    // TODO: indermediate error test, e.g. maximum depth reached
}

void tst_QJsonDocument::eq_not_eq()
{
    QJsonDocument nulljsondoc;
    QJsonDocument nulljsondoc2;
    QCOMPARE(nulljsondoc, nulljsondoc2);

    QJsonDocument jsondoc = QJsonDocument::fromVariant(testjsondata);
    QVERIFY(nulljsondoc != jsondoc);

    // assignment crash test
    QJsonDocument jsondoc2 = QJsonDocument::fromVariant(testjsondata);
    nulljsondoc = jsondoc;
    jsondoc2 = nulljsondoc2;

    // constructor crash test
    QJsonDocument nulljsondoc3(nulljsondoc);
    QCOMPARE(nulljsondoc3, nulljsondoc);
}

QTEST_MAIN(tst_QJsonDocument)

#include "moc_tst_qjsondocument.cpp"
