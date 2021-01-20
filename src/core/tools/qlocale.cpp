/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#include "qplatformdefs.h"
#include "qdatastream.h"
#include "qstring.h"
#include "qlocale.h"
#include "qlocale_p.h"
#include "qlocale_tools_p.h"
#include "qdatetime_p.h"
#include "qnamespace.h"
#include "qdatetime.h"
#include "qstringlist.h"
#include "qvariant.h"
#include "qnumeric.h"
#include "qlocale_data_p.h"
#include "qdebug.h"
#include "qcorecommon_p.h"

#include <errno.h>
#include <unicode/ulocdata.h>

// #define QLOCALE_DEBUG
#if defined (QLOCALE_DEBUG) && !defined(QT_NO_DEBUG_STREAM)
#  define QLOCALEDEBUG qDebug()
#else
#  define QLOCALEDEBUG if (false) qDebug()
#endif

QT_BEGIN_NAMESPACE

static const qint16 systemLocaleIndex = localeTblSize + 1;
#ifndef QT_NO_SYSTEMLOCALE
static QSystemLocale *_systemLocale = Q_NULLPTR;
class QSystemLocaleSingleton: public QSystemLocale
{
public:
    QSystemLocaleSingleton() : QSystemLocale(true) {}
};
Q_GLOBAL_STATIC(QSystemLocaleSingleton, QSystemLocale_globalSystemLocale)
static QLocalePrivate *system_lp = Q_NULLPTR;
#endif

/******************************************************************************
** Helpers for accessing Qt locale database
*/
QLocale::Language QLocalePrivate::codeToLanguage(const QString &code)
{
    QString lower = code.toLower();

    for (qint16 i = 0; i < languageAliasTblSize; i++) {
        if (languageAliasTbl[i].original == lower) {
            lower = languageAliasTbl[i].substitute;
            break;
        }
    }

    for (qint16 i = 0; i < languageTblSize; i++) {
        if (QLatin1String(languageTbl[i].code) == lower)
            return languageTbl[i].language;
    }

    return QLocale::C;
}

QLocale::Script QLocalePrivate::codeToScript(const QString &code)
{
    // script is titlecased in our data
    QString title = code.toLower();
    if (title.size() > 0) {
        title[0] = code.at(0).toUpper();
    }

    for (qint16 i = 0; i < scriptAliasTblSize; i++) {
        if (scriptAliasTbl[i].original == title) {
            title = scriptAliasTbl[i].substitute;
            break;
        }
    }

    for (qint16 i = 0; i < scriptTblSize; i++) {
        if (QLatin1String(scriptTbl[i].code) == title)
            return scriptTbl[i].script;
    }
    return QLocale::AnyScript;
}

QLocale::Country QLocalePrivate::codeToCountry(const QString &code)
{
    QString upper = code.toUpper();

    for (qint16 i = 0; i < countryAliasTblSize; i++) {
        if (countryAliasTbl[i].original == upper) {
            upper = countryAliasTbl[i].substitute;
            break;
        }
    }

    for (qint16 i = 0; i < countryTblSize; i++) {
        if (QLatin1String(countryTbl[i].code) == upper)
            return countryTbl[i].country;
    }

    return QLocale::AnyCountry;
}

QString QLocalePrivate::bcp47Name() const
{
    if (m_language == QLocale::AnyLanguage)
        return QString();
    if (m_language == QLocale::C)
        return QLatin1String("C");

    const char *lang = languageTbl[m_language].code;
    const char *script = scriptTbl[m_script].code;
    const char *country = countryTbl[m_country].code;
    const int langlen = qstrlen(lang);
    const int scriptlen = qstrlen(script);
    const int countrylen = qstrlen(country);
    const int totallen = langlen + scriptlen + countrylen + (script ? 1 : 0) + (country ? 1 : 0);

    QString name(totallen, Qt::Uninitialized);
    QChar *uc = name.data();
    int datapos = 0;
    for (int i = 0; i < langlen; i++) {
        uc[datapos] = ushort(lang[i]);
        datapos++;
    }
    if (script) {
        uc[datapos++] = QLatin1Char('-');
        for (int i = 0; i < scriptlen; i++) {
            uc[datapos] = ushort(script[i]);
            datapos++;
        }
    }
    if (country) {
        uc[datapos++] = QLatin1Char('-');
        for (int i = 0; i < countrylen; i++) {
            uc[datapos] = ushort(country[i]);
            datapos++;
        }
    }

    return name;
}

static const QLocalePrivate *systemPrivate()
{
#ifndef QT_NO_SYSTEMLOCALE
    // copy over the information from the fallback locale and modify
    if (!system_lp || system_lp->m_language == QLocale::AnyLanguage)
        QLocalePrivate::updateSystemPrivate();

    return system_lp;
#else
    return &localeTbl[0];
#endif
}

static const QLocalePrivate *default_lp = 0;
static QLocale::NumberOptions default_number_options = 0;

static const QLocalePrivate *defaultPrivate()
{
    if (!default_lp)
        default_lp = systemPrivate();
    return default_lp;
}

const QLocalePrivate *QLocalePrivate::findLocale(QLocale::Language language, QLocale::Script script, QLocale::Country country)
{
    // check likely substitutes first
    for (qint16 i = 0; i < subtagAliasTblSize; i++) {
        if (subtagAliasTbl[i].fromlanguage == language
            && subtagAliasTbl[i].fromscript == script
            && subtagAliasTbl[i].fromcountry == country) {
            QLOCALEDEBUG << "from" << language << script << country;
            language = subtagAliasTbl[i].tolanguage;
            script = subtagAliasTbl[i].toscript;
            country = subtagAliasTbl[i].tocountry;
            QLOCALEDEBUG << "to" << language << script << country;
            break;
        }
    }

    for (qint16 i = 0; i < localeTblSize; i++) {
        if ((language == QLocale::AnyLanguage || localeTbl[i].m_language == language)
            && (script == QLocale::AnyScript || localeTbl[i].m_script == QLocale::AnyScript || localeTbl[i].m_script == script)
            && (country == QLocale::AnyCountry || localeTbl[i].m_country == QLocale::AnyCountry || localeTbl[i].m_country == country)) {
            QLOCALEDEBUG << "exact match for" << language << script << country;
            return &localeTbl[i];
        }
    }

    // for compatibility match invalid cases, e.g. en_zz, to the first match for that language
    for (qint16 i = 0; i < localeTblSize; i++) {
        if (localeTbl[i].m_language == language) {
            QLOCALEDEBUG << "greedy match for" << language << script << country;
            return &localeTbl[i];
        }
    }

    return &localeTbl[0];
}

bool qt_splitLocaleName(const QString &name, QString &lang, QString &script, QString &cntry)
{
    const QByteArray latinname = name.toLatin1();
    char getbuffer[20];
    UErrorCode error = U_ZERO_ERROR;
    const int langresult = uloc_getLanguage(latinname.constData(), getbuffer, sizeof(getbuffer), &error);
    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("qt_splitLocaleName: uloc_getLanguage(%s) failed %s",
            latinname.constData(), u_errorName(error));
        return false;
    }
    lang = QString::fromLatin1(getbuffer, langresult);

    error = U_ZERO_ERROR;
    memset(getbuffer, 0, sizeof(getbuffer));
    const int scriptresult = uloc_getScript(latinname.constData(), getbuffer, sizeof(getbuffer), &error);
    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("qt_splitLocaleName: uloc_getScript(%s) failed %s",
            latinname.constData(), u_errorName(error));
        return false;
    }
    script = QString::fromLatin1(getbuffer, scriptresult);

    error = U_ZERO_ERROR;
    memset(getbuffer, 0, sizeof(getbuffer));
    const int countryresult = uloc_getCountry(latinname.constData(), getbuffer, sizeof(getbuffer), &error);
    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("qt_splitLocaleName: uloc_getCountry(%s) failed %s",
            latinname.constData(), u_errorName(error));
        return false;
    }
    cntry = QString::fromLatin1(getbuffer, countryresult);

    return true;
}

void QLocalePrivate::getLangAndCountry(const QString &name, QLocale::Language &lang,
                                       QLocale::Script &script, QLocale::Country &cntry)
{
    lang = QLocale::C;
    script = QLocale::AnyScript;
    cntry = QLocale::AnyCountry;

    QString lang_code;
    QString script_code;
    QString cntry_code;
    if (Q_UNLIKELY(!qt_splitLocaleName(name, lang_code, script_code, cntry_code)))
        return;

    lang = QLocalePrivate::codeToLanguage(lang_code);
    if (lang == QLocale::C)
        return;
    script = QLocalePrivate::codeToScript(script_code);
    cntry = QLocalePrivate::codeToCountry(cntry_code);
}

static const QLocalePrivate *findLocale(const QString &name)
{
    QLocale::Language lang;
    QLocale::Script script;
    QLocale::Country cntry;
    QLocalePrivate::getLangAndCountry(name, lang, script, cntry);

    return QLocalePrivate::findLocale(lang, script, cntry);
}

static QString qt_readEscapedFormatString(const QString &format, int *idx)
{
    int &i = *idx;

    Q_ASSERT(format.at(i) == QLatin1Char('\''));
    ++i;
    if (i == format.size())
        return QString();
    if (format.at(i).unicode() == '\'') { // "''" outside of a quoted stirng
        ++i;
        return QLatin1String("'");
    }

    QString result;

    while (i < format.size()) {
        if (format.at(i).unicode() == '\'') {
            if (i + 1 < format.size() && format.at(i + 1).unicode() == '\'') {
                // "''" inside of a quoted string
                result.append(QLatin1Char('\''));
                i += 2;
            } else {
                break;
            }
        } else {
            result.append(format.at(i++));
        }
    }
    if (i < format.size())
        ++i;

    return result;
}

static int qt_repeatCount(const QString &s, int i)
{
    int j = i + 1;
    while (j < s.size() && s.at(j) == s.at(i))
        ++j;
    return j - i;
}

