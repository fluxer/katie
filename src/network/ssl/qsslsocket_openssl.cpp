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

//#define QSSLSOCKET_DEBUG
//#define QT_DECRYPT_SSL_TRAFFIC

#include "qsslsocket_openssl_p.h"
#include "qsslsocket.h"
#include "qsslcertificate_p.h"
#include "qsslcipher_p.h"

#include <QtCore/qdatetime.h>
#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>
#include <QtCore/qdiriterator.h>
#include <QtCore/qelapsedtimer.h>
#include <QtCore/qfile.h>
#include <QtCore/qmutex.h>
#include <QtCore/qthread.h>
#include <QtCore/qurl.h>
#include "qcorecommon_p.h"

QT_BEGIN_NAMESPACE

bool QSslSocketPrivate::s_libraryLoaded = false;
bool QSslSocketPrivate::s_loadedCiphersAndCerts = false;
bool QSslSocketPrivate::s_loadRootCertsOnDemand = false;

QSslSocketBackendPrivate::QSslSocketBackendPrivate()
    : ssl(0),
      ctx(0),
      pkey(0),
      readBio(0),
      writeBio(0),
      session(0)
{
    // Calls SSL_library_init().
    ensureInitialized();
}

QSslSocketBackendPrivate::~QSslSocketBackendPrivate()
{
    destroySslContext();
}

QSslCipher QSslSocketBackendPrivate::QSslCipher_from_SSL_CIPHER(const SSL_CIPHER *cipher)
{
    QSslCipher ciph;

    QSTACKARRAY(char, buf, 256);
    QString descriptionOneLine = QString::fromLatin1(SSL_CIPHER_description(cipher, buf, sizeof(buf)));

    QStringList descriptionList = descriptionOneLine.split(QLatin1String(" "), QString::SkipEmptyParts);
    if (descriptionList.size() > 5) {
        // ### crude code.
        ciph.d->isNull = false;
        ciph.d->name = descriptionList.at(0);

        QString protoString = descriptionList.at(1);
        ciph.d->protocolString = protoString;
        ciph.d->protocol = QSsl::UnknownProtocol;
        if (protoString == QLatin1String("SSLv3"))
            ciph.d->protocol = QSsl::SslV3;
        else if (protoString == QLatin1String("TLSv1"))
            ciph.d->protocol = QSsl::TlsV1;

        if (descriptionList.at(2).startsWith(QLatin1String("Kx=")))
            ciph.d->keyExchangeMethod = descriptionList.at(2).mid(3);
        if (descriptionList.at(3).startsWith(QLatin1String("Au=")))
            ciph.d->authenticationMethod = descriptionList.at(3).mid(3);
        if (descriptionList.at(4).startsWith(QLatin1String("Enc=")))
            ciph.d->encryptionMethod = descriptionList.at(4).mid(4);
        ciph.d->exportable = (descriptionList.size() > 6 && descriptionList.at(6) == QLatin1String("export"));

        ciph.d->bits = SSL_CIPHER_get_bits(cipher, &ciph.d->supportedBits);
    }
    return ciph;
}

// ### This list is shared between all threads, and protected by a
// mutex. Investigate using thread local storage instead.
struct QSslErrorList
{
    QMutex mutex;
    QList<QPair<int, int> > errors;
};
Q_GLOBAL_STATIC(QSslErrorList, _q_sslErrorList)
static int _q_X509Callback(int ok, X509_STORE_CTX *ctx)
{
    if (!ok) {
        // Store the error and at which depth the error was detected.
        _q_sslErrorList()->errors << qMakePair<int, int>(X509_STORE_CTX_get_error(ctx), X509_STORE_CTX_get_error_depth(ctx));
    }
    // Always return OK to allow verification to continue. We're handle the
    // errors gracefully after collecting all errors, after verification has
    // completed.
    return 1;
}

