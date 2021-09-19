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
#include <QtGui/QDesktopWidget>
#include <QDebug>

//TESTED_CLASS=

class tst_QDesktopWidget : public QObject
{
    Q_OBJECT

public:
    tst_QDesktopWidget();
    virtual ~tst_QDesktopWidget();

public slots:
    void init();
    void cleanup();

private slots:
    void screenCount();
    void primaryScreen();
    void screenNumberForQWidget();
    void screenNumberForQPoint();
    void availableGeometry();
    void screenGeometry();
};

tst_QDesktopWidget::tst_QDesktopWidget()
{
}

tst_QDesktopWidget::~tst_QDesktopWidget()
{
}

void tst_QDesktopWidget::init()
{
}

void tst_QDesktopWidget::cleanup()
{
}

void tst_QDesktopWidget::screenCount()
{
    QDesktopWidget desktop;
    QVERIFY(desktop.screenCount() > 0);
}

void tst_QDesktopWidget::primaryScreen()
{
    QDesktopWidget desktop;
    QVERIFY(desktop.primaryScreen() >= 0);
    QVERIFY(desktop.primaryScreen() < desktop.screenCount());
}

void tst_QDesktopWidget::availableGeometry()
{
    QDesktopWidget desktop;
    QTest::ignoreMessage(QtWarningMsg, "QDesktopWidget::availableGeometry(): Attempt "
                                       "to get the available geometry of a null widget");
    desktop.availableGeometry((QWidget *)0);

    QRect total;
    QRect available;

    for (int i = 0; i < desktop.screenCount(); ++i) {
        total = desktop.screenGeometry(i);
        available = desktop.availableGeometry(i);
        QVERIFY(total.contains(available));
    }

    total = desktop.screenGeometry();
    available = desktop.availableGeometry();
    QVERIFY(total.contains(available));
    QCOMPARE(desktop.availableGeometry(desktop.primaryScreen()), available);
    QCOMPARE(desktop.screenGeometry(desktop.primaryScreen()), total);
}

void tst_QDesktopWidget::screenNumberForQWidget()
{
    QDesktopWidget desktop;

    QCOMPARE(desktop.screenNumber(0), 0);

    QWidget widget;
    widget.show();
    QTest::qWaitForWindowShown(&widget);
    QVERIFY(widget.isVisible());

    int widgetScreen = desktop.screenNumber(&widget);
    QVERIFY(widgetScreen > -1);
    QVERIFY(widgetScreen < desktop.screenCount());
}

void tst_QDesktopWidget::screenNumberForQPoint()
{
    // make sure QDesktopWidget::screenNumber(QPoint) returns the correct screen
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect allScreens;
    for (int i = 0; i < desktopWidget->screenCount(); ++i) {
        QRect screenGeometry = desktopWidget->screenGeometry(i);
        QCOMPARE(desktopWidget->screenNumber(screenGeometry.center()), i);
        allScreens |= screenGeometry;
    }

    // make sure QDesktopWidget::screenNumber(QPoint) returns a valid screen for points that aren't on any screen
    int screen;
    screen = desktopWidget->screenNumber(allScreens.topLeft() - QPoint(1, 1));

    QVERIFY(screen >= 0 && screen < desktopWidget->screenCount());
    screen = desktopWidget->screenNumber(allScreens.topRight() + QPoint(1, 1));
    QVERIFY(screen >= 0 && screen < desktopWidget->screenCount());
    screen = desktopWidget->screenNumber(allScreens.bottomLeft() - QPoint(1, 1));
    QVERIFY(screen >= 0 && screen < desktopWidget->screenCount());
    screen = desktopWidget->screenNumber(allScreens.bottomRight() + QPoint(1, 1));
    QVERIFY(screen >= 0 && screen < desktopWidget->screenCount());
}

void tst_QDesktopWidget::screenGeometry()
{
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QTest::ignoreMessage(QtWarningMsg, "QDesktopWidget::screenGeometry(): Attempt "
                                       "to get the screen geometry of a null widget");
    QRect r = desktopWidget->screenGeometry((QWidget *)0);
    QVERIFY(r.isNull());
    QWidget widget;
    widget.show();
    QTest::qWaitForWindowShown(&widget);
    r = desktopWidget->screenGeometry(&widget);

    QRect total;
    QRect available;
    for (int i = 0; i < desktopWidget->screenCount(); ++i) {
        total = desktopWidget->screenGeometry(i);
        available = desktopWidget->availableGeometry(i);
    }
}

QTEST_MAIN(tst_QDesktopWidget)

#include "moc_tst_qdesktopwidget.cpp"
