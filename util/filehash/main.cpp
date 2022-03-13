/****************************************************************************
**
** Copyright (C) 2022 Ivailo Monev
**
** This file is part of the utils of the Katie Toolkit.
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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qfile.h>
#include <QtNetwork/qcryptographichash.h>

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const QStringList args = app.arguments().mid(1);
    if (args.size() != 2) {
        qWarning() << "Usage: filehash <md5|sha1|sha256|sha512|kat> <filepath>";
        return 1;
    }

    QCryptographicHash::Algorithm algorithm = QCryptographicHash::Md5;
    if (args.at(0) == "md5") {
        algorithm = QCryptographicHash::Md5;
    } else if (args.at(0) == "sha1") {
        algorithm = QCryptographicHash::Sha1;
    } else if (args.at(0) == "sha256") {
        algorithm = QCryptographicHash::Sha256;
    } else if (args.at(0) == "sha512") {
        algorithm = QCryptographicHash::Sha512;
    } else if (args.at(0) == "kat") {
        algorithm = QCryptographicHash::KAT;
    } else {
        qWarning() << "Invalid algorithm" << args.at(0);
        return 2;
    }

    QFile file(args.at(1));
    if (file.open(QFile::ReadOnly) == false) {
        qWarning() << "Could not open" << args.at(1);
        return 3;
    }

    QCryptographicHash cryptohash(algorithm);
    cryptohash.addData(&file);
    const QByteArray hash = cryptohash.result().toHex();
    if (hash.isEmpty()) {
        qWarning() << "Could not hash" << args.at(1);
        return 4;
    }

    qDebug() << hash;

    return 0;
}