bool QSslSocketBackendPrivate::initSslContext()
{
    Q_Q(QSslSocket);

    // Create and initialize SSL context. Accept SSLv2, SSLv3 and TLSv1.
    bool client = (mode == QSslSocket::SslClientMode);

    bool reinitialized = false;
init_context:
    switch (configuration.protocol) {
    case QSsl::SslV3:
#ifndef OPENSSL_NO_SSL3_METHOD
        ctx = SSL_CTX_new(client ? SSLv3_client_method() : SSLv3_server_method());
#else
        ctx = 0; // SSL 3 not supported by the system, but chosen deliberately -> error
#endif
        break;
    case QSsl::TlsV1:
        ctx = SSL_CTX_new(client ? TLSv1_client_method() : TLSv1_server_method());
        break;
    case QSsl::SecureProtocols:
    case QSsl::AnyProtocol:
    default:
        ctx = SSL_CTX_new(client ? SSLv23_client_method() : SSLv23_server_method());
        break;
    }
    if (!ctx) {
        // After stopping Flash 10 the SSL library looses its ciphers. Try re-adding them
        // by re-initializing the library.
        if (!reinitialized) {
            reinitialized = true;
            if (SSL_library_init() == 1)
                goto init_context;
        }

        // ### Bad error code
        q->setErrorString(QSslSocket::tr("Error creating SSL context (%1)").arg(getErrorsFromOpenSsl()));
        q->setSocketError(QAbstractSocket::UnknownSocketError);
        emit q->error(QAbstractSocket::UnknownSocketError);
        return false;
    }

    // Enable bug workarounds.
    long options = SSL_OP_ALL;

    // This option is disabled by default, so we need to be able to clear it
    if (configuration.sslOptions & QSsl::SslOptionDisableEmptyFragments)
        options |= SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS;
    else
        options &= ~SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS;

#ifdef SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION
    // This option is disabled by default, so we need to be able to clear it
    if (configuration.sslOptions & QSsl::SslOptionDisableLegacyRenegotiation)
        options &= ~SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION;
    else
        options |= SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION;
#endif

#ifdef SSL_OP_NO_TICKET
    if (configuration.sslOptions & QSsl::SslOptionDisableSessionTickets)
        options |= SSL_OP_NO_TICKET;
#endif
#ifdef SSL_OP_NO_COMPRESSION
    if (configuration.sslOptions & QSsl::SslOptionDisableCompression)
        options |= SSL_OP_NO_COMPRESSION;
#endif

    SSL_CTX_set_options(ctx, options);

    // Initialize ciphers
    QByteArray cipherString;
    int first = true;
    QList<QSslCipher> ciphers = configuration.ciphers;
    if (ciphers.isEmpty())
        ciphers = defaultCiphers();
    foreach (const QSslCipher &cipher, ciphers) {
        if (first)
            first = false;
        else
            cipherString.append(':');
        cipherString.append(cipher.name().toLatin1());
    }

    if (!SSL_CTX_set_cipher_list(ctx, cipherString.data())) {
        // ### Bad error code
        q->setErrorString(QSslSocket::tr("Invalid or empty cipher list (%1)").arg(getErrorsFromOpenSsl()));
        q->setSocketError(QAbstractSocket::UnknownSocketError);
        emit q->error(QAbstractSocket::UnknownSocketError);
        return false;
    }

    // Add all our CAs to this store.
    foreach (const QSslCertificate &caCertificate, q->caCertificates()) {
        // From https://www.openssl.org/docs/ssl/SSL_CTX_load_verify_locations.html:
        //
        // If several CA certificates matching the name, key identifier, and
        // serial number condition are available, only the first one will be
        // examined. This may lead to unexpected results if the same CA
        // certificate is available with different expiration dates. If a
        // ``certificate expired'' verification error occurs, no other
        // certificate will be searched. Make sure to not have expired
        // certificates mixed with valid ones.
        //
        // See also: QSslContext::fromConfiguration()
        if (caCertificate.expiryDate() >= QDateTime::currentDateTime()) {
            X509_STORE_add_cert(SSL_CTX_get_cert_store(ctx), (X509 *)caCertificate.handle());
        }
    }

    if (s_loadRootCertsOnDemand && allowRootCertOnDemandLoading) {
        // tell OpenSSL the directory where to look up the root certs on demand
        SSL_CTX_load_verify_locations(ctx, 0, unixRootCertDirectory().constData());
    }

    if (!configuration.localCertificate.isNull()) {
        // Require a private key as well.
        if (configuration.privateKey.isNull()) {
            q->setErrorString(QSslSocket::tr("Cannot provide a certificate with no key, %1").arg(getErrorsFromOpenSsl()));
            emit q->error(QAbstractSocket::UnknownSocketError);
            return false;
        }

        // Load certificate
        if (!SSL_CTX_use_certificate(ctx, (X509 *)configuration.localCertificate.handle())) {
            q->setErrorString(QSslSocket::tr("Error loading local certificate, %1").arg(getErrorsFromOpenSsl()));
            emit q->error(QAbstractSocket::UnknownSocketError);
            return false;
        }

        // Load private key
        pkey = EVP_PKEY_new();
        // before we were using EVP_PKEY_assign_R* functions and did not use EVP_PKEY_free.
        // this lead to a memory leak. Now we use the *_set1_* functions which do not
        // take ownership of the RSA/DSA key instance because the QSslKey already has ownership.
        switch (configuration.privateKey.algorithm()) {
            case QSsl::Rsa: {
                EVP_PKEY_set1_RSA(pkey, (RSA *)configuration.privateKey.handle());
                break;
            }
            case QSsl::Dsa: {
                EVP_PKEY_set1_DSA(pkey, (DSA *)configuration.privateKey.handle());
                break;
            }
            case QSsl::Dh: {
                EVP_PKEY_set1_DH(pkey, (DH *)configuration.privateKey.handle());
                break;
            }
        }
        if (!SSL_CTX_use_PrivateKey(ctx, pkey)) {
            q->setErrorString(QSslSocket::tr("Error loading private key, %1").arg(getErrorsFromOpenSsl()));
            emit q->error(QAbstractSocket::UnknownSocketError);
            return false;
        }

        // Check if the certificate matches the private key.
        if (!SSL_CTX_check_private_key(ctx)) {
            q->setErrorString(QSslSocket::tr("Private key does not certify public key, %1").arg(getErrorsFromOpenSsl()));
            emit q->error(QAbstractSocket::UnknownSocketError);
            return false;
        }
    }

    // Initialize peer verification.
    if (configuration.peerVerifyMode == QSslSocket::VerifyNone) {
        SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, 0);
    } else {
        SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, _q_X509Callback);
    }

    // Set verification depth.
    if (configuration.peerVerifyDepth != 0)
        SSL_CTX_set_verify_depth(ctx, configuration.peerVerifyDepth);

    // Create and initialize SSL session
    if (!(ssl = SSL_new(ctx))) {
        // ### Bad error code
        q->setErrorString(QSslSocket::tr("Error creating SSL session, %1").arg(getErrorsFromOpenSsl()));
        q->setSocketError(QAbstractSocket::UnknownSocketError);
        emit q->error(QAbstractSocket::UnknownSocketError);
        return false;
    }

