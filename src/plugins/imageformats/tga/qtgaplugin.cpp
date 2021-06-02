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

#include <QtGui/QImageIOHandler>
#include <QtCore/QDebug>
#include "qtgahandler.h"

QT_BEGIN_NAMESPACE

class QTgaPlugin : public QImageIOPlugin
{
public:
    Capabilities capabilities(QIODevice * device, const QByteArray & format) const;
    QImageIOHandler * create(QIODevice * device, const QByteArray & format = QByteArray()) const;
    QStringList keys() const;
};

QImageIOPlugin::Capabilities QTgaPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "tga")
        return Capabilities(CanRead);
    if (!format.isEmpty())
        return 0;
    if (!device->isOpen())
        return 0;

    Capabilities cap;
    if (device->isReadable() && QTgaHandler::canRead(device))
        cap |= CanRead;
    return cap;
}

QImageIOHandler* QTgaPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *tgaHandler = new QTgaHandler();
    tgaHandler->setDevice(device);
    tgaHandler->setFormat(format);
    return tgaHandler;
}

QStringList QTgaPlugin::keys() const
{
    static const QStringList list = QStringList()
        << QLatin1String("tga");
    return list;
}

Q_EXPORT_PLUGIN2(qtga, QTgaPlugin)

QT_END_NAMESPACE
