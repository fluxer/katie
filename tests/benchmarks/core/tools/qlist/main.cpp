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

class tst_QList: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void removeAll_data();
    void removeAll();
};

template <class T>
void removeAll_test(const QList<int> &i10, ushort valueToRemove, int itemsToRemove)
{
    QList<T> list;
    for (int i = 0; i < 10 * N; ++i) {
        T t(i10.at(i % 10));
        list.append(t);
    }

    T t(valueToRemove);

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

Q_DECLARE_METATYPE(QList<int>);

void tst_QList::removeAll_data()
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

void tst_QList::removeAll()
{
    QFETCH(QList<int>, i10);
    QFETCH(int, valueToRemove);
    QFETCH(int, itemsToRemove);

    removeAll_test<ushort>(i10, valueToRemove, itemsToRemove);
}

QTEST_APPLESS_MAIN(tst_QList)

#include "moc_main.cpp"