#if !defined(OPENSSL_NO_TLSEXT)
    if (configuration.protocol == QSsl::TlsV1 ||
        configuration.protocol == QSsl::SecureProtocols ||
        configuration.protocol == QSsl::AnyProtocol) {
        // Set server hostname on TLS extension. RFC4366 section 3.1 requires it in ACE format.
        QString tlsHostName = verificationPeerName.isEmpty() ? q->peerName() : verificationPeerName;
        if (tlsHostName.isEmpty())
            tlsHostName = hostName;
        QByteArray ace = QUrl::toAce(tlsHostName);
        // only send the SNI header if the URL is valid and not an IP
        if (!ace.isEmpty()
            && !QHostAddress().setAddress(tlsHostName)
            && !(configuration.sslOptions & QSsl::SslOptionDisableServerNameIndication)) {
            if (!SSL_ctrl(ssl, SSL_CTRL_SET_TLSEXT_HOSTNAME, TLSEXT_NAMETYPE_host_name, ace.data()))
                qWarning("could not set SSL_CTRL_SET_TLSEXT_HOSTNAME, Server Name Indication disabled");
        }
    }
#endif

    // Clear the session.
    SSL_clear(ssl);
    errorList.clear();

    // Initialize memory BIOs for encryption and decryption.
    readBio = BIO_new(BIO_s_mem());
    writeBio = BIO_new(BIO_s_mem());
    if (!readBio || !writeBio) {
        // ### Bad error code
        q->setErrorString(QSslSocket::tr("Error creating SSL session: %1").arg(getErrorsFromOpenSsl()));
        q->setSocketError(QAbstractSocket::UnknownSocketError);
        emit q->error(QAbstractSocket::UnknownSocketError);
        return false;
    }

    // Assign the bios.
    SSL_set_bio(ssl, readBio, writeBio);

    if (mode == QSslSocket::SslClientMode)
        SSL_set_connect_state(ssl);
    else
        SSL_set_accept_state(ssl);

    return true;
}

void QSslSocketBackendPrivate::destroySslContext()
{
    if (ssl) {
        // We do not send a shutdown alert here. Just mark the session as
        // resumable for qhttpnetworkconnection's "optimization", otherwise
        // OpenSSL won't start a session resumption.
        SSL_shutdown(ssl);
        SSL_free(ssl);
        ssl = 0;
    }
    if (ctx) {
        SSL_CTX_free(ctx);
        ctx = 0;
    }
    if (pkey) {
        EVP_PKEY_free(pkey);
        pkey = 0;
    }
}

/*!
    \internal

    Does the minimum amount of initialization to determine whether SSL
    is supported or not.
*/

bool QSslSocketPrivate::supportsSsl()
{
    return ensureLibraryLoaded();
}

bool QSslSocketPrivate::ensureLibraryLoaded()
{
    // Check if the library itself needs to be initialized.
    if (!s_libraryLoaded) {
        // Initialize OpenSSL.
        if (SSL_library_init() != 1)
            return false;
        SSL_load_error_strings();
        OpenSSL_add_all_algorithms();

        // Initialize OpenSSL's random seed.
        if (!RAND_status()) {
            int randomish = qrand();
            RAND_seed(&randomish, sizeof(randomish));
            if (!RAND_status()) {
                qWarning("Random number generator not seeded, disabling SSL support");
                return false;
            }

            s_libraryLoaded = true;
        }
    }
    return true;
}

void QSslSocketPrivate::ensureCiphersAndCertsLoaded()
{
    if (s_loadedCiphersAndCerts)
        return;
    s_loadedCiphersAndCerts = true;

    resetDefaultCiphers();

    // check whether we can enable on-demand root-cert loading (i.e. check whether the sym links are there)
    const QStringList symLinkFilter = QStringList() << QLatin1String("[0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f].[0-9]");
    QDirIterator iterator(unixRootCertDirectory(), symLinkFilter, QDir::Files);
    if (iterator.hasNext()) {
        s_loadRootCertsOnDemand = true;
    }
    // if on-demand loading was not enabled, load the certs now
    if (!s_loadRootCertsOnDemand)
        setDefaultCaCertificates(systemCaCertificates());
}

/*!
    \internal

    Declared static in QSslSocketPrivate, makes sure the SSL libraries have
    been initialized.
*/

