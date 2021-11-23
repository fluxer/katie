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
#include <qdir.h>
#include <qtextcodec.h>
#include <qdatetime.h>
#include <qlocale.h>
#include <qnumeric.h>
#include <qprocess.h>
#include <qlocale_p.h>

#include <math.h>
#include <float.h>
#include <stdlib.h>

#if defined(QT_HAVE_FEENABLEEXCEPT)
#  include <fenv.h>
#endif

QT_USE_NAMESPACE

Q_DECLARE_METATYPE(qlonglong)
Q_DECLARE_METATYPE(QDate)
Q_DECLARE_METATYPE(QLocale::FormatType)

//TESTED_CLASS=
//TESTED_FILES=

class tst_QLocale : public QObject
{
    Q_OBJECT

public:
    tst_QLocale();

private slots:
    void initTestCase();

    void ctor();
    void emptyCtor();
    void unixLocaleName();
    void matchingLocales();
    void double_conversion_data();
    void double_conversion();
    void long_long_conversion_data();
    void long_long_conversion();
    void long_long_conversion_extra();
    void testInfAndNan();
    void fpExceptions();
    void negativeZero();
    void dayOfWeek();
    void dayOfWeek_data();
    void formatDate();
    void formatDate_data();
    void formatTime();
    void formatTime_data();
    void formatDateTime();
    void formatDateTime_data();
    void toDateTime_data();
    void toDateTime();
    void negativeNumbers();
    void numberOptions();
    void testNames_data();
    void testNames();
    void dayName_data();
    void dayName();
    void standaloneDayName_data();
    void standaloneDayName();
    void underflowOverflow();
    void measurementSystems_data();
    void measurementSystems();
    void systemMeasurementSystems_data();
    void systemMeasurementSystems();

    void dateFormat();
    void timeFormat();
    void dateTimeFormat();
    void monthName();
    void standaloneMonthName();

    // defaultNumberingSystem test
    void defaultNumeringSystem();

    void ampm();
    void currency();
    void quoteString();
    void uiLanguages();
    void weekendDays();
    void listPatterns();

private:
    QString m_decimal, m_thousand, m_sdate, m_ldate, m_time;
    QString m_sysLocaleApp;
};

tst_QLocale::tst_QLocale()
{
    qRegisterMetaType<QLocale::FormatType>("QLocale::FormatType");
}

void tst_QLocale::initTestCase()
{
    QDir workingDirectory = QCoreApplication::applicationDirPath();
    m_sysLocaleApp = workingDirectory.absoluteFilePath(QLatin1String("qlocale_syslocaleapp"));
    QVERIFY2(QFileInfo(m_sysLocaleApp).exists(),
             qPrintable(QString::fromLatin1("SysLocalApp executable '%1' does not exist!")
                        .arg(QDir::toNativeSeparators(m_sysLocaleApp))));
}

void tst_QLocale::ctor()
{
    QLocale default_locale = QLocale::system();
    QLocale::Language default_lang = default_locale.language();
    QLocale::Country default_country = default_locale.country();

    qDebug("Default: %s/%s", QLocale::languageToString(default_lang).toLatin1().constData(),
            QLocale::countryToString(default_country).toLatin1().constData());

    {
        QLocale l;
        QVERIFY(l.language() == default_lang);
        QVERIFY(l.country() == default_country);
    }

#define TEST_CTOR(req_lang, req_country, exp_lang, exp_country) \
    { \
        QLocale l(QLocale::req_lang, QLocale::req_country); \
        QVERIFY2(l.language() == exp_lang && l.country() == exp_country, \
        QString("requested: \"" \
        + QLocale::languageToString(exp_lang) \
        + "/" + QLocale::countryToString(exp_country) \
        + "\", got: " \
        + QLocale::languageToString(l.language()) \
        + "/" + QLocale::countryToString(l.country())).toLatin1().constData()); \
    }
    {
        QLocale l(QLocale::C, QLocale::AnyCountry);
        QCOMPARE(l.language(), QLocale::C);
        QCOMPARE(l.country(), QLocale::AnyCountry);
    }
    TEST_CTOR(AnyLanguage, AnyCountry, default_lang, default_country)
    TEST_CTOR(C, AnyCountry, QLocale::C, QLocale::AnyCountry)
    TEST_CTOR(Aymara, AnyCountry, default_lang, default_country)
    TEST_CTOR(Aymara, France, default_lang, default_country)

    TEST_CTOR(English, AnyCountry, QLocale::English, QLocale::UnitedStates)
    TEST_CTOR(English, UnitedStates, QLocale::English, QLocale::UnitedStates)
    TEST_CTOR(English, world, QLocale::English, QLocale::world)
    TEST_CTOR(English, France, QLocale::English, QLocale::world)
    TEST_CTOR(English, UnitedKingdom, QLocale::English, QLocale::UnitedKingdom)

    TEST_CTOR(French, France, QLocale::French, QLocale::France)
    TEST_CTOR(C, France, QLocale::C, QLocale::AnyCountry)
    TEST_CTOR(Spanish, LatinAmerica, QLocale::Spanish, QLocale::LatinAmerica)

    QLocale::setDefault(QLocale(QLocale::English, QLocale::France));

    {
        QLocale l;
        QVERIFY(l.language() == QLocale::English);
        QVERIFY(l.country() == QLocale::world);
    }

    TEST_CTOR(French, France, QLocale::French, QLocale::France)
    TEST_CTOR(English, UnitedKingdom, QLocale::English, QLocale::UnitedKingdom)

    TEST_CTOR(French, France, QLocale::French, QLocale::France)
    TEST_CTOR(C, AnyCountry, QLocale::C, QLocale::AnyCountry)
    TEST_CTOR(C, France, QLocale::C, QLocale::AnyCountry)
    TEST_CTOR(Aymara, AnyCountry, QLocale::English, QLocale::world)

    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedKingdom));

    {
        QLocale l;
        QVERIFY(l.language() == QLocale::English);
        QVERIFY(l.country() == QLocale::UnitedKingdom);
    }

    TEST_CTOR(French, France, QLocale::French, QLocale::France)
    TEST_CTOR(English, UnitedKingdom, QLocale::English, QLocale::UnitedKingdom)

    TEST_CTOR(C, AnyCountry, QLocale::C, QLocale::AnyCountry)
    TEST_CTOR(C, France, QLocale::C, QLocale::AnyCountry)

    QLocale::setDefault(QLocale(QLocale::Aymara, QLocale::France));

    {
        QLocale l;
        QVERIFY(l.language() == QLocale::English);
        QVERIFY(l.country() == QLocale::UnitedKingdom);
    }

    TEST_CTOR(Aymara, AnyCountry, QLocale::English, QLocale::UnitedKingdom)
    TEST_CTOR(Aymara, France, QLocale::English, QLocale::UnitedKingdom)

    TEST_CTOR(English, AnyCountry, QLocale::English, QLocale::UnitedStates)
    TEST_CTOR(English, UnitedStates, QLocale::English, QLocale::UnitedStates)
    TEST_CTOR(English, world, QLocale::English, QLocale::world)
    TEST_CTOR(English, France, QLocale::English, QLocale::world)
    TEST_CTOR(English, UnitedKingdom, QLocale::English, QLocale::UnitedKingdom)

    TEST_CTOR(French, France, QLocale::French, QLocale::France)
    TEST_CTOR(C, AnyCountry, QLocale::C, QLocale::AnyCountry)
    TEST_CTOR(C, France, QLocale::C, QLocale::AnyCountry)

    QLocale::setDefault(QLocale(QLocale::Aymara, QLocale::AnyCountry));

    {
        QLocale l;
        QVERIFY(l.language() == QLocale::English);
        QVERIFY(l.country() == QLocale::UnitedKingdom);
    }


    TEST_CTOR(Aymara, AnyCountry, QLocale::English, QLocale::UnitedKingdom)
    TEST_CTOR(Aymara, France, QLocale::English, QLocale::UnitedKingdom)

    TEST_CTOR(English, AnyCountry, QLocale::English, QLocale::UnitedStates)
    TEST_CTOR(English, UnitedStates, QLocale::English, QLocale::UnitedStates)
    TEST_CTOR(English, world, QLocale::English, QLocale::world)
    TEST_CTOR(English, France, QLocale::English, QLocale::world)
    TEST_CTOR(English, UnitedKingdom, QLocale::English, QLocale::UnitedKingdom)

    TEST_CTOR(French, France, QLocale::French, QLocale::France)
    TEST_CTOR(C, AnyCountry, QLocale::C, QLocale::AnyCountry)
    TEST_CTOR(C, France, QLocale::C, QLocale::AnyCountry)

    // substitutions
    TEST_CTOR(Arabic, AnyCountry, QLocale::Arabic, QLocale::Egypt)
    TEST_CTOR(Dutch, AnyCountry, QLocale::Dutch, QLocale::Netherlands)
    TEST_CTOR(German, AnyCountry, QLocale::German, QLocale::Germany)
    TEST_CTOR(Greek, AnyCountry, QLocale::Greek, QLocale::Greece)
    TEST_CTOR(Malay, AnyCountry, QLocale::Malay, QLocale::Malaysia)
    TEST_CTOR(Persian, AnyCountry, QLocale::Persian, QLocale::Iran)
    TEST_CTOR(Portuguese, AnyCountry, QLocale::Portuguese, QLocale::Brazil)
    TEST_CTOR(Serbian, AnyCountry, QLocale::Serbian, QLocale::Serbia)
    TEST_CTOR(Somali, AnyCountry, QLocale::Somali, QLocale::Somalia)
    TEST_CTOR(Spanish, AnyCountry, QLocale::Spanish, QLocale::Spain)
    TEST_CTOR(Swedish, AnyCountry, QLocale::Swedish, QLocale::Sweden)
    TEST_CTOR(Uzbek, AnyCountry, QLocale::Uzbek, QLocale::Uzbekistan)

#undef TEST_CTOR

