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

#ifndef QSVGIOHANDLER_H
#define QSVGIOHANDLER_H

#include <QImageIOHandler>
#include <QImage>
#include <QIODevice>
#include <QVariant>
#include <QSvgRenderer>
#include <QColor>
#include <QSize>

QT_BEGIN_NAMESPACE

class QSvgIOHandler : public QImageIOHandler
{
public:
    QSvgIOHandler();

    bool canRead() const final;
    bool read(QImage *image) final;

    QVariant option(QImageIOHandler::ImageOption option) const final;
    void setOption(QImageIOHandler::ImageOption option, const QVariant & value) final;
    bool supportsOption(QImageIOHandler::ImageOption option) const final;

    QByteArray name() const final;

    static bool canRead(QIODevice *device);

private:
    bool loadDevice() const;

    mutable QSvgRenderer m_renderer;
    mutable QSize        m_defaultSize;
    QSize                m_scaledSize;
    QColor               m_backColor;
};

QT_END_NAMESPACE

#endif // QSVGIOHANDLER_H
