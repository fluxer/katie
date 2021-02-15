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

#include "abstractviewitem.h"

AbstractViewItem::AbstractViewItem(QGraphicsWidget *parent)
    : GvbWidget(parent),
    m_index(),
    m_itemView(0),
    m_prototype(0)
{
}

/*virtual*/
AbstractViewItem::~AbstractViewItem()
{
}

QModelIndex AbstractViewItem::modelIndex() const
{
    return m_index;
}

AbstractViewItem *AbstractViewItem::prototype() const
{
    return m_prototype;
}

AbstractItemView *AbstractViewItem::itemView() const
{
    return m_itemView;
}

void AbstractViewItem::setItemView(AbstractItemView *itemView)
{
    m_itemView = itemView;
}

void AbstractViewItem::setModelIndex(const QModelIndex &index)
{
    if (m_index != index) {
        m_index = index;
        updateItemContents();
    }
}

/*virtual*/
QSizeF AbstractViewItem::effectiveSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return GvbWidget::effectiveSizeHint(which, constraint);
}

/*virtual*/
bool AbstractViewItem::event(QEvent *e)
{
    return QGraphicsWidget::event(e);
}

/*virtual*/
void AbstractViewItem::updateItemContents()
{
    ; // No impl yet
}

/*virtual*/
void AbstractViewItem::themeChange()
{
    ; // No impl yet
}

#if (QT_VERSION >= 0x040600)
/*virtual*/
void AbstractViewItem::setSubtreeCacheEnabled(bool enabled)
{
    Q_UNUSED(enabled)
    ; // No impl
}
#endif

