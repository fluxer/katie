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

#include "qglobal.h"

QT_BEGIN_NAMESPACE

/*!
    \class QSqlRelationalDelegate
    \brief The QSqlRelationalDelegate class provides a delegate that is used to
    display and edit data from a QSqlRelationalTableModel.

    Unlike the default delegate, QSqlRelationalDelegate provides a
    combobox for fields that are foreign keys into other tables. To
    use the class, simply call QAbstractItemView::setItemDelegate()
    on the view with an instance of QSqlRelationalDelegate:

    \snippet examples/sql/relationaltablemodel/relationaltablemodel.cpp 4

    The \l{sql/relationaltablemodel}{Relational Table Model} example
    (shown below) illustrates how to use QSqlRelationalDelegate in
    conjunction with QSqlRelationalTableModel to provide tables with
    foreign key support.

    \image relationaltable.png

    \sa QSqlRelationalTableModel, {Model/View Programming}
*/


/*!
    \fn QSqlRelationalDelegate::QSqlRelationalDelegate(QObject *parent)

    Constructs a QSqlRelationalDelegate object with the given \a
    parent.
*/

/*!
    \fn QSqlRelationalDelegate::~QSqlRelationalDelegate()

    Destroys the QSqlRelationalDelegate object and frees any
    allocated resources.
*/

/*!
    \fn QWidget *QSqlRelationalDelegate::createEditor(QWidget *parent,
                                                      const QStyleOptionViewItem &option,
                                                      const QModelIndex &index) const
    \reimp
*/

/*!
    \fn void QSqlRelationalDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
    \reimp
*/

/*!
    \fn void QSqlRelationalDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                                  const QModelIndex &index) const
    \reimp
*/

QT_END_NAMESPACE
