/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtSql module of the Katie Toolkit.
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

#ifndef QSQL_PSQL_H
#define QSQL_PSQL_H

#include <QtSql/qsqlresult.h>
#include <QtSql/qsqldriver.h>

#include <libpq-fe.h>
#include <pg_config.h>


QT_BEGIN_NAMESPACE

class QPSQLResultPrivate;
class QPSQLDriverPrivate;
class QPSQLDriver;
class QSqlRecordInfo;

class QPSQLResult : public QSqlResult
{
    friend class QPSQLResultPrivate;
public:
    QPSQLResult(const QPSQLDriver* db, const QPSQLDriverPrivate* p);
    ~QPSQLResult();

    QVariant handle() const;
    void virtual_hook(int id, void *data);

protected:
    void cleanup();
    bool fetch(int i);
    bool fetchFirst();
    bool fetchLast();
    QVariant data(int i) const;
    bool isNull(int field) const;
    bool reset (const QString& query);
    int size() const;
    int numRowsAffected() const;
    QSqlRecord record() const;
    QVariant lastInsertId() const;
    bool prepare(const QString& query);
    bool exec();

private:
    QPSQLResultPrivate *d;
};

class QPSQLDriver : public QSqlDriver
{
    Q_OBJECT
public:
    enum Protocol {
        VersionUnknown = -1,
        Version74 = 10,
        Version8 = 11,
        Version81 = 12,
        Version82 = 13,
        Version83 = 14,
        Version84 = 15,
        Version9 = 16,
        Version91 = 17,
        Version92 = 18,
        Version93 = 19,
        Version94 = 20,
        Version95 = 21,
        Version96 = 22,
        Version10 = 23,
        Version11 = 24,
        Version12 = 25,
        Version13 = 26,
        Version131 = 27
    };

    explicit QPSQLDriver(QObject *parent=0);
    ~QPSQLDriver();
    bool hasFeature(DriverFeature f) const;
    bool open(const QString & db,
              const QString & user,
              const QString & password,
              const QString & host,
              int port,
              const QString& connOpts);
    bool isOpen() const;
    void close();
    QSqlResult *createResult() const;
    QStringList tables(QSql::TableType) const;
    QSqlIndex primaryIndex(const QString& tablename) const;
    QSqlRecord record(const QString& tablename) const;

    Protocol protocol() const;
    QVariant handle() const;

    QString escapeIdentifier(const QString &identifier, IdentifierType type) const;
    QString formatValue(const QSqlField &field, bool trimStrings) const;

    bool subscribeToNotification(const QString &name);
    bool unsubscribeFromNotification(const QString &name);
    QStringList subscribedToNotifications() const;

protected:
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

private Q_SLOTS:
    void _q_handleNotification(int);

private:
    QPSQLDriverPrivate *d;
};

QT_END_NAMESPACE


#endif // QSQL_PSQL_H
