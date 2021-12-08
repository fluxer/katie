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
#include <QDebug>

#include "../../../shared/util.h"

class tst_typeimports : public QObject
{
    Q_OBJECT
public:
    tst_typeimports();

private slots:
    void cpp();
    void qml();

private:
    QDeclarativeEngine engine;
};

class TestType1 : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeListProperty<QObject> resources READ resources);
    Q_CLASSINFO("DefaultProperty", "resources");
public:
    TestType1(QObject *parent = 0) : QObject(parent) {}

    QDeclarativeListProperty<QObject> resources() {
        return QDeclarativeListProperty<QObject>(this, 0, resources_append);
    }

    static void resources_append(QDeclarativeListProperty<QObject> *p, QObject *o) {
        o->setParent(p->object);
    }
};

class TestType2 : public TestType1
{
    Q_OBJECT
public:
    TestType2(QObject *parent = 0) : TestType1(parent) {}
};


class TestType3 : public TestType1
{
    Q_OBJECT
public:
    TestType3(QObject *parent = 0) : TestType1(parent) {}
};

class TestType4 : public TestType1
{
    Q_OBJECT
public:
    TestType4(QObject *parent = 0) : TestType1(parent) {}
};


tst_typeimports::tst_typeimports()
{
    qmlRegisterType<TestType1>("Qt.test", 1, 0, "TestType1");
    qmlRegisterType<TestType2>("Qt.test", 1, 0, "TestType2");
    qmlRegisterType<TestType3>("Qt.test", 2, 0, "TestType3");
    qmlRegisterType<TestType4>("Qt.test", 2, 0, "TestType4");
}

void tst_typeimports::cpp()
{
    QBENCHMARK {
        QDeclarativeComponent component(&engine, TEST_FILE("cpp.qml"));
        QVERIFY(component.isReady());
    }
}

void tst_typeimports::qml()
{
    //get rid of initialization effects
    { QDeclarativeComponent component(&engine, TEST_FILE("qml.qml")); }

    QBENCHMARK {
        QDeclarativeComponent component(&engine, TEST_FILE("qml.qml"));
        QVERIFY(component.isReady());
    }
}

QTEST_MAIN(tst_typeimports)

#include "moc_tst_typeimports.cpp"
