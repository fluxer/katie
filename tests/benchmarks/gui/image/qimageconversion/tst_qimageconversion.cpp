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
#include <QImage>

Q_DECLARE_METATYPE(QImage::Format)

/*
 Fill a image with "random" pixel values.
 */
static QImage generateImage(const QSize &size, const QImage::Format format)
{
    QImage image(size.width(), size.height(), format);
    bool oddnumber = true;
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if (image.depth() == 1) {
                image.setPixel(x, y, uint(oddnumber));
                oddnumber = !oddnumber;
            } else {
                image.setPixel(x, y, x ^ y);
            }
        }
    }
    return image;
}


class tst_QImageConversion : public QObject
{
    Q_OBJECT
private slots:
    void convertToFormat_data();
    void convertToFormat();
};

void tst_QImageConversion::convertToFormat_data()
{
    QTest::addColumn<QImage::Format>("sourceFormat");
    QTest::addColumn<QImage::Format>("targetFormat");
    QTest::addColumn<QSize>("size");

    QList<QSize> sizes;
    sizes << QSize(1, 1)
          << QSize(10, 10)
          << QSize(100, 100)
          << QSize(1000, 1000);

    const char *formatNames[] = {
        "Invalid",
        "Mono",
        "MonoLSB",
        "RGB32",
        "ARGB32",
        "ARGB32_pm",
        "RGB16",
    };

    for (int tar=1; tar<QImage::NImageFormats; ++tar) {
        for (int src=1; src<QImage::NImageFormats; ++src) {

            foreach (const QSize &s, sizes) {
                QString name = QString::fromLatin1("%1 to %2, (%3x%4)")
                               .arg(formatNames[src])
                               .arg(formatNames[tar])
                               .arg(s.width()).arg(s.height());
                QTest::newRow(name.toLatin1()) << (QImage::Format) src
                                               << (QImage::Format) tar
                                               << s;
            }
        }
    }
}

void tst_QImageConversion::convertToFormat()
{
    QFETCH(QImage::Format, sourceFormat);
    QFETCH(QImage::Format, targetFormat);
    QFETCH(QSize, size);

    QImage sourceImage = generateImage(size, sourceFormat);

    QBENCHMARK {
        QImage result = sourceImage.convertToFormat(targetFormat);
        QVERIFY(!result.isNull());
    }
}

QTEST_MAIN(tst_QImageConversion)

#include "moc_tst_qimageconversion.cpp"
