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

// #define QIMAGEREADER_DEBUG

/*!
    \class QImageReader
    \brief The QImageReader class provides a format independent interface
    for reading images from files or other devices.

    \reentrant
    \ingroup painting
    \ingroup io

    The most common way to read images is through QImage and QPixmap's
    constructors, or by calling QImage::load() and
    QPixmap::load(). QImageReader is a specialized class which gives
    you more control when reading images. For example, you can read an
    image into a specific size by calling setScaledSize(). Depending
    on the underlying support in the image format, this can save
    memory and speed up loading of images.

    To read an image, you start by constructing a QImageReader object.
    Pass either a file name or a device pointer, and the image format
    to QImageReader's constructor. You can then set several options,
    such as the scaled size (by calling setScaledSize()). canRead()
    returns the image if the QImageReader can read the image (i.e.,
    the image format is supported and the device is open for reading).
    Call read() to read the image.

    If any error occurs when reading the image, read() will return a
    null QImage. You can then call error() to find the type of error
    that occurred, or errorString() to get a human readable
    description of what went wrong.

    Call supportedImageFormats() for a list of formats that
    QImageReader can read. QImageReader supports all built-in image
    formats, in addition to any image format plugins that support
    reading.

    QImageReader autodetects the image format by default, by looking at the
    provided (optional) format string, the file name suffix, and the data
    stream contents. You can enable or disable this feature, by calling
    setAutoDetectImageFormat().

    \sa QImageWriter, QImageIOHandler, QImageIOPlugin
*/

/*!
    \enum QImageReader::ImageReaderError

    This enum describes the different types of errors that can occur
    when reading images with QImageReader.

    \value FileNotFoundError QImageReader was used with a file name,
    but not file was found with that name. This can also happen if the
    file name contained no extension, and the file with the correct
    extension is not supported by Qt.

    \value DeviceError QImageReader encountered a device error when
    reading the image. You can consult your particular device for more
    details on what went wrong.

    \value UnsupportedFormatError Qt does not support the requested
    image format.

    \value InvalidDataError The image data was invalid, and
    QImageReader was unable to read an image from it. The can happen
    if the image file is damaged.

    \value UnknownError An unknown error occurred. If you get this
    value after calling read(), it is most likely caused by a bug in
    QImageReader.
*/
#include "qimagereader.h"

#include "qbytearray.h"
#include "qfile.h"
#include "qimage.h"
#include "qimageiohandler.h"
#include "qlist.h"
#include "qset.h"
#include "qsize.h"
#include "qcolor.h"
#include "qvariant.h"
#include "qapplication.h"

// factory loader
#include "qcoreapplication.h"
#include "qfactoryloader_p.h"
#include "qguicommon_p.h"

// image handlers
#include "qppmhandler_p.h"
#include "qxpmhandler_p.h"
#include "qkathandler_p.h"
#include "qpnghandler_p.h"

#ifdef QIMAGEREADER_DEBUG
#  include "qdebug.h"
#endif

QT_BEGIN_NAMESPACE

