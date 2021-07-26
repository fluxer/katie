/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtQuick3D module of the Katie Toolkit.
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

#include "qtgahandler.h"
#include "qtgafile.h"

#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtGui/QImage>

QT_BEGIN_NAMESPACE

QTgaHandler::QTgaHandler()
    : QImageIOHandler()
    , tga(0)
{
}

QTgaHandler::~QTgaHandler()
{
    delete tga;
}

bool QTgaHandler::canRead() const
{
    if (!tga)
        tga = new QTgaFile(device());
    if (tga->isValid())
    {
        setFormat("tga");
        return true;
    }
    return false;
}

bool QTgaHandler::canRead(QIODevice *device)
{
    if (Q_UNLIKELY(!device)) {
        qWarning("QTgaHandler::canRead() called with no device");
        return false;
    }

    const qint64 oldpos = device->pos();
    QTgaFile tga(device);
    const bool isvalid = tga.isValid();
    device->seek(oldpos);
    return isvalid;
}

bool QTgaHandler::read(QImage *image)
{
    if (!canRead())
        return false;
    *image = tga->readImage();
    return !image->isNull();
}

QByteArray QTgaHandler::name() const
{
    return "tga";
}

QVariant QTgaHandler::option(ImageOption option) const
{
    if (option == Size && canRead()) {
        return tga->size();
    } else if (option == CompressionRatio) {
        return tga->compression();
    } else if (option == ImageFormat) {
        return QImage::Format_ARGB32;
    }
    return QVariant();
}

void QTgaHandler::setOption(ImageOption option, const QVariant &value)
{
    Q_UNUSED(option);
    Q_UNUSED(value);
}

bool QTgaHandler::supportsOption(ImageOption option) const
{
    return option == CompressionRatio
            || option == Size
            || option == ImageFormat;
}

QT_END_NAMESPACE
