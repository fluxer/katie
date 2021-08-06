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

#ifndef QFILESYSTEMMODEL_H
#define QFILESYSTEMMODEL_H

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qpair.h>
#include <QtCore/qdir.h>
#include <QtGui/qicon.h>
#include <QtCore/qdiriterator.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_FILESYSTEMMODEL

class QFileSystemModelPrivate;
class QFileIconProvider;

class Q_GUI_EXPORT QFileSystemModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
    Q_PROPERTY(bool nameFilterDisables READ nameFilterDisables WRITE setNameFilterDisables)

Q_SIGNALS:
    void rootPathChanged(const QString &newPath);
    void fileRenamed(const QString &path, const QString &oldName, const QString &newName);
    void directoryLoaded(const QString &path);

public:
    enum Roles {
        FileIconRole = Qt::DecorationRole,
        FilePathRole = Qt::UserRole + 1,
        FileNameRole = Qt::UserRole + 2,
        FilePermissions = Qt::UserRole + 3
    };

    explicit QFileSystemModel(QObject *parent = nullptr);
    ~QFileSystemModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(const QString &path, int column = 0) const;
    QModelIndex parent(const QModelIndex &child) const;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant myComputer(int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);
    Qt::DropActions supportedDropActions() const;

    // QFileSystemModel specific API
    QModelIndex setRootPath(const QString &path);
    QString rootPath() const;
    QDir rootDirectory() const;

    void setIconProvider(QFileIconProvider *provider);
    QFileIconProvider *iconProvider() const;

    void setFilter(QDir::Filters filters);
    QDir::Filters filter() const;

    void setReadOnly(bool enable);
    bool isReadOnly() const;

    void setNameFilterDisables(bool enable);
    bool nameFilterDisables() const;

    void setNameFilters(const QStringList &filters);
    QStringList nameFilters() const;

    QString filePath(const QModelIndex &index) const;
    bool isDir(const QModelIndex &index) const;
    qint64 size(const QModelIndex &index) const;
    QString type(const QModelIndex &index) const;
    QDateTime lastModified(const QModelIndex &index) const;

    QModelIndex mkdir(const QModelIndex &parent, const QString &name);
    bool rmdir(const QModelIndex &index);
    inline QString fileName(const QModelIndex &index) const;
    inline QIcon fileIcon(const QModelIndex &index) const;
    QFile::Permissions permissions(const QModelIndex &index) const;
    inline QFileInfo fileInfo(const QModelIndex &index) const;
    bool remove(const QModelIndex &index);

protected:
    QFileSystemModel(QFileSystemModelPrivate &, QObject *parent = nullptr);
    void timerEvent(QTimerEvent *event);
    bool event(QEvent *event);

private:
    Q_DECLARE_PRIVATE(QFileSystemModel)
    Q_DISABLE_COPY(QFileSystemModel)

    Q_PRIVATE_SLOT(d_func(), void _q_directoryChanged(const QString &directory, const QStringList &list))
    Q_PRIVATE_SLOT(d_func(), void _q_performDelayedSort())
    Q_PRIVATE_SLOT(d_func(), void _q_fileSystemChanged(const QString &path, const QList<QPair<QString, QFileInfo> > &))

    friend class QFileDialogPrivate;
};

inline QString QFileSystemModel::fileName(const QModelIndex &aindex) const
{ return aindex.data(Qt::DisplayRole).toString(); }
inline QIcon QFileSystemModel::fileIcon(const QModelIndex &aindex) const
{ return qvariant_cast<QIcon>(aindex.data(Qt::DecorationRole)); }
inline QFileInfo QFileSystemModel::fileInfo(const QModelIndex &aindex) const
{ return QFileInfo(filePath(aindex)); }

#endif // QT_NO_FILESYSTEMMODEL

QT_END_NAMESPACE


#endif // QFILESYSTEMMODEL_H

