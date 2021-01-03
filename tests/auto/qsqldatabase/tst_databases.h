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
/* possible connection parameters */

#ifndef TST_DATABASES_H
#define TST_DATABASES_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QRegExp>
#include <QDir>
#include <QVariant>
#include <QDebug>
#include <QHostInfo>
#include <QtTest>

#include <unistd.h>

#define CHECK_DATABASE( db ) \
    if ( !db.isValid() ) { qFatal( "db is Invalid" ); }

#define QVERIFY_SQL(q, stmt) QVERIFY2((q).stmt, tst_Databases::printError((q).lastError(), db))
#define QFAIL_SQL(q, stmt) QVERIFY2(!(q).stmt, tst_Databases::printError((q).lastError(), db))

#define DBMS_SPECIFIC(db, driver) \
    if (!db.driverName().startsWith(driver)) { QSKIP(driver " specific test", SkipSingle); return; }

static QString qGetHostName()
{
    static QString hostname;

    if ( !hostname.isEmpty() )
        return hostname;

    hostname = QHostInfo::localHostName();
    hostname.replace( QLatin1Char( '.' ), QLatin1Char( '_' ) );
    hostname.replace( QLatin1Char( '-' ), QLatin1Char( '_' ) );

    return hostname;
}

// to prevent nameclashes on our database server, each machine
// will use its own set of table names. Call this function to get
// "tablename_hostname"
inline static QString qTableName( const QString& prefix, const char *sourceFileName )
{
    return QLatin1String("dbtst") + QString::number(qHash(QLatin1String(sourceFileName)
        + QLatin1Char('_') + qGetHostName().replace( QLatin1Char('-'), QLatin1Char('_') )), 16) + QLatin1Char('_') + prefix;
}

inline static QString qTableName( const QString& prefix, QSqlDriver* driver )
{
    return driver->escapeIdentifier( prefix + QLatin1Char('_') + qGetHostName(), QSqlDriver::TableName );
}

inline static bool testWhiteSpaceNames( const QString &name )
{
    return name.startsWith( QLatin1String("QPSQL") )
           || name.startsWith( QLatin1String("QODBC") )
           || name.startsWith( QLatin1String("QSQLITE") )
           || name.startsWith( QLatin1String("QMYSQL") );
}

