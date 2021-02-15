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


#ifndef QSSLCERTIFICATE_H
#define QSSLCERTIFICATE_H

#include <QtCore/qnamespace.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qregexp.h>
#include <QtCore/qsharedpointer.h>
#include <QtNetwork/qssl.h>
#include <QtNetwork/qcryptographichash.h>


QT_BEGIN_NAMESPACE

class QDateTime;
class QIODevice;
class QSslKey;
class QStringList;
template <typename T, typename U> class QMultiMap;

class QSslCertificatePrivate;
class Q_NETWORK_EXPORT QSslCertificate
{
public:
    enum SubjectInfo {
        Organization,
        CommonName,
        LocalityName,
        OrganizationalUnitName,
        CountryName,
        StateOrProvinceName
    };

    QSslCertificate(QIODevice *device, QSsl::EncodingFormat format = QSsl::Pem);
    QSslCertificate( // ### s/encoded/data (to be consistent with signature in .cpp file) ?
        const QByteArray &encoded = QByteArray(), QSsl::EncodingFormat format = QSsl::Pem);
    QSslCertificate(const QSslCertificate &other);
    ~QSslCertificate();
    QSslCertificate &operator=(const QSslCertificate &other);
    bool operator==(const QSslCertificate &other) const;
    inline bool operator!=(const QSslCertificate &other) const { return !operator==(other); }

    bool isNull() const;
    bool isValid() const;
    void clear();

    // Certificate info
    QByteArray version() const;
    QByteArray serialNumber() const;
    QByteArray digest(QCryptographicHash::Algorithm algorithm = QCryptographicHash::Md5) const;
    QString issuerInfo(SubjectInfo info) const;
    QString issuerInfo(const QByteArray &tag) const;
    QString subjectInfo(SubjectInfo info) const;
    QString subjectInfo(const QByteArray &tag) const;
    QMultiMap<QSsl::AlternateNameEntryType, QString> alternateSubjectNames() const;
    QDateTime effectiveDate() const;
    QDateTime expiryDate() const;
    QSslKey publicKey() const;

    QByteArray toPem() const;
    QByteArray toDer() const;

    static QList<QSslCertificate> fromPath(
        const QString &path, QSsl::EncodingFormat format = QSsl::Pem,
        QRegExp::PatternSyntax syntax = QRegExp::FixedString);
    static QList<QSslCertificate> fromDevice(
        QIODevice *device, QSsl::EncodingFormat format = QSsl::Pem);
    static QList<QSslCertificate> fromData(
        const QByteArray &data, QSsl::EncodingFormat format = QSsl::Pem);

    Qt::HANDLE handle() const;

private:
    QExplicitlySharedDataPointer<QSslCertificatePrivate> d;
    friend class QSslCertificatePrivate;
    friend class QSslSocketBackendPrivate;
};

#ifndef QT_NO_DEBUG_STREAM
class QDebug;
Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, const QSslCertificate &certificate);
Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, QSslCertificate::SubjectInfo info);
#endif

QT_END_NAMESPACE


#endif
