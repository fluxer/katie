/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEIMAGEPROVIDER_H
#define QDECLARATIVEIMAGEPROVIDER_H

#include <QtGui/qimage.h>
#include <QtGui/qpixmap.h>


QT_BEGIN_NAMESPACE


class QDeclarativeImageProviderPrivate;

class Q_DECLARATIVE_EXPORT QDeclarativeImageProvider
{
public:
    enum ImageType {
        Image,
        Pixmap
    };

    QDeclarativeImageProvider(ImageType type);
    virtual ~QDeclarativeImageProvider();

    ImageType imageType() const;

    virtual QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize);
    virtual QPixmap requestPixmap(const QString &id, QSize *size, const QSize& requestedSize);

private:
    QDeclarativeImageProviderPrivate *d;
};

QT_END_NAMESPACE


#endif // QDECLARATIVEIMAGEPROVIDER
