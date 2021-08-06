/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef ITEMRECYCLINGLISTVIEW_H
#define ITEMRECYCLINGLISTVIEW_H

#include "abstractitemview.h"

class ItemRecyclingListView : public AbstractItemView
{
public:
    ItemRecyclingListView(QGraphicsWidget *parent = nullptr);
    virtual ~ItemRecyclingListView();
    void setCurrentRow(const int row);
    int rows() const;
#if (QT_VERSION >= 0x040600)
    virtual bool listItemCaching() const = 0;
    virtual void setListItemCaching(bool enabled) = 0;
#endif

protected:
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsRemoved(const QModelIndex &parent,int start,int end);

private:
    QModelIndex m_rootIndex;
};

#endif // ITEMRECYCLINGLISTVIEW_H
