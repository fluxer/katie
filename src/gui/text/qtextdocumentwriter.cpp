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
#include "qtextdocumentwriter.h"

#include <QtCore/qfile.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qtextcodec.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qdebug.h>
#include "qtextdocument.h"
#include "qtextdocumentfragment.h"
#include "qtextdocumentfragment_p.h"

QT_BEGIN_NAMESPACE

class QTextDocumentWriterPrivate
{
public:
    QTextDocumentWriterPrivate();

    // device
    QByteArray format;
    QIODevice *device;
    bool deleteDevice;
#ifndef QT_NO_TEXTCODEC
    QTextCodec *codec;
#endif
};

/*!
    \since 4.5
    \class QTextDocumentWriter

    \brief The QTextDocumentWriter class provides a format-independent interface for writing a QTextDocument to files or other devices.

    \ingroup richtext-processing
    \ingroup io

    To write a document, construct a QTextDocumentWriter object with either a
    file name or a device object, and specify the document format to be
    written. You can construct a writer and set the format using setFormat()
    later.

    Call write() to write the document to the device. If the document is
    successfully written, this function returns true. However, if an error
    occurs when writing the document, it will return false.

    Call supportedDocumentFormats() for a list of formats that
    QTextDocumentWriter can write.

    Since the capabilities of the supported output formats vary considerably,
    the writer simply outputs the appropriate subset of objects for each format.
    This typically includes the formatted text and images contained in a
    document.
*/

/*!
    \internal
*/
QTextDocumentWriterPrivate::QTextDocumentWriterPrivate()
    : device(0),
    deleteDevice(false),
#ifndef QT_NO_TEXTCODEC
    codec(QTextCodec::codecForName("utf-8"))
#endif
{
}

/*!
    Constructs an empty QTextDocumentWriter object. Before writing, you must
    call setFormat() to set a document format, then setDevice() or
    setFileName().
*/
QTextDocumentWriter::QTextDocumentWriter()
    : d(new QTextDocumentWriterPrivate())
{
}

/*!
    Constructs a QTextDocumentWriter object to write to the given \a device
    in the document format specified by \a format.
*/
QTextDocumentWriter::QTextDocumentWriter(QIODevice *device, const QByteArray &format)
    : d(new QTextDocumentWriterPrivate())
{
    d->device = device;
    d->format = format;
}

/*!
    Constructs an QTextDocumentWriter object that will write to a file with
    the name \a fileName, using the document format specified by \a format.
    If \a format is not provided, QTextDocumentWriter will detect the document
    format by inspecting the extension of \a fileName.
*/
QTextDocumentWriter::QTextDocumentWriter(const QString &fileName, const QByteArray &format)
    : d(new QTextDocumentWriterPrivate())
{
    QFile *file = new QFile(fileName);
    d->device = file;
    d->deleteDevice = true;
    d->format = format;
}

/*!
    Destroys the QTextDocumentWriter object.
*/
QTextDocumentWriter::~QTextDocumentWriter()
{
    if (d->deleteDevice)
        delete d->device;
    delete d;
}

/*!
    Sets the format used to write documents to the \a format specified.
    \a format is a case insensitive text string. For example:

    \snippet doc/src/snippets/code/src.gui.text.qtextdocumentwriter.cpp 0

    You can call supportedDocumentFormats() for the full list of formats
    QTextDocumentWriter supports.

    \sa format()
*/
void QTextDocumentWriter::setFormat(const QByteArray &format)
{
    d->format = format;
}

/*!
    Returns the format used for writing documents.

    \sa setFormat()
*/
QByteArray QTextDocumentWriter::format() const
{
    return d->format;
}

/*!
    Sets the writer's device to the \a device specified. If a device has
    already been set, the old device is removed but otherwise left
    unchanged.

    If the device is not already open, QTextDocumentWriter will attempt to
    open the device in \l QIODevice::WriteOnly mode by calling open().

    \note This will not work for certain devices, such as QProcess,
    QTcpSocket and QUdpSocket, where some configuration is required before
    the device can be opened.

    \sa device(), setFileName()
*/
void QTextDocumentWriter::setDevice(QIODevice *device)
{
    if (d->device && d->deleteDevice)
        delete d->device;

    d->device = device;
    d->deleteDevice = false;
}

