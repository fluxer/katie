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
#include <qdeclarativemetatype_p.h>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDeclarativeItem>
#include <QDeclarativeContext>
#include <qdeclarativetextinput_p.h>
#include <qobject_p.h>

#include "../../../shared/util.h"

class tst_creation : public QObject
{
    Q_OBJECT
public:
    tst_creation();

private slots:
    void qobject_cpp();
    void qobject_qml();
    void qobject_qmltype();
    void qobject_alloc();

    void qobject_10flat_qml();
    void qobject_10flat_cpp();

    void qobject_10tree_qml();
    void qobject_10tree_cpp();

    void itemtree_notree_cpp();
    void itemtree_objtree_cpp();
    void itemtree_cpp();
    void itemtree_data_cpp();
    void itemtree_qml();
    void itemtree_scene_cpp();

    void elements_data();
    void elements();

private:
    QDeclarativeEngine engine;
};

class TestType : public QObject
{
Q_OBJECT
Q_PROPERTY(QDeclarativeListProperty<QObject> resources READ resources)
Q_CLASSINFO("DefaultProperty", "resources")
public:
    TestType(QObject *parent = 0)
    : QObject(parent) {}

    QDeclarativeListProperty<QObject> resources() {
        return QDeclarativeListProperty<QObject>(this, 0, resources_append);
    }

    static void resources_append(QDeclarativeListProperty<QObject> *p, QObject *o) {
        o->setParent(p->object);
    }
};

tst_creation::tst_creation()
{
    qmlRegisterType<TestType>("Qt.test", 1, 0, "TestType");

    //get rid of initialization effects
    QDeclarativeTextInput te;
}

void tst_creation::qobject_cpp()
{
    QBENCHMARK {
        QObject *obj = new QObject;
        delete obj;
    }
}

void tst_creation::qobject_qml()
{
    QDeclarativeComponent component(&engine);
    component.setData("import QtQuick 1.0\nQtObject {}", QUrl());
    QObject *obj = component.create();
    delete obj;

    QBENCHMARK {
        QObject *obj = component.create();
        delete obj;
    }
}

void tst_creation::qobject_10flat_qml()
{
    QDeclarativeComponent component(&engine);
    component.setData("import Qt.test 1.0\nTestType { resources: [ TestType{},TestType{},TestType{},TestType{},TestType{},TestType{},TestType{},TestType{},TestType{},TestType{} ] }", QUrl());
    QObject *obj = component.create();
    delete obj;

    QBENCHMARK {
        QObject *obj = component.create();
        delete obj;
    }
}

void tst_creation::qobject_10flat_cpp()
{
    QBENCHMARK {
        QObject *item = new TestType;
        new TestType(item);
        new TestType(item);
        new TestType(item);
        new TestType(item);
        new TestType(item);
        new TestType(item);
        new TestType(item);
        new TestType(item);
        new TestType(item);
        new TestType(item);
        delete item;
    }
}

void tst_creation::qobject_10tree_qml()
{
    QDeclarativeComponent component(&engine);
    component.setData("import Qt.test 1.0\nTestType { TestType{ TestType { TestType{ TestType{ TestType{ TestType{ TestType{ TestType{ TestType{ TestType{ } } } } } } } } } } }", QUrl());

    QObject *obj = component.create();
    delete obj;

    QBENCHMARK {
        QObject *obj = component.create();
        delete obj;
    }
}

void tst_creation::qobject_10tree_cpp()
{
    QBENCHMARK {
        QObject *item = new TestType;
        QObject *root = item;
        item = new TestType(item);
        item = new TestType(item);
        item = new TestType(item);
        item = new TestType(item);
        item = new TestType(item);
        item = new TestType(item);
        item = new TestType(item);
        item = new TestType(item);
        item = new TestType(item);
        item = new TestType(item);
        delete root;
    }
}

void tst_creation::qobject_qmltype()
{
    QDeclarativeType *t = QDeclarativeMetaType::qmlType("QtQuick/QtObject", 1, 0);

    QBENCHMARK {
        QObject *obj = t->create();
        delete obj;
    }
}

