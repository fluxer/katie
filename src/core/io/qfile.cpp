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

#include "qplatformdefs.h"
#include "qdebug.h"
#include "qfile.h"
#include "qtemporaryfile.h"
#include "qlist.h"
#include "qfileinfo.h"
#include "qiodevice_p.h"
#include "qfile_p.h"
#include "qfilesystemengine_p.h"
#include "qcorecommon_p.h"
#include "qcore_unix_p.h"

#ifdef QT_NO_QOBJECT
#define tr(X) QString::fromLatin1(X)
#endif

QT_BEGIN_NAMESPACE

//************* QFilePrivate
QFilePrivate::QFilePrivate()
    : error(QFile::NoError),
    fd(-1),
    closeFileHandle(false)
{
}

QFilePrivate::~QFilePrivate()
{
}

bool QFilePrivate::doStat(QFileSystemMetaData::MetaDataFlags flags) const
{
    if (!metaData.hasFlags(flags)) {
        if (!fileEntry.isEmpty())
            QFileSystemEngine::fillMetaData(fileEntry, metaData, metaData.missingFlags(flags));

        if (metaData.missingFlags(flags) && fd != -1)
            QFileSystemEngine::fillMetaData(fd, metaData);
    }

    return metaData.exists();
}

bool QFilePrivate::openExternalFile(QIODevice::OpenMode mode, int _fd, QFile::FileHandleFlags handleFlags)
{
    // Append implies WriteOnly.
    if (mode & QFile::Append)
        mode |= QFile::WriteOnly;

    // WriteOnly implies Truncate if neither ReadOnly nor Append are sent.
    if ((mode & QFile::WriteOnly) && !(mode & (QFile::ReadOnly | QFile::Append)))
        mode |= QFile::Truncate;

    closeFileHandle = (handleFlags & QFile::AutoCloseHandle);
    fileEntry.clear();
    fd = _fd;
    metaData.clear();

    // Seek to the end when in Append mode.
    if (mode & QFile::Append) {
        if (QT_LSEEK(fd, 0, SEEK_END) == -1) {
            setError(errno == EMFILE ? QFile::ResourceError : QFile::OpenError,
                     qt_error_string(errno));
            if (closeFileHandle) {
                qt_safe_close(fd);
            }
            fd = -1;
            return false;
        }
    }

    return true;
}

void QFilePrivate::setError(QFile::FileError err, const QString &errStr)
{
    error = err;
    errorString = errStr;
}

//************* QFile

/*!
    \class QFile
    \brief The QFile class provides an interface for reading from and writing to files.

    \ingroup io

    \reentrant

    QFile is an I/O device for reading and writing text and binary
    files and \l{The Qt Resource System}{resources}. A QFile may be
    used by itself or, more conveniently, with a QTextStream or
    QDataStream.

    The file name is usually passed in the constructor, but it can be
    set at any time using setFileName(). QFile expects the file
    separator to be '/' regardless of operating system. The use of
    other separators (e.g., '\\') is not supported.

    You can check for a file's existence using exists(), and remove a
    file using remove(). (More advanced file system related operations
    are provided by QFileInfo and QDir.)

    The file is opened with open(), closed with close(), and flushed
    with flush(). Data is usually read and written using QDataStream
    or QTextStream, but you can also call the QIODevice-inherited
    functions read(), readLine(), readAll(), write(). QFile also
    inherits getChar(), putChar(), and ungetChar(), which work one
    character at a time.

    The size of the file is returned by size(). You can get the
    current file position using pos(), or move to a new file position
    using seek(). If you've reached the end of the file, atEnd()
    returns true.

    \section1 Reading Files Directly

    The following example reads a text file line by line:

    \snippet doc/src/snippets/file/file.cpp 0

    The QIODevice::Text flag passed to open() tells Qt to convert
    Windows-style line terminators ("\\r\\n") into C++-style
    terminators ("\\n"). By default, QFile assumes binary, i.e. it
    doesn't perform any conversion on the bytes stored in the file.

    \section1 Using Streams to Read Files

    The next example uses QTextStream to read a text file
    line by line:

    \snippet doc/src/snippets/file/file.cpp 1

    QTextStream takes care of converting the 8-bit data stored on
    disk into a 16-bit Unicode QString. By default, it assumes that
    the user system's local 8-bit encoding is used (e.g., ISO 8859-1
    for most of Europe; see QTextCodec::codecForLocale() for
    details). This can be changed using setCodec().

    To write text, we can use operator<<(), which is overloaded to
    take a QTextStream on the left and various data types (including
    QString) on the right:

    \snippet doc/src/snippets/file/file.cpp 2

    QDataStream is similar, in that you can use operator<<() to write
    data and operator>>() to read it back. See the class
    documentation for details.

    When you use QFile, QFileInfo, and QDir to access the file system
    with Qt, you can use Unicode file names. On Unix, these file
    names are converted to an 8-bit encoding. If you want to use
    standard C++ APIs (\c <cstdio> or \c <iostream>) or
    platform-specific APIs to access files instead of QFile, you can
    use the encodeName() and decodeName() functions to convert
    between Unicode file names and 8-bit file names.

    On Unix, there are some special system files (e.g. in \c /proc) for which
    size() will always return 0, yet you may still be able to read more data
    from such a file; the data is generated in direct response to you calling
    read(). In this case, however, you cannot use atEnd() to determine if
    there is more data to read (since atEnd() will return true for a file that
    claims to have size 0). Instead, you should either call readAll(), or call
    read() or readLine() repeatedly until no more data can be read. The next
    example uses QTextStream to read \c /proc/modules line by line:

    \snippet doc/src/snippets/file/file.cpp 3

    \section1 Signals

    Unlike other QIODevice implementations, such as QTcpSocket, QFile does not
    emit the aboutToClose(), bytesWritten(), or readyRead() signals. This
    implementation detail means that QFile is not suitable for reading and
    writing certain types of files, such as device files on Unix platforms.

    \section1 Platform Specific Issues

    File permissions are handled differently on Linux/Mac OS X and
    Windows.  In a non \l{QIODevice::isWritable()}{writable}
    directory on Linux, files cannot be created. This is not always
    the case on Windows, where, for instance, the 'My Documents'
    directory usually is not writable, but it is still possible to
    create files in it.

    \sa QTextStream, QDataStream, QFileInfo, QDir, {The Qt Resource System}
*/

