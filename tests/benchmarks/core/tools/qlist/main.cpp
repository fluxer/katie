/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#include <QList>
#include <QTest>

QT_USE_NAMESPACE

static const int N = 1000;

struct MyBase
{
    MyBase(int i_)
        : isCopy(false)
    {
        ++liveCount;

        i = i_;
    }

    MyBase(const MyBase &other)
        : isCopy(true)
    {
        if (isCopy)
            ++copyCount;
        ++liveCount;

        i = other.i;
    }

    MyBase &operator=(const MyBase &other)
    {
        if (!isCopy) {
            isCopy = true;
            ++copyCount;
        } else {
            ++errorCount;
        }

        i = other.i;
        return *this;
    }

    ~MyBase()
    {
        if (isCopy) {
            if (!copyCount)
                ++errorCount;
            else
                --copyCount;
        }
        if (!liveCount)
            ++errorCount;
        else
            --liveCount;
    }

    bool operator==(const MyBase &other) const
    { return i == other.i; }

protected:
    ushort i;
    bool isCopy;

public:
    static int errorCount;
    static int liveCount;
    static int copyCount;
};

int MyBase::errorCount = 0;
int MyBase::liveCount = 0;
int MyBase::copyCount = 0;

struct MyPrimitive : public MyBase
{
    MyPrimitive(int i = -1) : MyBase(i)
    { ++errorCount; }
    MyPrimitive(const MyPrimitive &other) : MyBase(other)
    { ++errorCount; }
    ~MyPrimitive()
    { ++errorCount; }
};

struct MyMovable : public MyBase
{
    MyMovable(int i = -1) : MyBase(i) {}
};

struct MyComplex : public MyBase
{
    MyComplex(int i = -1) : MyBase(i) {}
};

QT_BEGIN_NAMESPACE

Q_DECLARE_TYPEINFO(MyPrimitive, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(MyMovable, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(MyComplex, Q_COMPLEX_TYPE);

QT_END_NAMESPACE


class tst_QList: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void removeAll_primitive_data();
    void removeAll_primitive();
    void removeAll_movable_data();
    void removeAll_movable();
    void removeAll_complex_data();
    void removeAll_complex();
};

template <class T>
void removeAll_test(const QList<int> &i10, ushort valueToRemove, int itemsToRemove)
{
    bool isComplex = QTypeInfo<T>::isComplex;

    MyBase::errorCount = 0;
    MyBase::liveCount = 0;
    MyBase::copyCount = 0;
    {
        QList<T> list;
        QCOMPARE(MyBase::liveCount, 0);
        QCOMPARE(MyBase::copyCount, 0);

        for (int i = 0; i < 10 * N; ++i) {
            T t(i10.at(i % 10));
            list.append(t);
        }
        QCOMPARE(MyBase::liveCount, isComplex ? list.size() : 0);
        QCOMPARE(MyBase::copyCount, isComplex ? list.size() : 0);

        T t(valueToRemove);
        QCOMPARE(MyBase::liveCount, isComplex ? list.size() + 1 : 1);
        QCOMPARE(MyBase::copyCount, isComplex ? list.size() : 0);

        int removedCount;
        QList<T> l;

        QBENCHMARK {
            l = list;
            removedCount = l.removeAll(t);
        }
        QCOMPARE(removedCount, itemsToRemove * N);
        QCOMPARE(l.size() + removedCount, list.size());
        QVERIFY(!l.contains(valueToRemove));
    }
    if (isComplex)
        QCOMPARE(MyBase::errorCount, 0);
}

Q_DECLARE_METATYPE(QList<int>);

void tst_QList::removeAll_primitive_data()
{
    qRegisterMetaType<QList<int> >();

    QTest::addColumn<QList<int> >("i10");
    QTest::addColumn<int>("valueToRemove");
    QTest::addColumn<int>("itemsToRemove");

    QTest::newRow("0%")   << (QList<int>() << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0) << 5 << 0;
    QTest::newRow("10%")  << (QList<int>() << 0 << 0 << 0 << 0 << 5 << 0 << 0 << 0 << 0 << 0) << 5 << 1;
    QTest::newRow("90%")  << (QList<int>() << 5 << 5 << 5 << 5 << 0 << 5 << 5 << 5 << 5 << 5) << 5 << 9;
    QTest::newRow("100%") << (QList<int>() << 5 << 5 << 5 << 5 << 5 << 5 << 5 << 5 << 5 << 5) << 5 << 10;
}

void tst_QList::removeAll_primitive()
{
    QFETCH(QList<int>, i10);
    QFETCH(int, valueToRemove);
    QFETCH(int, itemsToRemove);

    removeAll_test<MyPrimitive>(i10, valueToRemove, itemsToRemove);
}

void tst_QList::removeAll_movable_data()
{
    removeAll_primitive_data();
}

void tst_QList::removeAll_movable()
{
    QFETCH(QList<int>, i10);
    QFETCH(int, valueToRemove);
    QFETCH(int, itemsToRemove);

    removeAll_test<MyMovable>(i10, valueToRemove, itemsToRemove);
}

void tst_QList::removeAll_complex_data()
{
    removeAll_primitive_data();
}

void tst_QList::removeAll_complex()
{
    QFETCH(QList<int>, i10);
    QFETCH(int, valueToRemove);
    QFETCH(int, itemsToRemove);

    removeAll_test<MyComplex>(i10, valueToRemove, itemsToRemove);
}

QTEST_APPLESS_MAIN(tst_QList)

#include "moc_main.cpp"
