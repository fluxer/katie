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
#include <QtSql/QtSql>

#include "../../../../auto/qsqldatabase/tst_databases.h"

const QString qtest(qTableName( QLatin1String("qtest"), __FILE__ ));

class tst_QSqlQuery : public QObject
{
    Q_OBJECT

public:
    tst_QSqlQuery();
    virtual ~tst_QSqlQuery();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void benchmark_data() { generic_data(); }
    void benchmark();

private:
    // returns all database connections
    void generic_data(const QString &engine=QString());
    void dropTestTables( QSqlDatabase db );
    void createTestTables( QSqlDatabase db );
    void populateTestTables( QSqlDatabase db );

    tst_Databases dbs;
};

QTEST_MAIN(tst_QSqlQuery)

tst_QSqlQuery::tst_QSqlQuery()
{
}

tst_QSqlQuery::~tst_QSqlQuery()
{
}

void tst_QSqlQuery::initTestCase()
{
    dbs.open();

    for ( QStringList::ConstIterator it = dbs.dbNames.begin(); it != dbs.dbNames.end(); ++it ) {
        QSqlDatabase db = QSqlDatabase::database(( *it ) );
        CHECK_DATABASE( db );
        dropTestTables( db ); //in case of leftovers
        createTestTables( db );
        populateTestTables( db );
    }
}

void tst_QSqlQuery::cleanupTestCase()
{
    for ( QStringList::ConstIterator it = dbs.dbNames.begin(); it != dbs.dbNames.end(); ++it ) {
        QSqlDatabase db = QSqlDatabase::database(( *it ) );
        CHECK_DATABASE( db );
        dropTestTables( db );
    }

    dbs.close();
}

void tst_QSqlQuery::init()
{
}

void tst_QSqlQuery::cleanup()
{
    QFETCH( QString, dbName );
    QSqlDatabase db = QSqlDatabase::database( dbName );
    CHECK_DATABASE( db );

    if ( QTest::currentTestFunction() == QLatin1String( "numRowsAffected" )
            || QTest::currentTestFunction() == QLatin1String( "transactions" )
            || QTest::currentTestFunction() == QLatin1String( "size" )
            || QTest::currentTestFunction() == QLatin1String( "isActive" )
            || QTest::currentTestFunction() == QLatin1String( "lastInsertId" ) ) {
        populateTestTables( db );
    }

    if ( QTest::currentTestFailed() && ( db.driverName().startsWith( QLatin1String("QODBC") ) ) ) {
        //since Oracle ODBC totally craps out on error, we init again
        db.close();
        db.open();
    }
}

void tst_QSqlQuery::generic_data(const QString& engine)
{
    if ( dbs.fillTestTable(engine) == 0 ) {
        if (engine.isEmpty())
           QSKIP( "No database drivers are available in this Qt configuration", SkipAll );
        else
           QSKIP( (QString("No database drivers of type %1 are available in this Qt configuration").arg(engine)).toLocal8Bit(), SkipAll );
    }
}

void tst_QSqlQuery::dropTestTables( QSqlDatabase db )
{
    QStringList tablenames;
    // drop all the table in case a testcase failed
    tablenames <<  qtest
               << qTableName( QLatin1String("qtest_null"), __FILE__ )
               << qTableName( QLatin1String("qtest_blob"), __FILE__ )
               << qTableName( QLatin1String("qtest_bittest"), __FILE__ )
               << qTableName( QLatin1String("qtest_nullblob"), __FILE__ )
               << qTableName( QLatin1String("qtest_rawtest"), __FILE__ )
               << qTableName( QLatin1String("qtest_precision"), __FILE__ )
               << qTableName( QLatin1String("qtest_prepare"), __FILE__ )
               << qTableName( QLatin1String("qtestj1"), __FILE__ )
               << qTableName( QLatin1String("qtestj2"), __FILE__ )
               << qTableName( QLatin1String("char1Select"), __FILE__ )
               << qTableName( QLatin1String("char1SU"), __FILE__ )
               << qTableName( QLatin1String("qxmltest"), __FILE__ )
               << qTableName( QLatin1String("qtest_exerr"), __FILE__ )
               << qTableName( QLatin1String("qtest_empty"), __FILE__ )
               << qTableName( QLatin1String("clobby"), __FILE__ )
               << qTableName( QLatin1String("bindtest"), __FILE__ )
               << qTableName( QLatin1String("more_results"), __FILE__ )
               << qTableName( QLatin1String("blobstest"), __FILE__ )
               << qTableName( QLatin1String("oraRowId"), __FILE__ )
               << qTableName( QLatin1String("qtest_batch"), __FILE__ )
               << qTableName( QLatin1String("bug6421"), __FILE__).toUpper()
               << qTableName( QLatin1String("bug5765"), __FILE__)
               << qTableName( QLatin1String("bug6852"), __FILE__)
               << qTableName( QLatin1String("qtest_lockedtable"), __FILE__ )
               << qTableName( QLatin1String("Planet"), __FILE__ )
               << qTableName( QLatin1String("task_250026"), __FILE__ )
               << qTableName( QLatin1String("task_234422"), __FILE__ )
               << qTableName( QLatin1String("test141895"), __FILE__)
               << qTableName( QLatin1String("qtest_oraOCINumber"), __FILE__);

    if ( db.driverName().startsWith( QLatin1String("QPSQL") ) )
        tablenames << qTableName( QLatin1String("task_233829"), __FILE__);

    if ( db.driverName().startsWith( QLatin1String("QSQLITE") ) )
        tablenames << qTableName( QLatin1String("record_sqlite"), __FILE__ );

    if ( tst_Databases::isSqlServer( db ) )
        tablenames << qTableName( QLatin1String("qtest_longstr"), __FILE__ );

    if (tst_Databases::isSqlServer( db ))
        db.exec( QLatin1String("DROP PROCEDURE ") + qTableName( QLatin1String("test141895_proc"), __FILE__));

    if (tst_Databases::isMySQL( db ))
        db.exec( QLatin1String("DROP PROCEDURE IF EXISTS ") + qTableName( QLatin1String("bug6852_proc"), __FILE__));

    tst_Databases::safeDropTables( db, tablenames );
}

