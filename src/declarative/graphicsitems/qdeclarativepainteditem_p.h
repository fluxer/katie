/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEIMAGEITEM_H
#define QDECLARATIVEIMAGEITEM_H

#include "qdeclarativeitem.h"


QT_BEGIN_NAMESPACE


class QDeclarativePaintedItemPrivate;
class Q_AUTOTEST_EXPORT QDeclarativePaintedItem : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QSize contentsSize READ contentsSize WRITE setContentsSize NOTIFY contentsSizeChanged)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(int pixelCacheSize READ pixelCacheSize WRITE setPixelCacheSize)
    Q_PROPERTY(qreal contentsScale READ contentsScale WRITE setContentsScale NOTIFY contentsScaleChanged)


public:
    QDeclarativePaintedItem(QDeclarativeItem *parent=0);
    ~QDeclarativePaintedItem();

    QSize contentsSize() const;
    void setContentsSize(const QSize &);

    qreal contentsScale() const;
    void setContentsScale(qreal);

    int pixelCacheSize() const;
    void setPixelCacheSize(int pixels);

    QColor fillColor() const;
    void setFillColor(const QColor&);

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

protected:
    QDeclarativePaintedItem(QDeclarativePaintedItemPrivate &dd, QDeclarativeItem *parent);

    virtual void drawContents(QPainter *p, const QRect &) = 0;
    virtual void geometryChanged(const QRectF &newGeometry,
                                 const QRectF &oldGeometry);
    virtual QVariant itemChange(GraphicsItemChange change,
                                const QVariant &value);

    void setCacheFrozen(bool);
    QRectF boundingRect() const;

Q_SIGNALS:
    void fillColorChanged();
    void contentsSizeChanged();
    void contentsScaleChanged();

protected Q_SLOTS:
    void dirtyCache(const QRect &);
    void clearCache();

private:
    Q_DISABLE_COPY(QDeclarativePaintedItem)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativePaintedItem)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativePaintedItem)


#endif
