/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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

#ifndef QLOCALE_P_H
#define QLOCALE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of internal files.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/qstring.h"
#include "QtCore/qvarlengtharray.h"
#include "QtCore/qmetatype.h"

#include "qlocale.h"


QT_BEGIN_NAMESPACE

class Q_CORE_EXPORT QLocalePrivate
{
public:
    QChar decimal() const { return QChar(m_decimal); }
    QChar group() const { return QChar(m_group); }
    QChar list() const { return QChar(m_list); }
    QChar percent() const { return QChar(m_percent); }
    QChar zero() const { return QChar(m_zero); }
    QChar plus() const { return QChar(m_plus); }
    QChar minus() const { return QChar(m_minus); }
    QChar exponential() const { return QChar(m_exponential); }

    QString bcp47Name() const;

    static QLocale::Language codeToLanguage(const QString &code);
    static QLocale::Script codeToScript(const QString &code);
    static QLocale::Country codeToCountry(const QString &code);
    static void getLangAndCountry(const QString &name, QLocale::Language &lang,
                                  QLocale::Script &script, QLocale::Country &cntry);
    static const QLocalePrivate *findLocale(QLocale::Language language,
                                            QLocale::Script script,
                                            QLocale::Country country);


    QLocale::MeasurementSystem measurementSystem() const;

    enum DoubleForm {
        DFExponent = 0,
        DFDecimal,
        DFSignificantDigits,
        _DFMax = DFSignificantDigits
    };

    enum Flags {
        NoFlags             = 0,
        ForcePoint          = 0x01,
        ZeroPadded          = 0x02,
        LeftAdjusted        = 0x04,
        BlankBeforePositive = 0x08,
        AlwaysShowSign      = 0x10,
        ThousandsGroup      = 0x20,
        CapitalEorX         = 0x40,

        ShowBase            = 0x80,
        UppercaseBase       = 0x100,
    };

    enum GroupSeparatorMode {
        FailOnGroupSeparators,
        ParseGroupSeparators
    };

    static QString doubleToString(const QChar zero, const QChar plus,
                                  const QChar minus, const QChar exponent,
                                  const QChar group, const QChar decimal,
                                  double d, int precision,
                                  DoubleForm form,
                                  int width, unsigned flags);
    static QString longLongToString(const QChar zero, const QChar group,
                                    const QChar plus, const QChar minus,
                                    qint64 l, int precision, int base,
                                    int width, unsigned flags);
    static QString unsLongLongToString(const QChar zero, const QChar group,
                                       const QChar plus,
                                       quint64 l, int precision,
                                       int base, int width,
                                       unsigned flags);

    QString doubleToString(double d,
                           int precision = -1,
                           DoubleForm form = DFSignificantDigits,
                           int width = -1,
                           unsigned flags = NoFlags) const;
    QString longLongToString(qint64 l, int precision = -1,
                             int base = 10,
                             int width = -1,
                             unsigned flags = NoFlags) const;
    QString unsLongLongToString(quint64 l, int precision = -1,
                                int base = 10,
                                int width = -1,
                                unsigned flags = NoFlags) const;
    double stringToDouble(const QString &num, bool *ok, GroupSeparatorMode group_sep_mode) const;
    qint64 stringToLongLong(const QString &num, int base, bool *ok, GroupSeparatorMode group_sep_mode) const;
    quint64 stringToUnsLongLong(const QString &num, int base, bool *ok, GroupSeparatorMode group_sep_mode) const;


    static double bytearrayToDouble(const char *num, bool *ok);
    static qint64 bytearrayToLongLong(const char *num, int base, bool *ok);
    static quint64 bytearrayToUnsLongLong(const char *num, int base, bool *ok);

    typedef QVarLengthArray<char, 32> CharBuff;
    bool numberToCLocale(const QString &num,
                         GroupSeparatorMode group_sep_mode,
                         CharBuff *result) const;
    inline char digitToCLocale(const QChar &c) const;

    static void updateSystemPrivate();

    enum NumberMode { IntegerMode, DoubleStandardMode, DoubleScientificMode };
    bool validateChars(const QString &str, NumberMode numMode, QByteArray *buff, int decDigits = -1) const;

    QString dateTimeToString(const QString &format, const QDate *date, const QTime *time,
                             const QLocale *q) const;

    QLocale::Language m_language;
    QLocale::Script m_script;
    QLocale::Country m_country;
    Qt::DayOfWeek m_first_day_of_week;
    Qt::DayOfWeek m_weekend_start;
    Qt::DayOfWeek m_weekend_end;
    ushort m_decimal;
    ushort m_group;
    ushort m_list;
    ushort m_percent;
    ushort m_minus;
    ushort m_plus;
    ushort m_exponential;
    ushort m_currency_digits;
    uint m_zero;
    const uint m_quotation_start;
    const uint m_quotation_end;
    const uint m_alternate_quotation_start;
    const uint m_alternate_quotation_end;
    const char* m_language_endonym;
    const char* m_country_endonym;
    const char* m_list_pattern_part_start;
    const char* m_list_pattern_part_mid;
    const char* m_list_pattern_part_end;
    const char* m_list_pattern_part_two;
    const char* m_short_date_format;
    const char* m_long_date_format;
    const char* m_short_time_format;
    const char* m_long_time_format;
    const char* m_am;
    const char* m_pm;
    const char* m_currency_symbol;
    const char* m_currency_format;
    const char* m_currency_negative_format;
    const char* m_currency_iso_code;
    const char* m_currency_display_name;
    const char* m_standalone_short_month_names[12];
    const char* m_standalone_long_month_names[12];
    const char* m_standalone_narrow_month_names[12];
    const char* m_short_month_names[12];
    const char* m_long_month_names[12];
    const char* m_narrow_month_names[12];
    const char* m_standalone_short_day_names[7];
    const char* m_standalone_long_day_names[7];
    const char* m_standalone_narrow_day_names[7];
    const char* m_short_day_names[7];
    const char* m_long_day_names[7];
    const char* m_narrow_day_names[7];
};

inline char QLocalePrivate::digitToCLocale(const QChar &in) const
{
    const QChar _zero = zero();
    const QChar _group = group();
    const ushort zeroUnicode = _zero.unicode();
    const ushort tenUnicode = zeroUnicode + 10;

    if (in.unicode() >= zeroUnicode && in.unicode() < tenUnicode)
        return '0' + in.unicode() - zeroUnicode;

    if (in.unicode() >= '0' && in.unicode() <= '9')
        return in.toLatin1();

    if (in == plus() || in == QLatin1Char('+'))
        return '+';

    if (in == minus() || in == QLatin1Char('-') || in == QChar(0x2212))
        return '-';

    if (in == decimal())
        return '.';

    if (in == group())
        return ',';

    if (in == exponential() || in == exponential().toUpper())
        return 'e';

    // In several languages group() is the char 0xA0, which looks like a space.
    // People use a regular space instead of it and complain it doesn't work.
    if (_group.unicode() == 0xA0 && in.unicode() == ' ')
        return ',';

    return 0;
}

bool qt_splitLocaleName(const QString &name, QString &lang, QString &script, QString &cntry);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QStringRef)
Q_DECLARE_METATYPE(QList<Qt::DayOfWeek>)

#endif // QLOCALE_P_H
