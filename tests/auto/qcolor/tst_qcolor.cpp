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
#include <qcolor.h>
#include <qcolor_p.h>
#include <qdebug.h>

#include <limits.h>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QColor : public QObject
{
    Q_OBJECT

public:
    tst_QColor();

private slots:
    void getSetCheck();
    void isValid_data();
    void isValid();

    void name_data();
    void name();
    void setNamedColor();

    void constructNamedColorWithSpace();

    void colorNames();

    void spec();

    void globalColors_data();
    void globalColors();

    void alpha();
    void setAlpha();

    void red();
    void green();
    void blue();

    void setRed();
    void setGreen();
    void setBlue();

    void getRgb();
    void setRgb();

    void rgba();
    void setRgba();

    void rgb();

    void hue();
    void saturation();
    void value();

    void getHsv();
    void setHsv();

    void toRgb_data();
    void toRgb();
    void toRgbNonDestructive();

    void toHsv_data();
    void toHsv();
    void toHsvNonDestructive();

    void fromRgb();
    void fromHsv();

    void light();
    void dark();

    void assignmentOoperator();
    void equalityOperator();

#ifdef Q_WS_X11
    void allowX11ColorNames();
    void setallowX11ColorNames();
#endif
};

// Testing get/set functions
void tst_QColor::getSetCheck()
{
    QColor obj1;
    // int QColor::alpha()
    // void QColor::setAlpha(int)
    obj1.setAlpha(0);
    QCOMPARE(obj1.alpha(), 0);
    obj1.setAlpha(-1);
    QCOMPARE(obj1.alpha(), 0); // range<0, 255>
    obj1.setAlpha(INT_MIN);
    QCOMPARE(obj1.alpha(), 0); // range<0, 255>
    obj1.setAlpha(255);
    QCOMPARE(obj1.alpha(), 255); // range<0, 255>
    obj1.setAlpha(INT_MAX);
    QCOMPARE(obj1.alpha(), 255); // range<0, 255>

    // qreal QColor::alphaF()
    // void QColor::setAlphaF(qreal)
    obj1.setAlphaF(0.0);
    QCOMPARE(obj1.alphaF(), qreal(0.0)); // range<0.0, 1.0>
    obj1.setAlphaF(-0.2);
    QCOMPARE(obj1.alphaF(), qreal(0.0)); // range<0.0, 1.0>
    obj1.setAlphaF(1.0);
    QCOMPARE(obj1.alphaF(), qreal(1.0)); // range<0.0, 1.0>
    obj1.setAlphaF(1.1);
    QCOMPARE(obj1.alphaF(), qreal(1.0)); // range<0.0, 1.0>

    // int QColor::red()
    // void QColor::setRed(int)
    obj1.setRed(0);
    QCOMPARE(obj1.red(), 0);
    obj1.setRed(-1);
    QCOMPARE(obj1.red(), 0); // range<0, 255>
    obj1.setRed(INT_MIN);
    QCOMPARE(obj1.red(), 0); // range<0, 255>
    obj1.setRed(255);
    QCOMPARE(obj1.red(), 255); // range<0, 255>
    obj1.setRed(INT_MAX);
    QCOMPARE(obj1.red(), 255); // range<0, 255>

    // int QColor::green()
    // void QColor::setGreen(int)
    obj1.setGreen(0);
    QCOMPARE(obj1.green(), 0);
    obj1.setGreen(-1);
    QCOMPARE(obj1.green(), 0); // range<0, 255>
    obj1.setGreen(INT_MIN);
    QCOMPARE(obj1.green(), 0); // range<0, 255>
    obj1.setGreen(255);
    QCOMPARE(obj1.green(), 255); // range<0, 255>
    obj1.setGreen(INT_MAX);
    QCOMPARE(obj1.green(), 255); // range<0, 255>

    // int QColor::blue()
    // void QColor::setBlue(int)
    obj1.setBlue(0);
    QCOMPARE(obj1.blue(), 0);
    obj1.setBlue(-1);
    QCOMPARE(obj1.blue(), 0); // range<0, 255>
    obj1.setBlue(INT_MIN);
    QCOMPARE(obj1.blue(), 0); // range<0, 255>
    obj1.setBlue(255);
    QCOMPARE(obj1.blue(), 255); // range<0, 255>
    obj1.setBlue(INT_MAX);
    QCOMPARE(obj1.blue(), 255); // range<0, 255>

    // qreal QColor::redF()
    // void QColor::setRedF(qreal)
    obj1.setRedF(0.0);
    QCOMPARE(obj1.redF(), qreal(0.0));
    obj1.setRedF(-0.2);
    QCOMPARE(obj1.redF(), qreal(0.0)); // range<0.0, 1.0
    obj1.setRedF(1.1);
    QCOMPARE(obj1.redF(), qreal(1.0)); // range<0.0, 1.0

    // qreal QColor::greenF()
    // void QColor::setGreenF(qreal)
    obj1.setGreenF(0.0);
    QCOMPARE(obj1.greenF(), qreal(0.0));
    obj1.setGreenF(-0.2);
    QCOMPARE(obj1.greenF(), qreal(0.0)); // range<0.0, 1.0
    obj1.setGreenF(1.1);
    QCOMPARE(obj1.greenF(), qreal(1.0)); // range<0.0, 1.0

    // qreal QColor::blueF()
    // void QColor::setBlueF(qreal)
    obj1.setBlueF(0.0);
    QCOMPARE(obj1.blueF(), qreal(0.0));
    obj1.setBlueF(-0.2);
    QCOMPARE(obj1.blueF(), qreal(0.0)); // range<0.0, 1.0
    obj1.setBlueF(1.1);
    QCOMPARE(obj1.blueF(), qreal(1.0)); // range<0.0, 1.0

    // QRgb QColor::rgba()
    // void QColor::setRgba(QRgb)
    QRgb var9(qRgba(10, 20, 30, 40));
    obj1.setRgba(var9);
    QCOMPARE(obj1.rgba(), var9);
    obj1.setRgba(QRgb(0));
    QCOMPARE(obj1.rgba(), QRgb(0));

    // QRgb QColor::rgb()
    // void QColor::setRgb(QRgb)
    QRgb var10(qRgb(10, 20, 30));
    obj1.setRgb(var10);
    QCOMPARE(obj1.rgb(), var10);
    obj1.setRgb(QRgb(0));
    QCOMPARE(obj1.rgb(), qRgb(0, 0, 0));
}

