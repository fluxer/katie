/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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

#include "qppmhandler_p.h"

#ifndef QT_NO_IMAGEFORMAT_PPM

#include "qimage.h"
#include "qvariant.h"
#include "qvector.h"
#include "qcorecommon_p.h"
#include "qguicommon_p.h"

#include <ctype.h>

QT_BEGIN_NAMESPACE

/*****************************************************************************
  PBM/PPM (ASCII and RAW) image read/write functions
 *****************************************************************************/

static int read_pbm_int(QIODevice *d)
{
    char c;
    int          val = -1;
    bool  digit;
    const int buflen = 100;
    QSTACKARRAY(char, buf, buflen);
    for (;;) {
        if (!d->getChar(&c))                // end of file
            break;
        digit = isdigit((uchar) c);
        if (val != -1) {
            if (digit) {
                val = 10*val + c - '0';
                continue;
            } else {
                if (c == '#')                        // comment
                    d->readLine(buf, buflen);
                break;
            }
        }
        if (digit)                                // first digit
            val = c - '0';
        else if (isspace((uchar) c))
            continue;
        else if (c == '#')
            (void)d->readLine(buf, buflen);
        else
            break;
    }
    return val;
}

static bool read_pbm_header(QIODevice *device, char& type, int& w, int& h, int& mcc)
{
    QSTACKARRAY(char, buf, 3);
    if (device->read(buf, 3) != 3)                        // read P[1-6]<white-space>
        return false;

    if (!(buf[0] == 'P' && isdigit((uchar) buf[1]) && isspace((uchar) buf[2])))
        return false;

    type = buf[1];
    if (type < '1' || type > '6')
        return false;

    w = read_pbm_int(device);                        // get image width
    h = read_pbm_int(device);                        // get image height

    if (type == '1' || type == '4')
        mcc = 1;                                  // ignore max color component
    else
        mcc = read_pbm_int(device);               // get max color component

    if (w <= 0 || w > 32767 || h <= 0 || h > 32767 || mcc <= 0)
        return false;                                        // weird P.M image

    return true;
}

static bool read_pbm_body(QIODevice *device, char type, int w, int h, int mcc, QImage *outImage)
{
    int nbits, y;
    int pbm_bpl;
    bool raw;

    QImage::Format format;
    switch (type) {
        case '1':                                // ascii PBM
        case '4':                                // raw PBM
            nbits = 1;
            format = QImage::Format_Mono;
            break;
        case '3':                                // ascii PPM
        case '6':                                // raw PPM
            nbits = 32;
            format = QImage::Format_RGB32;
            break;
        default:
            return false;
    }
    raw = type >= '4';

    int maxc = mcc;
    if (maxc > 255)
        maxc = 255;
    if (outImage->size() != QSize(w, h) || outImage->format() != format) {
        *outImage = QImage(w, h, format);
        if (outImage->isNull())
            return false;
    }

    pbm_bpl = (nbits*w+7)/8;                        // bytes per scanline in PBM

    if (raw) {                                // read raw data
        if (nbits == 32) {                        // type 6
            pbm_bpl = mcc < 256 ? 3*w : 6*w;
            QSTACKARRAY(uchar, buf24, pbm_bpl);
            uchar *b;
            QRgb  *p;
            QRgb  *end;
            for (y=0; y<h; y++) {
                if (device->read((char *)buf24, pbm_bpl) != pbm_bpl) {
                    return false;
                }
                p = (QRgb *)outImage->scanLine(y);
                end = p + w;
                b = buf24;
                while (p < end) {
                    if (mcc < 256) {
                        *p++ = qRgb(b[0],b[1],b[2]);
                        b += 3;
                    } else {
                        *p++ = qRgb(((int(b[0]) * 256 + int(b[1]) + 1) * 256) / (mcc + 1) - 1,
                                    ((int(b[2]) * 256 + int(b[3]) + 1) * 256) / (mcc + 1) - 1,
                                    ((int(b[4]) * 256 + int(b[5]) + 1) * 256) / (mcc + 1) - 1);
                        b += 6;
                    }
                }
            }
        } else {                                // type 4,5
            for (y=0; y<h; y++) {
                if (device->read((char *)outImage->scanLine(y), pbm_bpl) != pbm_bpl)
                    return false;
            }
        }
    } else {                                        // read ascii data
        uchar *p;
        int n;
        for (y=0; y<h; y++) {
            p = outImage->scanLine(y);
            n = pbm_bpl;
            if (nbits == 1) {
                int b;
                int bitsLeft = w;
                while (n--) {
                    b = 0;
                    for (int i=0; i<8; i++) {
                        if (i < bitsLeft)
                            b = (b << 1) | (read_pbm_int(device) & 1);
                        else
                            b = (b << 1) | (0 & 1); // pad it our self if we need to
                    }
                    bitsLeft -= 8;
                    *p++ = b;
                }
            } else {                                // 32 bits
                n /= 4;
                int r, g, b;
                if (mcc == maxc) {
                    while (n--) {
                        r = read_pbm_int(device);
                        g = read_pbm_int(device);
                        b = read_pbm_int(device);
                        *((QRgb*)p) = qRgb(r, g, b);
                        p += 4;
                    }
                } else {
                    while (n--) {
                        r = read_pbm_int(device) * maxc / mcc;
                        g = read_pbm_int(device) * maxc / mcc;
                        b = read_pbm_int(device) * maxc / mcc;
                        *((QRgb*)p) = qRgb(r, g, b);
                        p += 4;
                    }
                }
            }
        }
    }

    if (nbits == 1) {                                // black/white bitmap
        outImage->setColorTable(monoColorTable());
    }

    return true;
}