/*!
    \enum QFile::FileError

    This enum describes the errors that may be returned by the error()
    function.

    \value NoError          No error occurred.
    \value ReadError        An error occurred when reading from the file.
    \value WriteError       An error occurred when writing to the file.
    \value FatalError       A fatal error occurred.
    \value ResourceError
    \value OpenError        The file could not be opened.
    \value AbortError       The operation was aborted.
    \value TimeOutError     A timeout occurred.
    \value UnspecifiedError An unspecified error occurred.
    \value RemoveError      The file could not be removed.
    \value RenameError      The file could not be renamed.
    \value PositionError    The position in the file could not be changed.
    \value ResizeError      The file could not be resized.
    \value PermissionsError The file could not be accessed.
    \value CopyError        The file could not be copied.

    \omitvalue ConnectError
*/

/*!
    \enum QFile::Permission

    This enum is used by the permission() function to report the
    permissions and ownership of a file. The values may be OR-ed
    together to test multiple permissions and ownership values.

    \value ReadOwner The file is readable by the owner of the file.
    \value WriteOwner The file is writable by the owner of the file.
    \value ExeOwner The file is executable by the owner of the file.
    \value ReadUser The file is readable by the user.
    \value WriteUser The file is writable by the user.
    \value ExeUser The file is executable by the user.
    \value ReadGroup The file is readable by the group.
    \value WriteGroup The file is writable by the group.
    \value ExeGroup The file is executable by the group.
    \value ReadOther The file is readable by anyone.
    \value WriteOther The file is writable by anyone.
    \value ExeOther The file is executable by anyone.

    \warning Because of differences in the platforms supported by Qt,
    the semantics of ReadUser, WriteUser and ExeUser are
    platform-dependent: On Unix, the rights of the owner of the file
    are returned and on Windows the rights of the current user are
    returned. This behavior might change in a future Qt version.

    Note that Qt does not by default check for permissions on NTFS
    file systems, as this may decrease the performance of file
    handling considerably. It is possible to force permission checking
    on NTFS by including the following code in your source:

    \snippet doc/src/snippets/ntfsp.cpp 0

    Permission checking is then turned on and off by incrementing and
    decrementing \c qt_ntfs_permission_lookup by 1.

    \snippet doc/src/snippets/ntfsp.cpp 1
*/

/*!
    \enum QFile::FileHandleFlag
    \since 4.8

    This enum is used when opening a file to specify additional
    options which only apply to files and not to a generic
    QIODevice.

    \value AutoCloseHandle The file handle passed into open() should be
    closed by close(), the default behaviour is that close just flushes
    the file and the application is responsible for closing the file handle.
    When opening a file by name, this flag is ignored as Qt always "owns" the
    file handle and must close it.
    \value DontCloseHandle The file handle passed into open() will not be
    closed by Qt. The application must ensure that close() is called.
 */


#ifdef QT_NO_QOBJECT
QFile::QFile()
    : QIODevice(*new QFilePrivate)
{
}
QFile::QFile(const QString &name)
    : QIODevice(*new QFilePrivate)
{
    d_func()->fileEntry = QFileSystemEntry(name);
}
QFile::QFile(QFilePrivate &dd)
    : QIODevice(dd)
{
}
#else
/*!
    \internal
*/
QFile::QFile()
    : QIODevice(*new QFilePrivate, 0)
{
}
/*!
    Constructs a new file object with the given \a parent.
*/
QFile::QFile(QObject *parent)
    : QIODevice(*new QFilePrivate, parent)
{
}

