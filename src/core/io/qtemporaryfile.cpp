/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#include "qtemporaryfile.h"

#ifndef QT_NO_TEMPORARYFILE

#include "qplatformdefs.h"
#include "qdir.h"
#include "qfile_p.h"
#include "qfilesystemengine_p.h"
#include "qdebug.h"
#include "qcore_unix_p.h"
#include "qcorecommon_p.h"

#include <errno.h>

QT_BEGIN_NAMESPACE

//************* QTemporaryFilePrivate
class QTemporaryFilePrivate : public QFilePrivate
{
    Q_DECLARE_PUBLIC(QTemporaryFile)

protected:
    QTemporaryFilePrivate();

    bool autoRemove;
    QString templateName;
};

QTemporaryFilePrivate::QTemporaryFilePrivate()
    : autoRemove(true),
    templateName(QDir::tempPath() + QLatin1String("/qt_temp.XXXXXXXXXX"))
{
}

//************* QTemporaryFile

/*!
    \class QTemporaryFile
    \reentrant
    \brief The QTemporaryFile class is an I/O device that operates on temporary files.

    \ingroup io


    QTemporaryFile is used to create unique temporary files safely.
    The file itself is created by calling open(). The name of the
    temporary file is guaranteed to be unique (i.e., you are
    guaranteed to not overwrite an existing file), and the file will
    subsequently be removed upon destruction of the QTemporaryFile
    object. This is an important technique that avoids data
    corruption for applications that store data in temporary files.
    The file name is either auto-generated, or created based on a
    template, which is passed to QTemporaryFile's constructor.

    Example:

    \snippet doc/src/snippets/code/src_corelib_io_qtemporaryfile.cpp 0

    Reopening a QTemporaryFile after calling close() is safe. For as long as
    the QTemporaryFile object itself is not destroyed, the unique temporary
    file will exist and be kept open internally by QTemporaryFile.

    The file name of the temporary file can be found by calling fileName().
    Note that this is only defined after the file is first opened; the function
    returns an empty string before this.

    A temporary file will have some static part of the name and some
    part that is calculated to be unique. The default filename \c
    qt_temp will be placed into the temporary path as returned by
    QDir::tempPath(). If you specify your own filename, a relative
    file path will not be placed in the temporary directory by
    default, but be relative to the current working directory.

    Specified filenames can contain the following template \c XXXXXXXXXX
    (six upper case "X" characters), which will be replaced by the
    auto-generated portion of the filename. Note that the template is
    case sensitive. If the template is not present in the filename,
    QTemporaryFile appends the generated part to the filename given.

    \sa QDir::tempPath(), QFile
*/

#ifdef QT_NO_QOBJECT
QTemporaryFile::QTemporaryFile()
    : QFile(*new QTemporaryFilePrivate)
{
}

QTemporaryFile::QTemporaryFile(const QString &templateName)
    : QFile(*new QTemporaryFilePrivate)
{
    Q_D(QTemporaryFile);
    d->templateName = templateName;
}

#else
/*!
    Constructs a QTemporaryFile in QDir::tempPath(), using the file template
    "qt_temp.XXXXXXXXXX". The file is stored in the system's temporary directory.

    \sa setFileTemplate(), QDir::tempPath()
*/
QTemporaryFile::QTemporaryFile()
    : QFile(*new QTemporaryFilePrivate, 0)
{
}

/*!
    Constructs a QTemporaryFile with a template filename of \a
    templateName. Upon opening the temporary file this will be used to create
    a unique filename.

    If the \a templateName does not contain XXXXXXXXXX it will automatically be
    appended and used as the dynamic portion of the filename.

    If \a templateName is a relative path, the path will be relative to the
    current working directory. You can use QDir::tempPath() to construct \a
    templateName if you want use the system's temporary directory.

    \sa open(), fileTemplate()
*/
QTemporaryFile::QTemporaryFile(const QString &templateName)
    : QFile(*new QTemporaryFilePrivate, 0)
{
    Q_D(QTemporaryFile);
    d->templateName = templateName;
}

/*!
    Constructs a QTemporaryFile (with the given \a parent) in
    QDir::tempPath(), using the file template "qt_temp.XXXXXXXXXX".

    \sa setFileTemplate()
*/
QTemporaryFile::QTemporaryFile(QObject *parent)
    : QFile(*new QTemporaryFilePrivate, parent)
{
}

