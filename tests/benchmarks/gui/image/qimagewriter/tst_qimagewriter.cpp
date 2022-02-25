/****************************************************************************
**
** Copyright (C) 2022 Ivailo Monev
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
#include <QImageWriter>
#include <QDir>

//TESTED_FILES=

class tst_QImageWriter : public QObject
{
    Q_OBJECT

private slots:
    void writeImage_data();
    void writeImage();
};

void tst_QImageWriter::writeImage_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QByteArray>("format");
    foreach (const QByteArray &format, QImageWriter::supportedImageFormats()) {
        QTest::newRow(format) << QString::fromLatin1("bench.png") << QByteArray(format);
    }
}

void tst_QImageWriter::writeImage()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    QImage source(QLatin1String(SRCDIR "/images/") + fileName, "PNG");
    QVERIFY(!source.isNull());

    const QString destination = QDir::tempPath() + QLatin1String("/bench.") + format;
    QBENCHMARK {
        QImageWriter writer(destination, format);
        QVERIFY(writer.write(source));
    }
}

QTEST_MAIN(tst_QImageWriter)

#include "moc_tst_qimagewriter.cpp"