/*!
    Constructs a new file object to represent the file with the given \a name.
*/
QFile::QFile(const QString &name)
    : QIODevice(*new QFilePrivate, 0)
{
    Q_D(QFile);
    d->fileEntry = QFileSystemEntry(name);
}

/*!
    Constructs a new file object with the given \a parent to represent the
    file with the specified \a name.
*/
QFile::QFile(const QString &name, QObject *parent)
    : QIODevice(*new QFilePrivate, parent)
{
    Q_D(QFile);
    d->fileEntry = QFileSystemEntry(name);
}

/*!
    \internal
*/
QFile::QFile(QFilePrivate &dd, QObject *parent)
    : QIODevice(dd, parent)
{
}
#endif

/*!
    Destroys the file object, closing it if necessary.
*/
QFile::~QFile()
{
    close();
}

/*!
    Returns the name set by setFileName() or to the QFile
    constructors.

    \sa setFileName(), QFileInfo::fileName()
*/
QString QFile::fileName() const
{
    Q_D(const QFile);
    return d->fileEntry.filePath();
}

/*!
    Sets the \a name of the file. The name can have no path, a
    relative path, or an absolute path.

    Do not call this function if the file has already been opened.

    If the file name has no path or a relative path, the path used
    will be the application's current directory path
    \e{at the time of the open()} call.

    Example:
    \snippet doc/src/snippets/code/src_corelib_io_qfile.cpp 0

    Note that the directory separator "/" works for all operating
    systems supported by Qt.

    \sa fileName(), QFileInfo, QDir
*/
void QFile::setFileName(const QString &name)
{
    Q_D(QFile);
    if (Q_UNLIKELY(isOpen())) {
        qWarning("QFile::setFileName: File (%s) is already opened",
                 qPrintable(fileName()));
    }
    close();
    d->fileEntry = QFileSystemEntry(name);
}

/*!
    This function converts \a fileName to the local 8-bit encoding
    determined by the user's locale. This is sufficient forfile names
    that the user chooses. File names hard-coded into the
    application should only use 7-bit ASCII filename characters.

    \sa decodeName()
*/

QByteArray QFile::encodeName(const QString &fileName)
{
    return fileName.toLocal8Bit();
}

/*!
    This does the reverse of QFile::encodeName() using \a localFileName.

    \sa setDecodingFunction(), encodeName()
*/
QString QFile::decodeName(const QByteArray &localFileName)
{
    return QString::fromLocal8Bit(localFileName.constData());
}

/*!
    \fn QString QFile::decodeName(const char *localFileName)

    \overload

    Returns the Unicode version of the given \a localFileName. See
    encodeName() for details.
*/

/*!
    \overload

    Returns true if the file specified by fileName() exists; otherwise
    returns false.

    \sa fileName(), setFileName()
*/
bool QFile::exists() const
{
    Q_D(const QFile);
    d->metaData.clear(); // always stat
    if (!d->doStat(QFileSystemMetaData::ExistsAttribute | QFileSystemMetaData::FileType))
        return false;
    return d->metaData.exists() && d->metaData.isFile();
}

/*!
    Returns true if the file specified by \a fileName exists; otherwise
    returns false.
*/
bool QFile::exists(const QString &fileName)
{
    QFileInfo info(fileName);
    return (info.exists() && info.isFile());
}

/*!
    \fn QString QFile::readLink() const
    \since 4.2
    \overload

    Returns the absolute path of the file or directory a symlink points to,
    or a an empty string if the object isn't a symbolic link.

    This name may not represent an existing file; it is only a string.
    QFile::exists() returns true if the symlink points to an existing file.

    \sa fileName() setFileName()
*/
QString QFile::readLink() const
{
    Q_D(const QFile);
    if (!d->metaData.hasFlags(QFileSystemMetaData::LinkType))
        QFileSystemEngine::fillMetaData(d->fileEntry, d->metaData, QFileSystemMetaData::LinkType);
    if (d->metaData.isLink()) {
        QFileSystemEntry entry = QFileSystemEngine::getLinkTarget(d->fileEntry, d->metaData);
        return entry.filePath();
    }
    return QString();
}

/*!
    \since 4.2

    Returns the absolute path of the file or directory referred to by the
    symlink (or shortcut on Windows) specified by \a fileName, or returns an
    empty string if the \a fileName does not correspond to a symbolic link.

    This name may not represent an existing file; it is only a string.
    QFile::exists() returns true if the symlink points to an existing file.
*/
QString QFile::readLink(const QString &fileName)
{
    QFileSystemMetaData metaData;
    QFileSystemEntry fileEntry(fileName);
    QFileSystemEngine::fillMetaData(fileEntry, metaData, QFileSystemMetaData::LinkType);
    if (metaData.isLink()) {
        QFileSystemEntry entry = QFileSystemEngine::getLinkTarget(fileEntry, metaData);
        return entry.filePath();
    }
    return QString();
}

