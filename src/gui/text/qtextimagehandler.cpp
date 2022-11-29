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


#include "qtextimagehandler_p.h"

#include "qapplication.h"
#include "qtextformat.h"
#include "qpainter.h"
#include "qdebug.h"
#include "qpalette.h"
#include "qstyle.h"
#include "qtextbrowser.h"
#include "qthread.h"
#include "qx11info_x11.h"

QT_BEGIN_NAMESPACE

static QImage getImage(QTextDocument *doc, const QTextImageFormat &format)
{
    QImage image;

    const QString name(format.name());
    const QUrl url = QUrl::fromEncoded(name.toUtf8());
    const QVariant data = doc->resource(QTextDocument::ImageResource, url);
    if (data.type() == QVariant::Image || data.type() == QVariant::Pixmap) {
        image = qvariant_cast<QImage>(data);
    } else if (data.type() == QVariant::ByteArray) {
        image.loadFromData(data.toByteArray());
    }

    if (image.isNull()) {
        // try direct loading
        if (name.isEmpty() || !image.load(name)) {
            return QApplication::style()->standardPixmap(QStyle::SP_FileIcon).toImage();
        }
        doc->addResource(QTextDocument::ImageResource, url, image);
    }

    return image;
}

static QSize getImageSize(QTextDocument *doc, const QTextImageFormat &format)
{
    QImage image;

    const bool hasWidth = format.hasProperty(QTextFormat::ImageWidth);
    const int width = qRound(format.width());
    const bool hasHeight = format.hasProperty(QTextFormat::ImageHeight);
    const int height = qRound(format.height());

    QSize size(width, height);
    if (!hasWidth || !hasHeight) {
        image = getImage(doc, format);
        if (!hasWidth)
            size.setWidth(image.width());
        if (!hasHeight)
            size.setHeight(image.height());
    }

    qreal scale = 1.0;
    QPaintDevice *pdev = doc->documentLayout()->paintDevice();
    if (pdev) {
        if (image.isNull())
            image = getImage(doc, format);
        if (!image.isNull())
            scale = qreal(pdev->logicalDpiY()) / qreal(QX11Info::appDpiY());
    }
    size *= scale;

    return size;
}

QTextImageHandler::QTextImageHandler(QObject *parent)
    : QObject(parent)
{
}

QSizeF QTextImageHandler::intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format)
{
    Q_UNUSED(posInDocument)
    return getImageSize(doc, format.toImageFormat());
}

void QTextImageHandler::drawObject(QPainter *p, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format)
{
    Q_UNUSED(posInDocument)
    const QImage image = getImage(doc, format.toImageFormat());
    p->drawImage(rect, image, image.rect());
}


#include "moc_qtextimagehandler_p.h"

QT_END_NAMESPACE
