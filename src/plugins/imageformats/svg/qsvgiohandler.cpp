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
#include "qpainter.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

QSvgIOHandler::QSvgIOHandler()
    : m_backColor(Qt::transparent)
{
}

bool QSvgIOHandler::canRead() const
{
    QIODevice* iodevice = device();
    if (Q_UNLIKELY(!iodevice)) {
        qWarning("QSvgIOHandler::canRead() called with no device");
        return false;
    }
    const QByteArray head = iodevice->peek(8);
    if (head.startsWith("\x1f\x8b")) {
        setFormat("svgz");
        return true;
    } else if (head.contains("<?xml") || head.contains("<svg")) {
        setFormat("svg");
        return true;
    }
    return false;
}

bool QSvgIOHandler::read(QImage *image)
{
    if (!loadDevice()) {
        return false;
    }
    QSize finalSize = m_defaultSize;
    QRectF bounds;
    if (m_scaledSize.isValid() && !m_defaultSize.isEmpty()) {
        bounds = QRectF(QPointF(0,0), QSizeF(m_defaultSize));
        QSizeF sc(1, 1);
        if (m_scaledSize.isValid()) {
            sc = QSizeF(qreal(m_scaledSize.width()) / finalSize.width(),
                        qreal(m_scaledSize.height()) / finalSize.height());
            finalSize = m_scaledSize;
        }
        QTransform t;
        t.scale(sc.width(), sc.height());
        bounds = t.mapRect(bounds);
    }
    *image = QImage(finalSize, QImage::Format_ARGB32_Premultiplied);
    if (!finalSize.isEmpty()) {
        image->fill(m_backColor.rgba());
        QPainter p(image);
        m_renderer.render(&p, bounds);
        p.end();
    }
    return true;
}

QVariant QSvgIOHandler::option(QImageIOHandler::ImageOption option) const
{
    switch (option) {
        case QImageIOHandler::Size: {
            if (!loadDevice()) {
                return QVariant();
            }
            return m_defaultSize;
        }
        case QImageIOHandler::ScaledSize: {
            return m_scaledSize;
        }
        case QImageIOHandler::BackgroundColor: {
            return m_backColor;
        }
        default: {
            return QVariant();
        }
    }
    Q_UNREACHABLE();
    return QVariant();
}

void QSvgIOHandler::setOption(QImageIOHandler::ImageOption option, const QVariant & value)
{
    switch (option) {
        case QImageIOHandler::ScaledSize: {
            m_scaledSize = value.toSize();
            break;
        }
        case QImageIOHandler::BackgroundColor: {
            m_backColor = value.value<QColor>();
            break;
        }
        default: {
            break;
        }
    }
}

bool QSvgIOHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    switch(option) {
        case QImageIOHandler::Size:
        case QImageIOHandler::ScaledSize:
        case QImageIOHandler::BackgroundColor: {
            return true;
        }
        default: {
            return false;
        }
    }
    Q_UNREACHABLE();
    return false;
}

QByteArray QSvgIOHandler::name() const
{
    return "svg";
}

bool QSvgIOHandler::canRead(QIODevice *device)
{
    if (Q_UNLIKELY(!device)) {
        qWarning("QSvgIOHandler::canRead() called with no device");
        return false;
    }
    const QByteArray head = device->peek(8);
    return head.startsWith("\x1f\x8b") || head.contains("<?xml") || head.contains("<svg");
}

bool QSvgIOHandler::loadDevice() const
{
    QIODevice* iodevice = device();
    if (Q_UNLIKELY(!iodevice)) {
        return false;
    }
    if (m_renderer.isValid()) {
        return true;
    }
    // # The SVG renderer doesn't handle trailing, unrelated data, so we must
    // assume that all available data in the device is to be read.
    const bool loaded = m_renderer.load(iodevice->readAll());
    if (!loaded) {
        return false;
    }
    m_defaultSize = m_renderer.viewBox().size();
    return true;
}

QT_END_NAMESPACE
