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

#include <qcoreapplication.h>
#include <qdebug.h>
#include <qsqlerror.h>


//TESTED_CLASS=
//TESTED_FILES=

class tst_QSqlError : public QObject
{
Q_OBJECT

public:
    tst_QSqlError();
    virtual ~tst_QSqlError();

private slots:
    void getSetCheck();
    void construction();
};

tst_QSqlError::tst_QSqlError()
{
}

tst_QSqlError::~tst_QSqlError()
{
}

// Testing get/set functions
void tst_QSqlError::getSetCheck()
{
    QSqlError obj1;
    // ErrorType QSqlError::type()
    // void QSqlError::setType(ErrorType)
    obj1.setType(QSqlError::ErrorType(QSqlError::NoError));
    QCOMPARE(QSqlError::ErrorType(QSqlError::NoError), obj1.type());
    obj1.setType(QSqlError::ErrorType(QSqlError::ConnectionError));
    QCOMPARE(QSqlError::ErrorType(QSqlError::ConnectionError), obj1.type());
    obj1.setType(QSqlError::ErrorType(QSqlError::StatementError));
    QCOMPARE(QSqlError::ErrorType(QSqlError::StatementError), obj1.type());
    obj1.setType(QSqlError::ErrorType(QSqlError::TransactionError));
    QCOMPARE(QSqlError::ErrorType(QSqlError::TransactionError), obj1.type());
    obj1.setType(QSqlError::ErrorType(QSqlError::UnknownError));
    QCOMPARE(QSqlError::ErrorType(QSqlError::UnknownError), obj1.type());

    // int QSqlError::number()
    // void QSqlError::setNumber(int)
    obj1.setNumber(0);
    QCOMPARE(0, obj1.number());
    obj1.setNumber(INT_MIN);
    QCOMPARE(INT_MIN, obj1.number());
    obj1.setNumber(INT_MAX);
    QCOMPARE(INT_MAX, obj1.number());
}

void tst_QSqlError::construction()
{
   QSqlError obj1("drivertext", "databasetext", QSqlError::UnknownError, 123);
   QCOMPARE(obj1.driverText(), QString("drivertext"));
   QCOMPARE(obj1.databaseText(), QString("databasetext"));
   QCOMPARE(obj1.type(), QSqlError::UnknownError);
   QCOMPARE(obj1.number(), 123);
   obj1.isValid();

   QSqlError obj2(obj1);
   QCOMPARE(obj2.driverText(), obj1.driverText());
   QCOMPARE(obj2.databaseText(), obj1.databaseText());
   QCOMPARE(obj2.type(), obj1.type());
   QCOMPARE(obj2.number(), obj1.number());
   QVERIFY(obj2.isValid());

   QSqlError obj3 = obj2;
   QCOMPARE(obj3.driverText(), obj2.driverText());
   QCOMPARE(obj3.databaseText(), obj2.databaseText());
   QCOMPARE(obj3.type(), obj2.type());
   QCOMPARE(obj3.number(), obj2.number());
   QVERIFY(obj3.isValid());

   QSqlError obj4;
   QVERIFY(!obj4.isValid());
}

QTEST_MAIN(tst_QSqlError)

#include "moc_tst_qsqlerror.cpp"
