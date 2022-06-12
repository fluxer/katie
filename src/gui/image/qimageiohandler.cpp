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

/*!
    \class QImageIOHandler
    \brief The QImageIOHandler class defines the common image I/O
    interface for all image formats in Qt.
    \reentrant

    Qt uses QImageIOHandler for reading and writing images through
    QImageReader and QImageWriter. You can also derive from this class
    to write your own image format handler using Qt's plugin mechanism.

    Call setDevice() to assign a device to the handler, and
    setFormat() to assign a format to it. One QImageIOHandler may
    support more than one image format. canRead() returns true if an
    image can be read from the device, and read() and write() return
    true if reading or writing an image was completed successfully.

    QImageIOHandler also has support for animations formats, through
    the functions loopCount(), imageCount(), nextImageDelay() and
    currentImageNumber().

    In order to determine what options an image handler supports, Qt
    will call supportsOption() and setOption(). Make sure to
    reimplement these functions if you can provide support for any of
    the options in the ImageOption enum.

    To write your own image handler, you must at least reimplement
    canRead() and read(). Then create a QImageIOPlugin that
    can create the handler. Finally, install your plugin, and
    QImageReader and QImageWriter will then automatically load the
    plugin, and start using it.

    \sa QImageIOPlugin, QImageReader, QImageWriter
*/

/*! \enum QImageIOHandler::ImageOption

    This enum describes the different options supported by
    QImageIOHandler.  Some options are used to query an image for
    properties, and others are used to toggle the way in which an
    image should be written.

    \value Size The original size of an image. A handler that supports
    this option is expected to read the size of the image from the
    image metadata, and return this size from option() as a QSize.

    \value ScaledSize The scaled size of the image. A handler that
    supports this option is expected to scale the image to the
    provided size (a QSize). If the handler does not support this
    option, QImageReader will perform the scaling after the image has
    been read.

    \value CompressionLevel The compression level of the image data.
    A handler that supports this option is expected to set its
    compression level depending on the value of this option (an int)
    when writing. The value is usually in the 0-9 range.

    \value Gamma The gamma level of the image. A handler that supports
    this option is expected to set the image gamma level depending on
    the value of this option (a float) when writing.

    \value Quality The quality level of the image. A handler that
    supports this option is expected to set the image quality level
    depending on the value of this option (an int) when writing.

    \value Animation Image formats that support animation return
    true for this value in supportsOption(); otherwise, false is returned.

    \value BackgroundColor Certain image formats allow the
    background color to be specified. A handler that supports
    BackgroundColor initializes the background color to this option
    (a QColor) when reading an image.
*/

/*!
    \class QImageIOPlugin
    \brief The QImageIOPlugin class defines an interface for writing
    an image format plugin.
    \reentrant

    \ingroup plugins

    QImageIOPlugin is a factory for creating QImageIOHandler objects,
    which are used internally by QImageReader and QImageWriter to add
    support for different image formats to Qt.

    Writing an image I/O plugin is achieved by subclassing this
    base class, reimplementing the pure virtual functions capabilities()
    and create(), and exporting the class with the Q_EXPORT_PLUGIN2()
    macro. See \l{How to Create Qt Plugins} for details.

    An image format plugin can support three capabilities: reading (\l
    CanRead) and writing (\l CanWrite). Reimplement capabilities() in your
    subclass to expose the capabilities of your image format.

    create() should create an instance of your QImageIOHandler
    subclass, with the provided device and format properly set, and
    return this handler.

    Different plugins can support different capabilities. For example,
    you may have one plugin that supports reading the SVG format, and
    another that supports writing. Qt will select the correct plugin
    for the job, depending on the return value of capabilities(). If
    several plugins support the same capability, Qt will select one
    arbitrarily.

    \sa QImageIOHandler, {How to Create Qt Plugins}
*/

/*!
    \enum QImageIOPlugin::Capability

    This enum describes the capabilities of a QImageIOPlugin.

    \value CanRead The plugin can read images.
    \value CanWrite The plugin can write images.
*/

#include "qimageiohandler.h"
#include "qbytearray.h"
#include "qimage.h"
#include "qvariant.h"

QT_BEGIN_NAMESPACE

class QIODevice;

class QImageIOHandlerPrivate
{
public:
    QImageIOHandlerPrivate();

    QIODevice *device;
    mutable QByteArray format;
};

QImageIOHandlerPrivate::QImageIOHandlerPrivate()
    : device(nullptr)
{
}

/*!
    Constructs a QImageIOHandler object.
*/
QImageIOHandler::QImageIOHandler()
    : d_ptr(new QImageIOHandlerPrivate())
{
}

/*!
    Destructs the QImageIOHandler object.
*/
QImageIOHandler::~QImageIOHandler()
{
    delete d_ptr;
}

/*!
    Sets the device of the QImageIOHandler to \a device. The image
    handler will use this device when reading and writing images.

    The device can only be set once and must be set before calling
    canRead(), read(), write(), etc. If you need to read multiple
    files, construct multiple instances of the appropriate
    QImageIOHandler subclass.

    \sa device()
*/
void QImageIOHandler::setDevice(QIODevice *device)
{
    Q_D(QImageIOHandler);
    d->device = device;
}

/*!
    Returns the device currently assigned to the QImageIOHandler. If
    not device has been assigned, 0 is returned.
*/
QIODevice *QImageIOHandler::device() const
{
    Q_D(const QImageIOHandler);
    return d->device;
}

/*!
    Sets the format of the QImageIOHandler to \a format. The format is
    most useful for handlers that support multiple image formats.

    \sa format()
*/
void QImageIOHandler::setFormat(const QByteArray &format)
{
    Q_D(QImageIOHandler);
    d->format = format;
}

