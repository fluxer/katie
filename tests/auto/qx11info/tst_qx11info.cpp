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

#ifdef Q_WS_X11

#include <QApplication>
#include <QX11Info>

class tst_QX11Info : public QObject
{
    Q_OBJECT

private slots:
    void staticFunctionsBeforeQApplication();
};

void tst_QX11Info::staticFunctionsBeforeQApplication()
{
    QVERIFY(!QApplication::instance());

    // none of these functions should crash if QApplication hasn't
    // been constructed

    Display *display = QX11Info::display();
    QCOMPARE(display, (Display *)0);
    const char *appClass = QX11Info::appClass();
    QCOMPARE(appClass, (const char *)0);
    int appScreen = QX11Info::appScreen();
    QCOMPARE(appScreen, 0);
    int appDepth = QX11Info::appDepth();
    QCOMPARE(appDepth, 32);
    int appCells = QX11Info::appCells();
    QCOMPARE(appCells, 0);
    Qt::HANDLE appColormap = QX11Info::appColormap();
    QCOMPARE(appColormap, static_cast<Qt::HANDLE>(0));
    void *appVisual = QX11Info::appVisual();
    QCOMPARE(appVisual, static_cast<void *>(0));
    Qt::HANDLE appRootWindow = QX11Info::appRootWindow();
    QCOMPARE(appRootWindow, static_cast<Qt::HANDLE>(0));

    bool appDefaultColormap = QX11Info::appDefaultColormap();
    QCOMPARE(appDefaultColormap, true);
    bool appDefaultVisual = QX11Info::appDefaultVisual();
    QCOMPARE(appDefaultVisual, true);

    int appDpiX = QX11Info::appDpiX();
    int appDpiY = QX11Info::appDpiY();
    QCOMPARE(appDpiX, 75);
    QCOMPARE(appDpiY, 75);

    // the setAppDpi{X,Y} calls do nothing if QApplication hasn't been
    // constructed
    QX11Info::setAppDpiX(-1, 120);
    QX11Info::setAppDpiY(-1, 120);
    appDpiX = QX11Info::appDpiX();
    appDpiY = QX11Info::appDpiY();
    QCOMPARE(appDpiX, 75);
    QCOMPARE(appDpiY, 75);

    unsigned long appTime = QX11Info::appTime();
    unsigned long appUserTime = QX11Info::appUserTime();
    QCOMPARE(appTime, 0ul);
    QCOMPARE(appUserTime, 0ul);
    // setApp*Time do nothing without QApplication
    QX11Info::setAppTime(1234);
    QX11Info::setAppUserTime(5678);
    appTime = QX11Info::appTime();
    appUserTime = QX11Info::appUserTime();
    QCOMPARE(appTime, 0ul);
    QCOMPARE(appUserTime, 0ul);
}

QTEST_APPLESS_MAIN(tst_QX11Info)

#include "moc_tst_qx11info.cpp"

#else // !Q_WS_X11

QTEST_NOOP_MAIN

#endif
