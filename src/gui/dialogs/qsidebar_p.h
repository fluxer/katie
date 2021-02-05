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

#ifndef QSIDEBAR_H
#define QSIDEBAR_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qlistwidget.h"
#include "qstandarditemmodel.h"
#include "qstyleditemdelegate.h"
#include "qurl.h"

#ifndef QT_NO_FILEDIALOG

QT_BEGIN_NAMESPACE

class QFileSystemModel;

class QSideBarDelegate : public QStyledItemDelegate
{
 public:
     QSideBarDelegate(QWidget *parent = Q_NULLPTR) : QStyledItemDelegate(parent) {}
     void initStyleOption(QStyleOptionViewItem *option,
                          const QModelIndex &index) const;
};

class Q_AUTOTEST_EXPORT QUrlModel : public QStandardItemModel
{
    Q_OBJECT

public:
    enum Roles {
        UrlRole = Qt::UserRole + 1,
        EnabledRole = Qt::UserRole + 2
    };

    QUrlModel(QObject *parent = Q_NULLPTR);

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
#ifndef QT_NO_DRAGANDDROP
    bool canDrop(QDragEnterEvent *event);
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
#endif
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

    void setUrls(const QList<QUrl> &list);
    void addUrls(const QList<QUrl> &urls, int row = -1, bool move = true);
    QList<QUrl> urls() const;
    void setFileSystemModel(QFileSystemModel *model);

private Q_SLOTS:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void layoutChanged();

private:
    void setUrl(const QModelIndex &index, const QUrl &url, const QModelIndex &dirIndex);
    void changed(const QString &path);
    void addIndexToWatch(const QString &path, const QModelIndex &index);
    QFileSystemModel *fileSystemModel;
    QList<QPair<QModelIndex, QString> > watching;
    QList<QUrl> invalidUrls;
};

class Q_AUTOTEST_EXPORT QSidebar : public QListView
{
    Q_OBJECT

Q_SIGNALS:
    void goToUrl(const QUrl &url);

public:
    QSidebar(QWidget *parent = Q_NULLPTR);
    void init(QFileSystemModel *model, const QList<QUrl> &newUrls);
    ~QSidebar();

    QSize sizeHint() const;

    void setUrls(const QList<QUrl> &list) { urlModel->setUrls(list); }
    void addUrls(const QList<QUrl> &list, int row) { urlModel->addUrls(list, row); }
    QList<QUrl> urls() const { return urlModel->urls(); }

    void selectUrl(const QUrl &url);

protected:
    bool event(QEvent * e);
    void focusInEvent(QFocusEvent *event);
#ifndef QT_NO_DRAGANDDROP
    void dragEnterEvent(QDragEnterEvent *event);
#endif

private Q_SLOTS:
    void clicked(const QModelIndex &index);
#ifndef QT_NO_MENU
    void showContextMenu(const QPoint &position);
#endif
    void removeEntry();

private:
    QUrlModel *urlModel;
};

QT_END_NAMESPACE

#endif // QT_NO_FILEDIALOG

#endif // QSIDEBAR_H

