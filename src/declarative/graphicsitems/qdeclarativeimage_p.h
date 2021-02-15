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

#ifndef QDECLARATIVEIMAGE_H
#define QDECLARATIVEIMAGE_H

#include "qdeclarativeimagebase_p.h"

#include <QtNetwork/qnetworkreply.h>

QT_BEGIN_NAMESPACE


class QDeclarativeImagePrivate;
class Q_AUTOTEST_EXPORT QDeclarativeImage : public QDeclarativeImageBase
{
    Q_OBJECT
    Q_ENUMS(FillMode)

    Q_PROPERTY(FillMode fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged)
    Q_PROPERTY(qreal paintedWidth READ paintedWidth NOTIFY paintedGeometryChanged)
    Q_PROPERTY(qreal paintedHeight READ paintedHeight NOTIFY paintedGeometryChanged)

public:
    QDeclarativeImage(QDeclarativeItem *parent=0);
    ~QDeclarativeImage();

    enum FillMode { Stretch, PreserveAspectFit, PreserveAspectCrop, Tile, TileVertically, TileHorizontally };
    FillMode fillMode() const;
    void setFillMode(FillMode);

    QPixmap pixmap() const;
    void setPixmap(const QPixmap &);

    qreal paintedWidth() const;
    qreal paintedHeight() const;

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
    QRectF boundingRect() const;

Q_SIGNALS:
    void fillModeChanged();
    void paintedGeometryChanged();

protected:
    QDeclarativeImage(QDeclarativeImagePrivate &dd, QDeclarativeItem *parent);
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);
    void pixmapChange();
    void updatePaintedGeometry();

private:
    Q_DISABLE_COPY(QDeclarativeImage)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeImage)
};

QT_END_NAMESPACE
QML_DECLARE_TYPE(QDeclarativeImage)

#endif // QDECLARATIVEIMAGE_H
