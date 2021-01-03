/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtTest module of the Katie Toolkit.
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

#include "qabstracttestlogger_p.h"
#include "qtestlog_p.h"
#include "qtestassert.h"
#include "qtestcommon_p.h"
#include "qplatformdefs.h"
#include "qbytearray.h"

#include <stdarg.h>
#include <unistd.h>

QT_BEGIN_NAMESPACE

void QAbstractTestLogger::outputString(const char *msg)
{
    QTEST_ASSERT(QTest::stream);

    ::fputs(msg, QTest::stream);
    ::fflush(QTest::stream);
}

bool QAbstractTestLogger::isTtyOutput()
{
    QTEST_ASSERT(QTest::stream);

    static bool ttyoutput = ::isatty(QT_FILENO(QTest::stream));
    return ttyoutput;
}


void QAbstractTestLogger::startLogging()
{
    QTEST_ASSERT(!QTest::stream);

    const char *out = QTestLog::outputFileName();
    if (!out) {
        QTest::stream = stdout;
        return;
    }
    QTest::stream = QT_FOPEN(out, "wt");
    if (!QTest::stream) {
        printf("Unable to open file for logging: %s", out);
        ::exit(1);
    }
}

void QAbstractTestLogger::stopLogging()
{
    QTEST_ASSERT(QTest::stream);
    if (QTest::stream != stdout) {
        ::fclose(QTest::stream);
    } else {
    }
    QTest::stream = 0;
}

namespace QTest
{

extern void filter_unprintable(char *str);

/*!
    \fn int QTest::qt_asprintf(QTestCharBuffer *buf, const char *format, ...);
    \internal
 */
int qt_asprintf(QTestCharBuffer *str, const char *format, ...)
{
    static const int MAXSIZE = 1024*1024*2;

    Q_ASSERT(str);

    int size = str->size();

    va_list ap;
    int res = 0;

    for (;;) {
        va_start(ap, format);
        res = qvsnprintf(str->data(), size, format, ap);
        va_end(ap);
        str->data()[size - 1] = '\0';
        if (res >= 0 && res < size) {
            // We succeeded
            break;
        }
        // buffer wasn't big enough, try again.
        // Note, we're assuming that a result of -1 is always due to running out of space.
        size *= 2;
        if (size > MAXSIZE) {
            break;
        }
        if (!str->reset(size))
            break; // out of memory - take what we have
    }

    filter_unprintable(str->data());

    return res;
}

}

QT_END_NAMESPACE