#ifndef QT_NO_SYSTEMLOCALE


/******************************************************************************
** Default system locale behavior
*/

/*!
  Constructs a QSystemLocale object. The constructor will automatically
  install this object as the system locale and remove any earlier installed
  system locales.
*/
QSystemLocale::QSystemLocale()
{
    delete _systemLocale;
    _systemLocale = this;

    if (system_lp)
        system_lp->m_language = QLocale::AnyLanguage;
}

/*! \internal */
QSystemLocale::QSystemLocale(bool)
{ }

/*!
  Deletes the object.
*/
QSystemLocale::~QSystemLocale()
{
    if (_systemLocale == this) {
        _systemLocale = Q_NULLPTR;

        if (system_lp)
            system_lp->m_language = QLocale::AnyLanguage;
    }
}

static const QSystemLocale *systemLocale()
{
    if (_systemLocale)
        return _systemLocale;
    return QSystemLocale_globalSystemLocale();
}

static int qFreeSystemLP() {
    if (system_lp)
        ::free(system_lp);
    return 0;
}
Q_DESTRUCTOR_FUNCTION(qFreeSystemLP);

void QLocalePrivate::updateSystemPrivate()
{
    const QSystemLocale *sys_locale = systemLocale();
    if (system_lp)
        ::free(system_lp);

    system_lp = static_cast<QLocalePrivate*>(::malloc(sizeof(QLocalePrivate)));
    ::memcpy(system_lp, sys_locale->fallbackLocale().d(), sizeof(QLocalePrivate));

    QVariant res = sys_locale->query(QSystemLocale::LanguageId, QVariant());
    if (!res.isNull()) {
        system_lp->m_language = QLocale::Language(res.toInt());
        system_lp->m_script = QLocale::AnyScript; // default for compatibility
    }
    res = sys_locale->query(QSystemLocale::CountryId, QVariant());
    if (!res.isNull()) {
        system_lp->m_country = QLocale::Country(res.toInt());
        system_lp->m_script = QLocale::AnyScript; // default for compatibility
    }
    res = sys_locale->query(QSystemLocale::ScriptId, QVariant());
    if (!res.isNull())
        system_lp->m_script = QLocale::Script(res.toInt());

    res = sys_locale->query(QSystemLocale::DecimalPoint, QVariant());
    if (!res.isNull())
        system_lp->m_decimal = res.toString().at(0).unicode();

    res = sys_locale->query(QSystemLocale::GroupSeparator, QVariant());
    if (!res.isNull())
        system_lp->m_group = res.toString().at(0).unicode();

    res = sys_locale->query(QSystemLocale::ListToSeparatedString, QVariant());
    if (!res.isNull())
        system_lp->m_list = res.toString().at(0).unicode();

    res = sys_locale->query(QSystemLocale::ZeroDigit, QVariant());
    if (!res.isNull())
        system_lp->m_zero = res.toString().at(0).unicode();

    res = sys_locale->query(QSystemLocale::PositiveSign, QVariant());
    if (!res.isNull())
        system_lp->m_plus = res.toString().at(0).unicode();

    res = sys_locale->query(QSystemLocale::NegativeSign, QVariant());
    if (!res.isNull())
        system_lp->m_minus = res.toString().at(0).unicode();

    // ### exponential cannot be queried

    if (!default_lp)
        qt_initLocale(system_lp->bcp47Name());

    // tell the object that the system locale has changed.
    sys_locale->query(QSystemLocale::LocaleChanged, QVariant());
}
#endif

static inline QString getLocaleListData(const char * const* data, int index)
{
    return QString::fromUtf8(data[index]);
}

static inline QString getLocaleData(const char *data)
{
    return QString::fromUtf8(data);
}


#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &ds, const QLocale &l)
{
    ds << l.name();
    return ds;
}

QDataStream &operator>>(QDataStream &ds, QLocale &l)
{
    QString s;
    ds >> s;
    l = QLocale(s);
    return ds;
}
#endif // QT_NO_DATASTREAM

static quint16 localePrivateIndex(const QLocalePrivate *p)
{
    if (Q_LIKELY(p)) {
#ifndef QT_NO_SYSTEMLOCALE
        if (p == system_lp) {
            return systemLocaleIndex;
        }
#endif
        for (qint16 i = 0; i < localeTblSize; i++) {
            if (p->m_language == localeTbl[i].m_language
                && p->m_country == localeTbl[i].m_country
                && p->m_script == localeTbl[i].m_script) {
                return i;
            }
        }
    }
    return 0;
}

/*!
    Constructs a QLocale object with the specified \a name,
    which has the format
    "language[_script][_country][.codeset][@modifier]" or "C", where:

    \list
    \i language is a lowercase, two-letter, ISO 639 language code,
    \i script is a titlecase, four-letter, ISO 15924 script code,
    \i country is an uppercase, two- or three-letter, ISO 3166 country code (also "419" as defined by United Nations),
    \i and codeset and modifier are ignored.
    \endlist

    The separator can be either underscore or a minus sign.

    If the string violates the locale format, or language is not
    a valid ISO 369 code, the "C" locale is used instead. If country
    is not present, or is not a valid ISO 3166 code, the most
    appropriate country is chosen for the specified language.

    The language, script and country codes are converted to their respective
    \c Language, \c Script and \c Country enums. After this conversion is
    performed the constructor behaves exactly like QLocale(Country, Script,
    Language).

    This constructor is much slower than QLocale(Country, Script, Language).

    \sa bcp47Name()
*/

QLocale::QLocale(const QString &name)
{
    p.numberOptions = 0;
    p.index = localePrivateIndex(findLocale(name));
}

/*!
    Constructs a QLocale object initialized with the default locale. If
    no default locale was set using setDefaultLocale(), this locale will
    be the same as the one returned by system().

    \sa setDefault()
*/

QLocale::QLocale()
{
    p.numberOptions = default_number_options;
    p.index = localePrivateIndex(defaultPrivate());
}

/*!
    Constructs a QLocale object with the specified \a language and \a
    country.

    \list
    \i If the language/country pair is found in the database, it is used.
    \i If the language is found but the country is not, or if the country
       is \c AnyCountry, the language is used with the most
       appropriate available country (for example, Germany for German),
    \i If neither the language nor the country are found, QLocale
       defaults to the default locale (see setDefault()).
    \endlist

    The language and country that are actually used can be queried
    using language() and country().

    \sa setDefault() language() country()
*/

QLocale::QLocale(Language language, Country country)
{
    const QLocalePrivate *dd = QLocalePrivate::findLocale(language, QLocale::AnyScript, country);

    // If not found, should default to system
    if (dd->m_language == QLocale::C && language != QLocale::C) {
        p.numberOptions = default_number_options;
        p.index = localePrivateIndex(defaultPrivate());
    } else {
        p.numberOptions = 0;
        p.index = localePrivateIndex(dd);
    }
}
\
/*!
    \since 4.8

    Constructs a QLocale object with the specified \a language, \a script and
    \a country.

    \list
    \i If the language/script/country is found in the database, it is used.
    \i If both \a script is AnyScript and \a country is AnyCountry, the
       language is used with the most appropriate available script and country
       (for example, Germany for German),
    \i If either \a script is AnyScript or \a country is AnyCountry, the
       language is used with the first locale that matches the given \a script
       and \a country.
    \i If neither the language nor the country are found, QLocale
       defaults to the default locale (see setDefault()).
    \endlist

    The language, script and country that are actually used can be queried
    using language(), script() and country().

    \sa setDefault() language() script() country()
*/

QLocale::QLocale(Language language, Script script, Country country)
{
    const QLocalePrivate *dd = QLocalePrivate::findLocale(language, script, country);

    // If not found, should default to system
    if (dd->m_language == QLocale::C && language != QLocale::C) {
        p.numberOptions = default_number_options;
        p.index = localePrivateIndex(defaultPrivate());
    } else {
        p.numberOptions = 0;
        p.index = localePrivateIndex(dd);
    }
}

/*!
    Constructs a QLocale object as a copy of \a other.
*/

QLocale::QLocale(const QLocale &other)
{
    p = other.p;
}

const QLocalePrivate *QLocale::d() const
{
#ifndef QT_NO_SYSTEMLOCALE
    Q_ASSERT(p.index <= systemLocaleIndex);
    if (p.index == systemLocaleIndex)
        return system_lp;
#else
    Q_ASSERT(p.index < systemLocaleIndex);
#endif

    return &localeTbl[p.index];
}

/*!
    Assigns \a other to this QLocale object and returns a reference
    to this QLocale object.
*/

QLocale &QLocale::operator=(const QLocale &other)
{
    p = other.p;
    return *this;
}

/*!
    \since 4.2

    Sets the \a options related to number conversions for this
    QLocale instance.
*/
void QLocale::setNumberOptions(NumberOptions options)
{
    p.numberOptions = options;
}

/*!
    \since 4.2

    Returns the options related to number conversions for this
    QLocale instance.

    By default, no options are set for the standard locales.
*/
QLocale::NumberOptions QLocale::numberOptions() const
{
    return p.numberOptions;
}

/*!
    \since 4.8

    Returns \a str quoted according to the current locale using the given
    quotation \a style.
*/
QString QLocale::quoteString(const QString &str, QuotationStyle style) const
{
    return quoteString(&str, style);
}

/*!
    \since 4.8

    \overload
*/
QString QLocale::quoteString(const QStringRef &str, QuotationStyle style) const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res;
        if (style == QLocale::AlternateQuotation)
            res = systemLocale()->query(QSystemLocale::StringToAlternateQuotation, QVariant::fromValue(str));
        if (res.isNull() || style == QLocale::StandardQuotation)
            res = systemLocale()->query(QSystemLocale::StringToStandardQuotation, QVariant::fromValue(str));
        if (!res.isNull())
            return res.toString();
    }
#endif

    if (style == QLocale::StandardQuotation)
        return QChar(d()->m_quotation_start) + str.toString() + QChar(d()->m_quotation_end);
    else
        return QChar(d()->m_alternate_quotation_start) + str.toString() + QChar(d()->m_alternate_quotation_end);
}