/*!
    Returns the device currently assigned, or 0 if no device has been
    assigned.
*/
QIODevice *QTextDocumentWriter::device() const
{
    return d->device;
}

/*!
    Sets the name of the file to be written to \a fileName. Internally,
    QTextDocumentWriter will create a QFile and open it in \l
    QIODevice::WriteOnly mode, and use this file when writing the document.

    \sa fileName(), setDevice()
*/
void QTextDocumentWriter::setFileName(const QString &fileName)
{
    setDevice(new QFile(fileName));
    d->deleteDevice = true;
}

/*!
    If the currently assigned device is a QFile, or if setFileName()
    has been called, this function returns the name of the file
    to be written to. In all other cases, it returns an empty string.

    \sa setFileName(), setDevice()
*/
QString QTextDocumentWriter::fileName() const
{
    QFile *file = qobject_cast<QFile *>(d->device);
    return file ? file->fileName() : QString();
}

/*!
    Writes the given \a document to the assigned device or file and
    returns true if successful; otherwise returns false.
*/
bool QTextDocumentWriter::write(const QTextDocument *document)
{
    QByteArray suffix;

    if (d->device && d->format.isEmpty()) {
        // if there's no format, see if device is a file, and if so, find
        // the file suffix
        if (QFile *file = qobject_cast<QFile *>(d->device))
            suffix = QFileInfo(file->fileName()).suffix().toLower().toLatin1();
    }

    QByteArray format = !d->format.isEmpty() ? d->format.toLower() : suffix;

#ifndef QT_NO_TEXTHTMLPARSER
    if (format == "html" || format == "htm") {
        if (!d->device->isWritable() && ! d->device->open(QIODevice::WriteOnly)) {
            qWarning() << "QTextDocumentWriter::write: the device can not be opened for writing";
            return false;
        }
        QTextStream ts(d->device);
#ifndef QT_NO_TEXTCODEC
        ts.setCodec(d->codec);
        ts << document->toHtml(d->codec->name());
#endif
        d->device->close();
        return true;
    }
#endif
    if (format == "txt" || format == "plaintext") {
        if (!d->device->isWritable() && ! d->device->open(QIODevice::WriteOnly)) {
            qWarning() << "QTextDocumentWriter::write: the device can not be opened for writing";
            return false;
        }
        QTextStream ts(d->device);
#ifndef QT_NO_TEXTCODEC
        ts.setCodec(d->codec);
#endif
        ts << document->toPlainText();
        d->device->close();
        return true;
    }

    return false;
}

/*!
    Writes the document fragment specified by \a fragment to the assigned device
    or file and returns true if successful; otherwise returns false.
*/
bool QTextDocumentWriter::write(const QTextDocumentFragment &fragment)
{
    if (!fragment.d)
        return false; // invalid fragment.
    QTextDocument *doc = fragment.d->doc;
    if (doc)
        return write(doc);
    return false;
}

/*!
    Sets the codec for this stream to \a codec. The codec is used for
    encoding any data that is written. By default, QTextDocumentWriter
    uses UTF-8.
*/

#ifndef QT_NO_TEXTCODEC
void QTextDocumentWriter::setCodec(QTextCodec *codec)
{
    if (!codec)
        codec = QTextCodec::codecForName("UTF-8");
    Q_ASSERT(codec);
    d->codec = codec;
}
#endif

/*!
    Returns the codec that is currently assigned to the writer.
*/
#ifndef QT_NO_TEXTCODEC
QTextCodec *QTextDocumentWriter::codec() const
{
    return d->codec;
}
#endif

/*!
    Returns the list of document formats supported by QTextDocumentWriter.

    By default, Qt can write the following formats:

    \table
    \header \o Format    \o Description
    \row    \o HTML      \o HyperText Markup Language
    \row    \o plaintext \o Plain text
    \endtable

    \sa setFormat()
*/
QList<QByteArray> QTextDocumentWriter::supportedDocumentFormats()
{
    static const QList<QByteArray> answer = QList<QByteArray>()
#ifndef QT_NO_TEXTHTMLPARSER
        << "HTML"
#endif
        << "plaintext";
    return answer;
}

QT_END_NAMESPACE




