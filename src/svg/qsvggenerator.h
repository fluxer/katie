/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtSvg module of the Katie Toolkit.
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

#ifndef QSVGGENERATOR_H
#define QSVGGENERATOR_H

#include <QtGui/qpaintdevice.h>

#ifndef QT_NO_SVGGENERATOR

#include <QtCore/qnamespace.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qobjectdefs.h>
#include <QtCore/qrect.h>


QT_BEGIN_NAMESPACE


class QSvgGeneratorPrivate;

class Q_SVG_EXPORT QSvgGenerator : public QObject, public QPaintDevice
{
    Q_OBJECT

    Q_DECLARE_PRIVATE(QSvgGenerator)

    Q_PROPERTY(QSize size READ size WRITE setSize)
    Q_PROPERTY(QRectF viewBox READ viewBoxF WRITE setViewBox)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
    Q_PROPERTY(QIODevice* outputDevice READ outputDevice WRITE setOutputDevice)
    Q_PROPERTY(int resolution READ resolution WRITE setResolution)
public:
    QSvgGenerator();
    ~QSvgGenerator();

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    QSize size() const;
    void setSize(const QSize &size);

    QRect viewBox() const;
    QRectF viewBoxF() const;
    void setViewBox(const QRect &viewBox);
    void setViewBox(const QRectF &viewBox);

    QString fileName() const;
    void setFileName(const QString &fileName);

    QIODevice *outputDevice() const;
    void setOutputDevice(QIODevice *outputDevice);

    void setResolution(int dpi);
    int resolution() const;
protected:
    QPaintEngine *paintEngine() const;
    int metric(QPaintDevice::PaintDeviceMetric metric) const;

private:
    QSvgGeneratorPrivate* d_ptr;
};

QT_END_NAMESPACE


#endif // QT_NO_SVGGENERATOR
#endif // QSVGGENERATOR_H
