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
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qlocale_p.h"
#include "qdatetime.h"
#include "qstringlist.h"
#include "qvariant.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_SYSTEMLOCALE
struct QSystemLocaleData
{
    QSystemLocaleData()
        : lc_numeric(QLocale::C)
         ,lc_time(QLocale::C)
         ,lc_monetary(QLocale::C)
         ,lc_messages(QLocale::C)
    {
        updateLocale();
    }

    void updateLocale()
    {
        const QByteArray all = qgetenv("LC_ALL");
        QByteArray numeric  = all.isEmpty() ? qgetenv("LC_NUMERIC") : all;
        QByteArray time     = all.isEmpty() ? qgetenv("LC_TIME") : all;
        QByteArray monetary = all.isEmpty() ? qgetenv("LC_MONETARY") : all;
        lc_messages_var     = all.isEmpty() ? qgetenv("LC_MESSAGES") : all;
        lc_measurement_var  = all.isEmpty() ? qgetenv("LC_MEASUREMENT") : all;

        QByteArray lang = qgetenv("LANG");
        if (lang.isEmpty())
            lang = QByteArray("C");
        if (numeric.isEmpty())
            numeric = lang;
        if (time.isEmpty())
            time = lang;
        if (monetary.isEmpty())
            monetary = lang;
        if (lc_messages_var.isEmpty())
            lc_messages_var = lang;
        if (lc_measurement_var.isEmpty())
            lc_measurement_var = lang;
        lc_numeric = QLocale(QString::fromLatin1(numeric.constData()));
        lc_time = QLocale(QString::fromLatin1(time.constData()));
        lc_monetary = QLocale(QString::fromLatin1(monetary.constData()));
        lc_messages = QLocale(QString::fromLatin1(lc_messages_var.constData()));
    }

    QLocale lc_numeric;
    QLocale lc_time;
    QLocale lc_monetary;
    QLocale lc_messages;
    QByteArray lc_messages_var;
    QByteArray lc_measurement_var;
};
Q_GLOBAL_STATIC(QSystemLocaleData, qSystemLocaleData)

QLocale QSystemLocale::fallbackLocale() const
{

    QByteArray lang = qgetenv("LC_ALL");

    if (lang.isEmpty())
        lang = qgetenv("LC_MESSAGES");
    if (lang.isEmpty())
        lang = qgetenv("LANG");
    return QLocale(QString::fromLatin1(lang.constData()));
}

