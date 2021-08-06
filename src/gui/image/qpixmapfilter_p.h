/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QPIXMAPFILTER_H
#define QPIXMAPFILTER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qnamespace.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qgraphicseffect.h>

#ifndef QT_NO_GRAPHICSEFFECT

QT_BEGIN_NAMESPACE

class QPainter;

class QPixmapFilterPrivate;

class Q_GUI_EXPORT QPixmapFilter : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPixmapFilter)
public:
    virtual ~QPixmapFilter() = 0;

    enum FilterType {
        ColorizeFilter,
        DropShadowFilter,
        BlurFilter,
    };

    FilterType type() const;

    virtual QRectF boundingRectFor(const QRectF &rect) const;

    virtual void draw(QPainter *painter, const QPointF &p, const QPixmap &src, const QRectF &srcRect = QRectF()) const = 0;

protected:
    QPixmapFilter(QPixmapFilterPrivate &d, FilterType type, QObject *parent);
    QPixmapFilter(FilterType type, QObject *parent);
};

class QPixmapBlurFilterPrivate;

class Q_GUI_EXPORT QPixmapBlurFilter : public QPixmapFilter
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPixmapBlurFilter)

public:
    QPixmapBlurFilter(QObject *parent = nullptr);
    ~QPixmapBlurFilter();

    void setRadius(qreal radius);
    void setBlurHints(QGraphicsBlurEffect::BlurHints hints);

    qreal radius() const;
    QGraphicsBlurEffect::BlurHints blurHints() const;

    QRectF boundingRectFor(const QRectF &rect) const;
    void draw(QPainter *painter, const QPointF &dest, const QPixmap &src, const QRectF &srcRect = QRectF()) const;
};

class QPixmapColorizeFilterPrivate;

class Q_GUI_EXPORT QPixmapColorizeFilter : public QPixmapFilter
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPixmapColorizeFilter)

public:
    QPixmapColorizeFilter(QObject *parent = nullptr);

    void setColor(const QColor& color);
    QColor color() const;

    void setStrength(qreal strength);
    qreal strength() const;

    void draw(QPainter *painter, const QPointF &dest, const QPixmap &src, const QRectF &srcRect = QRectF()) const;
};

class QPixmapDropShadowFilterPrivate;

class Q_GUI_EXPORT QPixmapDropShadowFilter : public QPixmapFilter
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPixmapDropShadowFilter)

public:
    QPixmapDropShadowFilter(QObject *parent = nullptr);
    ~QPixmapDropShadowFilter();

    QRectF boundingRectFor(const QRectF &rect) const;
    void draw(QPainter *p, const QPointF &pos, const QPixmap &px, const QRectF &src = QRectF()) const;

    qreal blurRadius() const;
    void setBlurRadius(qreal radius);

    QColor color() const;
    void setColor(const QColor &color);

    QPointF offset() const;
    void setOffset(const QPointF &offset);
    inline void setOffset(qreal dx, qreal dy) { setOffset(QPointF(dx, dy)); }
};

QT_END_NAMESPACE


#endif //QT_NO_GRAPHICSEFFECT
#endif // QPIXMAPFILTER_H
