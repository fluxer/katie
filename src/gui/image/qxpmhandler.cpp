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

#include "qxpmhandler_p.h"

#ifndef QT_NO_IMAGEFORMAT_XPM

#include "qcolor_p.h"
#include "qimage.h"
#include "qmap.h"
#include "qtextstream.h"
#include "qvariant.h"
#include "qplatformdefs.h"
#include "qcorecommon_p.h"

QT_BEGIN_NAMESPACE

static quint64 xpmHash(const char *str)
{
    unsigned int hashValue = 0;
    while (*str != '\0') {
        hashValue <<= 8;
        hashValue += (unsigned int)*str;
        ++str;
    }
    return hashValue;
}

// Skip until ", read until the next ", return the rest in *buf
// Returns false on error, true on success
static bool read_xpm_string(QByteArray &buf, QIODevice *d, const char * const *source, int &index,
                            QByteArray &state)
{
    if (source) {
        buf = source[index++];
        return true;
    }

    buf = "";
    bool gotQuote = false;
    int offset = 0;
    QSTACKARRAY(char, readbuf, QT_BUFFSIZE);
    forever {
        if (offset == state.size() || state.isEmpty()) {
            qint64 bytesRead = d->read(readbuf, sizeof(readbuf));
            if (bytesRead <= 0)
                return false;
            state = QByteArray(readbuf, int(bytesRead));
            offset = 0;
        }

        if (!gotQuote) {
            if (state.at(offset++) == '"')
                gotQuote = true;
        } else {
            char c = state.at(offset++);
            if (c == '"')
                break;
            buf += c;
        }
    }
    state.remove(0, offset);
    return true;
}

// Tests if the given prefix can be the start of an XPM color specification
static bool is_xpm_color_spec_prefix(const QByteArray& prefix)
{
    return prefix == "c" ||
           prefix == "g" ||
           prefix == "g4" ||
           prefix == "m" ||
           prefix == "s";
}

// Reads XPM header.
static bool read_xpm_header(
    QIODevice *device, const char * const * source, int& index, QByteArray &state,
    int *cpp, int *ncols, int *w, int *h)
{
    QByteArray buf(200, 0);

    if (!read_xpm_string(buf, device, source, index, state))
        return false;

    if (sscanf(buf, "%d %d %d %d", w, h, ncols, cpp) < 4)
        return false;                                        // < 4 numbers parsed

    return true;
}

