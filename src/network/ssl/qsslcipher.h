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


#ifndef QSSLCIPHER_H
#define QSSLCIPHER_H

#include <QtCore/qstring.h>
#include <QtCore/qscopedpointer.h>
#include <QtNetwork/qssl.h>


QT_BEGIN_NAMESPACE

class QSslCipherPrivate;
class Q_NETWORK_EXPORT QSslCipher
{
public:
    QSslCipher();
    QSslCipher(const QString &name, QSsl::SslProtocol protocol);
    QSslCipher(const QSslCipher &other);
    ~QSslCipher();
    QSslCipher &operator=(const QSslCipher &other);
    bool operator==(const QSslCipher &other) const;
    inline bool operator!=(const QSslCipher &other) const { return !operator==(other); }

    bool isNull() const;
    QString name() const;
    int supportedBits() const;
    int usedBits() const;

    QString keyExchangeMethod() const;
    QString authenticationMethod() const;
    QString encryptionMethod() const;
    QString protocolString() const;
    QSsl::SslProtocol protocol() const;

private:
    QScopedPointer<QSslCipherPrivate> d;
    friend class QSslSocketBackendPrivate;
};

#ifndef QT_NO_DEBUG_STREAM
class QDebug;
Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, const QSslCipher &cipher);
#endif

QT_END_NAMESPACE


#endif

