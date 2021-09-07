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

#include "qlocale_tools_p.h"
#include "qlocale_p.h"
#include "qstring.h"
#include "qcorecommon_p.h"

#include <stdio.h>
#include <string.h>

#include <unicode/ucol.h>
#include <unicode/ustring.h>

QT_BEGIN_NAMESPACE

QString qulltoa(qulonglong l, int base, const QChar zero)
{
    QSTACKARRAY(ushort, buff, 65); // length of MAX_ULLONG in base 2
    ushort *p = buff + 65;

    if (base != 10 || zero.unicode() == '0') {
        while (l != 0) {
            int c = l % base;

            --p;

            if (c < 10)
                *p = '0' + c;
            else
                *p = c - 10 + 'a';

            l /= base;
        }
    } else {
        while (l != 0) {
            int c = l % base;

            *(--p) = zero.unicode() + c;

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
    } else if (decpt > digits.length()) {
        for (int i = digits.length(); i < decpt; ++i)
            digits.append(zero);
    }

    if (pm == PMDecimalDigits) {
        uint decimal_digits = digits.length() - decpt;
        for (uint i = decimal_digits; i < precision; ++i)
            digits.append(zero);
    } else if (pm == PMSignificantDigits) {
        for (uint i = digits.length(); i < precision; ++i)
            digits.append(zero);
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

/******************************************************************************
** Helpers for string casing and collation
*/
static UCollator *icuCollator = nullptr;

static void qt_deinitLocale()
{
    if (icuCollator) {
        ucol_close(icuCollator);
        icuCollator = nullptr;
    }
}
Q_DESTRUCTOR_FUNCTION(qt_deinitLocale);

bool qt_initLocale(const QString &locale)
{
    qt_deinitLocale();

    QByteArray latinlocale = locale.toLatin1();
    UErrorCode error = U_ZERO_ERROR;
    icuCollator = ucol_open(latinlocale.constData(), &error);

    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("qt_initLocale: ucol_open(%s) failed %s",
            latinlocale.constData(), u_errorName(error));
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

    QByteArray latinbcp47 = locale.bcp47Name().toLatin1();
    UErrorCode error = U_ZERO_ERROR;
    const int upperresult = u_strToUpper(reinterpret_cast<UChar*>(out->data()), out->size(),
        reinterpret_cast<const UChar*>(str.unicode()), str.size(),
        latinbcp47.constData(), &error);
    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("qt_u_strToUpper: u_strToUpper(%s) failed %s",
            latinbcp47.constData(), u_errorName(error));
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

    QByteArray latinbcp47 = locale.bcp47Name().toLatin1();
    UErrorCode error = U_ZERO_ERROR;
    const int lowerresult = u_strToLower(reinterpret_cast<UChar*>(out->data()), out->size(),
        reinterpret_cast<const UChar*>(str.unicode()), str.size(),
        latinbcp47.constData(), &error);
    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("qt_u_strToLower: u_strToLower(%s) failed %s",
            latinbcp47.constData(), u_errorName(error));
        out->clear();
        return false;
    }

    out->resize(lowerresult);
    return true;
}


/*
----------------------------------------------------------------------
Copyright © 2005-2020 Rich Felker, et al.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------
*/
// the code bellow is copy from musl libc, modified to not use static buffer
// and use snprintf() instead of sprintf()
char *qfcvt(double x, int n, int *dp, int *sign, char* buf)
{
    QSTACKARRAY(char, tmp, 1500);
    int i, lz;

    if (n > 1400) n = 1400;
    ::snprintf(tmp, sizeof(tmp), "%.*f", n, x);
    i = (tmp[0] == '-');
    if (tmp[i] == '0') lz = strspn(tmp+i+2, "0");
    else lz = -(int)strcspn(tmp+i, ".");

    if (n<=lz) {
        *sign = i;
        *dp = 1;
        if (n>14) n = 14;
        return (char*)"000000000000000"+14-n;
    }

    return qecvt(x, n-lz, dp, sign, buf);
}

char *qecvt(double x, int n, int *dp, int *sign, char* buf)
{
    QSTACKARRAY(char, tmp, 32);
    int i, j;

    if (n-1 > 15) n = 15;
    ::snprintf(tmp, sizeof(tmp), "%.*e", n-1, x);
    i = *sign = (tmp[0]=='-');
    for (j=0; tmp[i]!='e'; j+=(tmp[i++]!='.'))
        buf[j] = tmp[i];
    buf[j] = 0;
    *dp = atoi(tmp+i+1)+1;

    return buf;
}

QT_END_NAMESPACE
