/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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
#include <QDebug>
#include <QString>
#include <qtest.h>

QT_USE_NAMESPACE

static const QByteArray lorem = QByteArray("Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.");

class tst_qchar : public QObject
{
    Q_OBJECT
private slots:
    void digit_value();
    void joining();
    void mirrored_char();
    void unicode_version();
};

void tst_qchar::digit_value()
{
    const char* data = lorem.constData();
    const int size = lorem.size();
    QBENCHMARK {
        for (int i = 0; i < size; i++) {
            QChar::digitValue(uint(data[i]));
        }
    }
}

void tst_qchar::joining()
{
    const char* data = lorem.constData();
    const int size = lorem.size();
    QBENCHMARK {
        for (int i = 0; i < size; i++) {
            QChar::joining(uint(data[i]));
        }
    }
}

void tst_qchar::mirrored_char()
{
    const char* data = lorem.constData();
    const int size = lorem.size();
    QBENCHMARK {
        for (int i = 0; i < size; i++) {
            QChar::mirroredChar(uint(data[i]));
        }
    }
}

void tst_qchar::unicode_version()
{
    const char* data = lorem.constData();
    const int size = lorem.size();
    QBENCHMARK {
        for (int i = 0; i < size; i++) {
            QChar::unicodeVersion(uint(data[i]));
        }
    }
}

QTEST_MAIN(tst_qchar)

#include "moc_main.cpp"
