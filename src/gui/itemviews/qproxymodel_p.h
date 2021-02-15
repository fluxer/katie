/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QPROXYMODEL_P_H
#define QPROXYMODEL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of QAbstractItemModel*.  This header file may change from version
// to version without notice, or even be removed.
//
// We mean it.
//
//

#include "QtCore/qabstractitemmodel.h"
#include "qabstractitemmodel_p.h"

#ifndef QT_NO_PROXYMODEL

QT_BEGIN_NAMESPACE

class QEmptyModel : public QAbstractItemModel
{
public:
    explicit QEmptyModel(QObject *parent = Q_NULLPTR) : QAbstractItemModel(parent) {}
    QModelIndex index(int, int, const QModelIndex &) const { return QModelIndex(); }
    QModelIndex parent(const QModelIndex &) const { return QModelIndex(); }
    int rowCount(const QModelIndex &) const { return 0; }
    int columnCount(const QModelIndex &) const { return 0; }
    bool hasChildren(const QModelIndex &) const { return false; }
    QVariant data(const QModelIndex &, int) const { return QVariant(); }
};

class QProxyModelPrivate : private QAbstractItemModelPrivate
{
    Q_DECLARE_PUBLIC(QProxyModel)

public:
    void _q_sourceDataChanged(const QModelIndex &tl,const QModelIndex &br);
    void _q_sourceRowsAboutToBeInserted(const QModelIndex &parent, int first ,int last);
    void _q_sourceRowsInserted(const QModelIndex &parent, int first ,int last);
    void _q_sourceRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void _q_sourceRowsRemoved(const QModelIndex &parent, int first, int last);
    void _q_sourceColumnsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void _q_sourceColumnsInserted(const QModelIndex &parent, int first, int last);
    void _q_sourceColumnsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void _q_sourceColumnsRemoved(const QModelIndex &parent, int first, int last);

    QProxyModelPrivate() : QAbstractItemModelPrivate(), model(0) {}
    QAbstractItemModel *model;
    QEmptyModel empty;
};

QT_END_NAMESPACE

#endif // QT_NO_PROXYMODEL

#endif // QPROXYMODEL_P_H
