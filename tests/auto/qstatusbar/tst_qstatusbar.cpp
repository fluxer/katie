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


#include <qstatusbar.h>
#include <QLabel>
#include <QMainWindow>
#include <QSizeGrip>

#include "../../shared/util.h"

// TESTED_CLASS=
// TESTED_FILES=

#ifndef QT_NO_STATUSBAR

class tst_QStatusBar: public QObject
{
    Q_OBJECT

public:
    tst_QStatusBar();
    virtual ~tst_QStatusBar();


protected slots:
    void messageChanged(const QString&);

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void tempMessage();
    void insertWidget();
    void insertPermanentWidget();
    void setSizeGripEnabled();
    void task194017_hiddenWidget();
    void QTBUG4334_hiddenOnMaximizedWindow();
    void QTBUG25492_msgtimeout();

private:
    QStatusBar *testWidget;
    QString currentMessage;
};

tst_QStatusBar::tst_QStatusBar()
{
}

tst_QStatusBar::~tst_QStatusBar()
{
}

void tst_QStatusBar::init()
{
    testWidget = new QStatusBar;
    connect(testWidget, SIGNAL(messageChanged(QString)), this, SLOT(messageChanged(QString)));

    QWidget *item1 = new QWidget(testWidget);
    testWidget->addWidget(item1);
}

void tst_QStatusBar::cleanup()
{
    delete testWidget;
}

void tst_QStatusBar::initTestCase()
{
}

void tst_QStatusBar::cleanupTestCase()
{
}

void tst_QStatusBar::messageChanged(const QString &m)
{
    currentMessage = m;
}