/*!
    \since 4.8

    Returns a string that represents a join of a given \a list of strings with
    a separator defined by the locale.
*/
QString QLocale::createSeparatedList(const QStringList &list) const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res;
        res = systemLocale()->query(QSystemLocale::ListToSeparatedString, QVariant::fromValue(list));

        if (!res.isNull())
            return res.toString();
    }
#endif

    const int size = list.size();
    if (size == 1) {
        return list.at(0);
    } else if (size == 2) {
        QString format = getLocaleData(d()->m_list_pattern_part_two);
        return format.arg(list.at(0), list.at(1));
    } else if (size > 2) {
        QString formatStart = getLocaleData(d()->m_list_pattern_part_start);
        QString formatMid = getLocaleData(d()->m_list_pattern_part_mid);
        QString formatEnd = getLocaleData(d()->m_list_pattern_part_end);
        QString result = formatStart.arg(list.at(0), list.at(1));
        for (int i = 2; i < size - 1; ++i)
            result = formatMid.arg(result, list.at(i));
        result = formatEnd.arg(result, list.at(size - 1));
        return result;
    }

    return QString();
}

/*!
    \nonreentrant

    Sets the global default locale to \a locale. These
    values are used when a QLocale object is constructed with
    no arguments. If this function is not called, the system's
    locale is used.

    \warning In a multithreaded application, the default locale
    should be set at application startup, before any non-GUI threads
    are created.

    \sa system() c()
*/

void QLocale::setDefault(const QLocale &locale)
{
    default_lp = locale.d();
    default_number_options = locale.numberOptions();

    qt_initLocale(locale.bcp47Name());
}

/*!
    Returns the language of this locale.

    \sa script(), country(), languageToString(), bcp47Name()
*/
QLocale::Language QLocale::language() const
{
    return d()->m_language;
}

/*!
    \since 4.8

    Returns the script of this locale.

    \sa language(), country(), languageToString(), scriptToString(), bcp47Name()
*/
QLocale::Script QLocale::script() const
{
    return d()->m_script;
}

/*!
    Returns the country of this locale.

    \sa language(), script(), countryToString(), bcp47Name()
*/
QLocale::Country QLocale::country() const
{
    return d()->m_country;
}

/*!
    Returns the language and country of this locale as a
    string of the form "language_country", where
    language is a lowercase, two-letter ISO 639 language code,
    and country is an uppercase, two- or three-letter ISO 3166 country code.

    Note that even if QLocale object was constructed with an explicit script,
    name() will not contain it for compatibility reasons. Use bcp47Name() instead
    if you need a full locale name.

    \sa QLocale(), language(), script(), country(), bcp47Name()
*/

QString QLocale::name() const
{
    const QLocalePrivate *dd = d();

    if (dd->m_language == QLocale::AnyLanguage)
        return QString();
    if (dd->m_language == QLocale::C)
        return QLatin1String("C");

    const char *lang = languageTbl[dd->m_language].code;
    if (dd->m_country != AnyCountry) {
        const char *country = countryTbl[dd->m_country].code;
        return QLatin1String(lang) + QLatin1Char('_') + QLatin1String(country);
    }
    return QLatin1String(lang);
}

/*!
    \since 4.8

    Returns the dash-separated language, script and country (and possibly other BCP47 fields)
    of this locale as a string.

    Unlike the uiLanguages() the returned value of the bcp47Name() represents
    the locale name of the QLocale data but not the language the user-interface
    should be in.

    This function tries to conform the locale name to BCP47.

    \sa language(), country(), script(), uiLanguages()
*/
QString QLocale::bcp47Name() const
{
    return d()->bcp47Name();
}

/*!
    Returns a QString containing the name of \a language.

    \sa countryToString(), scriptToString(), bcp47Name()
*/

QString QLocale::languageToString(Language language)
{
    return QString::fromUtf8(languageTbl[language].name);
}

/*!
    Returns a QString containing the name of \a country.

    \sa languageToString(), scriptToString(), country(), bcp47Name()
*/

QString QLocale::countryToString(Country country)
{
    return QString::fromUtf8(countryTbl[country].name);
}

/*!
    \since 4.8

    Returns a QString containing the name of \a script.

    \sa languageToString(), countryToString(), script(), bcp47Name()
*/
QString QLocale::scriptToString(QLocale::Script script)
{
    return QString::fromUtf8(scriptTbl[script].name);
}

/*!
    Returns the short int represented by the localized string \a s,
    using base \a base. If \a base is 0 the base is determined
    automatically using the following rules: If the string begins with
    "0x", it is assumed to be hexadecimal; if it begins with "0", it
    is assumed to be octal; otherwise it is assumed to be decimal.

    If the conversion fails the function returns 0.

    If \a ok is not 0, failure is reported by setting *ok to false, and
    success by setting *ok to true.

    This function ignores leading and trailing whitespace.

    \sa toUShort(), toString()
*/

short QLocale::toShort(const QString &s, bool *ok, int base) const
{
    qlonglong i = toLongLong(s, ok, base);
    if (i < SHRT_MIN || i > SHRT_MAX) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }
    return short(i);
}

/*!
    Returns the unsigned short int represented by the localized string
    \a s, using base \a base. If \a base is 0 the base is determined
    automatically using the following rules: If the string begins with
    "0x", it is assumed to be hexadecimal; if it begins with "0", it
    is assumed to be octal; otherwise it is assumed to be decimal.

    If the conversion fails the function returns 0.

    If \a ok is not 0, failure is reported by setting *ok to false, and
    success by setting *ok to true.

    This function ignores leading and trailing whitespace.

    \sa toShort(), toString()
*/

ushort QLocale::toUShort(const QString &s, bool *ok, int base) const
{
    qulonglong i = toULongLong(s, ok, base);
    if (i > USHRT_MAX) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }
    return ushort(i);
}

/*!
    Returns the int represented by the localized string \a s, using
    base \a base. If \a base is 0 the base is determined automatically
    using the following rules: If the string begins with "0x", it is
    assumed to be hexadecimal; if it begins with "0", it is assumed to
    be octal; otherwise it is assumed to be decimal.

    If the conversion fails the function returns 0.

    If \a ok is not 0, failure is reported by setting *ok to false, and
    success by setting *ok to true.

    This function ignores leading and trailing whitespace.

    \sa toUInt(), toString()
*/

int QLocale::toInt(const QString &s, bool *ok, int base) const
{
    qlonglong i = toLongLong(s, ok, base);
    if (i < INT_MIN || i > INT_MAX) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }
    return int(i);
}

/*!
    Returns the unsigned int represented by the localized string \a s,
    using base \a base. If \a base is 0 the base is determined
    automatically using the following rules: If the string begins with
    "0x", it is assumed to be hexadecimal; if it begins with "0", it
    is assumed to be octal; otherwise it is assumed to be decimal.

    If the conversion fails the function returns 0.

    If \a ok is not 0, failure is reported by setting *ok to false, and
    success by setting *ok to true.

    This function ignores leading and trailing whitespace.

    \sa toInt(), toString()
*/

uint QLocale::toUInt(const QString &s, bool *ok, int base) const
{
    qulonglong i = toULongLong(s, ok, base);
    if (i > UINT_MAX) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }
    return uint(i);
}

/*!
    Returns the long long int represented by the localized string \a
    s, using base \a base. If \a base is 0 the base is determined
    automatically using the following rules: If the string begins with
    "0x", it is assumed to be hexadecimal; if it begins with "0", it
    is assumed to be octal; otherwise it is assumed to be decimal.

    If the conversion fails the function returns 0.

    If \a ok is not 0, failure is reported by setting *ok to false, and
    success by setting *ok to true.

    This function ignores leading and trailing whitespace.

    \sa toInt(), toULongLong(), toDouble(), toString()
*/


qlonglong QLocale::toLongLong(const QString &s, bool *ok, int base) const
{
    QLocalePrivate::GroupSeparatorMode mode
        = p.numberOptions & RejectGroupSeparator
            ? QLocalePrivate::FailOnGroupSeparators
            : QLocalePrivate::ParseGroupSeparators;

    return d()->stringToLongLong(s, base, ok, mode);
}

/*!
    Returns the unsigned long long int represented by the localized
    string \a s, using base \a base. If \a base is 0 the base is
    determined automatically using the following rules: If the string
    begins with "0x", it is assumed to be hexadecimal; if it begins
    with "0", it is assumed to be octal; otherwise it is assumed to be
    decimal.

    If the conversion fails the function returns 0.

    If \a ok is not 0, failure is reported by setting *ok to false, and
    success by setting *ok to true.

    This function ignores leading and trailing whitespace.

    \sa toLongLong(), toInt(), toDouble(), toString()
*/

qulonglong QLocale::toULongLong(const QString &s, bool *ok, int base) const
{
    QLocalePrivate::GroupSeparatorMode mode
        = p.numberOptions & RejectGroupSeparator
            ? QLocalePrivate::FailOnGroupSeparators
            : QLocalePrivate::ParseGroupSeparators;

    return d()->stringToUnsLongLong(s, base, ok, mode);
}

/*!
    Returns the float represented by the localized string \a s, or 0.0
    if the conversion failed.

    If \a ok is not 0, reports failure by setting
    *ok to false and success by setting *ok to true.

    This function ignores leading and trailing whitespace.

    \sa toDouble(), toInt(), toString()
*/

#define QT_MAX_FLOAT 3.4028234663852886e+38

float QLocale::toFloat(const QString &s, bool *ok) const
{
    bool myOk;
    double d = toDouble(s, &myOk);
    if (!myOk || d > QT_MAX_FLOAT || d < -QT_MAX_FLOAT) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0.0;
    }
    if (ok != Q_NULLPTR)
        *ok = true;
    return float(d);
}

