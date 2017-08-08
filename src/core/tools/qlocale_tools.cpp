/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#if defined(Q_OS_LINUX) && !defined(__UCLIBC__)
#    include <fenv.h>
#endif

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

/*-
 * Copyright (c) 1992, 1993
 *        The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgment:
 *        This product includes software developed by the University of
 *        California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// static char sccsid[] = "@(#)strtouq.c        8.1 (Berkeley) 6/4/93";
//  "$FreeBSD: src/lib/libc/stdlib/strtoull.c,v 1.5.2.1 2001/03/02 09:45:20 obrien Exp $";

/*
 * Convert a string to an unsigned long long integer.
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
qulonglong qstrtoull(const char *nptr, const char **endptr, int base, bool *ok)
{
    const char *s = nptr;
    qulonglong acc;
    unsigned char c;
    qulonglong qbase, cutoff;
    int any, cutlim;

    if (ok != Q_NULLPTR)
        *ok = true;

    /*
     * See strtoq for comments as to the logic used.
     */
    s = nptr;
    do {
        c = *s++;
    } while (isspace(c));
    if (c == '-') {
        if (ok != Q_NULLPTR)
            *ok = false;
        if (endptr != 0)
            *endptr = s - 1;
        return 0;
    } else {
        if (c == '+')
            c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    qbase = unsigned(base);
    cutoff = qulonglong(ULLONG_MAX) / qbase;
    cutlim = qulonglong(ULLONG_MAX) % qbase;
    for (acc = 0, any = 0;; c = *s++) {
        if (!isascii(c))
            break;
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= qbase;
            acc += c;
        }
    }
    if (any == 0) {
        if (ok != Q_NULLPTR)
            *ok = false;
    } else if (any < 0) {
        acc = ULLONG_MAX;
        if (ok != Q_NULLPTR)
            *ok = false;
    }
    if (endptr != 0)
        *endptr = (any ? s - 1 : nptr);
    return acc;
}


//  "$FreeBSD: src/lib/libc/stdlib/strtoll.c,v 1.5.2.1 2001/03/02 09:45:20 obrien Exp $";


/*
 * Convert a string to a long long integer.
 *
 * Ignores `locale' stuff.  Assumes that the upper and lower case
 * alphabets and digits are each contiguous.
 */
qlonglong qstrtoll(const char *nptr, const char **endptr, int base, bool *ok)
{
    const char *s;
    qulonglong acc;
    unsigned char c;
    qulonglong qbase, cutoff;
    int neg, any, cutlim;

    /*
     * Skip white space and pick up leading +/- sign if any.
     * If base is 0, allow 0x for hex and 0 for octal, else
     * assume decimal; if base is already 16, allow 0x.
     */
    s = nptr;
    do {
        c = *s++;
    } while (isspace(c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else {
        neg = 0;
        if (c == '+')
            c = *s++;
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;

    /*
     * Compute the cutoff value between legal numbers and illegal
     * numbers.  That is the largest legal value, divided by the
     * base.  An input number that is greater than this value, if
     * followed by a legal input character, is too big.  One that
     * is equal to this value may be valid or not; the limit
     * between valid and invalid numbers is then based on the last
     * digit.  For instance, if the range for quads is
     * [-9223372036854775808..9223372036854775807] and the input base
     * is 10, cutoff will be set to 922337203685477580 and cutlim to
     * either 7 (neg==0) or 8 (neg==1), meaning that if we have
     * accumulated a value > 922337203685477580, or equal but the
     * next digit is > 7 (or 8), the number is too big, and we will
     * return a range error.
     *
     * Set any if any `digits' consumed; make it negative to indicate
     * overflow.
     */
    qbase = unsigned(base);
    cutoff = neg ? qulonglong(0-(LLONG_MIN + LLONG_MAX)) + LLONG_MAX : LLONG_MAX;
    cutlim = cutoff % qbase;
    cutoff /= qbase;
    for (acc = 0, any = 0;; c = *s++) {
        if (!isascii(c))
            break;
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= qbase;
            acc += c;
        }
    }
    if (any < 0) {
        acc = neg ? LLONG_MIN : LLONG_MAX;
        if (ok != Q_NULLPTR)
            *ok = false;
    } else if (neg) {
        acc = (~acc) + 1;
    }
    if (endptr != 0)
        *endptr = (any >= 0 ? s - 1 : nptr);

    if (ok != Q_NULLPTR)
        *ok = any > 0;

    return acc;
}

double qstrtod(const char *s00, const char **se, bool *ok)
{
    double ret = std::strtod((char*)s00, (char**)se);
    if (ok) {
      if((ret == 0.0l && errno == ERANGE)
         || ret == HUGE_VAL || ret == -HUGE_VAL)
        *ok = false;
      else
        *ok = true; // the result will be that we don't report underflow in this case
    }
    return ret;
}

QT_END_NAMESPACE
