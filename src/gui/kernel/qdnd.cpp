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

#include "qplatformdefs.h"

#include "qbitmap.h"
#include "qdrag.h"
#include "qpixmap.h"
#include "qevent.h"
#include "qfile.h"
#include "qtextcodec.h"
#include "qapplication.h"
#include "qpoint.h"
#include "qwidget.h"
#include "qbuffer.h"
#include "qimage.h"
#include "qregexp.h"
#include "qdir.h"
#include "qdnd_p.h"
#include "qimagereader.h"
#include "qimagewriter.h"
#include "qdebug.h"
#include <ctype.h>

#include "qapplication_p.h"

#ifndef QT_NO_DRAGANDDROP

QT_BEGIN_NAMESPACE

#ifndef QT_NO_DRAGANDDROP

//#define QDND_DEBUG

#ifdef QDND_DEBUG
static QString dragActionsToString(Qt::DropActions actions)
{
    QString str;
    if (actions == Qt::IgnoreAction) {
        str += "IgnoreAction";
    }
    if (actions & Qt::LinkAction) {
        if (!str.isEmpty())
            str += " | ";
        str += "LinkAction";
    }
    if (actions & Qt::CopyAction) {
        if (!str.isEmpty())
            str += " | ";
        str += "CopyAction";
    }
    if (actions & Qt::MoveAction) {
        if (!str.isEmpty())
            str += " | ";
        str += "MoveAction";
    }
    return str;
}

static QString KeyboardModifiersToString(Qt::KeyboardModifiers moderfies)
{
    QString str;
    if (moderfies & Qt::ControlModifier) {
        str += Qt::ControlModifier;
    }
    if (moderfies & Qt::AltModifier) {
        if (!str.isEmpty())
            str += " | ";
        str += Qt::AltModifier;
    }
    if (moderfies & Qt::ShiftModifier) {
        if (!str.isEmpty())
            str += " | ";
        str += Qt::ShiftModifier;
    }
    return str;
}
#endif


// the universe's only drag manager
QDragManager *QDragManager::instance = 0;


QDragManager::QDragManager()
    : QObject(qApp)
{
    Q_ASSERT(!instance);

    object = 0;
    beingCancelled = false;
    restoreCursor = false;
    willDrop = false;
    eventLoop = 0;
    dropData = new QDropData();
    currentDropTarget = 0;
}


QDragManager::~QDragManager()
{
#ifndef QT_NO_CURSOR
    if (restoreCursor)
        QApplication::restoreOverrideCursor();
#endif
    instance = 0;
    delete dropData;
}

QDragManager *QDragManager::self()
{
    if (!instance && !QApplication::closingDown())
        instance = new QDragManager;
    return instance;
}

Qt::DropAction QDragManager::defaultAction(Qt::DropActions possibleActions,
                                           Qt::KeyboardModifiers modifiers) const
{
#ifdef QDND_DEBUG
    qDebug("QDragManager::defaultAction(Qt::DropActions possibleActions)");
    qDebug("keyboard modifiers : %s", qPrintable(KeyboardModifiersToString(modifiers)));
#endif

    QDragPrivate *d = dragPrivate();
    Qt::DropAction defaultAction = d ? d->defaultDropAction : Qt::IgnoreAction;

    if (defaultAction == Qt::IgnoreAction) {
        //This means that the drag was initiated by QDrag::start and we need to
        //preserve the old behavior
        defaultAction = Qt::CopyAction;
    }

    if (modifiers & Qt::ControlModifier && modifiers & Qt::ShiftModifier)
        defaultAction = Qt::LinkAction;
    else if (modifiers & Qt::ControlModifier)
        defaultAction = Qt::CopyAction;
    else if (modifiers & Qt::ShiftModifier)
        defaultAction = Qt::MoveAction;
    else if (modifiers & Qt::AltModifier)
        defaultAction = Qt::LinkAction;

    // if the object is set take the list of possibles from it
    if (object)
        possibleActions = object->d_func()->possible_actions;

#ifdef QDND_DEBUG
    qDebug("possible actions : %s", qPrintable(dragActionsToString(possibleActions)));
#endif

    // Check if the action determined is allowed
    if (!(possibleActions & defaultAction)) {
        if (possibleActions & Qt::CopyAction)
            defaultAction = Qt::CopyAction;
        else if (possibleActions & Qt::MoveAction)
            defaultAction = Qt::MoveAction;
        else if (possibleActions & Qt::LinkAction)
            defaultAction = Qt::LinkAction;
        else
            defaultAction = Qt::IgnoreAction;
    }

#ifdef QDND_DEBUG
    qDebug("default action : %s", qPrintable(dragActionsToString(defaultAction)));
#endif

    return defaultAction;
}