/*!
    Returns the double represented by the localized string \a s, or
    0.0 if the conversion failed.

    If \a ok is not 0, reports failure by setting
    *ok to false and success by setting *ok to true.

    Unlike QString::toDouble(), this function does not fall back to
    the "C" locale if the string cannot be interpreted in this
    locale.

    \snippet doc/src/snippets/code/src_corelib_tools_qlocale.cpp 3

    Notice that the last conversion returns 1234.0, because '.' is the
    thousands group separator in the German locale.

    This function ignores leading and trailing whitespace.

    \sa toFloat(), toInt(), toString()
*/

double QLocale::toDouble(const QString &s, bool *ok) const
{
    QLocalePrivate::GroupSeparatorMode mode
        = p.numberOptions & RejectGroupSeparator
            ? QLocalePrivate::FailOnGroupSeparators
            : QLocalePrivate::ParseGroupSeparators;

    return d()->stringToDouble(s, ok, mode);
}

/*!
    Returns a localized string representation of \a i.

    \sa toLongLong()
*/

QString QLocale::toString(qlonglong i) const
{
    int flags = p.numberOptions & OmitGroupSeparator
                    ? 0
                    : QLocalePrivate::ThousandsGroup;

    return d()->longLongToString(i, -1, 10, -1, flags);
}

/*!
    \overload

    \sa toULongLong()
*/

QString QLocale::toString(qulonglong i) const
{
    int flags = p.numberOptions & OmitGroupSeparator
                    ? 0
                    : QLocalePrivate::ThousandsGroup;

    return d()->unsLongLongToString(i, -1, 10, -1, flags);
}

/*!
    Returns a localized string representation of the given \a date in the
    specified \a format.
    If \a format is an empty string, an empty string is returned.
*/

QString QLocale::toString(const QDate &date, const QString &format) const
{
    return d()->dateTimeToString(format, &date, 0, this);
}

/*!
    Returns a localized string representation of the given \a date according
    to the specified \a format.
*/

QString QLocale::toString(const QDate &date, FormatType format) const
{
    if (!date.isValid())
        return QString();

#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(format == LongFormat
                                             ? QSystemLocale::DateToStringLong : QSystemLocale::DateToStringShort,
                                             date);
        if (!res.isNull())
            return res.toString();
    }
#endif

    return toString(date, dateFormat(format));
}

static bool timeFormatContainsAP(const QString &format)
{
    int i = 0;
    while (i < format.size()) {
        if (format.at(i).unicode() == '\'') {
            qt_readEscapedFormatString(format, &i);
            continue;
        }

        if (format.at(i).toLower().unicode() == 'a')
            return true;

        ++i;
    }
    return false;
}

/*!
    Returns a localized string representation of the given \a time according
    to the specified \a format.
    If \a format is an empty string, an empty string is returned.
*/
QString QLocale::toString(const QTime &time, const QString &format) const
{
    return d()->dateTimeToString(format, 0, &time, this);
}

/*!
    \since 4.4

    Returns a localized string representation of the given \a dateTime according
    to the specified \a format.
    If \a format is an empty string, an empty string is returned.
*/

QString QLocale::toString(const QDateTime &dateTime, const QString &format) const
{
    const QDate dt = dateTime.date();
    const QTime tm = dateTime.time();
    return d()->dateTimeToString(format, &dt, &tm, this);
}

/*!
    \since 4.4

    Returns a localized string representation of the given \a dateTime according
    to the specified \a format.
*/

QString QLocale::toString(const QDateTime &dateTime, FormatType format) const
{
    if (!dateTime.isValid())
        return QString();

#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(format == LongFormat
                                             ? QSystemLocale::DateTimeToStringLong
                                             : QSystemLocale::DateTimeToStringShort,
                                             dateTime);
        if (!res.isNull())
            return res.toString();
    }
#endif

    return toString(dateTime, dateTimeFormat(format));
}


/*!
    Returns a localized string representation of the given \a time in the
    specified \a format.
*/

QString QLocale::toString(const QTime &time, FormatType format) const
{
    if (!time.isValid())
        return QString();

#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(format == LongFormat
                                             ? QSystemLocale::TimeToStringLong : QSystemLocale::TimeToStringShort,
                                             time);
        if (!res.isNull())
            return res.toString();
    }
#endif

    return toString(time, timeFormat(format));
}

/*!
    \since 4.1

    Returns the date format used for the current locale.

    If \a format is LongFormat the format will be a long version.
    Otherwise it uses a shorter version.

    \sa QDate::toString(), QDate::fromString()
*/

QString QLocale::dateFormat(FormatType format) const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(format == LongFormat
                                             ? QSystemLocale::DateFormatLong : QSystemLocale::DateFormatShort,
                                             QVariant());
        if (!res.isNull())
            return res.toString();
    }
#endif

    switch (format) {
        case LongFormat:
            return getLocaleData(d()->m_long_date_format);
        default:
            return getLocaleData(d()->m_short_date_format);
    }
    return QString();
}

/*!
    \since 4.1

    Returns the time format used for the current locale.

    If \a format is LongFormat the format will be a long version.
    Otherwise it uses a shorter version.

    \sa QTime::toString(), QTime::fromString()
*/

QString QLocale::timeFormat(FormatType format) const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(format == LongFormat
                                             ? QSystemLocale::TimeFormatLong : QSystemLocale::TimeFormatShort,
                                             QVariant());
        if (!res.isNull())
            return res.toString();
    }
#endif

    switch (format) {
        case LongFormat:
            return getLocaleData(d()->m_long_time_format);
        default:
            return getLocaleData(d()->m_short_time_format);
    }
    return QString();
}

/*!
    \since 4.4

    Returns the date time format used for the current locale.

    If \a format is ShortFormat the format will be a short version.
    Otherwise it uses a longer version.

    \sa QDateTime::toString(), QDateTime::fromString()
*/

QString QLocale::dateTimeFormat(FormatType format) const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(format == LongFormat
                                             ? QSystemLocale::DateTimeFormatLong
                                             : QSystemLocale::DateTimeFormatShort,
                                             QVariant());
        if (!res.isNull()) {
            return res.toString();
        }
    }
#endif
    return dateFormat(format) + QLatin1Char(' ') + timeFormat(format);
}

/*!
    \since 4.4

    Parses the time string given in \a string and returns the
    time. The format of the time string is chosen according to the
    \a format parameter (see timeFormat()).

    If the time could not be parsed, returns an invalid time.

    \sa timeFormat(), toDate(), toDateTime(), QTime::fromString()
*/
#ifndef QT_NO_DATESTRING
QTime QLocale::toTime(const QString &string, FormatType format) const
{
    return toTime(string, timeFormat(format));
}
#endif

/*!
    \since 4.4

    Parses the date string given in \a string and returns the
    date. The format of the date string is chosen according to the
    \a format parameter (see dateFormat()).

    If the date could not be parsed, returns an invalid date.

    \sa dateFormat(), toTime(), toDateTime(), QDate::fromString()
*/
#ifndef QT_NO_DATESTRING
QDate QLocale::toDate(const QString &string, FormatType format) const
{
    return toDate(string, dateFormat(format));
}
#endif

/*!
    \since 4.4

    Parses the date/time string given in \a string and returns the
    time. The format of the date/time string is chosen according to the
    \a format parameter (see dateTimeFormat()).

    If the string could not be parsed, returns an invalid QDateTime.

    \sa dateTimeFormat(), toTime(), toDate(), QDateTime::fromString()
*/

#ifndef QT_NO_DATESTRING
QDateTime QLocale::toDateTime(const QString &string, FormatType format) const
{
    return toDateTime(string, dateTimeFormat(format));
}
#endif

/*!
    \since 4.4

    Parses the time string given in \a string and returns the
    time. See QTime::fromString() for information on what is a valid
    format string.

    If the time could not be parsed, returns an invalid time.

    \sa timeFormat(), toDate(), toDateTime(), QTime::fromString()
*/
#ifndef QT_NO_DATESTRING
QTime QLocale::toTime(const QString &string, const QString &format) const
{
    QTime time;
#ifndef QT_BOOTSTRAPPED
    QDateTimeParser dt(QVariant::Time, QDateTimeParser::FromString);
    dt.defaultLocale = *this;
    if (Q_LIKELY(dt.parseFormat(format)))
        dt.fromString(string, 0, &time);
#else
    Q_UNUSED(string);
    Q_UNUSED(format);
#endif
    return time;
}
#endif

/*!
    \since 4.4

    Parses the date string given in \a string and returns the
    date. See QDate::fromString() for information on the expressions
    that can be used with this function.

    This function searches month names and the names of the days of
    the week in the current locale.

    If the date could not be parsed, returns an invalid date.

    \sa dateFormat(), toTime(), toDateTime(), QDate::fromString()
*/
#ifndef QT_NO_DATESTRING
QDate QLocale::toDate(const QString &string, const QString &format) const
{
    QDate date;
#ifndef QT_BOOTSTRAPPED
    QDateTimeParser dt(QVariant::Date, QDateTimeParser::FromString);
    dt.defaultLocale = *this;
    if (Q_LIKELY(dt.parseFormat(format)))
        dt.fromString(string, &date, 0);
#else
    Q_UNUSED(string);
    Q_UNUSED(format);
#endif
    return date;
}
#endif

/*!
    \since 4.4

    Parses the date/time string given in \a string and returns the
    time.  See QDateTime::fromString() for information on the expressions
    that can be used with this function.

    \note The month and day names used must be given in the user's local
    language.

    If the string could not be parsed, returns an invalid QDateTime.

    \sa dateTimeFormat(), toTime(), toDate(), QDateTime::fromString()
*/
#ifndef QT_NO_DATESTRING
QDateTime QLocale::toDateTime(const QString &string, const QString &format) const
{
    QTime time;
    QDate date;
#ifndef QT_BOOTSTRAPPED
    QDateTimeParser dt(QVariant::DateTime, QDateTimeParser::FromString);
    dt.defaultLocale = *this;
    if (Q_LIKELY(dt.parseFormat(format)))
        dt.fromString(string, &date, &time);
#else
    Q_UNUSED(string);
    Q_UNUSED(format);
#endif
    return QDateTime(date, time);
}
#endif


