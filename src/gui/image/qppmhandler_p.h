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

#ifndef QPPMHANDLER_P_H
#define QPPMHANDLER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "QtGui/qimageiohandler.h"

#ifndef QT_NO_IMAGEFORMAT_PPM

QT_BEGIN_NAMESPACE

class QByteArray;
class QPpmHandler : public QImageIOHandler
{
public:
    QPpmHandler();

    bool canRead() const final;
    bool read(QImage *image) final;
    bool write(const QImage &image) final;

    QVariant option(QImageIOHandler::ImageOption option) const final;
    bool supportsOption(QImageIOHandler::ImageOption option) const final;

    QByteArray name() const final;

    static bool canRead(QIODevice *device, QByteArray *subType = 0);

private:
    bool readHeader();
    enum State {
        Ready,
        ReadHeader,
        Error
    };
    State state;
    char type;
    int width;
    int height;
    int mcc;
    mutable QByteArray subType;
};

QT_END_NAMESPACE

#endif // QT_NO_IMAGEFORMAT_PPM

#endif // QPPMHANDLER_P_H
