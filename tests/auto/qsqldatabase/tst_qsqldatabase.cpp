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
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqldriver.h>
#include <qsqlrecord.h>
#include <qsqlfield.h>
#include <qsqlindex.h>
#include <qregexp.h>
#include <qvariant.h>
#include <qdatetime.h>
#include <qdebug.h>

#define NODATABASE_SKIP "No database drivers are available in this Qt configuration"

QT_USE_NAMESPACE

#include "tst_databases.h"

//TESTED_FILES=

struct FieldDef;

class tst_QSqlDatabase : public QObject
{
    Q_OBJECT

public:
    tst_QSqlDatabase();
    virtual ~tst_QSqlDatabase();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
private slots:
    void record_data() { generic_data(); }
    //void record();
    void open_data() { generic_data(); }
    void open();
    void tables_data() { generic_data(); }
    void tables();
    void transaction_data() { generic_data(); }
    void transaction();
    void eventNotification_data() { generic_data(); }
    void eventNotification();
    void addDatabase();

    //database specific tests
    void recordSQLite_data() { generic_data("QSQLITE"); }
    void recordSQLite();
    void recordAccess_data() { generic_data("QODBC"); }
    void recordAccess();
    void recordSQLServer_data() { generic_data("QODBC"); }
    void recordSQLServer();

    //database specific 64 bit integer test
    void bigIntField_data() { generic_data(); }
    void bigIntField();

    // general tests
    void getConnectionName_data() { generic_data(); }
    void getConnectionName(); // For task 129992

    //problem specific tests
    void alterTable_data() { generic_data(); }
    void alterTable();
    void caseSensivity_data() { generic_data(); }
    void caseSensivity();
    void noEscapedFieldNamesInRecord_data() { generic_data(); }
    void noEscapedFieldNamesInRecord();
    void whitespaceInIdentifiers_data() { generic_data(); }
    void whitespaceInIdentifiers();
    void formatValueTrimStrings_data() { generic_data(); }
    void formatValueTrimStrings();
    void precisionPolicy_data() { generic_data(); }
    void precisionPolicy();

    void mysqlOdbc_unsignedIntegers_data() { generic_data(); }
    void mysqlOdbc_unsignedIntegers();

    void accessOdbc_strings_data() { generic_data(); }
    void accessOdbc_strings();

    void odbc_reopenDatabase_data() { generic_data("QODBC"); }
    void odbc_reopenDatabase();
    void odbc_uniqueidentifier_data() { generic_data("QODBC"); }
    void odbc_uniqueidentifier(); // For task 141822
    void odbc_uintfield_data() { generic_data("QODBC"); }
    void odbc_uintfield();
    void odbc_bindBoolean_data() { generic_data("QODBC"); }
    void odbc_bindBoolean();
    void odbc_testqGetString_data() { generic_data("QODBC"); }
    void odbc_testqGetString();

    void sqlite_bindAndFetchUInt_data() { generic_data("QSQLITE"); }
    void sqlite_bindAndFetchUInt();

    void sqlStatementUseIsNull_189093_data() { generic_data(); }
    void sqlStatementUseIsNull_189093();

    void sqlite_enable_cache_mode_data() { generic_data("QSQLITE"); }
    void sqlite_enable_cache_mode();

private:
    void createTestTables(QSqlDatabase db);
    void dropTestTables(QSqlDatabase db);
    void populateTestTables(QSqlDatabase db);
    void generic_data(const QString &engine=QString());

    void testRecord(const FieldDef fieldDefs[], const QSqlRecord& inf, QSqlDatabase db);
    void commonFieldTest(const FieldDef fieldDefs[], QSqlDatabase, const int);

    tst_Databases dbs;
};

// number of records to be inserted per testfunction
static const int ITERATION_COUNT = 2;

//helper class for database specific tests
struct FieldDef {
    FieldDef(QString tn = QString(),
          QVariant::Type t = QVariant::Invalid,
          QVariant v = QVariant(),
          bool nl = true):
        typeName(tn), type(t), val(v), nullable(nl) {}

    QString fieldName() const
    {
        QString rt = typeName;
        rt.replace(QRegExp("\\s"), QString("_"));
        int i = rt.indexOf("(");
        if (i == -1)
            i = rt.length();
        if (i > 20)
            i = 20;
        return "t_" + rt.left(i);
    }
    QString typeName;
    QVariant::Type type;
    QVariant val;
    bool nullable;
};

// creates a table out of the FieldDefs and returns the number of fields
// excluding the primary key field
static int createFieldTable(const FieldDef fieldDefs[], QSqlDatabase db)
{
    tst_Databases::safeDropTable(db, qTableName("qtestfields", __FILE__));
    QSqlQuery q(db);
    // construct a create table statement consisting of all fieldtypes
    QString qs = "create table " + qTableName("qtestfields", __FILE__);
    if (tst_Databases::isMSAccess(db))
        qs.append(" (id int not null");
    else if (tst_Databases::isPostgreSQL(db))
        qs.append(" (id serial not null");
    else
        qs.append(QString("(id integer not null primary key"));

    int i = 0;
    for (i = 0; !fieldDefs[ i ].typeName.isNull(); ++i) {
        qs += QString(",\n %1 %2").arg(fieldDefs[ i ].fieldName()).arg(fieldDefs[ i ].typeName);
        if (tst_Databases::isSqlServer(db) && fieldDefs[ i ].nullable) {
            qs += " null";
        }
    }

    if (tst_Databases::isMSAccess(db))
        qs.append(",\n primary key (id)");

    qs += ')';
    if (!q.exec(qs)) {
        qDebug() << "Creation of Table failed:" << tst_Databases::printError(q.lastError(), db);
        qDebug() << "Query: " << qs;
        return -1;
    }
    return i;
}

tst_QSqlDatabase::tst_QSqlDatabase()
{
}

tst_QSqlDatabase::~tst_QSqlDatabase()
{
}

