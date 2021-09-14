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

class tst_QFlags: public QObject
{
    Q_OBJECT
private slots:
    void testFlag() const;
    void testFlagZeroFlag() const;
    void testFlagMultiBits() const;
    void constExpr();
};

void tst_QFlags::testFlag() const
{
    Qt::MouseButtons btn = Qt::LeftButton | Qt::RightButton;

    QVERIFY(btn.testFlag(Qt::LeftButton));
    QVERIFY(!btn.testFlag(Qt::MiddleButton));

    btn = 0;
    QVERIFY(!btn.testFlag(Qt::LeftButton));
}

void tst_QFlags::testFlagZeroFlag() const
{
    {
        Qt::MouseButtons btn = Qt::LeftButton | Qt::RightButton;
        /* Qt::NoButton has the value 0. */

        QVERIFY(!btn.testFlag(Qt::NoButton));
    }

    {
        /* A zero enum set should test true with zero. */
        QVERIFY(Qt::MouseButtons().testFlag(Qt::NoButton));
    }

    {
        Qt::MouseButtons btn = Qt::NoButton;
        QVERIFY(btn.testFlag(Qt::NoButton));
    }
}

void tst_QFlags::testFlagMultiBits() const
{
    /* Qt::Window is 0x00000001
     * Qt::Dialog is 0x00000002 | Window
     */
    {
        const Qt::WindowFlags onlyWindow(Qt::Window);
        QVERIFY(!onlyWindow.testFlag(Qt::Dialog));
    }

    {
        const Qt::WindowFlags hasDialog(Qt::Dialog);
        QVERIFY(hasDialog.testFlag(Qt::Dialog));
    }
}

template <int N, typename T> bool verifyConstExpr(T n) { return n == N; }

void tst_QFlags::constExpr()
{
    Qt::MouseButtons btn = Qt::LeftButton | Qt::RightButton;
    switch (btn) {
        case Qt::LeftButton: QVERIFY(false); break;
        case Qt::RightButton: QVERIFY(false); break;
        case Qt::LeftButton | Qt::RightButton: QVERIFY(true); break;
        default: QVERIFY(false);
    }

    QVERIFY(verifyConstExpr<(Qt::LeftButton | Qt::RightButton) & Qt::LeftButton>(Qt::LeftButton));
    QVERIFY(verifyConstExpr<(Qt::LeftButton | Qt::RightButton) & Qt::MiddleButton>(0));
    QVERIFY(verifyConstExpr<(Qt::LeftButton | Qt::RightButton) | Qt::MiddleButton>(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton));
    QVERIFY(verifyConstExpr<~(Qt::LeftButton | Qt::RightButton)>(~(Qt::LeftButton | Qt::RightButton)));
    QVERIFY(verifyConstExpr<Qt::MouseButtons(Qt::LeftButton) ^ Qt::RightButton>(Qt::LeftButton ^ Qt::RightButton));
    QVERIFY(verifyConstExpr<Qt::MouseButtons(0)>(0));
    QVERIFY(verifyConstExpr<Qt::MouseButtons(Qt::RightButton) & 0xff>(Qt::RightButton));
    QVERIFY(verifyConstExpr<Qt::MouseButtons(Qt::RightButton) | 0xff>(0xff));

    QVERIFY(!verifyConstExpr<Qt::RightButton>(!Qt::MouseButtons(Qt::LeftButton)));
}


QTEST_MAIN(tst_QFlags)

#include "moc_tst_qflags.cpp"