Q_DECLARE_METATYPE(QColor)


tst_QColor::tst_QColor()
{
}

void tst_QColor::isValid_data()
{
    QTest::addColumn<QColor>("color");
    QTest::addColumn<bool>("isValid");

    QTest::newRow("defaultConstructor") << QColor() << false;
    QTest::newRow("rgbConstructor-valid") << QColor(2,5,7) << true;
    QTest::newRow("rgbConstructor-invalid") << QColor(2,5,999) << false;
    QTest::newRow("nameQStringConstructor-valid") << QColor(QString("#ffffff")) << true;
    QTest::newRow("nameQStringConstructor-invalid") << QColor(QString("#ffffgg")) << false;
    QTest::newRow("nameQStringConstructor-empty") << QColor(QString("")) << false;
#ifndef QT_NO_COLORNAMES
    QTest::newRow("nameQStringConstructor-named") << QColor(QString("red")) << true;
#else
    QTest::newRow("nameQStringConstructor-named") << QColor(QString("red")) << false;
#endif
    QTest::newRow("nameCharConstructor-valid") << QColor("#ffffff") << true;
    QTest::newRow("nameCharConstructor-invalid") << QColor("#ffffgg") << false;
    QTest::newRow("nameCharConstructor-invalid-2") << QColor("#fffffg") << false;
}

void tst_QColor::isValid()
{
    QFETCH(QColor, color);
    QFETCH(bool, isValid);
    QVERIFY(color.isValid() == isValid);
}