void QDragManager::setCurrentTarget(QWidget *target, bool dropped)
{
    if (currentDropTarget == target)
        return;

    currentDropTarget = target;
    if (!dropped && object) {
        object->d_func()->target = target;
        emit object->targetChanged(target);
    }

}

QWidget *QDragManager::currentTarget()
{
    return currentDropTarget;
}
#endif

QDropData::QDropData()
    : QInternalMimeData()
{
}

QDropData::~QDropData()
{
}
#endif // QT_NO_DRAGANDDROP

#if !(defined(QT_NO_DRAGANDDROP) && defined(QT_NO_CLIPBOARD))
static QStringList imageReadMimeFormats()
{
    QStringList formats;
    foreach (const QByteArray &it, QImageReader::supportedImageFormats()) {
        QString format = QLatin1String("image/");
        format += QString::fromLatin1(it.toLower());
        formats.append(format);
    }

    //put png at the front because it is best
    int pngIndex = formats.indexOf(QLatin1String("image/png"));
    if (pngIndex != -1 && pngIndex != 0)
        formats.move(pngIndex, 0);

    return formats;
}


static QStringList imageWriteMimeFormats()
{
    QStringList formats;
    foreach (const QByteArray &it, QImageWriter::supportedImageFormats()) {
        QString format = QLatin1String("image/");
        format += QString::fromLatin1(it.toLower());
        formats.append(format);
    }

    //put png at the front because it is best
    int pngIndex = formats.indexOf(QLatin1String("image/png"));
    if (pngIndex != -1 && pngIndex != 0)
        formats.move(pngIndex, 0);

    return formats;
}

QInternalMimeData::QInternalMimeData()
    : QMimeData()
{
}

QInternalMimeData::~QInternalMimeData()
{
}

bool QInternalMimeData::hasFormat(const QString &mimeType) const
{
    bool foundFormat = hasFormat_sys(mimeType);
    if (!foundFormat && mimeType == QLatin1String("application/x-qt-image")) {
        foreach (const QString &it, imageReadMimeFormats()) {
            if ((foundFormat = hasFormat_sys(it)))
                break;
        }
    }
    return foundFormat;
}

QStringList QInternalMimeData::formats() const
{
    QStringList realFormats = formats_sys();
    if (!realFormats.contains(QLatin1String("application/x-qt-image"))) {
        foreach (const QString &it, imageReadMimeFormats()) {
            if (realFormats.contains(it)) {
                realFormats += QLatin1String("application/x-qt-image");
                break;
            }
        }
    }
    return realFormats;
}

