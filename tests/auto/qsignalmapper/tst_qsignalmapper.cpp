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


#include <qsignalmapper.h>
#include <qspinbox.h>

// TESTED_CLASS=
// TESTED_FILES=

#ifndef QT_NO_SIGNALMAPPER

class tst_QSignalMapper : public QObject
{
    Q_OBJECT

public:
    tst_QSignalMapper();
    virtual ~tst_QSignalMapper();


public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
private slots:
    void mapped();
};

tst_QSignalMapper::tst_QSignalMapper()
{
}

tst_QSignalMapper::~tst_QSignalMapper()
{
}

void tst_QSignalMapper::initTestCase()
{
}

void tst_QSignalMapper::cleanupTestCase()
{
}

void tst_QSignalMapper::init()
{
}

void tst_QSignalMapper::cleanup()
{
}

class QtTestObject : public QObject
{
    Q_OBJECT
public slots:
    void myslot(int id);
    void myslot(const QString &str);
 
public:
    int id;
    QString str;
};

void QtTestObject::myslot(int id)
{
    this->id = id;
}

void QtTestObject::myslot(const QString &str)
{
    this->str = str;
}

void tst_QSignalMapper::mapped()
{
    QSignalMapper mapper(0);

    QtTestObject target;
    QSpinBox spinBox1(0);
    QSpinBox spinBox2(0);
    QSpinBox spinBox3(0);

    connect(&spinBox1, SIGNAL(valueChanged(int)), &mapper, SLOT(map()));
    connect(&spinBox2, SIGNAL(valueChanged(int)), &mapper, SLOT(map()));
    connect(&spinBox3, SIGNAL(valueChanged(int)), &mapper, SLOT(map()));

    mapper.setMapping(&spinBox1, 7);
    mapper.setMapping(&spinBox1, 1);
    mapper.setMapping(&spinBox2, 2);
    mapper.setMapping(&spinBox2, "two");
    mapper.setMapping(&spinBox3, "three");

    connect(&mapper, SIGNAL(mapped(int)), &target, SLOT(myslot(int)));
    connect(&mapper, SIGNAL(mapped(const QString &)), &target, SLOT(myslot(const QString &)));

    spinBox1.setValue(20);
    QVERIFY(target.id == 1);
    QVERIFY(target.str.isEmpty());

    spinBox2.setValue(20);
    QVERIFY(target.id == 2);
    QVERIFY(target.str == "two");

    spinBox3.setValue(20);
    QVERIFY(target.id == 2);
    QVERIFY(target.str == "three");
}

QTEST_MAIN(tst_QSignalMapper)

#include "moc_tst_qsignalmapper.cpp"

#else // QT_NO_SIGNALMAPPER

QTEST_NOOP_MAIN

#endif // QT_NO_SIGNALMAPPER
