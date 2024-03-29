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


#include <QtTest/QtTest>

#include <QMimeData>

class tst_QMimeData : public QObject
{
    Q_OBJECT
public:
    tst_QMimeData()
    {
    }

private slots:
    void clear() const;
    void colorData() const;
    void data() const;
    void formats() const;
    void hasColor() const;
    void hasFormat() const;
    void hasHtml() const;
    void hasImage() const;
    // hasText() covered by setText()
    // hasUrls() covered by setUrls()
    // html() covered by setHtml()
    void imageData() const;
    void removeFormat() const;
    // setColorData() covered by hasColor()
    // setData() covered in a few different tests
    void setHtml() const;
    // setImageData() covered in a few tests
    void setText() const;
    void setUrls() const;
    // text() covered in setText()
    // urls() covered by setUrls()
};

void tst_QMimeData::clear() const
{
    QMimeData mimeData;

    // set, clear, verify empty
    mimeData.setData("text/plain", "pirates");
    QVERIFY(mimeData.hasText());
    mimeData.clear();
    QVERIFY(mimeData.hasText() == false);

    // repopulate, verify not empty
    mimeData.setData("text/plain", "pirates");
    QVERIFY(mimeData.hasText());
}

void tst_QMimeData::colorData() const
{
    QMimeData mimeData;
    QColor red = Qt::red;
    QColor blue = Qt::blue;

    // set, verify
    mimeData.setColorData(red);
    QVERIFY(mimeData.hasColor());
    QCOMPARE(qvariant_cast<QColor>(mimeData.colorData()), red);

    // change, verify
    mimeData.setColorData(blue);
    QVERIFY(mimeData.hasColor());
    QCOMPARE(qvariant_cast<QColor>(mimeData.colorData()), blue);
}

void tst_QMimeData::data() const
{
    QMimeData mimeData;

    // set text, verify
    mimeData.setData("text/plain", "pirates");
    QCOMPARE(mimeData.data("text/plain"), QByteArray("pirates"));
    QVERIFY(mimeData.data("text/html").length() == 0);

    // html time
    mimeData.setData("text/html", "ninjas");
    QCOMPARE(mimeData.data("text/html"), QByteArray("ninjas"));
    QCOMPARE(mimeData.data("text/plain"), QByteArray("pirates")); // make sure text not damaged
    QCOMPARE(mimeData.data("text/html"), mimeData.html().toLatin1());
}

void tst_QMimeData::formats() const
{
    QMimeData mimeData;

    // set text, verify
    mimeData.setData("text/plain", "pirates");
    QCOMPARE(mimeData.formats(), QStringList() << "text/plain");

    // set html, verify
    mimeData.setData("text/html", "ninjas");
    QCOMPARE(mimeData.formats(), QStringList() << "text/plain" << "text/html");

    // clear, verify
    mimeData.clear();
    QCOMPARE(mimeData.formats(), QStringList());

    // set an odd format, verify
    mimeData.setData("foo/bar", "somevalue");
    QCOMPARE(mimeData.formats(), QStringList() << "foo/bar");
}

void tst_QMimeData::hasColor() const
{
    QMimeData mimeData;

    // initial state
    QVERIFY(mimeData.hasColor() == false);

    // set, verify
    mimeData.setColorData(QColor(Qt::red));
    QVERIFY(mimeData.hasColor());

    // clear, verify
    mimeData.clear();
    QVERIFY(mimeData.hasColor() == false);

    // set something else, verify
    mimeData.setData("text/plain", "pirates");
    QVERIFY(mimeData.hasColor() == false);
}

void tst_QMimeData::hasFormat() const
{
    QMimeData mimeData;

    // initial state
    QVERIFY(mimeData.hasFormat("text/plain") == false);

    // add, verify
    mimeData.setData("text/plain", "pirates");
    QVERIFY(mimeData.hasFormat("text/plain"));
    QVERIFY(mimeData.hasFormat("text/html") == false);

    // clear, verify
    mimeData.clear();
    QVERIFY(mimeData.hasFormat("text/plain") == false);
    QVERIFY(mimeData.hasFormat("text/html") == false);
}