/*!
    Removes the file specified by fileName(). Returns true if successful;
    otherwise returns false.

    The file is closed before it is removed.

    \sa setFileName()
*/
bool QFile::remove()
{
    Q_D(QFile);
    if (Q_UNLIKELY(d->fileEntry.isEmpty())) {
        qWarning("QFile::remove: Empty or null file name");
        return false;
    }
    d->setError(QFile::NoError, QString());
    close();
    if(error() == QFile::NoError) {
        int error = 0;
        bool ret = QFileSystemEngine::removeFile(d->fileEntry, &error);
        d->metaData.clear();
        if (!ret) {
            d->setError(QFile::RemoveError, qt_error_string(error));
            return false;
        }
        d->setError(QFile::NoError, QString());
        return true;
    }
    return false;
}

/*!
    \overload

    Removes the file specified by the \a fileName given.

    Returns true if successful; otherwise returns false.

    \sa remove()
*/
bool QFile::remove(const QString &fileName)
{
    int error = 0;
    return QFileSystemEngine::removeFile(QFileSystemEntry(fileName), &error);
}

/*!
    Renames the file currently specified by fileName() to \a newName.
    Returns true if successful; otherwise returns false.

    If a file with the name \a newName already exists, rename() returns false
    (i.e., QFile will not overwrite it).

    The file is closed before it is renamed.

    \sa setFileName()
*/
bool QFile::rename(const QString &newName)
{
    Q_D(QFile);
    if (Q_UNLIKELY(d->fileEntry.isEmpty())) {
        qWarning("QFile::rename: Empty or null file name");
        return false;
    }
#ifndef QT_HAVE_RENAMEAT2
    if (QFile::exists(newName)) {
        // ### Race condition. If a file is moved in after this, it /will/ be
        // overwritten. On Unix, the proper solution is to use hardlinks:
        // return ::link(old, new) && ::remove(old);
        d->setError(QFile::RenameError, tr("Destination file exists"));
        return false;
    }
#endif
    d->setError(QFile::NoError, QString());
    close();
    if(error() == QFile::NoError) {
        int error = 0;
        bool ret = QFileSystemEngine::renameFile(d->fileEntry, QFileSystemEntry(newName), &error);
        d->metaData.clear();
        if (!ret) {
            d->setError(QFile::RenameError, qt_error_string(error));
            return false;
        }

        d->setError(QFile::NoError, QString());
        // just reset it
        d->metaData.clear();
        d->fd = -1;
        d->closeFileHandle = false;
        d->fileEntry = QFileSystemEntry(newName);
        return true;
    }
    return false;
}

/*!
    \overload

    Renames the file \a oldName to \a newName. Returns true if
    successful; otherwise returns false.

    If a file with the name \a newName already exists, rename() returns false
    (i.e., QFile will not overwrite it).

    \sa rename()
*/
bool QFile::rename(const QString &oldName, const QString &newName)
{
#ifndef QT_HAVE_RENAMEAT2
    if (QFile::exists(newName)) {
        return false;
    }
#endif
    int error = 0;
    return QFileSystemEngine::renameFile(QFileSystemEntry(oldName), QFileSystemEntry(newName), &error);
}

/*!

    Creates a link named \a linkName that points to the file currently specified by
    fileName().  What a link is depends on the underlying filesystem (be it a
    shortcut on Windows or a symbolic link on Unix). Returns true if successful;
    otherwise returns false.

    This function will not overwrite an already existing entity in the file system;
    in this case, \c link() will return false and set \l{QFile::}{error()} to
    return \l{QFile::}{RenameError}.

    \note To create a valid link on Windows, \a linkName must have a \c{.lnk} file extension.

    \sa setFileName()
*/
bool QFile::link(const QString &linkName)
{
    Q_D(QFile);
    if (Q_UNLIKELY(d->fileEntry.isEmpty())) {
        qWarning("QFile::link: Empty or null file name");
        return false;
    }
    QFileInfo fi(linkName);
    int error = 0;
    bool ret = QFileSystemEngine::createLink(d->fileEntry, QFileSystemEntry(fi.absoluteFilePath()), &error);
    if (!ret) {
        d->setError(QFile::RenameError, qt_error_string(error));
        return false;
    }
    d->setError(QFile::NoError, QString());
    return true;
}

/*!
    \overload

    Creates a link named \a linkName that points to the file \a fileName. What a link is
    depends on the underlying filesystem (be it a shortcut on Windows
    or a symbolic link on Unix). Returns true if successful; otherwise
    returns false.

    \sa link()
*/
bool QFile::link(const QString &fileName, const QString &linkName)
{
    int error = 0;
    return QFileSystemEngine::createLink(QFileSystemEntry(fileName), QFileSystemEntry(linkName), &error);
}

