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

/****************************************************************************
**
** In addition, as a special exception, Nokia gives permission to link
** the code of its release of Qt with the OpenSSL project's "OpenSSL" library
** (or modified versions of the "OpenSSL" library that use the same license
** as the original version), and distribute the linked executables.
**
** You must comply with the GNU General Public License version 2 in all
** respects for all of the code used other than the "OpenSSL" code.  If you
** modify this file, you may extend this exception to your version of the file,
** but you are not obligated to do so.  If you do not wish to do so, delete
** this exception statement from your version of this file.
**
****************************************************************************/

#ifndef QSSLCONFIGURATION_P_H
#define QSSLCONFIGURATION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QSslSocket API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qsslconfiguration.h"
#include "qlist.h"
#include "qsslcertificate.h"
#include "qsslcipher.h"
#include "qsslkey.h"

QT_BEGIN_NAMESPACE

class QSslConfigurationPrivate: public QSharedData
{
public:
    QSslConfigurationPrivate()
        : protocol(QSsl::SecureProtocols),
          peerVerifyMode(QSslSocket::AutoVerifyPeer),
          peerVerifyDepth(0),
          allowRootCertOnDemandLoading(true),
          sslOptions(QSsl::SslOptionDisableEmptyFragments
                     |QSsl::SslOptionDisableLegacyRenegotiation
                     |QSsl::SslOptionDisableCompression)
    { }

    QSslCertificate peerCertificate;
    QList<QSslCertificate> peerCertificateChain;
    QSslCertificate localCertificate;

    QSslKey privateKey;
    QSslCipher sessionCipher;
    QList<QSslCipher> ciphers;
    QList<QSslCertificate> caCertificates;

    QSsl::SslProtocol protocol;
    QSslSocket::PeerVerifyMode peerVerifyMode;
    int peerVerifyDepth;
    bool allowRootCertOnDemandLoading;

    QSsl::SslOptions sslOptions;

    // in qsslsocket.cpp:
    static QSslConfiguration defaultConfiguration();
    static void setDefaultConfiguration(const QSslConfiguration &configuration);
    static void deepCopyDefaultConfiguration(QSslConfigurationPrivate *config);
};

// implemented here for inlining purposes
inline QSslConfiguration::QSslConfiguration(QSslConfigurationPrivate *dd)
    : d(dd)
{
}

QT_END_NAMESPACE

#endif
