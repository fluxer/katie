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

#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QGraphicsWidget>
#include "simplelistview.h"
#include "scroller.h"
#include "gvbwidget.h"

class AbstractViewItem;
class QGraphicsSceneResizeEvent;
class QGraphicsGridLayout;
class QGraphicsLinearLayout;

class ListWidget : public GvbWidget
{
    Q_OBJECT

public:
    ListWidget(QGraphicsWidget *parent = nullptr);
    virtual ~ListWidget();
    void addItem(QGraphicsWidget *item);
    void insertItem(int index, QGraphicsWidget *item);
    QGraphicsWidget* takeItem(int row);
    QGraphicsWidget* itemAt(int row);
    int itemCount() const;
#if (QT_VERSION >= 0x040600)
    bool listItemCaching() const;
    void setListItemCaching(bool enable);
#endif
    ScrollBar* verticalScrollBar() const;

    void setTwoColumns(const bool twoColumns);
    bool twoColumns();

protected:
    virtual void resizeEvent( QGraphicsSceneResizeEvent * event );

private:
    Q_DISABLE_COPY(ListWidget)

    QGraphicsLinearLayout *m_layout;
    SimpleListView *m_listView;
    Scroller m_scroller;
};

#endif
