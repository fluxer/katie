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
#include <QIODevice>
#include <QFile>
#include <QString>
#include <QTemporaryFile>
#include <qtest.h>

QT_USE_NAMESPACE

class tst_qtemporaryfile : public QObject
{
    Q_OBJECT
private slots:
    void openclose_data();
    void openclose();
    void readwrite_data() { openclose_data(); }
    void readwrite();

private:
};

void tst_qtemporaryfile::openclose_data()
{
    QTest::addColumn<qint64>("amount");
    QTest::newRow("100")   << qint64(100);
    QTest::newRow("1000")  << qint64(1000);
    QTest::newRow("10000") << qint64(10000);
}

void tst_qtemporaryfile::openclose()
{
    QFETCH(qint64, amount);

    QBENCHMARK {
        for (qint64 i = 0; i < amount; ++i) {
            QTemporaryFile file;
            file.open();
            file.close();
        }
    }
}

void tst_qtemporaryfile::readwrite()
{
    QFETCH(qint64, amount);

    const int dataSize = 4096;
    QByteArray data;
    data.fill('a', dataSize);
    QBENCHMARK {
        for (qint64 i = 0; i < amount; ++i) {
            QTemporaryFile file;
            file.open();
            file.write(data);
            file.seek(0);
            file.read(dataSize);
            file.close();
        }
    }
}

QTEST_MAIN(tst_qtemporaryfile)

#include "moc_main.cpp"