void QSslSocketPrivate::ensureInitialized()
{
    if (!supportsSsl())
        return;

    ensureCiphersAndCertsLoaded();
}

/*!
    \internal

    Declared static in QSslSocketPrivate, backend-dependent loading of
    application-wide global ciphers.
*/
void QSslSocketPrivate::resetDefaultCiphers()
{
    SSL_CTX *myCtx = SSL_CTX_new(SSLv23_client_method());
    SSL *mySsl = SSL_new(myCtx);

    QList<QSslCipher> ciphers;

    STACK_OF(SSL_CIPHER) *supportedCiphers = SSL_get_ciphers(mySsl);
    for (int i = 0; i < sk_SSL_CIPHER_num(supportedCiphers); ++i) {
        const SSL_CIPHER *cipher = sk_SSL_CIPHER_value(supportedCiphers, i);
        if (cipher) {
            QSslCipher ciph = QSslSocketBackendPrivate::QSslCipher_from_SSL_CIPHER(cipher);
            if (!ciph.isNull()) {
                if (!ciph.name().toLower().startsWith(QLatin1String("adh")))
                    ciphers << ciph;
            }
        }
    }

    SSL_CTX_free(myCtx);
    SSL_free(mySsl);

    setDefaultSupportedCiphers(ciphers);
    setDefaultCiphers(ciphers);
}


QList<QSslCertificate> QSslSocketPrivate::systemCaCertificates()
{
    ensureInitialized();
#ifdef QSSLSOCKET_DEBUG
    QElapsedTimer timer;
    timer.start();
#endif
    QList<QSslCertificate> systemCerts;

    QDir currentDir(unixRootCertDirectory());
    QStringList nameFilters = QStringList() << QLatin1String("*.pem") << QLatin1String("*.crt");
    currentDir.setNameFilters(nameFilters);
    QDirIterator it(currentDir);
    while(it.hasNext()) {
        it.next();
        // use canonical path here to not load the same certificate twice if symlinked
        systemCerts.append(QSslCertificate::fromPath(it.fileInfo().canonicalFilePath()));
    }
    systemCerts.append(QSslCertificate::fromPath(unixRootCertFile(), QSsl::Pem));

#ifdef QSSLSOCKET_DEBUG
    qDebug() << "systemCaCertificates retrieval time " << timer.elapsed() << "ms";
    qDebug() << "imported " << systemCerts.count() << " certificates";
#endif

    return systemCerts;
}

void QSslSocketBackendPrivate::startClientEncryption()
{
    if (!initSslContext()) {
        // ### report error: internal OpenSSL failure
        return;
    }

    // Start connecting. This will place outgoing data in the BIO, so we
    // follow up with calling transmit().
    startHandshake();
    transmit();
}

void QSslSocketBackendPrivate::startServerEncryption()
{
    if (!initSslContext()) {
        // ### report error: internal OpenSSL failure
        return;
    }

    // Start connecting. This will place outgoing data in the BIO, so we
    // follow up with calling transmit().
    startHandshake();
    transmit();
}