struct QObjectFakeData {
    char data[sizeof(QObjectPrivate)];
};

struct QObjectFake {
    QObjectFake();
    virtual ~QObjectFake();
private:
    QObjectFakeData *d;
};

QObjectFake::QObjectFake()
{
    d = new QObjectFakeData;
}

QObjectFake::~QObjectFake()
{
    delete d;
}

void tst_creation::qobject_alloc()
{
    QBENCHMARK {
        QObjectFake *obj = new QObjectFake;
        delete obj;
    }
}

struct QDeclarativeGraphics_Derived : public QObject
{
    void setParent_noEvent(QObject *parent) {
        bool sce = d_ptr->sendChildEvents;
        d_ptr->sendChildEvents = false;
        setParent(parent);
        d_ptr->sendChildEvents = sce;
    }
};

inline void QDeclarativeGraphics_setParent_noEvent(QObject *object, QObject *parent)
{
    static_cast<QDeclarativeGraphics_Derived *>(object)->setParent_noEvent(parent);
}

void tst_creation::itemtree_notree_cpp()
{
    QBENCHMARK {
        QDeclarativeItem *item = new QDeclarativeItem;
        for (int i = 0; i < 30; ++i) {
            QDeclarativeItem *child = new QDeclarativeItem;
            Q_UNUSED(child);
        }
        delete item;
    }
}

void tst_creation::itemtree_objtree_cpp()
{
    QBENCHMARK {
        QDeclarativeItem *item = new QDeclarativeItem;
        for (int i = 0; i < 30; ++i) {
            QDeclarativeItem *child = new QDeclarativeItem;
            QDeclarativeGraphics_setParent_noEvent(child,item);
        }
        delete item;
    }
}

void tst_creation::itemtree_cpp()
{
    QBENCHMARK {
        QDeclarativeItem *item = new QDeclarativeItem;
        for (int i = 0; i < 30; ++i) {
            QDeclarativeItem *child = new QDeclarativeItem;
            QDeclarativeGraphics_setParent_noEvent(child,item);
            child->setParentItem(item);
        }
        delete item;
    }
}

void tst_creation::itemtree_data_cpp()
{
    QBENCHMARK {
        QDeclarativeItem *item = new QDeclarativeItem;
        for (int i = 0; i < 30; ++i) {
            QDeclarativeItem *child = new QDeclarativeItem;
            QDeclarativeGraphics_setParent_noEvent(child,item);
            QDeclarativeListReference ref(item, "data");
            ref.append(child);
        }
        delete item;
    }
}

void tst_creation::itemtree_qml()
{
    QDeclarativeComponent component(&engine, TEST_FILE("item.qml"));
    QObject *obj = component.create();
    delete obj;

    QBENCHMARK {
        QObject *obj = component.create();
        delete obj;
    }
}

void tst_creation::itemtree_scene_cpp()
{
    QGraphicsScene scene;
    QDeclarativeItem *root = new QDeclarativeItem;
    scene.addItem(root);
    QBENCHMARK {
        QDeclarativeItem *item = new QDeclarativeItem;
        for (int i = 0; i < 30; ++i) {
            QDeclarativeItem *child = new QDeclarativeItem;
            QDeclarativeGraphics_setParent_noEvent(child,item);
            child->setParentItem(item);
        }
        item->setParentItem(root);
        delete item;
    }
    delete root;
}

void tst_creation::elements_data()
{
    QTest::addColumn<QByteArray>("type");

    QList<QByteArray> types = QDeclarativeMetaType::qmlTypeNames();
    foreach (QByteArray type, types)
        QTest::newRow(type.constData()) << type;
}

void tst_creation::elements()
{
    QFETCH(QByteArray, type);
    QDeclarativeType *t = QDeclarativeMetaType::qmlType(type, 4, 7);
    if (!t || !t->isCreatable())
        QSKIP("Non-creatable type", SkipSingle);

    QBENCHMARK {
        QObject *obj = t->create();
        delete obj;
    }
}

QTEST_MAIN(tst_creation)

#include "moc_tst_creation.cpp"
