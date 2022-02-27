/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the plugins of the Katie Toolkit.
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

#include "qsvgiohandler.h"
#include "qsvgrenderer.h"
#include "qimage.h"
#include "qpixmap.h"
#include "qpainter.h"
#include "qvariant.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

class QSvgIOHandlerPrivate
{
public:
    QSvgIOHandlerPrivate(QSvgIOHandler *qq)
        : q(qq), loaded(false), readDone(false), backColor(Qt::transparent)
    {}

    bool load(QIODevice *device);

    QSvgIOHandler   *q;
    QSvgRenderer     r;
    QSize            defaultSize;
    QSize            scaledSize;
    bool             loaded;
    bool             readDone;
    QColor           backColor;
};


bool QSvgIOHandlerPrivate::load(QIODevice *device)
{
    if (loaded)
        return true;
    if (q->format().isEmpty())
        q->canRead();

    // # The SVG renderer doesn't handle trailing, unrelated data, so we must
    // assume that all available data in the device is to be read.
    bool res = r.load(device->readAll());
    if (res) {
        defaultSize = QSize(r.viewBox().width(), r.viewBox().height());
        loaded = true;
    }

    return loaded;
}


QSvgIOHandler::QSvgIOHandler()
    : d(new QSvgIOHandlerPrivate(this))
{

}


QSvgIOHandler::~QSvgIOHandler()
{
    delete d;
}


bool QSvgIOHandler::canRead() const
{
    if (!device())
        return false;
    if (d->loaded && !d->readDone)
        return true;        // Will happen if we have been asked for the size

    const QByteArray buf = device()->peek(8);
    if (buf.startsWith("\x1f\x8b")) {
        setFormat("svgz");
        return true;
    } else if (buf.contains("<?xml") || buf.contains("<svg")) {
        setFormat("svg");
        return true;
    }
    return false;
}


QByteArray QSvgIOHandler::name() const
{
    return "svg";
}

bool QSvgIOHandler::read(QImage *image)
{
    if (!d->readDone && d->load(device())) {
        QSize finalSize = d->defaultSize;
        QRectF bounds;
        if (d->scaledSize.isValid() && !d->defaultSize.isEmpty()) {
            bounds = QRectF(QPointF(0,0), QSizeF(d->defaultSize));
            QSizeF sc(1, 1);
            if (d->scaledSize.isValid()) {
                sc = QSizeF(qreal(d->scaledSize.width()) / finalSize.width(),
                            qreal(d->scaledSize.height()) / finalSize.height());
                finalSize = d->scaledSize;
            }
            QTransform t;
            t.scale(sc.width(), sc.height());
            bounds = t.mapRect(bounds);
        }
        *image = QImage(finalSize, QImage::Format_ARGB32_Premultiplied);
        if (!finalSize.isEmpty()) {
            image->fill(d->backColor.rgba());
            QPainter p(image);
            d->r.render(&p, bounds);
            p.end();
        }
        d->readDone = true;
        return true;
    }

    return false;
}

QVariant QSvgIOHandler::option(QImageIOHandler::ImageOption option) const
{
    switch(option) {
        case QImageIOHandler::Size:
            d->load(device());
            return d->defaultSize;
        case QImageIOHandler::ScaledSize:
            return d->scaledSize;
        case QImageIOHandler::BackgroundColor:
            return d->backColor;
        default:
            break;
    }
    return QVariant();
}


void QSvgIOHandler::setOption(QImageIOHandler::ImageOption option, const QVariant & value)
{
    switch(option) {
        case QImageIOHandler::ScaledSize:
            d->scaledSize = value.toSize();
            break;
        case QImageIOHandler::BackgroundColor:
            d->backColor = value.value<QColor>();
            break;
        default:
            break;
    }
}

bool QSvgIOHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    switch(option) {
        case QImageIOHandler::Size:
        case QImageIOHandler::ScaledSize:
        case QImageIOHandler::BackgroundColor:
            return true;
        default:
            break;
    }
    return false;
}


bool QSvgIOHandler::canRead(QIODevice *device)
{
    if (Q_UNLIKELY(!device)) {
        qWarning("QSvgIOHandler::canRead() called with no device");
        return false;
    }

    const QByteArray buf = device->peek(8);
    return buf.startsWith("\x1f\x8b") || buf.contains("<?xml") || buf.contains("<svg");
}

QT_END_NAMESPACE
