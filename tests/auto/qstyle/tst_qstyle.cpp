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
#include <qlayout.h>
#include <qstyle.h>
#include <qevent.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qstyleoption.h>
#include <qscrollbar.h>
#include <qprogressbar.h>
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qplastiquestyle.h>
#include <qwindowsstyle.h>
#include <qmotifstyle.h>
#include <qcommonstyle.h>
#include <qproxystyle.h>
#include <qstylefactory.h>
#include <qimagereader.h>
#include <qimagewriter.h>
#include <qmenu.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qmdiarea.h>
#include <qwidget.h>
#include <qdebug.h>
#include <qcleanlooksstyle.h>

//TESTED_CLASS=
//TESTED_FILES=gui/styles/qstyle.h gui/styles/qstyle.cpp gui/styles/qplastiquestyle.cpp gui/styles/qwindowsstyle.cpp gui/styles/qwindowsxpstyle.cpp gui/styles/qwindowsvistastyle.cpp gui/styles/qmotifstyle.cpp gui/styles/qs60style.cpp

// Make a widget frameless to prevent size constraints of title bars
// from interfering (Windows).
static inline void setFrameless(QWidget *w)
{
    Qt::WindowFlags flags = w->windowFlags();
    flags |= Qt::FramelessWindowHint;
    flags &= ~(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    w->setWindowFlags(flags);
}

class tst_QStyle : public QObject
{
    Q_OBJECT
public:
    tst_QStyle();
    virtual ~tst_QStyle();
private:
    void testAllFunctions(QStyle *);
    void testScrollBarSubControls(QStyle *);
private slots:
    void drawItemPixmap();
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testMotifStyle();
    void testPlastiqueStyle();
    void testWindowsStyle();
    void testCleanlooksStyle();
    void testStyleFactory();
    void testProxyStyle();
    void pixelMetric();
    void progressBarChangeStyle();
    void defaultFont();
    void testDrawingShortcuts();
private:
    void lineUpLayoutTest(QStyle *);
    QWidget *testWidget;
};


tst_QStyle::tst_QStyle()
{
    testWidget = 0;
}

tst_QStyle::~tst_QStyle()
{
}

class MyWidget : public QWidget
{
public:
    MyWidget( QWidget* QWidget=0, const char* name=0 );
protected:
    void paintEvent( QPaintEvent* );
};

void tst_QStyle::init()
{
    testWidget = new MyWidget( 0, "testObject");
}

void tst_QStyle::cleanup()
{
    delete testWidget;
    testWidget = 0;
}

void tst_QStyle::initTestCase()
{
}

void tst_QStyle::cleanupTestCase()
{
}

void tst_QStyle::testStyleFactory()
{
    QStringList keys = QStyleFactory::keys();
#ifndef QT_NO_STYLE_MOTIF
    QVERIFY(keys.contains("Motif"));
#endif
#ifndef QT_NO_STYLE_CLEANLOOKS
    QVERIFY(keys.contains("Cleanlooks"));
#endif
#ifndef QT_NO_STYLE_PLASTIQUE
    QVERIFY(keys.contains("Plastique"));
#endif
#ifndef QT_NO_STYLE_WINDOWS
    QVERIFY(keys.contains("Windows"));
#endif

    foreach (QString styleName , keys) {
        QStyle *style = QStyleFactory::create(styleName);
        QVERIFY2(style != 0, qPrintable(QString::fromLatin1("Fail to load style '%1'").arg(styleName)));
        delete style;
    }
}

class CustomProxy : public QProxyStyle
{
    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option = 0,
                            const QWidget *widget = 0) const
    {
        if (metric == QStyle::PM_ButtonIconSize)
            return 13;
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
};

void tst_QStyle::testProxyStyle()
{
    QProxyStyle *proxyStyle = new QProxyStyle();
    QVERIFY(proxyStyle->baseStyle());
    QStyle *style = new QWindowsStyle;
    QVERIFY(style->proxy() == style);

    proxyStyle->setBaseStyle(style);
    QVERIFY(style->proxy() == proxyStyle);
    QVERIFY(style->parent() == proxyStyle);
    QVERIFY(proxyStyle->baseStyle() == style);

    testAllFunctions(proxyStyle);
    proxyStyle->setBaseStyle(0);
    QVERIFY(proxyStyle->baseStyle());
    qApp->setStyle(proxyStyle);

    QProxyStyle doubleProxy(new QProxyStyle(new QWindowsStyle()));
    testAllFunctions(&doubleProxy);

    CustomProxy customStyle;
    QLineEdit edit;
    edit.setStyle(&customStyle);
    QVERIFY(!customStyle.parent());
    QVERIFY(edit.style()->pixelMetric(QStyle::PM_ButtonIconSize) == 13);
}

void tst_QStyle::drawItemPixmap()
{
    testWidget->resize(300, 300);
    testWidget->show();

    QPixmap p(QString(SRCDIR) + "/task_25863.png", "PNG");
    QPixmap actualPix = QPixmap::grabWidget(testWidget);

    QCOMPARE(actualPix, p);
    testWidget->hide();
}

void tst_QStyle::testAllFunctions(QStyle *style)
{
    QStyleOption opt;
    opt.init(testWidget);

    testWidget->setStyle(style);

    //Tests styleHint with default arguments for potential crashes
    for ( int hint = 0 ; hint < int(QStyle::SH_Menu_Mask); ++hint) {
        style->styleHint(QStyle::StyleHint(hint));
        style->styleHint(QStyle::StyleHint(hint), &opt, testWidget);
    }

    //Tests pixelMetric with default arguments for potential crashes
    for ( int pm = 0 ; pm < int(QStyle::PM_LayoutVerticalSpacing); ++pm) {
        style->pixelMetric(QStyle::PixelMetric(pm));
        style->pixelMetric(QStyle::PixelMetric(pm), &opt, testWidget);
    }

    //Tests drawControl with default arguments for potential crashes
    for ( int control = 0 ; control < int(QStyle::CE_ColumnViewGrip); ++control) {
        QPixmap surface(QSize(200, 200));
        QPainter painter(&surface);
        style->drawControl(QStyle::ControlElement(control), &opt, &painter, 0);
    }

    //Tests drawComplexControl with default arguments for potential crashes
    {
        QPixmap surface(QSize(200, 200));
        QPainter painter(&surface);
        QStyleOptionComboBox copt1;
        copt1.init(testWidget);

        QStyleOptionGroupBox copt2;
        copt2.init(testWidget);
        QStyleOptionSizeGrip copt3;
        copt3.init(testWidget);
        QStyleOptionSlider copt4;
        copt4.init(testWidget);
        copt4.minimum = 0;
        copt4.maximum = 100;
        copt4.tickInterval = 25;
        copt4.sliderValue = 50;
        QStyleOptionSpinBox copt5;
        copt5.init(testWidget);
        QStyleOptionTitleBar copt6;
        copt6.init(testWidget);
        QStyleOptionToolButton copt7;
        copt7.init(testWidget);
        QStyleOptionComplex copt8;
        copt8.initFrom(testWidget);

        style->drawComplexControl(QStyle::CC_SpinBox, &copt5, &painter, 0);
        style->drawComplexControl(QStyle::CC_ComboBox, &copt1, &painter, 0);
        style->drawComplexControl(QStyle::CC_ScrollBar, &copt4, &painter, 0);
        style->drawComplexControl(QStyle::CC_Slider, &copt4, &painter, 0);
        style->drawComplexControl(QStyle::CC_ToolButton, &copt7, &painter, 0);
        style->drawComplexControl(QStyle::CC_TitleBar, &copt6, &painter, 0);
        style->drawComplexControl(QStyle::CC_GroupBox, &copt2, &painter, 0);
        style->drawComplexControl(QStyle::CC_Dial, &copt4, &painter, 0);
    }

    //Check standard pixmaps/icons
    for ( int i = 0 ; i < int(QStyle::SP_ToolBarVerticalExtensionButton); ++i) {
        QPixmap pixmap = style->standardPixmap(QStyle::StandardPixmap(i));
        if (pixmap.isNull()) {
            qWarning("missing StandardPixmap: %d", i);
        }
        QIcon icn = style->standardIcon(QStyle::StandardPixmap(i));
        if (icn.isNull()) {
            qWarning("missing StandardIcon: %d", i);
        }
    }

    style->itemPixmapRect(QRect(0, 0, 100, 100), Qt::AlignHCenter, QPixmap(200, 200));
    style->itemTextRect(QFontMetrics(qApp->font()), QRect(0, 0, 100, 100), Qt::AlignHCenter, true, QString("Test"));

    testScrollBarSubControls(style);
}

void tst_QStyle::testScrollBarSubControls(QStyle* style)
{
    QScrollBar scrollBar;
    setFrameless(&scrollBar);
    scrollBar.show();
    const QStyleOptionSlider opt = qt_qscrollbarStyleOption(&scrollBar);
    const QList<int> subControls = QList<int>() << 1 << 2 << 4 << 8;
    foreach (int subControl, subControls) {
        QRect sr = testWidget->style()->subControlRect(QStyle::CC_ScrollBar, &opt,
                                    QStyle::SubControl(subControl), &scrollBar);
        QVERIFY(sr.isNull() == false);
    }
}

void tst_QStyle::testPlastiqueStyle()
{
#if !defined(QT_NO_STYLE_PLASTIQUE)
    QPlastiqueStyle pstyle;
    testAllFunctions(&pstyle);
    lineUpLayoutTest(&pstyle);
#else
    QSKIP("No Plastique style", SkipAll);
#endif
}

void tst_QStyle::testCleanlooksStyle()
{
#if !defined(QT_NO_STYLE_CLEANLOOKS)
    QCleanlooksStyle cstyle;
    testAllFunctions(&cstyle);
    lineUpLayoutTest(&cstyle);
#else
    QSKIP("No Cleanlooks style", SkipAll);
#endif
}

void tst_QStyle::testWindowsStyle()
{
    QWindowsStyle wstyle;
    testAllFunctions(&wstyle);
    lineUpLayoutTest(&wstyle);

    // Tests drawing indeterminate progress with 0 size: QTBUG-15973
    QStyleOptionProgressBar pb;
    pb.rect = QRect(0,0,-9,0);
    QPixmap surface(QSize(200, 200));
    QPainter painter(&surface);
    wstyle.drawControl(QStyle::CE_ProgressBar, &pb, &painter, 0);
}

void writeImage(const QString &fileName, QImage image)
{
    QImageWriter imageWriter(fileName);
    imageWriter.setFormat("png");
    qDebug() << "result " << imageWriter.write(image);
}

QImage readImage(const QString &fileName)
{
    QImageReader reader(fileName);
    return reader.read();
}

void comparePixmap(const QString &filename, const QPixmap &pixmap)
{
    QImage oldFile = readImage(filename);
    QPixmap oldPixmap = QPixmap::fromImage(oldFile);
    if (!oldFile.isNull())
        QCOMPARE(pixmap, oldPixmap);
    else
        writeImage(filename, pixmap.toImage());
}

void tst_QStyle::testMotifStyle()
{
#if !defined(QT_NO_STYLE_MOTIF)
    QMotifStyle mstyle;
    testAllFunctions(&mstyle);
#else
    QSKIP("No Motif style", SkipAll);
#endif
}

// Helper class...

MyWidget::MyWidget( QWidget* parent, const char* name )
    : QWidget( parent )
{
    setObjectName(name);
}

void MyWidget::paintEvent( QPaintEvent* )
{
    QPainter p(this);
    QPixmap big(400,400);
    big.fill(Qt::green);
    style()->drawItemPixmap(&p, rect(), Qt::AlignCenter, big);
}


class Qt42Style : public QWindowsStyle
{
    Q_OBJECT
public:
    Qt42Style() : QWindowsStyle()
    {
        margin_toplevel = 10;
        margin = 5;
        spacing = 0;
    }

    virtual int pixelMetric(PixelMetric metric, const QStyleOption * option = 0,
                            const QWidget * widget = 0 ) const;

    int margin_toplevel;
    int margin;
    int spacing;

};

int Qt42Style::pixelMetric(PixelMetric metric, const QStyleOption * option /*= 0*/,
                                   const QWidget * widget /*= 0*/ ) const
{
    switch (metric) {
        case QStyle::PM_DefaultTopLevelMargin:
            return margin_toplevel;
        break;
        case QStyle::PM_DefaultChildMargin:
            return margin;
        break;
        case QStyle::PM_DefaultLayoutSpacing:
            return spacing;
        break;
        default:
            break;
    }
    return QWindowsStyle::pixelMetric(metric, option, widget);
}


void tst_QStyle::pixelMetric()
{
    Qt42Style *style = new Qt42Style();
    QCOMPARE(style->pixelMetric(QStyle::PM_DefaultTopLevelMargin), 10);
    QCOMPARE(style->pixelMetric(QStyle::PM_DefaultChildMargin), 5);
    QCOMPARE(style->pixelMetric(QStyle::PM_DefaultLayoutSpacing), 0);

    style->margin_toplevel = 0;
    style->margin = 0;
    style->spacing = 0;
    QCOMPARE(style->pixelMetric(QStyle::PM_DefaultTopLevelMargin), 0);
    QCOMPARE(style->pixelMetric(QStyle::PM_DefaultChildMargin), 0);
    QCOMPARE(style->pixelMetric(QStyle::PM_DefaultLayoutSpacing), 0);

    style->margin_toplevel = -1;
    style->margin = -1;
    style->spacing = -1;
    QCOMPARE(style->pixelMetric(QStyle::PM_DefaultTopLevelMargin), -1);
    QCOMPARE(style->pixelMetric(QStyle::PM_DefaultChildMargin), -1);
    QCOMPARE(style->pixelMetric(QStyle::PM_DefaultLayoutSpacing), -1);

    delete style;
}

void tst_QStyle::progressBarChangeStyle()
{
#if !defined(QT_NO_STYLE_PLASTIQUE) && !defined(QT_NO_STYLE_WINDOWS)
    //test a crashing situation (task 143530)
    //where changing the styles and deleting a progressbar would crash

    QWindowsStyle style1;
    QPlastiqueStyle style2;

    QProgressBar *progress=new QProgressBar;
    progress->setStyle(&style1);

    progress->show();

    progress->setStyle(&style2);

    QTest::qWait(100);
    delete progress;

    QTest::qWait(100);

    //before the correction, there would be a crash here
#elif !defined(QT_NO_STYLE_S60) && !defined(QT_NO_STYLE_WINDOWS)
    //test a crashing situation (task 143530)
    //where changing the styles and deleting a progressbar would crash

    QWindowsStyle style1;
    QS60Style style2;

    QProgressBar *progress=new QProgressBar;
    progress->setStyle(&style1);

    progress->show();

    progress->setStyle(&style2);

    QTest::qWait(100);
    delete progress;

    QTest::qWait(100);

    //before the correction, there would be a crash here
#else
    QSKIP("Either style Plastique or Windows or S60 missing", SkipAll);
#endif
}

void tst_QStyle::lineUpLayoutTest(QStyle *style)
{
    QWidget widget;
    setFrameless(&widget);
    QHBoxLayout layout;
    QFont font;
    font.setPointSize(9); //Plastique is lined up for odd numbers...
    widget.setFont(font);
    QSpinBox spinbox(&widget);
    QLineEdit lineedit(&widget);
    QComboBox combo(&widget);
    combo.setEditable(true);
    layout.addWidget(&spinbox);
    layout.addWidget(&lineedit);
    layout.addWidget(&combo);
    widget.setLayout(&layout);
        widget.setStyle(style);
        // propagate the style.
        foreach (QWidget *w, widget.findChildren<QWidget *>())
            w->setStyle(style);
    widget.show();
    QVERIFY(QTest::qWaitForWindowShown(&widget));

    const int limit = 1;
    const int slDiff = qAbs(spinbox.height() - lineedit.height());
    const int scDiff = qAbs(spinbox.height() - combo.height());
    QVERIFY2(slDiff <= limit,
             qPrintable(QString::fromLatin1("%1 exceeds %2 for %3")
                        .arg(slDiff).arg(limit).arg(style->objectName())));
    QVERIFY2(scDiff <= limit,
             qPrintable(QString::fromLatin1("%1 exceeds %2 for %3")
                        .arg(scDiff).arg(limit).arg(style->objectName())));
}

void tst_QStyle::defaultFont()
{
    QFont defaultFont = qApp->font();
    QFont pointFont = defaultFont;
    pointFont.setPixelSize(9);
    qApp->setFont(pointFont);
    QPushButton button;
    setFrameless(&button);
    button.show();
    qApp->processEvents();
    qApp->setFont(defaultFont);
}

class DrawTextStyle : public QProxyStyle
{
    Q_OBJECT
public:
    DrawTextStyle(QStyle *base = 0) : QProxyStyle(), alignment(0) { setBaseStyle(base); }
    void drawItemText(QPainter *painter, const QRect &rect,
                              int flags, const QPalette &pal, bool enabled,
                              const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const
    {
        DrawTextStyle *that = (DrawTextStyle *)this;
        that->alignment = flags;
        QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
    }
    int alignment;
};

void tst_QStyle::testDrawingShortcuts()
{
    {   
        QWidget w;
        setFrameless(&w);
        QToolButton *tb = new QToolButton(&w);
        tb->setText("&abc");
        DrawTextStyle *dts = new DrawTextStyle;
        w.show();
        tb->setStyle(dts);
        QPixmap::grabWidget(tb);
        QStyleOptionToolButton sotb;
        sotb.initFrom(tb);
        bool showMnemonic = dts->styleHint(QStyle::SH_UnderlineShortcut, &sotb, tb);
        QVERIFY(dts->alignment & (showMnemonic ? Qt::TextShowMnemonic : Qt::TextHideMnemonic));
        delete dts;
    }
    {
        QToolBar w;
        setFrameless(&w);
        QToolButton *tb = new QToolButton(&w);
        tb->setText("&abc");
        DrawTextStyle *dts = new DrawTextStyle;
        w.addWidget(tb);
        w.show();
        tb->setStyle(dts);
        QPixmap::grabWidget(tb);
        QStyleOptionToolButton sotb;
        sotb.initFrom(tb);
        bool showMnemonic = dts->styleHint(QStyle::SH_UnderlineShortcut, &sotb, tb);
        QVERIFY(dts->alignment & (showMnemonic ? Qt::TextShowMnemonic : Qt::TextHideMnemonic));
        delete dts;
     }   
}

QTEST_MAIN(tst_QStyle)

#include "moc_tst_qstyle.cpp"
