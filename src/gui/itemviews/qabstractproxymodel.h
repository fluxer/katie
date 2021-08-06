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

#ifndef QABSTRACTPROXYMODEL_H
#define QABSTRACTPROXYMODEL_H

#include <QtCore/qabstractitemmodel.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_PROXYMODEL

class QAbstractProxyModelPrivate;
class QItemSelection;

class Q_GUI_EXPORT QAbstractProxyModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    QAbstractProxyModel(QObject *parent = nullptr);
    ~QAbstractProxyModel();

    virtual void setSourceModel(QAbstractItemModel *sourceModel);
    QAbstractItemModel *sourceModel() const;

    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const = 0;
    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const = 0;

    virtual QItemSelection mapSelectionToSource(const QItemSelection &selection) const;
    virtual QItemSelection mapSelectionFromSource(const QItemSelection &selection) const;

    bool submit();
    void revert();

    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QMap<int, QVariant> itemData(const QModelIndex &index) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool setItemData(const QModelIndex& index, const QMap<int, QVariant> &roles);
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    QModelIndex buddy(const QModelIndex &index) const;
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    QSize span(const QModelIndex &index) const;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;

    QMimeData* mimeData(const QModelIndexList &indexes) const;
    QStringList mimeTypes() const;
    Qt::DropActions supportedDropActions() const;

protected:
    QAbstractProxyModel(QAbstractProxyModelPrivate &, QObject *parent);

private:
    Q_DECLARE_PRIVATE(QAbstractProxyModel)
    Q_DISABLE_COPY(QAbstractProxyModel)
    Q_PRIVATE_SLOT(d_func(), void _q_sourceModelDestroyed())
};

#endif // QT_NO_PROXYMODEL

QT_END_NAMESPACE


#endif // QABSTRACTPROXYMODEL_H