/*!
    \internal

    Transmits encrypted data between the BIOs and the socket.
*/
void QSslSocketBackendPrivate::transmit()
{
    Q_Q(QSslSocket);

    // If we don't have any SSL context, don't bother transmitting.
    if (!ssl)
        return;

    bool transmitting;
    do {
        transmitting = false;

        // If the connection is secure, we can transfer data from the write
        // buffer (in plain text) to the write BIO through SSL_write.
        if (connectionEncrypted && !writeBuffer.isEmpty()) {
            qint64 totalBytesWritten = 0;
            int nextDataBlockSize;
            while ((nextDataBlockSize = writeBuffer.nextDataBlockSize()) > 0) {
                int writtenBytes = SSL_write(ssl, writeBuffer.readPointer(), nextDataBlockSize);
                if (writtenBytes <= 0) {
                    // ### Better error handling.
                    q->setErrorString(QSslSocket::tr("Unable to write data: %1").arg(getErrorsFromOpenSsl()));
                    q->setSocketError(QAbstractSocket::UnknownSocketError);
                    emit q->error(QAbstractSocket::UnknownSocketError);
                    return;
                }
#ifdef QSSLSOCKET_DEBUG
                qDebug() << "QSslSocketBackendPrivate::transmit: encrypted" << writtenBytes << "bytes";
#endif
                writeBuffer.free(writtenBytes);
                totalBytesWritten += writtenBytes;

                if (writtenBytes < nextDataBlockSize) {
                    // break out of the writing loop and try again after we had read
                    transmitting = true;
                    break;
                }
            }

            if (totalBytesWritten > 0) {
                // Don't emit bytesWritten() recursively.
                if (!emittedBytesWritten) {
                    emittedBytesWritten = true;
                    emit q->bytesWritten(totalBytesWritten);
                    emittedBytesWritten = false;
                }
            }
        }

        // Check if we've got any data to be written to the socket.
        int pendingBytes;
        while (plainSocket->isValid() && (pendingBytes = BIO_pending(writeBio)) > 0
            && plainSocket->openMode() != QIODevice::NotOpen) {
            // Read encrypted data from the write BIO into a buffer.
            QSTACKARRAY(char, data, pendingBytes);
            int encryptedBytesRead = BIO_read(writeBio, data, pendingBytes);

            // Write encrypted data from the buffer to the socket.
            qint64 actualWritten = plainSocket->write(data, encryptedBytesRead);
#ifdef QSSLSOCKET_DEBUG
            qDebug() << "QSslSocketBackendPrivate::transmit: wrote" << encryptedBytesRead << "encrypted bytes to the socket" << actualWritten << "actual.";
#endif
            if (actualWritten < 0) {
                //plain socket write fails if it was in the pending close state.
                q->setErrorString(plainSocket->errorString());
                q->setSocketError(plainSocket->error());
                emit q->error(plainSocket->error());
                return;
            }
            transmitting = true;
        }

        // Check if we've got any data to be read from the socket.
        if (!connectionEncrypted || !readBufferMaxSize || readBuffer.size() < readBufferMaxSize)
            while ((pendingBytes = plainSocket->bytesAvailable()) > 0) {
                // Read encrypted data from the socket into a buffer.
                QSTACKARRAY(char, data, pendingBytes);
                // just peek() here because BIO_write could write less data than expected
                int encryptedBytesRead = plainSocket->peek(data, pendingBytes);
#ifdef QSSLSOCKET_DEBUG
                qDebug() << "QSslSocketBackendPrivate::transmit: read" << encryptedBytesRead << "encrypted bytes from the socket";
#endif
                // Write encrypted data from the buffer into the read BIO.
                int writtenToBio = BIO_write(readBio, data, encryptedBytesRead);

                // do the actual read() here and throw away the results.
                if (writtenToBio > 0) {
                    // ### TODO: make this cheaper by not making it memcpy. E.g. make it work with data=0x0 or make it work with seek
                    plainSocket->read(data, writtenToBio);
                } else {
                    // ### Better error handling.
                    q->setErrorString(QSslSocket::tr("Unable to decrypt data: %1").arg(getErrorsFromOpenSsl()));
                    q->setSocketError(QAbstractSocket::UnknownSocketError);
                    emit q->error(QAbstractSocket::UnknownSocketError);
                    return;
                }

                transmitting = true;
            }

        // If the connection isn't secured yet, this is the time to retry the
        // connect / accept.
        if (!connectionEncrypted) {
#ifdef QSSLSOCKET_DEBUG
            qDebug() << "QSslSocketBackendPrivate::transmit: testing encryption";
#endif
            if (startHandshake()) {
#ifdef QSSLSOCKET_DEBUG
                qDebug() << "QSslSocketBackendPrivate::transmit: encryption established";
#endif
                connectionEncrypted = true;
                transmitting = true;
            } else if (plainSocket->state() != QAbstractSocket::ConnectedState) {
#ifdef QSSLSOCKET_DEBUG
                qDebug() << "QSslSocketBackendPrivate::transmit: connection lost";
#endif
                break;
            } else {
#ifdef QSSLSOCKET_DEBUG
                qDebug() << "QSslSocketBackendPrivate::transmit: encryption not done yet";
#endif
            }
        }

        // If the request is small and the remote host closes the transmission
        // after sending, there's a chance that startHandshake() will already
        // have triggered a shutdown.
        if (!ssl)
            continue;

        // We always read everything from the SSL decryption buffers, even if
        // we have a readBufferMaxSize. There's no point in leaving data there
        // just so that readBuffer.size() == readBufferMaxSize.
        int readBytes = 0;
        const int bytesToRead = 4096;
        do {
            // Don't use SSL_pending(). It's very unreliable.
            readBytes = SSL_read(ssl, buffer.reserve(bytesToRead), bytesToRead);
            if (readBytes > 0) {
#ifdef QSSLSOCKET_DEBUG
                qDebug() << "QSslSocketBackendPrivate::transmit: decrypted" << readBytes << "bytes";
#endif
                buffer.chop(bytesToRead - readBytes);

                if (readyReadEmittedPointer)
                    *readyReadEmittedPointer = true;
                emit q->readyRead();
                transmitting = true;
                continue;
            }
            buffer.chop(bytesToRead);

            // Error.
            switch (SSL_get_error(ssl, readBytes)) {
            case SSL_ERROR_WANT_READ:
            case SSL_ERROR_WANT_WRITE:
                // Out of data.
                break;
            case SSL_ERROR_ZERO_RETURN:
                // The remote host closed the connection.
#ifdef QSSLSOCKET_DEBUG
                qDebug() << "QSslSocketBackendPrivate::transmit: remote disconnect";
#endif
                shutdown = true; // the other side shut down, make sure we do not send shutdown ourselves
                q->setErrorString(QSslSocket::tr("The TLS/SSL connection has been closed"));
                q->setSocketError(QAbstractSocket::RemoteHostClosedError);
                emit q->error(QAbstractSocket::RemoteHostClosedError);
                return;
            case SSL_ERROR_SYSCALL: // some IO error
            case SSL_ERROR_SSL: // error in the SSL library
                // we do not know exactly what the error is, nor whether we can recover from it,
                // so just return to prevent an endless loop in the outer "while" statement
                q->setErrorString(QSslSocket::tr("Error while reading: %1").arg(getErrorsFromOpenSsl()));
                q->setSocketError(QAbstractSocket::UnknownSocketError);
                emit q->error(QAbstractSocket::UnknownSocketError);
                return;
            default:
                // SSL_ERROR_WANT_CONNECT, SSL_ERROR_WANT_ACCEPT: can only happen with a
                // BIO_s_connect() or BIO_s_accept(), which we do not call.
                // SSL_ERROR_WANT_X509_LOOKUP: can only happen with a
                // SSL_CTX_set_client_cert_cb(), which we do not call.
                // So this default case should never be triggered.
                q->setErrorString(QSslSocket::tr("Error while reading: %1").arg(getErrorsFromOpenSsl()));
                q->setSocketError(QAbstractSocket::UnknownSocketError);
                emit q->error(QAbstractSocket::UnknownSocketError);
                break;
            }
        } while (ssl && readBytes > 0);
    } while (ssl && ctx && transmitting);
}

