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
    void init();
    void cleanup();

private slots:
    void value_data();
    void value();
    void sync_data();
    void sync();
    void variant_data();
    void variant();
    void group_data();
    void group();
    void custom_data();
    void custom();
};

void tst_QSettings::init()
{
    const QString filename = QDir::tempPath() + QLatin1String("/tst_qsettings_")
        + QString::number(qrand());
    m_nativename = filename + QLatin1String(".json");
    m_ininame = filename + QLatin1String(".ini");
}

void tst_QSettings::cleanup()
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

void tst_QSettings::sync_data()
{
    tst_QSettings::value_data();
}

void tst_QSettings::sync()
{
    QFETCH(QString, filename);
    QFETCH(QSettings::Format, format);

    QSettings settings(filename, format);
    QSettings settings2(filename, format);

    settings.setValue("a", "1");
    QCOMPARE(settings.value("a"), QVariant("1"));
    QCOMPARE(settings2.value("a"), QVariant("1"));

    settings2.setValue("b", "2");
    QCOMPARE(settings.value("b"), QVariant("2"));
    QCOMPARE(settings2.value("b"), QVariant("2"));
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

    const QPoint qpoint(1, 2);
    const QRect qrect(1, 2, 3, 4);
    const QSize qsize(1, 2);
    const QByteArray qbytearray("abc");
    const QByteArray qstring("måndag");
    const QStringList qstringlist = QStringList() << "a" << "b" << "c";
    const QDate qdate = QDate::currentDate();
    const QColor qcolor(1, 2, 3);
    const QFont qfont = QApplication::font();

    QVARIANT_TEST(qpoint);
    QVARIANT_TEST(qrect);
    QVARIANT_TEST(qsize);
    QVARIANT_TEST(qbytearray);
    QVARIANT_TEST(qstring);
    QVARIANT_TEST(qstringlist);
    QVARIANT_TEST(qdate);
    QVARIANT_TEST(qcolor);
    QVARIANT_TEST(qfont);
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

struct CustomType {
    int a;
    QByteArray b;
    QSize c;
};
Q_DECLARE_METATYPE(CustomType);

QDataStream &operator<<(QDataStream &stream, const CustomType &custom)
{
    stream << custom.a;
    stream << custom.b;
    stream << custom.c;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, CustomType &custom)
{
    stream >> custom.a;
    stream >> custom.b;
    stream >> custom.c;
    return stream;
}

void tst_QSettings::custom_data()
{
    tst_QSettings::value_data();
}

void tst_QSettings::custom()
{
    QFETCH(QString, filename);
    QFETCH(QSettings::Format, format);

    qRegisterMetaType<CustomType>();
    qRegisterMetaTypeStreamOperators<CustomType>();

    CustomType test;
    test.a = 10;
    test.b = QByteArray("test");
    test.c = QSize(10, 10);

    QSettings settings(filename, format);
    settings.setValue("a", QVariant::fromValue(test));

    CustomType result = qvariant_cast<CustomType>(settings.value("a"));
    QCOMPARE(result.a, 10);
    QCOMPARE(result.b, QByteArray("test"));
    QCOMPARE(result.c, QSize(10, 10));
}

QTEST_MAIN(tst_QSettings)

#include "moc_tst_qsettings.cpp"