#define TEST_CTOR(req_lc, exp_lang, exp_country) \
    { \
	QLocale l(req_lc); \
	QVERIFY2(l.language() == QLocale::exp_lang \
		&& l.country() == QLocale::exp_country, \
		QString("requested: \"" + QString(req_lc) + "\", got: " \
		+ QLocale::languageToString(l.language()) \
		+ "/" + QLocale::countryToString(l.country())).toLatin1().constData()); \
    }

    QLocale::setDefault(QLocale(QLocale::C));

    QChar c;
    TEST_CTOR("C", C, AnyCountry)
    TEST_CTOR("bla", C, AnyCountry)
    TEST_CTOR("zz", C, AnyCountry)
    TEST_CTOR("zz_zz", C, AnyCountry)
    TEST_CTOR("zz...", C, AnyCountry)
    TEST_CTOR("", C, AnyCountry)
    TEST_CTOR("en/", C, AnyCountry)
    TEST_CTOR(c, C, AnyCountry)
    TEST_CTOR("en", English, UnitedStates)
    TEST_CTOR("en", English, UnitedStates)
    TEST_CTOR("en.", English, UnitedStates)
    TEST_CTOR("en@", English, UnitedStates)
    TEST_CTOR("en.@", English, UnitedStates)
    TEST_CTOR("en_", English, UnitedStates)
    TEST_CTOR("en_U", English, UnitedStates)
    TEST_CTOR("en_.", English, UnitedStates)
    TEST_CTOR("en_.@", English, UnitedStates)
    TEST_CTOR("en.bla", English, UnitedStates)
    TEST_CTOR("en@bla", English, UnitedStates)
    TEST_CTOR("en_blaaa", English, UnitedStates)
    TEST_CTOR("en_zz", English, world)
    TEST_CTOR("en_001", English, world)
    TEST_CTOR("en_001.bla", English, world)
    TEST_CTOR("en_001@.bla", English, world)
    TEST_CTOR("en_001@bla", English, world)
    TEST_CTOR("en-001", English, world)
    TEST_CTOR("en-001@bla", English, world)
    TEST_CTOR("en_GB", English, UnitedKingdom)
    TEST_CTOR("en_GB.bla", English, UnitedKingdom)
    TEST_CTOR("en_GB@.bla", English, UnitedKingdom)
    TEST_CTOR("en_GB@bla", English, UnitedKingdom)
    TEST_CTOR("en-GB", English, UnitedKingdom)
    TEST_CTOR("en-GB@bla", English, UnitedKingdom)

    TEST_CTOR("nb", NorwegianBokmal, Norway)
    TEST_CTOR("nn", NorwegianNynorsk, Norway)
    TEST_CTOR("nb_NO", NorwegianBokmal, Norway)
    TEST_CTOR("nn_NO", NorwegianNynorsk, Norway)
    TEST_CTOR("es_ES", Spanish, Spain)
    TEST_CTOR("es_419", Spanish, LatinAmerica)
    TEST_CTOR("es-419", Spanish, LatinAmerica)
    TEST_CTOR("fr_MA", French, Morocco)

    // test default countries for languages
    TEST_CTOR("zh", Chinese, China)
    TEST_CTOR("zh-Hans", Chinese, China)
    TEST_CTOR("mn", Mongolian, Mongolia)
    TEST_CTOR("ne", Nepali, Nepal)
    TEST_CTOR("ne_NP", Nepali, Nepal)
    TEST_CTOR("ne_IN", Nepali, India)

#undef TEST_CTOR
#define TEST_CTOR(req_lc, exp_lang, exp_script, exp_country) \
    { \
    QLocale l(req_lc); \
    QVERIFY2(l.language() == QLocale::exp_lang \
        && l.script() == QLocale::exp_script \
        && l.country() == QLocale::exp_country, \
        QString("requested: \"" + QString(req_lc) + "\", got: " \
        + QLocale::languageToString(l.language()) \
        + "/" + QLocale::scriptToString(l.script()) \
        + "/" + QLocale::countryToString(l.country())).toLatin1().constData()); \
    }

    TEST_CTOR("zh_CN", Chinese, SimplifiedScript, China)
    TEST_CTOR("zh_Hans_CN", Chinese, SimplifiedHanScript, China)
    TEST_CTOR("zh_Hans", Chinese, SimplifiedHanScript, China)
    TEST_CTOR("zh_Hant", Chinese, TraditionalHanScript, HongKong)
    TEST_CTOR("zh_Hans_MO", Chinese, SimplifiedHanScript, Macao)
    TEST_CTOR("zh_Hant_MO", Chinese, TraditionalHanScript, Macao)
    TEST_CTOR("az_Latn_AZ", Azerbaijani, LatinScript, Azerbaijan)
    TEST_CTOR("ha_NG", Hausa, AnyScript, Nigeria)
    // substitutions
    TEST_CTOR("ha_Latn_GH", Hausa, AnyScript, Ghana)
    TEST_CTOR("ha_Latn_NG", Hausa, AnyScript, Nigeria)

#undef TEST_CTOR
}

void tst_QLocale::emptyCtor()
{
#define TEST_CTOR(req_lc, exp_str) \
    { \
    /* Test constructor without arguments. Needs separate process */ \
    /* because of caching of the system locale. */ \
    QProcess process; \
    process.setEnvironment(QStringList(env) << QString("LANG=%1").arg(req_lc)); \
    process.start(m_sysLocaleApp); \
    QVERIFY(process.waitForStarted()); \
    process.waitForReadyRead(); \
    QString ret = QString(process.readAll()); \
    process.waitForFinished(); \
    QVERIFY2(process.exitStatus() == QProcess::NormalExit, "Cannot launch external process"); \
    QVERIFY2(QString(exp_str) == ret, QString("Expected: " + QString(exp_str) + ", got: " \
            + ret + ". Requested: " + QString(req_lc)).toLatin1().constData()); \
    }

    // Get an environment free of any locale-related variables
    QStringList env;
    foreach (QString const& entry, QProcess::systemEnvironment()) {
        if (entry.startsWith("LANG=") || entry.startsWith("LC_"))
            continue;
        env << entry;
    }

    // Get default locale.
    QProcess p;
    p.setEnvironment(env);
    p.start(m_sysLocaleApp);
    p.waitForReadyRead();
    QString defaultLoc = QString(p.readAll());
    p.waitForFinished();

    TEST_CTOR("C", "C")
    TEST_CTOR("bla", "C")
    TEST_CTOR("zz", "C")
    TEST_CTOR("zz_zz", "C")
    TEST_CTOR("zz...", "C")
    TEST_CTOR("en", "en_US")
    TEST_CTOR("en", "en_US")
    TEST_CTOR("en.", "en_US")
    TEST_CTOR("en@", "en_US")
    TEST_CTOR("en.@", "en_US")
    TEST_CTOR("en_", "en_US")
    TEST_CTOR("en_.", "en_US")
    TEST_CTOR("en_.@", "en_US")
    TEST_CTOR("en.bla", "en_US")
    TEST_CTOR("en@bla", "en_US")
    TEST_CTOR("en_blaaa", "en_US")
    TEST_CTOR("en_zz", "en_001")
    TEST_CTOR("en_GB", "en_GB")
    TEST_CTOR("en_GB.bla", "en_GB")
    TEST_CTOR("en_GB@.bla", "en_GB")
    TEST_CTOR("en_GB@bla", "en_GB")
    TEST_CTOR("en_US", "en_US")
    TEST_CTOR("en_US.bla", "en_US")
    TEST_CTOR("en_US@.bla", "en_US")
    TEST_CTOR("en_US@bla", "en_US")
    TEST_CTOR("de", "de_DE")
    TEST_CTOR("de_DE", "de_DE")
    TEST_CTOR("de_AT", "de_AT")

    TEST_CTOR("nb", "nb_NO")
    TEST_CTOR("nn", "nn_NO")
    TEST_CTOR("nb_NO", "nb_NO")
    TEST_CTOR("nn_NO", "nn_NO")

    TEST_CTOR("DE", "de_DE");
    TEST_CTOR("DE_DE", "de_DE");
    TEST_CTOR("DE_AT", "de_AT");
    TEST_CTOR("EN", "en_US");
    TEST_CTOR("EN_US", "en_US");
    TEST_CTOR("EN_001", "en_001");

    TEST_CTOR("en/", defaultLoc)
    TEST_CTOR("asdfghj", defaultLoc);
    TEST_CTOR("123456", defaultLoc);

#undef TEST_CTOR
}

void tst_QLocale::unixLocaleName()
{
#define TEST_NAME(req_lang, req_country, exp_name) \
    { \
        QLocale l(QLocale::req_lang, QLocale::req_country); \
        QCOMPARE(l.name(), QString(exp_name)); \
    }

    QLocale::setDefault(QLocale(QLocale::C));

    TEST_NAME(C, AnyCountry, "C")
    TEST_NAME(English, world, "en_001")
    TEST_NAME(English, AnyCountry, "en_US")
    TEST_NAME(English, UnitedStates, "en_US")
    TEST_NAME(English, UnitedKingdom, "en_GB")
    TEST_NAME(Aymara, UnitedKingdom, "C")

#undef TEST_NAME
}

void tst_QLocale::matchingLocales()
{
    const QLocale c(QLocale::C);
    const QLocale ru_RU(QLocale::Russian, QLocale::Russia);

    QList<QLocale> locales = QLocale::matchingLocales(QLocale::C, QLocale::AnyScript, QLocale::AnyCountry);
    QCOMPARE(locales.size(), 1);
    QVERIFY(locales.contains(c));

    locales = QLocale::matchingLocales(QLocale::Russian, QLocale::CyrillicScript, QLocale::Russia);
    QCOMPARE(locales.size(), 1);
    QVERIFY(locales.contains(ru_RU));

    locales = QLocale::matchingLocales(QLocale::Russian, QLocale::AnyScript, QLocale::AnyCountry);
    QVERIFY(!locales.isEmpty());
    QVERIFY(!locales.contains(c));
    QVERIFY(locales.contains(ru_RU));

    locales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::CyrillicScript, QLocale::AnyCountry);
    QVERIFY(!locales.isEmpty());
    QVERIFY(!locales.contains(c));
    QVERIFY(locales.contains(ru_RU));

    locales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::Russia);
    QVERIFY(!locales.isEmpty());
    QVERIFY(!locales.contains(c));
    QVERIFY(locales.contains(ru_RU));
}

