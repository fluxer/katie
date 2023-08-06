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
#include <QtCore/QtCore>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QSettings : public QObject
{
    Q_OBJECT

public:
    QString m_ininame;

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void value_data();
    void value();
    void variant_data();
    void variant();
    void group_data();
    void group();
};

void tst_QSettings::initTestCase()
{
    const QString filename = QDir::tempPath() + QLatin1String("/tst_qsettings_")
        + QString::number(qrand());
    m_ininame = filename + QLatin1String(".ini");
}

void tst_QSettings::cleanupTestCase()
{
    QFile::remove(m_ininame);
}

void tst_QSettings::value_data()
{
    QTest::addColumn<QString>("filename");

    QTest::newRow("ini") << m_ininame;
}

void tst_QSettings::value()
{
    QFETCH(QString, filename);

    QSettings settings(filename);

    settings.setString("a", "1");
    QVERIFY(settings.contains("a"));
    QCOMPARE(settings.string("a"), QString("1"));

    settings.setString("a/with/separator", "2");
    QCOMPARE(settings.string("a/with/separator"), QString("2"));

    settings.remove("a");
    QVERIFY(!settings.contains("a"));
    QVERIFY(settings.string("a").isEmpty());

    const QString fallback("true");
    QCOMPARE(settings.string("a", fallback), fallback);
}

void tst_QSettings::variant_data()
{
    tst_QSettings::value_data();
}

void tst_QSettings::variant()
{
    QFETCH(QString, filename);

    QSettings settings(filename);

    const qlonglong qll = 123;
    const qreal qrl = 123.456;
    const QByteArray qbytearray("abc");
    const QString qstring = QString::fromUtf8("måndag");
    const QStringList qstringlist = QStringList() << "a" << "b" << "c";

    settings.setInteger("variant_qll", qll);
    QCOMPARE(settings.integer("variant_qll"), qll);
    settings.setString("variant_qrl", QString::number(qrl));
    QCOMPARE(settings.string("variant_qrl"), QString::number(qrl));
    settings.setString("variant_qbytearray", QString(qbytearray));
    QCOMPARE(settings.string("variant_qbytearray"), QString(qbytearray));
    settings.setString("variant_qstring", qstring);
    QCOMPARE(settings.string("variant_qstring"), qstring);
    settings.setStringList("variant_qstringlist", qstringlist);
    QCOMPARE(settings.stringList("variant_qstringlist"), qstringlist);
}

void tst_QSettings::group_data()
{
    tst_QSettings::value_data();
}

void tst_QSettings::group()
{
    QFETCH(QString, filename);

    QSettings settings(filename);
    settings.setInteger("a/a", 1);
    settings.setInteger("a/b", 2);
    settings.setInteger("a/c", 3);
    settings.setInteger("a/c/d", 4);

    QCOMPARE(settings.groupKeys(), QStringList());

    settings.beginGroup("a");
    QCOMPARE(settings.groupKeys(), QStringList() << "a" << "b" << "c");
    QVERIFY(settings.contains("b"));
    settings.remove("c");
    QVERIFY(!settings.contains("c"));
    QCOMPARE(settings.string("c"), QString());
    QCOMPARE(settings.string("c/d"), QString());
    settings.endGroup();
}

QTEST_MAIN(tst_QSettings)

#include "moc_tst_qsettings.cpp"
