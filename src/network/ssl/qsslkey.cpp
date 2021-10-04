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

#include "qsslsocket_openssl_p.h"
#include "qsslkey.h"
#include "qsslkey_p.h"
#include "qsslsocket.h"
#include "qsslsocket_p.h"
#include <QtCore/qatomic.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qiodevice.h>

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

/*!
    \class QSslKey
    \brief The QSslKey class provides an interface for private and public keys.
    \since 4.3

    \reentrant
    \ingroup network
    \ingroup ssl
    \inmodule QtNetwork

    QSslKey provides a simple API for managing keys.

    \sa QSslSocket, QSslCertificate, QSslCipher
*/

QT_BEGIN_NAMESPACE

/*!
    \internal
 */
void QSslKeyPrivate::clear()
{
    isNull = true;
    if (!QSslSocket::supportsSsl())
        return;
    if (rsa) {
        RSA_free(rsa);
        rsa = 0;
    }
    if (dsa) {
        DSA_free(dsa);
        dsa = 0;
    }

    if (dh) {
        DH_free(dh);
        dh = 0;
    }
}

/*!
    \internal

    Allocates a new rsa or dsa struct and decodes \a pem into it
    according to the current algorithm and type.

    If \a passPhrase is non-empty, it will be used for decrypting
    \a pem.
*/
void QSslKeyPrivate::decodePem(const QByteArray &pem, const QByteArray &passPhrase)
{
    if (pem.isEmpty())
        return;

    clear();

    if (!QSslSocket::supportsSsl())
        return;

    BIO *bio = BIO_new_mem_buf(pem.constData(), pem.size());
    if (!bio)
        return;

    void *phrase = (void *)passPhrase.constData();

    switch (algorithm) {
        case QSsl::Rsa: {
            RSA *result = (type == QSsl::PublicKey)
                ? PEM_read_bio_RSA_PUBKEY(bio, &rsa, 0, phrase)
                : PEM_read_bio_RSAPrivateKey(bio, &rsa, 0, phrase);
            if (rsa && rsa == result)
                isNull = false;
            break;
        }
        case QSsl::Dsa: {
            DSA *result = (type == QSsl::PublicKey)
                ? PEM_read_bio_DSA_PUBKEY(bio, &dsa, 0, phrase)
                : PEM_read_bio_DSAPrivateKey(bio, &dsa, 0, phrase);
            if (dsa && dsa == result)
                isNull = false;
            break;
        }
        case QSsl::Dh: {
            EVP_PKEY *result = (type == QSsl::PublicKey)
                ? PEM_read_bio_PUBKEY(bio, 0, 0, phrase)
                : PEM_read_bio_PrivateKey(bio, 0, 0, phrase);
            if (result)
                dh = EVP_PKEY_get1_DH(result);
            if (dh)
                isNull = false;
            EVP_PKEY_free(result);
            break;
        }
    }

    BIO_free(bio);
}

/*!
    Constructs a null key.

    \sa isNull()
*/
QSslKey::QSslKey()
    : d(new QSslKeyPrivate)
{
}

/*!
    \internal
*/
QByteArray QSslKeyPrivate::pemHeader() const
{
    if (type == QSsl::PublicKey)
        return QByteArray::fromRawData("-----BEGIN PUBLIC KEY-----\n", 27);
    switch (algorithm) {
        case QSsl::Rsa:
            return QByteArray::fromRawData("-----BEGIN RSA PRIVATE KEY-----\n", 32);
        case QSsl::Dsa:
            return QByteArray::fromRawData("-----BEGIN DSA PRIVATE KEY-----\n", 32);
        case QSsl::Dh:
            return QByteArray::fromRawData("-----BEGIN PRIVATE KEY-----\n", 28);
    }
    return QByteArray();
}

/*!
    \internal
*/
QByteArray QSslKeyPrivate::pemFooter() const
{
    if (type == QSsl::PublicKey)
        return QByteArray::fromRawData("-----END PUBLIC KEY-----\n", 25);
    switch (algorithm) {
        case QSsl::Rsa:
            return QByteArray::fromRawData("-----END RSA PRIVATE KEY-----\n", 30);
        case QSsl::Dsa:
            return QByteArray::fromRawData("-----END DSA PRIVATE KEY-----\n", 30);
        case QSsl::Dh:
            return QByteArray::fromRawData("-----END PRIVATE KEY-----\n", 26);
    }
    return QByteArray();
}