void tst_QColor::name_data()
{
    QTest::addColumn<QColor>("color");
    QTest::addColumn<QString>("name");

    QTest::newRow("invalid") << QColor() << "#000000";
    QTest::newRow("global color black") << QColor(Qt::black) << "#000000";
    QTest::newRow("global color white") << QColor(Qt::white) << "#ffffff";
    QTest::newRow("global color darkGray") << QColor(Qt::darkGray) << "#808080";
    QTest::newRow("global color gray") << QColor(Qt::gray) << "#a0a0a4";
    QTest::newRow("global color lightGray") << QColor(Qt::lightGray) << "#c0c0c0";
    QTest::newRow("global color red") << QColor(Qt::red) << "#ff0000";
    QTest::newRow("global color green") << QColor(Qt::green) << "#00ff00";
    QTest::newRow("global color blue") << QColor(Qt::blue) << "#0000ff";
    QTest::newRow("global color cyan") << QColor(Qt::cyan) << "#00ffff";
    QTest::newRow("global color magenta") << QColor(Qt::magenta) << "#ff00ff";
    QTest::newRow("global color yellow") << QColor(Qt::yellow) << "#ffff00";
    QTest::newRow("global color darkRed") << QColor(Qt::darkRed) << "#800000";
    QTest::newRow("global color darkGreen") << QColor(Qt::darkGreen) << "#008000";
    QTest::newRow("global color darkBlue") << QColor(Qt::darkBlue) << "#000080";
    QTest::newRow("global color darkCyan") << QColor(Qt::darkCyan) << "#008080";
    QTest::newRow("global color darkMagenta") << QColor(Qt::darkMagenta) << "#800080";
    QTest::newRow("global color darkYellow") << QColor(Qt::darkYellow) << "#808000";
}

void tst_QColor::name()
{
    QFETCH(QColor, color);
    QFETCH(QString, name);
    QCOMPARE(color.name(), name);
}

void tst_QColor::globalColors_data()
{
    QTest::addColumn<QColor>("color");
    QTest::addColumn<uint>("argb");

    QTest::newRow("invalid") << QColor() << 0xff000000;
    QTest::newRow("global color black") << QColor(Qt::black) << 0xff000000;
    QTest::newRow("global color white") << QColor(Qt::white) << 0xffffffff;
    QTest::newRow("global color darkGray") << QColor(Qt::darkGray) << 0xff808080;
    QTest::newRow("global color gray") << QColor(Qt::gray) << 0xffa0a0a4;
    QTest::newRow("global color lightGray") << QColor(Qt::lightGray) << 0xffc0c0c0;
    QTest::newRow("global color red") << QColor(Qt::red) << 0xffff0000;
    QTest::newRow("global color green") << QColor(Qt::green) << 0xff00ff00;
    QTest::newRow("global color blue") << QColor(Qt::blue) << 0xff0000ff;
    QTest::newRow("global color cyan") << QColor(Qt::cyan) << 0xff00ffff;
    QTest::newRow("global color magenta") << QColor(Qt::magenta) << 0xffff00ff;
    QTest::newRow("global color yellow") << QColor(Qt::yellow) << 0xffffff00;
    QTest::newRow("global color darkRed") << QColor(Qt::darkRed) << 0xff800000;
    QTest::newRow("global color darkGreen") << QColor(Qt::darkGreen) << 0xff008000;
    QTest::newRow("global color darkBlue") << QColor(Qt::darkBlue) << 0xff000080;
    QTest::newRow("global color darkCyan") << QColor(Qt::darkCyan) << 0xff008080;
    QTest::newRow("global color darkMagenta") << QColor(Qt::darkMagenta) << 0xff800080;
    QTest::newRow("global color darkYellow") << QColor(Qt::darkYellow) << 0xff808000;
    QTest::newRow("global color transparent") << QColor(Qt::transparent) << 0x00000000u;
}


void tst_QColor::globalColors()
{
    QFETCH(QColor, color);
    QFETCH(uint, argb);
    QCOMPARE(color.rgba(), argb);
}

void tst_QColor::setNamedColor()
{
    for (int i = 0; i < RGBTblSize; ++i) {
        QColor color;
        color.setNamedColor(QLatin1String(RGBTbl[i].name));
        QColor expected;
        expected.setRgba(RGBTbl[i].value);
        QCOMPARE(color, expected);
    }
}

void tst_QColor::constructNamedColorWithSpace()
{
    QColor whiteSmoke("white smoke");
    QCOMPARE(whiteSmoke, QColor(245, 245, 245));
}

void tst_QColor::colorNames()
{
    DEPENDS_ON("setNamedColor()");

    QStringList all = QColor::colorNames();
    QCOMPARE(all.size(), (int)RGBTblSize);
    for (int i = 0; i < all.size(); ++i)
        QCOMPARE(all.at(i), QString::fromLatin1(RGBTbl[i].name));
}

