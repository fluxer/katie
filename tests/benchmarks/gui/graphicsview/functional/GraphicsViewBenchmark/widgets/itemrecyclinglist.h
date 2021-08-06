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

#ifndef ITEMRECYCLINGLIST_H
#define ITEMRECYCLINGLIST_H

#include "listitem.h"
#include "abstractitemview.h"
#include "listmodel.h"
#include "itemrecyclinglistview.h"
#include "recycledlistitem.h"

class QGraphicsWidget;

class ItemRecyclingList : public ItemRecyclingListView
{
    Q_OBJECT

public:
    ItemRecyclingList(const int itemBuffer = 4, QGraphicsWidget *parent = nullptr);
    virtual ~ItemRecyclingList();

    virtual void insertItem(int index, RecycledListItem *item);
    virtual void addItem(RecycledListItem *item);    
    virtual void clear();
    virtual AbstractViewItem *takeItem(const int row);
    virtual void setItemPrototype(AbstractViewItem* prototype);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual bool listItemCaching() const;
    virtual void setListItemCaching(bool enabled);

    void setTwoColumns(const bool enabled);
    bool twoColumns();

public slots:
    void themeChange();

private:
    void updateListItemBackgrounds(int index);

private:
    Q_DISABLE_COPY(ItemRecyclingList)

    ListModel *m_listModel;
};

#endif // ITEMRECYCLINGLIST_H
