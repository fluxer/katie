/****************************************************************************
**
** Copyright (C) 2021 Ivailo Monev
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

#ifndef QXPMHANDLER_P_H
#define QXPMHANDLER_P_H

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

#ifndef QT_NO_IMAGEFORMAT_XPM

QT_BEGIN_NAMESPACE

class QXpmHandler : public QImageIOHandler
{
public:
    QXpmHandler();

    bool canRead() const final;
    bool read(QImage *image) final;

    bool supportsOption(QImageIOHandler::ImageOption option) const final;
    QVariant option(QImageIOHandler::ImageOption option) const final;

    QByteArray name() const final;

    static bool canRead(QIODevice *device);

private:
    bool readHeader();
    enum State {
        Ready,
        ReadHeader,
        Error
    };
    State state;
    int width;
    int height;
    int ncols;
    int cpp;
    QByteArray buffer;
};

QT_END_NAMESPACE

#endif // QT_NO_IMAGEFORMAT_XPM

#endif // QXPMHANDLER_P_H