void tst_QSqlDatabase::createTestTables(QSqlDatabase db)
{
    if (!db.isValid())
    return;
    QSqlQuery q(db);
    if (tst_Databases::isSqlServer(db)) {
        QVERIFY_SQL(q, exec("SET ANSI_DEFAULTS ON"));
        QVERIFY_SQL(q, exec("SET IMPLICIT_TRANSACTIONS OFF"));
    } else if(tst_Databases::isPostgreSQL(db))
        QVERIFY_SQL( q, exec("set client_min_messages='warning'"));

    // please never ever change this table; otherwise fix all tests ;)
    if (tst_Databases::isMSAccess(db)) {
        QVERIFY_SQL(q, exec("create table " + qTableName("qtest", __FILE__) +
                   " (id int not null, t_varchar varchar(40) not null, t_char char(40), "
                   "t_numeric number, primary key (id, t_varchar))"));
    } else {
        QVERIFY_SQL(q, exec("create table " + qTableName("qtest", __FILE__) +
               " (id integer not null, t_varchar varchar(40) not null, "
               "t_char char(40), t_numeric numeric(6, 3), primary key (id, t_varchar))"));
    }

    if (testWhiteSpaceNames(db.driverName())) {
        QString qry = "create table "
            + db.driver()->escapeIdentifier(qTableName("qtest", __FILE__) + " test", QSqlDriver::TableName)
            + '('
            + db.driver()->escapeIdentifier(QLatin1String("test test"), QSqlDriver::FieldName)
            + " int not null primary key)";
        QVERIFY_SQL(q, exec(qry));
    }
}

void tst_QSqlDatabase::dropTestTables(QSqlDatabase db)
{
    if (!db.isValid())
        return;

    if(tst_Databases::isPostgreSQL(db)) {
        QSqlQuery q(db);
        QVERIFY_SQL( q, exec("set client_min_messages='warning'"));
    }

    // drop the view first, otherwise we'll get dependency problems
    tst_Databases::safeDropViews(db, QStringList() << qTableName("qtest_view", __FILE__) << qTableName("qtest_view2", __FILE__));

    QStringList tableNames;
    tableNames << qTableName("qtest", __FILE__)
            << qTableName("qtestfields", __FILE__)
            << qTableName("qtestalter", __FILE__)
            << qTableName("qtest_temp", __FILE__)
            << qTableName("qtest_bigint", __FILE__)
            << qTableName("qtest_xmltype", __FILE__)
            << qTableName("latin1table", __FILE__)
            << qTableName("qtest_sqlguid", __FILE__)
            << qTableName("batable", __FILE__)
            << qTableName("qtest_prec", __FILE__)
            << qTableName("uint", __FILE__)
            << qTableName("strings", __FILE__)
            << qTableName("numericfields", __FILE__)
            << qTableName("qtestBindBool", __FILE__)
            << qTableName("testqGetString", __FILE__)
            << qTableName("qtest_sqlguid", __FILE__)
            << qTableName("uint_table", __FILE__)
            << qTableName("uint_test", __FILE__);

    if (testWhiteSpaceNames(db.driverName()))
        tableNames <<  db.driver()->escapeIdentifier(qTableName("qtest", __FILE__) + " test", QSqlDriver::TableName);

    tst_Databases::safeDropTables(db, tableNames);
}

void tst_QSqlDatabase::populateTestTables(QSqlDatabase db)
{
    if (!db.isValid())
        return;
    QSqlQuery q(db);
    const QString qtest(qTableName("qtest", __FILE__));

    q.exec("delete from " + qtest); //non-fatal
    QVERIFY_SQL(q, exec("insert into " + qtest + " (id, t_varchar, t_char, t_numeric) values (0, 'VarChar0', 'Char0', 1.1)"));
    QVERIFY_SQL(q, exec("insert into " + qtest + " (id, t_varchar, t_char, t_numeric) values (1, 'VarChar1', 'Char1', 2.2)"));
    QVERIFY_SQL(q, exec("insert into " + qtest + " (id, t_varchar, t_char, t_numeric) values (2, 'VarChar2', 'Char2', 3.3)"));
    QVERIFY_SQL(q, exec("insert into " + qtest + " (id, t_varchar, t_char, t_numeric) values (3, 'VarChar3', 'Char3', 4.4)"));
    QVERIFY_SQL(q, exec("insert into " + qtest + " (id, t_varchar, t_char, t_numeric) values (4, 'VarChar4', NULL, NULL)"));
}

void tst_QSqlDatabase::initTestCase()
{
    dbs.open();

    foreach (const QString &name, dbs.dbNames) {
        QSqlDatabase db = QSqlDatabase::database(name);
        CHECK_DATABASE(db);
        dropTestTables(db); //in case of leftovers
        createTestTables(db);
        populateTestTables(db);
    }
}

void tst_QSqlDatabase::cleanupTestCase()
{
    foreach (const QString &name, dbs.dbNames) {
        QSqlDatabase db = QSqlDatabase::database(name);
        CHECK_DATABASE(db);
        dropTestTables(db);
    }

    dbs.close();
}

void tst_QSqlDatabase::init()
{
}

void tst_QSqlDatabase::cleanup()
{
}

void tst_QSqlDatabase::generic_data(const QString& engine)
{
    if ( dbs.fillTestTable(engine) == 0 ) {
        if(engine.isEmpty())
           QSKIP( "No database drivers are available in this Qt configuration", SkipAll );
        else
           QSKIP( (QString("No database drivers of type %1 are available in this Qt configuration").arg(engine)).toLocal8Bit(), SkipAll );
    }
}

void tst_QSqlDatabase::addDatabase()
{
    QTest::ignoreMessage(QtWarningMsg, "QSqlDatabase: BLAH_FOO_NONEXISTENT_DRIVER driver not loaded");
    QTest::ignoreMessage(QtWarningMsg, qPrintable("QSqlDatabase: available drivers: " + QSqlDatabase::drivers().join(QLatin1String(" "))));
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("BLAH_FOO_NONEXISTENT_DRIVER",
                                                    "INVALID_CONNECTION");
        QVERIFY(!db.isValid());
    }
    QVERIFY(QSqlDatabase::contains("INVALID_CONNECTION"));
    QSqlDatabase::removeDatabase("INVALID_CONNECTION");
    QVERIFY(!QSqlDatabase::contains("INVALID_CONNECTION"));
}

