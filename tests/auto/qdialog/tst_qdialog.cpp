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

#include <qdialog.h>
#include <qapplication.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstyle.h>
#include <QVBoxLayout>
#include <QSizeGrip>

#include "../../shared/util.h"

Q_DECLARE_METATYPE(QSize)

//TESTED_CLASS=
//TESTED_FILES=

class tst_QDialog : public QObject
{
    Q_OBJECT
public:
    tst_QDialog();

public slots:
    void initTestCase();
    void cleanupTestCase();
private slots:
    void getSetCheck();
    void defaultButtons();
    void showMaximized();
    void showMinimized();
    void showFullScreen();
    void showAsTool();
    void toolDialogPosition();
    void deleteMainDefault();
    void deleteInExec();
    void throwInExec();
    void showSizeGrip();
    void setVisible();
    void reject();

private:
    QDialog *testWidget;
};

// Testing get/set functions
void tst_QDialog::getSetCheck()
{
    QDialog obj1;

    // int QDialog::result()
    // void QDialog::setResult(int)
    obj1.setResult(0);
    QCOMPARE(0, obj1.result());
    obj1.setResult(INT_MIN);
    QCOMPARE(INT_MIN, obj1.result());
    obj1.setResult(INT_MAX);
    QCOMPARE(INT_MAX, obj1.result());
}

class ToolDialog : public QDialog
{
public:
    ToolDialog(QWidget *parent = 0) : QDialog(parent, Qt::Tool), mWasActive(false), tId(-1) {
    }
    bool wasActive() const { return mWasActive; }

    int exec() {
        tId = startTimer(300);
        return QDialog::exec();
    }
protected:
    void timerEvent(QTimerEvent *event) {
        if (tId == event->timerId()) {
            killTimer(tId);
            mWasActive = isActiveWindow();
            reject();
        }
    }

private:
    int mWasActive;
    int tId;
};

tst_QDialog::tst_QDialog()

{
}

void tst_QDialog::initTestCase()
{
    // Create the test class
    testWidget = new QDialog(0, Qt::X11BypassWindowManagerHint);
    testWidget->resize(200,200);
    testWidget->show();
    qApp->setActiveWindow(testWidget);
}

void tst_QDialog::cleanupTestCase()
{
    if (testWidget) {
        delete testWidget;
        testWidget = 0;
    }
}

void tst_QDialog::defaultButtons()
{
    QLineEdit *lineEdit = new QLineEdit(testWidget);
    QPushButton *push = new QPushButton("Button 1", testWidget);
    QPushButton *pushTwo = new QPushButton("Button 2", testWidget);
    QPushButton *pushThree = new QPushButton("Button 3", testWidget);
    pushThree->setAutoDefault(false);

    //we need to show the buttons. Otherwise they won't get the focus
    push->show();
    pushTwo->show();
    pushThree->show();

    push->setDefault(true);
    QVERIFY(push->isDefault());

    pushTwo->setFocus();
    QVERIFY(pushTwo->isDefault());
    pushThree->setFocus();
    QVERIFY(push->isDefault());
    lineEdit->setFocus();
    QVERIFY(push->isDefault());

    pushTwo->setDefault(true);
    QVERIFY(pushTwo->isDefault());

    pushTwo->setFocus();
    QVERIFY(pushTwo->isDefault());
    lineEdit->setFocus();
    QVERIFY(pushTwo->isDefault());
}

void tst_QDialog::showMaximized()
{
    QDialog dialog(0);
    dialog.setSizeGripEnabled(true);
    QSizeGrip *sizeGrip = dialog.findChild<QSizeGrip *>();
    QVERIFY(sizeGrip);

    dialog.showMaximized();
    QVERIFY(dialog.isMaximized());
    QVERIFY(dialog.isVisible());
    QVERIFY(!sizeGrip->isVisible());

    dialog.showNormal();
    QVERIFY(!dialog.isMaximized());
    QVERIFY(dialog.isVisible());
    QVERIFY(sizeGrip->isVisible());

    dialog.showMaximized();
    QVERIFY(dialog.isMaximized());
    QVERIFY(dialog.isVisible());

    dialog.hide();
    QVERIFY(dialog.isMaximized());
    QVERIFY(!dialog.isVisible());

    dialog.show();
    QVERIFY(dialog.isMaximized());
    QVERIFY(dialog.isVisible());

    dialog.hide();
    QVERIFY(dialog.isMaximized());
    QVERIFY(!dialog.isVisible());

    dialog.showMaximized();
    QVERIFY(dialog.isMaximized());
    QVERIFY(dialog.isVisible());
}

