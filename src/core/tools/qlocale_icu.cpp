/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
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

#include "qglobal.h"
#include "qdebug.h"

#include <unicode/ucol.h>
#include <unicode/ustring.h>

QT_BEGIN_NAMESPACE

typedef int32_t (*Ptr_u_strToCase)(UChar *dest, int32_t destCapacity, const UChar *src, int32_t srcLength, const char *locale, UErrorCode *pErrorCode);

static UCollator *icuCollator = Q_NULLPTR;

bool qt_initIcu(const QString &localeString)
{
    if (icuCollator) {
        ucol_close(icuCollator);
        icuCollator = Q_NULLPTR;
    }

    UErrorCode icuStatus = U_ZERO_ERROR;
    icuCollator = ucol_open(localeString.toLatin1().constData(), &icuStatus);

    if (Q_UNLIKELY(!icuCollator)) {
        qWarning("Unable to open locale %s in ICU, error code %d", qPrintable(localeString), icuStatus);
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

    *result = ucol_strcoll(icuCollator, reinterpret_cast<const UChar *>(source), int32_t(sourceLength),
                               reinterpret_cast<const UChar *>(target), int32_t(targetLength));

    return true;
}

// caseFunc can either be u_strToUpper or u_strToLower
static bool qt_u_strToCase(const QString &str, QString *out, const QLocale &locale, Ptr_u_strToCase caseFunc)
{
    Q_ASSERT(out);

    if (!icuCollator)
        return false;

    QString result(str.size(), Qt::Uninitialized);

    UErrorCode status = U_ZERO_ERROR;

    int32_t size = caseFunc(reinterpret_cast<UChar *>(result.data()), result.size(),
            reinterpret_cast<const UChar *>(str.constData()), str.size(),
            locale.bcp47Name().toLatin1().constData(), &status);

    if (U_FAILURE(status))
        return false;

    if (size < result.size()) {
        result.resize(size);
    } else if (size > result.size()) {
        // the resulting string is larger than our source string
        result.resize(size);

        status = U_ZERO_ERROR;
        size = caseFunc(reinterpret_cast<UChar *>(result.data()), result.size(),
            reinterpret_cast<const UChar *>(str.constData()), str.size(),
            locale.bcp47Name().toLatin1().constData(), &status);

        if (U_FAILURE(status))
            return false;

        // if the sizes don't match now, we give up.
        if (size != result.size())
            return false;
    }

    *out = result;
    return true;
}

bool qt_u_strToUpper(const QString &str, QString *out, const QLocale &locale)
{
    return qt_u_strToCase(str, out, locale, u_strToUpper);
}

bool qt_u_strToLower(const QString &str, QString *out, const QLocale &locale)
{
    return qt_u_strToCase(str, out, locale, u_strToLower);
}

QT_END_NAMESPACE
