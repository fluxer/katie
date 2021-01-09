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

#ifndef QSQLDATABASE_H
#define QSQLDATABASE_H

#include <QtCore/qstring.h>
#include <QtSql/qsql.h>


QT_BEGIN_NAMESPACE

class QDebug;
class QSqlError;
class QSqlDriver;
class QSqlIndex;
class QSqlRecord;
class QSqlQuery;
class QSqlDatabasePrivate;

class Q_SQL_EXPORT QSqlDatabase
{
public:
    QSqlDatabase();
    QSqlDatabase(const QSqlDatabase &other);
    ~QSqlDatabase();

    QSqlDatabase &operator=(const QSqlDatabase &other);

    bool open();
    bool open(const QString& user, const QString& password);
    void close();
    bool isOpen() const;
    bool isOpenError() const;
    QStringList tables(QSql::TableType type = QSql::Tables) const;
    QSqlIndex primaryIndex(const QString& tablename) const;
    QSqlRecord record(const QString& tablename) const;
    QSqlQuery exec(const QString& query = QString()) const;
    QSqlError lastError() const;
    bool isValid() const;

    bool transaction();
    bool commit();
    bool rollback();

    void setDatabaseName(const QString& name);
    void setUserName(const QString& name);
    void setPassword(const QString& password);
    void setHostName(const QString& host);
    void setPort(int p);
    void setConnectOptions(const QString& options = QString());
    QString databaseName() const;
    QString userName() const;
    QString password() const;
    QString hostName() const;
    QString driverName() const;
    int port() const;
    QString connectOptions() const;
    QString connectionName() const;
    void setNumericalPrecisionPolicy(QSql::NumericalPrecisionPolicy precisionPolicy);
    QSql::NumericalPrecisionPolicy numericalPrecisionPolicy() const;

    QSqlDriver* driver() const;

    static const char *defaultConnection;

    static QSqlDatabase addDatabase(const QString& type,
                                 const QString& connectionName = QLatin1String(defaultConnection));
    static QSqlDatabase addDatabase(QSqlDriver* driver,
                                 const QString& connectionName = QLatin1String(defaultConnection));
    static QSqlDatabase cloneDatabase(const QSqlDatabase &other, const QString& connectionName);
    static QSqlDatabase database(const QString& connectionName = QLatin1String(defaultConnection),
                                 bool open = true);
    static void removeDatabase(const QString& connectionName);
    static bool contains(const QString& connectionName = QLatin1String(defaultConnection));
    static QStringList drivers();
    static QStringList connectionNames();
    static bool isDriverAvailable(const QString &name);

protected:
    explicit QSqlDatabase(const QString& type);
    explicit QSqlDatabase(QSqlDriver* driver);

private:
    friend class QSqlDatabasePrivate;
    QSqlDatabasePrivate *d;
};

#ifndef QT_NO_DEBUG_STREAM
Q_SQL_EXPORT QDebug operator<<(QDebug, const QSqlDatabase &);
#endif

QT_END_NAMESPACE


#endif // QSQLDATABASE_H
