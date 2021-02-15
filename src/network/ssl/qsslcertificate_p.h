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


#ifndef QSSLCERTIFICATE_P_H
#define QSSLCERTIFICATE_P_H

#include "qsslcertificate.h"

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

#include "qsslsocket_p.h"
#include <QtCore/qdatetime.h>
#include <QtCore/qmap.h>

#include <openssl/x509.h>

QT_BEGIN_NAMESPACE

class QSslCertificatePrivate
{
public:
    QSslCertificatePrivate()
        : null(true), x509(0)
    {
        QSslSocketPrivate::ensureInitialized();
    }

    ~QSslCertificatePrivate()
    {
        if (x509)
            X509_free(x509);
    }

    bool null;
    QByteArray versionString;
    QByteArray serialNumberString;

    QMap<QString, QString> issuerInfo;
    QMap<QString, QString> subjectInfo;
    QDateTime notValidAfter;
    QDateTime notValidBefore;

    X509 *x509;

    void init(const QByteArray &data, QSsl::EncodingFormat format);

    static QByteArray QByteArray_from_X509(X509 *x509, QSsl::EncodingFormat format);
    static QSslCertificate QSslCertificate_from_X509(X509 *x509);
    static QList<QSslCertificate> certificatesFromPem(const QByteArray &pem, int count = -1);
    static QList<QSslCertificate> certificatesFromDer(const QByteArray &der, int count = -1);
    static bool isBlacklisted(const QSslCertificate &certificate);

    friend class QSslSocketBackendPrivate;

    QAtomicInt ref;
};

QT_END_NAMESPACE

#endif