/*!
    \since 4.1

    Returns the decimal point character of this locale.
*/
QChar QLocale::decimalPoint() const
{
    return d()->decimal();
}

/*!
    \since 4.1

    Returns the group separator character of this locale.
*/
QChar QLocale::groupSeparator() const
{
    return d()->group();
}

/*!
    \since 4.1

    Returns the percent character of this locale.
*/
QChar QLocale::percent() const
{
    return d()->percent();
}

/*!
    \since 4.1

    Returns the zero digit character of this locale.
*/
QChar QLocale::zeroDigit() const
{
    return d()->zero();
}

/*!
    \since 4.1

    Returns the negative sign character of this locale.
*/
QChar QLocale::negativeSign() const
{
    return d()->minus();
}

/*!
    \since 4.5

    Returns the positive sign character of this locale.
*/
QChar QLocale::positiveSign() const
{
    return d()->plus();
}

/*!
    \since 4.1

    Returns the exponential character of this locale.
*/
QChar QLocale::exponential() const
{
    return d()->exponential();
}

/*!
    \overload

    \a f and \a prec have the same meaning as in QString::number(double, char, int).

    \sa toDouble()
*/

QString QLocale::toString(double i, char f, int prec) const
{
    QLocalePrivate::DoubleForm form = QLocalePrivate::DFDecimal;
    uint flags = 0;

    if (qIsUpper(f))
        flags = QLocalePrivate::CapitalEorX;
    f = qToLower(f);

    switch (f) {
        case 'f':
            form = QLocalePrivate::DFDecimal;
            break;
        case 'e':
            form = QLocalePrivate::DFExponent;
            break;
        case 'g':
            form = QLocalePrivate::DFSignificantDigits;
            break;
        default:
            break;
    }

    if (!(p.numberOptions & OmitGroupSeparator))
        flags |= QLocalePrivate::ThousandsGroup;
    return d()->doubleToString(i, prec, form, -1, flags);
}

/*!
    \fn QLocale QLocale::c()

    Returns a QLocale object initialized to the "C" locale.

    \sa system()
*/

/*!
    Returns a QLocale object initialized to the system locale.

    \sa c()
*/

QLocale QLocale::system()
{
    QLocale result(C);
    result.p.index = localePrivateIndex(systemPrivate());
    return result;
}


/*!
    \since 4.8

    Returns a list of valid locale objects that match the given \a language, \a
    script and \a country.

    Getting a list of all locales:
    QList<QLocale> allLocales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);
*/
QList<QLocale> QLocale::matchingLocales(QLocale::Language language,
                                        QLocale::Script script,
                                        QLocale::Country country)
{
    if (language == QLocale::C)
        return QList<QLocale>() << QLocale(QLocale::C);

    QList<QLocale> result;
    for (qint16 i = 0; i < localeTblSize; i++) {
        if ((language == QLocale::AnyLanguage || localeTbl[i].m_language == language)
            && (script == QLocale::AnyScript || localeTbl[i].m_script == script)
            && (country == QLocale::AnyCountry || localeTbl[i].m_country == country)) {
            QLocale locale(localeTbl[i].m_language, localeTbl[i].m_script, localeTbl[i].m_country);
            result.append(locale);
        }
    }
    return result;
}

/*!
    \obsolete
    \since 4.3

    Returns the list of countries that have entries for \a language in Qt's locale
    database. If the result is an empty list, then \a language is not represented in
    Qt's locale database.

    \sa matchingLocales()
*/
QList<QLocale::Country> QLocale::countriesForLanguage(Language language)
{
    QList<QLocale::Country> result;

    if (language == QLocale::C) {
        result.append(QLocale::AnyCountry);
        return result;
    }

    for (qint16 i = 0; i < localeTblSize; i++) {
        if (localeTbl[i].m_language == language) {
            result.append(localeTbl[i].m_country);
        }
    }

    return result;
}

/*!
    \since 4.2

    Returns the localized name of \a month, in the format specified
    by \a type.

    \sa dayName(), standaloneMonthName()
*/
QString QLocale::monthName(int month, FormatType type) const
{
    if (month < 1 || month > 12)
        return QString();

#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(type == LongFormat
                                             ? QSystemLocale::MonthNameLong : QSystemLocale::MonthNameShort,
                                             month);
        if (!res.isNull())
            return res.toString();
    }
#endif

    const qint16 idx = month - 1;
    switch (type) {
        case QLocale::LongFormat:
            return getLocaleListData(d()->m_long_month_names, idx);
        case QLocale::ShortFormat:
            return getLocaleListData(d()->m_short_month_names, idx);
        case QLocale::NarrowFormat:
            return getLocaleListData(d()->m_narrow_month_names, idx);
    }
    return QString();
}

/*!
    \since 4.5

    Returns the localized name of \a month that is used as a
    standalone text, in the format specified by \a type.

    If the locale information doesn't specify the standalone month
    name then return value is the same as in monthName().

    \sa monthName(), standaloneDayName()
*/
QString QLocale::standaloneMonthName(int month, FormatType type) const
{
    if (month < 1 || month > 12)
        return QString();

#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(type == LongFormat
                                             ? QSystemLocale::StandaloneMonthNameLong : QSystemLocale::StandaloneMonthNameShort,
                                             month);
        if (!res.isNull())
            return res.toString();
    }
#endif

    const qint16 idx = month - 1;
    switch (type) {
        case QLocale::LongFormat:
            return getLocaleListData(d()->m_standalone_long_month_names, idx);
        case QLocale::ShortFormat:
            return getLocaleListData(d()->m_standalone_short_month_names, idx);
        case QLocale::NarrowFormat:
            return getLocaleListData(d()->m_standalone_narrow_month_names, idx);
    }
    return monthName(month, type);
}

/*!
    \since 4.2

    Returns the localized name of the \a day (where 1 represents
    Monday, 2 represents Tuesday and so on), in the format specified
    by \a type.

    \sa monthName(), standaloneDayName()
*/
QString QLocale::dayName(int day, FormatType type) const
{
    if (day < 1 || day > 7)
        return QString();

#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(type == LongFormat
                                             ? QSystemLocale::DayNameLong : QSystemLocale::DayNameShort,
                                             day);
        if (!res.isNull())
            return res.toString();
    }
#endif

    const qint16 idx = day - 1;
    switch (type) {
        case QLocale::LongFormat:
            return getLocaleListData(d()->m_long_day_names, idx);
        case QLocale::ShortFormat:
            return getLocaleListData(d()->m_short_day_names, idx);
        case QLocale::NarrowFormat:
            return getLocaleListData(d()->m_narrow_day_names, idx);
    }
    return QString();
}

/*!
    \since 4.5

    Returns the localized name of the \a day (where 1 represents
    Monday, 2 represents Tuesday and so on) that is used as a
    standalone text, in the format specified by \a type.

    If the locale information does not specify the standalone day
    name then return value is the same as in dayName().

    \sa dayName(), standaloneMonthName()
*/
QString QLocale::standaloneDayName(int day, FormatType type) const
{
    if (day < 1 || day > 7)
        return QString();

#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(type == LongFormat
                                             ? QSystemLocale::DayNameLong : QSystemLocale::DayNameShort,
                                             day);
        if (!res.isNull())
            return res.toString();
    }
#endif

    const qint16 idx = day - 1;
    switch (type) {
        case QLocale::LongFormat:
            return getLocaleListData(d()->m_standalone_long_day_names, idx);
        case QLocale::ShortFormat:
            return getLocaleListData(d()->m_standalone_short_day_names, idx);
        case QLocale::NarrowFormat:
            return getLocaleListData(d()->m_standalone_narrow_day_names, idx);
    }
    return dayName(day, type);
}

/*!
    \since 4.8

    Returns the first day of the week according to the current locale.
*/
Qt::DayOfWeek QLocale::firstDayOfWeek() const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(QSystemLocale::FirstDayOfWeek, QVariant());
        if (!res.isNull())
            return static_cast<Qt::DayOfWeek>(res.toUInt());
    }
#endif
    return d()->m_first_day_of_week;
}

QLocale::MeasurementSystem QLocalePrivate::measurementSystem() const
{
    QByteArray latinbcp47 = bcp47Name().toLatin1();
    UErrorCode error = U_ZERO_ERROR;
    UMeasurementSystem measurement = ulocdata_getMeasurementSystem(latinbcp47.constData(), &error);
    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("QLocale::measurementSystem: ulocdata_getMeasurementSystem(%s) failed %s",
            latinbcp47.constData(), u_errorName(error));
        return QLocale::MetricSystem;
    }
    switch (measurement) {
        case UMS_SI:
            return QLocale::MetricSystem;
        case UMS_US:
            return QLocale::ImperialSystem;
        case UMS_UK:
            return QLocale::UKSystem;
        // just to silence compiler warning
#ifndef U_HIDE_DEPRECATED_API
        case UMS_LIMIT:
            break;
#endif
    }
    Q_UNREACHABLE();
}

/*!
    \since 4.8

    Returns a list of days that are considered weekdays according to the current locale.
*/
QList<Qt::DayOfWeek> QLocale::weekdays() const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(QSystemLocale::Weekdays, QVariant());
        if (!res.isNull())
            return static_cast<QList<Qt::DayOfWeek> >(res.value<QList<Qt::DayOfWeek> >());
    }
#endif
    QList<Qt::DayOfWeek> weekdays;
    Qt::DayOfWeek weekendStart = d()->m_weekend_start;
    Qt::DayOfWeek weekendEnd = d()->m_weekend_end;
    for (int day = Qt::Monday; day <= Qt::Sunday; day++) {
        if ((weekendEnd >= weekendStart && (day < weekendStart || day > weekendEnd)) ||
            (weekendEnd < weekendStart && (day > weekendEnd && day < weekendStart)))
                weekdays << static_cast<Qt::DayOfWeek>(day);
    }
    return weekdays;
}

