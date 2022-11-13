/****************************************************************************
**
** Copyright (C) 2022 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#include "qkathandler_p.h"

#ifndef QT_NO_IMAGEFORMAT_KAT

#include "qimage.h"
#include "qimage_p.h"
#include "qvariant.h"
#include "qdebug.h"
#include "qcorecommon_p.h"

#include <libdeflate.h>
#include <limits.h>
#include <stdlib.h>

QT_BEGIN_NAMESPACE

QKatHandler::QKatHandler()
    : m_complevel(1)
{
}

bool QKatHandler::canRead() const
{
    if (QKatHandler::canRead(device())) {
        setFormat("kat");
        return true;
    }

    return false;
}

bool QKatHandler::canRead(QIODevice *device)
{
    if (Q_UNLIKELY(!device)) {
        qWarning("QKatHandler::canRead() Called with no device");
        return false;
    }

    QSTACKARRAY(char, head, 5);
    if (device->peek(head, sizeof(head)) != sizeof(head))
        return false;

    return (::memcmp(head, "KATIE", 5) == 0);
}

bool QKatHandler::read(QImage *image)
{
    QDataStream imagestream(device());
    imagestream.setByteOrder(QDataStream::BigEndian);

    QSTACKARRAY(char, header, 5);
    imagestream.readRawData(header, 5);
    if (Q_UNLIKELY(::memcmp(header, "KATIE", 5) != 0)) {
        qWarning("QKatHandler::read() Invalid header (%s)", header);
        *image = QImage();
        return false;
    }

    qint8 format = 0;
    quint16 width = 0;
    quint16 height = 0;
    quint32 uncompressedsize = 0;
    quint32 compressedsize = 0;
    imagestream >> format;
    imagestream >> width;
    imagestream >> height;
    imagestream >> uncompressedsize;
    imagestream >> compressedsize;

    *image = QImage(width, height, static_cast<QImage::Format>(format));
    if (Q_UNLIKELY(image->isNull())) {
        qWarning("QKatHandler::read() Could not create image");
        return false;
    }

    char* compressedimage = static_cast<char*>(::malloc(compressedsize * sizeof(char)));
    Q_CHECK_PTR(compressedimage);
    imagestream.readRawData(compressedimage, compressedsize);

    if (Q_UNLIKELY(imagestream.status() != QDataStream::Ok)) {
        qWarning("QKatHandler::read() Could not read image");
        *image = QImage();
        ::free(compressedimage);
        return false;
    }

    struct libdeflate_decompressor* decomp = libdeflate_alloc_decompressor();
    if (Q_UNLIKELY(!decomp)) {
        qWarning("QKatHandler::read(): Could not allocate decompressor");
        *image = QImage();
        ::free(compressedimage);
        return false;
    }
    size_t decompsize = uncompressedsize;
    const libdeflate_result decompresult = libdeflate_zlib_decompress(
        decomp,
        compressedimage, compressedsize,
        image->d->data, image->byteCount(),
        &decompsize
    );
    libdeflate_free_decompressor(decomp);
    ::free(compressedimage);

    if (Q_UNLIKELY(decompresult != LIBDEFLATE_SUCCESS)) {
        qWarning("QKatHandler::read() Could not decompress image (%d)", decompresult);
        *image = QImage();
        return false;
    }

    return true;
}

bool QKatHandler::write(const QImage &image)
{
    if (image.isNull()) {
        return false;
    } else if (Q_UNLIKELY(image.height() >= USHRT_MAX || image.width() >= USHRT_MAX)) {
        qWarning("QKatHandler::write() Limitation in Katie");
        return false;
    }

    struct libdeflate_compressor* comp = libdeflate_alloc_compressor(m_complevel);
    if (Q_UNLIKELY(!comp)) {
        qWarning("QKatHandler::write() Could not allocate compressor");
        return false;
    }
    const size_t boundresult = libdeflate_zlib_compress_bound(comp, image.byteCount());
    if (Q_UNLIKELY(boundresult <= 0)) {
        qWarning("QKatHandler::write() Compression boundary is negative or zero");
        libdeflate_free_compressor(comp);
        return false;
    }
    char* compressedimage = static_cast<char*>(::malloc(boundresult * sizeof(char)));
    Q_CHECK_PTR(compressedimage);
    const size_t compresult = libdeflate_zlib_compress(
        comp,
        reinterpret_cast<const char*>(image.constBits()), image.byteCount(),
        compressedimage, boundresult
    );
    libdeflate_free_compressor(comp);
    if (Q_UNLIKELY(compresult <= 0)) {
        qWarning("QKatHandler::write() Could not compress data");
        ::free(compressedimage);
        return false;
    }

    QDataStream imagestream(device());
    imagestream.setByteOrder(QDataStream::BigEndian);
    imagestream.writeRawData("KATIE", 5);
    imagestream << (qint8) image.format();
    imagestream << (quint16) image.width();
    imagestream << (quint16) image.height();
    imagestream << (quint32) image.byteCount();
    imagestream << (quint32) compresult;
    imagestream.writeRawData(compressedimage, compresult);
    ::free(compressedimage);

    if (Q_UNLIKELY(imagestream.status() != QDataStream::Ok)) {
        qWarning("QKatHandler::write() Could not write image");
        return false;
    }

    return true;
}

QVariant QKatHandler::option(QImageIOHandler::ImageOption option) const
{
    if (option == QImageIOHandler::Size) {
        const qint64 devicepos = device()->pos();
        QDataStream imagestream(device());
        imagestream.setByteOrder(QDataStream::BigEndian);

        QSTACKARRAY(char, header, 5);
        imagestream.readRawData(header, 5);
        if (Q_UNLIKELY(::memcmp(header, "KATIE", 5) != 0)) {
            qWarning("QKatHandler::option() Invalid header (%s)", header);
            device()->seek(devicepos);
            return QVariant();
        }

        qint8 format = 0;
        quint16 width = 0;
        quint16 height = 0;
        imagestream >> format;
        imagestream >> width;
        imagestream >> height;
        device()->seek(devicepos);
        return QSize(width, height);
    } else if (option == QImageIOHandler::CompressionLevel) {
        return QVariant(m_complevel);
    }
    return QVariant();
}

void QKatHandler::setOption(QImageIOHandler::ImageOption option, const QVariant &value)
{
    if (option == QImageIOHandler::CompressionLevel) {
        const int newcomplevel = value.toInt();
        if (Q_UNLIKELY(newcomplevel < 0 || newcomplevel > 9)) {
            qWarning("QKatHandler::setOption() Invalid compression level (%d)", newcomplevel);
            m_complevel = 1;
        } else {
            m_complevel = newcomplevel;
        }
    }
}

bool QKatHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    switch (option) {
        case QImageIOHandler::Size:
        case QImageIOHandler::CompressionLevel: {
            return true;
        }
        default: {
            return false;
        }
    }
    Q_UNREACHABLE();
    return false;
}

QByteArray QKatHandler::name() const
{
    return "kat";
}

QT_END_NAMESPACE

#endif // QT_NO_IMAGEFORMAT_KAT
