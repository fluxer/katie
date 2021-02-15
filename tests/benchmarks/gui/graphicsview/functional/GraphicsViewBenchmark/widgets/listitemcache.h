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

#ifndef LISTITEMCACHE_H
#define LISTITEMCACHE_H

#include <QGraphicsEffect>
#include <QPixmapCache>

class ListItemCache : public QGraphicsEffect 
{
    Q_OBJECT

public:

    ListItemCache();
    ~ListItemCache();

public: // QGraphicsEffect

    void draw(QPainter *painter);
    void sourceChanged(ChangeFlags flags);

private:

    QPixmapCache::Key m_cacheKey;
    QRectF m_visibleRect;
};

#endif // LISTITEMCACHE_H