void tst_QDialog::showMinimized()
{
    QDialog dialog(0);

    dialog.showMinimized();
    QVERIFY(dialog.isMinimized());
    QVERIFY(dialog.isVisible());

    dialog.showNormal();
    QVERIFY(!dialog.isMinimized());
    QVERIFY(dialog.isVisible());

    dialog.showMinimized();
    QVERIFY(dialog.isMinimized());
    QVERIFY(dialog.isVisible());

    dialog.hide();
    QVERIFY(dialog.isMinimized());
    QVERIFY(!dialog.isVisible());

    dialog.show();
    QVERIFY(dialog.isMinimized());
    QVERIFY(dialog.isVisible());

    dialog.hide();
    QVERIFY(dialog.isMinimized());
    QVERIFY(!dialog.isVisible());

    dialog.showMinimized();
    QVERIFY(dialog.isMinimized());
    QVERIFY(dialog.isVisible());
}

void tst_QDialog::showFullScreen()
{
    QDialog dialog(0, Qt::X11BypassWindowManagerHint);
    dialog.setSizeGripEnabled(true);
    QSizeGrip *sizeGrip = dialog.findChild<QSizeGrip *>();
    QVERIFY(sizeGrip);

    qApp->syncX();
    dialog.showFullScreen();
    QVERIFY(dialog.isFullScreen());
    QVERIFY(dialog.isVisible());
    QVERIFY(!sizeGrip->isVisible());

    qApp->syncX();
    dialog.showNormal();
    QVERIFY(!dialog.isFullScreen());
    QVERIFY(dialog.isVisible());
    QVERIFY(sizeGrip->isVisible());

    qApp->syncX();
    dialog.showFullScreen();
    QVERIFY(dialog.isFullScreen());
    QVERIFY(dialog.isVisible());

    qApp->syncX();
    dialog.hide();
    QVERIFY(dialog.isFullScreen());
    QVERIFY(!dialog.isVisible());

    qApp->syncX();
    dialog.show();
    QVERIFY(dialog.isFullScreen());
    QVERIFY(dialog.isVisible());

    qApp->syncX();
    dialog.hide();
    QVERIFY(dialog.isFullScreen());
    QVERIFY(!dialog.isVisible());

    qApp->syncX();
    dialog.showFullScreen();
    QVERIFY(dialog.isFullScreen());
    QVERIFY(dialog.isVisible());

    qApp->syncX();
    dialog.hide();
    QVERIFY(dialog.isFullScreen());
    QVERIFY(!dialog.isVisible());
}

void tst_QDialog::showAsTool()
{
#if defined(Q_WS_X11)
    QSKIP("Qt/X11: Skipped since activeWindow() is not respected by all window managers", SkipAll);
#endif
    ToolDialog dialog(testWidget);
    testWidget->activateWindow();
    dialog.exec();
    QTest::qWait(100);
    if (testWidget->style()->styleHint(QStyle::SH_Widget_ShareActivation, 0, testWidget)) {
        QCOMPARE(dialog.wasActive(), true);
    } else {
        QCOMPARE(dialog.wasActive(), false);
    }
}

// Verify that pos() returns the same before and after show()
// for a dialog with the Tool window type.
void tst_QDialog::toolDialogPosition()
{
    QDialog dialog(0, Qt::Tool);
    dialog.move(QPoint(100,100));
    const QPoint beforeShowPosition = dialog.pos();
    dialog.show();
    const QPoint afterShowPosition = dialog.pos();
    QCOMPARE(afterShowPosition, beforeShowPosition);
}

class Dialog : public QDialog
{
public:
    Dialog(QPushButton *&button)
    {
        button = new QPushButton(this);
    }
};

void tst_QDialog::deleteMainDefault()
{
    QPushButton *button;
    Dialog dialog(button);
    button->setDefault(true);
    delete button;
    dialog.show();
    QTestEventLoop::instance().enterLoop(2);
}

