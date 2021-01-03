/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#ifndef QSQL_MYSQL_H
#define QSQL_MYSQL_H

#include <QtSql/qsqldriver.h>
#include <QtSql/qsqlresult.h>

#include <mysql.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QMYSQLDriverPrivate;
class QMYSQLResultPrivate;
class QMYSQLDriver;
class QSqlRecordInfo;

class QMYSQLResult : public QSqlResult
{
    friend class QMYSQLDriver;
    friend class QMYSQLResultPrivate;
public:
    explicit QMYSQLResult(const QMYSQLDriver* db);
    ~QMYSQLResult();

    QVariant handle() const;
protected:
    void cleanup();
    bool fetch(int i);
    bool fetchNext();
    bool fetchLast();
    bool fetchFirst();
    QVariant data(int field) const;
    bool isNull(int field) const;
    bool reset (const QString& query);
    int size() const;
    int numRowsAffected() const;
    QVariant lastInsertId() const;
    QSqlRecord record() const;
    void virtual_hook(int id, void *data);
    bool nextResult();

    bool prepare(const QString& stmt);
    bool exec();
private:
    QMYSQLResultPrivate* d;
};

class QMYSQLDriver : public QSqlDriver
{
    Q_OBJECT
    friend class QMYSQLResult;
public:
    explicit QMYSQLDriver(QObject *parent=0);
    ~QMYSQLDriver();
    bool hasFeature(DriverFeature f) const;
    bool open(const QString & db,
               const QString & user,
               const QString & password,
               const QString & host,
               int port,
               const QString& connOpts);
    void close();
    QSqlResult *createResult() const;
    QStringList tables(QSql::TableType) const;
    QSqlIndex primaryIndex(const QString& tablename) const;
    QSqlRecord record(const QString& tablename) const;
    QString formatValue(const QSqlField &field,
                                     bool trimStrings) const;
    QVariant handle() const;
    QString escapeIdentifier(const QString &identifier, IdentifierType type) const;

    bool isIdentifierEscaped(const QString &identifier, IdentifierType type) const;

protected:
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
private:
    QMYSQLDriverPrivate* d;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QSQL_MYSQL_H
