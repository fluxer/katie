/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QTextCodec>
#include <QtTest/QtTest>

class tst_QString: public QObject
{
    Q_OBJECT
public:
    tst_QString();
private slots:
    void equals() const;
    void equals_data() const;
    void fromUtf8() const;
    void fromLatin1_data() const;
    void fromLatin1() const;
    void toLower() const;
    void toUpper() const;
    void toCaseFolded() const;
};

void tst_QString::equals() const
{
    QFETCH(QString, a);
    QFETCH(QString, b);

    QBENCHMARK {
        (void)(a == b);
    }
}

tst_QString::tst_QString()
{
}

void tst_QString::equals_data() const
{
    static const struct {
        ushort data[80];
        int dummy;              // just to ensure 4-byte alignment
    } data = {
        {
            64, 64, 64, 64,  64, 64, 64, 64,
            64, 64, 64, 64,  64, 64, 64, 64, // 16
            64, 64, 64, 64,  64, 64, 64, 64,
            64, 64, 64, 64,  64, 64, 64, 64, // 32
            64, 64, 64, 64,  64, 64, 64, 64,
            64, 64, 64, 64,  64, 64, 64, 64, // 48
            64, 64, 64, 64,  64, 64, 64, 64,
            64, 64, 64, 64,  64, 64, 64, 64, // 64
            64, 64, 64, 64,  96, 96, 96, 96,
            64, 64, 96, 96,  96, 96, 96, 96  // 80
        }, 0
    };
    const QChar *ptr = reinterpret_cast<const QChar *>(data.data);

    QTest::addColumn<QString>("a");
    QTest::addColumn<QString>("b");
    QString base = QString::fromRawData(ptr, 64);

    QTest::newRow("different-length") << base << QString::fromRawData(ptr, 4);
    QTest::newRow("same-string") << base << base;
    QTest::newRow("same-data") << base << QString::fromRawData(ptr, 64);

    // try to avoid crossing a cache line (that is, at ptr[64])
    QTest::newRow("aligned-aligned-4n")
            << QString::fromRawData(ptr, 60) << QString::fromRawData(ptr + 2, 60);
    QTest::newRow("aligned-unaligned-4n")
            << QString::fromRawData(ptr, 60) << QString::fromRawData(ptr + 1, 60);
    QTest::newRow("unaligned-unaligned-4n")
            << QString::fromRawData(ptr + 1, 60) << QString::fromRawData(ptr + 3, 60);

    QTest::newRow("aligned-aligned-4n+1")
            << QString::fromRawData(ptr, 61) << QString::fromRawData(ptr + 2, 61);
    QTest::newRow("aligned-unaligned-4n+1")
            << QString::fromRawData(ptr, 61) << QString::fromRawData(ptr + 1, 61);
    QTest::newRow("unaligned-unaligned-4n+1")
            << QString::fromRawData(ptr + 1, 61) << QString::fromRawData(ptr + 3, 61);

    QTest::newRow("aligned-aligned-4n-1")
            << QString::fromRawData(ptr, 59) << QString::fromRawData(ptr + 2, 59);
    QTest::newRow("aligned-unaligned-4n-1")
            << QString::fromRawData(ptr, 59) << QString::fromRawData(ptr + 1, 59);
    QTest::newRow("unaligned-unaligned-4n-1")
            << QString::fromRawData(ptr + 1, 59) << QString::fromRawData(ptr + 3, 59);

    QTest::newRow("aligned-aligned-2n")
            << QString::fromRawData(ptr, 58) << QString::fromRawData(ptr + 2, 58);
    QTest::newRow("aligned-unaligned-2n")
            << QString::fromRawData(ptr, 58) << QString::fromRawData(ptr + 1, 58);
    QTest::newRow("unaligned-unaligned-2n")
            << QString::fromRawData(ptr + 1, 58) << QString::fromRawData(ptr + 3, 58);
}

void tst_QString::fromUtf8() const
{
    QFile file(QLatin1String(SRCDIR "utf-8.txt"));
    if (!file.open(QFile::ReadOnly)) {
        qFatal("Cannot open input file");
        return;
    }
    QByteArray data = file.readAll();
    const char *d = data.constData();
    int size = data.size();

    QBENCHMARK {
        QString::fromUtf8(d, size);
    }
}

void tst_QString::fromLatin1_data() const
{
    QTest::addColumn<QByteArray>("latin1");

    // make all the strings have the same length
    QTest::newRow("ascii-only") << QByteArray("HelloWorld");
    QTest::newRow("ascii+control") << QByteArray("Hello\1\r\n\x7f\t");
    QTest::newRow("ascii+nul") << QByteArray("a\0zbc\0defg", 10);
    QTest::newRow("non-ascii") << QByteArray("\x80\xc0\xff\x81\xc1\xfe\x90\xd0\xef\xa0");
}

void tst_QString::fromLatin1() const
{
    QFETCH(QByteArray, latin1);

    while (latin1.length() < 128) {
        latin1 += latin1;
    }

    QByteArray copy1 = latin1, copy2 = latin1, copy3 = latin1;
    copy1.chop(1);
    copy2.detach();
    copy3 += latin1; // longer length
    copy2.clear();

    QBENCHMARK {
        QString s1 = QString::fromLatin1(latin1);
        QString s2 = QString::fromLatin1(latin1);
        QString s3 = QString::fromLatin1(copy1);
        QString s4 = QString::fromLatin1(copy3);
        s3 = QString::fromLatin1(copy3);
    }
}

void tst_QString::toLower() const
{
    QFile file(QLatin1String(SRCDIR "utf-8.txt"));
    if (!file.open(QFile::ReadOnly)) {
        qFatal("Cannot open input file");
        return;
    }
    QByteArray data = file.readAll();
    const char *d = data.constData();
    int size = data.size();
    QString s = QString::fromUtf8(d, size);

    QBENCHMARK {
        (void)s.toLower();
    }
}

void tst_QString::toUpper() const
{
    QFile file(QLatin1String(SRCDIR "utf-8.txt"));
    if (!file.open(QFile::ReadOnly)) {
        qFatal("Cannot open input file");
        return;
    }
    QByteArray data = file.readAll();
    const char *d = data.constData();
    int size = data.size();
    QString s = QString::fromUtf8(d, size);

    QBENCHMARK {
        (void)s.toUpper();
    }
}

void tst_QString::toCaseFolded() const
{
    QFile file(QLatin1String(SRCDIR "utf-8.txt"));
    if (!file.open(QFile::ReadOnly)) {
        qFatal("Cannot open input file");
        return;
    }
    QByteArray data = file.readAll();
    const char *d = data.constData();
    int size = data.size();
    QString s = QString::fromUtf8(d, size);

    QBENCHMARK {
        (void)s.toCaseFolded();
    }
}

QTEST_MAIN(tst_QString)

#include "moc_tst_bench_qstring.cpp"
