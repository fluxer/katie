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

#ifndef QSQLRELATIONALDELEGATE_H
#define QSQLRELATIONALDELEGATE_H

#ifdef QT_GUI_LIB

#include <QtGui/qitemdelegate.h>
#include <QtGui/qlistview.h>
#include <QtGui/qcombobox.h>
#include <QtSql/qsqlrelationaltablemodel.h>


QT_BEGIN_NAMESPACE


class QSqlRelationalDelegate: public QItemDelegate
{
public:

    explicit QSqlRelationalDelegate(QObject *aParent = Q_NULLPTR)
        : QItemDelegate(aParent)
    {}

    ~QSqlRelationalDelegate()
    {}

    QWidget *createEditor(QWidget *aParent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        const QSqlRelationalTableModel *sqlModel = qobject_cast<const QSqlRelationalTableModel *>(index.model());
        QSqlTableModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
        if (!childModel)
            return QItemDelegate::createEditor(aParent, option, index);

        QComboBox *combo = new QComboBox(aParent);
        combo->setModel(childModel);
        combo->setModelColumn(childModel->fieldIndex(sqlModel->relation(index.column()).displayColumn()));
        combo->installEventFilter(const_cast<QSqlRelationalDelegate *>(this));

        return combo;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        const QSqlRelationalTableModel *sqlModel = qobject_cast<const QSqlRelationalTableModel *>(index.model());
        QComboBox *combo = qobject_cast<QComboBox *>(editor);
        if (!sqlModel || !combo) {
            QItemDelegate::setEditorData(editor, index);
            return;
        }
        combo->setCurrentIndex(combo->findText(sqlModel->data(index).toString()));
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        if (!index.isValid())
            return;

        QSqlRelationalTableModel *sqlModel = qobject_cast<QSqlRelationalTableModel *>(model);
        QSqlTableModel *childModel = sqlModel ? sqlModel->relationModel(index.column()) : 0;
        QComboBox *combo = qobject_cast<QComboBox *>(editor);
        if (!sqlModel || !childModel || !combo) {
            QItemDelegate::setModelData(editor, model, index);
            return;
        }

        int currentItem = combo->currentIndex();
        int childColIndex = childModel->fieldIndex(sqlModel->relation(index.column()).displayColumn());
        int childEditIndex = childModel->fieldIndex(sqlModel->relation(index.column()).indexColumn());
        sqlModel->setData(index,
                childModel->data(childModel->index(currentItem, childColIndex), Qt::DisplayRole),
                Qt::DisplayRole);
        sqlModel->setData(index,
                childModel->data(childModel->index(currentItem, childEditIndex), Qt::EditRole),
                Qt::EditRole);
    }

};

QT_END_NAMESPACE


#endif // QT_GUI_LIB

#endif // QSQLRELATIONALDELEGATE_H