static QImageIOHandler *createReadHandlerHelper(QIODevice *device,
                                                const QByteArray &format,
                                                bool autoDetectImageFormat)
{
    if (!autoDetectImageFormat && format.isEmpty())
        return nullptr;

    const QByteArray form = format.toLower();
    QImageIOHandler *handler = nullptr;

    // check if we have built-in support for the format first
    if (form == "png") {
        handler = new QPngHandler();
#ifndef QT_NO_IMAGEFORMAT_KAT
    } else if (form == "kat") {
        handler = new QKatHandler();
#endif
#ifndef QT_NO_IMAGEFORMAT_XPM
    } else if (form == "xpm") {
        handler = new QXpmHandler();
#endif
#ifndef QT_NO_IMAGEFORMAT_PPM
    } else if (form == "pbm" || form == "pbmraw" || form == "ppm" || form == "ppmraw") {
        handler = new QPpmHandler();
        handler->setOption(QImageIOHandler::SubType, form);
#endif
    }

    if (handler) {
#ifdef QIMAGEREADER_DEBUG

        qDebug() << "QImageReader::createReadHandler: using the built-in handler for" << form;
#endif
        handler->setDevice(device);
        handler->setFormat(form);
        return handler;
    }

#ifndef QT_NO_DEBUG
    const qint64 devicepos = device->pos();
#endif
    if (device && autoDetectImageFormat) {
        if (QPngHandler::canRead(device)) {
            handler = new QPngHandler();
        }
#ifndef QT_NO_IMAGEFORMAT_KAT
        if (!handler && QKatHandler::canRead(device)) {
            handler = new QKatHandler();
        }
#endif
#ifndef QT_NO_IMAGEFORMAT_XPM
        if (!handler && QXpmHandler::canRead(device)) {
            handler = new QXpmHandler();
        }
#endif
#ifndef QT_NO_IMAGEFORMAT_PPM
        QByteArray subType;
        if (!handler && QPpmHandler::canRead(device, &subType)) {
            handler = new QPpmHandler();
            handler->setOption(QImageIOHandler::SubType, subType);
        }
#endif
    }
#ifndef QT_NO_DEBUG
    Q_ASSERT(device->pos() == devicepos);
#endif

    if (handler) {
#ifdef QIMAGEREADER_DEBUG
        qDebug() << "QImageReader::createReadHandler: the" << handler->name()
                 << "built-in handler can read this data";
#endif
        handler->setDevice(device);
        handler->setFormat(handler->name());
        return handler;
    }

#ifndef QT_NO_LIBRARY
    QFactoryLoader *l = imageloader();
    const QStringList keys = l->keys();

#ifdef QIMAGEREADER_DEBUG
    qDebug() << "QImageReader::createReadHandler( device =" << (void *)device << ", format =" << format << "),"
             << keys.size() << "plugins available: " << keys;
#endif

    // check if any plugin recognize the format
    if (!form.isEmpty()) {
        foreach (const QString &key, keys) {
            QImageIOPlugin *plugin = qobject_cast<QImageIOPlugin *>(l->instance(key));
            if (plugin && plugin->capabilities(device, form) & QImageIOPlugin::CanRead) {
                handler = plugin->create(device, key.toLatin1());
#ifdef QIMAGEREADER_DEBUG
                qDebug() << "QImageReader::createReadHandler: the" << key << "plugin can read this format";
#endif
                break;
            }
        }
    }

    // check if any plugin recognize the content
    if (!handler && device && autoDetectImageFormat) {
        foreach (const QString &key, keys) {
            QImageIOPlugin *plugin = qobject_cast<QImageIOPlugin *>(l->instance(key));
            if (plugin && plugin->capabilities(device, QByteArray()) & QImageIOPlugin::CanRead) {
                handler = plugin->create(device, key.toLatin1());
#ifdef QIMAGEREADER_DEBUG
                qDebug() << "QImageReader::createReadHandler: the" << key << "plugin can read this data";
#endif
                break;
            }
        }
    }
#ifndef QT_NO_DEBUG
    Q_ASSERT(device->pos() == devicepos);
#endif
#endif // QT_NO_LIBRARY

    if (!handler) {
#ifdef QIMAGEREADER_DEBUG
        qDebug() << "QImageReader::createReadHandler: no handlers found. giving up.";
#endif
        // no handler: give up.
        return nullptr;
    }

    return handler;
}

class QImageReaderPrivate
{
public:
    QImageReaderPrivate();
    ~QImageReaderPrivate();

    // device
    QByteArray format;
    bool autoDetectImageFormat;
    QIODevice *device;
    bool deleteDevice;
    QImageIOHandler *handler;
    bool initHandler();

    // image options
    QSize scaledSize;

    // error
    QImageReader::ImageReaderError imageReaderError;
};

/*!
    \internal
*/
QImageReaderPrivate::QImageReaderPrivate()
    : autoDetectImageFormat(true),
    device(nullptr),
    deleteDevice(false),
    handler(nullptr),
    imageReaderError(QImageReader::UnknownError)
{
}