/*!
    \since 4.4

    Returns the measurement system for the locale.
*/
QLocale::MeasurementSystem QLocale::measurementSystem() const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(QSystemLocale::MeasurementSystem, QVariant());
        if (!res.isNull())
            return MeasurementSystem(res.toInt());
    }
#endif

    return d()->measurementSystem();
}

/*!
  \since 4.7

  Returns the text direction of the language.
*/
Qt::LayoutDirection QLocale::textDirection() const
{
    QByteArray latinbcp47 = bcp47Name().toLatin1();
    if (uloc_isRightToLeft(latinbcp47.constData()))
        return Qt::RightToLeft;
    return Qt::LeftToRight;
}

/*!
  \since 4.8

  Returns an uppercase copy of \a str.
*/
QString QLocale::toUpper(const QString &str) const
{
    QString result;
    if (qt_u_strToUpper(str, &result, *this))
        return result;
    return str.toUpper();
}

/*!
  \since 4.8

  Returns a lowercase copy of \a str.
*/
QString QLocale::toLower(const QString &str) const
{
    QString result;
    if (qt_u_strToLower(str, &result, *this))
        return result;
    return str.toLower();
}


/*!
    \since 4.5

    Returns the localized name of the "AM" suffix for times specified using
    the conventions of the 12-hour clock.

    \sa pmText()
*/
QString QLocale::amText() const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(QSystemLocale::AMText, QVariant());
        if (!res.isNull())
            return res.toString();
    }
#endif
    return getLocaleData(d()->m_am);
}

/*!
    \since 4.5

    Returns the localized name of the "PM" suffix for times specified using
    the conventions of the 12-hour clock.

    \sa amText()
*/
QString QLocale::pmText() const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(QSystemLocale::PMText, QVariant());
        if (!res.isNull())
            return res.toString();
    }
#endif
    return getLocaleData(d()->m_pm);
}


QString QLocalePrivate::dateTimeToString(const QString &format, const QDate *date, const QTime *time,
                                         const QLocale *q) const
{
    Q_ASSERT(date || time);
    if ((date && !date->isValid()) || (time && !time->isValid()))
        return QString();
    const bool format_am_pm = time && timeFormatContainsAP(format);

    enum { AM, PM } am_pm = AM;
    int hour12 = time ? time->hour() : -1;
    if (time) {
        if (hour12 == 0) {
            am_pm = AM;
            hour12 = 12;
        } else if (hour12 < 12) {
            am_pm = AM;
        } else if (hour12 == 12) {
            am_pm = PM;
        } else {
            am_pm = PM;
            hour12 -= 12;
        }
    }

    QString result;

    int i = 0;
    while (i < format.size()) {
        if (format.at(i).unicode() == '\'') {
            result.append(qt_readEscapedFormatString(format, &i));
            continue;
        }

        const QChar c = format.at(i);
        int repeat = qt_repeatCount(format, i);
        bool used = false;
        if (date) {
            switch (c.unicode()) {
            case 'y':
                used = true;
                if (repeat >= 4)
                    repeat = 4;
                else if (repeat >= 2)
                    repeat = 2;

                switch (repeat) {
                case 4: {
                    const int yr = date->year();
                    const int len = (yr < 0) ? 5 : 4;
                    result.append(longLongToString(yr, -1, 10, len, QLocalePrivate::ZeroPadded));
                    break;
                }
                case 2: {
                    result.append(longLongToString(date->year() % 100, -1, 10, 2,
                                                   QLocalePrivate::ZeroPadded));
                    break;
                }
                default:
                    repeat = 1;
                    result.append(c);
                    break;
                }
                break;

            case 'M':
                used = true;
                repeat = qMin(repeat, 4);
                switch (repeat) {
                case 1:
                    result.append(longLongToString(date->month()));
                    break;
                case 2:
                    result.append(longLongToString(date->month(), -1, 10, 2, QLocalePrivate::ZeroPadded));
                    break;
                case 3:
                    result.append(q->monthName(date->month(), QLocale::ShortFormat));
                    break;
                case 4:
                    result.append(q->monthName(date->month(), QLocale::LongFormat));
                    break;
                }
                break;

            case 'd':
                used = true;
                repeat = qMin(repeat, 4);
                switch (repeat) {
                case 1:
                    result.append(longLongToString(date->day()));
                    break;
                case 2:
                    result.append(longLongToString(date->day(), -1, 10, 2, QLocalePrivate::ZeroPadded));
                    break;
                case 3:
                    result.append(q->dayName(date->dayOfWeek(), QLocale::ShortFormat));
                    break;
                case 4:
                    result.append(q->dayName(date->dayOfWeek(), QLocale::LongFormat));
                    break;
                }
                break;

            default:
                break;
            }
        }
        if (!used && time) {
            switch (c.unicode()) {
            case 'h': {
                used = true;
                repeat = qMin(repeat, 2);
                const int hour = format_am_pm ? hour12 : time->hour();

                switch (repeat) {
                case 1:
                    result.append(longLongToString(hour));
                    break;
                case 2:
                    result.append(longLongToString(hour, -1, 10, 2, QLocalePrivate::ZeroPadded));
                    break;
                }
                break;
            }
            case 'H':
                used = true;
                repeat = qMin(repeat, 2);
                switch (repeat) {
                case 1:
                    result.append(longLongToString(time->hour()));
                    break;
                case 2:
                    result.append(longLongToString(time->hour(), -1, 10, 2, QLocalePrivate::ZeroPadded));
                    break;
                }
                break;

            case 'm':
                used = true;
                repeat = qMin(repeat, 2);
                switch (repeat) {
                case 1:
                    result.append(longLongToString(time->minute()));
                    break;
                case 2:
                    result.append(longLongToString(time->minute(), -1, 10, 2, QLocalePrivate::ZeroPadded));
                    break;
                }
                break;

            case 's':
                used = true;
                repeat = qMin(repeat, 2);
                switch (repeat) {
                case 1:
                    result.append(longLongToString(time->second()));
                    break;
                case 2:
                    result.append(longLongToString(time->second(), -1, 10, 2, QLocalePrivate::ZeroPadded));
                    break;
                }
                break;

            case 'a':
                used = true;
                if (i + 1 < format.length() && format.at(i + 1).unicode() == 'p') {
                    repeat = 2;
                } else {
                    repeat = 1;
                }
                result.append(am_pm == AM ? q->amText().toLower() : q->pmText().toLower());
                break;

            case 'A':
                used = true;
                if (i + 1 < format.length() && format.at(i + 1).unicode() == 'P') {
                    repeat = 2;
                } else {
                    repeat = 1;
                }
                result.append(am_pm == AM ? q->amText().toUpper() : q->pmText().toUpper());
                break;

            case 'z':
                used = true;
                if (repeat >= 3) {
                    repeat = 3;
                } else {
                    repeat = 1;
                }
                switch (repeat) {
                case 1:
                    result.append(longLongToString(time->msec()));
                    break;
                case 3:
                    result.append(longLongToString(time->msec(), -1, 10, 3, QLocalePrivate::ZeroPadded));
                    break;
                }
                break;

            case 't':
                used = true;
                repeat = 1;
                result.append(timeZone());
                break;
            default:
                break;
            }
        }
        if (!used) {
            result.append(QString(repeat, c));
        }
        i += repeat;
    }

    return result;
}

QString QLocalePrivate::doubleToString(double d,
                                       int precision,
                                       DoubleForm form,
                                       int width,
                                       unsigned flags) const
{
    return QLocalePrivate::doubleToString(zero(), plus(), minus(), exponential(),
                                          group(), decimal(),
                                          d, precision, form, width, flags);
}

QString QLocalePrivate::doubleToString(const QChar _zero, const QChar plus, const QChar minus,
                                       const QChar exponential, const QChar group, const QChar decimal,
                                       double d,
                                       int precision,
                                       DoubleForm form,
                                       int width,
                                       unsigned flags)
{
    if (precision == -1)
        precision = 6;
    if (width == -1)
        width = 0;

    bool negative = false;
    bool special_number = false; // nan, +/-inf
    QString num_str;

    // Detect special numbers (nan, +/-inf)
    if (qIsInf(d)) {
        num_str = QLatin1String("inf");
        special_number = true;
        negative = d < 0;
    } else if (qIsNaN(d)) {
        num_str = QLatin1String("nan");
        special_number = true;
    }

    // Handle normal numbers
    if (!special_number) {
        int decpt, sign;
        QString digits;

        // NOT thread safe!
        if (form == DFDecimal) {
#ifdef QT_HAVE_FCVT
            digits = QString::fromLatin1(::fcvt(d, precision, &decpt, &sign));
#else
            digits = QString::fromLatin1(qfcvt(d, precision, &decpt, &sign));
#endif
        } else {
            int pr = precision;
            if (form == DFExponent)
                ++pr;
            else if (form == DFSignificantDigits && pr == 0)
                pr = 1;
#ifdef QT_HAVE_ECVT
            digits = QString::fromLatin1(::ecvt(d, pr, &decpt, &sign));
#else
            digits = QString::fromLatin1(qecvt(d, pr, &decpt, &sign));
#endif

            // Chop trailing zeros
            if (digits.length() > 0) {
                int last_nonzero_idx = digits.length() - 1;
                while (last_nonzero_idx > 0
                       && digits.unicode()[last_nonzero_idx] == QLatin1Char('0'))
                    --last_nonzero_idx;
                digits.truncate(last_nonzero_idx + 1);
            }

        }

        if (_zero.unicode() != '0') {
            ushort z = _zero.unicode() - '0';
            for (int i = 0; i < digits.length(); ++i)
                reinterpret_cast<ushort *>(digits.data())[i] += z;
        }

        bool always_show_decpt = (flags & ForcePoint);
        switch (form) {
            case DFExponent: {
                num_str = exponentForm(_zero, decimal, exponential, group, plus, minus,
                                       digits, decpt, precision, PMDecimalDigits,
                                       always_show_decpt);
                break;
            }
            case DFDecimal: {
                num_str = decimalForm(_zero, decimal, group,
                                      digits, decpt, precision, PMDecimalDigits,
                                      always_show_decpt, flags & ThousandsGroup);
                break;
            }
            case DFSignificantDigits: {
                PrecisionMode mode = (flags & ForcePoint) ?
                            PMSignificantDigits : PMChopTrailingZeros;

                if (decpt != digits.length() && (decpt <= -4 || decpt > precision))
                    num_str = exponentForm(_zero, decimal, exponential, group, plus, minus,
                                           digits, decpt, precision, mode,
                                           always_show_decpt);
                else
                    num_str = decimalForm(_zero, decimal, group,
                                          digits, decpt, precision, mode,
                                          always_show_decpt, flags & ThousandsGroup);
                break;
            }
        }

        negative = sign != 0 && !qIsZero(d);
    }

    // pad with zeros. LeftAdjusted overrides this flag). Also, we don't
    // pad special numbers
    if (flags & QLocalePrivate::ZeroPadded
            && !(flags & QLocalePrivate::LeftAdjusted)
            && !special_number) {
        int num_pad_chars = width - num_str.length();
        // leave space for the sign
        if (negative
                || flags & QLocalePrivate::AlwaysShowSign
                || flags & QLocalePrivate::BlankBeforePositive)
            --num_pad_chars;

        for (int i = 0; i < num_pad_chars; ++i)
            num_str.prepend(_zero);
    }

    // add sign
    if (negative)
        num_str.prepend(minus);
    else if (flags & QLocalePrivate::AlwaysShowSign)
        num_str.prepend(plus);
    else if (flags & QLocalePrivate::BlankBeforePositive)
        num_str.prepend(QLatin1Char(' '));

    if (flags & QLocalePrivate::CapitalEorX)
        num_str = num_str.toUpper();

    return num_str;
}