/*!
    Copies the file currently specified by fileName() to a file called
    \a newName.  Returns true if successful; otherwise returns false.

    Note that if a file with the name \a newName already exists,
    copy() returns false (i.e. QFile will not overwrite it).

    The source file is closed before it is copied.

    \sa setFileName()
*/
bool QFile::copy(const QString &newName)
{
    Q_D(QFile);
    if (Q_UNLIKELY(d->fileEntry.isEmpty())) {
        qWarning("QFile::copy: Empty or null file name");
        return false;
    }

    d->setError(QFile::NoError, QString());
    close();
    if(error() == QFile::NoError) {
        int error = 0;
        if(QFileSystemEngine::copyFile(d->fileEntry, QFileSystemEntry(newName), &error)) {
            d->setError(QFile::NoError, QString());
            return true;
        }
        d->setError(QFile::CopyError, qt_error_string(error));
    }
    return false;
}

/*!
    \overload

    Copies the file \a fileName to \a newName. Returns true if successful;
    otherwise returns false.

    If a file with the name \a newName already exists, copy() returns false
    (i.e., QFile will not overwrite it).

    \sa rename()
*/
bool QFile::copy(const QString &fileName, const QString &newName)
{
    int error = 0;
    return QFileSystemEngine::copyFile(QFileSystemEntry(fileName), QFileSystemEntry(newName), &error);
}

/*!
    Returns true if the file can only be manipulated sequentially;
    otherwise returns false.

    Most files support random-access, but some special files may not.

    \sa QIODevice::isSequential()
*/
bool QFile::isSequential() const
{
    Q_D(const QFile);
    if (d->doStat(QFileSystemMetaData::SequentialType))
        return d->metaData.isSequential();
    return true;
}