void tst_QMimeData::hasHtml() const
{
    QMimeData mimeData;

    // initial state
    QVERIFY(mimeData.hasHtml() == false);

    // add plain, verify false
    mimeData.setData("text/plain", "pirates");
    QVERIFY(mimeData.hasHtml() == false);

    // add html, verify
    mimeData.setData("text/html", "ninjas");
    QVERIFY(mimeData.hasHtml());

    // clear, verify
    mimeData.clear();
    QVERIFY(mimeData.hasHtml() == false);

    // readd, verify
    mimeData.setData("text/html", "ninjas");
    QVERIFY(mimeData.hasHtml());
}

void tst_QMimeData::hasImage() const
{
    QMimeData mimeData;

    // initial state
    QVERIFY(mimeData.hasImage() == false);

    // add text, verify false
    mimeData.setData("text/plain", "pirates");
    QVERIFY(mimeData.hasImage() == false);

    // add image
    mimeData.setImageData(QImage());
    QVERIFY(mimeData.hasImage());

    // clear, verify
    mimeData.clear();
    QVERIFY(mimeData.hasImage() == false);
}

void tst_QMimeData::imageData() const
{
    QMimeData mimeData;

    // initial state
    QCOMPARE(mimeData.imageData(), QVariant());

    // set, test
    mimeData.setImageData(QImage());
    QVERIFY(mimeData.hasImage());
    QCOMPARE(mimeData.imageData(), QVariant(QImage()));

    // clear, verify
    mimeData.clear();
    QCOMPARE(mimeData.imageData(), QVariant());
}

void tst_QMimeData::removeFormat() const
{
    QMimeData mimeData;

    // add, verify
    mimeData.setData("text/plain", "pirates");
    QVERIFY(mimeData.hasFormat("text/plain"));

    // add another, verify
    mimeData.setData("text/html", "ninjas");
    QVERIFY(mimeData.hasFormat("text/html"));

    // remove, verify
    mimeData.removeFormat("text/plain");
    QVERIFY(mimeData.hasFormat("text/plain") == false);
    QVERIFY(mimeData.hasFormat("text/html"));

    // remove, verify
    mimeData.removeFormat("text/html");
    QVERIFY(mimeData.hasFormat("text/plain") == false);
    QVERIFY(mimeData.hasFormat("text/html") == false);
}

void tst_QMimeData::setHtml() const
{
    QMimeData mimeData;

    // initial state
    QVERIFY(mimeData.hasHtml() == false);

    // add html, verify
    mimeData.setHtml("ninjas");
    QVERIFY(mimeData.hasHtml());
    QCOMPARE(mimeData.html(), QLatin1String("ninjas"));

    // reset html
    mimeData.setHtml("pirates");
    QVERIFY(mimeData.hasHtml());
    QCOMPARE(mimeData.html(), QLatin1String("pirates"));
}

void tst_QMimeData::setText() const
{
    QMimeData mimeData;

    // verify initial state
    QCOMPARE(mimeData.text(), QLatin1String(""));
    QVERIFY(mimeData.hasText() == false);

    // set, verify
    mimeData.setText("pirates");
    QVERIFY(mimeData.hasText());
    QCOMPARE(mimeData.text(), QLatin1String("pirates"));
    QCOMPARE(mimeData.text().toLatin1(), mimeData.data("text/plain"));

    // reset, verify
    mimeData.setText("ninjas");
    QVERIFY(mimeData.hasText());
    QCOMPARE(mimeData.text(), QLatin1String("ninjas"));
    QCOMPARE(mimeData.text().toLatin1(), mimeData.data("text/plain"));

    // clear, verify
    mimeData.clear();
    QCOMPARE(mimeData.text(), QLatin1String(""));
    QVERIFY(mimeData.hasText() == false);
}

void tst_QMimeData::setUrls() const
{
    QMimeData mimeData;
    QList<QUrl> shortUrlList;
    QList<QUrl> longUrlList;

    // set up
    shortUrlList += QUrl("http://qt.nokia.com");
    longUrlList = shortUrlList;
    longUrlList += QUrl("http://www.google.com");

    // verify initial state
    QVERIFY(mimeData.hasUrls() == false);

    // set a few, verify
    mimeData.setUrls(shortUrlList);
    QCOMPARE(mimeData.urls(), shortUrlList);

    // change them, verify
    mimeData.setUrls(longUrlList);
    QCOMPARE(mimeData.urls(), longUrlList);

    // clear, verify
    mimeData.clear();
    QVERIFY(mimeData.hasUrls() == false);
}

QTEST_MAIN(tst_QMimeData)

#include "moc_tst_qmimedata.cpp"
