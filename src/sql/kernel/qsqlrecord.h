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

#ifndef QSQLRECORD_H
#define QSQLRECORD_H

#include <QtCore/qstring.h>


QT_BEGIN_NAMESPACE

class QDebug;
class QSqlField;
class QVariant;
class QSqlRecordPrivate;

class Q_SQL_EXPORT QSqlRecord
{
public:
    QSqlRecord();
    QSqlRecord(const QSqlRecord& other);
    QSqlRecord& operator=(const QSqlRecord& other);
    ~QSqlRecord();

    bool operator==(const QSqlRecord &other) const;
    inline bool operator!=(const QSqlRecord &other) const { return !operator==(other); }

    QVariant value(int i) const;
    QVariant value(const QString& name) const;
    void setValue(int i, const QVariant& val);
    void setValue(const QString& name, const QVariant& val);

    void setNull(int i);
    void setNull(const QString& name);
    bool isNull(int i) const;
    bool isNull(const QString& name) const;

    int indexOf(const QString &name) const;
    QString fieldName(int i) const;

    QSqlField field(int i) const;
    QSqlField field(const QString &name) const;

    bool isGenerated(int i) const;
    bool isGenerated(const QString& name) const;
    void setGenerated(const QString& name, bool generated);
    void setGenerated(int i, bool generated);


    void append(const QSqlField& field);
    void replace(int pos, const QSqlField& field);
    void insert(int pos, const QSqlField& field);
    void remove(int pos);

    bool isEmpty() const;
    bool contains(const QString& name) const;
    void clear();
    void clearValues();
    int count() const;

private:
    void detach();
    QSqlRecordPrivate* d;
};

#ifndef QT_NO_DEBUG_STREAM
Q_SQL_EXPORT QDebug operator<<(QDebug, const QSqlRecord &);
#endif

QT_END_NAMESPACE


#endif // QSQLRECORD_H
