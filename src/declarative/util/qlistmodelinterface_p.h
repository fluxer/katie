/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#ifndef QLISTMODELINTERFACE_H
#define QLISTMODELINTERFACE_H

#include <QtCore/QHash>
#include <QtCore/QVariant>

#include "qdeclarativeglobal_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class Q_DECLARATIVE_PRIVATE_EXPORT QListModelInterface : public QObject
{
    Q_OBJECT
 public:
    QListModelInterface(QObject *parent = Q_NULLPTR) : QObject(parent) {}
    virtual ~QListModelInterface() {}

    virtual int count() const = 0;
    virtual QVariant data(int index, int role) const = 0;

    virtual QList<int> roles() const = 0;
    virtual QString toString(int role) const = 0;

 Q_SIGNALS:
    void itemsInserted(int index, int count);
    void itemsRemoved(int index, int count);
    void itemsMoved(int from, int to, int count);
    void itemsChanged(int index, int count, const QList<int> &roles);

 protected:
    QListModelInterface(QObjectPrivate &dd, QObject *parent) 
        : QObject(dd, parent) {}
};


QT_END_NAMESPACE

QT_END_HEADER
#endif //QTREEMODELINTERFACE_H
