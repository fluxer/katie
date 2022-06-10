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
#include "qbuffer.h"
#include "qdebug.h"
#include "qcorecommon_p.h"

#include <libdeflate.h>
#include <limits.h>

QT_BEGIN_NAMESPACE

QKatHandler::QKatHandler()
    : m_complevel(1),
    m_quality(75)
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

    return (qstrncmp(head, "KATIE", 5) == 0);
}

bool QKatHandler::read(QImage *image)
{
    QDataStream imagestream(device());

    QSTACKARRAY(char, header, 5);
    imagestream.readRawData(header, 5);
    if (Q_UNLIKELY(qstrncmp(header, "KATIE", 5) != 0)) {
        qWarning("QKatHandler::read() Invalid header (%s)", header);
        *image = QImage();
        return false;
    }

    qint8 format = 0;
    qint64 width = 0;
    qint64 height = 0;
    qint64 uncompressedsize = 0;
    imagestream >> format;
    imagestream >> width;
    imagestream >> height;
    imagestream >> uncompressedsize;

    *image = QImage(width, height, static_cast<QImage::Format>(format));
    if (Q_UNLIKELY(image->isNull())) {
        qWarning("QKatHandler::read() Could not create image");
        return false;
    }

    QByteArray imagedata;
    imagestream >> imagedata;

    if (Q_UNLIKELY(imagestream.status() != QDataStream::Ok)) {
        qWarning("QKatHandler::read() Could not read image");
        *image = QImage();
        return false;
    }

    struct libdeflate_decompressor* decomp = libdeflate_alloc_decompressor();
    if (Q_UNLIKELY(!decomp)) {
        qWarning("QKatHandler::read(): Could not allocate decompressor");
        *image = QImage();
        return false;
    }
    size_t decompsize = uncompressedsize;
    const libdeflate_result decompresult = libdeflate_zlib_decompress(
        decomp,
        imagedata.constData(), imagedata.size(),
        image->d->data, image->byteCount(),
        &decompsize
    );
    libdeflate_free_decompressor(decomp);

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
    } else if (Q_UNLIKELY(image.height() >= INT_MAX || image.width() >= INT_MAX)) {
        // the limit is actually lower but QImage::width() and QImage::height() return int and
        // QImage can be fixed at some point, notably QImage::byteCount() return type change
        qWarning("QKatHandler::write() Limitation in Katie");
        return false;
    }

    QImage::Format imageformat = QImage::Format_ARGB32;
    if (Q_LIKELY(m_quality <= 75)) {
        imageformat = (image.d->checkForAlphaPixels() ? QImage::Format_ARGB32 : QImage::Format_RGB16);
    }
    const QImage image32 = image.convertToFormat(imageformat);

    struct libdeflate_compressor* comp = libdeflate_alloc_compressor(m_complevel);
    if (Q_UNLIKELY(!comp)) {
        qWarning("QKatHandler::write() Could not allocate compressor");
        return false;
    }
    const size_t boundresult = libdeflate_zlib_compress_bound(comp, image32.byteCount());
    if (Q_UNLIKELY(boundresult <= 0)) {
        qWarning("QKatHandler::write() Compression boundary is negative or zero");
        libdeflate_free_compressor(comp);
        return false;
    }
    QByteArray compressedimage(boundresult, Qt::Uninitialized);
    const size_t compresult = libdeflate_zlib_compress(
        comp,
        reinterpret_cast<const char*>(image32.constBits()), image32.byteCount(),
        compressedimage.data(), compressedimage.size()
    );
    libdeflate_free_compressor(comp);
    if (Q_UNLIKELY(compresult <= 0)) {
        qWarning("QKatHandler::write() Could not compress data");
        return false;
    }

    QBuffer imagebuffer;
    imagebuffer.open(QBuffer::ReadWrite);
    QDataStream imagestream(&imagebuffer);
    imagestream.writeRawData("KATIE", 5);
    imagestream << (qint8) image32.format();
    imagestream << (qint64) image32.width();
    imagestream << (qint64) image32.height();
    imagestream << (qint64) image32.byteCount();
    imagestream << (QByteArray) QByteArray::fromRawData(compressedimage.constData(), compresult);

    if (Q_UNLIKELY(device()->write(imagebuffer.data(), imagebuffer.size()) != imagebuffer.size())) {
        qWarning("QKatHandler::write() Could not write image");
        return false;
    }

    return true;
}

QVariant QKatHandler::option(QImageIOHandler::ImageOption option) const
{
    if (option == QImageIOHandler::Size) {
        QDataStream imagestream(device());

        QSTACKARRAY(char, header, 5);
        imagestream.readRawData(header, 5);
        if (Q_UNLIKELY(qstrncmp(header, "KATIE", 5) != 0)) {
            qWarning("QKatHandler::option() Invalid header (%s)", header);
            return QVariant();
        }

        qint8 format = 0;
        qint64 width = 0;
        qint64 height = 0;
        imagestream >> format;
        imagestream >> width;
        imagestream >> height;
        return QSize(width, height);
    } else if (option == QImageIOHandler::CompressionLevel) {
        return QVariant(m_complevel);
    } else if (option == QImageIOHandler::Quality) {
        return QVariant(m_quality);
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
    } else if (option == QImageIOHandler::Quality) {
        const int newquality = value.toInt();
        if (newquality == -1) {
            m_quality = 75;
        } else if (Q_UNLIKELY(newquality < 0 || newquality > 100)) {
            qWarning("QKatHandler::setOption() Invalid quality (%d)", newquality);
            m_quality = 75;
        } else {
            m_quality = newquality;
        }
    }
}

bool QKatHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    switch (option) {
        case QImageIOHandler::Size:
        case QImageIOHandler::CompressionLevel:
        case QImageIOHandler::Quality: {
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