/*!
    \internal
*/
QImageReaderPrivate::~QImageReaderPrivate()
{
    if (deleteDevice)
        delete device;
    delete handler;
}

/*!
    \internal
*/
bool QImageReaderPrivate::initHandler()
{
    // check some preconditions
    if (!device) {
        imageReaderError = QImageReader::DeviceError;
        return false;
    }

    if (!device->isOpen() && !device->open(QIODevice::ReadOnly)) {
        imageReaderError = QImageReader::FileNotFoundError;
        return false;
    }

    // assign a handler
    if (!handler && (handler = createReadHandlerHelper(device, format, autoDetectImageFormat)) == 0) {
        imageReaderError = QImageReader::UnsupportedFormatError;
        return false;
    }
    return true;
}

/*!
    Constructs an empty QImageReader object. Before reading an image,
    call setDevice() or setFileName().
*/
QImageReader::QImageReader()
    : d(new QImageReaderPrivate())
{
}

/*!
    Constructs a QImageReader object with the device \a device and the
    image format \a format.
*/
QImageReader::QImageReader(QIODevice *device, const QByteArray &format)
    : d(new QImageReaderPrivate())
{
    d->device = device;
    d->format = format;
}

/*!
    Constructs a QImageReader object with the file name \a fileName
    and the image format \a format.

    \sa setFileName()
*/
QImageReader::QImageReader(const QString &fileName, const QByteArray &format)
    : d(new QImageReaderPrivate())
{
    d->device = new QFile(fileName);
    d->deleteDevice = true;
    d->format = format;
}

/*!
    Destructs the QImageReader object.
*/
QImageReader::~QImageReader()
{
    delete d;
}

/*!
    Sets the format QImageReader will use when reading images, to \a
    format. \a format is a case insensitive text string. Example:

    \snippet doc/src/snippets/code/src_gui_image_qimagereader.cpp 0

    You can call supportedImageFormats() for the full list of formats
    QImageReader supports.

    \sa format()
*/
void QImageReader::setFormat(const QByteArray &format)
{
    delete d->handler;
    d->handler = nullptr;
    d->format = format;
}

/*!
    Returns the format QImageReader uses for reading images.

    You can call this function after assigning a device to the
    reader to determine the format of the device. For example:

    \snippet doc/src/snippets/code/src_gui_image_qimagereader.cpp 1

    If the reader cannot read any image from the device (e.g., there is no
    image there, or the image has already been read), or if the format is
    unsupported, this function returns an empty QByteArray().

    \sa setFormat(), supportedImageFormats()
*/
QByteArray QImageReader::format() const
{
    if (d->format.isEmpty()) {
        if (!d->initHandler())
            return QByteArray();
        return d->handler->canRead() ? d->handler->format() : QByteArray();
    }

    return d->format;
}

/*!
    If \a enabled is true, image format autodetection is enabled; otherwise,
    it is disabled. By default, autodetection is enabled.

    QImageReader uses an extensive approach to detecting the image format;
    firstly, if you pass a file name to QImageReader. It uses the following
    approach to detect the image format:

    \list

    \o Built-in handlers are checked based on format string.

    \o If no built-in image handler recognizes the format and auto detection
    is enabled the image contents is inspected.

    \o Image plugins are queried based on the format string. No content
    detection is done at this stage. QImageReader will choose the first
    plugin that supports reading for this format.

    \o If no capable plugins handlers are found, each plugin is tested by
    inspecting the content.

    \o Finally, if all above approaches fail, QImageReader will report failure
    when trying to read the image.

    \endlist

    By disabling image format autodetection, QImageReader will only query the
    plugins and built-in handlers based on the format string.

    \sa QImageIOHandler::canRead(), QImageIOPlugin::capabilities()
*/
void QImageReader::setAutoDetectImageFormat(bool enabled)
{
    d->autoDetectImageFormat = enabled;
}

/*!
    Returns true if image format autodetection is enabled on this image
    reader; otherwise returns false. By default, autodetection is enabled.

    \sa setAutoDetectImageFormat()
*/
bool QImageReader::autoDetectImageFormat() const
{
    return d->autoDetectImageFormat;
}