void tst_QSqlDatabase::open()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    int i;
    for (i = 0; i < 10; ++i) {
        db.close();
        QVERIFY(!db.isOpen());
        QVERIFY_SQL(db, open());
        QVERIFY(db.isOpen());
        QVERIFY(!db.isOpenError());
    }

    if (db.driverName().startsWith("QSQLITE") && db.databaseName() == ":memory:") {
        // tables in in-memory databases don't survive an open/close
        createTestTables(db);
        populateTestTables(db);
    }
}

void tst_QSqlDatabase::tables()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    const QString qtest(qTableName("qtest", __FILE__)), qtest_view(qTableName("qtest_view", __FILE__)), temp_tab(qTableName("test_tab", __FILE__));

    bool views = true;
    bool tempTables = false;

    QSqlQuery q(db);

    if (!q.exec("CREATE VIEW " + qtest_view + " as select * from " + qtest)) {
        qDebug() << "DBMS '" << tst_Databases::dbToString(db) << "' cannot handle VIEWs:"
            << tst_Databases::printError(q.lastError());
        views = false;
    }

    if (db.driverName().startsWith("QSQLITE3")) {
        QVERIFY_SQL(q, exec("CREATE TEMPORARY TABLE " + temp_tab + " (id int)"));
        tempTables = true;
    }

    QStringList tables = db.tables(QSql::Tables);
    QVERIFY(tables.contains(qtest, Qt::CaseInsensitive));
    QVERIFY(!tables.contains("sql_features", Qt::CaseInsensitive)); //check for postgres 7.4 internal tables
    if (views) {
        QVERIFY(!tables.contains(qtest_view, Qt::CaseInsensitive));
    }
    if (tempTables)
        QVERIFY(tables.contains(temp_tab, Qt::CaseInsensitive));

    tables = db.tables(QSql::Views);
    if (views) {
        if(!tables.contains(qtest_view, Qt::CaseInsensitive))
            qDebug() << "failed to find" << qtest_view << "in" << tables;
        QVERIFY(tables.contains(qtest_view, Qt::CaseInsensitive));
    }
    if (tempTables)
        QVERIFY(!tables.contains(temp_tab, Qt::CaseInsensitive));
    QVERIFY(!tables.contains(qtest, Qt::CaseInsensitive));

    tables = db.tables(QSql::SystemTables);
    QVERIFY(!tables.contains(qtest, Qt::CaseInsensitive));
    QVERIFY(!tables.contains(qtest_view, Qt::CaseInsensitive));
    QVERIFY(!tables.contains(temp_tab, Qt::CaseInsensitive));

    tables = db.tables(QSql::AllTables);
    if (views)
        QVERIFY(tables.contains(qtest_view, Qt::CaseInsensitive));
    if (tempTables)
        QVERIFY(tables.contains(temp_tab, Qt::CaseInsensitive));
    QVERIFY(tables.contains(qtest, Qt::CaseInsensitive));
}

void tst_QSqlDatabase::whitespaceInIdentifiers()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    if (testWhiteSpaceNames(db.driverName())) {
        const QString tableName(qTableName("qtest", __FILE__) + " test");
        QVERIFY(db.tables().contains(tableName, Qt::CaseInsensitive));

        QSqlRecord rec = db.record(db.driver()->escapeIdentifier(tableName, QSqlDriver::TableName));
        QCOMPARE(rec.count(), 1);
        QCOMPARE(rec.fieldName(0), QString("test test"));
        QCOMPARE(rec.field(0).type(), QVariant::Int);

        QSqlIndex idx = db.primaryIndex(db.driver()->escapeIdentifier(tableName, QSqlDriver::TableName));
        QCOMPARE(idx.count(), 1);
        QCOMPARE(idx.fieldName(0), QString("test test"));
        QCOMPARE(idx.field(0).type(), QVariant::Int);
    } else {
        QSKIP("DBMS does not support whitespaces in identifiers", SkipSingle);
    }
}

void tst_QSqlDatabase::alterTable()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);
    const QString qtestalter(qTableName("qtestalter", __FILE__));

    QSqlQuery q(db);

    QVERIFY_SQL(q, exec("create table " + qtestalter + " (F1 char(20), F2 char(20), F3 char(20))"));
    QSqlRecord rec = db.record(qtestalter);
    QCOMPARE((int)rec.count(), 3);

    for (int i = 0; i < 3; ++i) {
        QCOMPARE(rec.field(i).name().toUpper(), QString("F%1").arg(i + 1));
    }

    if (!q.exec("alter table " + qtestalter + " drop column F2")) {
        QSKIP("DBMS doesn't support dropping columns in ALTER TABLE statement", SkipSingle);
    }

    rec = db.record(qtestalter);

    QCOMPARE((int)rec.count(), 2);

    QCOMPARE(rec.field(0).name().toUpper(), QString("F1"));
    QCOMPARE(rec.field(1).name().toUpper(), QString("F3"));

    q.exec("select * from " + qtestalter);
}

#if 0
// this is the general test that should work on all databases.
// unfortunately no DBMS supports SQL 92/ 99 so the general
// test is more or less a joke. Please write a test for each
// database plugin (see recordSQLite and so on). Use this test
// as a template.
void tst_QSqlDatabase::record()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    static const FieldDef fieldDefs[] = {
        FieldDef("char(20)", QVariant::String,         QString("blah1"), false),
        FieldDef("varchar(20)", QVariant::String,      QString("blah2"), false),
        FieldDef()
    };

    const int fieldCount = createFieldTable(fieldDefs, db);
    QVERIFY(fieldCount > 0);

    commonFieldTest(fieldDefs, db, fieldCount);
}
#endif

