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

#if !defined(QT_NO_SVGRENDERER)

#include "qsvgiohandler.h"
#include "qiodevice.h"
#include "qbytearray.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

class QSvgPlugin : public QImageIOPlugin
{
public:
    QStringList keys() const;
    Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
    QImageIOHandler *create(QIODevice *device, const QByteArray &format = QByteArray()) const;
};

QStringList QSvgPlugin::keys() const
{
    static const QStringList list = QStringList()
        << QLatin1String("svg")
        << QLatin1String("svgz");
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

Q_EXPORT_PLUGIN2(qsvg, QSvgPlugin)

QT_END_NAMESPACE

#endif // !QT_NO_SVGRENDERER