void tst_QColor::spec()
{
    QColor invalid;
    QCOMPARE(invalid.spec(), QColor::Invalid);

    QColor rgb = QColor::fromRgb(0, 0, 0);
    QCOMPARE(rgb.spec(), QColor::Rgb);

    QColor hsv = QColor::fromHsv(0, 0, 0);
    QCOMPARE(hsv.spec(), QColor::Hsv);

}

void tst_QColor::alpha()
{ DEPENDS_ON(setRgb()); }

void tst_QColor::red()
{ DEPENDS_ON(setRgb()); }

void tst_QColor::green()
{ DEPENDS_ON(setRgb()); }

void tst_QColor::blue()
{ DEPENDS_ON(setRgb()); }

void tst_QColor::getRgb()
{ DEPENDS_ON(setRgb()); }

void tst_QColor::setAlpha()
{ DEPENDS_ON(setRgb()); }

bool veryFuzzyCompare(double a, double b)
{
    return qAbs(a - b) < 0.01;
}

void tst_QColor::setRed()
{
    DEPENDS_ON(setRgb());

    QColor c = QColor(Qt::blue).toHsv();
    c.setRed(127);
    QCOMPARE(c.red(), 127);
    QCOMPARE(c.green(), 0);
    QCOMPARE(c.blue(), 255);

    c = QColor(Qt::blue).toHsv();
    c.setRedF(0.5);
    QVERIFY(veryFuzzyCompare(c.redF(), 0.5));
    QCOMPARE(c.greenF(), qreal(0.0));
    QCOMPARE(c.blueF(), qreal(1.0));
}

void tst_QColor::setGreen()
{
    DEPENDS_ON(setRgb());

    QColor c = QColor(Qt::blue).toHsv();
    c.setGreen(127);
    QCOMPARE(c.red(), 0);
    QCOMPARE(c.green(), 127);
    QCOMPARE(c.blue(), 255);

    c = QColor(Qt::blue).toHsv();
    c.setGreenF(0.5);
    QCOMPARE(c.redF(), qreal(0.0));
    QVERIFY(veryFuzzyCompare(c.greenF(), 0.5));
    QCOMPARE(c.blueF(), qreal(1.0));
}

void tst_QColor::setBlue()
{
    DEPENDS_ON(setRgb());

    QColor c = QColor(Qt::red).toHsv();
    c.setBlue(127);
    QCOMPARE(c.red(), 255);
    QCOMPARE(c.green(), 0);
    QCOMPARE(c.blue(), 127);

    c = QColor(Qt::red).toHsv();
    c.setBlueF(0.5);
    QCOMPARE(c.redF(), qreal(1.0));
    QCOMPARE(c.greenF(), qreal(0.0));
    QVERIFY(veryFuzzyCompare(c.blueF(), 0.5));
}