void tst_QStatusBar::tempMessage()
{
    QVERIFY(testWidget->currentMessage().isNull());
    QVERIFY(currentMessage.isNull());

    testWidget->showMessage("Ready", 500);
    QCOMPARE(testWidget->currentMessage(), QString("Ready"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    QTest::qWait(1000);

    QVERIFY(testWidget->currentMessage().isNull());
    QVERIFY(currentMessage.isNull());

    testWidget->showMessage("Ready again", 500);
    QCOMPARE(testWidget->currentMessage(), QString("Ready again"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    testWidget->clearMessage();
    QVERIFY(testWidget->currentMessage().isNull());
    QVERIFY(currentMessage.isNull());
}

void tst_QStatusBar::insertWidget()
{
    QStatusBar sb;
    sb.addPermanentWidget(new QLabel("foo"));
    QTest::ignoreMessage(QtWarningMsg, "QStatusBar::insertWidget: Index out of range (-1), appending widget");
    QCOMPARE(sb.insertWidget(-1, new QLabel("foo")), 0);
    QTest::ignoreMessage(QtWarningMsg, "QStatusBar::insertWidget: Index out of range (2), appending widget");
    QCOMPARE(sb.insertWidget(2, new QLabel("foo")), 1);
    QCOMPARE(sb.insertWidget(0, new QLabel("foo")), 0);
    QCOMPARE(sb.insertWidget(3, new QLabel("foo")), 3);
}

void tst_QStatusBar::insertPermanentWidget()
{
    QStatusBar sb;
    sb.addWidget(new QLabel("foo"));
    QTest::ignoreMessage(QtWarningMsg, "QStatusBar::insertPermanentWidget: Index out of range (-1), appending widget");
    QCOMPARE(sb.insertPermanentWidget(-1, new QLabel("foo")), 1);
    QTest::ignoreMessage(QtWarningMsg, "QStatusBar::insertPermanentWidget: Index out of range (0), appending widget");
    QCOMPARE(sb.insertPermanentWidget(0, new QLabel("foo")), 2);
    QCOMPARE(sb.insertPermanentWidget(2, new QLabel("foo")), 2);
    QTest::ignoreMessage(QtWarningMsg, "QStatusBar::insertPermanentWidget: Index out of range (5), appending widget");
    QCOMPARE(sb.insertPermanentWidget(5, new QLabel("foo")), 4);
    QCOMPARE(sb.insertWidget(1, new QLabel("foo")), 1);
    QTest::ignoreMessage(QtWarningMsg, "QStatusBar::insertPermanentWidget: Index out of range (1), appending widget");
    QCOMPARE(sb.insertPermanentWidget(1, new QLabel("foo")), 6);
}

void tst_QStatusBar::setSizeGripEnabled()
{
#ifndef QT_NO_SIZEGRIP
    QMainWindow mainWindow;
    QPointer<QStatusBar> statusBar = mainWindow.statusBar();
    QVERIFY(statusBar);
    mainWindow.show();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&mainWindow);
#endif

    QTRY_VERIFY(statusBar->isVisible());
    QPointer<QSizeGrip> sizeGrip = statusBar->findChild<QSizeGrip *>();
    QVERIFY(sizeGrip);
    QVERIFY(sizeGrip->isVisible());

    statusBar->setSizeGripEnabled(true);
    QVERIFY(sizeGrip);
    QVERIFY(sizeGrip->isVisible());

    statusBar->hide();
    QVERIFY(!sizeGrip->isVisible());
    statusBar->show();
    QVERIFY(sizeGrip->isVisible());

    sizeGrip->setVisible(false);
    QVERIFY(!sizeGrip->isVisible());
    statusBar->hide();
    statusBar->show();
    QVERIFY(!sizeGrip->isVisible());

    statusBar->setSizeGripEnabled(false);
    QVERIFY(!sizeGrip);

    qApp->processEvents();
    mainWindow.showFullScreen();
#ifdef Q_WS_X11
    qt_x11_wait_for_window_manager(&mainWindow);
#endif
    qApp->processEvents();

    mainWindow.setStatusBar(new QStatusBar(&mainWindow));
    //we now call deleteLater on the previous statusbar
    QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);
    QVERIFY(!statusBar);
    statusBar = mainWindow.statusBar();
    QVERIFY(statusBar);

    sizeGrip = statusBar->findChild<QSizeGrip *>();
    QVERIFY(sizeGrip);
    QVERIFY(!sizeGrip->isVisible());

    statusBar->setSizeGripEnabled(true);
    QVERIFY(!sizeGrip->isVisible());

    qApp->processEvents();
    mainWindow.showNormal();
    qApp->processEvents();
    QTRY_VERIFY(sizeGrip->isVisible());
#else // QT_NO_SIZEGRIP
    QSKIP("Katie compiled without sizegrip support (QT_NO_SIZEGRIP)", SkipAll);
#endif // QT_NO_SIZEGRIP
}

void tst_QStatusBar::task194017_hiddenWidget()
{
    QStatusBar sb;

    QWidget *label= new QLabel("bar",&sb);
    sb.addWidget(label);
    sb.show();
    QVERIFY(label->isVisible());
    sb.showMessage("messssage");
    QVERIFY(!label->isVisible());
    sb.hide();
    QVERIFY(!label->isVisible());
    sb.show();
    QVERIFY(!label->isVisible());
    sb.clearMessage();
    QVERIFY(label->isVisible());
    label->hide();
    QVERIFY(!label->isVisible());
    sb.showMessage("messssage");
    QVERIFY(!label->isVisible());
    sb.clearMessage();
    QVERIFY(!label->isVisible());
    sb.hide();
    QVERIFY(!label->isVisible());
    sb.show();
    QVERIFY(!label->isVisible());
}

void tst_QStatusBar::QTBUG4334_hiddenOnMaximizedWindow()
{
#ifndef QT_NO_SIZEGRIP
    QMainWindow main;
    QStatusBar statusbar;
    statusbar.setSizeGripEnabled(true);
    main.setStatusBar(&statusbar);
    main.showMaximized();
    QTest::qWaitForWindowShown(&main);
    QVERIFY(!statusbar.findChild<QSizeGrip*>()->isVisible());
    main.showNormal();
    QTest::qWaitForWindowShown(&main);
    QVERIFY(statusbar.findChild<QSizeGrip*>()->isVisible());
    main.showFullScreen();
    QTest::qWaitForWindowShown(&main);
    QVERIFY(!statusbar.findChild<QSizeGrip*>()->isVisible());
#else // QT_NO_SIZEGRIP
    QSKIP("Katie compiled without sizegrip support (QT_NO_SIZEGRIP)", SkipAll);
#endif // QT_NO_SIZEGRIP
}

void tst_QStatusBar::QTBUG25492_msgtimeout()
{
    QVERIFY(testWidget->currentMessage().isNull());
    QVERIFY(currentMessage.isNull());
    testWidget->show();

    // Set display message forever first
    testWidget->showMessage("Ready", 0);
    QCOMPARE(testWidget->currentMessage(), QString("Ready"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    QTest::qWait(1000);

    // Set display message for 2 seconds again
    testWidget->showMessage("Ready", 2000);
    QCOMPARE(testWidget->currentMessage(), QString("Ready"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    QTest::qWait(3000);

    // Message disappears after 2 seconds
    QVERIFY(testWidget->currentMessage().isNull());
    QVERIFY(currentMessage.isNull());

    // Set display message for 2 seconds first
    testWidget->showMessage("Ready 25492", 2000);
    QCOMPARE(testWidget->currentMessage(), QString("Ready 25492"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    QTest::qWait(1000);

    // Set display message forever again
    testWidget->showMessage("Ready 25492", 0);
    QCOMPARE(testWidget->currentMessage(), QString("Ready 25492"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);

    QTest::qWait(3000);

    // Message displays forever
    QCOMPARE(testWidget->currentMessage(), QString("Ready 25492"));
    QCOMPARE(testWidget->currentMessage(), currentMessage);
}


QTEST_MAIN(tst_QStatusBar)

#include "moc_tst_qstatusbar.cpp"

#else // QT_NO_STATUSBAR

QTEST_NOOP_MAIN

#endif // QT_NO_STATUSBAR