void tst_QSqlDatabase::testRecord(const FieldDef fieldDefs[], const QSqlRecord& inf, QSqlDatabase db)
{
    int i = 0;
    for (i = 0; !fieldDefs[ i ].typeName.isNull(); ++i) {
        QCOMPARE(inf.field(i+1).name().toUpper(), fieldDefs[ i ].fieldName().toUpper());
        if (inf.field(i+1).type() != fieldDefs[ i ].type) {
            QFAIL(qPrintable(QString(" Expected: '%1' Received: '%2' for field %3 in testRecord").arg(
              QVariant::typeToName(fieldDefs[ i ].type)).arg(
            QVariant::typeToName(inf.field(i+1).type())).arg(
              fieldDefs[ i ].fieldName())));
        }
        QVERIFY(!inf.field(i+1).isAutoValue());

//	qDebug(QString(" field: %1 type: %2 variant type: %3").arg(fieldDefs[ i ].fieldName()).arg(QVariant::typeToName(inf.field(i+1)->type())).arg(QVariant::typeToName(inf.field(i+1)->value().type())));
    }
}

// non-dbms specific tests
void tst_QSqlDatabase::commonFieldTest(const FieldDef fieldDefs[], QSqlDatabase db, const int fieldCount)
{
    CHECK_DATABASE(db);

    // check whether recordInfo returns the right types
    QSqlRecord rec = db.record(qTableName("qtestfields", __FILE__));
    QCOMPARE((int)rec.count(), fieldCount+1);
    testRecord(fieldDefs, rec, db);

    QSqlQuery q(db);
    QVERIFY_SQL(q, exec("select * from " + qTableName("qtestfields", __FILE__)));
}

void tst_QSqlDatabase::recordSQLite()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    static const FieldDef fieldDefs[] = {
        // The affinity of these fields are TEXT so SQLite should give us strings, not ints or doubles.
        FieldDef("char(20)", QVariant::String,          QString("123")),
        FieldDef("varchar(20)", QVariant::String,       QString("123.4")),
        FieldDef("clob", QVariant::String,              QString("123.45")),
        FieldDef("text", QVariant::String,              QString("123.456")),

        FieldDef("integer", QVariant::Int,              QVariant(13)),
        FieldDef("int", QVariant::Int,                  QVariant(12)),
        FieldDef("real", QVariant::Double,              QVariant(1.234567890123456)),

        FieldDef()
    };

    const int fieldCount = createFieldTable(fieldDefs, db);
    QVERIFY(fieldCount > 0);

    commonFieldTest(fieldDefs, db, fieldCount);
}

void tst_QSqlDatabase::recordSQLServer()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    if (!tst_Databases::isSqlServer(db)) {
    QSKIP("SQL server specific test", SkipSingle);
    return;
    }

    // ### TODO: Add the rest of the fields
    static const FieldDef fieldDefs[] = {
        FieldDef("varchar(20)", QVariant::String, QString("Blah1")),
        FieldDef("bigint", QVariant::LongLong, 12345),
        FieldDef("int", QVariant::Int, 123456),
        FieldDef("tinyint", QVariant::UInt, 255),
        FieldDef("float", QVariant::Double, 1.12345),
        FieldDef("numeric(5,2)", QVariant::Double, 123.45),
        FieldDef("uniqueidentifier", QVariant::String,
            QString("AA7DF450-F119-11CD-8465-00AA00425D90")),

        FieldDef()
    };

    const int fieldCount = createFieldTable(fieldDefs, db);
    QVERIFY(fieldCount > 0);

    commonFieldTest(fieldDefs, db, fieldCount);
}

void tst_QSqlDatabase::recordAccess()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    if (!tst_Databases::isMSAccess(db)) {
    QSKIP("MS Access specific test", SkipSingle);
    return;
    }

    QString memo;
    for (int i = 0; i < 32; i++)
        memo.append("ABCDEFGH12345678abcdefgh12345678");

    // ### TODO: Add the rest of the fields
    static const FieldDef fieldDefs[] = {
    FieldDef("varchar(20)", QVariant::String, QString("Blah1")),
    FieldDef("single", QVariant::Double, 1.12345),
    FieldDef("double", QVariant::Double, 1.123456),
    FieldDef("byte", QVariant::UInt, 255),
    FieldDef("long", QVariant::Int, 2147483647),
        FieldDef("memo", QVariant::String, memo),
    FieldDef()
    };

    const int fieldCount = createFieldTable(fieldDefs, db);
    QVERIFY(fieldCount > 0);

    commonFieldTest(fieldDefs, db, fieldCount);
}

void tst_QSqlDatabase::transaction()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);
    const QString qtest(qTableName("qtest", __FILE__));

    if (!db.driver()->hasFeature(QSqlDriver::Transactions)) {
    QSKIP("DBMS not transaction capable", SkipSingle);
    }

    QVERIFY(db.transaction());

    QSqlQuery q(db);
    QVERIFY_SQL(q, exec("insert into " + qtest + " values (40, 'VarChar40', 'Char40', 40.40)"));
    QVERIFY_SQL(q, exec("select * from " + qtest + " where id = 40"));
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toInt(), 40);
    q.clear();

    QVERIFY(db.commit());

    QVERIFY(db.transaction());
    QVERIFY_SQL(q, exec("select * from " + qtest + " where id = 40"));
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toInt(), 40);
    q.clear();
    QVERIFY(db.commit());

    QVERIFY(db.transaction());
    QVERIFY_SQL(q, exec("insert into " + qtest + " values (41, 'VarChar41', 'Char41', 41.41)"));
    QVERIFY_SQL(q, exec("select * from " + qtest + " where id = 41"));
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toInt(), 41);
    q.clear(); // for SQLite which does not allow any references on rows that shall be rolled back
    if (!db.rollback()) {
        QFAIL("Could not rollback transaction: " + tst_Databases::printError(db.lastError()));
    }

    QVERIFY_SQL(q, exec("select * from " + qtest + " where id = 41"));
    if(db.driverName().startsWith("QODBC") && dbName.contains("MySQL"))
        QEXPECT_FAIL("", "Some odbc drivers don't actually roll back despite telling us they do, especially the mysql driver", Continue);
    QVERIFY(!q.next());

    populateTestTables(db);
}

