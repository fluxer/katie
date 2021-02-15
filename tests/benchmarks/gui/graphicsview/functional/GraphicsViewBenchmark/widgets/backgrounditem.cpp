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

#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneResizeEvent>
#include <QPainter>
#include <QRectF>

#include "backgrounditem.h"
#include "theme.h"

BackgroundItem::BackgroundItem(const QString &filename, QGraphicsWidget *parent)
    : GvbWidget(parent),
      m_background(),
      m_fileName(filename)
{
    setContentsMargins(0,0,0,0);

    connect(Theme::p(), SIGNAL(themeChanged()), this, SLOT(themeChange()));
}

BackgroundItem::~BackgroundItem()
{
    
}

void BackgroundItem::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    GvbWidget::resizeEvent(event);
    m_background = Theme::p()->pixmap(m_fileName, size().toSize());
}

void BackgroundItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    Q_UNUSED(widget)
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    painter->drawPixmap(option->exposedRect, m_background, option->exposedRect);
}

void BackgroundItem::themeChange()
{
    m_background = Theme::p()->pixmap(m_fileName, size().toSize());
    update();
}