void tst_QLocale::double_conversion_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("num_str");
    QTest::addColumn<bool>("good");
    QTest::addColumn<double>("num");

    QTest::newRow("C 1")         << QString("C") << QString("1")          << true  << 1.0;
    QTest::newRow("C 1.0")       << QString("C") << QString("1.0")        << true  << 1.0;
    QTest::newRow("C 1.234")     << QString("C") << QString("1.234")      << true  << 1.234;
    QTest::newRow("C 1.234e-10") << QString("C") << QString("1.234e-10")  << true  << 1.234e-10;
    QTest::newRow("C 1.234E10")  << QString("C") << QString("1.234E10")   << true  << 1.234e10;
    QTest::newRow("C 1e10")      << QString("C") << QString("1e10")       << true  << 1.0e10;
    QTest::newRow("C  1")        << QString("C") << QString(" 1")         << true  << 1.0;
    QTest::newRow("C   1")       << QString("C") << QString("  1")        << true  << 1.0;
    QTest::newRow("C 1 ")        << QString("C") << QString("1 ")         << true  << 1.0;
    QTest::newRow("C 1  ")       << QString("C") << QString("1  ")        << true  << 1.0;

    QTest::newRow("C 1,")	        << QString("C") << QString("1,")	 << false << 0.0;
    QTest::newRow("C 1,2")	        << QString("C") << QString("1,2")	 << false << 0.0;
    QTest::newRow("C 1,23")        << QString("C") << QString("1,23")	 << false << 0.0;
    QTest::newRow("C 1,234")       << QString("C") << QString("1,234")	 << true  << 1234.0;
    QTest::newRow("C 1,234,")      << QString("C") << QString("1,234,")	 << false << 0.0;
    QTest::newRow("C 1,234,5")     << QString("C") << QString("1,234,5")	 << false << 0.0;
    QTest::newRow("C 1,234,56")    << QString("C") << QString("1,234,56")   << false << 0.0;
    QTest::newRow("C 1,234,567")   << QString("C") << QString("1,234,567")  << true  << 1234567.0;
    QTest::newRow("C 1,234,567.")  << QString("C") << QString("1,234,567.") << true  << 1234567.0;
    QTest::newRow("C 1,234,567.8")	<< QString("C") << QString("1,234,567.8")<< true  << 1234567.8;
    QTest::newRow("C 1,234567.8")  << QString("C") << QString("1,234567.8") << false << 0.0;
    QTest::newRow("C 12,34567.8")  << QString("C") << QString("12,34567.8") << false << 0.0;
    QTest::newRow("C 1234,567.8")  << QString("C") << QString("1234,567.8") << false << 0.0;
    QTest::newRow("C 1234567.8")   << QString("C") << QString("1234567.8")  << true  << 1234567.8;
    QTest::newRow("C ,")	    	<< QString("C") << QString(",")	         << false << 0.0;
    QTest::newRow("C ,123")	<< QString("C") << QString(",123")       << false << 0.0;
    QTest::newRow("C ,3")	    	<< QString("C") << QString(",3")         << false << 0.0;
    QTest::newRow("C , 3")	    	<< QString("C") << QString(", 3")        << false << 0.0;
    QTest::newRow("C ,  3")	<< QString("C") << QString(",  3")       << false << 0.0;
    QTest::newRow("C ,  3.2")	<< QString("C") << QString(",  3.2")     << false << 0.0;
    QTest::newRow("C ,  3.2e2")	<< QString("C") << QString(",  3.2e2")   << false << 0.0;
    QTest::newRow("C ,  e2")	<< QString("C") << QString(",  e2")	 << false << 0.0;
    QTest::newRow("C 1,,234")	<< QString("C") << QString("1,,234")	 << false << 0.0;

    QTest::newRow("C empty")     << QString("C") << QString("")           << false << 0.0;
    QTest::newRow("C null")      << QString("C") << QString()         << false << 0.0;
    QTest::newRow("C .")         << QString("C") << QString(".")          << false << 0.0;
    QTest::newRow("C 1e")        << QString("C") << QString("1e")         << false << 0.0;
    QTest::newRow("C 1,0")       << QString("C") << QString("1,0")        << false << 0.0;
    QTest::newRow("C 1,000")     << QString("C") << QString("1,000")      << true  << 1000.0;
    QTest::newRow("C 1,000e-6")  << QString("C") << QString("1,000e-6")   << true  << 1000.0e-6;
    QTest::newRow("C 1e1.0")     << QString("C") << QString("1e1.0")      << false << 0.0;
    QTest::newRow("C 1e+")       << QString("C") << QString("1e+")        << false << 0.0;
    QTest::newRow("C 1e-")       << QString("C") << QString("1e-")        << false << 0.0;

    QTest::newRow("C .1")        << QString("C") << QString(".1")         << true  << 0.1;
    QTest::newRow("C -.1")       << QString("C") << QString("-.1")        << true  << -0.1;
    QTest::newRow("C 1.")        << QString("C") << QString("1.")         << true  << 1.0;
    QTest::newRow("C 1.E10")     << QString("C") << QString("1.E10")      << true  << 1.0e10;
    QTest::newRow("C 1e+10")     << QString("C") << QString("1e+10")      << true  << 1.0e+10;

    QTest::newRow("de_DE 1.")	    << QString("de_DE") << QString("1.")	 << false << 0.0;
    QTest::newRow("de_DE 1.2")	    << QString("de_DE") << QString("1.2")	 << false << 0.0;
    QTest::newRow("de_DE 1.23")        << QString("de_DE") << QString("1.23")       << false << 0.0;
    QTest::newRow("de_DE 1.234")       << QString("de_DE") << QString("1.234")	 << true  << 1234.0;
    QTest::newRow("de_DE 1.234,")      << QString("de_DE") << QString("1.234.")	 << false << 0.0;
    QTest::newRow("de_DE 1.234.5")     << QString("de_DE") << QString("1.234.5")	 << false << 0.0;
    QTest::newRow("de_DE 1.234.56")    << QString("de_DE") << QString("1.234.56")   << false << 0.0;
    QTest::newRow("de_DE 1.234.567")   << QString("de_DE") << QString("1.234.567")  << true  << 1234567.0;
    QTest::newRow("de_DE 1.234.567,")  << QString("de_DE") << QString("1.234.567,") << true  << 1234567.0;
    QTest::newRow("de_DE 1.234.567,8") << QString("de_DE") << QString("1.234.567,8")<< true  << 1234567.8;
    QTest::newRow("de_DE 1.234567,8")  << QString("de_DE") << QString("1.234567,8") << false << 0.0;
    QTest::newRow("de_DE 12.34567,8")  << QString("de_DE") << QString("12.34567,8") << false << 0.0;
    QTest::newRow("de_DE 1234.567,8")  << QString("de_DE") << QString("1234.567,8") << false << 0.0;
    QTest::newRow("de_DE 1234567,8")   << QString("de_DE") << QString("1234567,8")  << true  << 1234567.8;
    QTest::newRow("de_DE .123")	    << QString("de_DE") << QString(".123")       << false << 0.0;
    QTest::newRow("de_DE .3")	    << QString("de_DE") << QString(".3")         << false << 0.0;
    QTest::newRow("de_DE . 3")	    << QString("de_DE") << QString(". 3")        << false << 0.0;
    QTest::newRow("de_DE .  3")	    << QString("de_DE") << QString(".  3")       << false << 0.0;
    QTest::newRow("de_DE .  3,2")	    << QString("de_DE") << QString(".  3,2")     << false << 0.0;
    QTest::newRow("de_DE .  3,2e2")    << QString("de_DE") << QString(".  3,2e2")   << false << 0.0;
    QTest::newRow("de_DE .  e2")	    << QString("de_DE") << QString(".  e2")	 << false << 0.0;
    QTest::newRow("de_DE 1..234")	    << QString("de_DE") << QString("1..234")	 << false << 0.0;

    QTest::newRow("de_DE 1")         << QString("de_DE") << QString("1")          << true  << 1.0;
    QTest::newRow("de_DE 1.0")       << QString("de_DE") << QString("1.0")        << false << 0.0;
    QTest::newRow("de_DE 1.234e-10") << QString("de_DE") << QString("1.234e-10")  << true  << 1234.0e-10;
    QTest::newRow("de_DE 1.234E10")  << QString("de_DE") << QString("1.234E10")   << true  << 1234.0e10;
    QTest::newRow("de_DE 1e10")      << QString("de_DE") << QString("1e10")       << true  << 1.0e10;
    QTest::newRow("de_DE .1")        << QString("de_DE") << QString(".1")         << false << 0.0;
    QTest::newRow("de_DE -.1")       << QString("de_DE") << QString("-.1")        << false << 0.0;
    QTest::newRow("de_DE 1.E10")     << QString("de_DE") << QString("1.E10")      << false << 0.0;
    QTest::newRow("de_DE 1e+10")     << QString("de_DE") << QString("1e+10")      << true  << 1.0e+10;

    QTest::newRow("de_DE 1,0")       << QString("de_DE") << QString("1,0")        << true  << 1.0;
    QTest::newRow("de_DE 1,234")     << QString("de_DE") << QString("1,234")      << true  << 1.234;
    QTest::newRow("de_DE 1,234e-10") << QString("de_DE") << QString("1,234e-10")  << true  << 1.234e-10;
    QTest::newRow("de_DE 1,234E10")  << QString("de_DE") << QString("1,234E10")   << true  << 1.234e10;
    QTest::newRow("de_DE ,1")        << QString("de_DE") << QString(",1")         << true  << 0.1;
    QTest::newRow("de_DE -,1")       << QString("de_DE") << QString("-,1")        << true  << -0.1;
    QTest::newRow("de_DE 1,")        << QString("de_DE") << QString("1,")         << true  << 1.0;
    QTest::newRow("de_DE 1,E10")     << QString("de_DE") << QString("1,E10")      << true  << 1.0e10;

    QTest::newRow("de_DE empty")     << QString("de_DE") << QString("")           << false << 0.0;
    QTest::newRow("de_DE null")      << QString("de_DE") << QString()         << false << 0.0;
    QTest::newRow("de_DE .")         << QString("de_DE") << QString(".")          << false << 0.0;
    QTest::newRow("de_DE 1e")        << QString("de_DE") << QString("1e")         << false << 0.0;
    QTest::newRow("de_DE 1e1.0")     << QString("de_DE") << QString("1e1.0")      << false << 0.0;
    QTest::newRow("de_DE 1e+")       << QString("de_DE") << QString("1e+")        << false << 0.0;
    QTest::newRow("de_DE 1e-")       << QString("de_DE") << QString("1e-")        << false << 0.0;

    QTest::newRow("C 9,876543")      << QString("C") << QString("9,876543")        << false << 0.0;
    QTest::newRow("C 9,876543.2")    << QString("C") << QString("9,876543.2")      << false << 0.0;
    QTest::newRow("C 9,876543e-2")   << QString("C") << QString("9,876543e-2")     << false << 0.0;
    QTest::newRow("C 9,876543.0e-2") << QString("C") << QString("9,876543.0e-2")   << false << 0.0;

    QTest::newRow("de_DE 9.876543")      << QString("de_DE") << QString("9876.543")        << false << 0.0;
    QTest::newRow("de_DE 9.876543,2")    << QString("de_DE") << QString("9.876543,2")      << false << 0.0;
    QTest::newRow("de_DE 9.876543e-2")   << QString("de_DE") << QString("9.876543e-2")     << false << 0.0;
    QTest::newRow("de_DE 9.876543,0e-2") << QString("de_DE") << QString("9.876543,0e-2")   << false << 0.0;
    QTest::newRow("de_DE 9.876543e--2")   << QString("de_DE") << QString("9.876543e")+QChar(8722)+QString("2")     << false << 0.0;
    QTest::newRow("de_DE 9.876543,0e--2") << QString("de_DE") << QString("9.876543,0e")+QChar(8722)+QString("2")   << false << 0.0;
}

void tst_QLocale::double_conversion()
{
#define MY_DOUBLE_EPSILON (2.22045e-16)

    QFETCH(QString, locale_name);
    QFETCH(QString, num_str);
    QFETCH(bool, good);
    QFETCH(double, num);

    QLocale locale(locale_name);
    QCOMPARE(locale.name(), locale_name);

    bool ok;
    double d = locale.toDouble(num_str, &ok);
    QCOMPARE(ok, good);

    if (ok) {
        double diff = d - num;
        if (diff < 0)
            diff = -diff;
        QVERIFY(diff <= MY_DOUBLE_EPSILON);
    }
}