class tst_Databases
{

public:
    tst_Databases(): counter( 0 ), m_sqLitePrefix(QDir::tempPath())
    {
        if (!m_sqLitePrefix.endsWith(QLatin1Char('/')))
            m_sqLitePrefix += QLatin1Char('/');
        m_sqLitePrefix += QLatin1String("foo");
        m_sqLitePrefix += QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch() % quint64(1000));
    }

    ~tst_Databases()
    {
        close();
        foreach (const QString &sqlfile, m_sqLiteFiles) {
            QFile sqLiteFile(sqlfile);
            if (sqLiteFile.exists() && !sqLiteFile.remove()) {
                qWarning() << "Cannot remove " << QDir::toNativeSeparators(sqLiteFile.fileName())
                           << ':' << sqLiteFile.errorString();
            }
        }
    }

    // returns a testtable consisting of the names of all database connections if
    // driverPrefix is empty, otherwise only those that start with driverPrefix.
    int fillTestTable( const QString& driverPrefix = QString() ) const
    {
        QTest::addColumn<QString>( "dbName" );
        int count = 0;

        foreach (const QString &name, dbNames) {
            QSqlDatabase db = QSqlDatabase::database( name );

            if ( !db.isValid() )
                continue;

            if ( driverPrefix.isEmpty() || db.driverName().startsWith( driverPrefix ) ) {
                QTest::newRow( name.toLatin1() ) << name;
                ++count;
            }
        }

        return count;
    }

    int fillTestTableWithStrategies( const QString& driverPrefix = QString() ) const
    {
        QTest::addColumn<QString>( "dbName" );
        QTest::addColumn<int>("submitpolicy_i");
        int count = 0;

        foreach (const QString &name, dbNames) {
            QSqlDatabase db = QSqlDatabase::database( name );

            if ( !db.isValid() )
                continue;

            if ( driverPrefix.isEmpty() || db.driverName().startsWith( driverPrefix ) ) {
                QTest::newRow( QString("%1 [field]").arg(name).toLatin1() ) << name << (int)QSqlTableModel::OnFieldChange;
                QTest::newRow( QString("%1 [row]").arg(name).toLatin1() ) << name << (int)QSqlTableModel::OnRowChange;
                QTest::newRow( QString("%1 [manual]").arg(name).toLatin1() ) << name << (int)QSqlTableModel::OnManualSubmit;
                ++count;
            }
        }

        return count;
    }

    void addDb( const QString& driver, const QString& dbName,
                const QString& user = QString(), const QString& passwd = QString(),
                const QString& host = QString(), int port = -1, const QString params = QString() )
    {
        QSqlDatabase db;

        if ( !QSqlDatabase::drivers().contains( driver ) ) {
            qWarning() <<  "Driver" << driver << "is not installed";
            return;
        }

        // construct a stupid unique name
        QString cName = QString::number( counter++ ) + QLatin1Char('_') + driver + QLatin1Char('@');

        cName += host.isEmpty() ? dbName : host;

        if ( port > 0 )
            cName += QLatin1Char(':') + QString::number( port );

        db = QSqlDatabase::addDatabase( driver, cName );

        if ( !db.isValid() ) {
            qWarning( "Could not create database object" );
            return;
        }

        db.setDatabaseName( dbName );

        db.setUserName( user );
        db.setPassword( passwd );
        db.setHostName( host );
        db.setPort( port );
        db.setConnectOptions( params );
        dbNames.append( cName );
    }

    void addDbs()
    {
//      This requires a local ODBC data source to be configured( pointing to a MySql database )
//         addDb( "QODBC", "mysqlodbc", "troll", "trond" );
//         addDb( "QODBC", "SqlServer", "troll", "trond" );
//         addDb( "QODBC", "silencetestdb", "troll", "trond", "silence" );
//         addDb( "QODBC", "horseheadtestdb", "troll", "trondk", "horsehead" );

//         addDb( "QMYSQL3", "testdb", "troll", "trond", "horsehead.nokia.troll.no" );
//         addDb( "QMYSQL3", "testdb", "troll", "trond", "horsehead.nokia.troll.no", 3307 );
//         addDb( "QMYSQL3", "testdb", "troll", "trond", "horsehead.nokia.troll.no", 3308, "CLIENT_COMPRESS=1;CLIENT_SSL=1" ); // MySQL 4.1.1
//         addDb( "QMYSQL3", "testdb", "troll", "trond", "horsehead.nokia.troll.no", 3309, "CLIENT_COMPRESS=1;CLIENT_SSL=1" ); // MySQL 5.0.18 Linux
//         addDb( "QMYSQL3", "testdb", "troll", "trond", "silence.nokia.troll.no" ); // MySQL 5.1.36 Windows

//         addDb( "QMYSQL3", "testdb", "testuser", "Ee4Gabf6_", "bq-mysql41.apac.nokia.com" ); // MySQL 4.1.22-2.el4  linux
//         addDb( "QMYSQL3", "testdb", "testuser", "Ee4Gabf6_", "bq-mysql50.apac.nokia.com" ); // MySQL 5.0.45-7.el5 linux
//         addDb( "QMYSQL3", "testdb", "testuser", "Ee4Gabf6_", "bq-mysql51.apac.nokia.com" ); // MySQL 5.1.36-6.7.2.i586 linux

//         addDb( "QPSQL7", "testdb", "troll", "trond", "horsehead.nokia.troll.no" ); // V7.2 NOT SUPPORTED!
//         addDb( "QPSQL7", "testdb", "troll", "trond", "horsehead.nokia.troll.no", 5434 ); // V7.2 NOT SUPPORTED! Multi-byte
//         addDb( "QPSQL7", "testdb", "troll", "trond", "horsehead.nokia.troll.no", 5435 ); // V7.3
//         addDb( "QPSQL7", "testdb", "troll", "trond", "horsehead.nokia.troll.no", 5436 ); // V7.4
//         addDb( "QPSQL7", "testdb", "troll", "trond", "horsehead.nokia.troll.no", 5437 ); // V8.0.3
//         addDb( "QPSQL7", "testdb", "troll", "trond", "silence.nokia.troll.no" );         // V8.2.1, UTF-8

//         addDb( "QPSQL7", "testdb", "testuser", "Ee4Gabf6_", "bq-postgres74.apac.nokia.com" );         // Version 7.4.19-1.el4_6.1
//         addDb( "QPSQL7", "testdb", "testuser", "Ee4Gabf6_", "bq-pgsql81.apac.nokia.com" );         // Version 8.1.11-1.el5_1.1
//         addDb( "QPSQL7", "testdb", "testuser", "Ee4Gabf6_", "bq-pgsql84.apac.nokia.com" );         // Version 8.4.1-2.1.i586
//         addDb( "QPSQL7", "testdb", "testuser", "Ee4Gabf6_", "bq-pgsql90.apac.nokia.com" );         // Version 9.0.0

//      use in-memory database to prevent local files
//         addDb("QSQLITE", ":memory:");
         addDb( QLatin1String("QSQLITE"), QDir::toNativeSeparators(sqLiteFileName()));
//         addDb( "QODBC3", "DRIVER={SQL SERVER};SERVER=iceblink.nokia.troll.no\\ICEBLINK", "troll", "trond", "" );
//         addDb( "QODBC3", "DRIVER={SQL Native Client};SERVER=silence.nokia.troll.no\\SQLEXPRESS", "troll", "trond", "" );

//         addDb( "QODBC", "DRIVER={MySQL ODBC 5.1 Driver};SERVER=bq-mysql50.apac.nokia.com;DATABASE=testdb", "testuser", "Ee4Gabf6_", "" );
//         addDb( "QODBC", "DRIVER={MySQL ODBC 5.1 Driver};SERVER=bq-mysql51.apac.nokia.com;DATABASE=testdb", "testuser", "Ee4Gabf6_", "" );
//         addDb( "QODBC", "DRIVER={FreeTDS};SERVER=horsehead.nokia.troll.no;DATABASE=testdb;PORT=4101;UID=troll;PWD=trondk", "troll", "trondk", "" );
//         addDb( "QODBC", "DRIVER={FreeTDS};SERVER=silence.nokia.troll.no;DATABASE=testdb;PORT=2392;UID=troll;PWD=trond", "troll", "trond", "" );
//         addDb( "QODBC", "DRIVER={FreeTDS};SERVER=bq-winserv2003-x86-01.apac.nokia.com;DATABASE=testdb;PORT=1433;UID=testuser;PWD=Ee4Gabf6_;TDS_Version=8.0", "", "", "" );
//         addDb( "QODBC", "DRIVER={FreeTDS};SERVER=bq-winserv2008-x86-01.apac.nokia.com;DATABASE=testdb;PORT=1433;UID=testuser;PWD=Ee4Gabf6_;TDS_Version=8.0", "", "", "" );
//         addDb( "QODBC3", "DRIVER={SQL SERVER};SERVER=bq-winserv2003-x86-01.apac.nokia.com;DATABASE=testdb;PORT=1433", "testuser", "Ee4Gabf6_", "" );
//         addDb( "QODBC3", "DRIVER={SQL SERVER};SERVER=bq-winserv2008-x86-01.apac.nokia.com;DATABASE=testdb;PORT=1433", "testuser", "Ee4Gabf6_", "" );
//         addDb( "QODBC", "DRIVER={Microsoft Access Driver (*.mdb)};DBQ=c:\\dbs\\access\\testdb.mdb", "", "", "" );
//         addDb( "QODBC", "DRIVER={Postgresql};SERVER=bq-pgsql84.apac.nokia.com;DATABASE=testdb", "testuser", "Ee4Gabf6_", "" );
    }

    void open()
    {
        addDbs();

        foreach (const QString &name, dbNames) {
            QSqlDatabase db = QSqlDatabase::database( name, false );
            qDebug() << "Opening:" << name;

            if ( db.isValid() && !db.isOpen() ) {
                if ( !db.open() ) {
                    qWarning( "tst_Databases: Unable to open %s on %s:\n%s", qPrintable( db.driverName() ), qPrintable( name ), qPrintable( db.lastError().databaseText() ) );
                    // well... opening failed, so we just ignore the server, maybe it is not running
                    dbNames.removeAll( name );
                }
            }
        }
    }

    void close()
    {
        foreach (const QString &name, dbNames) {
            {
                QSqlDatabase db = QSqlDatabase::database( name, false );

                if ( db.isValid() && db.isOpen() )
                    db.close();
            }

            QSqlDatabase::removeDatabase( name );
        }

        dbNames.clear();
    }

    // for debugging only: outputs the connection as string
    static QString dbToString( const QSqlDatabase db )
    {
        QString res = db.driverName() + QLatin1Char('@');

        if ( db.driverName().startsWith( QLatin1String("QODBC") ) ) {
            res += db.databaseName();
        } else {
            res += db.hostName();
        }

        if ( db.port() > 0 ) {
            res += QLatin1String(":") + QString::number( db.port() );
        }

        return res;
    }

    // drop a table only if it exists to prevent warnings
    static void safeDropTables( QSqlDatabase db, const QStringList& tableNames )
    {
        bool wasDropped;
        QSqlQuery q( db );
        QStringList dbtables=db.tables();

        foreach(const QString &tableName, tableNames)
        {
            wasDropped = true;
            QString table=tableName;
            if ( db.driver()->isIdentifierEscaped(table, QSqlDriver::TableName))
                table = db.driver()->stripDelimiters(table, QSqlDriver::TableName);

            if ( dbtables.contains( table, Qt::CaseInsensitive ) ) {
                foreach(const QString &table2, dbtables.filter(table, Qt::CaseInsensitive)) {
                    if(table2.compare(table.section(QLatin1Char('.'), -1, -1), Qt::CaseInsensitive) == 0) {
                        table=db.driver()->escapeIdentifier(table2, QSqlDriver::TableName);
                        if(isPostgreSQL(db))
                            wasDropped = q.exec( QLatin1String("drop table ") + table + QLatin1String(" cascade"));
                        else
                            wasDropped = q.exec( QLatin1String("drop table ") + table);
                        dbtables.removeAll(table2);
                    }
                }
            }
            if ( !wasDropped ) {
                qWarning() << dbToString(db) << "unable to drop table" << tableName << ':' << q.lastError();
//              qWarning() << "last query:" << q.lastQuery();
//              qWarning() << "dbtables:" << dbtables;
//              qWarning() << "db.tables():" << db.tables();
            }
        }
    }

    static void safeDropTable( QSqlDatabase db, const QString& tableName )
    {
        safeDropTables(db, QStringList() << tableName);
    }

    static void safeDropViews( QSqlDatabase db, const QStringList &viewNames )
    {
        if ( isMSAccess( db ) ) // Access is sooo stupid.
            safeDropTables( db, viewNames );

        bool wasDropped;
        QSqlQuery q( db );
        QStringList dbtables=db.tables(QSql::Views);

        foreach(QString viewName, viewNames)
        {
            wasDropped = true;
            QString view=viewName;
            if ( db.driver()->isIdentifierEscaped(view, QSqlDriver::TableName))
                view = db.driver()->stripDelimiters(view, QSqlDriver::TableName);

            if ( dbtables.contains( view, Qt::CaseInsensitive ) ) {
                foreach(const QString &view2, dbtables.filter(view, Qt::CaseInsensitive)) {
                    if(view2.compare(view.section(QLatin1Char('.'), -1, -1), Qt::CaseInsensitive) == 0) {
                        view=db.driver()->escapeIdentifier(view2, QSqlDriver::TableName);
                        wasDropped = q.exec( QLatin1String("drop view ") + view);
                        dbtables.removeAll(view);
                    }
                }
            }

            if ( !wasDropped )
                qWarning() << dbToString(db) << "unable to drop view" << viewName << ':' << q.lastError();
//                  << "\nlast query:" << q.lastQuery()
//                  << "\ndbtables:" << dbtables
//                  << "\ndb.tables(QSql::Views):" << db.tables(QSql::Views);
        }
    }

    static void safeDropView( QSqlDatabase db, const QString& tableName )
    {
        safeDropViews(db, QStringList() << tableName);
    }

    // returns the type name of the blob datatype for the database db.
    // blobSize is only used if the db doesn't have a generic blob type
    static QString blobTypeName( QSqlDatabase db, int blobSize = 10000 )
    {
        if ( db.driverName().startsWith( QLatin1String("QMYSQL") ) )
            return QLatin1String( "longblob" );

        if ( db.driverName().startsWith( QLatin1String("QPSQL") ) )
            return QLatin1String( "bytea" );

        if ( db.driverName().startsWith( QLatin1String("QSQLITE") ) )
            return QLatin1String( "blob" );

        qDebug() <<  "tst_Databases::blobTypeName: Don't know the blob type for" << dbToString( db );

        return QLatin1String( "blob" );
    }

    static QString autoFieldName( QSqlDatabase db )
    {
        if ( db.driverName().startsWith( QLatin1String("QMYSQL") ) )
            return QLatin1String( "AUTO_INCREMENT" );
/*        if ( db.driverName().startsWith( QLatin1String("QPSQL") ) )
            return QLatin1String( "SERIAL" );*/

        return QString();
    }

    static QByteArray printError( const QSqlError& err )
    {
        QString result;
        if(err.number() > 0)
            result += QLatin1Char('(') + QString::number(err.number()) + QLatin1String(") ");
        result += QLatin1Char('\'');
        if(!err.driverText().isEmpty())
            result += err.driverText() + QLatin1String("' || '");
        result += err.databaseText() + QLatin1Char('\'');
        return result.toLocal8Bit();
    }

    static QByteArray printError( const QSqlError& err, const QSqlDatabase& db )
    {
        QString result(dbToString(db) + QLatin1String(": "));
        if(err.number() > 0)
            result += QLatin1Char('(') + QString::number(err.number()) + QLatin1String(") ");
        result += QLatin1Char('\'');
        if(!err.driverText().isEmpty())
            result += err.driverText() + QLatin1String("' || '");
        result += err.databaseText() + QLatin1Char('\'');
        return result.toLocal8Bit();
    }

    static bool isSqlServer( QSqlDatabase db )
    {
        return db.databaseName().contains( QLatin1String("sql server"), Qt::CaseInsensitive )
               || db.databaseName().contains( QLatin1String("sqlserver"), Qt::CaseInsensitive )
               || db.databaseName().contains( QLatin1String("sql native client"), Qt::CaseInsensitive )
               || db.databaseName().contains( QLatin1String("bq-winserv"), Qt::CaseInsensitive )
               || db.hostName().contains( QLatin1String("bq-winserv"), Qt::CaseInsensitive );
    }

    static bool isMSAccess( QSqlDatabase db )
    {
        return db.databaseName().contains( QLatin1String("Access Driver"), Qt::CaseInsensitive );
    }

    static bool isPostgreSQL( QSqlDatabase db )
    {
        return db.driverName().startsWith(QLatin1String("QPSQL")) || (db.driverName().startsWith(QLatin1String("QODBC")) && ( db.databaseName().contains(QLatin1String("PostgreSQL"), Qt::CaseInsensitive) || db.databaseName().contains(QLatin1String("pgsql"), Qt::CaseInsensitive) ) );
    }

    static bool isMySQL( QSqlDatabase db )
    {
        return db.driverName().startsWith(QLatin1String("QMYSQL")) || (db.driverName().startsWith(QLatin1String("QODBC")) && db.databaseName().contains(QLatin1String("MySQL"), Qt::CaseInsensitive) );
    }

    // -1 on fail, else Oracle version
    static int getOraVersion( QSqlDatabase db )
    {
        int ver = -1;
        QSqlQuery q( QLatin1String("SELECT banner FROM v$version"), db );
        q.next();

        QRegExp vers( QLatin1String("([0-9]+)\\.[0-9\\.]+[0-9]") );

        if ( vers.indexIn( q.value( 0 ).toString() ) ) {
            bool ok;
            ver = vers.cap( 1 ).toInt( &ok );

            if ( !ok )
                ver = -1;
        }

        return ver;
    }

    static QString getMySqlVersion( const QSqlDatabase &db )
    {
        QSqlQuery q(db);
        q.exec( QLatin1String("select version()") );
        if(q.next())
            return q.value( 0 ).toString();
        else
            return QString();
    }

    static QString getPSQLVersion( const QSqlDatabase &db )
    {
        QSqlQuery q(db);
        q.exec( QLatin1String("select version()") );
        if(q.next())
            return q.value( 0 ).toString();
        else
            return QString();
    }

    QString sqLiteFileName() // Return a temporary file name for SQLite DB
    {
        const QString newFileName = m_sqLitePrefix + QLatin1Char('_')
            + QString::number(m_sqLiteFiles.size()) + QLatin1String(".db");
        m_sqLiteFiles.append(newFileName);
        return newFileName;
    }

    QStringList     dbNames;
    int      counter;

private:
    QString m_sqLitePrefix;
    QStringList m_sqLiteFiles;
};

#endif