static QSslError _q_OpenSSL_to_QSslError(int errorCode, const QSslCertificate &cert)
{
    switch (errorCode) {
    case X509_V_OK:
        // X509_V_OK is also reported if the peer had no certificate.
        return QSslError();
    case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
        return QSslError(QSslError::UnableToGetIssuerCertificate, cert);
    case X509_V_ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE:
        return QSslError(QSslError::UnableToDecryptCertificateSignature, cert);
    case X509_V_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY:
        return QSslError(QSslError::UnableToDecodeIssuerPublicKey, cert);
    case X509_V_ERR_CERT_SIGNATURE_FAILURE:
        return QSslError(QSslError::CertificateSignatureFailed, cert);
    case X509_V_ERR_CERT_NOT_YET_VALID:
        return QSslError(QSslError::CertificateNotYetValid, cert);
    case X509_V_ERR_CERT_HAS_EXPIRED:
        return QSslError(QSslError::CertificateExpired, cert);
    case X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD:
        return QSslError(QSslError::InvalidNotBeforeField, cert);
    case X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD:
        return QSslError(QSslError::InvalidNotAfterField, cert);
    case X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT:
        return QSslError(QSslError::SelfSignedCertificate, cert);
    case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
        return QSslError(QSslError::SelfSignedCertificateInChain, cert);
    case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY:
        return QSslError(QSslError::UnableToGetLocalIssuerCertificate, cert);
    case X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE:
        return QSslError(QSslError::UnableToVerifyFirstCertificate, cert);
    case X509_V_ERR_CERT_REVOKED:
        return QSslError(QSslError::CertificateRevoked, cert);
    case X509_V_ERR_INVALID_CA:
        return QSslError(QSslError::InvalidCaCertificate, cert);
    case X509_V_ERR_PATH_LENGTH_EXCEEDED:
        return QSslError(QSslError::PathLengthExceeded, cert);
    case X509_V_ERR_INVALID_PURPOSE:
        return QSslError(QSslError::InvalidPurpose, cert);
    case X509_V_ERR_CERT_UNTRUSTED:
        return QSslError(QSslError::CertificateUntrusted, cert);
    case X509_V_ERR_CERT_REJECTED:
        return QSslError(QSslError::CertificateRejected, cert);
    default:
        return QSslError(QSslError::UnspecifiedError, cert);
    }
    Q_UNREACHABLE();
}

