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

#include "qimageiohandler.h"
#include "qstringlist.h"
#include "qsvgiohandler.h"
#include "qiodevice.h"
#include "qbytearray.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

class QSvgPlugin : public QImageIOPlugin
{
public:
    QList<QByteArray> mimeTypes() const final;
    Capabilities capabilities(QIODevice *device, const QByteArray &format) const final;
    QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const final;
};

QList<QByteArray> QSvgPlugin::mimeTypes() const
{
    static const QList<QByteArray> list = QList<QByteArray>()
        << "image/svg+xml"
        << "image/svg+xml-compressed";
    return list;
}

QImageIOPlugin::Capabilities QSvgPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "svg" || format == "svgz")
        return Capabilities(CanRead);
    if (!format.isEmpty())
        return 0;

    Capabilities cap;
    if (device->isReadable() && QSvgIOHandler::canRead(device))
        cap |= CanRead;
    return cap;
}

QImageIOHandler *QSvgPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QSvgIOHandler *hand = new QSvgIOHandler();
    hand->setDevice(device);
    hand->setFormat(format);
    return hand;
}

Q_EXPORT_PLUGIN(QSvgPlugin)

QT_END_NAMESPACE
