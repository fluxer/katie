/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#ifndef QBITMAP_H
#define QBITMAP_H

#include <QtGui/qpixmap.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QVariant;

class Q_GUI_EXPORT QBitmap : public QPixmap
{
public:
    QBitmap();
    QBitmap(const QPixmap &);
    QBitmap(int w, int h);
    explicit QBitmap(const QSize &);
    explicit QBitmap(const QString &fileName, const char *format=0);
    ~QBitmap();

    QBitmap &operator=(const QPixmap &);
    inline void swap(QBitmap &other) { QPixmap::swap(other); } // prevent QBitmap<->QPixmap swaps
    operator QVariant() const;

    inline void clear() { fill(Qt::color0); }

    static QBitmap fromImage(const QImage &image, Qt::ImageConversionFlags flags = Qt::AutoColor);
    static QBitmap fromData(const QSize &size, const uchar *bits,
                            QImage::Format monoFormat = QImage::Format_MonoLSB);

    typedef QExplicitlySharedDataPointer<QPixmapData> DataPtr;
};
Q_DECLARE_SHARED(QBitmap)


QT_END_NAMESPACE

QT_END_HEADER

#endif // QBITMAP_H