void tst_QSqlDatabase::bigIntField()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);
    QString drvName = db.driverName();
    const QString qtest_bigint(qTableName("qtest_bigint", __FILE__));

    QSqlQuery q(db);
    q.setForwardOnly(true);

    if (tst_Databases::isSqlServer(db)) {
        QVERIFY_SQL(q, exec("create table " + qtest_bigint + "(id int, t_s64bit bigint, t_u64bit bigint)"));
    } else {
        QSKIP("no 64 bit integer support", SkipAll);
    }
    QVERIFY(q.prepare("insert into " + qtest_bigint + " values (?, ?, ?)"));
    qlonglong ll = Q_INT64_C(9223372036854775807);
    qulonglong ull = Q_UINT64_C(18446744073709551615);


    // usinged bigint fields not supported - a cast is necessary
    q.bindValue(0, 0);
    q.bindValue(1, ll);
    q.bindValue(2, (qlonglong) ull);
    QVERIFY_SQL(q, exec());
    q.bindValue(0, 1);
    q.bindValue(1, -ll);
    q.bindValue(2,  (qlonglong) ull);
    QVERIFY_SQL(q, exec());
    QVERIFY(q.exec("select * from " + qtest_bigint + " order by id"));
    QVERIFY(q.next());
    QCOMPARE(q.value(1).toDouble(), (double)ll);
    QCOMPARE(q.value(1).toLongLong(), ll);
    QCOMPARE(q.value(2).toULongLong(), ull);
    QVERIFY(q.next());
    QCOMPARE(q.value(1).toLongLong(), -ll);
    QCOMPARE(q.value(2).toULongLong(), ull);
}

void tst_QSqlDatabase::caseSensivity()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    bool cs = false;
    if (db.driverName().startsWith("QSQLITE")
     || db.driverName().startsWith("QODBC"))
    cs = true;

    QSqlRecord rec = db.record(qTableName("qtest", __FILE__));
    QVERIFY((int)rec.count() > 0);
    if (!cs) {
    rec = db.record(qTableName("QTEST", __FILE__).toUpper());
    QVERIFY((int)rec.count() > 0);
    rec = db.record(qTableName("qTesT", __FILE__));
    QVERIFY((int)rec.count() > 0);
    }

    rec = db.primaryIndex(qTableName("qtest", __FILE__));
    QVERIFY((int)rec.count() > 0);
    if (!cs) {
    rec = db.primaryIndex(qTableName("QTEST", __FILE__).toUpper());
    QVERIFY((int)rec.count() > 0);
    rec = db.primaryIndex(qTableName("qTesT", __FILE__));
    QVERIFY((int)rec.count() > 0);
    }
}

void tst_QSqlDatabase::noEscapedFieldNamesInRecord()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    QString fieldname("t_varchar");

    QSqlQuery q(db);
    QString query = "SELECT " + db.driver()->escapeIdentifier(fieldname, QSqlDriver::FieldName) + " FROM " + qTableName("qtest", __FILE__);
    QVERIFY_SQL(q, exec(query));
    QCOMPARE(q.record().fieldName(0), fieldname);
}

// This test should be rewritten to work with Oracle as well - or the Oracle driver
// should be fixed to make this test pass (handle overflows)
void tst_QSqlDatabase::precisionPolicy()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    QSqlQuery q(db);
    const QString tableName(qTableName("qtest_prec", __FILE__));
    if(!db.driver()->hasFeature(QSqlDriver::LowPrecisionNumbers))
        QSKIP("Driver or database doesn't support setting precision policy", SkipSingle);

    // Create a test table with some data
    if(tst_Databases::isMSAccess(db))
        QVERIFY_SQL(q, exec(QString("CREATE TABLE %1 (id smallint, num number)").arg(tableName)));
    else
        QVERIFY_SQL(q, exec(QString("CREATE TABLE %1 (id smallint, num numeric(18,5))").arg(tableName)));
    QVERIFY_SQL(q, prepare(QString("INSERT INTO %1 VALUES (?, ?)").arg(tableName)));
    q.bindValue(0, 1);
    q.bindValue(1, 123);
    QVERIFY_SQL(q, exec());
    q.bindValue(0, 2);
    q.bindValue(1, 1850000000000.0001);
    QVERIFY_SQL(q, exec());

    // These are expected to pass
    q.setNumericalPrecisionPolicy(QSql::HighPrecision);
    QString query = QString("SELECT num FROM %1 WHERE id = 1").arg(tableName);
    QVERIFY_SQL(q, exec(query));
    QVERIFY_SQL(q, next());
    if(db.driverName().startsWith("QSQLITE"))
        QEXPECT_FAIL("", "SQLite returns this value as determined by contents of the field, not the declaration", Continue);
    QCOMPARE(q.value(0).type(), QVariant::String);

    q.setNumericalPrecisionPolicy(QSql::LowPrecisionInt64);
    QVERIFY_SQL(q, exec(query));
    QVERIFY_SQL(q, next());
    if(q.value(0).type() != QVariant::LongLong)
        QEXPECT_FAIL("", "SQLite returns this value as determined by contents of the field, not the declaration", Continue);
    QCOMPARE(q.value(0).type(), QVariant::LongLong);
    QCOMPARE(q.value(0).toLongLong(), (qlonglong)123);

    q.setNumericalPrecisionPolicy(QSql::LowPrecisionInt32);
    QVERIFY_SQL(q, exec(query));
    QVERIFY_SQL(q, next());
    if(db.driverName().startsWith("QSQLITE"))
        QEXPECT_FAIL("", "SQLite returns this value as determined by contents of the field, not the declaration", Continue);
    QCOMPARE(q.value(0).type(), QVariant::Int);
    QCOMPARE(q.value(0).toInt(), 123);

    q.setNumericalPrecisionPolicy(QSql::LowPrecisionDouble);
    QVERIFY_SQL(q, exec(query));
    QVERIFY_SQL(q, next());
    if(db.driverName().startsWith("QSQLITE"))
        QEXPECT_FAIL("", "SQLite returns this value as determined by contents of the field, not the declaration", Continue);
    QCOMPARE(q.value(0).type(), QVariant::Double);
    QCOMPARE(q.value(0).toDouble(), (double)123);

    query = QString("SELECT num FROM %1 WHERE id = 2").arg(tableName);
    QVERIFY_SQL(q, exec(query));
    QVERIFY_SQL(q, next());
    if(db.driverName().startsWith("QSQLITE"))
        QEXPECT_FAIL("", "SQLite returns this value as determined by contents of the field, not the declaration", Continue);
    QCOMPARE(q.value(0).type(), QVariant::Double);
    QCOMPARE(q.value(0).toDouble(), QString("1850000000000.0001").toDouble());

    // Postgres returns invalid QVariants on overflow
    q.setNumericalPrecisionPolicy(QSql::HighPrecision);
    QVERIFY_SQL(q, exec(query));
    QVERIFY_SQL(q, next());
    if(db.driverName().startsWith("QSQLITE"))
        QEXPECT_FAIL("", "SQLite returns this value as determined by contents of the field, not the declaration", Continue);
    QCOMPARE(q.value(0).type(), QVariant::String);

    q.setNumericalPrecisionPolicy(QSql::LowPrecisionInt64);
    QVERIFY_SQL(q, exec(query));
    QVERIFY_SQL(q, next());
    QCOMPARE(q.value(0).type(), QVariant::LongLong);

    QSql::NumericalPrecisionPolicy oldPrecision= db.numericalPrecisionPolicy();
    db.setNumericalPrecisionPolicy(QSql::LowPrecisionInt64);
    QSqlQuery q2(db);
    q2.exec(QString("SELECT num FROM %1 WHERE id = 2").arg(tableName));
    QVERIFY_SQL(q2, exec(query));
    QVERIFY_SQL(q2, next());
    QCOMPARE(q2.value(0).type(), QVariant::LongLong);
    db.setNumericalPrecisionPolicy(oldPrecision);
}

