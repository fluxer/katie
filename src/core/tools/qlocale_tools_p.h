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

QT_BEGIN_NAMESPACE

QString qulltoa(qulonglong l, int base, const QChar _zero);
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
    uchar *ch = (uchar *)&d;
#ifdef QT_ARMFPA
        return !(ch[3] & 0x7F || ch[2] || ch[1] || ch[0] || ch[7] || ch[6] || ch[5] || ch[4]);
#else
    if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
        return !(ch[0] & 0x7F || ch[1] || ch[2] || ch[3] || ch[4] || ch[5] || ch[6] || ch[7]);
    } else {
        return !(ch[7] & 0x7F || ch[6] || ch[5] || ch[4] || ch[3] || ch[2] || ch[1] || ch[0]);
    }
#endif
}

static inline bool qIsUpper(char ch)
{
    return ch >= 'A' && ch <= 'Z';
}

static inline bool qIsDigit(char ch)
{
    return ch >= '0' && ch <= '9';
}

static inline char qToLower(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
        return ch - 'A' + 'a';
    else
        return ch;
}

static inline bool qIsAlnum(char c)
{
    return (c >= '0' && c <= '9') || ((c | 0x20) >= 'a' && (c | 0x20) <= 'z');
}

// Removes thousand-group separators in "C" locale.
bool removeGroupSeparators(QLocalePrivate::CharBuff *num);

double qstrtod(const char *s00, char const **se, bool *ok);
qlonglong qstrtoll(const char *nptr, const char **endptr, int base, bool *ok);
qulonglong qstrtoull(const char *nptr, const char **endptr, int base, bool *ok);

bool qt_initLocale(const QString &locale);
bool qt_ucol_strcoll(const QChar *source, int sourceLength, const QChar *target, int targetLength, int *result);
bool qt_u_strToUpper(const QString &str, QString *out, const QLocale &locale);
bool qt_u_strToLower(const QString &str, QString *out, const QLocale &locale);

#if !defined(QT_HAVE_FCVT) || !defined(QT_HAVE_ECVT)
char *qfcvt(double x, int n, int *dp, int *sign);
char *qecvt(double x, int n, int *dp, int *sign);
#endif // QT_HAVE_ECVT

QT_END_NAMESPACE

#endif