/*!
    Constructs a QTemporaryFile with a template filename of \a
    templateName and the specified \a parent.
    Upon opening the temporary file this will be used to
    create a unique filename.

    If the \a templateName does not contain XXXXXXXXXX it will automatically
    be appended and used as the dynamic portion of the filename.

    If \a templateName is a relative path, the path will be relative to the
    current working directory. You can use QDir::tempPath() to construct \a
    templateName if you want use the system's temporary directory.

    \sa open(), fileTemplate()
*/
QTemporaryFile::QTemporaryFile(const QString &templateName, QObject *parent)
    : QFile(*new QTemporaryFilePrivate, parent)
{
    Q_D(QTemporaryFile);
    d->templateName = templateName;
}
#endif

/*!
    Destroys the temporary file object, the file is automatically
    closed if necessary and if in auto remove mode it will
    automatically delete the file.

    \sa autoRemove()
*/
QTemporaryFile::~QTemporaryFile()
{
    Q_D(QTemporaryFile);
    close();
    if (!d->fileEntry.isEmpty() && d->autoRemove)
        remove();
}

/*!
    A QTemporaryFile will always be opened in QIODevice::ReadWrite mode,
    this allows easy access to the data in the file. This function will
    return true upon success and will set the fileName() to the unique
    filename used.

    \sa fileName()
*/
bool QTemporaryFile::open()
{
    Q_D(QTemporaryFile);
    if (Q_UNLIKELY(isOpen())) {
        qWarning("QTemporaryFile::open: File (%s) already open", qPrintable(fileName()));
        return false;
    }

    // "Nativify" :-)
    QFileSystemEntry::NativePath filename =
        QFileSystemEngine::absoluteName(QFileSystemEntry(d->templateName)).nativeFilePath();

    // Find placeholder in native path
    uint phPos = filename.length();
    uint phLength = 0;
    while (phPos != 0) {
        --phPos;

        if (filename.at(phPos) == 'X') {
            ++phLength;
            continue;
        }

        // require atleast 6 for compatibility
        if (phLength >= 6 || filename.at(phPos) == '/') {
            ++phPos;
            break;
        }

        // start over
        phLength = 0;
    }

    // Ensure there is a placeholder mask
    if (phLength < 6) {
        filename.append(".XXXXXXXXXX");
        phPos = filename.length() - 10;
        phLength = 10;
    }

    Q_ASSERT(phLength >= 6);
    Q_ASSERT(phPos < filename.length());
    Q_ASSERT(phLength <= filename.length() - phPos);

    static const char tmpnamechars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    char *data = filename.data();
    for (uint i = 0; i < phLength; i++) {
        data[i + phPos] = tmpnamechars[qrand() % 52];
    }

    // Create file and obtain handle
    d->fd = qt_safe_open(data, QT_OPEN_CREAT | O_EXCL | QT_OPEN_RDWR, 0600);

    if (d->fd == -1) {
        d->setError(QFile::OpenError, qt_error_string(errno));
        return false;
    }

    d->fileEntry = QFileSystemEntry(filename, QFileSystemEntry::FromNativePath());
    d->closeFileHandle = true;
    d->metaData.clear();

    QIODevice::open(QIODevice::ReadWrite);

    return true;
}

/*!
    Returns true if the QTemporaryFile is in auto remove
    mode. Auto-remove mode will automatically delete the filename from
    disk upon destruction. This makes it very easy to create your
    QTemporaryFile object on the stack, fill it with data, read from
    it, and finally on function return it will automatically clean up
    after itself.

    Auto-remove is on by default.

    \sa setAutoRemove(), remove()
*/
bool QTemporaryFile::autoRemove() const
{
    Q_D(const QTemporaryFile);
    return d->autoRemove;
}

/*!
    Sets the QTemporaryFile into auto-remove mode if \a b is true.

    Auto-remove is on by default.

    \sa autoRemove(), remove()
*/
void QTemporaryFile::setAutoRemove(bool b)
{
    Q_D(QTemporaryFile);
    d->autoRemove = b;
}

/*!
  Returns the set file template. The default file template will be
  called qt_temp and be placed in QDir::tempPath().

  \sa setFileTemplate()
*/
QString QTemporaryFile::fileTemplate() const
{
    Q_D(const QTemporaryFile);
    return d->templateName;
}

/*!
    Sets the static portion of the file name to \a name. If the file
    template ends in XXXXXXXXXX that will automatically be replaced with
    the unique part of the filename, otherwise a filename will be
    determined automatically based on the static portion specified.

    If \a name contains a relative file path, the path will be relative to the
    current working directory. You can use QDir::tempPath() to construct \a
    name if you want use the system's temporary directory.

   \sa fileTemplate()
*/
void QTemporaryFile::setFileTemplate(const QString &name)
{
    Q_D(QTemporaryFile);
    d->templateName = name;
}

#ifndef QT_NO_QOBJECT
#include "moc_qtemporaryfile.h"
#endif

QT_END_NAMESPACE

#endif // QT_NO_TEMPORARYFILE


