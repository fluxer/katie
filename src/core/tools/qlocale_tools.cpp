/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#include "qlocale_tools_p.h"
#include "qlocale_p.h"
#include "qstring.h"
#include "qcorecommon_p.h"

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <cmath>

#if defined(Q_OS_LINUX) && !defined(__UCLIBC__)
#    include <fenv.h>
#endif

#include <unicode/ucol.h>
#include <unicode/ustring.h>

QT_BEGIN_NAMESPACE

QString qulltoa(qulonglong l, int base, const QChar _zero)
{
    ushort buff[65]; // length of MAX_ULLONG in base 2
    ushort *p = buff + 65;

    if (base != 10 || _zero.unicode() == '0') {
        while (l != 0) {
            int c = l % base;

            --p;

            if (c < 10)
                *p = '0' + c;
            else
                *p = c - 10 + 'a';

            l /= base;
        }
    }
    else {
        while (l != 0) {
            int c = l % base;

            *(--p) = _zero.unicode() + c;

            l /= base;
        }
    }

    return QString(reinterpret_cast<QChar *>(p), 65 - (p - buff));
}

QString qlltoa(qlonglong l, int base, const QChar zero)
{
    return qulltoa(l < 0 ? -l : l, base, zero);
}

QString &decimalForm(QChar zero, QChar decimal, QChar group,
                     QString &digits, int decpt, uint precision,
                     PrecisionMode pm,
                     bool always_show_decpt,
                     bool thousands_group)
{
    if (decpt < 0) {
        for (int i = 0; i < -decpt; ++i)
            digits.prepend(zero);
        decpt = 0;
    }
    else if (decpt > digits.length()) {
        for (int i = digits.length(); i < decpt; ++i)
            digits.append(zero);
    }

    if (pm == PMDecimalDigits) {
        uint decimal_digits = digits.length() - decpt;
        for (uint i = decimal_digits; i < precision; ++i)
            digits.append(zero);
    }
    else if (pm == PMSignificantDigits) {
        for (uint i = digits.length(); i < precision; ++i)
            digits.append(zero);
    }
    else { // pm == PMChopTrailingZeros
    }

    if (always_show_decpt || decpt < digits.length())
        digits.insert(decpt, decimal);

    if (thousands_group) {
        for (int i = decpt - 3; i > 0; i -= 3)
            digits.insert(i, group);
    }

    if (decpt == 0)
        digits.prepend(zero);

    return digits;
}

QString &exponentForm(QChar zero, QChar decimal, QChar exponential,
                      QChar group, QChar plus, QChar minus,
                      QString &digits, int decpt, uint precision,
                      PrecisionMode pm,
                      bool always_show_decpt)
{
    int exp = decpt - 1;

    if (pm == PMDecimalDigits) {
        for (uint i = digits.length(); i < precision + 1; ++i)
            digits.append(zero);
    }
    else if (pm == PMSignificantDigits) {
        for (uint i = digits.length(); i < precision; ++i)
            digits.append(zero);
    }
    else { // pm == PMChopTrailingZeros
    }

    if (always_show_decpt || digits.length() > 1)
        digits.insert(1, decimal);

    digits.append(exponential);
    digits.append(QLocalePrivate::longLongToString(zero, group, plus, minus,
                   exp, 2, 10, -1, QLocalePrivate::AlwaysShowSign));

    return digits;
}

// Removes thousand-group separators in "C" locale.
bool removeGroupSeparators(QLocalePrivate::CharBuff *num)
{
    int group_cnt = 0; // counts number of group chars
    int decpt_idx = -1;

    char *data = num->data();
    int l = qstrlen(data);

    // Find the decimal point and check if there are any group chars
    int i = 0;
    for (; i < l; ++i) {
        char c = data[i];

        if (c == ',') {
            if (i == 0 || data[i - 1] < '0' || data[i - 1] > '9')
                return false;
            if (i == l - 1 || data[i + 1] < '0' || data[i + 1] > '9')
                return false;
            ++group_cnt;
        }
        else if (c == '.') {
            // Fail if more than one decimal points
            if (decpt_idx != -1)
                return false;
            decpt_idx = i;
        } else if (c == 'e' || c == 'E') {
            // an 'e' or 'E' - if we have not encountered a decimal
            // point, this is where it "is".
            if (decpt_idx == -1)
                decpt_idx = i;
        }
    }

    // If no group chars, we're done
    if (group_cnt == 0)
        return true;

    // No decimal point means that it "is" at the end of the string
    if (decpt_idx == -1)
        decpt_idx = l;

    i = 0;
    while (i < l && group_cnt > 0) {
        char c = data[i];

        if (c == ',') {
            // Don't allow group chars after the decimal point
            if (i > decpt_idx)
                return false;

            // Check that it is placed correctly relative to the decpt
            if ((decpt_idx - i) % 4 != 0)
                return false;

            // Remove it
            memmove(data + i, data + i + 1, l - i - 1);
            data[--l] = '\0';

            --group_cnt;
            --decpt_idx;
        } else {
            // Check that we are not missing a separator
            if (i < decpt_idx
                    && (decpt_idx - i) % 4 == 0
                    && !(i == 0 && c == '-')) // check for negative sign at start of string
                return false;
            ++i;
        }
    }

    return true;
}