// This test needs a ODBC data source containing MYSQL in it's name
void tst_QSqlDatabase::mysqlOdbc_unsignedIntegers()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    if (!db.driverName().startsWith("QODBC") || !dbName.toUpper().contains("MYSQL")) {
       QSKIP("MySQL through ODBC-driver specific test", SkipSingle);
       return;
    }

    QSqlQuery q(db);
    const QString tableName(qTableName("uint", __FILE__));
    QVERIFY_SQL(q, exec(QString("CREATE TABLE %1 (foo integer(10) unsigned, bar integer(10))").arg(tableName)));
    QVERIFY_SQL(q, exec(QString("INSERT INTO %1 VALUES (-4000000000, -4000000000)").arg(tableName)));
    QVERIFY_SQL(q, exec(QString("INSERT INTO %1 VALUES (4000000000, 4000000000)").arg(tableName)));

    QVERIFY_SQL(q, exec(QString("SELECT foo, bar FROM %1").arg(tableName)));
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toString(), QString("0"));
    QCOMPARE(q.value(1).toString(), QString("-2147483648"));
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toString(), QString("4000000000"));
    QCOMPARE(q.value(1).toString(), QString("2147483647"));
}

void tst_QSqlDatabase::accessOdbc_strings()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    if (!tst_Databases::isMSAccess(db)) {
    QSKIP("MS Access specific test", SkipSingle);
    return;
    }

    QSqlQuery q(db);
    const QString tableName(qTableName("strings", __FILE__));
    QVERIFY_SQL(q, exec(QString("CREATE TABLE %1 (aStr memo, bStr memo, cStr memo, dStr memo"
            ", eStr memo, fStr memo, gStr memo, hStr memo)").arg(tableName)));

    QVERIFY_SQL(q, prepare(QString("INSERT INTO %1 VALUES (?, ?, ?, ?, ?, ?, ?, ?)").arg(tableName)));
    QString aStr, bStr, cStr, dStr, eStr, fStr, gStr, hStr;

    q.bindValue(0, aStr.fill('A', 32));
    q.bindValue(1, bStr.fill('B', 127));
    q.bindValue(2, cStr.fill('C', 128));
    q.bindValue(3, dStr.fill('D', 129));
    q.bindValue(4, eStr.fill('E', 254));
    q.bindValue(5, fStr.fill('F', 255));
    q.bindValue(6, gStr.fill('G', 256));
    q.bindValue(7, hStr.fill('H', 512));

    QVERIFY_SQL(q, exec());

    QVERIFY_SQL(q, exec(QString("SELECT aStr, bStr, cStr, dStr, eStr, fStr, gStr, hStr FROM %1").arg(tableName)));
    q.next();
    QCOMPARE(q.value(0).toString(), aStr);
    QCOMPARE(q.value(1).toString(), bStr);
    QCOMPARE(q.value(2).toString(), cStr);
    QCOMPARE(q.value(3).toString(), dStr);
    QCOMPARE(q.value(4).toString(), eStr);
    QCOMPARE(q.value(5).toString(), fStr);
    QCOMPARE(q.value(6).toString(), gStr);
    QCOMPARE(q.value(7).toString(), hStr);
}

void tst_QSqlDatabase::formatValueTrimStrings()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    QSqlQuery q(db);

    QVERIFY_SQL(q, exec(QString("INSERT INTO %1 (id, t_varchar, t_char) values (50, 'Trim Test ', 'Trim Test 2   ')").arg(qTableName("qtest", __FILE__))));
    QVERIFY_SQL(q, exec(QString("INSERT INTO %1 (id, t_varchar, t_char) values (51, 'TrimTest', 'Trim Test 2')").arg(qTableName("qtest", __FILE__))));
    QVERIFY_SQL(q, exec(QString("INSERT INTO %1 (id, t_varchar, t_char) values (52, ' ', '    ')").arg(qTableName("qtest", __FILE__))));

    QVERIFY_SQL(q, exec(QString("SELECT t_varchar, t_char FROM %1 WHERE id >= 50 AND id <= 52 ORDER BY id").arg(qTableName("qtest", __FILE__))));

    QVERIFY_SQL(q, next());

    QCOMPARE(db.driver()->formatValue(q.record().field(0), true), QString("'Trim Test'"));
    QCOMPARE(db.driver()->formatValue(q.record().field(1), true), QString("'Trim Test 2'"));

    QVERIFY_SQL(q, next());
    QCOMPARE(db.driver()->formatValue(q.record().field(0), true), QString("'TrimTest'"));
    QCOMPARE(db.driver()->formatValue(q.record().field(1), true), QString("'Trim Test 2'"));

    QVERIFY_SQL(q, next());
    QCOMPARE(db.driver()->formatValue(q.record().field(0), true), QString("''"));
    QCOMPARE(db.driver()->formatValue(q.record().field(1), true), QString("''"));

}