/*!
    \internal

    Returns a DER key formatted as PEM.
*/
QByteArray QSslKeyPrivate::pemFromDer(const QByteArray &der) const
{
    QByteArray pem(der.toBase64());

    const int lineWidth = 64; // RFC 1421
    const int newLines = pem.size() / lineWidth;
    const bool rem = pem.size() % lineWidth;

    // ### optimize
    for (int i = 0; i < newLines; ++i)
        pem.insert((i + 1) * lineWidth + i, '\n');
    if (rem)
        pem.append('\n'); // ###

    pem.prepend(pemHeader());
    pem.append(pemFooter());

    return pem;
}

/*!
    \internal

    Returns a PEM key formatted as DER.
*/
QByteArray QSslKeyPrivate::derFromPem(const QByteArray &pem) const
{
    const QByteArray header = pemHeader();
    const QByteArray footer = pemFooter();

    QByteArray der(pem);

    const int headerIndex = der.indexOf(header);
    const int footerIndex = der.indexOf(footer);
    if (headerIndex == -1 || footerIndex == -1)
        return QByteArray();

    der = der.mid(headerIndex + header.size(), footerIndex - (headerIndex + header.size()));

    return QByteArray::fromBase64(der); // ignores newlines
}

/*!
    Constructs a QSslKey by decoding the string in the byte array
    \a encoded using a specified \a algorithm and \a encoding format.
    If the encoded key is encrypted, \a passPhrase is used to decrypt
    it. \a type specifies whether the key is public or private.

    After construction, use isNull() to check if \a encoded contained
    a valid key.
*/
QSslKey::QSslKey(const QByteArray &encoded, QSsl::KeyAlgorithm algorithm,
                 QSsl::EncodingFormat encoding, QSsl::KeyType type, const QByteArray &passPhrase)
    : d(new QSslKeyPrivate)
{
    d->type = type;
    d->algorithm = algorithm;
    d->decodePem((encoding == QSsl::Der)
                 ? d->pemFromDer(encoded) : encoded,
                 passPhrase);
}

/*!
    Constructs a QSslKey by reading and decoding data from a
    \a device using a specified \a algorithm and \a encoding format.
    If the encoded key is encrypted, \a passPhrase is used to decrypt
    it. \a type specifies whether the key is public or private.

    After construction, use isNull() to check if \a device provided
    a valid key.
*/
QSslKey::QSslKey(QIODevice *device, QSsl::KeyAlgorithm algorithm, QSsl::EncodingFormat encoding,
                 QSsl::KeyType type, const QByteArray &passPhrase)
    : d(new QSslKeyPrivate)
{
    QByteArray encoded;
    if (device)
        encoded = device->readAll();
    d->type = type;
    d->algorithm = algorithm;
    d->decodePem((encoding == QSsl::Der) ?
                 d->pemFromDer(encoded) : encoded,
                 passPhrase);
}

/*!
    Constructs an identical copy of \a other.
*/
QSslKey::QSslKey(const QSslKey &other) : d(other.d)
{
}

/*!
    Destroys the QSslKey object.
*/
QSslKey::~QSslKey()
{
}

/*!
    Copies the contents of \a other into this key, making the two keys
    identical.

    Returns a reference to this QSslKey.
*/
QSslKey &QSslKey::operator=(const QSslKey &other)
{
    d = other.d;
    return *this;
}

/*!
    Returns true if this is a null key; otherwise false.

    \sa clear()
*/
bool QSslKey::isNull() const
{
    return d->isNull;
}

/*!
    Clears the contents of this key, making it a null key.

    \sa isNull()
*/
void QSslKey::clear()
{
    d = new QSslKeyPrivate;
}

/*!
    Returns the length of the key in bits, or -1 if the key is null.
*/
int QSslKey::length() const
{
    if (d->isNull)
        return -1;
    switch (d->algorithm) {
        case QSsl::Rsa: {
            return RSA_bits(d->rsa);
        }
        case QSsl::Dsa: {
            const BIGNUM *p = NULL;
            DSA_get0_pqg(d->dsa, &p, NULL, NULL);
            return BN_num_bits(p);
        }
        case QSsl::Dh: {
            return DH_bits(d->dh);
        }
    }
    return -1;
}

/*!
    Returns the type of the key (i.e., PublicKey or PrivateKey).
*/
QSsl::KeyType QSslKey::type() const
{
    return d->type;
}

/*!
    Returns the key algorithm.
*/
QSsl::KeyAlgorithm QSslKey::algorithm() const
{
    return d->algorithm;
}

/*!
  Returns the key in DER encoding. The result is encrypted with
  \a passPhrase if the key is a private key and \a passPhrase is
  non-empty.
*/
// ### autotest failure for non-empty passPhrase and private key
QByteArray QSslKey::toDer(const QByteArray &passPhrase) const
{
    if (d->isNull)
        return QByteArray();
    return d->derFromPem(toPem(passPhrase));
}

