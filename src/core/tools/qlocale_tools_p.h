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

#ifndef QLOCALE_TOOLS_P_H
#define QLOCALE_TOOLS_P_H

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

#include "qlocale_p.h"
#include "qstring.h"

#include <cmath>

QT_BEGIN_NAMESPACE

QString qulltoa(qulonglong l, int base, const QChar zero);
QString qlltoa(qlonglong l, int base, const QChar zero);

enum PrecisionMode {
    PMDecimalDigits =       0x01,
    PMSignificantDigits =   0x02,
    PMChopTrailingZeros =   0x03
};

QString &decimalForm(QChar zero, QChar decimal, QChar group,
                     QString &digits, int decpt, uint precision,
                     PrecisionMode pm,
                     bool always_show_decpt,
                     bool thousands_group);
QString &exponentForm(QChar zero, QChar decimal, QChar exponential,
                      QChar group, QChar plus, QChar minus,
                      QString &digits, int decpt, uint precision,
                      PrecisionMode pm,
                      bool always_show_decpt);

static inline bool qIsZero(double d)
{
    return (std::fpclassify(d) == FP_ZERO);
}

static inline bool qIsUpper(char ch)
{
    return (ch >= 'A' && ch <= 'Z');
}

static inline bool qIsDigit(char ch)
{
    return (ch >= '0' && ch <= '9');
}

static inline char qToLower(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
        return (ch + 32);
    return ch;
}

static inline char qToUpper(char ch)
{
    if (ch >= 'a' && ch <= 'z')
        return (ch - 32);
    return ch;
}

// Removes thousand-group separators in "C" locale.
bool removeGroupSeparators(QLocalePrivate::CharBuff *num);

bool qt_initLocale(const QString &locale);
bool qt_ucol_strcoll(const QChar *source, int sourceLength, const QChar *target, int targetLength, int *result);
bool qt_u_strToUpper(const QString &str, QString *out, const QLocale &locale);
bool qt_u_strToLower(const QString &str, QString *out, const QLocale &locale);

#define QECVT_BUFFSIZE 16
char *qfcvt(double x, int n, int *dp, int *sign, char* buf);
char *qecvt(double x, int n, int *dp, int *sign, char* buf);

QT_END_NAMESPACE

#endif