void tst_QLocale::long_long_conversion_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("num_str");
    QTest::addColumn<bool>("good");
    QTest::addColumn<qlonglong>("num");

    QTest::newRow("C null")       << QString("C") << QString()	   << false  << (qlonglong) 0;
    QTest::newRow("C empty")      << QString("C") << QString("")	   << false  << (qlonglong) 0;
    QTest::newRow("C 1")          << QString("C") << "1"	   << true  << (qlonglong) 1;
    QTest::newRow("C 1,")         << QString("C") << "1,"     << false << (qlonglong) 0;
    QTest::newRow("C 1,2")        << QString("C") << "1,2"    << false << (qlonglong) 0;
    QTest::newRow("C 1,23")       << QString("C") << "1,23"   << false << (qlonglong) 0;
    QTest::newRow("C 1,234")      << QString("C") << "1,234"  << true  << (qlonglong) 1234;
    QTest::newRow("C 1234567")    << QString("C") << "1234567"<< true  << (qlonglong) 1234567;
    QTest::newRow("C 1,234567")    << QString("C") << "1,234567"<< false  << (qlonglong) 0;
    QTest::newRow("C 12,34567")    << QString("C") << "12,34567"<< false  << (qlonglong) 0;
    QTest::newRow("C 123,4567")    << QString("C") << "123,4567"<< false  << (qlonglong) 0;
    QTest::newRow("C 1234,567")    << QString("C") << "1234,567"<< false  << (qlonglong) 0;
    QTest::newRow("C 12345,67")    << QString("C") << "12345,67"<< false  << (qlonglong) 0;
    QTest::newRow("C 123456,7")    << QString("C") << "123456,7"<< false  << (qlonglong) 0;
    QTest::newRow("C 1,234,567")    << QString("C")<< "1,234,567"<< true  << (qlonglong) 1234567;

    QTest::newRow("de_DE 1")      << QString("de_DE") << "1"      << true  << (qlonglong) 1;
    QTest::newRow("de_DE 1.")     << QString("de_DE") << "1."     << false << (qlonglong) 0;
    QTest::newRow("de_DE 1.2")    << QString("de_DE") << "1.2"    << false << (qlonglong) 0;
    QTest::newRow("de_DE 1.23")   << QString("de_DE") << "1.23"   << false << (qlonglong) 0;
    QTest::newRow("de_DE 1.234")  << QString("de_DE") << "1.234"  << true  << (qlonglong) 1234;
    QTest::newRow("de_DE 1234567")     << QString("de_DE") << "1234567"<< true  << (qlonglong) 1234567;
    QTest::newRow("de_DE 1.234567")    << QString("de_DE") << "1.234567"<< false  << (qlonglong) 0;
    QTest::newRow("de_DE 12.34567")    << QString("de_DE") << "12.34567"<< false  << (qlonglong) 0;
    QTest::newRow("de_DE 123.4567")    << QString("de_DE") << "123.4567"<< false  << (qlonglong) 0;
    QTest::newRow("de_DE 1234.567")    << QString("de_DE") << "1234.567"<< false  << (qlonglong) 0;
    QTest::newRow("de_DE 12345.67")    << QString("de_DE") << "12345.67"<< false  << (qlonglong) 0;
    QTest::newRow("de_DE 123456.7")    << QString("de_DE") << "123456.7"<< false  << (qlonglong) 0;
    QTest::newRow("de_DE 1.234.567")   << QString("de_DE")<< "1.234.567"<< true  << (qlonglong) 1234567;
    QTest::newRow("de_DE 1.234.567 ldspcs")   << QString("de_DE")<< "  1.234.567" << true  << (qlonglong) 1234567;
    QTest::newRow("de_DE 1.234.567 trspcs")   << QString("de_DE")<< "1.234.567  "<< true  << (qlonglong) 1234567;
    QTest::newRow("de_DE 1.234.567 ldtrspcs")   << QString("de_DE")<< "  1.234.567  "<< true  << (qlonglong) 1234567;

    QTest::newRow("de_DE 123 ldspcs")   << QString("de_DE")<< "  123" << true  << (qlonglong) 123;
    QTest::newRow("de_DE 123 trspcs")   << QString("de_DE")<< "123  "<< true  << (qlonglong) 123;
    QTest::newRow("de_DE 123 ldtrspcs")   << QString("de_DE")<< "  123  "<< true  << (qlonglong) 123;

    QTest::newRow("C   1234")       << QString("C") << "  1234"   << true  << (qlonglong) 1234;
    QTest::newRow("C 1234  ")       << QString("C") << "1234  "   << true  << (qlonglong) 1234;
    QTest::newRow("C   1234  ")     << QString("C") << "  1234  " << true  << (qlonglong) 1234;
}

void tst_QLocale::long_long_conversion()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, num_str);
    QFETCH(bool, good);
    QFETCH(qlonglong, num);

    QLocale locale(locale_name);
    QCOMPARE(locale.name(), locale_name);

    bool ok;
    qlonglong l = locale.toLongLong(num_str, &ok);
    QCOMPARE(ok, good);

    if (ok) {
        QCOMPARE(l, num);
    }
}

void tst_QLocale::long_long_conversion_extra()
{
    QLocale l(QLocale::C);
    QCOMPARE(l.toString((qlonglong)1), QString("1"));
    QCOMPARE(l.toString((qlonglong)12), QString("12"));
    QCOMPARE(l.toString((qlonglong)123), QString("123"));
    QCOMPARE(l.toString((qlonglong)1234), QString("1,234"));
    QCOMPARE(l.toString((qlonglong)12345), QString("12,345"));
    QCOMPARE(l.toString((qlonglong)-1), QString("-1"));
    QCOMPARE(l.toString((qlonglong)-12), QString("-12"));
    QCOMPARE(l.toString((qlonglong)-123), QString("-123"));
    QCOMPARE(l.toString((qlonglong)-1234), QString("-1,234"));
    QCOMPARE(l.toString((qlonglong)-12345), QString("-12,345"));
    QCOMPARE(l.toString((qulonglong)1), QString("1"));
    QCOMPARE(l.toString((qulonglong)12), QString("12"));
    QCOMPARE(l.toString((qulonglong)123), QString("123"));
    QCOMPARE(l.toString((qulonglong)1234), QString("1,234"));
    QCOMPARE(l.toString((qulonglong)12345), QString("12,345"));
}

/*
void tst_QLocale::languageToString()
{
}

void tst_QLocale::setDefault()
{
}
*/

void tst_QLocale::testInfAndNan()
{
    double neginf = log(0.0);
    double nan = sqrt(-1.0);

    QVERIFY(qIsInf(-neginf));
    QVERIFY(!qIsNaN(-neginf));
    QVERIFY(!qIsFinite(-neginf));

    QVERIFY(!qIsInf(nan));
    QVERIFY(qIsNaN(nan));
    QVERIFY(!qIsFinite(nan));

    QVERIFY(!qIsInf(1.234));
    QVERIFY(!qIsNaN(1.234));
    QVERIFY(qIsFinite(1.234));
}

void tst_QLocale::fpExceptions()
{
#ifdef QT_HAVE_FEENABLEEXCEPT
    fenv_t envp;
    fegetenv(&envp);
    feclearexcept(FE_ALL_EXCEPT);
    feenableexcept(FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INVALID);
#endif

    QString::number(1000.1245);
    QString::number(1.1);
    QString::number(0.0);

    QVERIFY(true);

#ifdef QT_HAVE_FEENABLEEXCEPT
    fesetenv(&envp);
#endif
}

void tst_QLocale::negativeZero()
{
    double negativeZero( 0.0 ); // Initialise to zero.
    uchar *ptr = (uchar *)&negativeZero;
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    ptr[0] = 0x80;
#else
    ptr[7] = 0x80;
#endif
    QString s = QString::number(negativeZero);
    QCOMPARE(s, QString("0"));
}

void tst_QLocale::dayOfWeek_data()
{
    QTest::addColumn<QDate>("date");
    QTest::addColumn<QString>("shortName");
    QTest::addColumn<QString>("longName");

    QTest::newRow("Sun") << QDate(2006, 1, 1) << "Sun" << "Sunday";
    QTest::newRow("Mon") << QDate(2006, 1, 2) << "Mon" << "Monday";
    QTest::newRow("Tue") << QDate(2006, 1, 3) << "Tue" << "Tuesday";
    QTest::newRow("Wed") << QDate(2006, 1, 4) << "Wed" << "Wednesday";
    QTest::newRow("Thu") << QDate(2006, 1, 5) << "Thu" << "Thursday";
    QTest::newRow("Fri") << QDate(2006, 1, 6) << "Fri" << "Friday";
    QTest::newRow("Sat") << QDate(2006, 1, 7) << "Sat" << "Saturday";
}

void tst_QLocale::dayOfWeek()
{
    QFETCH(QDate, date);
    QFETCH(QString, shortName);
    QFETCH(QString, longName);

    QCOMPARE(QLocale::c().toString(date, "ddd"), shortName);
    QCOMPARE(QLocale::c().toString(date, "dddd"), longName);
}

void tst_QLocale::formatDate_data()
{
    QTest::addColumn<QDate>("date");
    QTest::addColumn<QString>("format");
    QTest::addColumn<QString>("result");

    QTest::newRow("1") << QDate(1974, 12, 1) << "d/M/yyyy" << "1/12/1974";
    QTest::newRow("2") << QDate(1974, 12, 1) << "d/M/yyyyy" << "1/12/1974y";
    QTest::newRow("4") << QDate(1974, 1, 1) << "d/M/yyyy" << "1/1/1974";
    QTest::newRow("5") << QDate(1974, 1, 1) << "dd/MM/yyy" << "01/01/74y";
    QTest::newRow("6") << QDate(1974, 12, 1) << "ddd/MMM/yy" << "Sun/Dec/74";
    QTest::newRow("7") << QDate(1974, 12, 1) << "dddd/MMMM/y" << "Sunday/December/y";
    QTest::newRow("8") << QDate(1974, 12, 1) << "ddddd/MMMMM/yy" << "Sunday1/December12/74";
    QTest::newRow("9") << QDate(1974, 12, 1) << "'dddd'/MMMM/yy" << "dddd/December/74";
    QTest::newRow("10") << QDate(1974, 12, 1) << "d'dd'd/MMMM/yyy" << "1dd1/December/74y";
    QTest::newRow("11") << QDate(1974, 12, 1) << "d'dd'd/MMM'M'/yy" << "1dd1/DecM/74";
    QTest::newRow("12") << QDate(1974, 12, 1) << "d'd'dd/M/yy" << "1d01/12/74";

    QTest::newRow("20") << QDate(1974, 12, 1) << "foo" << "foo";
    QTest::newRow("21") << QDate(1974, 12, 1) << "'" << "";
    QTest::newRow("22") << QDate(1974, 12, 1) << "''" << "'";
    QTest::newRow("23") << QDate(1974, 12, 1) << "'''" << "'";
    QTest::newRow("24") << QDate(1974, 12, 1) << "\"" << "\"";
    QTest::newRow("25") << QDate(1974, 12, 1) << "\"\"" << "\"\"";
    QTest::newRow("26") << QDate(1974, 12, 1) << "\"yy\"" << "\"74\"";
    QTest::newRow("27") << QDate(1974, 12, 1) << "'\"yy\"'" << "\"yy\"";
    QTest::newRow("28") << QDate() << "'\"yy\"'" << "";
    QTest::newRow("29") << QDate(1974, 12, 1) << "hh:mm:ss.zzz ap d'd'dd/M/yy" << "hh:mm:ss.zzz ap 1d01/12/74";

}

