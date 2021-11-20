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

#ifndef QDECLARATIVEBORDERIMAGE_H
#define QDECLARATIVEBORDERIMAGE_H

#include "qdeclarativeimagebase_p.h"

#include <QtNetwork/qnetworkreply.h>

QT_BEGIN_NAMESPACE


class QDeclarativeScaleGrid;
class QDeclarativeBorderImagePrivate;
class Q_AUTOTEST_EXPORT QDeclarativeBorderImage : public QDeclarativeImageBase
{
    Q_OBJECT
    Q_ENUMS(TileMode)

    Q_PROPERTY(QDeclarativeScaleGrid *border READ border CONSTANT)
    Q_PROPERTY(TileMode horizontalTileMode READ horizontalTileMode WRITE setHorizontalTileMode NOTIFY horizontalTileModeChanged)
    Q_PROPERTY(TileMode verticalTileMode READ verticalTileMode WRITE setVerticalTileMode NOTIFY verticalTileModeChanged)

public:
    QDeclarativeBorderImage(QDeclarativeItem *parent=0);
    ~QDeclarativeBorderImage();

    QDeclarativeScaleGrid *border();

    enum TileMode { Stretch = Qt::StretchTile, Repeat = Qt::RepeatTile, Round = Qt::RoundTile };

    TileMode horizontalTileMode() const;
    void setHorizontalTileMode(TileMode);

    TileMode verticalTileMode() const;
    void setVerticalTileMode(TileMode);

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
    void setSource(const QUrl &url);

    void setSourceSize(const QSize&);

Q_SIGNALS:
    void horizontalTileModeChanged();
    void verticalTileModeChanged();

protected:
    virtual void load();

private Q_SLOTS:
    void doUpdate();
    void requestFinished();

private:
    Q_DISABLE_COPY(QDeclarativeBorderImage)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeBorderImage)
};

QT_END_NAMESPACE
QML_DECLARE_TYPE(QDeclarativeBorderImage)

#endif // QDECLARATIVEBORDERIMAGE_H
