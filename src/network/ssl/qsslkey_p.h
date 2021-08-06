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


#ifndef QSSLKEY_P_H
#define QSSLKEY_P_H

#include "qsslkey.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of qsslcertificate.cpp.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include <openssl/rsa.h>
#include <openssl/dsa.h>
#include <openssl/dh.h>

QT_BEGIN_NAMESPACE

class QSslKeyPrivate
{
public:
    inline QSslKeyPrivate()
        : isNull(true)
        , rsa(nullptr)
        , dsa(nullptr)
        , dh(nullptr)
    {
    }

    inline ~QSslKeyPrivate()
    { clear(); }

    void clear();

    void decodePem(const QByteArray &pem, const QByteArray &passPhrase);
    QByteArray pemHeader() const;
    QByteArray pemFooter() const;
    QByteArray pemFromDer(const QByteArray &der) const;
    QByteArray derFromPem(const QByteArray &pem) const;

    bool isNull;
    QSsl::KeyType type;
    QSsl::KeyAlgorithm algorithm;
    RSA *rsa;
    DSA *dsa;
    DH *dh;

    QAtomicInt ref;

private:
    Q_DISABLE_COPY(QSslKeyPrivate)
};

QT_END_NAMESPACE

#endif // QSSLKEY_P_H