void tst_QColor::setRgb()
{
    QColor color;

    for (int A = 0; A <= USHRT_MAX; ++A) {
        {
            // 0-255
            int a = A >> 8;
            QRgb rgb = qRgba(0, 0, 0, a);

            color.setRgb(0, 0, 0, a);
            QCOMPARE(color.alpha(), a);
            QCOMPARE(color.rgb(),  qRgb(0, 0, 0));

            color.setRgb(rgb);
            QCOMPARE(color.alpha(), 255);
            QCOMPARE(color.rgb(),   qRgb(0, 0, 0));

            int r, g, b, a2;
            color.setRgb(0, 0, 0, a);
            color.getRgb(&r, &g, &b, &a2);
            QCOMPARE(a2, a);

            QColor c(0, 0, 0);
            c.setAlpha(a);
            QCOMPARE(c.alpha(), a);
        }

        {
            // 0.0-1.0
            qreal a = A / qreal(USHRT_MAX);
            color.setRgbF(0.0, 0.0, 0.0, a);
            QCOMPARE(color.alphaF(), a);

            qreal r, g, b, a2;
            color.getRgbF(&r, &g, &b, &a2);
            QCOMPARE(a2, a);

            QColor c(0, 0, 0);
            c.setAlphaF(a);

            QCOMPARE(c.alphaF(), a);
        }
    }

    for (int R = 0; R <= USHRT_MAX; ++R) {
        {
            // 0-255
            int r = R >> 8;
            QRgb rgb = qRgb(r, 0, 0);

            color.setRgb(r, 0, 0);
            QCOMPARE(color.red(), r);
            QCOMPARE(color.rgb(), rgb);

            color.setRgb(rgb);
            QCOMPARE(color.red(), r);
            QCOMPARE(color.rgb(), rgb);

            int r2, g, b, a;
            color.getRgb(&r2, &g, &b, &a);
            QCOMPARE(r2, r);
        }

        {
            // 0.0-1.0
            qreal r = R / qreal(USHRT_MAX);
            color.setRgbF(r, 0.0, 0.0);
            QCOMPARE(color.redF(), r);

            qreal r2, g, b, a;
            color.getRgbF(&r2, &g, &b, &a);
            QCOMPARE(r2, r);
        }
    }

    for (int G = 0; G <= USHRT_MAX; ++G) {
        {
            // 0-255
            int g = G >> 8;
            QRgb rgb = qRgb(0, g, 0);

            color.setRgb(0, g, 0);
            QCOMPARE(color.green(), g);
            QCOMPARE(color.rgb(),   rgb);

            color.setRgb(rgb);
            QCOMPARE(color.green(), g);
            QCOMPARE(color.rgb(),   rgb);

            int r, g2, b, a;
            color.getRgb(&r, &g2, &b, &a);
            QCOMPARE(g2, g);
        }

        {
            // 0.0-1.0
            qreal g = G / qreal(USHRT_MAX);
            color.setRgbF(0.0, g, 0.0);
            QCOMPARE(color.greenF(), g);

            qreal r, g2, b, a;
            color.getRgbF(&r, &g2, &b, &a);
            QCOMPARE(g2, g);
        }
    }

    for (int B = 0; B <= USHRT_MAX; ++B) {
        {
            // 0-255
            int b = B >> 8;
            QRgb rgb = qRgb(0, 0, b);

            color.setRgb(0, 0, b);
            QCOMPARE(color.blue(),  b);
            QCOMPARE(color.rgb(),   rgb);

            color.setRgb(rgb);
            QCOMPARE(color.blue(),  b);
            QCOMPARE(color.rgb(),   rgb);

            int r, g, b2, a;
            color.getRgb(&r, &g, &b2, &a);
            QCOMPARE(b2, b);
        }

        {
            // 0.0-1.0
            qreal b = B / qreal(USHRT_MAX);
            color.setRgbF(0.0, 0.0, b);
            QCOMPARE(color.blueF(), b);

            qreal r, g, b2, a;
            color.getRgbF(&r, &g, &b2, &a);
            QCOMPARE(b2, b);
        }
    }
}

void tst_QColor::rgba()
{ DEPENDS_ON("setRgba()"); }

void tst_QColor::setRgba()
{
    for (int a = 0; a < 255; ++a) {
        const QRgb rgba1 = qRgba(a, a, a, a);
        QColor color;
        color.setRgba(rgba1);
        QCOMPARE(color.alpha(), a);
        const QRgb rgba2 = color.rgba();
        QCOMPARE(rgba2, rgba1);
        QCOMPARE(qAlpha(rgba2), a);
    }
}

void tst_QColor::rgb()
{ DEPENDS_ON(setRgb()); }

void tst_QColor::hue()
{ DEPENDS_ON(setHsv()); }

void tst_QColor::saturation()
{ DEPENDS_ON(setHsv()); }

void tst_QColor::value()
{ DEPENDS_ON(setHsv()); }

void tst_QColor::getHsv()
{ DEPENDS_ON(setHsv()); }

