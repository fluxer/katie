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

#include "qtiffhandler_p.h"

QT_BEGIN_NAMESPACE

class QTiffPlugin : public QImageIOPlugin
{
public:
    QTiffPlugin();
    ~QTiffPlugin();

    QStringList keys() const;
    Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
    QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const;
};

QTiffPlugin::QTiffPlugin()
{
}

QTiffPlugin::~QTiffPlugin()
{
}

QStringList QTiffPlugin::keys() const
{
    static const QStringList list = QStringList()
        << QLatin1String("tif")
        << QLatin1String("tiff");
    return list;
}

QImageIOPlugin::Capabilities QTiffPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "tif" || format == "tiff")
        return Capabilities(CanRead | CanWrite);
    if (!format.isEmpty())
        return 0;
    if (!device->isOpen())
        return 0;

    Capabilities cap;
    if (device->isReadable() && QTiffHandler::canRead(device))
        cap |= CanRead;
    if (device->isWritable())
        cap |= CanWrite;
    return cap;
}

QImageIOHandler *QTiffPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *handler = new QTiffHandler;
    handler->setDevice(device);
    handler->setFormat(format);
    return handler;
}

Q_EXPORT_PLUGIN2(qtiff, QTiffPlugin)

QT_END_NAMESPACE