QVariant QSystemLocale::query(QueryType type, QVariant in) const
{
    QSystemLocaleData *d = qSystemLocaleData();

    switch (type) {
    case DecimalPoint:
        return d->lc_numeric.decimalPoint();
    case GroupSeparator:
        return d->lc_numeric.groupSeparator();
    case ZeroDigit:
        return d->lc_numeric.zeroDigit();
    case NegativeSign:
        return d->lc_numeric.negativeSign();
    case DateFormatLong:
        return d->lc_time.dateFormat(QLocale::LongFormat);
    case DateFormatShort:
        return d->lc_time.dateFormat(QLocale::ShortFormat);
    case TimeFormatLong:
        return d->lc_time.timeFormat(QLocale::LongFormat);
    case TimeFormatShort:
        return d->lc_time.timeFormat(QLocale::ShortFormat);
    case DayNameLong:
        return d->lc_time.dayName(in.toInt(), QLocale::LongFormat);
    case DayNameShort:
        return d->lc_time.dayName(in.toInt(), QLocale::ShortFormat);
    case MonthNameLong:
        return d->lc_time.monthName(in.toInt(), QLocale::LongFormat);
    case MonthNameShort:
        return d->lc_time.monthName(in.toInt(), QLocale::ShortFormat);
    case StandaloneMonthNameLong:
         return d->lc_time.standaloneMonthName(in.toInt(), QLocale::LongFormat);
    case StandaloneMonthNameShort:
         return d->lc_time.standaloneMonthName(in.toInt(), QLocale::ShortFormat);
    case DateToStringLong:
        return d->lc_time.toString(in.toDate(), QLocale::LongFormat);
    case DateToStringShort:
        return d->lc_time.toString(in.toDate(), QLocale::ShortFormat);
    case TimeToStringLong:
        return d->lc_time.toString(in.toTime(), QLocale::LongFormat);
    case TimeToStringShort:
        return d->lc_time.toString(in.toTime(), QLocale::ShortFormat);
    case DateTimeFormatLong:
        return d->lc_time.dateTimeFormat(QLocale::LongFormat);
    case DateTimeFormatShort:
        return d->lc_time.dateTimeFormat(QLocale::ShortFormat);
    case DateTimeToStringLong:
        return d->lc_time.toString(in.toDateTime(), QLocale::LongFormat);
    case DateTimeToStringShort:
        return d->lc_time.toString(in.toDateTime(), QLocale::ShortFormat);
    case PositiveSign:
        return d->lc_numeric.positiveSign();
    case AMText:
        return d->lc_time.amText();
    case PMText:
        return d->lc_time.pmText();
    case FirstDayOfWeek:
        return d->lc_time.firstDayOfWeek();
    case CurrencySymbol:
        return d->lc_monetary.currencySymbol(QLocale::CurrencySymbolFormat(in.toUInt()));
    case CurrencyToString: {
        switch (in.type()) {
        case QVariant::Int:
            return d->lc_monetary.toCurrencyString(in.toInt());
        case QVariant::UInt:
            return d->lc_monetary.toCurrencyString(in.toUInt());
        case QVariant::Double:
            return d->lc_monetary.toCurrencyString(in.toDouble());
        case QVariant::LongLong:
            return d->lc_monetary.toCurrencyString(in.toLongLong());
        case QVariant::ULongLong:
            return d->lc_monetary.toCurrencyString(in.toULongLong());
        default:
            break;
        }
        return QString();
    }
    case MeasurementSystem: {
        if (qstrnicmp("Metric", d->lc_measurement_var.constData(), d->lc_measurement_var.size()) == 0
            || qstrnicmp("Other", d->lc_measurement_var.constData(), d->lc_measurement_var.size()) == 0) {
            return QLocale::MetricSystem;
        }
        const QString meas_locale = QString::fromLatin1(d->lc_measurement_var.constData(), d->lc_measurement_var.size());
        return QVariant((int)QLocale(meas_locale).measurementSystem());
    }
    case UILanguages: {
        static QString languages = QString::fromLatin1(qgetenv("LANGUAGE").constData());
        if (!languages.isEmpty()) {
            QStringList lst = languages.split(QLatin1Char(':'));
            for (int i = 0; i < lst.size();) {
                const QString &name = lst.at(i);
                QString lang, script, cntry;
                if (name.isEmpty() || !qt_splitLocaleName(name, lang, script, cntry))
                    lst.removeAt(i);
                else
                    ++i;
            }
            return lst;
        }
        if (!d->lc_messages_var.isEmpty()) {
            QString lang, script, cntry;
            if (qt_splitLocaleName(QString::fromLatin1(d->lc_messages_var.constData(), d->lc_messages_var.size()),
                                lang, script, cntry)) {
                if (!cntry.length() && lang.length())
                    return QStringList(lang);
                return QStringList(lang + QLatin1Char('-') + cntry);
            }
        }
        return QVariant();
    }
    case StringToStandardQuotation:
        return d->lc_messages.quoteString(in.value<QStringRef>());
    case StringToAlternateQuotation:
        return d->lc_messages.quoteString(in.value<QStringRef>(), QLocale::AlternateQuotation);
    case ListToSeparatedString:
        return d->lc_messages.createSeparatedList(in.value<QStringList>());
    case LocaleChanged:
        d->updateLocale();
        break;
    default:
        break;
    }
    return QVariant();
}
#endif // QT_NO_SYSTEMLOCALE

QT_END_NAMESPACE
