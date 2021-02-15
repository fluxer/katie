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

#ifndef QSQLRELATIONALTABLEMODEL_H
#define QSQLRELATIONALTABLEMODEL_H

#include <QtSql/qsqltablemodel.h>


QT_BEGIN_NAMESPACE


class Q_SQL_EXPORT QSqlRelation
{
public:
    QSqlRelation() {}
    QSqlRelation(const QString &aTableName, const QString &indexCol,
               const QString &displayCol)
        : tName(aTableName), iColumn(indexCol), dColumn(displayCol) {}
    inline QString tableName() const
    { return tName; }
    inline QString indexColumn() const
    { return iColumn; }
    inline QString displayColumn() const
    { return dColumn; }
    inline bool isValid() const
    { return !(tName.isEmpty() || iColumn.isEmpty() || dColumn.isEmpty()); }
private:
    QString tName, iColumn, dColumn;
};

class QSqlRelationalTableModelPrivate;

class Q_SQL_EXPORT QSqlRelationalTableModel: public QSqlTableModel
{
    Q_OBJECT

public:
    enum JoinMode {
        InnerJoin,
        LeftJoin
    };

    explicit QSqlRelationalTableModel(QObject *parent = Q_NULLPTR,
                                      QSqlDatabase db = QSqlDatabase());
    virtual ~QSqlRelationalTableModel();

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &item, const QVariant &value, int role = Qt::EditRole);
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex());

    void clear();
    bool select();

    void setTable(const QString &tableName);
    virtual void setRelation(int column, const QSqlRelation &relation);
    QSqlRelation relation(int column) const;
    virtual QSqlTableModel *relationModel(int column) const;
    void setJoinMode( QSqlRelationalTableModel::JoinMode joinMode );

public Q_SLOTS:
    void revertRow(int row);

protected:
    QString selectStatement() const;
    bool updateRowInTable(int row, const QSqlRecord &values);
    bool insertRowIntoTable(const QSqlRecord &values);
    QString orderByClause() const;

private:
    Q_DECLARE_PRIVATE(QSqlRelationalTableModel)
};

QT_END_NAMESPACE


#endif // QSQLRELATIONALTABLEMODEL_H
