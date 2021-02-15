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

#ifndef QGIFHANDLER_P_H
#define QGIFHANDLER_P_H

#include <QtGui/qimageiohandler.h>
#include <QtGui/qimage.h>
#include <QtCore/qbytearray.h>

QT_BEGIN_NAMESPACE

class QGIFFormat;
class QGifHandler : public QImageIOHandler
{
public:
    QGifHandler();
    ~QGifHandler();

    bool canRead() const;
    bool read(QImage *image);
    bool write(const QImage &image);

    QByteArray name() const;

    static bool canRead(QIODevice *device);

    QVariant option(ImageOption option) const;
    void setOption(ImageOption option, const QVariant &value);
    bool supportsOption(ImageOption option) const;

    int imageCount() const;
    int loopCount() const;
    int nextImageDelay() const;
    int currentImageNumber() const;

private:
    bool imageIsComing() const;
    QGIFFormat *gifFormat;
    QString fileName;
    mutable QByteArray buffer;
    mutable QImage lastImage;

    mutable int nextDelay;
    mutable int loopCnt;
    int frameNumber;
    mutable QVector<QSize> imageSizes;
    mutable bool scanIsCached;
};

QT_END_NAMESPACE

#endif // QGIFHANDLER_P_H