QVariant QInternalMimeData::retrieveData(const QString &mimeType, QVariant::Type type) const
{
    QVariant data = retrieveData_sys(mimeType, type);
    if (mimeType == QLatin1String("application/x-qt-image")) {
        if (data.isNull() || (data.type() == QVariant::ByteArray && data.toByteArray().isEmpty())) {
            // try to find an image
            foreach (const QString &it, imageReadMimeFormats()) {
                data = retrieveData_sys(it, type);
                if (data.isNull() || (data.type() == QVariant::ByteArray && data.toByteArray().isEmpty()))
                    continue;
                break;
            }
        }
        // we wanted some image type, but all we got was a byte array. Convert it to an image.
        if (data.type() == QVariant::ByteArray
            && (type == QVariant::Image || type == QVariant::Pixmap || type == QVariant::Bitmap))
            data = QImage::fromData(data.toByteArray());

    } else if (mimeType == QLatin1String("application/x-color") && data.type() == QVariant::ByteArray) {
        QColor c;
        QByteArray ba = data.toByteArray();
        if (ba.size() == 8) {
            ushort * colBuf = (ushort *)ba.data();
            c.setRgbF(qreal(colBuf[0]) / qreal(0xFFFF),
                      qreal(colBuf[1]) / qreal(0xFFFF),
                      qreal(colBuf[2]) / qreal(0xFFFF),
                      qreal(colBuf[3]) / qreal(0xFFFF));
            data = c;
        } else {
            qWarning("Qt: Invalid color format");
        }
    } else if (data.type() != type && data.type() == QVariant::ByteArray) {
        // try to use mime data's internal conversion stuf.
        QInternalMimeData *that = const_cast<QInternalMimeData *>(this);
        that->setData(mimeType, data.toByteArray());
        data = QMimeData::retrieveData(mimeType, type);
        that->clear();
    }
    return data;
}

// helper functions for rendering mimedata to the system, this is needed because QMimeData is in core.
QStringList QInternalMimeData::formatsHelper(const QMimeData *data)
{
    QStringList realFormats = data->formats();
    if (realFormats.contains(QLatin1String("application/x-qt-image"))) {
        // add all supported image formats
        foreach (const QString &it, imageWriteMimeFormats()) {
            if (!realFormats.contains(it))
                realFormats.append(it);
        }
    }
    return realFormats;
}

bool QInternalMimeData::hasFormatHelper(const QString &mimeType, const QMimeData *data)
{

    bool foundFormat = data->hasFormat(mimeType);
    if (!foundFormat) {
        QStringList imageFormats = imageWriteMimeFormats();
        if (mimeType == QLatin1String("application/x-qt-image")) {
            // check all supported image formats
            foreach (const QString &it, imageFormats) {
                if ((foundFormat = data->hasFormat(it)))
                    break;
            }
        } else if (mimeType.startsWith(QLatin1String("image/"))) {
            return data->hasImage() && imageFormats.contains(mimeType);
        }
    }
    return foundFormat;
}

QByteArray QInternalMimeData::renderDataHelper(const QString &mimeType, const QMimeData *data)
{
    QByteArray ba;
    if (mimeType == QLatin1String("application/x-color")) {
        /* QMimeData can only provide colors as QColor or the name
           of a color as a QByteArray or a QString. So we need to do
           the conversion to application/x-color here.
           The application/x-color format is :
           type: application/x-color
           format: 16
           data[0]: red
           data[1]: green
           data[2]: blue
           data[3]: opacity
        */
        ba.resize(8);
        ushort * colBuf = (ushort *)ba.data();
        QColor c = qvariant_cast<QColor>(data->colorData());
        colBuf[0] = ushort(c.redF() * 0xFFFF);
        colBuf[1] = ushort(c.greenF() * 0xFFFF);
        colBuf[2] = ushort(c.blueF() * 0xFFFF);
        colBuf[3] = ushort(c.alphaF() * 0xFFFF);
    } else {
        ba = data->data(mimeType);
        if (ba.isEmpty()) {
            if (mimeType == QLatin1String("application/x-qt-image") && data->hasImage()) {
                QImage image = qvariant_cast<QImage>(data->imageData());
                QBuffer buf(&ba);
                buf.open(QBuffer::WriteOnly);
                // would there not be PNG ??
                image.save(&buf, "PNG");
            } else if (mimeType.startsWith(QLatin1String("image/")) && data->hasImage()) {
                QImage image = qvariant_cast<QImage>(data->imageData());
                QBuffer buf(&ba);
                buf.open(QBuffer::WriteOnly);
                image.save(&buf, mimeType.mid(mimeType.indexOf(QLatin1Char('/')) + 1).toLatin1().toUpper());
            }
        }
    }
    return ba;
}

#endif // QT_NO_DRAGANDDROP && QT_NO_CLIPBOARD

QT_END_NAMESPACE


#include "moc_qdnd_p.h"