/*!
    Opens the file using OpenMode \a mode, returning true if successful;
    otherwise false.

    The \a mode must be QIODevice::ReadOnly, QIODevice::WriteOnly, or
    QIODevice::ReadWrite. It may also have additional flags, such as
    QIODevice::Text and QIODevice::Unbuffered.

    \note In \l{QIODevice::}{WriteOnly} or \l{QIODevice::}{ReadWrite}
    mode, if the relevant file does not already exist, this function
    will try to create a new file before opening it.

    \sa QIODevice::OpenMode, setFileName()
*/
bool QFile::open(OpenMode mode)
{
    Q_D(QFile);
    if (Q_UNLIKELY(isOpen())) {
        qWarning("QFile::open: File (%s) already open", qPrintable(fileName()));
        return false;
    }
    d->setError(QFile::NoError, QString());
    if (mode & Append) {
        mode |= WriteOnly;
    }
    if (Q_UNLIKELY((mode & (ReadOnly | WriteOnly)) == 0)) {
        qWarning("QFile::open: File access not specified");
        return false;
    }

    if (Q_UNLIKELY(d->fileEntry.isEmpty())) {
        qWarning("QFile::open: No file name specified");
        d->setError(QFile::OpenError, QLatin1String("No file name specified"));
        return false;
    }

    // Append implies WriteOnly.
    if (mode & QFile::Append)
        mode |= QFile::WriteOnly;

    // WriteOnly implies Truncate if neither ReadOnly nor Append are sent.
    if ((mode & QFile::WriteOnly) && !(mode & (QFile::ReadOnly | QFile::Append)))
        mode |= QFile::Truncate;

    d->metaData.clear();
    d->fd = -1;

    int flags = QT_OPEN_RDONLY;

    if ((mode & QFile::ReadWrite) == QFile::ReadWrite) {
        flags = QT_OPEN_RDWR | QT_OPEN_CREAT;
    } else if (mode & QFile::WriteOnly) {
        flags = QT_OPEN_WRONLY | QT_OPEN_CREAT;
    }

    if (mode & QFile::Append) {
        flags |= QT_OPEN_APPEND;
    } else if (mode & QFile::WriteOnly) {
        if ((mode & QFile::Truncate) || !(mode & QFile::ReadOnly))
            flags |= QT_OPEN_TRUNC;
    }

    if (mode & QFile::Unbuffered) {
#ifdef O_DSYNC
        flags |= O_DSYNC;
#else
        flags |= O_SYNC;
#endif
    }

    // Try to open the file.
    const QByteArray native = d->fileEntry.nativeFilePath();
    d->fd = qt_safe_open(native.constData(), flags, 0666);

    // On failure, return and report the error.
    if (d->fd == -1) {
        d->setError(errno == EMFILE ? QFile::ResourceError : QFile::OpenError,
                    qt_error_string(errno));
        return false;
    }

    // Refuse to open directories, EISDIR is not a thing (by standards) for
    // non-write modes.
    QT_STATBUF statbuf;
    if (QT_FSTAT(d->fd, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
        qt_safe_close(d->fd);
        d->fd = -1;
        d->setError(QFile::OpenError, QLatin1String("file to open is a directory"));
        return false;
    }

    // Seek to the end when in Append mode.
    if (mode & QFile::Append) {
        if (QT_LSEEK(d->fd, 0, SEEK_END) == -1) {
            qt_safe_close(d->fd);
            d->fd = -1;
            d->setError(errno == EMFILE ? QFile::ResourceError : QFile::OpenError,
                        qt_error_string(errno));
            return false;
        }
    }

    d->closeFileHandle = true;

    QIODevice::open(mode);
    return true;
}

/*! \fn QFile::open(OpenMode, FILE*)

    Use open(FILE *, OpenMode) instead.
*/

/*!
    \overload

    Opens the existing file handle \a fh in the given \a mode.
    Returns true if successful; otherwise returns false.

    Example:
    \snippet doc/src/snippets/code/src_corelib_io_qfile.cpp 3

    When a QFile is opened using this function, behaviour of close() is
    controlled by the AutoCloseHandle flag.
    If AutoCloseHandle is specified, and this function succeeds,
    then calling close() closes the adopted handle.
    Otherwise, close() does not actually close the file, but only flushes it.

    \bold{Warning:}
    \list 1
        \o If \a fh does not refer to a regular file, e.g., it is \c stdin,
           \c stdout, or \c stderr, you may not be able to seek(). size()
           returns \c 0 in those cases. See QIODevice::isSequential() for
           more information.
        \o Since this function opens the file without specifying the file name,
           you cannot use this QFile with a QFileInfo.
    \endlist

    \sa close(), {qmake Variable Reference#CONFIG}{qmake Variable Reference}

    You need to enable support for console applications in order to use the
    stdin, stdout and stderr streams at the console. To do this, add the
    following declaration to your application's project file:

    \snippet doc/src/snippets/code/src_corelib_io_qfile.cpp 4
*/
bool QFile::open(FILE *fh, OpenMode mode, FileHandleFlags handleFlags)
{
    Q_D(QFile);
    if (Q_UNLIKELY(isOpen())) {
        qWarning("QFile::open: File (%s) already open", qPrintable(fileName()));
        return false;
    }
    d->setError(QFile::NoError, QString());
    if (mode & Append) {
        mode |= WriteOnly;
    }
    if (Q_UNLIKELY((mode & (ReadOnly | WriteOnly)) == 0)) {
        qWarning("QFile::open: File access not specified");
        return false;
    }
    if (d->openExternalFile(mode, QT_FILENO(fh), handleFlags)) {
        QIODevice::open(mode);
        if (mode & Append) {
            seek(size());
        } else {
            const qint64 pos = (qint64)QT_FTELL(fh);
            if (pos != -1 && !isSequential()) {
                seek(pos);
            } else if (pos != -1) {
                d->pos = pos;
                d->devicePos = pos;
            }
        }
        return true;
    }
    return false;
}

/*! \fn QFile::open(OpenMode, int)

    Use open(int, OpenMode) instead.
*/

/*!
    \overload

    Opens the existing file descriptor \a fd in the given \a mode.
    Returns true if successful; otherwise returns false.

    When a QFile is opened using this function, behaviour of close() is
    controlled by the \a handleFlags argument.
    If AutoCloseHandle is specified, and this function succeeds,
    then calling close() closes the adopted handle.
    Otherwise, close() does not actually close the file, but only flushes it.

    The QFile that is opened using this function is automatically set
    to be in raw mode; this means that the file input/output functions
    are slow. If you run into performance issues, you should try to
    use one of the other open functions.

    \warning If \a fd is not a regular file, e.g, it is 0 (\c stdin),
    1 (\c stdout), or 2 (\c stderr), you may not be able to seek(). In
    those cases, size() returns \c 0.  See QIODevice::isSequential()
    for more information.

    \warning Since this function opens the file without specifying the file name,
             you cannot use this QFile with a QFileInfo.

    \sa close()
*/
bool QFile::open(int fd, OpenMode mode, FileHandleFlags handleFlags)
{
    Q_D(QFile);
    if (Q_UNLIKELY(isOpen())) {
        qWarning("QFile::open: File (%s) already open", qPrintable(fileName()));
        return false;
    }
    d->setError(QFile::NoError, QString());
    if (mode & Append) {
        mode |= WriteOnly;
    }
    if (Q_UNLIKELY((mode & (ReadOnly | WriteOnly)) == 0)) {
        qWarning("QFile::open: File access not specified");
        return false;
    }
    if (d->openExternalFile(mode, fd, handleFlags)) {
        QIODevice::open(mode);
        if (mode & Append) {
            seek(size());
        } else {
            const qint64 pos = (qint64)QT_LSEEK(fd, 0, SEEK_CUR);
            if (pos != -1 && !isSequential()) {
                seek(pos);
            } else if (pos != -1) {
                d->pos = pos;
                d->devicePos = pos;
            }
        }
        return true;
    }
    return false;
}


/*!
    Returns the file handle of the file.

    This is a small positive integer, suitable for use with C library
    functions such as fdopen() and fcntl(). On systems that use file
    descriptors for sockets (i.e. Unix systems, but not Windows) the handle
    can be used with QSocketNotifier as well.

    If the file is not open, or there is an error, handle() returns -1.

    \sa QSocketNotifier
*/
int QFile::handle() const
{
    Q_D(const QFile);
    if (!isOpen()) {
        return -1;
    }
    return d->fd;
}

/*!
    \fn QString QFile::name() const

    Use fileName() instead.
*/

/*!
    \fn void QFile::setName(const QString &name)

    Use setFileName() instead.
*/

/*!
    Sets the file size (in bytes) \a sz. Returns true if the file if the
    resize succeeds; false otherwise. If \a sz is larger than the file
    currently is the new bytes will be set to 0, if \a sz is smaller the
    file is simply truncated.

    \sa size(), setFileName()
*/
bool QFile::resize(qint64 sz)
{
    Q_D(QFile);
    if (isOpen() && QT_LSEEK(d->fd, 0, SEEK_CUR) > sz) {
        seek(sz);
    }
    int ret = 0;
    if (d->fd != -1) {
        Q_EINTR_LOOP(ret, QT_FTRUNCATE(d->fd, sz));
    } else {
        const QByteArray native = d->fileEntry.nativeFilePath();
        Q_EINTR_LOOP(ret, QT_TRUNCATE(native.constData(), sz));
    }
    d->metaData.clearFlags(QFileSystemMetaData::SizeAttribute);
    if (ret == -1) {
        d->setError(QFile::ResizeError, qt_error_string(errno));
        return false;
    }
    d->setError(QFile::NoError, QString());
    return true;
}

/*!
    \overload

    Sets \a fileName to size (in bytes) \a sz. Returns true if the file if
    the resize succeeds; false otherwise. If \a sz is larger than \a
    fileName currently is the new bytes will be set to 0, if \a sz is
    smaller the file is simply truncated.

    \sa resize()
*/
bool QFile::resize(const QString &fileName, qint64 sz)
{
    const QByteArray native = QFileSystemEntry(fileName).nativeFilePath();
    int ret = 0;
    Q_EINTR_LOOP(ret, QT_TRUNCATE(native.constData(), sz));
    return (ret != -1);
}

/*!
    Returns the complete OR-ed together combination of
    QFile::Permission for the file.

    \sa setPermissions(), setFileName()
*/
QFile::Permissions QFile::permissions() const
{
    Q_D(const QFile);
    bool exists = d->doStat(QFileSystemMetaData::Permissions | QFileSystemMetaData::LinkType);
    if (!exists && !d->metaData.isLink())
        return 0;
    return d->metaData.permissions();
}

/*!
    \overload

    Returns the complete OR-ed together combination of
    QFile::Permission for \a fileName.
*/
QFile::Permissions QFile::permissions(const QString &fileName)
{
    QFileSystemMetaData metaData;
    QFileSystemEntry fileEntry(fileName);
    QFileSystemEngine::fillMetaData(fileEntry, metaData, QFileSystemMetaData::Permissions | QFileSystemMetaData::LinkType);
    return metaData.permissions();
}

/*!
    Sets the permissions for the file to the \a permissions specified.
    Returns true if successful, or false if the permissions cannot be
    modified.

    \sa permissions(), setFileName()
*/
bool QFile::setPermissions(Permissions permissions)
{
    Q_D(QFile);
    int error = 0;
    if (!QFileSystemEngine::setPermissions(d->fileEntry, permissions, &error)) {
        d->setError(QFile::PermissionsError, qt_error_string(error));
        return false;
    }
    d->setError(QFile::NoError, QString());
    return true;
}

/*!
    \overload

    Sets the permissions for \a fileName file to \a permissions.
*/
bool QFile::setPermissions(const QString &fileName, Permissions permissions)
{
    int error = 0;
    return QFileSystemEngine::setPermissions(QFileSystemEntry(fileName), permissions, &error);
}

/*!
    Flushes any buffered data to the file. Returns true if successful;
    otherwise returns false.
*/
bool QFile::flush()
{
    Q_D(QFile);
    if (d->fd == -1) {
        d->setError(QFile::WriteError, QString());
        return false;
    }
    return true;
}

/*!
    Calls QFile::flush() and closes the file. Errors from flush are ignored.

    \sa QIODevice::close()
*/
void QFile::close()
{
    Q_D(QFile);
    if(!isOpen())
        return;
    const bool flushed = flush();
    QIODevice::close();

    if (d->fd == -1) {
        d->setError(QFile::UnspecifiedError, QString());
        return;
    }

    d->metaData.clear();

    // Close the file if we created the handle.
    if (d->closeFileHandle) {
        const int ret = qt_safe_close(d->fd);

        // We must reset these guys regardless; calling close again after a
        // failed close causes crashes on some systems.
        d->fd = -1;

        // Report errors.
        if (ret != 0) {
            d->setError(QFile::UnspecifiedError, qt_error_string(errno));
            return;
        }
    }

    if (flushed) {
        d->setError(QFile::NoError, QString());
    }
}

/*!
    Returns the size of the file.

    For regular empty files on Unix (e.g. those in \c /proc), this function
    returns 0; the contents of such a file are generated on demand in response
    to you calling read().
*/
qint64 QFile::size() const
{
    Q_D(const QFile);
    d->metaData.clear();
    if (!d->doStat(QFileSystemMetaData::SizeAttribute))
        return 0;
    return d->metaData.size();
}

/*!
    Returns true if the end of the file has been reached; otherwise returns
    false.

    For regular empty files on Unix (e.g. those in \c /proc), this function
    returns true, since the file system reports that the size of such a file is
    0. Therefore, you should not depend on atEnd() when reading data from such a
    file, but rather call read() until no more data can be read.
*/
bool QFile::atEnd() const
{
    if (!isOpen()) {
        return true;
    }

    // Check how much is available (will re-stat file).
    return bytesAvailable() == 0;
}

/*!
    For random-access devices, this function sets the current position
    to \a pos, returning true on success, or false if an error occurred.
    For sequential devices, the default behavior is to do nothing and
    return false.

    Seeking beyond the end of a file:
    If the position is beyond the end of a file, then seek() shall not
    immediately extend the file. If a write is performed at this position,
    then the file shall be extended. The content of the file between the
    previous end of file and the newly written data is UNDEFINED and
    varies between platforms and file systems.
*/
bool QFile::seek(qint64 off)
{
    Q_D(QFile);
    if (Q_UNLIKELY(!isOpen())) {
        qWarning("QFile::seek: IODevice is not open");
        return false;
    }

    if (off == d->pos && off == d->devicePos) {
        return true; // avoid expensive flush for NOP seek to current position
    }

    if (off < 0 || off != qint64(QT_OFF_T(off))) {
        d->setError(QFile::PositionError, QString());
        return false;
    }

    if (Q_UNLIKELY(QT_LSEEK(d->fd, QT_OFF_T(off), SEEK_SET) == -1)) {
        qWarning("QFile::seek: Cannot set file position %lld", off);
        d->setError(QFile::PositionError, qt_error_string(errno));
        return false;
    }
    if (!QIODevice::seek(off)) {
        d->setError(QFile::PositionError, QString());
        return false;
    }
    d->setError(QFile::NoError, QString());
    return true;
}

/*!
    \reimp
*/
qint64 QFile::readLineData(char *data, qint64 maxlen)
{
    Q_D(QFile);
    if (d->fd != -1 && isSequential()) {
        qint64 readSoFar = 0;
        while (readSoFar < maxlen) {
            char c;
            qint64 readResult = qt_safe_read(d->fd, &c, 1);
            if (readResult <= 0)
                return (readSoFar > 0) ? readSoFar : -1;
            ++readSoFar;
            *data++ = c;
            if (c == '\n')
                return readSoFar;
        }
        return readSoFar;
    }
    // Fall back to QIODevice's readLine implementation if the engine
    // cannot do it faster.
    return QIODevice::readLineData(data, maxlen);
}

/*!
    \reimp
*/
qint64 QFile::readData(char *data, qint64 len)
{
    Q_D(QFile);
    d->setError(QFile::NoError, QString());

    if (len < 0) {
        d->setError(QFile::ReadError, qt_error_string(EINVAL));
        return -1;
    }

    qint64 readBytes = 0;
    bool eof = false;

    if (d->fd != -1) {
        ssize_t result;
        do {
            result = QT_READ(d->fd, data + readBytes, size_t(len - readBytes));
        } while ((result == -1 && errno == EINTR)
                || (result > 0 && (readBytes += result) < len));

        eof = !(result == -1);
    }

    if (!eof && readBytes == 0) {
        readBytes = -1;
        d->setError(QFile::ReadError, qt_error_string(errno));
    }

    return readBytes;
}

/*!
    \reimp
*/
qint64 QFile::writeData(const char *data, qint64 len)
{
    Q_D(QFile);
    d->setError(QFile::NoError, QString());

    if (len < 0 || len != qint64(size_t(len))) {
        d->setError(QFile::WriteError, qt_error_string(EINVAL));
        return -1;
    }

    qint64 writtenBytes = 0;

    if (d->fd != -1) {
        ssize_t result;
        do {
            result = QT_WRITE(d->fd, data + writtenBytes, size_t(len - writtenBytes));
        } while ((result == -1 && errno == EINTR)
                || (result > 0 && (writtenBytes += result) < len));
    }

    if (len && writtenBytes == 0) {
        writtenBytes = -1;
        d->setError(errno == ENOSPC ? QFile::ResourceError : QFile::WriteError, qt_error_string(errno));
    }

    d->metaData.clearFlags(QFileSystemMetaData::SizeAttribute);

    return writtenBytes;
}

/*!
    Returns the file error status.

    The I/O device status returns an error code. For example, if open()
    returns false, or a read/write operation returns -1, this function can
    be called to find out the reason why the operation failed.

    \sa QIODevice::errorString()
*/
QFile::FileError QFile::error() const
{
    Q_D(const QFile);
    return d->error;
}

#ifndef QT_NO_QOBJECT
#  include "moc_qfile.h"
#endif

QT_END_NAMESPACE
