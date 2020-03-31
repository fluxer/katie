/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qmemrotate_p.h"

QT_BEGIN_NAMESPACE

template <class DST, class SRC>
static inline void qt_memrotate90_template(const SRC *src,
                                           int srcWidth, int srcHeight, int srcStride,
                                           DST *dest, int dstStride)
{
#if QT_ROTATION_ALGORITHM == QT_ROTATION_CACHEDREAD
    const char *s = reinterpret_cast<const char*>(src);
    char *d = reinterpret_cast<char*>(dest);
    for (int y = 0; y < srcHeight; ++y) {
        for (int x = srcWidth - 1; x >= 0; --x) {
            DST *destline = reinterpret_cast<DST*>(d + (srcWidth - x - 1) * dstStride);
            destline[y] = src[x];
        }
        s += srcStride;
        src = reinterpret_cast<const SRC*>(s);
    }
#elif QT_ROTATION_ALGORITHM == QT_ROTATION_CACHEDWRITE
    for (int x = srcWidth - 1; x >= 0; --x) {
        DST *d = dest + (srcWidth - x - 1) * dstStride;
        for (int y = 0; y < srcHeight; ++y) {
            *d++ = src[y * srcStride + x];
        }
    }
#endif
}

template <class DST, class SRC>
static inline void qt_memrotate180_template(const SRC *src,
                                            int w, int h, int sstride,
                                            DST *dest, int dstride)
{
    const char *s = (const char*)(src) + (h - 1) * sstride;
    for (int y = h - 1; y >= 0; --y) {
        DST *d = reinterpret_cast<DST*>((char *)(dest) + (h - y - 1) * dstride);
        src = reinterpret_cast<const SRC*>(s);
        for (int x = w - 1; x >= 0; --x) {
            d[w - x - 1] = src[x];
        }
        s -= sstride;
    }
}

template <class DST, class SRC>
static inline void qt_memrotate270_template(const SRC *src,
                                            int srcWidth, int srcHeight, int srcStride,
                                            DST *dest, int dstStride)
{
#if QT_ROTATION_ALGORITHM == QT_ROTATION_CACHEDREAD
    const char *s = reinterpret_cast<const char*>(src);
    char *d = reinterpret_cast<char*>(dest);
    s += (srcHeight - 1) * srcStride;
    for (int y = srcHeight - 1; y >= 0; --y) {
        src = reinterpret_cast<const SRC*>(s);
        for (int x = 0; x < srcWidth; ++x) {
            DST *destline = reinterpret_cast<DST*>(d + x * dstStride);
            destline[srcHeight - y - 1] = src[x];
        }
        s -= srcStride;
    }
#elif QT_ROTATION_ALGORITHM == QT_ROTATION_CACHEDWRITE
    for (int x = 0; x < srcWidth; ++x) {
        DST *d = dest + x * dstStride;
        for (int y = srcHeight - 1; y >= 0; --y) {
            *d++ = src[y * srcStride + x];
        }
    }
#endif
}

#define QT_IMPL_MEMROTATE(srctype, desttype)                        \
void qt_memrotate90(const srctype *src, int w, int h, int sstride,  \
                    desttype *dest, int dstride)                    \
{                                                                   \
    qt_memrotate90_template(src, w, h, sstride, dest, dstride);     \
}                                                                   \
void qt_memrotate180(const srctype *src, int w, int h, int sstride, \
                     desttype *dest, int dstride)                   \
{                                                                   \
    qt_memrotate180_template(src, w, h, sstride, dest, dstride);    \
}                                                                   \
void qt_memrotate270(const srctype *src, int w, int h, int sstride, \
                     desttype *dest, int dstride)                   \
{                                                                   \
    qt_memrotate270_template(src, w, h, sstride, dest, dstride);    \
}

QT_IMPL_MEMROTATE(quint8, quint8)
QT_IMPL_MEMROTATE(quint16, quint16)
QT_IMPL_MEMROTATE(quint24, quint24)
QT_IMPL_MEMROTATE(quint32, quint32)

#undef QT_IMPL_MEMROTATE

QT_END_NAMESPACE