// Reads XPM body (color information & pixels).
static bool read_xpm_body(
    QIODevice *device, const char * const * source, int& index, QByteArray& state,
    int cpp, int ncols, int w, int h, QImage& image)
{
    if (cpp < 0 || cpp > 15)
        return false;

    QMap<quint64, QRgb> colorMap;
    bool hasTransparency = false;
    QByteArray buf(200, 0);

    for(int currentColor=0; currentColor < ncols; ++currentColor) {
        if (Q_UNLIKELY(!read_xpm_string(buf, device, source, index, state))) {
            qWarning("QImage: XPM color specification missing");
            return false;
        }
        QByteArray index = buf.left(cpp);
        buf = buf.mid(cpp).simplified().trimmed().toLower();
        QList<QByteArray> tokens = buf.split(' ');
        int i = tokens.indexOf("c");
        if (i < 0)
            i = tokens.indexOf("g");
        if (i < 0)
            i = tokens.indexOf("g4");
        if (i < 0)
            i = tokens.indexOf("m");
        if (Q_UNLIKELY(i < 0)) {
            qWarning("QImage: XPM color specification is missing: %s", buf.constData());
            return false;        // no c/g/g4/m specification at all
        }
        QByteArray color;
        while ((++i < tokens.size()) && !is_xpm_color_spec_prefix(tokens.at(i))) {
            color.append(tokens.at(i));
        }
        if (Q_UNLIKELY(color.isEmpty())) {
            qWarning("QImage: XPM color value is missing from specification: %s", buf.constData());
            return false;        // no color value
        }
        buf = color;
        if (buf == "none") {
            hasTransparency = true;
            colorMap.insert(xpmHash(index.constData()), 0);
        } else {
            QRgb c_rgb = 0;
            if (buf[0] == '#') {
                if ((buf.length()-1) % 3) {
                    buf.truncate(((buf.length()-1) / 4 * 3) + 1); // remove alpha channel left by imagemagick
                }
                qt_get_hex_rgb(buf, buf.length(), &c_rgb);
            } else {
                for (qint16 j = 0; j < X11RGBTblSize; j++) {
                    if (qstrcmp(X11RGBTbl[j].name, buf) == 0) {
                        c_rgb = X11RGBTbl[j].value;
                        break;
                    }
                }
            }
            colorMap.insert(xpmHash(index.constData()), 0xff000000 | c_rgb);
        }
    }

    // Now we can create 32-bit image of appropriate format
    image = QImage(w, h, hasTransparency ? QImage::Format_ARGB32 : QImage::Format_RGB32);
    if (Q_UNLIKELY(image.isNull())) {
        return false;
    }

    // Read pixels
    QSTACKARRAY(char, b, 16);
    for(int y=0; y<h; y++) {
        if (!read_xpm_string(buf, device, source, index, state)) {
            qWarning("QImage: XPM pixels missing on image line %d", y);
            return false;
        }
        QRgb *p = (QRgb*)image.scanLine(y);
        uchar *d = (uchar *)buf.data();
        uchar *end = d + buf.length();
        int x;
        for (x = 0; x < w && d < end; x++) {
            ::memcpy(b, (char *)d, cpp);
            *p++ = (QRgb)colorMap[xpmHash(b)];
            d += cpp;
        }
        // avoid uninitialized memory for malformed xpms
        if (Q_UNLIKELY(x < w)) {
            qWarning("QImage: XPM pixels missing on image line %d (possibly a C++ trigraph).", y);
            ::memset(p, 0, (w - x)*4);
        }
    }

    if (device) {
        // Rewind unused characters, and skip to the end of the XPM struct.
        for (int i = state.size() - 1; i >= 0; --i)
            device->ungetChar(state[i]);
        char c;
        while (device->getChar(&c) && c != ';') {}
        while (device->getChar(&c) && c != '\n') {}
    }
    return true;
}

//
// INTERNAL
//
// Reads an .xpm from characters array
//
bool qt_read_xpm_array(const char * const * source, QImage &image)
{
    if (!source)
        return true;

    QByteArray state;

    int cpp, ncols, w, h, index = 0;

    if (!read_xpm_header(nullptr, source, index, state, &cpp, &ncols, &w, &h))
        return false;

    return read_xpm_body(nullptr, source, index, state, cpp, ncols, w, h, image);
}

QXpmHandler::QXpmHandler()
    : state(Ready), index(0)
{
}

bool QXpmHandler::readHeader()
{
    state = Error;
    if (!read_xpm_header(device(), nullptr, index, buffer, &cpp, &ncols, &width, &height))
        return false;
    state = ReadHeader;
    return true;
}

bool QXpmHandler::canRead() const
{
    if (state == Ready && !canRead(device()))
        return false;

    if (state != Error) {
        setFormat("xpm");
        return true;
    }

    return false;
}

bool QXpmHandler::read(QImage *image)
{
    if (!canRead())
        return false;

    if (state == Error)
        return false;

    if (state == Ready && !readHeader()) {
        state = Error;
        return false;
    }

    if (!read_xpm_body(device(), nullptr, index, buffer, cpp, ncols, width, height, *image)) {
        state = Error;
        return false;
    }

    state = Ready;
    return true;
}

bool QXpmHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    return (option == QImageIOHandler::Size);
}

QVariant QXpmHandler::option(QImageIOHandler::ImageOption option) const
{
    if (option == QImageIOHandler::Size) {
        if (state == Error)
            return QVariant();
        if (state == Ready && !const_cast<QXpmHandler*>(this)->readHeader())
            return QVariant();
        return QSize(width, height);
    }
    return QVariant();
}

QByteArray QXpmHandler::name() const
{
    return "xpm";
}

bool QXpmHandler::canRead(QIODevice *device)
{
    if (Q_UNLIKELY(!device)) {
        qWarning("QXpmHandler::canRead() called with no device");
        return false;
    }

    QSTACKARRAY(char, head, 6);
    if (device->peek(head, sizeof(head)) != sizeof(head))
        return false;

    return (::memcmp(head, "/* XPM", 6) == 0);
}

QT_END_NAMESPACE

#endif // QT_NO_IMAGEFORMAT_XPM
