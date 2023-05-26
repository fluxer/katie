/****************************************************************************
**
** Copyright (C) 2023 Ivailo Monev
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
#include <QIcon>
#include <QPixmap>
#include <QImageReader>

//TESTED_FILES=

class tst_QIcon : public QObject
{
    Q_OBJECT

public:
    tst_QIcon();
    virtual ~tst_QIcon();

private slots:
    void iconPixmap_data();
    void iconPixmap();

    void iconFromTheme_data();
    void iconFromTheme();

private:
    QList< QPair<QString, QByteArray> > images; // filename, format
};

tst_QIcon::tst_QIcon()
{
    foreach (const QByteArray &format, QImageReader::supportedImageFormats()) {
        const QString benchfilepath = QLatin1String(SRCDIR "/images/bench.") + format;

        if (QFile::exists(benchfilepath)) {
            images << QPair<QString, QByteArray>(benchfilepath, format);
        }
    }
}

tst_QIcon::~tst_QIcon()
{
}

void tst_QIcon::iconPixmap_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QByteArray>("format");

    for (int i = 0; i < images.size(); ++i) {
        const QString file = images[i].first;
        const QByteArray format = images[i].second;
        QTest::newRow(qPrintable(file)) << file << format;
    }
}

void tst_QIcon::iconPixmap()
{
    QFETCH(QString, fileName);
    QFETCH(QByteArray, format);

    const QSize iconSize = QSize(32, 32);
    QBENCHMARK {
        QIcon icon(fileName);
        QPixmap pixmap = icon.pixmap(iconSize);
        QVERIFY(!pixmap.isNull());
    }
}

void tst_QIcon::iconFromTheme_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::newRow("folder") << QString::fromLatin1("folder");
    QTest::newRow("dialog-close") << QString::fromLatin1("dialog-close");
    QTest::newRow("user-home") << QString::fromLatin1("user-home");
}

void tst_QIcon::iconFromTheme()
{
    QFETCH(QString, fileName);

    const QSize iconSize = QSize(32, 32);
    QBENCHMARK {
        QIcon icon = QIcon::fromTheme(fileName);
        QPixmap pixmap = icon.pixmap(iconSize);
        QVERIFY(!pixmap.isNull());
    }
}

QTEST_MAIN(tst_QIcon)

#include "moc_tst_qicon.cpp"