void tst_QDialog::deleteInExec()
{
    QDialog *dialog = new QDialog(0);
    QMetaObject::invokeMethod(dialog, "deleteLater", Qt::QueuedConnection);
    QCOMPARE(dialog->exec(), int(QDialog::Rejected));
}

#ifndef QT_NO_EXCEPTIONS
class QDialogTestException : public std::exception { };

class ExceptionDialog : public QDialog
{
    Q_OBJECT
public:
    ExceptionDialog() : QDialog(0) { }
public slots:
    void throwException()
    {
        QDialogTestException e;
        throw e;
    }
};

void tst_QDialog::throwInExec()
{
    int caughtExceptions = 0;
    try {
        ExceptionDialog dialog;
        QMetaObject::invokeMethod(&dialog, "throwException", Qt::QueuedConnection);
        QMetaObject::invokeMethod(&dialog, "reject", Qt::QueuedConnection);
        (void) dialog.exec();
    } catch(...) {
        ++caughtExceptions;
    }
    QCOMPARE(caughtExceptions, 1);
}
#else
void tst_QDialog::throwInExec()
{
    QSKIP("Exceptions are disabled", SkipAll);
}
#endif //QT_NO_EXCEPTIONS

// From Task 124269
void tst_QDialog::showSizeGrip()
{
#ifndef QT_NO_SIZEGRIP
    QDialog dialog(0);
    dialog.show();
    QVERIFY(!dialog.isSizeGripEnabled());

    dialog.setSizeGripEnabled(true);
    QPointer<QSizeGrip> sizeGrip = dialog.findChild<QSizeGrip *>();
    QVERIFY(sizeGrip);
    QVERIFY(sizeGrip->isVisible());
    QVERIFY(dialog.isSizeGripEnabled());

    dialog.setSizeGripEnabled(false);
    QVERIFY(!dialog.isSizeGripEnabled());

    dialog.setSizeGripEnabled(true);
    sizeGrip = dialog.findChild<QSizeGrip *>();
    QVERIFY(sizeGrip);
    QVERIFY(sizeGrip->isVisible());
    sizeGrip->hide();
    dialog.hide();
    dialog.show();
    QVERIFY(!sizeGrip->isVisible());
#endif
}

void tst_QDialog::setVisible()
{
    QWidget topLevel;
    topLevel.show();

    QDialog *dialog = new QDialog;
    dialog->setLayout(new QVBoxLayout);
    dialog->layout()->addWidget(new QPushButton("dialog button"));

    QWidget *widget = new QWidget(&topLevel);
    widget->setLayout(new QVBoxLayout);
    widget->layout()->addWidget(dialog);

    QVERIFY(!dialog->isVisible());
    QVERIFY(!dialog->isHidden());

    widget->show();
    QVERIFY(dialog->isVisible());
    QVERIFY(!dialog->isHidden());

    widget->hide();
    dialog->hide();
    widget->show();
    QVERIFY(!dialog->isVisible());
    QVERIFY(dialog->isHidden());
}

class TestRejectDialog : public QDialog
{
    public:
        TestRejectDialog() : cancelReject(false), called(0) {}
        void reject()
        {
            called++;
            if (!cancelReject)
                QDialog::reject();
        }
        bool cancelReject;
        int called;
};

void tst_QDialog::reject()
{
    TestRejectDialog dialog;
    dialog.show();
    QTest::qWaitForWindowShown(&dialog);
    QTRY_VERIFY(dialog.isVisible());
    dialog.reject();
    QTRY_VERIFY(!dialog.isVisible());
    QCOMPARE(dialog.called, 1);

    dialog.show();
    QTest::qWaitForWindowShown(&dialog);
    QTRY_VERIFY(dialog.isVisible());
    QVERIFY(dialog.close());
    QTRY_VERIFY(!dialog.isVisible());
    QCOMPARE(dialog.called, 2);

    dialog.cancelReject = true;
    dialog.show();
    QTest::qWaitForWindowShown(&dialog);
    QTRY_VERIFY(dialog.isVisible());
    dialog.reject();
    QTRY_VERIFY(dialog.isVisible());
    QCOMPARE(dialog.called, 3);
    QVERIFY(!dialog.close());
    QTRY_VERIFY(dialog.isVisible());
    QCOMPARE(dialog.called, 4);
}

QTEST_MAIN(tst_QDialog)

#include "moc_tst_qdialog.cpp"