static bool write_pbm_image(QIODevice *out, const QImage &sourceImage, const QByteArray &sourceFormat)
{
    QImage image = sourceImage;
    const QByteArray format = sourceFormat.left(3); // ignore RAW part

    if (format == "pbm") {
        image = image.convertToFormat(QImage::Format_Mono);
    } else if (format == "ppm") {
        image = image.convertToFormat(QImage::Format_ARGB32);
    } else {
        return false;
    }

    if (image.depth() == 1) {
        if (qGray(image.color(0)) < qGray(image.color(1))) {
            // 0=dark/black, 1=light/white - invert
            for (int y=0; y<image.height(); y++) {
                uchar *p = image.scanLine(y);
                uchar *end = p + image.bytesPerLine();
                while (p < end)
                    *p++ ^= 0xff;
            }
        }
    }

    uint w = image.width();
    uint h = image.height();

    QByteArray str("P\n");
    str += QByteArray::number(w);
    str += ' ';
    str += QByteArray::number(h);
    str += '\n';

    switch (image.depth()) {
        case 1: {
            str.insert(1, '4');
            if (out->write(str, str.length()) != str.length())
                return false;
            w = (w+7)/8;
            for (uint y=0; y<h; y++) {
                uchar* line = image.scanLine(y);
                if (w != (uint)out->write((char*)line, w))
                    return false;
            }
            break;
        }

        case 32: {
            str.insert(1, '6');
            str.append("255\n");
            if (out->write(str, str.length()) != str.length())
                return false;
            uint bpl = w*3;
            QSTACKARRAY(uchar, buf, bpl);
            for (uint y=0; y<h; y++) {
                QRgb  *b = (QRgb*)image.scanLine(y);
                uchar *p = buf;
                uchar *end = buf+bpl;
                while (p < end) {
                    QRgb rgb = *b++;
                    *p++ = qRed(rgb);
                    *p++ = qGreen(rgb);
                    *p++ = qBlue(rgb);
                }
                if (bpl != (uint)out->write((char*)buf, bpl))
                    return false;
            }
            break;
        }

        default:
            return false;
    }

    return true;
}

QPpmHandler::QPpmHandler(const QByteArray &subtype)
    : state(Ready),
    subType(subtype)
{
}

bool QPpmHandler::readHeader()
{
    state = Error;
    if (!read_pbm_header(device(), type, width, height, mcc))
        return false;
    state = ReadHeader;
    return true;
}

bool QPpmHandler::canRead() const
{
    if (state == Ready && !canRead(device(), &subType))
        return false;

    if (state != Error) {
        setFormat(subType);
        return true;
    }

    return false;
}

bool QPpmHandler::read(QImage *image)
{
    if (state == Error)
        return false;

    if (state == Ready && !readHeader()) {
        state = Error;
        return false;
    }

    if (!read_pbm_body(device(), type, width, height, mcc, image)) {
        state = Error;
        return false;
    }

    state = Ready;
    return true;
}

bool QPpmHandler::write(const QImage &image)
{
    return write_pbm_image(device(), image, subType);
}

bool QPpmHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    return (option == QImageIOHandler::Size);
}

QVariant QPpmHandler::option(QImageIOHandler::ImageOption option) const
{
    if (option == QImageIOHandler::Size) {
        if (state == Error)
            return QVariant();
        if (state == Ready && !const_cast<QPpmHandler*>(this)->readHeader())
            return QVariant();
        return QSize(width, height);
    }
    return QVariant();
}

QByteArray QPpmHandler::name() const
{
    return subType.isEmpty() ? QByteArray("ppm") : subType;
}

bool QPpmHandler::canRead(QIODevice *device, QByteArray *subType)
{
    if (Q_UNLIKELY(!device)) {
        qWarning("QPpmHandler::canRead() called with no device");
        return false;
    }

    QSTACKARRAY(char, head, 2);
    if (device->peek(head, sizeof(head)) != sizeof(head)) {
        return false;
    }

    if (head[0] != 'P') {
        return false;
    }

    if (head[1] == '1' || head[1] == '4') {
        Q_ASSERT(subType);
        *subType = "pbm";
        return true;
    } else if (head[1] == '3' || head[1] == '6') {
        Q_ASSERT(subType);
        *subType = "ppm";
        return true;
    }
    return false;
}

QT_END_NAMESPACE

#endif // QT_NO_IMAGEFORMAT_PPM




