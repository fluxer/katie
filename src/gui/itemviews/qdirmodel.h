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

#ifndef QDIRMODEL_H
#define QDIRMODEL_H

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qdir.h>
#include <QtGui/qfileiconprovider.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_DIRMODEL

class QDirModelPrivate;

class Q_GUI_EXPORT QDirModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(bool resolveSymlinks READ resolveSymlinks WRITE setResolveSymlinks)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool lazyChildCount READ lazyChildCount WRITE setLazyChildCount)

public:
    enum Roles {
        FileIconRole = Qt::DecorationRole,
        FilePathRole = Qt::UserRole + 1,
        FileNameRole
    };

    QDirModel(const QStringList &nameFilters, QDir::Filters filters,
              QDir::SortFlags sort, QObject *parent = nullptr);
    explicit QDirModel(QObject *parent = nullptr);
    ~QDirModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool hasChildren(const QModelIndex &index = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);
    Qt::DropActions supportedDropActions() const;

    // QDirModel specific API

    void setIconProvider(QFileIconProvider *provider);
    QFileIconProvider *iconProvider() const;

    void setNameFilters(const QStringList &filters);
    QStringList nameFilters() const;

    void setFilter(QDir::Filters filters);
    QDir::Filters filter() const;

    void setSorting(QDir::SortFlags sort);
    QDir::SortFlags sorting() const;

    void setResolveSymlinks(bool enable);
    bool resolveSymlinks() const;

    void setReadOnly(bool enable);
    bool isReadOnly() const;

    void setLazyChildCount(bool enable);
    bool lazyChildCount() const;

    QModelIndex index(const QString &path, int column = 0) const;

    bool isDir(const QModelIndex &index) const;
    QModelIndex mkdir(const QModelIndex &parent, const QString &name);
    bool rmdir(const QModelIndex &index);
    bool remove(const QModelIndex &index);

    QString filePath(const QModelIndex &index) const;
    QString fileName(const QModelIndex &index) const;
    QIcon fileIcon(const QModelIndex &index) const;
    QFileInfo fileInfo(const QModelIndex &index) const;

public Q_SLOTS:
    void refresh(const QModelIndex &parent = QModelIndex());

protected:
    QDirModel(QDirModelPrivate &, QObject *parent = nullptr);
    friend class QFileDialogPrivate;

private:
    Q_DECLARE_PRIVATE(QDirModel)
    Q_DISABLE_COPY(QDirModel)
    Q_PRIVATE_SLOT(d_func(), void _q_refresh())
};

#endif // QT_NO_DIRMODEL

QT_END_NAMESPACE


#endif // QDIRMODEL_H