void tst_QSqlDatabase::odbc_reopenDatabase()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    QSqlQuery q(db);
    QVERIFY_SQL(q, exec("SELECT * from " + qTableName("qtest", __FILE__)));
    QVERIFY_SQL(q, next());
    db.open();
    QVERIFY_SQL(q, exec("SELECT * from " + qTableName("qtest", __FILE__)));
    QVERIFY_SQL(q, next());
    db.open();
}

void tst_QSqlDatabase::odbc_bindBoolean()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    if (tst_Databases::isMySQL(db)) {
        QSKIP("MySql has inconsistent behaviour of bit field type across versions.", SkipSingle);
        return;
    }

    QSqlQuery q(db);
    QVERIFY_SQL(q, exec("CREATE TABLE " + qTableName("qtestBindBool", __FILE__) + "(id int, boolvalue bit)"));

    // Bind and insert
    QVERIFY_SQL(q, prepare("INSERT INTO " + qTableName("qtestBindBool", __FILE__) + " VALUES(?, ?)"));
    q.bindValue(0, 1);
    q.bindValue(1, true);
    QVERIFY_SQL(q, exec());
    q.bindValue(0, 2);
    q.bindValue(1, false);
    QVERIFY_SQL(q, exec());

    // Retrive
    QVERIFY_SQL(q, exec("SELECT id, boolvalue FROM " + qTableName("qtestBindBool", __FILE__) + " ORDER BY id"));
    QVERIFY_SQL(q, next());
    QCOMPARE(q.value(0).toInt(), 1);
    QCOMPARE(q.value(1).toBool(), true);
    QVERIFY_SQL(q, next());
    QCOMPARE(q.value(0).toInt(), 2);
    QCOMPARE(q.value(1).toBool(), false);
}

void tst_QSqlDatabase::odbc_testqGetString()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);
    const QString testqGetString(qTableName("testqGetString", __FILE__));

    QSqlQuery q(db);
    if (tst_Databases::isSqlServer(db))
        QVERIFY_SQL(q, exec("CREATE TABLE " + testqGetString + "(id int, vcvalue varchar(MAX))"));
    else if(tst_Databases::isMSAccess(db))
        QVERIFY_SQL(q, exec("CREATE TABLE " + testqGetString + "(id int, vcvalue memo)"));
    else
        QVERIFY_SQL(q, exec("CREATE TABLE " + testqGetString + "(id int, vcvalue varchar(65538))"));

    QString largeString;
    largeString.fill('A', 65536);

    // Bind and insert
    QVERIFY_SQL(q, prepare("INSERT INTO " + testqGetString + " VALUES(?, ?)"));
    q.bindValue(0, 1);
    q.bindValue(1, largeString);
    QVERIFY_SQL(q, exec());
    q.bindValue(0, 2);
    q.bindValue(1, largeString+QLatin1Char('B'));
    QVERIFY_SQL(q, exec());
    q.bindValue(0, 3);
    q.bindValue(1, largeString+QLatin1Char('B')+QLatin1Char('C'));
    QVERIFY_SQL(q, exec());

    // Retrive
    QVERIFY_SQL(q, exec("SELECT id, vcvalue FROM " + testqGetString + " ORDER BY id"));
    QVERIFY_SQL(q, next());
    QCOMPARE(q.value(0).toInt(), 1);
    QCOMPARE(q.value(1).toString().length(), 65536);
    QVERIFY_SQL(q, next());
    QCOMPARE(q.value(0).toInt(), 2);
    QCOMPARE(q.value(1).toString().length(), 65537);
    QVERIFY_SQL(q, next());
    QCOMPARE(q.value(0).toInt(), 3);
    QCOMPARE(q.value(1).toString().length(), 65538);
}

// This test isn't really necessary as SQL_GUID / uniqueidentifier is
// already tested in recordSQLServer().
void tst_QSqlDatabase::odbc_uniqueidentifier()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);
    if (!tst_Databases::isSqlServer(db)) {
        QSKIP("SQL Server (ODBC) specific test", SkipSingle);
        return;
    }

    const QString tableName(qTableName("qtest_sqlguid", __FILE__));
    QString guid = QString("AAAAAAAA-BBBB-CCCC-DDDD-EEEEEEEEEEEE");
    QString invalidGuid = QString("GAAAAAAA-BBBB-CCCC-DDDD-EEEEEEEEEEEE");

    QSqlQuery q(db);
    QVERIFY_SQL(q, exec(QString("CREATE TABLE %1(id uniqueidentifier)").arg(tableName)));

    q.prepare(QString("INSERT INTO %1 VALUES(?)").arg(tableName));;
    q.addBindValue(guid);
    QVERIFY_SQL(q, exec());

    q.addBindValue(invalidGuid);
    QEXPECT_FAIL("", "The GUID string is required to be correctly formated!",
        Continue);
    QVERIFY_SQL(q, exec());

    QVERIFY_SQL(q, exec(QString("SELECT id FROM %1").arg(tableName)));
    QVERIFY_SQL(q, next());
    QCOMPARE(q.value(0).toString(), guid);
}