void tst_QSqlQuery::createTestTables( QSqlDatabase db )
{
    QSqlQuery q( db );

    if ( db.driverName().startsWith( QLatin1String("QMYSQL") ) )
        // ### stupid workaround until we find a way to hardcode this
        // in the MySQL server startup script
        q.exec(  QLatin1String("set table_type=innodb") );
    else if (tst_Databases::isPostgreSQL(db))
        QVERIFY_SQL( q, exec( QLatin1String("set client_min_messages='warning'") ));

    if (tst_Databases::isPostgreSQL(db))
        QVERIFY_SQL( q, exec(  QLatin1String("create table ") + qtest +  QLatin1String(" (id serial NOT NULL, t_varchar varchar(20), t_char char(20), primary key(id)) WITH OIDS") ) );
    else
        QVERIFY_SQL( q, exec(  QLatin1String("create table ") + qtest +  QLatin1String(" (id int ") + tst_Databases::autoFieldName(db) +  QLatin1String(" NOT NULL, t_varchar varchar(20), t_char char(20), primary key(id))") ) );

    if ( tst_Databases::isSqlServer( db ) || db.driverName().startsWith( QLatin1String("QTDS") ) )
        QVERIFY_SQL( q, exec( QLatin1String("create table ") + qTableName( QLatin1String( "qtest_null"), __FILE__ ) + QLatin1String(" (id int null, t_varchar varchar(20) null)") ) );
    else
        QVERIFY_SQL( q, exec( QLatin1String("create table ") + qTableName( QLatin1String("qtest_null"), __FILE__ ) + QLatin1String(" (id int, t_varchar varchar(20))") ) );
}

void tst_QSqlQuery::populateTestTables( QSqlDatabase db )
{
    QSqlQuery q( db );
    const QString qtest_null(qTableName( QLatin1String("qtest_null"), __FILE__ ));
    q.exec( QLatin1String("delete from ") + qtest );
    QVERIFY_SQL( q, exec( QLatin1String("insert into ") + qtest + QLatin1String(" values (1, 'VarChar1', 'Char1')") ) );
    QVERIFY_SQL( q, exec( QLatin1String("insert into ") + qtest + QLatin1String(" values (2, 'VarChar2', 'Char2')") ) );
    QVERIFY_SQL( q, exec( QLatin1String("insert into ") + qtest + QLatin1String(" values (3, 'VarChar3', 'Char3')") ) );
    QVERIFY_SQL( q, exec( QLatin1String("insert into ") + qtest + QLatin1String(" values (4, 'VarChar4', 'Char4')") ) );
    QVERIFY_SQL( q, exec( QLatin1String("insert into ") + qtest + QLatin1String(" values (5, 'VarChar5', 'Char5')") ) );

    q.exec( QLatin1String("delete from ") + qtest_null );
    QVERIFY_SQL( q, exec( QLatin1String("insert into ") + qtest_null + QLatin1String(" values (0, NULL)") ) );
    QVERIFY_SQL( q, exec( QLatin1String("insert into ") + qtest_null + QLatin1String(" values (1, 'n')") ) );
    QVERIFY_SQL( q, exec( QLatin1String("insert into ") + qtest_null + QLatin1String(" values (2, 'i')") ) );
    QVERIFY_SQL( q, exec( QLatin1String("insert into ") + qtest_null + QLatin1String(" values (3, NULL)") ) );
}

void tst_QSqlQuery::benchmark()
{
    QFETCH( QString, dbName );
    QSqlDatabase db = QSqlDatabase::database( dbName );
    CHECK_DATABASE( db );
    if ( tst_Databases::getMySqlVersion( db ).section( QLatin1Char('.'), 0, 0 ).toInt()<5 )
        QSKIP( "Test requires MySQL >= 5.0", SkipSingle );

    QSqlQuery q(db);
    const QString tableName(qTableName(QLatin1String("benchmark"), __FILE__));

    tst_Databases::safeDropTable( db, tableName );

    QVERIFY_SQL(q, exec(QLatin1String("CREATE TABLE ") + tableName + QLatin1String("(\n"
                        "MainKey INT NOT NULL,\n"
                        "OtherTextCol VARCHAR(45) NOT NULL,\n"
                        "PRIMARY KEY(`MainKey`))")));

    int i=1;

    QBENCHMARK {
        QVERIFY_SQL(q, exec(QLatin1String("INSERT INTO ") + tableName + QLatin1String(" VALUES(")
            + QString::number(i) + QLatin1String(", \"Value") + QString::number(i) + QLatin1String("\")")));
        i++;
    }

    tst_Databases::safeDropTable( db, tableName );
}

#include "moc_main.cpp"
