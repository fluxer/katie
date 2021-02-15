/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtNetwork module of the Katie Toolkit.
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

#ifndef QSSLCIPHER_P_H
#define QSSLCIPHER_P_H

#include "qstring.h"

QT_BEGIN_NAMESPACE

//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

class QSslCipherPrivate
{
public:
    QSslCipherPrivate()
        : isNull(true), supportedBits(0), bits(0),
          exportable(false), protocol(QSsl::UnknownProtocol)
    {
    }

    bool isNull;
    QString name;
    int supportedBits;
    int bits;
    QString keyExchangeMethod;
    QString authenticationMethod;
    QString encryptionMethod;
    bool exportable;
    QString protocolString;
    QSsl::SslProtocol protocol;
};

QT_END_NAMESPACE

#endif // QSSLCIPHER_P_H
