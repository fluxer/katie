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

#include <qtest.h>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <QFile>
#include <QDebug>
#include "testtypes.h"

class tst_binding : public QObject
{
    Q_OBJECT

public:
    tst_binding();
    virtual ~tst_binding();

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void objectproperty_data();
    void objectproperty();
    void basicproperty_data();
    void basicproperty();

private:
    QDeclarativeEngine engine;
};

tst_binding::tst_binding()
{
}

tst_binding::~tst_binding()
{
}

void tst_binding::initTestCase()
{
    registerTypes();
}

void tst_binding::cleanupTestCase()
{
}

#define COMPONENT(filename, binding) \
    QDeclarativeComponent c(&engine); \
    { \
        QFile f(filename); \
        QVERIFY(f.open(QIODevice::ReadOnly)); \
        QByteArray data = f.readAll(); \
        data.replace("###", binding.toUtf8()); \
        c.setData(data, QUrl()); \
        QVERIFY(c.isReady()); \
    }

void tst_binding::objectproperty_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("binding");

    QTest::newRow("object.value") << SRCDIR "/data/objectproperty.txt" << "object.value";
    QTest::newRow("object.value + 10") << SRCDIR "/data/objectproperty.txt" << "object.value + 10";
}

void tst_binding::objectproperty()
{
    QFETCH(QString, file);
    QFETCH(QString, binding);

    COMPONENT(file, binding);

    MyQmlObject object1;
    MyQmlObject object2;

    MyQmlObject *object = qobject_cast<MyQmlObject *>(c.create());
    QVERIFY(object != 0);
    object->setObject(&object2);

    QBENCHMARK {
        object->setObject(&object1);
        object->setObject(&object2);
    }
}

void tst_binding::basicproperty_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("binding");

    QTest::newRow("value") << SRCDIR "/data/localproperty.txt" << "value";
    QTest::newRow("value + 10") << SRCDIR "/data/localproperty.txt" << "value + 10";
    QTest::newRow("value + value + 10") << SRCDIR "/data/localproperty.txt" << "value + value + 10";

    QTest::newRow("myObject.value") << SRCDIR "/data/idproperty.txt" << "myObject.value";
    QTest::newRow("myObject.value + 10") << SRCDIR "/data/idproperty.txt" << "myObject.value + 10";
    QTest::newRow("myObject.value + myObject.value + 10") << SRCDIR "/data/idproperty.txt" << "myObject.value + myObject.value + 10";
}

void tst_binding::basicproperty()
{
    QFETCH(QString, file);
    QFETCH(QString, binding);

    COMPONENT(file, binding);

    MyQmlObject *object = qobject_cast<MyQmlObject *>(c.create());
    QVERIFY(object != 0);
    object->setValue(10);

    QBENCHMARK {
        object->setValue(1);
    }
}

QTEST_MAIN(tst_binding)

#include "moc_tst_binding.cpp"
#include "moc_testtypes.h"
