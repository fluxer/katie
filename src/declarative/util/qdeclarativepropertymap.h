/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEPROPERTYMAP_H
#define QDECLARATIVEPROPERTYMAP_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QStringList>
#include <QtCore/QVariant>


QT_BEGIN_NAMESPACE


class QDeclarativePropertyMapPrivate;
class Q_DECLARATIVE_EXPORT QDeclarativePropertyMap : public QObject
{
    Q_OBJECT
public:
    QDeclarativePropertyMap(QObject *parent = nullptr);
    virtual ~QDeclarativePropertyMap();

    QVariant value(const QString &key) const;
    void insert(const QString &key, const QVariant &value);
    void clear(const QString &key);

    Q_INVOKABLE QStringList keys() const;

    int count() const;
    int size() const;
    bool isEmpty() const;
    bool contains(const QString &key) const;

    QVariant &operator[](const QString &key);
    QVariant operator[](const QString &key) const;

Q_SIGNALS:
    void valueChanged(const QString &key, const QVariant &value);

private:
    Q_DECLARE_PRIVATE(QDeclarativePropertyMap)
    Q_DISABLE_COPY(QDeclarativePropertyMap)
};

QT_END_NAMESPACE


#endif
