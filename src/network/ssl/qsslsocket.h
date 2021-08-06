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


#ifndef QSSLSOCKET_H
#define QSSLSOCKET_H

#include <QtCore/qlist.h>
#include <QtCore/qregexp.h>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qsslerror.h>


QT_BEGIN_NAMESPACE

class QSslCipher;
class QSslCertificate;
class QSslConfiguration;

class QSslSocketPrivate;
class Q_NETWORK_EXPORT QSslSocket : public QTcpSocket
{
    Q_OBJECT
public:
    enum SslMode {
        UnencryptedMode,
        SslClientMode,
        SslServerMode
    };

    enum PeerVerifyMode {
        VerifyNone,
        QueryPeer,
        VerifyPeer,
        AutoVerifyPeer
    };

    QSslSocket(QObject *parent = nullptr);
    ~QSslSocket();

    virtual void connectToHost(const QString &hostName, quint16 port, OpenMode mode = ReadWrite);
    virtual void disconnectFromHost();

    // Autostarting the SSL client handshake.
    void connectToHostEncrypted(const QString &hostName, quint16 port, OpenMode mode = ReadWrite);
    void connectToHostEncrypted(const QString &hostName, quint16 port, const QString &sslPeerName, OpenMode mode = ReadWrite);
    bool setSocketDescriptor(int socketDescriptor, SocketState state = ConnectedState,
                             OpenMode openMode = ReadWrite);

    void setSocketOption(QAbstractSocket::SocketOption option, const QVariant &value);
    QVariant socketOption(QAbstractSocket::SocketOption option);

    SslMode mode() const;
    bool isEncrypted() const;

    QSsl::SslProtocol protocol() const;
    void setProtocol(QSsl::SslProtocol protocol);

    QSslSocket::PeerVerifyMode peerVerifyMode() const;
    void setPeerVerifyMode(QSslSocket::PeerVerifyMode mode);

    int peerVerifyDepth() const;
    void setPeerVerifyDepth(int depth);

    QString peerVerifyName() const;
    void setPeerVerifyName(const QString &hostName);

    // From QIODevice
    qint64 bytesAvailable() const;
    qint64 bytesToWrite() const;
    bool canReadLine() const;
    void close();
    bool atEnd() const;

    bool flush();
    void abort();

    // From QAbstractSocket:
    void setReadBufferSize(qint64 size);

    // Similar to QIODevice's:
    qint64 encryptedBytesAvailable() const;
    qint64 encryptedBytesToWrite() const;

    // SSL configuration
    QSslConfiguration sslConfiguration() const;
    void setSslConfiguration(const QSslConfiguration &config);

    // Certificate & cipher accessors.
    void setLocalCertificate(const QSslCertificate &certificate);
    void setLocalCertificate(const QString &fileName, QSsl::EncodingFormat format = QSsl::Pem);
    QSslCertificate localCertificate() const;
    QSslCertificate peerCertificate() const;
    QList<QSslCertificate> peerCertificateChain() const;
    QSslCipher sessionCipher() const;

    // Private keys, for server sockets.
    void setPrivateKey(const QSslKey &key);
    void setPrivateKey(const QString &fileName, QSsl::KeyAlgorithm algorithm = QSsl::Rsa,
                       QSsl::EncodingFormat format = QSsl::Pem,
                       const QByteArray &passPhrase = QByteArray());
    QSslKey privateKey() const;

    // Cipher settings.
    QList<QSslCipher> ciphers() const;
    void setCiphers(const QList<QSslCipher> &ciphers);
    void setCiphers(const QString &ciphers);
    static void setDefaultCiphers(const QList<QSslCipher> &ciphers);
    static QList<QSslCipher> defaultCiphers();
    static QList<QSslCipher> supportedCiphers();

    // CA settings.
    bool addCaCertificates(const QString &path, QSsl::EncodingFormat format = QSsl::Pem,
                           QRegExp::PatternSyntax syntax = QRegExp::FixedString);
    void addCaCertificate(const QSslCertificate &certificate);
    void addCaCertificates(const QList<QSslCertificate> &certificates);
    void setCaCertificates(const QList<QSslCertificate> &certificates);
    QList<QSslCertificate> caCertificates() const;
    static bool addDefaultCaCertificates(const QString &path, QSsl::EncodingFormat format = QSsl::Pem,
                                         QRegExp::PatternSyntax syntax = QRegExp::FixedString);
    static void addDefaultCaCertificate(const QSslCertificate &certificate);
    static void addDefaultCaCertificates(const QList<QSslCertificate> &certificates);
    static void setDefaultCaCertificates(const QList<QSslCertificate> &certificates);
    static QList<QSslCertificate> defaultCaCertificates();
    static QList<QSslCertificate> systemCaCertificates();

    bool waitForConnected(int msecs = 30000);
    bool waitForEncrypted(int msecs = 30000);
    bool waitForReadyRead(int msecs = 30000);
    bool waitForBytesWritten(int msecs = 30000);
    bool waitForDisconnected(int msecs = 30000);

    QList<QSslError> sslErrors() const;

    static bool supportsSsl();
    void ignoreSslErrors(const QList<QSslError> &errors);

public Q_SLOTS:
    void startClientEncryption();
    void startServerEncryption();
    void ignoreSslErrors();

Q_SIGNALS:
    void encrypted();
    void peerVerifyError(const QSslError &error);
    void sslErrors(const QList<QSslError> &errors);
    void modeChanged(QSslSocket::SslMode newMode);
    void encryptedBytesWritten(qint64 totalBytes);

protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    Q_DECLARE_PRIVATE(QSslSocket)
    Q_DISABLE_COPY(QSslSocket)
    Q_PRIVATE_SLOT(d_func(), void _q_connectedSlot())
    Q_PRIVATE_SLOT(d_func(), void _q_hostFoundSlot())
    Q_PRIVATE_SLOT(d_func(), void _q_disconnectedSlot())
    Q_PRIVATE_SLOT(d_func(), void _q_stateChangedSlot(QAbstractSocket::SocketState))
    Q_PRIVATE_SLOT(d_func(), void _q_errorSlot(QAbstractSocket::SocketError))
    Q_PRIVATE_SLOT(d_func(), void _q_readyReadSlot())
    Q_PRIVATE_SLOT(d_func(), void _q_bytesWrittenSlot(qint64))
    Q_PRIVATE_SLOT(d_func(), void _q_flushWriteBuffer())
    Q_PRIVATE_SLOT(d_func(), void _q_flushReadBuffer())
    friend class QSslSocketBackendPrivate;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QList<QSslError>)


#endif
