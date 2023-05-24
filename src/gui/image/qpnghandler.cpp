/****************************************************************************
**
** Copyright (C) 2023 Ivailo Monev
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

#include "qpnghandler_p.h"
#include "qiodevice.h"
#include "qvariant.h"
#include "qimage.h"
#include "qcorecommon_p.h"
#include "qdebug.h"

#include "spng/spng.h"

// for reference:
// https://www.w3.org/TR/PNG-Chunks.html

QT_BEGIN_NAMESPACE

int spng_read_proc(spng_ctx *ctx, void *user, void *dst_src, size_t length)
{
    // qDebug() << Q_FUNC_INFO << length;
    Q_UNUSED(ctx);
    QIODevice* iodevice = static_cast<QIODevice*>(user);
    char* dst = static_cast<char*>(dst_src);
    const qint64 result = iodevice->read(dst, length);
    if (result <= 0) {
        return SPNG_IO_ERROR;
    }
    return SPNG_OK;
}

int spng_write_proc(spng_ctx *ctx, void *user, void *dst_src, size_t length)
{
    // qDebug() << Q_FUNC_INFO << length;
    Q_UNUSED(ctx);
    QIODevice* iodevice = static_cast<QIODevice*>(user);
    const char* src = static_cast<char*>(dst_src);
    const qint64 result = iodevice->write(src, length);
    if (result <= 0) {
        return SPNG_IO_ERROR;
    }
    return SPNG_OK;
}


QPngHandler::QPngHandler()
    : m_compression(1)
{
}

QPngHandler::~QPngHandler()
{
}

bool QPngHandler::canRead() const
{
    if (QPngHandler::canRead(device())) {
        setFormat("png");
        return true;
    }

    return false;
}

bool QPngHandler::read(QImage *image)
{
    if (!canRead()) {
        return false;
    }

    spng_ctx *spngctx = spng_ctx_new(static_cast<spng_ctx_flags>(0));
    if (Q_UNLIKELY(!spngctx)) {
        qWarning("QPngHandler::read() Could not create context");
        return false;
    }

    int spngresult = spng_set_png_stream(spngctx, spng_read_proc, device());
    if (Q_UNLIKELY(spngresult != SPNG_OK)) {
        qWarning("QPngHandler::read() Could not set stream: %s", spng_strerror(spngresult));
        spng_ctx_free(spngctx);
        return false;
    }

    struct spng_ihdr spngihdr;
    ::memset(&spngihdr, 0, sizeof(struct spng_ihdr));
    spngresult = spng_get_ihdr(spngctx, &spngihdr);
    if (Q_UNLIKELY(spngresult != SPNG_OK)) {
        qWarning("QPngHandler::read() Could not get IHDR: %s", spng_strerror(spngresult));
        spng_ctx_free(spngctx);
        return false;
    }

    *image = QImage(spngihdr.width, spngihdr.height, QImage::Format_ARGB32);
    if (image->isNull()) {
        qWarning("QPngHandler::read() Could not create image");
        spng_ctx_free(spngctx);
        return false;
    }

    spngresult = spng_decode_image(
        spngctx,
        image->bits(), image->byteCount(),
        SPNG_FMT_RGBA8,
        SPNG_DECODE_TRNS
    );
    if (Q_UNLIKELY(spngresult != SPNG_OK)) {
        qWarning("QPngHandler::read() Could not decode image: %s", spng_strerror(spngresult));
        spng_ctx_free(spngctx);
        return false;
    }

    *image = image->rgbSwapped();
    spng_ctx_free(spngctx);
    return true;
}

bool QPngHandler::write(const QImage &image)
{
    QImage copy = image.convertToFormat(QImage::Format_ARGB32);
    copy = copy.rgbSwapped();

    spng_ctx *spngctx = spng_ctx_new(SPNG_CTX_ENCODER);
    if (Q_UNLIKELY(!spngctx)) {
        qWarning("QPngHandler::write() Could not create context");
        return false;
    }

    int spngresult = spng_set_png_stream(spngctx, spng_write_proc, device());
    if (Q_UNLIKELY(spngresult != SPNG_OK)) {
        qWarning("QPngHandler::write() Could not set stream: %s", spng_strerror(spngresult));
        spng_ctx_free(spngctx);
        return false;
    }

    struct spng_ihdr spngihdr; 
    ::memset(&spngihdr, 0, sizeof(struct spng_ihdr));
    spngihdr.width = copy.width();
    spngihdr.height = copy.height();
    spngihdr.bit_depth = 8;
    spngihdr.color_type = SPNG_COLOR_TYPE_TRUECOLOR_ALPHA;
    spngihdr.compression_method = 0; // no enum for it
    spngihdr.filter_method = SPNG_FILTER_NONE;
    spngihdr.interlace_method = SPNG_INTERLACE_NONE;
    spngresult = spng_set_ihdr(spngctx, &spngihdr);
    if (Q_UNLIKELY(spngresult != SPNG_OK)) {
        qWarning("QPngHandler::write() Could not set IHDR: %s", spng_strerror(spngresult));
        spng_ctx_free(spngctx);
        return false;
    }

    spngresult = spng_set_option(spngctx, SPNG_IMG_COMPRESSION_LEVEL, m_compression);
    if (Q_UNLIKELY(spngresult != SPNG_OK)) {
        qWarning("QPngHandler::write() Could not set image compression level: %s", spng_strerror(spngresult));
        spng_ctx_free(spngctx);
        return false;
    }

    spngresult = spng_encode_image(
        spngctx,
        copy.constBits(), copy.byteCount(),
        SPNG_FMT_PNG,
        SPNG_ENCODE_FINALIZE
    );
    if (Q_UNLIKELY(spngresult != SPNG_OK)) {
        qWarning("QPngHandler::write() Could not encode image: %s", spng_strerror(spngresult));
        spng_ctx_free(spngctx);
        return false;
    }

    spng_ctx_free(spngctx);
    return true;
}

bool QPngHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    return (option == QImageIOHandler::CompressionLevel);
}

void QPngHandler::setOption(QImageIOHandler::ImageOption option, const QVariant &value)
{
    if (option == QImageIOHandler::CompressionLevel) {
        const int newlevel = value.toInt();
        if (Q_UNLIKELY(newlevel < 0 || newlevel > 9)) {
            qWarning("QPngHandler::setOption() invalid compression level value");
            m_compression = 1;
        } else {
            m_compression = newlevel;
        }
    }
}

QByteArray QPngHandler::name() const
{
    return "png";
}

bool QPngHandler::canRead(QIODevice *device)
{
    if (Q_UNLIKELY(!device)) {
        qWarning("QPngHandler::canRead() called with no device");
        return false;
    }

    QSTACKARRAY(char, head, 8);
    if (device->peek(head, sizeof(head)) != sizeof(head)) {
        return false;
    }

    static const uchar pngheader[]
        = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };
    return (::memcmp(head, pngheader, 8) == 0);
}

QT_END_NAMESPACE