/*!
  Returns the key in PEM encoding. The result is encrypted with
  \a passPhrase if the key is a private key and \a passPhrase is
  non-empty.
*/
QByteArray QSslKey::toPem(const QByteArray &passPhrase) const
{
    if (!QSslSocket::supportsSsl() || d->isNull)
        return QByteArray();

    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio)
        return QByteArray();

    bool fail = false;

    switch (d->algorithm) {
        case QSsl::Rsa: {
            if (d->type == QSsl::PublicKey) {
                if (!PEM_write_bio_RSA_PUBKEY(bio, d->rsa))
                    fail = true;
            } else {
                if (!PEM_write_bio_RSAPrivateKey(
                    bio, d->rsa,
                    // ### the cipher should be selectable in the API:
                    passPhrase.isEmpty() ? (const EVP_CIPHER *)0 : EVP_des_ede3_cbc(),
                    (uchar *)passPhrase.data(), passPhrase.size(), 0, 0)) {
                    fail = true;
                }
            }
            break;
        }
        case QSsl::Dsa: {
            if (d->type == QSsl::PublicKey) {
                if (!PEM_write_bio_DSA_PUBKEY(bio, d->dsa))
                    fail = true;
            } else {
                if (!PEM_write_bio_DSAPrivateKey(
                    bio, d->dsa,
                    // ### the cipher should be selectable in the API:
                    passPhrase.isEmpty() ? (const EVP_CIPHER *)0 : EVP_des_ede3_cbc(),
                    (uchar *)passPhrase.data(), passPhrase.size(), 0, 0)) {
                    fail = true;
                }
            }
            break;
        }
        case QSsl::Dh: {
            EVP_PKEY *result = EVP_PKEY_new();
            if (!result || !EVP_PKEY_set1_DH(result, d->dh)) {
                fail = true;
            } else if (d->type == QSsl::PublicKey) {
                if (!PEM_write_bio_PUBKEY(bio, result))
                    fail = true;
            } else if (!PEM_write_bio_PrivateKey(
                // ### the cipher should be selectable in the API:
                bio, result, passPhrase.isEmpty() ? (const EVP_CIPHER *)0 : EVP_des_ede3_cbc(),
                (uchar *)passPhrase.data(), passPhrase.size(), 0, 0)) {
                fail = true;
            }
            EVP_PKEY_free(result);
            break;
        }
    }

    QByteArray pem;
    if (!fail) {
        char *data;
        long size = BIO_get_mem_data(bio, &data);
        pem = QByteArray(data, size);
    }
    BIO_free(bio);
    return pem;
}

/*!
    Returns a pointer to the native key handle, if it is available;
    otherwise a null pointer is returned.

    You can use this handle together with the native API to access
    extended information about the key.

    \warning Use of this function has a high probability of being
    non-portable, and its return value may vary across platforms, and
    between minor Qt releases.
*/
Qt::HANDLE QSslKey::handle() const
{
    switch (d->algorithm) {
        case QSsl::Rsa:
            return Qt::HANDLE(d->rsa);
        case QSsl::Dsa:
            return Qt::HANDLE(d->dsa);
        case QSsl::Dh:
            return Qt::HANDLE(d->dh);
    }
    Q_UNREACHABLE();
}

/*!
    Returns true if this key is equal to \a other; otherwise returns false.
*/
bool QSslKey::operator==(const QSslKey &other) const
{
    if (isNull())
        return other.isNull();
    if (other.isNull())
        return isNull();
    if (algorithm() != other.algorithm())
        return false;
    if (type() != other.type())
        return false;
    if (length() != other.length())
        return false;
    return toDer() == other.toDer();
}

/*! \fn bool QSslKey::operator!=(const QSslKey &other) const

  Returns true if this key is not equal to key \a other; otherwise
  returns false.
*/

#ifndef QT_NO_DEBUG_STREAM
class QDebug;
QDebug operator<<(QDebug debug, const QSslKey &key)
{
    debug << "QSslKey("
          << (key.type() == QSsl::PublicKey ? "PublicKey" : "PrivateKey");
    switch (key.algorithm()) {
        case QSsl::Rsa: {
            debug << ", " << "RSA";
            break;
        }
        case QSsl::Dsa: {
            debug << ", " << "DSA";
            break;
        }
        case QSsl::Dh: {
            debug << ", " << "DH";
            break;
        }
    }
    debug << ", " << key.length()
          << ')';
    return debug;
}
#endif

QT_END_NAMESPACE
