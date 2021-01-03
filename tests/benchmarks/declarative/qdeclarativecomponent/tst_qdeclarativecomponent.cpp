/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
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

class tst_qmlcomponent : public QObject
{
    Q_OBJECT

public:
    tst_qmlcomponent();
    virtual ~tst_qmlcomponent();

public slots:
    void initTestCase();
    void cleanupTestCase();

private slots:
    void creation_data();
    void creation();

private:
    QDeclarativeEngine engine;
};

tst_qmlcomponent::tst_qmlcomponent()
{
}

tst_qmlcomponent::~tst_qmlcomponent()
{
}

void tst_qmlcomponent::initTestCase()
{
    registerTypes();
}

void tst_qmlcomponent::cleanupTestCase()
{
}

void tst_qmlcomponent::creation_data()
{
    QTest::addColumn<QString>("file");

    QTest::newRow("Object") << SRCDIR "/data/object.qml";
    QTest::newRow("Object - Id") << SRCDIR "/data/object_id.qml";
    QTest::newRow("MyQmlObject") << SRCDIR "/data/myqmlobject.qml";
    QTest::newRow("MyQmlObject: basic binding") << SRCDIR "/data/myqmlobject_binding.qml";
    QTest::newRow("Synthesized properties") << SRCDIR "/data/synthesized_properties.qml";
    QTest::newRow("Synthesized properties.2") << SRCDIR "/data/synthesized_properties.2.qml";
}

void tst_qmlcomponent::creation()
{
    QFETCH(QString, file);

    QDeclarativeComponent c(&engine, file);
    QVERIFY(c.isReady());

    QObject *obj = c.create();
    delete obj;

    QBENCHMARK {
        QObject *obj = c.create();
        delete obj;
    }
}

QTEST_MAIN(tst_qmlcomponent)

#include "moc_tst_qdeclarativecomponent.cpp"
#include "moc_testtypes.h"
