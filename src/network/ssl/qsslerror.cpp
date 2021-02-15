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

#include "qsslerror.h"
#include "qsslsocket.h"

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif


/*!
    \class QSslError
    \brief The QSslError class provides an SSL error.
    \since 4.3

    \reentrant
    \ingroup network
    \ingroup ssl
    \inmodule QtNetwork

    QSslError provides a simple API for managing errors during QSslSocket's
    SSL handshake.

    \sa QSslSocket, QSslCertificate, QSslCipher
*/

/*!
    \enum QSslError::SslError

    Describes all recognized errors that can occur during an SSL handshake.
    
    \value NoError
    \value UnableToGetIssuerCertificate
    \value UnableToDecryptCertificateSignature
    \value UnableToDecodeIssuerPublicKey
    \value CertificateSignatureFailed
    \value CertificateNotYetValid
    \value CertificateExpired
    \value InvalidNotBeforeField
    \value InvalidNotAfterField
    \value SelfSignedCertificate
    \value SelfSignedCertificateInChain
    \value UnableToGetLocalIssuerCertificate
    \value UnableToVerifyFirstCertificate
    \value CertificateRevoked
    \value InvalidCaCertificate
    \value PathLengthExceeded
    \value InvalidPurpose
    \value CertificateUntrusted
    \value CertificateRejected
    \value SubjectIssuerMismatch
    \value AuthorityIssuerSerialNumberMismatch
    \value NoPeerCertificate
    \value HostNameMismatch
    \value UnspecifiedError
    \value NoSslSupport
    \value CertificateBlacklisted

    \sa QSslError::errorString()
*/

QT_BEGIN_NAMESPACE

class QSslErrorPrivate
{
public:
    QSslError::SslError error;
    QSslCertificate certificate;
};

/*!
    Constructs a QSslError object with no error and default certificate. 

*/

// RVCT compiler in debug build does not like about default values in const-
// So as an workaround we define all constructor overloads here explicitly
QSslError::QSslError()
    : d(new QSslErrorPrivate)
{
    d->error = QSslError::NoError;
    d->certificate = QSslCertificate();
}

/*!
    Constructs a QSslError object. The argument specifies the \a
    error that occurred.

*/
QSslError::QSslError(SslError error)
    : d(new QSslErrorPrivate)
{
    d->error = error;
    d->certificate = QSslCertificate();
}

/*!
    Constructs a QSslError object. The two arguments specify the \a
    error that occurred, and which \a certificate the error relates to.

    \sa QSslCertificate
*/
QSslError::QSslError(SslError error, const QSslCertificate &certificate)
    : d(new QSslErrorPrivate)
{
    d->error = error;
    d->certificate = certificate;
}

/*!
    Constructs an identical copy of \a other.
*/
QSslError::QSslError(const QSslError &other)
    : d(new QSslErrorPrivate)
{
    d->error = other.d->error;
    d->certificate = other.d->certificate;
}

/*!
    Destroys the QSslError object.
*/
QSslError::~QSslError()
{
    delete d;
}

/*!
    \since 4.4

    Assigns the contents of \a other to this error.
*/
QSslError &QSslError::operator=(const QSslError &other)
{
    d->error = other.d->error;
    d->certificate = other.d->certificate;
    return *this;
}

/*!
    \since 4.4

    Returns true if this error is equal to \a other; otherwise returns false.
*/
bool QSslError::operator==(const QSslError &other) const
{
    return d->error == other.d->error
        && d->certificate == other.d->certificate;
}

/*!
    \fn bool QSslError::operator!=(const QSslError &other) const
    \since 4.4

    Returns true if this error is not equal to \a other; otherwise returns
    false.
*/

/*!
    Returns the type of the error.

    \sa errorString(), certificate()
*/
QSslError::SslError QSslError::error() const
{
    return d->error;
}

/*!
    Returns a short localized human-readable description of the error.

    \sa error(), certificate()
*/
QString QSslError::errorString() const
{
    switch (d->error) {
        case QSslError::NoError:
            return QSslSocket::tr("No error");
        case QSslError::UnableToGetIssuerCertificate:
            return QSslSocket::tr("The issuer certificate could not be found");
        case QSslError::UnableToDecryptCertificateSignature:
            return QSslSocket::tr("The certificate signature could not be decrypted");
        case QSslError::UnableToDecodeIssuerPublicKey:
            return QSslSocket::tr("The public key in the certificate could not be read");
        case QSslError::CertificateSignatureFailed:
            return QSslSocket::tr("The signature of the certificate is invalid");
        case QSslError::CertificateNotYetValid:
            return QSslSocket::tr("The certificate is not yet valid");
        case QSslError::CertificateExpired:
            return QSslSocket::tr("The certificate has expired");
        case QSslError::InvalidNotBeforeField:
            return QSslSocket::tr("The certificate's notBefore field contains an invalid time");
        case QSslError::InvalidNotAfterField:
            return QSslSocket::tr("The certificate's notAfter field contains an invalid time");
        case QSslError::SelfSignedCertificate:
            return QSslSocket::tr("The certificate is self-signed, and untrusted");
        case QSslError::SelfSignedCertificateInChain:
            return QSslSocket::tr("The root certificate of the certificate chain is self-signed, and untrusted");
        case QSslError::UnableToGetLocalIssuerCertificate:
            return QSslSocket::tr("The issuer certificate of a locally looked up certificate could not be found");
        case QSslError::UnableToVerifyFirstCertificate:
            return QSslSocket::tr("No certificates could be verified");
        case QSslError::InvalidCaCertificate:
            return QSslSocket::tr("One of the CA certificates is invalid");
        case QSslError::PathLengthExceeded:
            return QSslSocket::tr("The basicConstraints path length parameter has been exceeded");
        case QSslError::InvalidPurpose:
            return QSslSocket::tr("The supplied certificate is unsuitable for this purpose");
        case QSslError::CertificateUntrusted:
            return QSslSocket::tr("The root CA certificate is not trusted for this purpose");
        case CertificateRejected:
            return QSslSocket::tr("The root CA certificate is marked to reject the specified purpose");
        case QSslError::SubjectIssuerMismatch: // hostname mismatch
            return QSslSocket::tr("The current candidate issuer certificate was rejected because its"
                                  " subject name did not match the issuer name of the current certificate");
        case QSslError::AuthorityIssuerSerialNumberMismatch:
            return QSslSocket::tr("The current candidate issuer certificate was rejected because"
                                  " its issuer name and serial number was present and did not match the"
                                  " authority key identifier of the current certificate");
        case QSslError::NoPeerCertificate:
            return QSslSocket::tr("The peer did not present any certificate");
        case QSslError::HostNameMismatch:
            return QSslSocket::tr("The host name did not match any of the valid hosts"
                                  " for this certificate");
        case QSslError::CertificateBlacklisted:
            return QSslSocket::tr("The peer certificate is blacklisted");
        case QSslError::NoSslSupport:
            return QString();
        default:
            return QSslSocket::tr("Unknown error");
    }

    return QString();
}

/*!
    Returns the certificate associated with this error, or a null certificate
    if the error does not relate to any certificate.

    \sa error(), errorString()
*/
QSslCertificate QSslError::certificate() const
{
    return d->certificate;
}

#ifndef QT_NO_DEBUG_STREAM
//class QDebug;
QDebug operator<<(QDebug debug, const QSslError &error)
{
    debug << error.errorString();
    return debug;
}
QDebug operator<<(QDebug debug, const QSslError::SslError &error)
{
    debug << QSslError(error).errorString();
    return debug;
}
#endif

QT_END_NAMESPACE
