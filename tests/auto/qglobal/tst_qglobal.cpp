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

class tst_QGlobal: public QObject
{
    Q_OBJECT
private slots:
    void qIsNull();
    void for_each();
    void qassert();
    void qtry();
    void checkptr();
};

void tst_QGlobal::qIsNull()
{
    double d = 0.0;
    float f = 0.0f;

    QVERIFY(::qIsNull(d));
    QVERIFY(::qIsNull(f));

    d += 0.000000001;
    f += 0.0000001f;

    QVERIFY(!::qIsNull(d));
    QVERIFY(!::qIsNull(f));
}

void tst_QGlobal::for_each()
{
    QList<int> list;
    list << 0 << 1 << 2 << 3 << 4 << 5;

    int counter = 0;
    foreach(int i, list) {
        QCOMPARE(i, counter++);
    }
    QCOMPARE(counter, list.count());

    // do it again, to make sure we don't have any for-scoping
    // problems with older compilers
    counter = 0;
    foreach(int i, list) {
        QCOMPARE(i, counter++);
    }
    QCOMPARE(counter, list.count());
}

void tst_QGlobal::qassert()
{
    bool passed = false;
    if (false) {
        Q_ASSERT(false);
    } else {
        passed = true;
    }
    QVERIFY(passed);

    passed = false;
    if (false) {
        Q_ASSERT_X(false, "tst_QGlobal", "qassert");
    } else {
        passed = true;
    }
    QVERIFY(passed);

    passed = false;
    if (false)
        Q_ASSERT(false);
    else
        passed = true;
    QVERIFY(passed);

    passed = false;
    if (false)
        Q_ASSERT_X(false, "tst_QGlobal", "qassert");
    else
        passed = true;
    QVERIFY(passed);
}

void tst_QGlobal::qtry()
{
    int i = 0;
    QT_TRY {
        i = 1;
        QT_THROW(42);
        i = 2;
    } QT_CATCH(int) {
        QCOMPARE(i, 1);
        i = 7;
    }
#ifdef QT_NO_EXCEPTIONS
    QCOMPARE(i, 2);
#else
    QCOMPARE(i, 7);
#endif

    // check propper if/else scoping
    i = 0;
    if (true)
        QT_TRY {
            i = 2;
            QT_THROW(42);
            i = 4;
        } QT_CATCH(int) {
            QCOMPARE(i, 2);
            i = 4;
        }
    else
        QCOMPARE(i, 0);
    QCOMPARE(i, 4);

    i = 0;
    if (false)
        QT_TRY {
            i = 2;
            QT_THROW(42);
            i = 4;
        } QT_CATCH(int) {
            QCOMPARE(i, 2);
            i = 2;
        }
    else
        i = 8;
    QCOMPARE(i, 8);

    i = 0;
    if (false)
        i = 42;
    else
        QT_TRY {
            i = 2;
            QT_THROW(42);
            i = 4;
        } QT_CATCH(int) {
            QCOMPARE(i, 2);
            i = 4;
        }
    QCOMPARE(i, 4);
}

void tst_QGlobal::checkptr()
{
    int i;
    Q_CHECK_PTR(&i);

    const char *c = "hello";
    Q_CHECK_PTR(c);
}

QTEST_MAIN(tst_QGlobal)

#include "moc_tst_qglobal.cpp"