QString QLocalePrivate::longLongToString(qlonglong l, int precision,
                                            int base, int width,
                                            unsigned flags) const
{
    return QLocalePrivate::longLongToString(zero(), group(), plus(), minus(),
                                            l, precision, base, width, flags);
}

QString QLocalePrivate::longLongToString(const QChar zero, const QChar group,
                                         const QChar plus, const QChar minus,
                                         qlonglong l, int precision,
                                         int base, int width,
                                         unsigned flags)
{
    bool precision_not_specified = false;
    if (precision == -1) {
        precision_not_specified = true;
        precision = 1;
    }

    bool negative = l < 0;
    if (base != 10) {
        // these are not supported by sprintf for octal and hex
        flags &= ~AlwaysShowSign;
        flags &= ~BlankBeforePositive;
        negative = false; // neither are negative numbers
    }

    QString num_str;
    if (base == 10)
        num_str = qlltoa(l, base, zero);
    else
        num_str = qulltoa(l, base, zero);

    uint cnt_thousand_sep = 0;
    if (flags & ThousandsGroup && base == 10) {
        for (int i = num_str.length() - 3; i > 0; i -= 3) {
            num_str.insert(i, group);
            ++cnt_thousand_sep;
        }
    }

    for (int i = num_str.length()/* - cnt_thousand_sep*/; i < precision; ++i)
        num_str.prepend(base == 10 ? zero : QChar::fromLatin1('0'));

    if ((flags & ForcePoint || flags & ShowBase)
            && base == 8
            && (num_str.isEmpty() || num_str[0].unicode() != QLatin1Char('0')))
        num_str.prepend(QLatin1Char('0'));

    // LeftAdjusted overrides this flag ZeroPadded. sprintf only padds
    // when precision is not specified in the format string
    bool zero_padded = flags & ZeroPadded
                        && !(flags & LeftAdjusted)
                        && precision_not_specified;

    if (zero_padded) {
        int num_pad_chars = width - num_str.length();

        // leave space for the sign
        if (negative
                || flags & AlwaysShowSign
                || flags & BlankBeforePositive)
            --num_pad_chars;

        // leave space for optional '0x' in hex form
        if (base == 16 && (flags & ForcePoint || flags & ShowBase))
            num_pad_chars -= 2;
        // leave space for optional '0b' in binary form
        else if (base == 2 && (flags & ForcePoint || flags & ShowBase))
            num_pad_chars -= 2;

        for (int i = 0; i < num_pad_chars; ++i)
            num_str.prepend(base == 10 ? zero : QChar::fromLatin1('0'));
    }

    if (flags & CapitalEorX)
        num_str = num_str.toUpper();

    if (base == 16 && (flags & ForcePoint || flags & ShowBase))
        num_str.prepend(QLatin1String(flags & UppercaseBase ? "0X" : "0x"));
    if (base == 2 && (flags & ForcePoint || flags & ShowBase))
        num_str.prepend(QLatin1String(flags & UppercaseBase ? "0B" : "0b"));

    // add sign
    if (negative)
        num_str.prepend(minus);
    else if (flags & AlwaysShowSign)
        num_str.prepend(plus);
    else if (flags & BlankBeforePositive)
        num_str.prepend(QLatin1Char(' '));

    return num_str;
}

QString QLocalePrivate::unsLongLongToString(qulonglong l, int precision,
                                            int base, int width,
                                            unsigned flags) const
{
    return QLocalePrivate::unsLongLongToString(zero(), group(), plus(),
                                               l, precision, base, width, flags);
}

QString QLocalePrivate::unsLongLongToString(const QChar zero, const QChar group,
                                            const QChar plus,
                                            qulonglong l, int precision,
                                            int base, int width,
                                            unsigned flags)
{
    bool precision_not_specified = false;
    if (precision == -1) {
        precision_not_specified = true;
        precision = 1;
    }

    QString num_str = qulltoa(l, base, zero);

    uint cnt_thousand_sep = 0;
    if (flags & ThousandsGroup && base == 10) {
        for (int i = num_str.length() - 3; i > 0; i -=3) {
            num_str.insert(i, group);
            ++cnt_thousand_sep;
        }
    }

    for (int i = num_str.length()/* - cnt_thousand_sep*/; i < precision; ++i)
        num_str.prepend(base == 10 ? zero : QChar::fromLatin1('0'));

    if ((flags & ForcePoint || flags & ShowBase)
            && base == 8
            && (num_str.isEmpty() || num_str[0].unicode() != QLatin1Char('0')))
        num_str.prepend(QLatin1Char('0'));

    // LeftAdjusted overrides this flag ZeroPadded. sprintf only padds
    // when precision is not specified in the format string
    bool zero_padded = flags & ZeroPadded
                        && !(flags & LeftAdjusted)
                        && precision_not_specified;

    if (zero_padded) {
        int num_pad_chars = width - num_str.length();

        // leave space for optional '0x' in hex form
        if (base == 16 && flags & ForcePoint)
            num_pad_chars -= 2;
        // leave space for optional '0b' in binary form
        else if (base == 2 && flags & ForcePoint)
            num_pad_chars -= 2;

        for (int i = 0; i < num_pad_chars; ++i)
            num_str.prepend(base == 10 ? zero : QChar::fromLatin1('0'));
    }

    if (flags & CapitalEorX)
        num_str = num_str.toUpper();

    if (base == 16 && (flags & ForcePoint || flags & ShowBase))
        num_str.prepend(QLatin1String(flags & UppercaseBase ? "0X" : "0x"));
    else if (base == 2 && (flags & ForcePoint || flags & ShowBase))
        num_str.prepend(QLatin1String(flags & UppercaseBase ? "0B" : "0b"));

    // add sign
    if (flags & AlwaysShowSign)
        num_str.prepend(plus);
    else if (flags & BlankBeforePositive)
        num_str.prepend(QLatin1Char(' '));

    return num_str;
}

/*
    Converts a number in locale to its representation in the C locale.
    Only has to guarantee that a string that is a correct representation of
    a number will be converted. If junk is passed in, junk will be passed
    out and the error will be detected during the actual conversion to a
    number. We can't detect junk here, since we don't even know the base
    of the number.
*/
bool QLocalePrivate::numberToCLocale(const QString &num,
                                            GroupSeparatorMode group_sep_mode,
                                            CharBuff *result) const
{
    const QChar *uc = num.unicode();
    int l = num.length();
    int idx = 0;

    // Skip whitespace
    while (idx < l && uc[idx].isSpace())
        ++idx;
    if (idx == l)
        return false;

    while (idx < l) {
        const QChar &in = uc[idx];

        char out = digitToCLocale(in);
        if (out == 0) {
            if (in == list())
                out = ';';
            else if (in == percent())
                out = '%';
            // for handling base-x numbers
            else if (in.unicode() >= 'A' && in.unicode() <= 'Z')
                out = in.toLower().toLatin1();
            else if (in.unicode() >= 'a' && in.unicode() <= 'z')
                out = in.toLatin1();
            else
                break;
        }

        result->append(out);

        ++idx;
    }

    // Check trailing whitespace
    for (; idx < l; ++idx) {
        if (!uc[idx].isSpace())
            return false;
    }

    result->append('\0');

    // Check separators
    if (group_sep_mode == ParseGroupSeparators
            && !removeGroupSeparators(result))
        return false;


    return true;
}