bool QSslSocketBackendPrivate::startHandshake()
{
    Q_Q(QSslSocket);

    // Check if the connection has been established. Get all errors from the
    // verification stage.
    QMutexLocker locker(&_q_sslErrorList()->mutex);
    _q_sslErrorList()->errors.clear();
    int result = (mode == QSslSocket::SslClientMode) ? SSL_connect(ssl) : SSL_accept(ssl);

    const QList<QPair<int, int> > &lastErrors = _q_sslErrorList()->errors;
    for (int i = 0; i < lastErrors.size(); ++i) {
        const QPair<int, int> &currentError = lastErrors.at(i);
        // Initialize the peer certificate chain in order to find which certificate caused this error
        if (configuration.peerCertificateChain.isEmpty())
            configuration.peerCertificateChain = STACKOFX509_to_QSslCertificates(SSL_get_peer_cert_chain(ssl));
        emit q->peerVerifyError(_q_OpenSSL_to_QSslError(currentError.first,
                                configuration.peerCertificateChain.value(currentError.second)));
        if (q->state() != QAbstractSocket::ConnectedState)
            break;
    }

    errorList << lastErrors;
    locker.unlock();

    // Connection aborted during handshake phase.
    if (q->state() != QAbstractSocket::ConnectedState)
        return false;

    // Check if we're encrypted or not.
    if (result <= 0) {
        switch (SSL_get_error(ssl, result)) {
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:
            // The handshake is not yet complete.
            break;
        default:
            q->setErrorString(QSslSocket::tr("Error during SSL handshake: %1").arg(getErrorsFromOpenSsl()));
            q->setSocketError(QAbstractSocket::SslHandshakeFailedError);
#ifdef QSSLSOCKET_DEBUG
            qDebug() << "QSslSocketBackendPrivate::startHandshake: error!" << q->errorString();
#endif
            emit q->error(QAbstractSocket::SslHandshakeFailedError);
            q->abort();
        }
        return false;
    }

    // Store the peer certificate and chain. For clients, the peer certificate
    // chain includes the peer certificate; for servers, it doesn't. Both the
    // peer certificate and the chain may be empty if the peer didn't present
    // any certificate.
    if (configuration.peerCertificateChain.isEmpty())
        configuration.peerCertificateChain = STACKOFX509_to_QSslCertificates(SSL_get_peer_cert_chain(ssl));
    X509 *x509 = SSL_get_peer_certificate(ssl);
    configuration.peerCertificate = QSslCertificatePrivate::QSslCertificate_from_X509(x509);
    X509_free(x509);

    // Start translating errors.
    QList<QSslError> errors;

    // check the whole chain for blacklisting (including root, as we check for subjectInfo and issuer)
    foreach (const QSslCertificate &cert, configuration.peerCertificateChain) {
        if (QSslCertificatePrivate::isBlacklisted(cert)) {
            QSslError error(QSslError::CertificateBlacklisted, cert);
            errors << error;
            emit q->peerVerifyError(error);
            if (q->state() != QAbstractSocket::ConnectedState)
                return false;
        }
    }

    bool doVerifyPeer = configuration.peerVerifyMode == QSslSocket::VerifyPeer
                        || (configuration.peerVerifyMode == QSslSocket::AutoVerifyPeer
                            && mode == QSslSocket::SslClientMode);

    // Check the peer certificate itself. First try the subject's common name
    // (CN) as a wildcard, then try all alternate subject name DNS entries the
    // same way.
    if (!configuration.peerCertificate.isNull()) {
        // but only if we're a client connecting to a server
        // if we're the server, don't check CN
        if (mode == QSslSocket::SslClientMode) {
            QString peerName = (verificationPeerName.isEmpty () ? q->peerName() : verificationPeerName);
            QString commonName = configuration.peerCertificate.subjectInfo(QSslCertificate::CommonName);

            if (!isMatchingHostname(commonName.toLower(), peerName.toLower())) {
                bool matched = false;
                foreach (const QString &altName, configuration.peerCertificate
                         .alternateSubjectNames().values(QSsl::DnsEntry)) {
                    if (isMatchingHostname(altName.toLower(), peerName.toLower())) {
                        matched = true;
                        break;
                    }
                }

                if (!matched) {
                    // No matches in common names or alternate names.
                    QSslError error(QSslError::HostNameMismatch, configuration.peerCertificate);
                    errors << error;
                    emit q->peerVerifyError(error);
                    if (q->state() != QAbstractSocket::ConnectedState)
                        return false;
                }
            }
        }
    } else {
        // No peer certificate presented. Report as error if the socket
        // expected one.
        if (doVerifyPeer) {
            QSslError error(QSslError::NoPeerCertificate);
            errors << error;
            emit q->peerVerifyError(error);
            if (q->state() != QAbstractSocket::ConnectedState)
                return false;
        }
    }

    // Translate errors from the error list into QSslErrors.
    for (int i = 0; i < errorList.size(); ++i) {
        const QPair<int, int> &errorAndDepth = errorList.at(i);
        int err = errorAndDepth.first;
        int depth = errorAndDepth.second;
        errors << _q_OpenSSL_to_QSslError(err, configuration.peerCertificateChain.value(depth));
    }

    if (!errors.isEmpty()) {
        sslErrors = errors;
        emit q->sslErrors(errors);

        bool doEmitSslError;
        if (!ignoreErrorsList.empty()) {
            // check whether the errors we got are all in the list of expected errors
            // (applies only if the method QSslSocket::ignoreSslErrors(const QList<QSslError> &errors)
            // was called)
            doEmitSslError = false;
            for (int a = 0; a < errors.count(); a++) {
                if (!ignoreErrorsList.contains(errors.at(a))) {
                    doEmitSslError = true;
                    break;
                }
            }
        } else {
            // if QSslSocket::ignoreSslErrors(const QList<QSslError> &errors) was not called and
            // we get an SSL error, emit a signal unless we ignored all errors (by calling
            // QSslSocket::ignoreSslErrors() )
            doEmitSslError = !ignoreAllSslErrors;
        }
        // check whether we need to emit an SSL handshake error
        if (doVerifyPeer && doEmitSslError) {
            q->setErrorString(sslErrors.first().errorString());
            q->setSocketError(QAbstractSocket::SslHandshakeFailedError);
            emit q->error(QAbstractSocket::SslHandshakeFailedError);
            plainSocket->disconnectFromHost();
            return false;
        }
    } else {
        sslErrors.clear();
    }

    // if we have a max read buffer size, reset the plain socket's to 32k
    if (readBufferMaxSize)
        plainSocket->setReadBufferSize(32768);

#ifdef QT_DECRYPT_SSL_TRAFFIC
    const SSL_SESSION* const session = SSL_get_session(ssl);
    if (session) {
        QSTACKARRAY(uchar, mk, SSL_MAX_MASTER_KEY_LENGTH);
        SSL_SESSION_get_master_key(session, mk, SSL_MAX_MASTER_KEY_LENGTH);
        QByteArray masterKey(reinterpret_cast<char *>(mk));

        QSTACKARRAY(uchar, random, SSL3_RANDOM_SIZE);
        SSL_get_client_random(ssl, random, SSL3_RANDOM_SIZE);
        QByteArray clientRandom(reinterpret_cast<const char *>(random));

        // different format, needed for e.g. older Wireshark versions:
//        const char *sid = reinterpret_cast<const char *>(session->session_id);
//        QByteArray sessionID(sid, session->session_id_length);
//        QByteArray debugLineRSA("RSA Session-ID:");
//        debugLineRSA.append(sessionID.toHex().toUpper());
//        debugLineRSA.append(" Master-Key:");
//        debugLineRSA.append(masterKey.toHex().toUpper());
//        debugLineRSA.append("\n");

        QByteArray debugLineClientRandom("CLIENT_RANDOM ");
        debugLineClientRandom.append(clientRandom.toHex().toUpper());
        debugLineClientRandom.append(" ");
        debugLineClientRandom.append(masterKey.toHex().toUpper());
        debugLineClientRandom.append("\n");

        QString sslKeyFile = QDir::tempPath() + QLatin1String("/qt-ssl-keys");
        QFile file(sslKeyFile);
        if (!file.open(QIODevice::Append))
            qWarning() << "could not open file" << sslKeyFile << "for appending";
        if (!file.write(debugLineClientRandom))
            qWarning() << "could not write to file" << sslKeyFile;
        file.close();
    } else {
        qWarning("could not decrypt SSL traffic");
    }
#endif

    connectionEncrypted = true;
    emit q->encrypted();
    if (autoStartHandshake && pendingClose) {
        pendingClose = false;
        q->disconnectFromHost();
    }
    return true;
}

