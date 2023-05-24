/****************************************************************************
**
** Copyright (C) 2023 Ivailo Monev
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

#ifndef QPNGHANDLER_P_H
#define QPNGHANDLER_P_H

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

QT_BEGIN_NAMESPACE

class QPngHandler : public QImageIOHandler
{
public:
    QPngHandler();
    ~QPngHandler();

    bool canRead() const final;
    bool read(QImage *image) final;
    bool write(const QImage &image) final;

    QVariant option(QImageIOHandler::ImageOption option) const final;
    void setOption(QImageIOHandler::ImageOption option, const QVariant &value) final;
    bool supportsOption(QImageIOHandler::ImageOption option) const final;

    QByteArray name() const final;

    static bool canRead(QIODevice *device);

private:
    int m_complevel;
};

QT_END_NAMESPACE

#endif // QPNGHANDLER_P_H