bool QLocalePrivate::validateChars(const QString &str, NumberMode numMode, QByteArray *buff,
                                    int decDigits) const
{
    buff->clear();
    buff->reserve(str.length());

    const bool scientific = numMode == DoubleScientificMode;
    bool lastWasE = false;
    bool lastWasDigit = false;
    int eCnt = 0;
    int decPointCnt = 0;
    bool dec = false;
    int decDigitCnt = 0;

    for (int i = 0; i < str.length(); ++i) {
        char c = digitToCLocale(str.at(i));

        if (c >= '0' && c <= '9') {
            if (numMode != IntegerMode) {
                // If a double has too many digits after decpt, it shall be Invalid.
                if (dec && decDigits != -1 && decDigits < ++decDigitCnt)
                    return false;
            }
            lastWasDigit = true;
        } else {
            switch (c) {
                case '.':
                    if (numMode == IntegerMode) {
                        // If an integer has a decimal point, it shall be Invalid.
                        return false;
                    } else {
                        // If a double has more than one decimal point, it shall be Invalid.
                        if (++decPointCnt > 1)
                            return false;
#if 0
                        // If a double with no decimal digits has a decimal point, it shall be
                        // Invalid.
                        if (decDigits == 0)
                            return false;
#endif                  // On second thoughts, it shall be Valid.

                        dec = true;
                    }
                    break;

                case '+':
                case '-':
                    if (scientific) {
                        // If a scientific has a sign that's not at the beginning or after
                        // an 'e', it shall be Invalid.
                        if (i != 0 && !lastWasE)
                            return false;
                    } else {
                        // If a non-scientific has a sign that's not at the beginning,
                        // it shall be Invalid.
                        if (i != 0)
                            return false;
                    }
                    break;

                case ',':
                    //it can only be placed after a digit which is before the decimal point
                    if (!lastWasDigit || decPointCnt > 0)
                        return false;
                    break;

                case 'e':
                    if (scientific) {
                        // If a scientific has more than one 'e', it shall be Invalid.
                        if (++eCnt > 1)
                            return false;
                        dec = false;
                    } else {
                        // If a non-scientific has an 'e', it shall be Invalid.
                        return false;
                    }
                    break;

                default:
                    // If it's not a valid digit, it shall be Invalid.
                    return false;
            }
            lastWasDigit = false;
        }

        lastWasE = c == 'e';
        if (c != ',')
            buff->append(c);
    }

    return true;
}

double QLocalePrivate::stringToDouble(const QString &number, bool *ok,
                                        GroupSeparatorMode group_sep_mode) const
{
    CharBuff buff;
    if (!numberToCLocale(group().unicode() == 0xa0 ? number.trimmed() : number,
                         group_sep_mode, &buff)) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0.0;
    }
    return bytearrayToDouble(buff.constData(), ok);
}

qlonglong QLocalePrivate::stringToLongLong(const QString &number, int base,
                                           bool *ok, GroupSeparatorMode group_sep_mode) const
{
    CharBuff buff;
    if (!numberToCLocale(group().unicode() == 0xa0 ? number.trimmed() : number,
                         group_sep_mode, &buff)) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }

    return bytearrayToLongLong(buff.constData(), base, ok);
}

qulonglong QLocalePrivate::stringToUnsLongLong(const QString &number, int base,
                                               bool *ok, GroupSeparatorMode group_sep_mode) const
{
    CharBuff buff;
    if (!numberToCLocale(group().unicode() == 0xa0 ? number.trimmed() : number,
                         group_sep_mode, &buff)) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }

    return bytearrayToUnsLongLong(buff.constData(), base, ok);
}


double QLocalePrivate::bytearrayToDouble(const char *num, bool *ok)
{
    if (Q_UNLIKELY(*num == '\0')) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0.0;
    }

    char *endptr;
    double ret = std::strtod(num, &endptr);
    if ((ret == 0.0l && errno == ERANGE) || ret == HUGE_VAL || ret == -HUGE_VAL) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0.0;
    }

    if (*endptr != '\0') {
        // stopped at a non-digit character after converting some digits
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0.0;
    }

    if (ok != Q_NULLPTR)
        *ok = true;
    return ret;
}

qlonglong QLocalePrivate::bytearrayToLongLong(const char *num, int base, bool *ok)
{
    if (Q_UNLIKELY(*num == '\0')) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }

    char *endptr;
    qlonglong ret = std::strtoll(num, &endptr, base);
    if ((ret == LLONG_MIN || ret == LLONG_MAX) && (errno == ERANGE || errno == EINVAL)) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }

    if (*endptr != '\0') {
        // stopped at a non-digit character after converting some digits
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }

    if (ok != Q_NULLPTR)
        *ok = true;
    return ret;
}

qulonglong QLocalePrivate::bytearrayToUnsLongLong(const char *num, int base, bool *ok)
{
    if (Q_UNLIKELY(*num == '\0')) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }

    char *endptr;
    qulonglong ret = std::strtoull(num, &endptr, base);
    if (ret == ULLONG_MAX && (errno == ERANGE || errno == EINVAL)) {
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }

    if (*endptr != '\0') {
        // stopped at a non-digit character after converting some digits
        if (ok != Q_NULLPTR)
            *ok = false;
        return 0;
    }

    if (ok != Q_NULLPTR)
        *ok = true;
    return ret;
}

/*!
    \since 4.8

    \enum QLocale::CurrencySymbolFormat

    Specifies the format of the currency symbol.

    \value CurrencyIsoCode a ISO-4217 code of the currency.
    \value CurrencySymbol a currency symbol.
    \value CurrencyDisplayName a user readable name of the currency.
*/

/*!
    \since 4.8
    Returns a currency symbol according to the \a format.
*/
QString QLocale::currencySymbol(QLocale::CurrencySymbolFormat format) const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(QSystemLocale::CurrencySymbol, format);
        if (!res.isNull())
            return res.toString();
    }
#endif
    switch (format) {
        case CurrencySymbol:
            return getLocaleData(d()->m_currency_symbol);
        case CurrencyDisplayName:
            return getLocaleData(d()->m_currency_display_name);
        case CurrencyIsoCode: {
            return QString::fromUtf8(d()->m_currency_iso_code);
        }
    }
    return QString();
}

/*!
    \since 4.8

    Returns a localized string representation of \a value as a currency.
    If the \a symbol is provided it is used instead of the default currency symbol.

    \sa currencySymbol()
*/
QString QLocale::toCurrencyString(qlonglong value, const QString &symbol) const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QSystemLocale::CurrencyToStringArgument arg(value, symbol);
        QVariant res = systemLocale()->query(QSystemLocale::CurrencyToString, QVariant::fromValue(arg));
        if (!res.isNull())
            return res.toString();
    }
#endif
    const QLocalePrivate *dd = this->d();
    const char* currency_negative_format = dd->m_currency_negative_format;
    bool tonegative = false;
    if (currency_negative_format && value < 0) {
        tonegative = true;
        value = -value;
    }
    QString str = toString(value);
    QString sym = symbol.isNull() ? currencySymbol() : symbol;
    if (sym.isEmpty())
        sym = currencySymbol(QLocale::CurrencyIsoCode);
    QString format = (tonegative ? getLocaleData(currency_negative_format) : getLocaleData(dd->m_currency_format));
    return format.arg(str, sym);
}

/*!
    \since 4.8
    \overload
*/
QString QLocale::toCurrencyString(qulonglong value, const QString &symbol) const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QSystemLocale::CurrencyToStringArgument arg(value, symbol);
        QVariant res = systemLocale()->query(QSystemLocale::CurrencyToString, QVariant::fromValue(arg));
        if (!res.isNull())
            return res.toString();
    }
#endif
    const QLocalePrivate *dd = this->d();
    QString str = toString(value);
    QString sym = symbol.isNull() ? currencySymbol() : symbol;
    if (sym.isEmpty())
        sym = currencySymbol(QLocale::CurrencyIsoCode);
    QString format = getLocaleData(dd->m_currency_format);
    return format.arg(str, sym);
}

/*!
    \since 4.8
    \overload
*/
QString QLocale::toCurrencyString(double value, const QString &symbol) const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QSystemLocale::CurrencyToStringArgument arg(value, symbol);
        QVariant res = systemLocale()->query(QSystemLocale::CurrencyToString, QVariant::fromValue(arg));
        if (!res.isNull())
            return res.toString();
    }
#endif
    const QLocalePrivate *dd = this->d();
    const char* currency_negative_format = dd->m_currency_negative_format;
    bool tonegative = false;
    if (currency_negative_format && value < 0) {
        tonegative = true;
        value = -value;
    }
    QString str = toString(value, 'f', dd->m_currency_digits);
    QString sym = symbol.isNull() ? currencySymbol() : symbol;
    if (sym.isEmpty())
        sym = currencySymbol(QLocale::CurrencyIsoCode);
    QString format = (tonegative ? getLocaleData(currency_negative_format) : getLocaleData(dd->m_currency_format));
    return format.arg(str, sym);
}

/*!
    \since 4.8

    Returns an ordered list of locale names for translation purposes in
    preference order.

    The return value represents locale names that the user expects to see the
    UI translation in.

    Most like you do not need to use this function directly, but just pass the
    QLocale object to the QTranslator::load() function.

    The first item in the list is the most preferred one.

    \sa QTranslator, bcp47Name()
*/
QStringList QLocale::uiLanguages() const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(QSystemLocale::UILanguages, QVariant());
        if (!res.isNull()) {
            QStringList result = res.toStringList();
            if (!result.isEmpty())
                return result;
        }
    }
#endif
    return QStringList(bcp47Name());
}

/*!
    \since 4.8

    Returns a native name of the language for the locale. For example
    "Schwiizertüütsch" for Swiss-German locale.

    \sa nativeCountryName(), languageToString()
*/
QString QLocale::nativeLanguageName() const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(QSystemLocale::NativeLanguageName, QVariant());
        if (!res.isNull())
            return res.toString();
    }
#endif
    return getLocaleData(d()->m_language_endonym);
}

/*!
    \since 4.8

    Returns a native name of the country for the locale. For example
    "España" for Spanish/Spain locale.

    \sa nativeLanguageName(), countryToString()
*/
QString QLocale::nativeCountryName() const
{
#ifndef QT_NO_SYSTEMLOCALE
    if (d() == systemPrivate()) {
        QVariant res = systemLocale()->query(QSystemLocale::NativeCountryName, QVariant());
        if (!res.isNull())
            return res.toString();
    }
#endif
    return getLocaleData(d()->m_country_endonym);
}

QT_END_NAMESPACE

#ifndef QT_NO_QOBJECT
#include "moc_qlocale.h"
#endif

