/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QCRYPTOGRAPHICSHASH_H
#define QCRYPTOGRAPHICSHASH_H

#include <QtCore/qbytearray.h>


QT_BEGIN_NAMESPACE


class QCryptographicHashPrivate;
class QIODevice;

class Q_NETWORK_EXPORT QCryptographicHash
{
public:
    enum Algorithm {
        Md4,
        Md5,
        Sha1,
        Sha256,
        Sha512
    };

    explicit QCryptographicHash(Algorithm method);
    ~QCryptographicHash();

    void reset();

    void addData(const char *data, int length);
    inline void addData(const QByteArray &data)
        { addData(data.constData(), data.length()); }
    bool addData(QIODevice* device);

    QByteArray result() const;

    static QByteArray hash(const QByteArray &data, Algorithm method);
private:
    Q_DISABLE_COPY(QCryptographicHash)
    QCryptographicHashPrivate *d;
};

QT_END_NAMESPACE


#endif // QCRYPTOGRAPHICSHASH_H