void tst_QLocale::formatDate()
{
    QFETCH(QDate, date);
    QFETCH(QString, format);
    QFETCH(QString, result);

    QLocale l(QLocale::C);
    QCOMPARE(l.toString(date, format), result);
}

Q_DECLARE_METATYPE(QTime)

void tst_QLocale::formatTime_data()
{
    QTest::addColumn<QTime>("time");
    QTest::addColumn<QString>("format");
    QTest::addColumn<QString>("result");

    QTest::newRow("1") << QTime(1, 2, 3) << "h:m:s" << "1:2:3";
    QTest::newRow("3") << QTime(1, 2, 3) << "H:m:s" << "1:2:3";
    QTest::newRow("4") << QTime(1, 2, 3) << "hh:mm:ss" << "01:02:03";
    QTest::newRow("5") << QTime(1, 2, 3) << "HH:mm:ss" << "01:02:03";
    QTest::newRow("6") << QTime(1, 2, 3) << "hhh:mmm:sss" << "011:022:033";

    QTest::newRow("8") << QTime(14, 2, 3) << "h:m:s" << "14:2:3";
    QTest::newRow("9") << QTime(14, 2, 3) << "H:m:s" << "14:2:3";
    QTest::newRow("10") << QTime(14, 2, 3) << "hh:mm:ss" << "14:02:03";
    QTest::newRow("11") << QTime(14, 2, 3) << "HH:mm:ss" << "14:02:03";
    QTest::newRow("12") << QTime(14, 2, 3) << "hhh:mmm:sss" << "1414:022:033";

    QTest::newRow("14") << QTime(14, 2, 3) << "h:m:s ap" << "2:2:3 pm";
    QTest::newRow("15") << QTime(14, 2, 3) << "H:m:s AP" << "14:2:3 PM";
    QTest::newRow("16") << QTime(14, 2, 3) << "hh:mm:ss aap" << "02:02:03 pmpm";
    QTest::newRow("17") << QTime(14, 2, 3) << "HH:mm:ss AP aa" << "14:02:03 PM pmpm";

    QTest::newRow("18") << QTime(1, 2, 3) << "h:m:s ap" << "1:2:3 am";
    QTest::newRow("19") << QTime(1, 2, 3) << "H:m:s AP" << "1:2:3 AM";

    QTest::newRow("20") << QTime(1, 2, 3) << "foo" << "foo";
    QTest::newRow("21") << QTime(1, 2, 3) << "'" << "";
    QTest::newRow("22") << QTime(1, 2, 3) << "''" << "'";
    QTest::newRow("23") << QTime(1, 2, 3) << "'''" << "'";
    QTest::newRow("24") << QTime(1, 2, 3) << "\"" << "\"";
    QTest::newRow("25") << QTime(1, 2, 3) << "\"\"" << "\"\"";
    QTest::newRow("26") << QTime(1, 2, 3) << "\"H\"" << "\"1\"";
    QTest::newRow("27") << QTime(1, 2, 3) << "'\"H\"'" << "\"H\"";

    QTest::newRow("28") << QTime(1, 2, 3, 456) << "H:m:s.z" << "1:2:3.456";
    QTest::newRow("29") << QTime(1, 2, 3, 456) << "H:m:s.zz" << "1:2:3.456456";
    QTest::newRow("30") << QTime(1, 2, 3, 456) << "H:m:s.zzz" << "1:2:3.456";
    QTest::newRow("31") << QTime(1, 2, 3, 4) << "H:m:s.z" << "1:2:3.4";
    QTest::newRow("32") << QTime(1, 2, 3, 4) << "H:m:s.zzz" << "1:2:3.004";
    QTest::newRow("33") << QTime() << "H:m:s.zzz" << "";
    QTest::newRow("34") << QTime(1, 2, 3, 4) << "dd MM yyyy H:m:s.zzz" << "dd MM yyyy 1:2:3.004";
}

void tst_QLocale::formatTime()
{
    QFETCH(QTime, time);
    QFETCH(QString, format);
    QFETCH(QString, result);

    QLocale l(QLocale::C);
    QCOMPARE(l.toString(time, format), result);
}