void QSslSocketBackendPrivate::disconnectFromHost()
{
    if (ssl) {
        if (!shutdown) {
            SSL_shutdown(ssl);
            shutdown = true;
            transmit();
        }
    }
    plainSocket->disconnectFromHost();
}

void QSslSocketBackendPrivate::disconnected()
{
    if (plainSocket->bytesAvailable() <= 0)
        destroySslContext();
    //if there is still buffered data in the plain socket, don't destroy the ssl context yet.
    //it will be destroyed when the socket is deleted.
}

QSslCipher QSslSocketBackendPrivate::sessionCipher() const
{
    if (!ssl || !ctx)
        return QSslCipher();
    const SSL_CIPHER *sessionCipher = SSL_get_current_cipher(ssl);
    return sessionCipher ? QSslCipher_from_SSL_CIPHER(sessionCipher) : QSslCipher();
}

QList<QSslCertificate> QSslSocketBackendPrivate::STACKOFX509_to_QSslCertificates(STACK_OF(X509) *x509)
{
    ensureInitialized();
    QList<QSslCertificate> certificates;
    for (int i = 0; i < sk_X509_num(x509); ++i) {
        if (X509 *entry = sk_X509_value(x509, i))
            certificates << QSslCertificatePrivate::QSslCertificate_from_X509(entry);
    }
    return certificates;
}

QString QSslSocketBackendPrivate::getErrorsFromOpenSsl()
{
    QString errorString;
    unsigned long errNum;
    while((errNum = ERR_get_error())) {
        if (! errorString.isEmpty())
            errorString.append(QLatin1String(", "));
        const char *error = ERR_error_string(errNum, NULL);
        errorString.append(QString::fromAscii(error)); // error is ascii according to man ERR_error_string
    }
    return errorString;
}

bool QSslSocketBackendPrivate::isMatchingHostname(const QString &cn, const QString &hostname)
{
    int wildcard = cn.indexOf(QLatin1Char('*'));

    // Check this is a wildcard cert, if not then just compare the strings
    if (wildcard < 0)
        return cn == hostname;

    int firstCnDot = cn.indexOf(QLatin1Char('.'));
    int secondCnDot = cn.indexOf(QLatin1Char('.'), firstCnDot+1);

    // Check at least 3 components
    if ((-1 == secondCnDot) || (secondCnDot+1 >= cn.length()))
        return false;

    // Check * is last character of 1st component (ie. there's a following .)
    if (wildcard+1 != firstCnDot)
        return false;

    // Check only one star
    if (cn.lastIndexOf(QLatin1Char('*')) != wildcard)
        return false;

    // Check characters preceding * (if any) match
    if (wildcard && (hostname.leftRef(wildcard) != cn.leftRef(wildcard)))
        return false;

    // Check characters following first . match
    if (hostname.midRef(hostname.indexOf(QLatin1Char('.'))) != cn.midRef(firstCnDot))
        return false;

    // Check if the hostname is an IP address, if so then wildcards are not allowed
    QHostAddress addr(hostname);
    if (!addr.isNull())
        return false;

    // Ok, I guess this was a wildcard CN and the hostname matches.
    return true;
}

QT_END_NAMESPACE