qulonglong qstrtoull(const char *nptr, const char **endptr, int base, bool *ok)
{
    qulonglong ret = std::strtoull(const_cast<char*>(nptr), const_cast<char**>(endptr), base);
    if (ok) {
      if(ret == ULLONG_MAX && (errno == ERANGE || errno == EINVAL))
        *ok = false;
      else
        *ok = true;
    }
    return ret;
}

qlonglong qstrtoll(const char *nptr, const char **endptr, int base, bool *ok)
{
    qlonglong ret = std::strtoll(const_cast<char*>(nptr), const_cast<char**>(endptr), base);
    if (ok) {
      if((ret == LLONG_MIN || ret == LLONG_MAX)
         && (errno == ERANGE || errno == EINVAL))
        *ok = false;
      else
        *ok = true;
    }
    return ret;
}

double qstrtod(const char *s00, const char **se, bool *ok)
{
    double ret = std::strtod(const_cast<char*>(s00), const_cast<char**>(se));
    if (ok) {
      if((ret == 0.0l && errno == ERANGE)
         || ret == HUGE_VAL || ret == -HUGE_VAL)
        *ok = false;
      else
        *ok = true; // the result will be that we don't report underflow in this case
    }
    return ret;
}


/******************************************************************************
** Helpers for string casing and collation
*/
static UCollator *icuCollator = Q_NULLPTR;

static void qt_deinitLocale()
{
    if (icuCollator) {
        ucol_close(icuCollator);
        icuCollator = Q_NULLPTR;
    }
}
Q_DESTRUCTOR_FUNCTION(qt_deinitLocale);

bool qt_initLocale(const QString &locale)
{
    qt_deinitLocale();

    UErrorCode error = U_ZERO_ERROR;
    icuCollator = ucol_open(locale.toLatin1().constData(), &error);

    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("qt_initLocale: ucol_open(%s) failed %s",
            locale.toLatin1().constData(), u_errorName(error));
        return false;
    }

    return true;
}

bool qt_ucol_strcoll(const QChar *source, int sourceLength, const QChar *target, int targetLength, int *result)
{
    Q_ASSERT(result);
    Q_ASSERT(source);
    Q_ASSERT(target);

    if (!icuCollator)
        return false;

    *result = ucol_strcoll(icuCollator, reinterpret_cast<const UChar *>(source), sourceLength,
                           reinterpret_cast<const UChar *>(target), targetLength);

    return true;
}

bool qt_u_strToUpper(const QString &str, QString *out, const QLocale &locale)
{
    Q_ASSERT(out);
    out->resize(QMAXUSTRLEN(str.size()));

    UErrorCode error = U_ZERO_ERROR;
    const int upperresult = u_strToUpper(reinterpret_cast<UChar*>(out->data()), out->size(),
        reinterpret_cast<const UChar*>(str.unicode()), str.size(),
        locale.bcp47Name().toLatin1().constData(), &error);
    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("qt_u_strToUpper: u_strToUpper(%s) failed %s",
            locale.bcp47Name().toLatin1().constData(), u_errorName(error));
        out->clear();
        return false;
    }

    out->resize(upperresult);
    return true;
}

bool qt_u_strToLower(const QString &str, QString *out, const QLocale &locale)
{
    Q_ASSERT(out);
    out->resize(QMAXUSTRLEN(str.size()));

    UErrorCode error = U_ZERO_ERROR;
    const int lowerresult = u_strToLower(reinterpret_cast<UChar*>(out->data()), out->size(),
        reinterpret_cast<const UChar*>(str.unicode()), str.size(),
        locale.bcp47Name().toLatin1().constData(), &error);
    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("qt_u_strToLower: u_strToLower(%s) failed %s",
            locale.bcp47Name().toLatin1().constData(), u_errorName(error));
        out->clear();
        return false;
    }

    out->resize(lowerresult);
    return true;
}

QT_END_NAMESPACE