void tst_QLocale::formatDateTime_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QDateTime>("dateTime");
    QTest::addColumn<QString>("format");
    QTest::addColumn<QString>("result");

    QTest::newRow("1C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(5, 14, 13))
                        << "d/M/yyyy hh:h:mm" << "1/12/1974 05:5:14";
    QTest::newRow("2C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                        << "d/M/yyyyy h" << "1/12/1974y 15";
    QTest::newRow("4C") << "C" << QDateTime(QDate(1974, 1, 1), QTime(15, 14, 13))
                        << "d/M/yyyy zzz" << "1/1/1974 000";
    QTest::newRow("5C") << "C" << QDateTime(QDate(1974, 1, 1), QTime(15, 14, 13))
                        << "dd/MM/yyy z" << "01/01/74y 0";
    QTest::newRow("6C") << "C" << QDateTime(QDate(1974, 12, 2), QTime(15, 14, 13))
                        << "ddd/MMM/yy AP" << "Mon/Dec/74 PM";
    QTest::newRow("7C") << "C" << QDateTime(QDate(1974, 12, 2), QTime(15, 14, 13))
                        << "dddd/MMMM/y apa" << "Monday/December/y pmpm";
    QTest::newRow("8C") << "C" << QDateTime(QDate(1974, 12, 2), QTime(15, 14, 13))
                        << "ddddd/MMMMM/yy ss" << "Monday2/December12/74 13";
    QTest::newRow("9C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                        << "'dddd'/MMMM/yy s" << "dddd/December/74 13";
    QTest::newRow("10C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 4, 13))
                         << "d'dd'd/MMMM/yyy m'm'mm" << "1dd1/December/74y 4m04";
    QTest::newRow("11C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 3))
                         << "d'dd'd/MMM'M'/yysss" << "1dd1/DecM/74033";
    QTest::newRow("12C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                         << "d'd'dd/M/yyh" << "1d01/12/7415";

    QTest::newRow("20C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                         << "foo" << "foo";
    QTest::newRow("21C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                         << "'" << "";
    QTest::newRow("22C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                         << "''" << "'";
    QTest::newRow("23C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                         << "'''" << "'";
    QTest::newRow("24C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                         << "\"" << "\"";
    QTest::newRow("25C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                         << "\"\"" << "\"\"";
    QTest::newRow("26C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                         << "\"yymm\"" << "\"7414\"";
    QTest::newRow("27C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                         << "'\"yymm\"'" << "\"yymm\"";
    QTest::newRow("27C") << "C" << QDateTime()
                         << "'\"yymm\"'" << "";

    QTest::newRow("1nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(5, 14, 13))
                            << "d/M/yyyy hh:h:mm" << "1/12/1974 05:5:14";
    QTest::newRow("2nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                            << "d/M/yyyyy h" << "1/12/1974y 15";
    QTest::newRow("4nn_NO") << "nn_NO" << QDateTime(QDate(1974, 1, 1), QTime(15, 14, 13))
                            << "d/M/yyyy zzz" << "1/1/1974 000";
    QTest::newRow("5nn_NO") << "nn_NO" << QDateTime(QDate(1974, 1, 1), QTime(15, 14, 13))
                            << "dd/MM/yyy z" << "01/01/74y 0";
    QTest::newRow("6nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 2), QTime(15, 14, 13))
                            << "ddd/MMM/yy AP" << "må./des./74 PM";
    QTest::newRow("7nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 2), QTime(15, 14, 13))
                            << "dddd/MMMM/y apa" << "måndag/M12/y pmpm";
    QTest::newRow("8nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 2), QTime(15, 14, 13))
                            << "ddddd/MMMMM/yy ss" << "måndag2/M1212/74 13";
    QTest::newRow("9nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                            << "'dddd'/MMMM/yy s" << "dddd/M12/74 13";
    QTest::newRow("10nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 4, 13))
                             << "d'dd'd/MMMM/yyy m'm'mm" << "1dd1/M12/74y 4m04";
    QTest::newRow("11nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 3))
                             << "d'dd'd/MMM'M'/yysss" << "1dd1/des.M/74033";
    QTest::newRow("12nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                             << "d'd'dd/M/yyh" << "1d01/12/7415";

    QTest::newRow("20nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                             << "foo" << "foo";
    QTest::newRow("21nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                             << "'" << "";
    QTest::newRow("22nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                             << "''" << "'";
    QTest::newRow("23nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                             << "'''" << "'";
    QTest::newRow("24nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                             << "\"" << "\"";
    QTest::newRow("25nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                             << "\"\"" << "\"\"";
    QTest::newRow("26nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                             << "\"yymm\"" << "\"7414\"";
    QTest::newRow("27nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 14, 13))
                             << "'\"yymm\"'" << "\"yymm\"";
    QTest::newRow("27nn_NO") << "nn_NO" << QDateTime()
                             << "'\"yymm\"'" << "";

}

void tst_QLocale::formatDateTime()
{
    QFETCH(QString, localeName);
    QFETCH(QDateTime, dateTime);
    QFETCH(QString, format);
    QFETCH(QString, result);

    QLocale l(localeName);
    QCOMPARE(l.toString(dateTime, format), result);
}

void tst_QLocale::toDateTime_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QDateTime>("result");
    QTest::addColumn<QString>("format");
    QTest::addColumn<QString>("string");

    QTest::newRow("1C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(5, 14, 0))
                        << "d/M/yyyy hh:h:mm" << "1/12/1974 05:5:14";
    QTest::newRow("2C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 0, 0))
                        << "d/M/yyyyy h" << "1/12/1974y 15";
    QTest::newRow("4C") << "C" << QDateTime(QDate(1974, 1, 1), QTime(0, 0, 0))
                        << "d/M/yyyy zzz" << "1/1/1974 000";
    QTest::newRow("5C") << "C" << QDateTime(QDate(1974, 1, 1), QTime(0, 0, 0))
                        << "dd/MM/yyy z" << "01/01/74y 0";
    QTest::newRow("8C") << "C" << QDateTime(QDate(1974, 12, 2), QTime(0, 0, 13))
                        << "ddddd/MMMMM/yy ss" << "Monday2/December12/74 13";
    QTest::newRow("9C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(0, 0, 13))
                        << "'dddd'/MMMM/yy s" << "dddd/December/74 13";
    QTest::newRow("10C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(0, 4, 0))
                         << "d'dd'd/MMMM/yyy m'm'mm" << "1dd1/December/74y 4m04";
    QTest::newRow("11C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(0, 0, 3))
                         << "d'dd'd/MMM'M'/yysss" << "1dd1/DecM/74033";
    QTest::newRow("12C") << "C" << QDateTime(QDate(1974, 12, 1), QTime(15, 0, 0))
                         << "d'd'dd/M/yyh" << "1d01/12/7415";

    QTest::newRow("1nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(5, 14, 0))
                            << "d/M/yyyy hh:h:mm" << "1/12/1974 05:5:14";
    QTest::newRow("2nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 0, 0))
                            << "d/M/yyyyy h" << "1/12/1974y 15";
    QTest::newRow("4nn_NO") << "nn_NO" << QDateTime(QDate(1974, 1, 1), QTime(0, 0, 0))
                            << "d/M/yyyy zzz" << "1/1/1974 000";
    QTest::newRow("5nn_NO") << "nn_NO" << QDateTime(QDate(1974, 1, 1), QTime(0, 0, 0))
                            << "dd/MM/yyy z" << "01/01/74y 0";
    QTest::newRow("8nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 2), QTime(0, 0, 13))
                            << "ddddd/MMMMM/yy ss" << "måndag2/M1212/74 13";
    QTest::newRow("9nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(0, 0, 13))
                            << "'dddd'/MMMM/yy s" << "dddd/M12/74 13";
    QTest::newRow("10nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(0, 4, 0))
                             << "d'dd'd/MMMM/yyy m'm'mm" << "1dd1/M12/74y 4m04";
    QTest::newRow("11nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(0, 0, 3))
                             << "d'dd'd/MMM'M'/yysss" << "1dd1/des.M/74033";
    QTest::newRow("12nn_NO") << "nn_NO" << QDateTime(QDate(1974, 12, 1), QTime(15, 0, 0))
                             << "d'd'dd/M/yyh" << "1d01/12/7415";

    QTest::newRow("RFC-1123") << "C" << QDateTime(QDate(2007, 11, 1), QTime(18, 8, 30))
                              << "ddd, dd MMM yyyy hh:mm:ss 'GMT'" << "Thu, 01 Nov 2007 18:08:30 GMT";

    QTest::newRow("longFormat") << "en_US" << QDateTime(QDate(2009, 1, 5), QTime(11, 48, 32))
                      << "dddd, MMMM d, yyyy h:mm:ss AP " << "Monday, January 5, 2009 11:48:32 AM ";
}

void tst_QLocale::toDateTime()
{
    QFETCH(QString, localeName);
    QFETCH(QDateTime, result);
    QFETCH(QString, format);
    QFETCH(QString, string);

    QLocale l(localeName);
    QCOMPARE(l.toDateTime(string, format), result);
    if (l.dateTimeFormat(QLocale::LongFormat) == format)
        QCOMPARE(l.toDateTime(string, QLocale::LongFormat), result);
}

void tst_QLocale::numberOptions()
{
    bool ok;

    QLocale locale(QLocale::C);
    QCOMPARE(locale.numberOptions(), 0);
    QCOMPARE(locale.toInt(QString("12,345"), &ok), 12345);
    QVERIFY(ok);
    QCOMPARE(locale.toInt(QString("12345"), &ok), 12345);
    QVERIFY(ok);
    QCOMPARE(locale.toString(12345), QString("12,345"));

    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    QCOMPARE(locale.numberOptions(), QLocale::OmitGroupSeparator);
    QCOMPARE(locale.toInt(QString("12,345"), &ok), 12345);
    QVERIFY(ok);
    QCOMPARE(locale.toInt(QString("12345"), &ok), 12345);
    QVERIFY(ok);
    QCOMPARE(locale.toString(12345), QString("12345"));

    locale.setNumberOptions(QLocale::RejectGroupSeparator);
    QCOMPARE(locale.numberOptions(), QLocale::RejectGroupSeparator);
    locale.toInt(QString("12,345"), &ok);
    QVERIFY(!ok);
    QCOMPARE(locale.toInt(QString("12345"), &ok), 12345);
    QVERIFY(ok);
    QCOMPARE(locale.toString(12345), QString("12,345"));

    QLocale locale2 = locale;
    QCOMPARE(locale2.numberOptions(), QLocale::RejectGroupSeparator);
}

void tst_QLocale::negativeNumbers()
{
    QLocale locale(QLocale::C);

    bool ok;
    int i;

    i = locale.toInt(QLatin1String("-100"), &ok);
    QVERIFY(ok);
    QCOMPARE(i, -100);

    i = locale.toInt(QLatin1String("-1,000"), &ok);
    QVERIFY(ok);
    QCOMPARE(i, -1000);

    i = locale.toInt(QLatin1String("-1000"), &ok);
    QVERIFY(ok);
    QCOMPARE(i, -1000);

    i = locale.toInt(QLatin1String("-10,000"), &ok);
    QVERIFY(ok);
    QCOMPARE(i, -10000);

    i = locale.toInt(QLatin1String("-10000"), &ok);
    QVERIFY(ok);
    QCOMPARE(i, -10000);

    i = locale.toInt(QLatin1String("-100,000"), &ok);
    QVERIFY(ok);
    QCOMPARE(i, -100000);

    i = locale.toInt(QLatin1String("-100000"), &ok);
    QVERIFY(ok);
    QCOMPARE(i, -100000);

    i = locale.toInt(QLatin1String("-1,000,000"), &ok);
    QVERIFY(ok);
    QCOMPARE(i, -1000000);

    i = locale.toInt(QLatin1String("-1000000"), &ok);
    QVERIFY(ok);
    QCOMPARE(i, -1000000);
}

void tst_QLocale::testNames_data()
{
    QTest::addColumn<int>("language");
    QTest::addColumn<int>("country");

    int i = 0;
    foreach (const QLocale &locale, QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry)) {
        const QString testName = QString::fromLatin1("data_%1 (%2/%3)").arg(i)
                .arg(QLocale::languageToString(locale.language()))
                .arg(QLocale::countryToString(locale.country()));
        QTest::newRow(testName.toLatin1().constData()) << (int)locale.language() << (int)locale.country();
        i++;
    }
}

void tst_QLocale::testNames()
{
    QFETCH(int, language);
    QFETCH(int, country);

    QLocale l1((QLocale::Language)language, (QLocale::Country)country);
    if (language == QLocale::AnyLanguage && country == QLocale::AnyCountry)
        language = QLocale::C;
    QCOMPARE((int)l1.language(), language);
    QCOMPARE((int)l1.country(), country);

    QString name = l1.name();

    QLocale l2(name);
    QCOMPARE((int)l2.language(), language);
    QCOMPARE((int)l2.country(), country);
    QCOMPARE(l2.name(), name);

    QLocale l3(name + QLatin1String("@foo"));
    QCOMPARE((int)l3.language(), language);
    QCOMPARE((int)l3.country(), country);
    QCOMPARE(l3.name(), name);

    QLocale l4(name + QLatin1String(".foo"));
    QCOMPARE((int)l4.language(), language);
    QCOMPARE((int)l4.country(), country);
    QCOMPARE(l4.name(), name);

    if (language != QLocale::C) {
        int idx = name.indexOf(QLatin1Char('_'));
        QVERIFY(idx != -1);
        QString lang = name.left(idx);

        QCOMPARE((int)QLocale(lang).language(), language);
        QCOMPARE((int)QLocale(lang + QLatin1String("@foo")).language(), language);
        QCOMPARE((int)QLocale(lang + QLatin1String(".foo")).language(), language);
    }
}

void tst_QLocale::dayName_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("dayName");
    QTest::addColumn<int>("day");
    QTest::addColumn<QLocale::FormatType>("format");

    QTest::newRow("C long")  << QString("C") << QString("Sunday") << 7 << QLocale::LongFormat;
    QTest::newRow("C short")  << QString("C") << QString("Sun") << 7 << QLocale::ShortFormat;
    QTest::newRow("C narrow")  << QString("C") << QString("7") << 7 << QLocale::NarrowFormat;

    QTest::newRow("ru_RU long")  << QString("ru_RU") << QString::fromUtf8("воскресенье") << 7 << QLocale::LongFormat;
    QTest::newRow("ru_RU short")  << QString("ru_RU") << QString::fromUtf8("вс") << 7 << QLocale::ShortFormat;
    QTest::newRow("ru_RU narrow")  << QString("ru_RU") << QString::fromUtf8("7") << 7 << QLocale::NarrowFormat;
}

void tst_QLocale::dayName()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, dayName);
    QFETCH(int, day);
    QFETCH(QLocale::FormatType, format);

    QLocale l(locale_name);
    QCOMPARE(l.dayName(day, format), dayName);

    QLocale ir("ga_IE");
    QCOMPARE(ir.dayName(1, QLocale::ShortFormat), QLatin1String("Luan"));
    QCOMPARE(ir.dayName(7, QLocale::ShortFormat), QLatin1String("Domh"));

    QLocale gr("el_GR");
    QCOMPARE(gr.dayName(2, QLocale::ShortFormat), QString::fromUtf8("\316\244\317\201\316\257"));
    QCOMPARE(gr.dayName(4, QLocale::ShortFormat), QString::fromUtf8("\316\240\316\255\316\274"));
    QCOMPARE(gr.dayName(6, QLocale::ShortFormat), QString::fromUtf8("\316\243\316\254\316\262"));
}

void tst_QLocale::standaloneDayName_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("dayName");
    QTest::addColumn<int>("day");
    QTest::addColumn<QLocale::FormatType>("format");

    QTest::newRow("C invalid: 0 long")  << QString("C") << QString() << 0 << QLocale::LongFormat;
    QTest::newRow("C invalid: 0 short")  << QString("C") << QString() << 0 << QLocale::ShortFormat;
    QTest::newRow("C invalid: 0 narrow")  << QString("C") << QString() << 0 << QLocale::NarrowFormat;
    QTest::newRow("C invalid: 8 long")  << QString("C") << QString() << 8 << QLocale::LongFormat;
    QTest::newRow("C invalid: 8 short")  << QString("C") << QString() << 8 << QLocale::ShortFormat;
    QTest::newRow("C invalid: 8 narrow")  << QString("C") << QString() << 8 << QLocale::NarrowFormat;

    QTest::newRow("C long")  << QString("C") << QString("Sunday") << 7 << QLocale::LongFormat;
    QTest::newRow("C short")  << QString("C") << QString("Sun") << 7 << QLocale::ShortFormat;
    QTest::newRow("C narrow")  << QString("C") << QString("S") << 7 << QLocale::NarrowFormat;

    QTest::newRow("ru_RU long")  << QString("ru_RU") << QString::fromUtf8("воскресенье") << 7 << QLocale::LongFormat;
    QTest::newRow("ru_RU short")  << QString("ru_RU") << QString::fromUtf8("вс") << 7 << QLocale::ShortFormat;
    QTest::newRow("ru_RU narrow")  << QString("ru_RU") << QString::fromUtf8("В") << 7 << QLocale::NarrowFormat;
}

void tst_QLocale::standaloneDayName()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, dayName);
    QFETCH(int, day);
    QFETCH(QLocale::FormatType, format);

    QLocale l(locale_name);
    QCOMPARE(l.standaloneDayName(day, format), dayName);
}

void tst_QLocale::underflowOverflow()
{
    QString a(QLatin1String("0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001e10"));

    bool ok = false;
    a.toDouble(&ok);
    QVERIFY(!ok);

    a = QLatin1String("1e600");
    ok = false;
    a.toDouble(&ok);
    QVERIFY(!ok);

    a = QLatin1String("-9223372036854775809");
    a.toLongLong(&ok);
    QVERIFY(!ok);
}

void tst_QLocale::measurementSystems_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<int>("mSystem");

    QTest::newRow("sv_SE") << QString("sv_SE") << (int)QLocale::MetricSystem; // Swedish/Sweden
    QTest::newRow("en_US") << QString("en_US") << (int)QLocale::ImperialSystem; // English/United States
    QTest::newRow("en_GB") << QString("en_GB") << (int)QLocale::MetricSystem; // English/Great Britain
    QTest::newRow("en")    << QString("en")    << (int)QLocale::ImperialSystem; // English
    QTest::newRow("es_US") << QString("es_US") << (int)QLocale::ImperialSystem; // Spanish/United States
    QTest::newRow("es_ES") << QString("es_ES") << (int)QLocale::MetricSystem; // Spanish/Spain
    QTest::newRow("es")    << QString("es")    << (int)QLocale::MetricSystem; // Spanish
}

void tst_QLocale::measurementSystems()
{
    QSKIP("Meh, skip the test as we do not reread the environment variables anymore", SkipAll);
    QFETCH(QString, localeName);
    QFETCH(int, mSystem);

    // Method should be const.
    const QLocale locale(localeName);
    QCOMPARE((int)locale.measurementSystem(), mSystem);
}

void tst_QLocale::systemMeasurementSystems_data()
{
    QTest::addColumn<QString>("lcAllLocale");
    QTest::addColumn<QString>("lcMeasurementLocale");
    QTest::addColumn<QString>("langLocale");
    QTest::addColumn<int>("mSystem");

    QTest::newRow("row0") << QString("nn_NO") << QString("nn_NO") << QString("nn_NO")
            << (int)QLocale::MetricSystem;
    QTest::newRow("row1") << QString("nn_NO") << QString("nn_NO") << QString("en_US")
            << (int)QLocale::MetricSystem;
    QTest::newRow("row2") << QString("nn_NO") << QString("en_US") << QString("nn_NO")
            << (int)QLocale::MetricSystem;
    QTest::newRow("row3") << QString("nn_NO") << QString("en_US") << QString("en_US")
            << (int)QLocale::MetricSystem;
    QTest::newRow("row4") << QString("en_US") << QString("nn_NO") << QString("nn_NO")
            << (int)QLocale::ImperialSystem;
    QTest::newRow("row5") << QString("en_US") << QString("nn_NO") << QString("en_US")
            << (int)QLocale::ImperialSystem;
    QTest::newRow("row6") << QString("en_US") << QString("en_US") << QString("nn_NO")
            << (int)QLocale::ImperialSystem;
    QTest::newRow("row7") << QString("en_US") << QString("en_US") << QString("en_US")
            << (int)QLocale::ImperialSystem;
    QTest::newRow("row8") << QString("") << QString("") << QString("")
            << (int)QLocale::MetricSystem;
    QTest::newRow("row9") << QString("") << QString("") << QString("nn_NO")
            << (int)QLocale::MetricSystem;
    QTest::newRow("row10") << QString("") << QString("") << QString("en_US")
            << (int)QLocale::ImperialSystem;
    QTest::newRow("row11") << QString("") << QString("nn_NO") << QString("")
            << (int)QLocale::MetricSystem;
    QTest::newRow("row12") << QString("") << QString("en_US") << QString("")
            << (int)QLocale::ImperialSystem;
    QTest::newRow("row13") << QString("") << QString("nn_NO") << QString("en_US")
            << (int)QLocale::MetricSystem;
    QTest::newRow("row14") << QString("") << QString("en_US") << QString("nn_NO")
            << (int)QLocale::ImperialSystem;
    QTest::newRow("row15") << QString("nn_NO") << QString("") << QString("")
            << (int)QLocale::MetricSystem;
    QTest::newRow("row16") << QString("en_US") << QString("") << QString("")
            << (int)QLocale::ImperialSystem;
    QTest::newRow("row17") << QString("nn_NO") << QString("en_US") << QString("")
            << (int)QLocale::MetricSystem;
    QTest::newRow("row18") << QString("en_US") << QString("nn_NO") << QString("")
            << (int)QLocale::ImperialSystem;
    QTest::newRow("row19") << QString("nn_NO") << QString("") << QString("en_US")
            << (int)QLocale::MetricSystem;
    QTest::newRow("row20") << QString("en_US") << QString("") << QString("nn_NO")
            << (int)QLocale::ImperialSystem;
}

void tst_QLocale::systemMeasurementSystems()
{
    QSKIP("Meh, skip the test as we do not reread the environment variables anymore", SkipAll);
    // Theoretically, we could include HPUX in this test, but its setenv implementation
    // stinks. It's called putenv, and it requires you to keep the variable you pass
    // to it around forever.
    QFETCH(QString, lcAllLocale);
    QFETCH(QString, lcMeasurementLocale);
    QFETCH(QString, langLocale);
    QFETCH(int, mSystem);

    // Save any old environment variables.
    QString oldLcAll = QString::fromLocal8Bit(getenv("LC_ALL"));
    QString oldLcMeasurement = QString::fromLocal8Bit(getenv("LC_MEASUREMENT"));
    QString oldLang = QString::fromLocal8Bit(getenv("LANG"));

    qputenv("LC_ALL", lcAllLocale.toLocal8Bit());
    qputenv("LC_MEASUREMENT", lcMeasurementLocale.toLocal8Bit());
    qputenv("LANG", langLocale.toLocal8Bit());

    // Method should be const.
    const QLocale locale(QLocale::system());
    QCOMPARE((int)locale.measurementSystem(), mSystem);

    // Restore environment.
    qputenv("LC_ALL", oldLcAll.toLocal8Bit());
    qputenv("LC_MEASUREMENT", oldLcMeasurement.toLocal8Bit());
    qputenv("LANG", oldLang.toLocal8Bit());
}

void tst_QLocale::defaultNumeringSystem()
{
    QLocale sk("sk_SK");
    QCOMPARE(sk.toString(123), QLatin1String("123"));

    QLocale ta("ta_IN");
    QCOMPARE(ta.toString(123), QLatin1String("123"));

    QLocale te("te_IN");
    QCOMPARE(te.toString(123), QLatin1String("123"));

    QLocale hi("hi_IN");
    QCOMPARE(hi.toString(123), QLatin1String("123"));

    QLocale gu("gu_IN");
    QCOMPARE(gu.toString(123), QLatin1String("123"));

    QLocale kn("kn_IN");
    QCOMPARE(kn.toString(123), QLatin1String("123"));

    QLocale pa("pa_IN");
    QCOMPARE(pa.toString(123), QLatin1String("123"));

    QLocale ne("ne_IN");
    QCOMPARE(ne.toString(123), QString::fromUtf8("१२३"));

    QLocale mr("mr_IN");
    QCOMPARE(mr.toString(123), QString::fromUtf8("१२३"));

    QLocale ml("ml_IN");
    QCOMPARE(ml.toString(123), QLatin1String("123"));

    QLocale kok("kok_IN");
    QCOMPARE(kok.toString(123), QLatin1String("123"));
}

void tst_QLocale::ampm()
{
    QLocale c(QLocale::C);
    QCOMPARE(c.amText(), QLatin1String("AM"));
    QCOMPARE(c.pmText(), QLatin1String("PM"));

    QLocale de("de_DE");
    QCOMPARE(de.amText(), QLatin1String("AM"));
    QCOMPARE(de.pmText(), QLatin1String("PM"));

    QLocale sv("sv_SE");
    QCOMPARE(sv.amText(), QString::fromUtf8("förmiddag"));
    QCOMPARE(sv.pmText(), QString::fromUtf8("eftermiddag"));

    QLocale nn("nl_NL");
    QCOMPARE(nn.amText(), QLatin1String("a.m."));
    QCOMPARE(nn.pmText(), QLatin1String("p.m."));

    QLocale ua("uk_UA");
    QCOMPARE(ua.amText(), QString::fromUtf8("\320\264\320\277"));
    QCOMPARE(ua.pmText(), QString::fromUtf8("\320\277\320\277"));

    QLocale tr("tr_TR");
    QCOMPARE(tr.amText(), QString::fromUtf8("\303\226\303\226"));
    QCOMPARE(tr.pmText(), QString::fromUtf8("\303\226\123"));

    QLocale id("id_ID");
    QCOMPARE(id.amText(), QLatin1String("AM"));
    QCOMPARE(id.pmText(), QLatin1String("PM"));

    QLocale ta("ta_LK");
    QCOMPARE(ta.amText(), QString::fromUtf8("முற்பகல்"));
    QCOMPARE(ta.pmText(), QString::fromUtf8("பிற்பகல்"));
}

void tst_QLocale::dateFormat()
{
    const QLocale c(QLocale::C);
    // check that the NarrowFormat is the same as ShortFormat.
    QCOMPARE(c.dateFormat(QLocale::NarrowFormat), c.dateFormat(QLocale::ShortFormat));

    const QLocale nn("nn_NO");
    QCOMPARE(nn.dateFormat(QLocale::NarrowFormat), QLatin1String("dd.MM.yyyy"));
    QCOMPARE(nn.dateFormat(QLocale::ShortFormat), QLatin1String("dd.MM.yyyy"));
    QCOMPARE(nn.dateFormat(QLocale::LongFormat), QLatin1String("d. MMMM yyyy"));

    const QLocale ca("en_CA");
    QCOMPARE(ca.dateFormat(QLocale::ShortFormat), QLatin1String("yyyy-MM-dd"));
    QCOMPARE(ca.dateFormat(QLocale::LongFormat), QLatin1String("MMMM d, yyyy"));

    const QLocale ja("ja_JP");
    QCOMPARE(ja.dateFormat(QLocale::ShortFormat), QLatin1String("yyyy/MM/dd"));
}

void tst_QLocale::timeFormat()
{
    const QLocale c(QLocale::C);
    // check that the NarrowFormat is the same as ShortFormat.
    QCOMPARE(c.timeFormat(QLocale::NarrowFormat), c.timeFormat(QLocale::ShortFormat));

    const QLocale nn("nn_NO");
    QCOMPARE(nn.timeFormat(QLocale::NarrowFormat), QLatin1String("HH:mm"));
    QCOMPARE(nn.timeFormat(QLocale::ShortFormat), QLatin1String("HH:mm"));
    QCOMPARE(nn.timeFormat(QLocale::LongFormat), QLatin1String("HH:mm:ss Z"));

    const QLocale en("en_FJ");
    QCOMPARE(en.timeFormat(QLocale::ShortFormat), QLatin1String("h:mm AP"));
    QCOMPARE(en.timeFormat(QLocale::LongFormat), QLatin1String("h:mm:ss AP Z"));

    const QLocale cat("ca_ES");
    QCOMPARE(cat.timeFormat(QLocale::ShortFormat), QLatin1String("H:mm"));
    QCOMPARE(cat.timeFormat(QLocale::LongFormat), QLatin1String("H:mm:ss Z"));
}

void tst_QLocale::dateTimeFormat()
{
    const QLocale c(QLocale::C);
    // check that the NarrowFormat is the same as ShortFormat.
    QCOMPARE(c.dateTimeFormat(QLocale::NarrowFormat), c.dateTimeFormat(QLocale::ShortFormat));

    const QLocale nn("nn_NO");
    QCOMPARE(nn.dateTimeFormat(QLocale::NarrowFormat), QLatin1String("dd.MM.yyyy HH:mm"));
    QCOMPARE(nn.dateTimeFormat(QLocale::ShortFormat), QLatin1String("dd.MM.yyyy HH:mm"));
    QCOMPARE(nn.dateTimeFormat(QLocale::LongFormat), QLatin1String("d. MMMM yyyy HH:mm:ss Z"));
}

void tst_QLocale::monthName()
{
    const QLocale c(QLocale::C);
    QCOMPARE(c.monthName(0, QLocale::ShortFormat), QString());
    QCOMPARE(c.monthName(0, QLocale::LongFormat), QString());
    QCOMPARE(c.monthName(0, QLocale::NarrowFormat), QString());
    QCOMPARE(c.monthName(13, QLocale::ShortFormat), QString());
    QCOMPARE(c.monthName(13, QLocale::LongFormat), QString());
    QCOMPARE(c.monthName(13, QLocale::NarrowFormat), QString());

    QCOMPARE(c.monthName(1, QLocale::LongFormat), QLatin1String("January"));
    QCOMPARE(c.monthName(1, QLocale::ShortFormat), QLatin1String("Jan"));
    QCOMPARE(c.monthName(1, QLocale::NarrowFormat), QLatin1String("1"));

    const QLocale de("de_DE");
    QCOMPARE(de.monthName(12, QLocale::LongFormat), QLatin1String("Dezember"));
    QCOMPARE(de.monthName(12, QLocale::ShortFormat), QLatin1String("Dez."));
    // 'de' locale doesn't have narrow month name
    QCOMPARE(de.monthName(12, QLocale::NarrowFormat), QLatin1String("D"));

    QLocale ru("ru_RU");
    QCOMPARE(ru.monthName(1, QLocale::LongFormat), QString::fromUtf8("января"));
    QCOMPARE(ru.monthName(1, QLocale::ShortFormat), QString::fromUtf8("янв."));
    QCOMPARE(ru.monthName(1, QLocale::NarrowFormat), QString::fromUtf8("Я"));

    // check that our CLDR scripts handle surrogate pairs correctly
    QLocale dsrt("en-Dsrt-US");
    QCOMPARE(dsrt.monthName(1, QLocale::LongFormat), QString::fromUtf8("M01"));

    QLocale ir("ga_IE");
    QCOMPARE(ir.monthName(1, QLocale::ShortFormat), QLatin1String("Ean"));
    QCOMPARE(ir.monthName(12, QLocale::ShortFormat), QLatin1String("Noll"));

    QLocale cz("cs_CZ");
    QCOMPARE(cz.monthName(1, QLocale::ShortFormat), QLatin1String("led"));
    QCOMPARE(cz.monthName(12, QLocale::ShortFormat), QLatin1String("pro"));
}

void tst_QLocale::standaloneMonthName()
{
    const QLocale c(QLocale::C);
    QCOMPARE(c.monthName(0, QLocale::ShortFormat), QString());
    QCOMPARE(c.monthName(0, QLocale::LongFormat), QString());
    QCOMPARE(c.monthName(0, QLocale::NarrowFormat), QString());
    QCOMPARE(c.monthName(13, QLocale::ShortFormat), QString());
    QCOMPARE(c.monthName(13, QLocale::LongFormat), QString());
    QCOMPARE(c.monthName(13, QLocale::NarrowFormat), QString());

    QCOMPARE(c.standaloneMonthName(1, QLocale::LongFormat), QLatin1String("January"));
    QCOMPARE(c.standaloneMonthName(1, QLocale::ShortFormat), QLatin1String("Jan"));

    const QLocale de("de_DE");
    // For de_DE locale Unicode CLDR database doesn't contain standalone long months
    // so just checking if the return value is the same as in monthName().
    QCOMPARE(de.standaloneMonthName(12, QLocale::LongFormat), QLatin1String("Dezember"));
    QCOMPARE(de.standaloneMonthName(12, QLocale::LongFormat), de.monthName(12, QLocale::LongFormat));
    QCOMPARE(de.standaloneMonthName(12, QLocale::ShortFormat), QLatin1String("Dez"));
    QCOMPARE(de.standaloneMonthName(12, QLocale::NarrowFormat), QLatin1String("D"));

    QLocale ru("ru_RU");
    QCOMPARE(ru.standaloneMonthName(1, QLocale::LongFormat), QString::fromUtf8("январь"));
    QCOMPARE(ru.standaloneMonthName(1, QLocale::ShortFormat), QString::fromUtf8("янв."));
    QCOMPARE(ru.standaloneMonthName(1, QLocale::NarrowFormat), QString::fromUtf8("Я"));
}

void tst_QLocale::currency()
{
    const QLocale c(QLocale::C);
    QCOMPARE(c.toCurrencyString(qulonglong(1234)), QString("1,234"));
    QCOMPARE(c.toCurrencyString(qlonglong(-1234)), QString("-1,234"));
    QCOMPARE(c.toCurrencyString(double(1234.56)), QString("1,234.56"));
    QCOMPARE(c.toCurrencyString(double(-1234.56)), QString("-1,234.56"));

    const QLocale en_US("en_US");
    QCOMPARE(en_US.toCurrencyString(qulonglong(1234)), QString("$1,234"));
    QCOMPARE(en_US.toCurrencyString(qlonglong(-1234)), QString("$-1,234"));
    QCOMPARE(en_US.toCurrencyString(double(1234.56)), QString("$1,234.56"));
    QCOMPARE(en_US.toCurrencyString(double(-1234.56)), QString("$-1,234.56"));

    const QLocale ru_RU("ru_RU");
    QCOMPARE(ru_RU.toCurrencyString(qulonglong(1234)), QString::fromUtf8("1 234 ₽"));
    QCOMPARE(ru_RU.toCurrencyString(qlonglong(-1234)), QString::fromUtf8("-1 234 ₽"));
    QCOMPARE(ru_RU.toCurrencyString(double(1234.56)), QString::fromUtf8("1 234,56 ₽"));
    QCOMPARE(ru_RU.toCurrencyString(double(-1234.56)), QString::fromUtf8("-1 234,56 ₽"));

    const QLocale de_DE("de_DE");
    QCOMPARE(de_DE.toCurrencyString(qulonglong(1234)), QString::fromUtf8("1.234 €"));
    QCOMPARE(de_DE.toCurrencyString(qulonglong(1234), QLatin1String("BAZ")), QString::fromUtf8("1.234\xc2\xa0" "BAZ"));
    QCOMPARE(de_DE.toCurrencyString(qlonglong(-1234)), QString::fromUtf8("-1.234 €"));
    QCOMPARE(de_DE.toCurrencyString(qlonglong(-1234), QLatin1String("BAZ")), QString::fromUtf8("-1.234\xc2\xa0" "BAZ"));
    QCOMPARE(de_DE.toCurrencyString(double(1234.56)), QString::fromUtf8("1.234,56 €"));
    QCOMPARE(de_DE.toCurrencyString(double(-1234.56)), QString::fromUtf8("-1.234,56 €"));
    QCOMPARE(de_DE.toCurrencyString(double(-1234.56), QLatin1String("BAZ")), QString::fromUtf8("-1.234,56\xc2\xa0" "BAZ"));

    const QLocale system = QLocale::system();
    QVERIFY(system.toCurrencyString(1, QLatin1String("FOO")).contains(QLatin1String("FOO")));
}

void tst_QLocale::quoteString()
{
    const QString someText("text");
    const QLocale c(QLocale::C);
    QCOMPARE(c.quoteString(someText), QString::fromUtf8("\x22" "text" "\x22"));
    QCOMPARE(c.quoteString(someText, QLocale::AlternateQuotation), QString::fromUtf8("\x27" "text" "\x27"));

    const QLocale de_CH("de_CH");
    QCOMPARE(de_CH.quoteString(someText), QString::fromUtf8("„text“"));
    QCOMPARE(de_CH.quoteString(someText, QLocale::AlternateQuotation), QString::fromUtf8("‚text‘"));

}

void tst_QLocale::uiLanguages()
{
    const QLocale c(QLocale::C);
    QCOMPARE(c.uiLanguages().size(), 1);
    QCOMPARE(c.uiLanguages().at(0), QLatin1String("C"));

    const QLocale en_US("en_US");
    QCOMPARE(en_US.uiLanguages().size(), 1);
    QCOMPARE(en_US.uiLanguages().at(0), QLatin1String("en-Latn-US"));

    const QLocale ru_RU("ru_RU");
    QCOMPARE(ru_RU.uiLanguages().size(), 1);
    QCOMPARE(ru_RU.uiLanguages().at(0), QLatin1String("ru-Cyrl-RU"));
}

void tst_QLocale::weekendDays()
{
    const QLocale c(QLocale::C);
    QList<Qt::DayOfWeek> days;
    days << Qt::Monday << Qt::Tuesday << Qt::Wednesday << Qt::Thursday << Qt::Friday;
    QCOMPARE(c.weekdays(), days);
}

void tst_QLocale::listPatterns()
{
    QStringList sl1;
    QStringList sl2;
    sl2 << "aaa";
    QStringList sl3;
    sl3 << "aaa" << "bbb";
    QStringList sl4;
    sl4 << "aaa" << "bbb" << "ccc";
    QStringList sl5;
    sl5 << "aaa" << "bbb" << "ccc" << "ddd";

    const QLocale c(QLocale::C);
    QCOMPARE(c.createSeparatedList(sl1), QString(""));
    QCOMPARE(c.createSeparatedList(sl2), QString("aaa"));
    QCOMPARE(c.createSeparatedList(sl3), QString("aaa, bbb"));
    QCOMPARE(c.createSeparatedList(sl4), QString("aaa, bbb, ccc"));
    QCOMPARE(c.createSeparatedList(sl5), QString("aaa, bbb, ccc, ddd"));

    const QLocale en_US("en_US");
    QCOMPARE(en_US.createSeparatedList(sl1), QString(""));
    QCOMPARE(en_US.createSeparatedList(sl2), QString("aaa"));
    QCOMPARE(en_US.createSeparatedList(sl3), QString("aaa and bbb"));
    QCOMPARE(en_US.createSeparatedList(sl4), QString("aaa, bbb, and ccc"));
    QCOMPARE(en_US.createSeparatedList(sl5), QString("aaa, bbb, ccc, and ddd"));

    const QLocale zh_CN("zh_CN");
    QCOMPARE(zh_CN.createSeparatedList(sl1), QString(""));
    QCOMPARE(zh_CN.createSeparatedList(sl2), QString("aaa"));
    QCOMPARE(zh_CN.createSeparatedList(sl3), QString::fromUtf8("aaa" "\xe5\x92\x8c" "bbb"));
    QCOMPARE(zh_CN.createSeparatedList(sl4), QString::fromUtf8("aaa" "\xe3\x80\x81" "bbb" "\xe5\x92\x8c" "ccc"));
    QCOMPARE(zh_CN.createSeparatedList(sl5), QString::fromUtf8("aaa" "\xe3\x80\x81" "bbb" "\xe3\x80\x81" "ccc" "\xe5\x92\x8c" "ddd"));
}

QTEST_MAIN(tst_QLocale)

#include "moc_tst_qlocale.cpp"