void tst_QSqlDatabase::getConnectionName()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    QCOMPARE(db.connectionName(), dbName);
    QSqlDatabase clone = QSqlDatabase::cloneDatabase(db, "clonedDatabase");
    QCOMPARE(clone.connectionName(), QString("clonedDatabase"));
    QTest::ignoreMessage(QtWarningMsg, "QSqlDatabasePrivate::removeDatabase: "
        "connection 'clonedDatabase' is still in use, all queries will cease to work.");
    QSqlDatabase::removeDatabase("clonedDatabase");
    QCOMPARE(clone.connectionName(), QString());
    QCOMPARE(db.connectionName(), dbName);
}

void tst_QSqlDatabase::odbc_uintfield()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    const QString tableName(qTableName("uint_table", __FILE__));
    unsigned int val = 4294967295U;

    QSqlQuery q(db);
    if ( tst_Databases::isMSAccess( db ) )
        QVERIFY_SQL(q, exec(QString("CREATE TABLE %1(num number)").arg(tableName)));
    else
        QVERIFY_SQL(q, exec(QString("CREATE TABLE %1(num numeric(10))").arg(tableName)));
    q.prepare(QString("INSERT INTO %1 VALUES(?)").arg(tableName));
    q.addBindValue(val);
    QVERIFY_SQL(q, exec());

    q.exec(QString("SELECT num FROM %1").arg(tableName));
    if (q.next())
        QCOMPARE(q.value(0).toUInt(), val);
}

void tst_QSqlDatabase::eventNotification()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    QSqlDriver *driver = db.driver();
    if (!driver->hasFeature(QSqlDriver::EventNotifications))
        QSKIP("DBMS doesn't support event notifications", SkipSingle);

    // Not subscribed to any events yet
    QCOMPARE(driver->subscribedToNotifications().size(), 0);

    // Subscribe to "event_foo"
    QVERIFY_SQL(*driver, subscribeToNotification(QLatin1String("event_foo")));
    QCOMPARE(driver->subscribedToNotifications().size(), 1);
    QVERIFY(driver->subscribedToNotifications().contains("event_foo"));

    // Can't subscribe to the same event multiple times
    QVERIFY2(!driver->subscribeToNotification(QLatin1String("event_foo")), "Shouldn't be able to subscribe to event_foo twice");
    QCOMPARE(driver->subscribedToNotifications().size(), 1);

    // Unsubscribe from "event_foo"
    QVERIFY_SQL(*driver, unsubscribeFromNotification(QLatin1String("event_foo")));
    QCOMPARE(driver->subscribedToNotifications().size(), 0);

    // Re-subscribing to "event_foo" now is allowed
    QVERIFY_SQL(*driver, subscribeToNotification(QLatin1String("event_foo")));
    QCOMPARE(driver->subscribedToNotifications().size(), 1);

    // closing the connection causes automatically unsubscription from all events
    db.close();
    QCOMPARE(driver->subscribedToNotifications().size(), 0);

    // Can't subscribe to anything while database is closed
    QVERIFY2(!driver->subscribeToNotification(QLatin1String("event_foo")), "Shouldn't be able to subscribe to event_foo");
    QCOMPARE(driver->subscribedToNotifications().size(), 0);

    db.open();
}

void tst_QSqlDatabase::sqlite_bindAndFetchUInt()
{
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    QSqlQuery q(db);
    const QString tableName(qTableName("uint_test", __FILE__));
    QVERIFY_SQL(q, exec(QString("CREATE TABLE %1(uint_field UNSIGNED INTEGER)").arg(tableName)));
    QVERIFY_SQL(q, prepare(QString("INSERT INTO %1 VALUES(?)").arg(tableName)));
    q.addBindValue(4000000000U);
    QVERIFY_SQL(q, exec());
    QVERIFY_SQL(q, exec(QString("SELECT uint_field FROM %1").arg(tableName)));
    QVERIFY_SQL(q, next());

    // All integers in SQLite are signed, so even though we bound the value
    // as an UInt it will come back as a LongLong
    QCOMPARE(q.value(0).type(), QVariant::LongLong);
    QCOMPARE(q.value(0).toUInt(), 4000000000U);
}

void tst_QSqlDatabase::sqlStatementUseIsNull_189093()
{
    // NULL = NULL is unknown, the sqlStatment must use IS NULL
    QFETCH(QString, dbName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);

    // select a record with NULL value
    QSqlQuery q(QString(), db);
    QVERIFY_SQL(q, exec("select * from " + qTableName("qtest", __FILE__) + " where id = 4"));
    QVERIFY_SQL(q, next());

    QSqlDriver *driver = db.driver();
    QVERIFY(driver);

    QString preparedStatment = driver->sqlStatement(QSqlDriver::WhereStatement, QString("qtest"), q.record(), true);
    QCOMPARE(preparedStatment.count("IS NULL", Qt::CaseInsensitive), 2);

    QString statment = driver->sqlStatement(QSqlDriver::WhereStatement, QString("qtest"), q.record(), false);
    QCOMPARE(statment.count("IS NULL", Qt::CaseInsensitive), 2);
}

void tst_QSqlDatabase::sqlite_enable_cache_mode()
{
    QFETCH(QString, dbName);
    if(dbName.endsWith(":memory:"))
        QSKIP( "cache mode is meaningless for :memory: databases", SkipSingle );
    QSqlDatabase db = QSqlDatabase::database(dbName);
    CHECK_DATABASE(db);
    db.close();
    db.setConnectOptions("QSQLITE_ENABLE_SHARED_CACHE");
    QVERIFY_SQL(db, open());
    QSqlDatabase db2 = QSqlDatabase::cloneDatabase(db, dbName+":cachemodeconn2");
    db2.setConnectOptions("QSQLITE_ENABLE_SHARED_CACHE");
    QVERIFY_SQL(db2, open());
    QSqlQuery q(db), q2(db2);
    QVERIFY_SQL(q, exec("select * from "+qTableName("qtest", __FILE__)));
    QVERIFY_SQL(q2, exec("select * from "+qTableName("qtest", __FILE__)));
    db2.close();
}

QTEST_MAIN(tst_QSqlDatabase)
#include "moc_tst_qsqldatabase.cpp"
