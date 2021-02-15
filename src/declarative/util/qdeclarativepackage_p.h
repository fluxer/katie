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

#ifndef QDECLARATIVEPACKAGE_H
#define QDECLARATIVEPACKAGE_H

#include "qdeclarative.h"


QT_BEGIN_NAMESPACE


class QDeclarativePackagePrivate;
class QDeclarativePackageAttached;
class Q_AUTOTEST_EXPORT QDeclarativePackage : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativePackage)

    Q_CLASSINFO("DefaultProperty", "data")
    Q_PROPERTY(QDeclarativeListProperty<QObject> data READ data SCRIPTABLE false)

public:
    QDeclarativePackage(QObject *parent=0);
    virtual ~QDeclarativePackage();

    QDeclarativeListProperty<QObject> data();

    QObject *part(const QString & = QString());
    bool hasPart(const QString &);

    static QDeclarativePackageAttached *qmlAttachedProperties(QObject *);
};

class QDeclarativePackageAttached : public QObject
{
Q_OBJECT
Q_PROPERTY(QString name READ name WRITE setName)
public:
    QDeclarativePackageAttached(QObject *parent);
    virtual ~QDeclarativePackageAttached();

    QString name() const;
    void setName(const QString &n);

    static QHash<QObject *, QDeclarativePackageAttached *> attached;
private:
    QString _name;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativePackage)
QML_DECLARE_TYPEINFO(QDeclarativePackage, QML_HAS_ATTACHED_PROPERTIES)


#endif // QDECLARATIVEPACKAGE_H
