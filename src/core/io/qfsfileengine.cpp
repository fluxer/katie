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

#include "qfsfileengine_p.h"
#include "qfsfileengine_iterator_p.h"
#include "qfilesystemengine_p.h"
#include "qdatetime.h"
#include "qdiriterator.h"
#include "qset.h"
#include "qdebug.h"
#include "qcore_unix_p.h"

#ifndef QT_NO_FSFILEENGINE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

QT_BEGIN_NAMESPACE

/*! \class QFSFileEngine
    \brief The QFSFileEngine class implements Qt's default file engine.
    \since 4.1

    This class is part of the file engine framework in Qt. If you only want to
    access files or directories, use QFile, QFileInfo or QDir instead.

    QFSFileEngine is the default file engine for accessing regular files. It
    is provided for convenience; by subclassing this class, you can alter its
    behavior slightly, without having to write a complete QAbstractFileEngine
    subclass

    It can also be useful to create a QFSFileEngine object directly if you
    need to use the local file system inside QAbstractFileEngine::create(), in
    order to avoid recursion (as higher-level classes tend to call
    QAbstractFileEngine::create()).
*/

//**************** QFSFileEnginePrivate
QFSFileEnginePrivate::QFSFileEnginePrivate() : QAbstractFileEnginePrivate()
{
    init();
}

/*!
    \internal
*/
void QFSFileEnginePrivate::init()
{
    metaData.clear();
    openMode = QIODevice::NotOpen;
    fd = -1;
    closeFileHandle = false;
}

/*!
    Constructs a QFSFileEngine for the file name \a file.
*/
QFSFileEngine::QFSFileEngine(const QString &file)
    : QAbstractFileEngine(*new QFSFileEnginePrivate)
{
    Q_D(QFSFileEngine);
    d->fileEntry = QFileSystemEntry(file);
}

/*!
    Constructs a QFSFileEngine.
*/
QFSFileEngine::QFSFileEngine() : QAbstractFileEngine(*new QFSFileEnginePrivate)
{
}

/*!
    \internal
*/
QFSFileEngine::QFSFileEngine(QFSFileEnginePrivate &dd)
    : QAbstractFileEngine(dd)
{
}

/*!
    Destructs the QFSFileEngine.
*/
QFSFileEngine::~QFSFileEngine()
{
    Q_D(QFSFileEngine);
    if (d->closeFileHandle) {
        if (d->fd != -1) {
            qt_safe_close(d->fd);
        }
    }
    QList<uchar*> keys = d->maps.keys();
    for (int i = 0; i < keys.count(); ++i)
        unmap(keys.at(i));
}

/*!
    \reimp
*/
void QFSFileEngine::setFileName(const QString &file)
{
    Q_D(QFSFileEngine);
    d->init();
    d->fileEntry = QFileSystemEntry(file);
}