/*!
    Sets QImageReader's device to \a device. If a device has already
    been set, the old device is removed from QImageReader and is
    otherwise left unchanged.

    If the device is not already open, QImageReader will attempt to
    open the device in \l QIODevice::ReadOnly mode by calling
    open(). Note that this does not work for certain devices, such as
    QProcess, QTcpSocket and QUdpSocket, where more logic is required
    to open the device.

    \sa device(), setFileName()
*/
void QImageReader::setDevice(QIODevice *device)
{
    if (d->device && d->deleteDevice)
        delete d->device;
    d->device = device;
    d->deleteDevice = false;
    delete d->handler;
    d->handler = nullptr;
}

/*!
    Returns the device currently assigned to QImageReader, or 0 if no
    device has been assigned.
*/
QIODevice *QImageReader::device() const
{
    return d->device;
}

/*!
    Sets the file name of QImageReader to \a fileName. Internally,
    QImageReader will create a QFile object and open it in \l
    QIODevice::ReadOnly mode, and use this when reading images.

    If \a fileName does not include a file extension (e.g., .png or .xpm),
    QImageReader will cycle through all supported extensions until it finds
    a matching file.

    \sa fileName(), setDevice(), supportedImageFormats()
*/
void QImageReader::setFileName(const QString &fileName)
{
    setDevice(new QFile(fileName));
    d->deleteDevice = true;
}

/*!
    If the currently assigned device is a QFile, or if setFileName()
    has been called, this function returns the name of the file
    QImageReader reads from. Otherwise (i.e., if no device has been
    assigned or the device is not a QFile), an empty QString is
    returned.

    \sa setFileName(), setDevice()
*/
QString QImageReader::fileName() const
{
    QFile *file = qobject_cast<QFile *>(d->device);
    return file ? file->fileName() : QString();
}

/*!
    Returns the size of the image, without actually reading the image
    contents.

    If the image format does not support this feature, this function returns
    an invalid size. Some of Katie's built-in image handlers all support
    this feature, but custom image format plugins are not required to do so.

    \sa QImageIOHandler::ImageOption, QImageIOHandler::option(), QImageIOHandler::supportsOption()
*/
QSize QImageReader::size() const
{
    if (!d->initHandler())
        return QSize();
    if (d->handler->supportsOption(QImageIOHandler::Size))
        return d->handler->option(QImageIOHandler::Size).toSize();
    return QSize();
}

/*!
    Sets the scaled size of the image to \a size. The algorithm
    used for scaling depends on the image format. By default
    (i.e., if the image format does not support scaling),
    QImageReader will use QImage::scale() with Qt::SmoothScaling.

    \sa scaledSize()
*/
void QImageReader::setScaledSize(const QSize &size)
{
    d->scaledSize = size;
}

/*!
    Returns the scaled size of the image.

    \sa setScaledSize()
*/
QSize QImageReader::scaledSize() const
{
    return d->scaledSize;
}

/*!
    \since 4.1

    Sets the background color to \a color.
    Image formats that support this operation are expected to
    initialize the background to \a color before reading an image.

    \sa backgroundColor(), read()
*/
void QImageReader::setBackgroundColor(const QColor &color)
{
    if (!d->initHandler())
        return;
    if (d->handler->supportsOption(QImageIOHandler::BackgroundColor))
        d->handler->setOption(QImageIOHandler::BackgroundColor, color);
}

/*!
    \since 4.1

    Returns the background color that's used when reading an image.
    If the image format does not support setting the background color
    an invalid color is returned.

    \sa setBackgroundColor(), read()
*/
QColor QImageReader::backgroundColor() const
{
    if (!d->initHandler())
        return QColor();
    if (d->handler->supportsOption(QImageIOHandler::BackgroundColor))
        return qvariant_cast<QColor>(d->handler->option(QImageIOHandler::BackgroundColor));
    return QColor();
}

