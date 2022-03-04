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

#include <qtest.h>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>

//TESTED_FILES=

class tst_QImageReader : public QObject
{
    Q_OBJECT

public:
    tst_QImageReader();
    virtual ~tst_QImageReader();

private slots:
    void readImage_data();
    void readImage();

    void setScaledSize_data();
    void setScaledSize();

private:
    QList< QPair<QString, QByteArray> > images; // filename, format
};

tst_QImageReader::tst_QImageReader()
{
    foreach (const QByteArray &format, QImageReader::supportedImageFormats()) {
        const QString benchfilepath = QLatin1String(SRCDIR "/images/bench.") + format;
        const QString benchlargefilepath = QLatin1String(SRCDIR "/images/bench-large.") + format;
        const QString benchtransparentfilepath = QLatin1String(SRCDIR "/images/bench-transparent.") + format;

        if (QFile::exists(benchfilepath)) {
            images << QPair<QString, QByteArray>(benchfilepath, format);
        }
        if (QFile::exists(benchlargefilepath)) {
            images << QPair<QString, QByteArray>(benchlargefilepath, format);
        }
        if (QFile::exists(benchtransparentfilepath)) {
            images << QPair<QString, QByteArray>(benchtransparentfilepath, format);
        }
    }
}

tst_QImageReader::~tst_QImageReader()
{
}

void tst_QImageReader::readImage_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QByteArray>("format");

    for (int i = 0; i < images.size(); ++i) {
        const QString file = images[i].first;
        const QByteArray format = images[i].second;
        QTest::newRow(qPrintable(file)) << file << format;
    }
}

void tst_QImageReader::readImage()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    QBENCHMARK {
        QImageReader io(fileName, format);
        QImage image = io.read();
        QVERIFY(!image.isNull());
    }
}

void tst_QImageReader::setScaledSize_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QByteArray>("format");
    QTest::addColumn<QSize>("newSize");

    for (int i = 0; i < images.size(); ++i) {
        const QString file = images[i].first;
        const QByteArray format = images[i].second;
        QTest::newRow(qPrintable(file)) << file << format << QSize(200, 200);
    }
}

void tst_QImageReader::setScaledSize()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);
    QFETCH(QSize, newSize);

    QBENCHMARK {
        QImageReader reader(fileName, format);
        reader.setScaledSize(newSize);
        QImage image = reader.read();
        QCOMPARE(image.size(), newSize);
    }
}

QTEST_MAIN(tst_QImageReader)

#include "moc_tst_qimagereader.cpp"
