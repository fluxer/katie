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

#ifndef __BACKGROUNDITEM_H__
#define __BACKGROUNDITEM_H__

#include <QPixmap>

#include "gvbwidget.h"

class QGraphicsWidget;

class BackgroundItem : public GvbWidget
{
    Q_OBJECT
    
public:
    BackgroundItem(const QString &filename, QGraphicsWidget *parent=0);
    ~BackgroundItem();
    
    void paint(QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget *widget = 0);
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    
public slots:
    void themeChange();
    
private:
    QPixmap m_background;
    QString m_fileName;
};

#endif /* __BACKGROUNDITEM_H__ */