void tst_QColor::setHsv()
{
    QColor color;

    for (int A = 0; A <= USHRT_MAX; ++A) {
        {
            // 0-255
            int a = A >> 8;
            color.setHsv(0, 0, 0, a);
            QCOMPARE(color.alpha(), a);

            int h, s, v, a2;
            color.getHsv(&h, &s, &v, &a2);
            QCOMPARE(a2, a);
        }

        {
            // 0.0-1.0
            qreal a = A / qreal(USHRT_MAX);
            color.setHsvF(0.0, 0.0, 0.0, a); QCOMPARE(color.alphaF(), a);

            qreal h, s, v, a2;
            color.getHsvF(&h, &s, &v, &a2);
            QCOMPARE(a2, a);
        }
    }

    for (int H = 0; H < 36000; ++H) {
        {
            // 0-255
            int h = H / 100;

            color.setHsv(h, 0, 0, 0);
            QCOMPARE(color.hue(), h);

            int h2, s, v, a;
            color.getHsv(&h2, &s, &v, &a);
            QCOMPARE(h2, h);
        }

        {
            // 0.0-1.0
            qreal h = H / 36000.0;
            color.setHsvF(h, 0.0, 0.0, 0.0);
            QCOMPARE(color.hueF(), h);

            qreal h2, s, v, a;
            color.getHsvF(&h2, &s, &v, &a);
            QCOMPARE(h2, h);
        }
    }

    for (int S = 0; S <= USHRT_MAX; ++S) {
        {
            // 0-255
            int s = S >> 8;
            color.setHsv(0, s, 0, 0);
            QCOMPARE(color.saturation(), s);

            int h, s2, v, a;
            color.getHsv(&h, &s2, &v, &a);
            QCOMPARE(s2, s);
        }

        {
            // 0.0-1.0
            qreal s = S / qreal(USHRT_MAX);
            color.setHsvF(0.0, s, 0.0, 0.0);
            QCOMPARE(color.saturationF(), s);

            qreal h, s2, v, a;
            color.getHsvF(&h, &s2, &v, &a);
            QCOMPARE(s2, s);
        }
    }

    for (int V = 0; V <= USHRT_MAX; ++V) {
        {
            // 0-255
            int v = V >> 8;
            color.setHsv(0, 0, v, 0);
            QCOMPARE(color.value(),  v);

            int h, s, v2, a;
            color.getHsv(&h, &s, &v2, &a);
            QCOMPARE(v2, v);
        }

        {
            // 0.0-1.0
            qreal v = V / qreal(USHRT_MAX);
            color.setHsvF(0.0, 0.0, v, 0.0);
            QCOMPARE(color.valueF(), v);

            qreal h, s, v2, a;
            color.getHsvF(&h, &s, &v2, &a);
            QCOMPARE(v2, v);
        }
    }
}

void tst_QColor::toRgb_data()
{
    QTest::addColumn<QColor>("expectedColor");
    QTest::addColumn<QColor>("hsvColor");

    QTest::newRow("black")
        << QColor::fromRgbF(0.0, 0.0, 0.0)
        << QColor::fromHsvF(-1.0, 0.0, 0.0);

    QTest::newRow("white")
        << QColor::fromRgbF(1.0, 1.0, 1.0)
        << QColor::fromHsvF(-1.0, 0.0, 1.0);

    QTest::newRow("red")
        << QColor::fromRgbF(1.0, 0.0, 0.0)
        << QColor::fromHsvF(0.0, 1.0, 1.0);

    QTest::newRow("green")
        << QColor::fromRgbF(0.0, 1.0, 0.0)
        << QColor::fromHsvF(0.33333, 1.0, 1.0);

    QTest::newRow("blue")
        << QColor::fromRgbF(0.0, 0.0, 1.0)
        << QColor::fromHsvF(0.66667, 1.0, 1.0);

    QTest::newRow("cyan")
        << QColor::fromRgbF(0.0, 1.0, 1.0)
        << QColor::fromHsvF(0.5, 1.0, 1.0);

    QTest::newRow("magenta")
        << QColor::fromRgbF(1.0, 0.0, 1.0)
        << QColor::fromHsvF(0.83333, 1.0, 1.0);

    QTest::newRow("yellow")
        << QColor::fromRgbF(1.0, 1.0, 0.0)
        << QColor::fromHsvF(0.16667, 1.0, 1.0);

    QTest::newRow("gray")
        << QColor::fromRgbF(0.6431375, 0.6431375, 0.6431375)
        << QColor::fromHsvF(-1.0, 0.0, 0.6431375);

    // ### add colors using the 0-255 functions
}

void tst_QColor::toRgb()
{
    // invalid should remain invalid
    QVERIFY(!QColor().toRgb().isValid());

    QFETCH(QColor, expectedColor);
    QFETCH(QColor, hsvColor);
    QCOMPARE(hsvColor.toRgb(), expectedColor);

}

void tst_QColor::toHsv_data()
{
    QTest::addColumn<QColor>("expectedColor");
    QTest::addColumn<QColor>("rgbColor");

    QTest::newRow("data0")
        << QColor::fromHsv(300, 255, 255)
        << QColor(255, 0, 255);

    QTest::newRow("data1")
        << QColor::fromHsvF(1., 1., 1., 1.)
        << QColor(255, 0, 0, 255);
}