/*!
    \since 4.1

    Returns true if the image format supports animation;
    otherwise, false is returned.

    \sa QMovie::supportedFormats()
*/
bool QImageReader::supportsAnimation() const
{
    if (!d->initHandler())
        return false;
    if (d->handler->supportsOption(QImageIOHandler::Animation))
        return d->handler->option(QImageIOHandler::Animation).toBool();
    return false;
}

/*!
    Returns true if an image can be read for the device (i.e., the
    image format is supported, and the device seems to contain valid
    data); otherwise returns false.

    canRead() is a lightweight function that only does a quick test to
    see if the image data is valid. read() may still return false
    after canRead() returns true, if the image data is corrupt.

    For images that support animation, canRead() returns false when
    all frames have been read.

    \sa read(), supportedImageFormats()
*/
bool QImageReader::canRead() const
{
    if (!d->initHandler())
        return false;

    return d->handler->canRead();
}

/*!
    Reads an image from the device. On success, the image that was
    read is returned; otherwise, a null QImage is returned. You can
    then call error() to find the type of error that occurred, or
    errorString() to get a human readable description of the error.

    For image formats that support animation, calling read()
    repeatedly will return the next frame. When all frames have been
    read, a null image will be returned.

    \sa canRead(), supportedImageFormats(), supportsAnimation(), QMovie
*/
QImage QImageReader::read()
{
    // Because failed image reading might have side effects, we explicitly
    // return a null image instead of the image we've just created.
    QImage image;
    return read(&image) ? image : QImage();
}

