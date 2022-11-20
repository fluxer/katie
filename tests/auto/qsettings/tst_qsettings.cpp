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

Q_DECLARE_METATYPE(QSettings::Format)

class tst_QSettings : public QObject
{
    Q_OBJECT

public:
    QString m_nativename;
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
    m_nativename = filename + QLatin1String(".json");
    m_ininame = filename + QLatin1String(".ini");
}

void tst_QSettings::cleanupTestCase()
{
    QFile::remove(m_nativename);
    QFile::remove(m_ininame);
}

void tst_QSettings::value_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QSettings::Format>("format");

    QTest::newRow("native") << m_nativename << QSettings::NativeFormat;
    QTest::newRow("ini") << m_ininame << QSettings::IniFormat;
}

void tst_QSettings::value()
{
    QFETCH(QString, filename);
    QFETCH(QSettings::Format, format);

    QSettings settings(filename, format);

    settings.setValue("a", "1");
    QVERIFY(settings.contains("a"));
    QCOMPARE(settings.value("a"), QVariant("1"));

    settings.setValue("a/with/separator", "2");
    QCOMPARE(settings.value("a/with/separator"), QVariant("2"));

    settings.remove("a");
    QVERIFY(!settings.contains("a"));
    QVERIFY(settings.value("a").isNull());

    const QVariant fallback(true);
    QCOMPARE(settings.value("a", fallback), fallback);
}

void tst_QSettings::variant_data()
{
    tst_QSettings::value_data();
}

#define QVARIANT_TEST(V) \
    settings.setValue("variant_" #V, V); \
    QCOMPARE(settings.value("variant_" #V), QVariant(V));

void tst_QSettings::variant()
{
    QFETCH(QString, filename);
    QFETCH(QSettings::Format, format);

    QSettings settings(filename, format);

    const qlonglong qll = 123;
    const qreal qrl = 123.456;
    const QByteArray qbytearray("abc");
    const QByteArray qstring("måndag");
    const QStringList qstringlist = QStringList() << "a" << "b" << "c";

    QVARIANT_TEST(qll);
    QVARIANT_TEST(qrl);
    QVARIANT_TEST(qbytearray);
    QVARIANT_TEST(qstring);
    QVARIANT_TEST(qstringlist);
}
#undef QVARIANT_TEST

void tst_QSettings::group_data()
{
    tst_QSettings::value_data();
}

void tst_QSettings::group()
{
    QFETCH(QString, filename);
    QFETCH(QSettings::Format, format);

    QSettings settings(filename, format);
    settings.setValue("a/a", 1);
    settings.setValue("a/b", 2);
    settings.setValue("a/c", 3);
    settings.setValue("a/c/d", 4);

    QCOMPARE(settings.groupKeys(), QStringList());

    settings.beginGroup("a");
    QCOMPARE(settings.groupKeys(), QStringList() << "a" << "b" << "c");
    QVERIFY(settings.contains("b"));
    settings.remove("c");
    QVERIFY(!settings.contains("c"));
    QCOMPARE(settings.value("c"), QVariant());
    QCOMPARE(settings.value("c/d"), QVariant());
    settings.endGroup();
}

QTEST_MAIN(tst_QSettings)

#include "moc_tst_qsettings.cpp"
