/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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

#include "qtestfilelogger.h"
#include "qtestassert.h"
#include "qtestcommon_p.h"
#include "qtestlog_p.h"
#include "qtestresult_p.h"
#include "qdir.h"
#include "qplatformdefs.h"

QT_BEGIN_NAMESPACE

QTestFileLogger::QTestFileLogger()
{
}

QTestFileLogger::~QTestFileLogger()
{
    if(QTest::stream)
        ::fclose(QTest::stream);

    QTest::stream = 0;
}

void QTestFileLogger::init()
{
    char filename[100];
    int index = 0;
    QTest::qt_snprintf(filename, sizeof(filename), "%s.log",
                QTestResult::currentTestObjectName());
 
     // Keep filenames simple
    for (uint i = index; i < sizeof(filename) && filename[i]; ++i) {
        char& c = filename[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
              || (c >= '0' && c <= '9') || c == '-' || c == '.')) {
            c = '_';
        }
    }

    QTest::stream = QT_FOPEN(filename, "wt");
    if (!QTest::stream) {
        printf("Unable to open file for simple logging: %s", filename);
        ::exit(1);
    }
}

void QTestFileLogger::flush(const char *msg)
{
    QTEST_ASSERT(QTest::stream);

    ::fputs(msg, QTest::stream);
    ::fflush(QTest::stream);
}

QT_END_NAMESPACE