/*!
    Sets the format of the QImageIOHandler to \a format. The format is
    most useful for handlers that support multiple image formats.

    This function is declared const so that it can be called from canRead().

    \sa format()
*/
void QImageIOHandler::setFormat(const QByteArray &format) const
{
    Q_D(const QImageIOHandler);
    d->format = format;
}

/*!
    Returns the format that is currently assigned to
    QImageIOHandler. If no format has been assigned, an empty string
    is returned.

    \sa setFormat()
*/
QByteArray QImageIOHandler::format() const
{
    Q_D(const QImageIOHandler);
    return d->format;
}

/*!
    \fn bool QImageIOHandler::read(QImage *image)

    Read an image from the device, and stores it in \a image.
    Returns true if the image is successfully read; otherwise returns
    false.

    For animation formats, the image handler can assume that \a image
    points to the previous frame.

    \sa canRead()
*/

/*!
    \fn bool QImageIOHandler::canRead() const

    Returns true if an image can be read from the device (i.e., the
    image format is supported, the device can be read from and the
    initial header information suggests that the image can be read);
    otherwise returns false.

    When reimplementing canRead(), make sure that the I/O device
    (device()) is left in its original state (e.g., by using peek()
    rather than read()).

    \sa read(), QIODevice::peek()
*/

/*!
    \obsolete

    Use format() instead.
*/

QByteArray QImageIOHandler::name() const
{
    return format();
}

/*!
    Writes the image \a image to the assigned device. Returns true on
    success; otherwise returns false.

    The default implementation does nothing, and simply returns false.
*/
bool QImageIOHandler::write(const QImage &image)
{
    Q_UNUSED(image);
    return false;
}

/*!
    Sets the option \a option with the value \a value.

    \sa option(), ImageOption
*/
void QImageIOHandler::setOption(ImageOption option, const QVariant &value)
{
    Q_UNUSED(option);
    Q_UNUSED(value);
}

/*!
    Returns the value assigned to \a option as a QVariant. The type of
    the value depends on the option. For example, option(Size) returns
    a QSize variant.

    \sa setOption(), supportsOption()
*/
QVariant QImageIOHandler::option(ImageOption option) const
{
    Q_UNUSED(option);
    return QVariant();
}

/*!
    Returns true if the QImageIOHandler supports the option \a option;
    otherwise returns false. For example, if the QImageIOHandler
    supports the \l Size option, supportsOption(Size) must return
    true.

    \sa setOption(), option()
*/
bool QImageIOHandler::supportsOption(ImageOption option) const
{
    Q_UNUSED(option);
    return false;
}

/*!
    For image formats that support animation, this function returns
    the sequence number of the current image in the animation. If
    this function is called before any image is read(), -1 is
    returned. The number of the first image in the sequence is 0.
    
    If the image format does not support animation, 0 is returned.

    \sa read()
*/
int QImageIOHandler::currentImageNumber() const
{
    return 0;
}

/*!
    For image formats that support animation, this function returns
    the number of images in the animation. If the image format does
    not support animation, or if it is unable to determine the number
    of images, 0 is returned.

    The default implementation returns 1 if canRead() returns true;
    otherwise 0 is returned.
*/
int QImageIOHandler::imageCount() const
{
    return canRead() ? 1 : 0;
}

/*!
   For image formats that support animation, this function jumps to the
   next image.

   The default implementation does nothing, and returns false.
*/
bool QImageIOHandler::jumpToNextImage()
{
    return false;
}

/*!
   For image formats that support animation, this function jumps to the image
   whose sequence number is \a imageNumber. The next call to read() will
   attempt to read this image.

   The default implementation does nothing, and returns false.
*/
bool QImageIOHandler::jumpToImage(int imageNumber)
{
    Q_UNUSED(imageNumber);
    return false;
}

/*!
    For image formats that support animation, this function returns
    the number of times the animation should loop. If the image format
    does not support animation, 0 is returned.
*/
int QImageIOHandler::loopCount() const
{
    return 0;
}

/*!
    For image formats that support animation, this function returns
    the number of milliseconds to wait until reading the next
    image. If the image format does not support animation, 0 is
    returned.
*/
int QImageIOHandler::nextImageDelay() const
{
    return 0;
}

/*!
    Constructs an image plugin with the given \a parent. This is
    invoked automatically by the Q_EXPORT_PLUGIN2() macro.
*/
QImageIOPlugin::QImageIOPlugin(QObject *parent)
    : QObject(parent)
{
}

/*!
    Destroys the picture format plugin.

    You never have to call this explicitly. Qt destroys a plugin
    automatically when it is no longer used.
*/
QImageIOPlugin::~QImageIOPlugin()
{
}

/*! \fn QImageIOPlugin::capabilities(QIODevice *device, const QByteArray &format) const

    Returns the capabilities on the plugin, based on the data in \a
    device and the format \a format. For example, if the
    QImageIOHandler supports the PNG format, and the data in the
    device starts with the magick bits for that format, this function
    should return \l CanRead. If \a format is "png" and the handler
    supports both reading and writing, this function should return
    \l CanRead | \l CanWrite.
*/

/*!
    \fn QImageIOPlugin::mimeTypes() const

    Returns the list of image MIME types this plugin supports.

    These MIME types are usually the names of the image formats that are implemented
    in the plugin prefixed with "image/" (e.g., "image/png").

    \sa capabilities()
*/

/*!
    \fn QImageIOHandler *QImageIOPlugin::create(QIODevice *device, const QByteArray &format) const

    Creates and returns a QImageIOHandler subclass, with \a device
    and \a format set. The \a format is a hint. Format names are case sensitive.
*/

QT_END_NAMESPACE

#include "moc_qimageiohandler.h"