/*!
    \overload

    Reads an image from the device into \a image, which must point to a
    QImage. Returns true on success; otherwise, returns false.

    If \a image has same format and size as the image data that is about to be
    read, this function may not need to allocate a new image before
    reading. Because of this, it can be faster than the other read() overload,
    which always constructs a new image; especially when reading several
    images with the same format and size.

    \snippet doc/src/snippets/code/src_gui_image_qimagereader.cpp 2

    For image formats that support animation, calling read() repeatedly will
    return the next frame. When all frames have been read, a null image will
    be returned.

    \sa canRead(), supportedImageFormats(), supportsAnimation(), QMovie
*/
bool QImageReader::read(QImage *image)
{
    if (Q_UNLIKELY(!image)) {
        qWarning("QImageReader::read: cannot read into null pointer");
        return false;
    }

    if (!d->initHandler())
        return false;

    // set the handler specific options.
    if (d->handler->supportsOption(QImageIOHandler::ScaledSize) && d->scaledSize.isValid()) {
        d->handler->setOption(QImageIOHandler::ScaledSize, d->scaledSize);
    }

    // read the image
    if (!d->handler->read(image)) {
        d->imageReaderError = QImageReader::InvalidDataError;
        return false;
    }

    // provide default implementations for any unsupported image options
    if (!d->handler->supportsOption(QImageIOHandler::ScaledSize) && d->scaledSize.isValid()) {
        *image = image->scaled(d->scaledSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    return true;
}

/*!
   For image formats that support animation, this function steps over the
   current image, returning true if successful or false if there is no
   following image in the animation.

   The default implementation calls read(), then discards the resulting
   image, but the image handler may have a more efficient way of implementing
   this operation.

   \sa jumpToImage(), QImageIOHandler::jumpToNextImage()
*/
bool QImageReader::jumpToNextImage()
{
    if (!d->initHandler())
        return false;
    return d->handler->jumpToNextImage();
}

/*!
   For image formats that support animation, this function skips to the image
   whose sequence number is \a imageNumber, returning true if successful
   or false if the corresponding image cannot be found.

   The next call to read() will attempt to read this image.

   \sa jumpToNextImage(), QImageIOHandler::jumpToImage()
*/
bool QImageReader::jumpToImage(int imageNumber)
{
    if (!d->initHandler())
        return false;
    return d->handler->jumpToImage(imageNumber);
}

/*!
    For image formats that support animation, this function returns the number
    of times the animation should loop. If this function returns -1, it can
    either mean the animation should loop forever, or that an error occurred.
    If an error occurred, canRead() will return false.

    \sa supportsAnimation(), QImageIOHandler::loopCount(), canRead()
*/
int QImageReader::loopCount() const
{
    if (!d->initHandler())
        return -1;
    return d->handler->loopCount();
}

/*!
    For image formats that support animation, this function returns the total
    number of images in the animation. If the format does not support
    animation, 0 is returned.

    This function returns -1 if an error occurred.

    \sa supportsAnimation(), QImageIOHandler::imageCount(), canRead()
*/
int QImageReader::imageCount() const
{
    if (!d->initHandler())
        return -1;
    return d->handler->imageCount();
}

/*!
    For image formats that support animation, this function returns the number
    of milliseconds to wait until displaying the next frame in the animation.
    If the image format doesn't support animation, 0 is returned.

    This function returns -1 if an error occurred.

    \sa supportsAnimation(), QImageIOHandler::nextImageDelay(), canRead()
*/
int QImageReader::nextImageDelay() const
{
    if (!d->initHandler())
        return -1;
    return d->handler->nextImageDelay();
}

/*!
    For image formats that support animation, this function returns the
    sequence number of the current frame. If the image format doesn't support
    animation, 0 is returned.

    This function returns -1 if an error occurred.

    \sa supportsAnimation(), QImageIOHandler::currentImageNumber(), canRead()
*/
int QImageReader::currentImageNumber() const
{
    if (!d->initHandler())
        return -1;
    return d->handler->currentImageNumber();
}

/*!
    Returns the type of error that occurred last.

    \sa ImageReaderError, errorString()
*/
QImageReader::ImageReaderError QImageReader::error() const
{
    return d->imageReaderError;
}

/*!
    Returns a human readable description of the last error that
    occurred.

    \sa error()
*/
QString QImageReader::errorString() const
{
    switch (d->imageReaderError) {
        case QImageReader::UnknownError: {
            return QApplication::translate("QImageReader", "Unknown error");
        }
        case QImageReader::FileNotFoundError: {
            return QApplication::translate("QImageReader", "File not found");
        }
        case QImageReader::DeviceError: {
            return QApplication::translate("QImageReader", "Device not readable");
        }
        case QImageReader::UnsupportedFormatError: {
            return QApplication::translate("QImageReader", "Unsupported image format");
        }
        case QImageReader::InvalidDataError: {
            return QApplication::translate("QImageReader", "Unable to read image data");
        }
    }
    Q_UNREACHABLE();
    return QString();
}

/*!
    \since 4.2

    Returns true if the reader supports \a option; otherwise returns
    false.

    Different image formats support different options. Call this function to
    determine whether a certain option is supported by the current format.

    \snippet doc/src/snippets/code/src_gui_image_qimagereader.cpp 3

    \sa QImageWriter::supportsOption()
*/
bool QImageReader::supportsOption(QImageIOHandler::ImageOption option) const
{
    if (!d->initHandler())
        return false;
    return d->handler->supportsOption(option);
}

/*!
    If supported, this function returns the image format of the file
    \a fileName. Otherwise, an empty string is returned.
*/
QByteArray QImageReader::imageFormat(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return QByteArray();

    return imageFormat(&file);
}

/*!
    If supported, this function returns the image format of the device
    \a device. Otherwise, an empty string is returned.

    \sa QImageReader::autoDetectImageFormat()
*/
QByteArray QImageReader::imageFormat(QIODevice *device)
{
    QByteArray format;
    QImageIOHandler *handler = createReadHandlerHelper(device, format, /* autoDetectImageFormat = */ true);
    if (handler) {
        if (handler->canRead())
            format = handler->format();
        delete handler;
    }
    return format;
}

/*!
    Returns the list of image formats supported by QImageReader.

    By default, Katie can read the following formats:

    \table
    \header \o Format \o Description
    \row    \o PNG    \o Portable Network Graphics
    \row    \o KAT    \o Katie Image
    \row    \o PBM    \o Portable Bitmap
    \row    \o PPM    \o Portable Pixmap
    \row    \o XPM    \o X11 Pixmap
    \row    \o SVG    \o Scalable Vector Graphics
    \endtable

    Reading and writing SVG files is supported through Qt's
    \l{QtSvg Module}{SVG Module}.

    Note that the QApplication instance must be created before this function is
    called.

    \sa setFormat(), QImageWriter::supportedImageFormats(), QImageIOPlugin
*/
QList<QByteArray> QImageReader::supportedImageFormats()
{
    QList<QByteArray> formats = QList<QByteArray>()
        << "png"
#ifndef QT_NO_IMAGEFORMAT_KAT
        << "kat"
#endif
#ifndef QT_NO_IMAGEFORMAT_XPM
        << "xpm"
#endif
#ifndef QT_NO_IMAGEFORMAT_PPM
        << "ppm" << "pbm"
#endif
        ;

#ifndef QT_NO_LIBRARY
    QFactoryLoader *l = imageloader();
    foreach (const QString &key, l->keys()) {
        QImageIOPlugin *plugin = qobject_cast<QImageIOPlugin *>(l->instance(key));
        if (plugin && plugin->capabilities(0, key.toLatin1()) & QImageIOPlugin::CanRead)
            formats << key.toLatin1();
    }
#endif // QT_NO_LIBRARY

    qSort(formats);
    return formats;
}

/*!
    \since 4.12

    Returns the format string for image MIME specified by \a mime.

    \sa supportedMimeTypes()
*/
QByteArray QImageReader::formatForMimeType(const QByteArray &mime)
{
    if (mime == "image/png") {
        return QByteArray("png");
    }
#ifndef QT_NO_IMAGEFORMAT_KAT
    if (mime == "image/katie") {
        return QByteArray("kat");
    }
#endif
#ifndef QT_NO_IMAGEFORMAT_XPM
    if (mime == "image/x-xpixmap") {
        return QByteArray("xpm");
    }
#endif
#ifndef QT_NO_IMAGEFORMAT_PPM
    if (mime == "image/x-portable-pixmap") {
        return QByteArray("ppm");
    } else if (mime == "image/x-portable-bitmap") {
        return QByteArray("pbm");
    }
#endif

#ifndef QT_NO_LIBRARY
    QFactoryLoader *l = imageloader();
    foreach (const QString &key, l->keys()) {
        QImageIOPlugin *plugin = qobject_cast<QImageIOPlugin *>(l->instance(key));
        if (plugin && plugin->capabilities(0, key.toLatin1()) & QImageIOPlugin::CanRead) {
            if (plugin->mimeTypes().contains(mime)) {
                return key.toLatin1();
            }
        }
    }
#endif // QT_NO_LIBRARY

    return QByteArray();
}

/*!
    \since 4.12

    Returns the list of image MIME types supported by QImageReader.

    \sa supportedImageFormats(), QImageWriter::supportedImageFormats()
*/
QList<QByteArray> QImageReader::supportedMimeTypes()
{
    QList<QByteArray> mimes = QList<QByteArray>()
        << "image/png"
#ifndef QT_NO_IMAGEFORMAT_KAT
        << "image/katie"
#endif
#ifndef QT_NO_IMAGEFORMAT_XPM
        << "image/x-xpixmap"
#endif
#ifndef QT_NO_IMAGEFORMAT_PPM
        << "image/x-portable-pixmap" << "image/x-portable-bitmap"
#endif
        ;

#ifndef QT_NO_LIBRARY
    QFactoryLoader *l = imageloader();
    foreach (const QString &key, l->keys()) {
        QImageIOPlugin *plugin = qobject_cast<QImageIOPlugin *>(l->instance(key));
        if (plugin && plugin->capabilities(0, key.toLatin1()) & QImageIOPlugin::CanRead) {
            foreach (const QByteArray &mime, plugin->mimeTypes()) {
                if (!mimes.contains(mime)) {
                    mimes << mime;
                }
            }
        }
    }
#endif // QT_NO_LIBRARY


    qSort(mimes);
    return mimes;
}

QT_END_NAMESPACE