void tst_QColor::toRgbNonDestructive()
{
    QColor aColor = QColor::fromRgbF(0.11, 0.22, 0.33, 0.44);
    QCOMPARE(aColor, aColor.toRgb());
}

void tst_QColor::toHsv()
{
    // invalid should remain invalid
    QVERIFY(!QColor().toHsv().isValid());

    QFETCH(QColor, expectedColor);
    QFETCH(QColor, rgbColor);
    QCOMPARE(rgbColor.toHsv(), expectedColor);
}

void tst_QColor::toHsvNonDestructive()
{
    QColor aColor = QColor::fromHsvF(0.11, 0.22, 0.33, 0.44);
    QCOMPARE(aColor, aColor.toHsv());
}

void tst_QColor::fromRgb()
{ DEPENDS_ON(convertTo()); }

void tst_QColor::fromHsv()
{ DEPENDS_ON(convertTo()); }

void tst_QColor::light()
{
    QColor gray(Qt::gray);
    QColor lighter = gray.light();
    QVERIFY(lighter.value() > gray.value());
}

void tst_QColor::dark()
{
    QColor gray(Qt::gray);
    QColor darker = gray.dark();
    QVERIFY(darker.value() < gray.value());
}

void tst_QColor::assignmentOoperator()
{ DEPENDS_ON(convertTo()); }

void tst_QColor::equalityOperator()
{ DEPENDS_ON(convertTo()); }

Q_DECLARE_METATYPE(QColor::Spec);

#ifdef Q_WS_X11
void tst_QColor::allowX11ColorNames()
{
    DEPENDS_ON(setallowX11ColorNames());
}

void tst_QColor::setallowX11ColorNames()
{
    RGBData x11RgbTbl[] = {
        // a few standard X11 color names
        { "DodgerBlue1", qRgb(30, 144, 255) },
        { "DodgerBlue2", qRgb(28, 134, 238) },
        { "DodgerBlue3", qRgb(24, 116, 205) },
        { "DodgerBlue4", qRgb(16, 78, 139) },
        { "SteelBlue1", qRgb(99, 184, 255) },
        { "SteelBlue2", qRgb(92, 172, 238) },
        { "SteelBlue3", qRgb(79, 148, 205) },
        { "SteelBlue4", qRgb(54, 100, 139) },
        { "DeepSkyBlue1", qRgb(0, 191, 255) },
        { "DeepSkyBlue2", qRgb(0, 178, 238) },
        { "DeepSkyBlue3", qRgb(0, 154, 205) },
        { "DeepSkyBlue4", qRgb(0, 104, 139) },
        { "SkyBlue1", qRgb(135, 206, 255) },
        { "SkyBlue2", qRgb(126, 192, 238) },
        { "SkyBlue3", qRgb(108, 166, 205) },
        { "SkyBlue4", qRgb(74, 112, 139) }
    };
    static const int x11RgbTblSize = sizeof(x11RgbTbl) / sizeof(RGBData);

    // X11 color names should not work by default
    QVERIFY(!QColor::allowX11ColorNames());
    for (int i = 0; i < x11RgbTblSize; ++i) {
        QString colorName = QLatin1String(x11RgbTbl[i].name);
        QColor color;
        color.setNamedColor(colorName);
        QVERIFY(!color.isValid());
    }

    // enable X11 color names
    QColor::setAllowX11ColorNames(true);
    QVERIFY(QColor::allowX11ColorNames());
    for (int i = 0; i < x11RgbTblSize; ++i) {
        QString colorName = QLatin1String(x11RgbTbl[i].name);
        QColor color;
        color.setNamedColor(colorName);
        QColor expected(x11RgbTbl[i].value);
        QCOMPARE(color, expected);
    }

    // should be able to turn off X11 color names
    QColor::setAllowX11ColorNames(false);
    QVERIFY(!QColor::allowX11ColorNames());
    for (int i = 0; i < x11RgbTblSize; ++i) {
        QString colorName = QLatin1String(x11RgbTbl[i].name);
        QColor color;
        color.setNamedColor(colorName);
        QVERIFY(!color.isValid());
    }
}
#endif // Q_WS_X11

QTEST_MAIN(tst_QColor)

#include "moc_tst_qcolor.cpp"
