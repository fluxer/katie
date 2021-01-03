/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the examples of the Katie Toolkit.
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

#include "itemrecyclinglistview.h"

ItemRecyclingListView::ItemRecyclingListView(QGraphicsWidget * parent)
    : AbstractItemView(parent), m_rootIndex()
{
}

/*virtual*/
ItemRecyclingListView::~ItemRecyclingListView()
{
}
void ItemRecyclingListView::setCurrentRow(const int row)
{
    setCurrentIndex(model()->index(row,0));
}

int ItemRecyclingListView::rows() const
{
    if (m_model)
        return m_model->rowCount();
    return 0;
}

/*virtual*/
void ItemRecyclingListView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    if (parent == m_rootIndex) {
        AbstractItemView::rowsInserted(parent, start, end);
    }
}

/*virtual*/
void ItemRecyclingListView::rowsRemoved(const QModelIndex &parent, int start, int end)
{
    if (parent == m_rootIndex) {
        AbstractItemView::rowsRemoved(parent, start, end);
    }
}
