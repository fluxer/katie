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

Q_DECLARE_METATYPE(QImage)

class tst_QImageConversion : public QObject
{
    Q_OBJECT
private slots:
    void convertRgb16ToRGB32_data();
    void convertRgb16ToRGB32();

private:
    QImage generateImage(int width, int height);
};

void tst_QImageConversion::convertRgb16ToRGB32_data()
{
    QTest::addColumn<QImage>("inputImage");
    // height = 5000 to get interesting timing.

    // 3 pixels wide -> smaller than regular vector of 128bits
    QTest::newRow("width: 3px; height: 5000px;") << generateImage(3, 5000);

    // 8 pixels wide -> potential for 2 vectors
    QTest::newRow("width: 8px; height: 5000px;") << generateImage(3, 5000);

    // 16 pixels, minimum for the SSSE3 implementation
    QTest::newRow("width: 16px; height: 5000px;") << generateImage(16, 5000);

    // 50 pixels, more realistic use case
    QTest::newRow("width: 50px; height: 5000px;") << generateImage(50, 5000);

    // 2000 pixels -> typical values for pictures
    QTest::newRow("width: 2000px; height: 5000px;") << generateImage(2000, 5000);
}

void tst_QImageConversion::convertRgb16ToRGB32()
{
    QFETCH(QImage, inputImage);

    QBENCHMARK {
        volatile QImage output = inputImage.convertToFormat(QImage::Format_RGB32);
        // we need the volatile and the following to make sure the compiler does not do
        // anything stupid :)
        (void)output;
    }
}

/*
 Fill a RGB16 image with "random" pixel values.
 */
QImage tst_QImageConversion::generateImage(int width, int height)
{
    QImage image(width, height, QImage::Format_RGB16);
    const int byteWidth = width * 2;

    for (int y = 0; y < image.height(); ++y) {
        uchar *scanline = image.scanLine(y);
        for (int x = 0; x < byteWidth; ++x)
            scanline[x] = x ^ y;
    }
    return image;
}

QTEST_MAIN(tst_QImageConversion)

#include "moc_tst_qimageconversion.cpp"