/*!
    \reimp
*/
bool QFSFileEngine::open(QIODevice::OpenMode openMode)
{
    Q_D(QFSFileEngine);
    if (Q_UNLIKELY(d->fileEntry.isEmpty())) {
        qWarning("QFSFileEngine::open: No file name specified");
        setError(QFile::OpenError, QLatin1String("No file name specified"));
        return false;
    }

    // Append implies WriteOnly.
    if (openMode & QFile::Append)
        openMode |= QFile::WriteOnly;

    // WriteOnly implies Truncate if neither ReadOnly nor Append are sent.
    if ((openMode & QFile::WriteOnly) && !(openMode & (QFile::ReadOnly | QFile::Append)))
        openMode |= QFile::Truncate;

    d->openMode = openMode;
    d->metaData.clear();
    d->fd = -1;

    int flags = QT_OPEN_RDONLY;

    if ((d->openMode & QFile::ReadWrite) == QFile::ReadWrite) {
        flags = QT_OPEN_RDWR | QT_OPEN_CREAT;
    } else if (d->openMode & QFile::WriteOnly) {
        flags = QT_OPEN_WRONLY | QT_OPEN_CREAT;
    }

    if (d->openMode & QFile::Append) {
        flags |= QT_OPEN_APPEND;
    } else if (d->openMode & QFile::WriteOnly) {
        if ((d->openMode & QFile::Truncate) || !(d->openMode & QFile::ReadOnly))
            flags |= QT_OPEN_TRUNC;
    }

    if (d->openMode & QFile::Unbuffered) {
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
        setError(errno == EMFILE ? QFile::ResourceError : QFile::OpenError,
                 qt_error_string(errno));
        d->openMode = QIODevice::NotOpen;
        return false;
    }

    // Refuse to open directories, EISDIR is not a thing (by standards) for
    // non-write modes.
    QT_STATBUF statbuf;
    if (QT_FSTAT(d->fd, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
        setError(QFile::OpenError, QLatin1String("file to open is a directory"));
        qt_safe_close(d->fd);
        d->openMode = QIODevice::NotOpen;
        d->fd = -1;
        return false;
    }

    // Seek to the end when in Append mode.
    if (d->openMode & QFile::Append) {
        if (QT_LSEEK(d->fd, 0, SEEK_END) == -1) {
            setError(errno == EMFILE ? QFile::ResourceError : QFile::OpenError,
                     qt_error_string(errno));
            qt_safe_close(d->fd);
            d->openMode = QIODevice::NotOpen;
            d->fd = -1;
            return false;
        }
    }

    d->closeFileHandle = true;

    return true;
}

/*!
    Opens the file descriptor \a fd in \a openMode mode. Returns true
    on success; otherwise returns false.

    The \a handleFlags argument specifies whether the file handle will be
    closed by Katie. See the QFile::FileHandleFlags documentation for more
    information.
*/
bool QFSFileEngine::open(QIODevice::OpenMode openMode, int fd, QFile::FileHandleFlags handleFlags)
{
    Q_D(QFSFileEngine);

    // Append implies WriteOnly.
    if (openMode & QFile::Append)
        openMode |= QFile::WriteOnly;

    // WriteOnly implies Truncate if neither ReadOnly nor Append are sent.
    if ((openMode & QFile::WriteOnly) && !(openMode & (QFile::ReadOnly | QFile::Append)))
        openMode |= QFile::Truncate;

    d->openMode = openMode;
    d->closeFileHandle = (handleFlags & QFile::AutoCloseHandle);
    d->fileEntry.clear();
    d->fd = fd;
    d->metaData.clear();

    // Seek to the end when in Append mode.
    if (d->openMode & QFile::Append) {
        if (QT_LSEEK(d->fd, 0, SEEK_END) == -1) {
            setError(errno == EMFILE ? QFile::ResourceError : QFile::OpenError,
                     qt_error_string(errno));
            if (d->closeFileHandle) {
                qt_safe_close(d->fd);
            }
            d->openMode = QIODevice::NotOpen;
            d->fd = -1;
            return false;
        }
    }

    return true;
}

/*!
    \reimp
*/
bool QFSFileEngine::close()
{
    Q_D(QFSFileEngine);
    d->openMode = QIODevice::NotOpen;

    if (d->fd == -1)
        return false;

    d->metaData.clear();

    // Close the file if we created the handle.
    if (d->closeFileHandle) {
        int ret = qt_safe_close(d->fd);

        // We must reset these guys regardless; calling close again after a
        // failed close causes crashes on some systems.
        d->fd = -1;

        // Report errors.
        if (ret != 0) {
            setError(QFile::UnspecifiedError, qt_error_string(errno));
            return false;
        }
    }

    return true;
}

/*!
    \reimp
*/
bool QFSFileEngine::flush()
{
    Q_D(const QFSFileEngine);
    return (d->fd != -1);
}

/*!
    \reimp
*/
qint64 QFSFileEngine::size() const
{
    Q_D(const QFSFileEngine);

    if (!d->doStat(QFileSystemMetaData::SizeAttribute))
        return 0;
    return d->metaData.size();
}

/*!
    \reimp
*/
qint64 QFSFileEngine::pos() const
{
    Q_D(const QFSFileEngine);
    return (qint64)QT_LSEEK(d->fd, 0, SEEK_CUR);
}

/*!
    \reimp
*/
bool QFSFileEngine::seek(qint64 pos)
{
    Q_D(QFSFileEngine);

    if (pos < 0 || pos != qint64(QT_OFF_T(pos)))
        return false;

    if (Q_UNLIKELY(QT_LSEEK(d->fd, QT_OFF_T(pos), SEEK_SET) == -1)) {
        qWarning() << "QFile::at: Cannot set file position" << pos;
        setError(QFile::PositionError, qt_error_string(errno));
        return false;
    }
    return true;
}

/*!
    \reimp
*/
int QFSFileEngine::handle() const
{
    Q_D(const QFSFileEngine);
    return d->fd;
}

/*!
    \reimp
*/
qint64 QFSFileEngine::read(char *data, qint64 maxlen)
{
    Q_D(QFSFileEngine);

    if (maxlen < 0) {
        setError(QFile::ReadError, qt_error_string(EINVAL));
        return -1;
    }

    qint64 readBytes = 0;
    bool eof = false;

    if (d->fd != -1) {
        ssize_t result;
        do {
            result = QT_READ(d->fd, data + readBytes, size_t(maxlen - readBytes));
        } while ((result == -1 && errno == EINTR)
                || (result > 0 && (readBytes += result) < maxlen));

        eof = !(result == -1);
    }

    if (!eof && readBytes == 0) {
        readBytes = -1;
        setError(QFile::ReadError, qt_error_string(errno));
    }

    return readBytes;
}

/*!
    \reimp
*/
qint64 QFSFileEngine::write(const char *data, qint64 len)
{
    Q_D(QFSFileEngine);

    if (len < 0 || len != qint64(size_t(len))) {
        setError(QFile::WriteError, qt_error_string(EINVAL));
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
        setError(errno == ENOSPC ? QFile::ResourceError : QFile::WriteError, qt_error_string(errno));
    }

    d->metaData.clearFlags(QFileSystemMetaData::SizeAttribute);

    return writtenBytes;
}

#ifndef QT_NO_FILESYSTEMITERATOR
/*!
    \internal
*/
QAbstractFileEngine::Iterator *QFSFileEngine::beginEntryList(QDir::Filters filters, const QStringList &filterNames)
{
    return new QFSFileEngineIterator(filters, filterNames);
}
#endif

/*!
    \reimp
*/
bool QFSFileEngine::isSequential() const
{
    Q_D(const QFSFileEngine);
    if (d->doStat(QFileSystemMetaData::SequentialType))
        return d->metaData.isSequential();
    return true;
}

/*!
    \reimp
*/
bool QFSFileEngine::extension(Extension extension, const ExtensionOption *option, ExtensionReturn *output)
{
    Q_D(QFSFileEngine);

    if (extension == MapExtension) {
        const MapExtensionOption *options = (MapExtensionOption*)(option);
        MapExtensionReturn *returnValue = static_cast<MapExtensionReturn*>(output);
        returnValue->address = d->map(options->offset, options->size);
        return (returnValue->address != 0);
    } else if (extension == UnMapExtension) {
        UnMapExtensionOption *options = (UnMapExtensionOption*)option;
        return d->unmap(options->address);
    }

    return false;
}

/*!
    \reimp
*/
bool QFSFileEngine::supportsExtension(Extension extension) const
{
    Q_D(const QFSFileEngine);
    if (extension == FastReadLineExtension && d->fd != -1 && isSequential())
        return true;
    if (extension == UnMapExtension || extension == MapExtension)
        return true;
    return false;
}

/*! \fn bool QFSFileEngine::caseSensitive() const
  Returns true for Windows, false for Unix.
*/

/*! \fn bool QFSFileEngine::copy(const QString &copyName)

  For windows, copy the file to file \a copyName.

  Not implemented for Unix.
*/

/*! \fn QString QFSFileEngine::currentPath(const QString &fileName)
  For Unix, returns the current working directory for the file
  engine.

  For Windows, returns the canonicalized form of the current path used
  by the file engine for the drive specified by \a fileName.  On
  Windows, each drive has its own current directory, so a different
  path is returned for file names that include different drive names
  (e.g. A: or C:).

  \sa setCurrentPath()
*/

/*! \fn QString QFSFileEngine::fileName(FileName file) const
  \reimp
*/

/*! \fn QDateTime QFSFileEngine::fileTime(FileTime time) const
  \reimp
*/

/*! \fn QString QFSFileEngine::homePath()
  Returns the home path of the current user.

  \sa rootPath()
*/

/*! \fn bool QFSFileEngine::isRelativePath() const
  \reimp
*/

/*! \fn bool QFSFileEngine::link(const QString &newName)

  Creates a link from the file currently specified by fileName() to
  \a newName. What a link is depends on the underlying filesystem
  (be it a shortcut on Windows or a symbolic link on Unix). Returns
  true if successful; otherwise returns false.
*/

/*! \fn bool QFSFileEngine::mkdir(const QString &name, bool createParentDirectories) const
  \reimp
*/

/*! \fn uint QFSFileEngine::ownerId(FileOwner own) const
  In Unix, if stat() is successful, the \c uid is returned if
  \a own is the owner. Otherwise the \c gid is returned. If stat()
  is unsuccessful, -2 is returned.
*/

/*! \fn QString QFSFileEngine::owner(FileOwner own) const
  \reimp
*/

/*! \fn bool QFSFileEngine::remove()
  \reimp
*/

/*! \fn bool QFSFileEngine::rename(const QString &newName)
  \reimp
*/

/*! \fn bool QFSFileEngine::rmdir(const QString &name, bool recurseParentDirectories) const
  \reimp
*/

/*! \fn QString QFSFileEngine::rootPath()
  Returns the root path.

  \sa homePath()
*/

/*! \fn bool QFSFileEngine::setCurrentPath(const QString &path)
  Sets the current path (e.g., for QDir), to \a path. Returns true if the
  new path exists; otherwise this function does nothing, and returns false.

  \sa currentPath()
*/

/*! \fn bool QFSFileEngine::setPermissions(uint perms)
  \reimp
*/

/*! \fn bool QFSFileEngine::setSize(qint64 size)
  \reimp
*/

/*! \fn QString QFSFileEngine::tempPath()
  Returns the temporary path (i.e., a path in which it is safe
  to store temporary files).
*/

QT_END_NAMESPACE

#endif // QT_NO_FSFILEENGINE
