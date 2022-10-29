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

#include "qabstractfileengine.h"
#include "qabstractfileengine_p.h"
#include "qdatetime.h"
#include "qvariant.h"
#include "qdebug.h"
#include "qfilesystementry_p.h"
#include "qfilesystemengine_p.h"
#include "qfileinfo_p.h"
#include "qcore_unix_p.h"
// built-in handlers
#include "qdiriterator.h"

#include <sys/mman.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

QT_BEGIN_NAMESPACE

QAbstractFileEnginePrivate::QAbstractFileEnginePrivate()
    : fileError(QFile::UnspecifiedError),
    openMode(QIODevice::NotOpen),
    fd(-1),
    closeFileHandle(false)
{
}

bool QAbstractFileEnginePrivate::doStat(QFileSystemMetaData::MetaDataFlags flags) const
{
    if (!metaData.hasFlags(flags)) {
        if (!fileEntry.isEmpty())
            QFileSystemEngine::fillMetaData(fileEntry, metaData, metaData.missingFlags(flags));

        if (metaData.missingFlags(flags) && fd != -1)
            QFileSystemEngine::fillMetaData(fd, metaData);
    }

    return metaData.exists();
}

uchar *QAbstractFileEnginePrivate::map(qint64 offset, qint64 size)
{
    Q_Q(QAbstractFileEngine);
    if (openMode == QIODevice::NotOpen) {
        q->setError(QFile::PermissionsError, qt_error_string(EACCES));
        return 0;
    }

    if (offset < 0 || offset != qint64(QT_OFF_T(offset))
            || size < 0 || quint64(size) > quint64(size_t(-1))) {
        q->setError(QFile::UnspecifiedError, qt_error_string(EINVAL));
        return 0;
    }

    // If we know the mapping will extend beyond EOF, fail early to avoid
    // undefined behavior. Otherwise, let mmap have its say.
    if (doStat(QFileSystemMetaData::SizeAttribute)
            && (QT_OFF_T(size) > metaData.size() - QT_OFF_T(offset)))
        qWarning("QAbstractFileEngine::map: Mapping a file beyond its size is not portable");

    int access = 0;
    if (openMode & QIODevice::ReadOnly) access |= PROT_READ;
    if (openMode & QIODevice::WriteOnly) access |= PROT_WRITE;

    static const int pageSize = ::getpagesize();
    const int extra = offset % pageSize;

    if (quint64(size + extra) > quint64((size_t)-1)) {
        q->setError(QFile::UnspecifiedError, qt_error_string(EINVAL));
        return 0;
    }

    size_t realSize = (size_t)size + extra;
    QT_OFF_T realOffset = QT_OFF_T(offset);
    realOffset &= ~(QT_OFF_T(pageSize - 1));

    void *mapAddress = QT_MMAP(nullptr, realSize,
                   access, MAP_SHARED, fd, realOffset);
    if (mapAddress != MAP_FAILED) {
        uchar *address = extra + static_cast<uchar*>(mapAddress);
        maps[address] = QPair<int,size_t>(extra, realSize);
        return address;
    }

    switch(errno) {
    case EBADF:
        q->setError(QFile::PermissionsError, qt_error_string(EACCES));
        break;
    case ENFILE:
    case ENOMEM:
        q->setError(QFile::ResourceError, qt_error_string(errno));
        break;
    case EINVAL:
        // size are out of bounds
    default:
        q->setError(QFile::UnspecifiedError, qt_error_string(errno));
        break;
    }
    return 0;
}

bool QAbstractFileEnginePrivate::unmap(uchar *ptr)
{
    Q_Q(QAbstractFileEngine);
    if (!maps.contains(ptr)) {
        q->setError(QFile::PermissionsError, qt_error_string(EACCES));
        return false;
    }

    uchar *start = ptr - maps[ptr].first;
    size_t len = maps[ptr].second;
    if (::munmap(start, len) == -1) {
        q->setError(QFile::UnspecifiedError, qt_error_string(errno));
        return false;
    }
    maps.remove(ptr);
    return true;
}

/*!
    Creates and returns a QAbstractFileEngine suitable for processing \a
    fileName.
*/
QAbstractFileEngine *QAbstractFileEngine::create(const QString &fileName)
{
#ifndef QT_NO_FSFILEENGINE
    // fall back to regular file engine
    QAbstractFileEngine *engine = new QAbstractFileEngine();
    engine->d_ptr->fileEntry = QFileSystemEntry(fileName);
    return engine;
#else
    return nullptr;
#endif
}

/*!
    \enum QAbstractFileEngine::FileName

    These values are used to request a file name in a particular
    format.

    \value DefaultName The same filename that was passed to the
    QAbstractFileEngine.
    \value BaseName The name of the file excluding the path.
    \value PathName The path to the file excluding the base name.
    \value AbsoluteName The absolute path to the file (including
    the base name).
    \value AbsolutePathName The absolute path to the file (excluding
    the base name).
    \value LinkName The full file name of the file that this file is a
    link to. (This will be empty if this file is not a link.)
    \value CanonicalName Often very similar to LinkName. Will return the true path to the file.
    \value CanonicalPathName Same as CanonicalName, excluding the base name.

    \omitvalue NFileNames

    \sa fileName(), setFileName()
*/

/*!
    \enum QAbstractFileEngine::FileFlag

    The permissions and types of a file, suitable for OR'ing together.

    \value ReadOwnerPerm The owner of the file has permission to read
    it.
    \value WriteOwnerPerm The owner of the file has permission to
    write to it.
    \value ExeOwnerPerm The owner of the file has permission to
    execute it.
    \value ReadUserPerm The current user has permission to read the
    file.
    \value WriteUserPerm The current user has permission to write to
    the file.
    \value ExeUserPerm The current user has permission to execute the
    file.
    \value ReadGroupPerm Members of the current user's group have
    permission to read the file.
    \value WriteGroupPerm Members of the current user's group have
    permission to write to the file.
    \value ExeGroupPerm Members of the current user's group have
    permission to execute the file.
    \value ReadOtherPerm All users have permission to read the file.
    \value WriteOtherPerm All users have permission to write to the
    file.
    \value ExeOtherPerm All users have permission to execute the file.

    \value LinkType The file is a link to another file (or link) in
    the file system (i.e. not a file or directory).
    \value FileType The file is a regular file to the file system
    (i.e. not a link or directory)
    \value DirectoryType The file is a directory in the file system
    (i.e. not a link or file).

    \value HiddenFlag The file is hidden.
    \value ExistsFlag The file actually exists in the file system.
    \value RootFlag  The file or the file pointed to is the root of the filesystem.
    \value LocalDiskFlag The file resides on the local disk and can be passed to standard file functions.
    \value Refresh Passing this flag will force the file engine to refresh all flags.

    \omitvalue PermsMask
    \omitvalue TypesMask
    \omitvalue FlagsMask
    \omitvalue FileInfoAll

    \sa fileFlags(), setFileName()
*/

/*!
    \enum QAbstractFileEngine::FileTime

    These are used by the fileTime() function.

    \value CreationTime When the file was created.
    \value ModificationTime When the file was most recently modified.
    \value AccessTime When the file was most recently accessed (e.g.
    read or written to).

    \sa setFileName()
*/

/*!
    \enum QAbstractFileEngine::FileOwner

    \value OwnerUser The user who owns the file.
    \value OwnerGroup The group who owns the file.

    \sa owner(), ownerId(), setFileName()
*/

/*!
   Constructs a new QAbstractFileEngine that does not refer to any file or directory.

   \sa setFileName()
 */
QAbstractFileEngine::QAbstractFileEngine() : d_ptr(new QAbstractFileEnginePrivate)
{
    d_ptr->q_ptr = this;
}

/*!
   \internal

   Constructs a QAbstractFileEngine.
 */
QAbstractFileEngine::QAbstractFileEngine(QAbstractFileEnginePrivate &dd) : d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys the QAbstractFileEngine.
 */
QAbstractFileEngine::~QAbstractFileEngine()
{
    Q_D(QAbstractFileEngine);
    if (d->closeFileHandle) {
        if (d->fd != -1) {
            qt_safe_close(d->fd);
        }
    }
    QList<uchar*> keys = d->maps.keys();
    for (int i = 0; i < keys.count(); ++i)
        unmap(keys.at(i));

    delete d_ptr;
}

/*!
    \fn bool QAbstractFileEngine::open(QIODevice::OpenMode mode)

    Opens the file in the specified \a mode. Returns true if the file
    was successfully opened; otherwise returns false.

    The \a mode is an OR combination of QIODevice::OpenMode and
    QIODevice::HandlingMode values.
*/
bool QAbstractFileEngine::open(QIODevice::OpenMode openMode)
{
    Q_D(QAbstractFileEngine);
    if (Q_UNLIKELY(d->fileEntry.isEmpty())) {
        qWarning("QAbstractFileEngine::open: No file name specified");
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
    Closes the file, returning true if successful; otherwise returns false.

    The default implementation always returns false.
*/
bool QAbstractFileEngine::close()
{
    Q_D(QAbstractFileEngine);
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
    Flushes the open file, returning true if successful; otherwise returns
    false.

    The default implementation always returns false.
*/
bool QAbstractFileEngine::flush()
{
    Q_D(const QAbstractFileEngine);
    return (d->fd != -1);
}

/*!
    Returns the size of the file.
*/
qint64 QAbstractFileEngine::size() const
{
    Q_D(const QAbstractFileEngine);

    if (!d->doStat(QFileSystemMetaData::SizeAttribute))
        return 0;
    return d->metaData.size();
}

/*!
    Returns the current file position.

    This is the position of the data read/write head of the file.
*/
qint64 QAbstractFileEngine::pos() const
{
    Q_D(const QAbstractFileEngine);
    return (qint64)QT_LSEEK(d->fd, 0, SEEK_CUR);
}

/*!
    \fn bool QAbstractFileEngine::seek(qint64 offset)

    Sets the file position to the given \a offset. Returns true if
    the position was successfully set; otherwise returns false.

    The offset is from the beginning of the file, unless the
    file is sequential.

    \sa isSequential()
*/
bool QAbstractFileEngine::seek(qint64 pos)
{
    Q_D(QAbstractFileEngine);

    if (pos < 0 || pos != qint64(QT_OFF_T(pos)))
        return false;

    if (Q_UNLIKELY(QT_LSEEK(d->fd, QT_OFF_T(pos), SEEK_SET) == -1)) {
        qWarning("QAbstractFileEngine::seek: Cannot set file position %lld", pos);
        setError(QFile::PositionError, qt_error_string(errno));
        return false;
    }
    return true;
}

/*!
    Returns true if the file is a sequential access device; returns
    false if the file is a direct access device.

    Operations involving size() and seek(int) are not valid on
    sequential devices.
*/
bool QAbstractFileEngine::isSequential() const
{
    Q_D(const QAbstractFileEngine);
    if (d->doStat(QFileSystemMetaData::SequentialType))
        return d->metaData.isSequential();
    return true;
}

/*!
    Requests that the file is deleted from the file system. If the
    operation succeeds return true; otherwise return false.

    \sa setFileName() rmdir()
 */
bool QAbstractFileEngine::remove()
{
    Q_D(QAbstractFileEngine);
    int error;
    bool ret = QFileSystemEngine::removeFile(d->fileEntry, &error);
    d->metaData.clear();
    if (!ret) {
        setError(QFile::RemoveError, qt_error_string(error));
    }
    return ret;
}

/*!
    Copies the contents of this file to a file with the name \a newName.
    Returns true on success; otherwise, false is returned.
*/
bool QAbstractFileEngine::copy(const QString &newName)
{
    Q_D(QAbstractFileEngine);
    int error;
    bool ret = QFileSystemEngine::copyFile(d->fileEntry, QFileSystemEntry(newName), &error);
    if (!ret) {
        setError(QFile::CopyError, qt_error_string(error));
    }
    return ret;
}

/*!
    Requests that the file be renamed to \a newName in the file
    system. If the operation succeeds return true; otherwise return
    false.

    \sa setFileName()
 */
bool QAbstractFileEngine::rename(const QString &newName)
{
    Q_D(QAbstractFileEngine);
    int error;
    bool ret = QFileSystemEngine::renameFile(d->fileEntry, QFileSystemEntry(newName), &error);
    d->metaData.clear();
    if (!ret) {
        setError(QFile::RenameError, qt_error_string(error));
    }

    return ret;
}

/*!
    Creates a link from the file currently specified by fileName() to
    \a newName. What a link is depends on the underlying filesystem
    (be it a shortcut on Windows or a symbolic link on Unix). Returns
    true if successful; otherwise returns false.
*/
bool QAbstractFileEngine::link(const QString &newName)
{
    Q_D(QAbstractFileEngine);
    int error;
    bool ret = QFileSystemEngine::createLink(d->fileEntry, QFileSystemEntry(newName), &error);
    if (!ret) {
        setError(QFile::RenameError, qt_error_string(error));
    }
    return ret;
}

/*!
    Requests that the directory \a dirName be created. If
    \a createParentDirectories is true, then any sub-directories in \a dirName
    that don't exist must be created. If \a createParentDirectories is false then
    any sub-directories in \a dirName must already exist for the function to
    succeed. If the operation succeeds return true; otherwise return
    false.

    \sa setFileName() rmdir() isRelativePath()
 */
bool QAbstractFileEngine::mkdir(const QString &dirName, bool createParentDirectories) const
{
    return QFileSystemEngine::createDirectory(QFileSystemEntry(dirName), createParentDirectories);
}

/*!
    Requests that the directory \a dirName is deleted from the file
    system. When \a recurseParentDirectories is true, then any empty
    parent-directories in \a dirName must also be deleted. If
    \a recurseParentDirectories is false, only the \a dirName leaf-node
    should be deleted. In most file systems a directory cannot be deleted
    using this function if it is non-empty. If the operation succeeds
    return true; otherwise return false.

    \sa setFileName() remove() mkdir() isRelativePath()
 */
bool QAbstractFileEngine::rmdir(const QString &dirName, bool recurseParentDirectories) const
{
    return QFileSystemEngine::removeDirectory(QFileSystemEntry(dirName), recurseParentDirectories);
}

/*!
    Requests that the file be set to size \a size. If \a size is larger
    than the current file then it is filled with 0's, if smaller it is
    simply truncated. If the operations succceeds return true; otherwise
    return false;

    \sa size()
*/
bool QAbstractFileEngine::setSize(qint64 size)
{
    Q_D(QAbstractFileEngine);
    int ret = 0;
    if (d->fd != -1) {
        Q_EINTR_LOOP(ret, QT_FTRUNCATE(d->fd, size));
    } else {
        Q_EINTR_LOOP(ret, QT_TRUNCATE(d->fileEntry.nativeFilePath().constData(), size));
    }
    d->metaData.clearFlags(QFileSystemMetaData::SizeAttribute);
    if (ret == -1) {
        setError(QFile::ResizeError, qt_error_string(errno));
        return false;
    }
    return true;
}

/*!
    Return true if the file referred to by this file engine has a
    relative path; otherwise return false.

    \sa setFileName()
 */
bool QAbstractFileEngine::isRelativePath() const
{
    Q_D(const QAbstractFileEngine);
    return d->fileEntry.filePath().length() ? d->fileEntry.filePath()[0] != QLatin1Char('/') : true;
}

/*!
    Requests that a list of all the files matching the \a filters
    list based on the \a filterNames in the file engine's directory
    are returned.

    Should return an empty list if the file engine refers to a file
    rather than a directory, or if the directory is unreadable or does
    not exist or if nothing matches the specifications.

    \sa setFileName()
 */
QStringList QAbstractFileEngine::entryList(QDir::Filters filters, const QStringList &filterNames) const
{
    QStringList ret;
    QDirIterator it(fileName(), filterNames, filters);
    while (it.hasNext()) {
        it.next();
        ret << it.fileName();
    }
    return ret;
}

/*!
    Return the set of OR'd flags that are true for the file engine's
    file, and that are in the \a type's OR'd members.

    \sa setFileName()
*/
QAbstractFileEngine::FileFlags QAbstractFileEngine::fileFlags(FileFlags type) const
{
    Q_D(const QAbstractFileEngine);

    if (type & Refresh)
        d->metaData.clear();

    QAbstractFileEngine::FileFlags ret = 0;

    if (type & FlagsMask)
        ret |= LocalDiskFlag;

    QFileSystemMetaData::MetaDataFlags queryFlags =
            QFileSystemMetaData::MetaDataFlags(uint(type))
            & QFileSystemMetaData::Permissions;
    queryFlags |= QFileSystemMetaData::LinkType;

    if (type & TypesMask)
        queryFlags |= QFileSystemMetaData::LinkType
                | QFileSystemMetaData::FileType
                | QFileSystemMetaData::DirectoryType;

    if (type & FlagsMask)
        queryFlags |= QFileSystemMetaData::HiddenAttribute
                | QFileSystemMetaData::ExistsAttribute;

    bool exists = d->doStat(queryFlags);

    if (!exists && !d->metaData.isLink())
        return ret;

    if (exists && (type & PermsMask))
        ret |= FileFlags(uint(d->metaData.permissions()));

    if (type & TypesMask) {
        if ((type & LinkType) && d->metaData.isLink())
            ret |= LinkType;
        if (exists) {
            if (d->metaData.isFile()) {
                ret |= FileType;
            } else if (d->metaData.isDirectory()) {
                ret |= DirectoryType;
            }
        }
    }

    if (type & FlagsMask) {
        if (exists)
            ret |= ExistsFlag;
        if (d->fileEntry.isRoot())
            ret |= RootFlag;
        else if (d->metaData.isHidden())
            ret |= HiddenFlag;
    }

    return ret;
}

/*!
    Requests that the file's permissions be set to \a perms. The argument
    perms will be set to the OR-ed together combination of
    QAbstractFileEngine::FileInfo, with only the QAbstractFileEngine::PermsMask being
    honored. If the operations succceeds return true; otherwise return
    false;

    \sa size()
*/
bool QAbstractFileEngine::setPermissions(uint perms)
{
    Q_D(QAbstractFileEngine);
    int error;
    if (!QFileSystemEngine::setPermissions(d->fileEntry, QFile::Permissions(perms), &error)) {
        setError(QFile::PermissionsError, qt_error_string(error));
        return false;
    }
    return true;
}

/*!
    Return  the file engine's current file name in the format
    specified by \a file.

    If you don't handle some \c FileName possibilities, return the
    file name set in setFileName() when an unhandled format is
    requested.

    \sa setFileName(), FileName
 */
QString QAbstractFileEngine::fileName(FileName file) const
{
    Q_D(const QAbstractFileEngine);
    if (file == BaseName) {
        return d->fileEntry.fileName();
    } else if (file == PathName) {
        return d->fileEntry.path();
    } else if (file == AbsoluteName || file == AbsolutePathName) {
        QFileSystemEntry entry(QFileSystemEngine::absoluteName(d->fileEntry));
        if (file == AbsolutePathName) {
            return entry.path();
        }
        return entry.filePath();
    } else if (file == CanonicalName || file == CanonicalPathName) {
        QFileSystemEntry entry(QFileSystemEngine::canonicalName(d->fileEntry, d->metaData));
        if (file == CanonicalPathName)
            return entry.path();
        return entry.filePath();
    } else if (file == LinkName) {
        if (!d->metaData.hasFlags(QFileSystemMetaData::LinkType))
            QFileSystemEngine::fillMetaData(d->fileEntry, d->metaData, QFileSystemMetaData::LinkType);
        if (d->metaData.isLink()) {
            QFileSystemEntry entry = QFileSystemEngine::getLinkTarget(d->fileEntry, d->metaData);
            return entry.filePath();
        }
        return QString();
    }
    return d->fileEntry.filePath();
}

/*!
    If \a owner is \c OwnerUser return the ID of the user who owns
    the file. If \a owner is \c OwnerGroup return the ID of the group
    that own the file. If you can't determine the owner return -2.

    \sa owner() setFileName(), FileOwner
 */
uint QAbstractFileEngine::ownerId(FileOwner owner) const
{
    Q_D(const QAbstractFileEngine);

    if (d->doStat(QFileSystemMetaData::OwnerIds)) {
        if (owner == QAbstractFileEngine::OwnerUser)
            return d->metaData.userId();
        return d->metaData.groupId();
    }

    return QFileSystemMetaData::nobodyID;
}

/*!
    If \a owner is \c OwnerUser return the name of the user who owns
    the file. If \a owner is \c OwnerGroup return the name of the group
    that own the file. If you can't determine the owner return
    QString().

    \sa ownerId() setFileName(), FileOwner
 */
QString QAbstractFileEngine::owner(FileOwner owner) const
{
    if (owner == QAbstractFileEngine::OwnerUser)
        return QFileSystemEngine::resolveUserName(ownerId(owner));
    return QFileSystemEngine::resolveGroupName(ownerId(owner));
}

/*!
    If \a time is \c CreationTime, return when the file was created.
    If \a time is \c ModificationTime, return when the file was most
    recently modified. If \a time is \c AccessTime, return when the
    file was most recently accessed (e.g. read or written).
    If the time cannot be determined return QDateTime() (an invalid
    date time).

    \sa setFileName(), QDateTime, QDateTime::isValid(), FileTime
 */
QDateTime QAbstractFileEngine::fileTime(FileTime time) const
{
    Q_D(const QAbstractFileEngine);

    if (d->doStat(QFileSystemMetaData::Times)) {
        switch (time) {
        case QAbstractFileEngine::ModificationTime:
            return d->metaData.modificationTime();
        case QAbstractFileEngine::AccessTime:
            return d->metaData.accessTime();
        case QAbstractFileEngine::CreationTime:
            return d->metaData.creationTime();
        }
    }

    return QDateTime();
}

/*!
    Sets the file engine's file name to \a file. This file name is the
    file that the rest of the functions will operate on.

    \sa rename()
 */
void QAbstractFileEngine::setFileName(const QString &file)
{
    Q_D(QAbstractFileEngine);
    d->metaData.clear();
    d->openMode = QIODevice::NotOpen;
    d->fd = -1;
    d->closeFileHandle = false;
    d->fileEntry = QFileSystemEntry(file);
}

/*!
    Returns the native file handle for this file engine. This handle must be
    used with care; its value and type are platform specific, and using it
    will most likely lead to non-portable code.
*/
int QAbstractFileEngine::handle() const
{
    Q_D(const QAbstractFileEngine);
    return d->fd;
}

/*!
    \since 4.4

    Maps \a size bytes of the file into memory starting at \a offset.
    Returns a pointer to the memory if successful; otherwise returns false
    if, for example, an error occurs.

    \sa unmap(), supportsExtension()
 */

uchar *QAbstractFileEngine::map(qint64 offset, qint64 size)
{
    MapExtensionOption option;
    option.offset = offset;
    option.size = size;
    MapExtensionReturn r;
    if (!extension(MapExtension, &option, &r))
        return 0;
    return r.address;
}

/*!
    \since 4.4

    Unmaps the memory \a address.  Returns true if the unmap succeeds; otherwise
    returns false.

    \sa map(), supportsExtension()
 */
bool QAbstractFileEngine::unmap(uchar *address)
{
    UnMapExtensionOption options;
    options.address = address;
    return extension(UnMapExtension, &options);
}

/*!
    Returns an instance of a QAbstractFileEngineIterator using \a filters for
    entry filtering and \a filterNames for name filtering. This function is
    called by QDirIterator to initiate directory iteration.

    QDirIterator takes ownership of the returned instance, and deletes it when
    it's done.

    \sa QDirIterator
*/
QAbstractFileEngine::Iterator *QAbstractFileEngine::beginEntryList(QDir::Filters filters, const QStringList &filterNames)
{
#ifndef QT_NO_FILESYSTEMITERATOR
    return new QAbstractFileEngineIterator(filters, filterNames);
#else
    Q_UNUSED(filters);
    Q_UNUSED(filterNames);
    return nullptr;
#endif
}

/*!
    \internal
*/
QAbstractFileEngine::Iterator *QAbstractFileEngine::endEntryList()
{
    return nullptr;
}

/*!
    Reads a number of characters from the file into \a data. At most
    \a maxlen characters will be read.

    Returns -1 if a fatal error occurs, or 0 if there are no bytes to
    read.
*/
qint64 QAbstractFileEngine::read(char *data, qint64 maxlen)
{
    Q_D(QAbstractFileEngine);

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
    Writes \a len bytes from \a data to the file. Returns the number
    of characters written on success; otherwise returns -1.
*/
qint64 QAbstractFileEngine::write(const char *data, qint64 len)
{
    Q_D(QAbstractFileEngine);

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

/*!
    Reads one line, terminated by a '\n' character, from the file info
    \a data. At most \a maxlen characters will be read. The end-of-line
    character is included.
*/
qint64 QAbstractFileEngine::readLine(char *data, qint64 maxlen)
{
    qint64 readSoFar = 0;
    while (readSoFar < maxlen) {
        char c;
        qint64 readResult = read(&c, 1);
        if (readResult <= 0)
            return (readSoFar > 0) ? readSoFar : -1;
        ++readSoFar;
        *data++ = c;
        if (c == '\n')
            return readSoFar;
    }
    return readSoFar;
}

/*!
   \enum QAbstractFileEngine::Extension
   \since 4.3

   This enum describes the types of extensions that the file engine can
   support. Before using these extensions, you must verify that the extension
   is supported (i.e., call supportsExtension()).

   \value FastReadLineExtension Whether the file engine provides a
   fast implementation for readLine() or not. If readLine() remains
   unimplemented in the file engine, QAbstractFileEngine will provide
   an implementation based on calling read() repeatedly. If
   supportsExtension() returns false for this extension, however,
   QIODevice can provide a faster implementation by making use of its
   internal buffer. For engines that already provide a fast readLine()
   implementation, returning false for this extension can avoid
   unnnecessary double-buffering in QIODevice.

   \value MapExtension Whether the file engine provides the ability to map
   a file to memory.

   \value UnMapExtension Whether the file engine provides the ability to
   unmap memory that was previously mapped.
*/

/*!
   \class QAbstractFileEngine::ExtensionOption
   \since 4.3
   \brief provides an extended input argument to QAbstractFileEngine's
   extension support.

   \sa QAbstractFileEngine::extension()
*/

/*!
   \class QAbstractFileEngine::ExtensionReturn
   \since 4.3
   \brief provides an extended output argument to QAbstractFileEngine's
   extension support.

   \sa QAbstractFileEngine::extension()
*/

/*!
    \since 4.3

    The \a option argument is provided as input to the extension, and
    this function can store output results in \a output.

    The behavior of this function is determined by \a extension; see the
    Extension documentation for details.

    You can call supportsExtension() to check if an extension is supported by
    the file engine.

    By default, map and unmap extensions are supported.

    \sa supportsExtension(), Extension
*/
bool QAbstractFileEngine::extension(Extension extension, const ExtensionOption *option, ExtensionReturn *output)
{
    Q_D(QAbstractFileEngine);

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
    \since 4.3

    Returns true if the file engine supports \a extension; otherwise, false
    is returned. By default map, unmap and fast readline extensions are supported.

    \sa extension()
*/
bool QAbstractFileEngine::supportsExtension(Extension extension) const
{
    Q_D(const QAbstractFileEngine);
    if (extension == FastReadLineExtension && d->fd != -1 && isSequential())
        return true;
    if (extension == UnMapExtension || extension == MapExtension)
        return true;
    return false;
}

/*!
    Returns the QFile::FileError that resulted from the last failed
    operation. If QFile::UnspecifiedError is returned, QFile will
    use its own idea of the error status.

    \sa QFile::FileError, errorString()
 */
QFile::FileError QAbstractFileEngine::error() const
{
    Q_D(const QAbstractFileEngine);
    return d->fileError;
}

/*!
    Returns the human-readable message appropriate to the current error
    reported by error(). If no suitable string is available, an
    empty string is returned.

    \sa error()
 */
QString QAbstractFileEngine::errorString() const
{
    Q_D(const QAbstractFileEngine);
    return d->errorString;
}

/*!
    Sets the error type to \a error, and the error string to \a errorString.

    \sa QFile::error(), QIODevice::errorString(), QIODevice::setErrorString()
*/
void QAbstractFileEngine::setError(QFile::FileError error, const QString &errorString)
{
    Q_D(QAbstractFileEngine);
    d->fileError = error;
    d->errorString = errorString;
}

/*!
    Opens the file descriptor \a fd in \a openMode mode. Returns true
    on success; otherwise returns false.

    The \a handleFlags argument specifies whether the file handle will be
    closed by Katie. See the QFile::FileHandleFlags documentation for more
    information.
*/
bool QAbstractFileEngine::open(QIODevice::OpenMode openMode, int fd, QFile::FileHandleFlags handleFlags)
{
    Q_D(QAbstractFileEngine);

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

QString QAbstractFileEngine::currentPath(const QString &)
{
    return QFileSystemEngine::currentPath().filePath();
}

QString QAbstractFileEngine::homePath()
{
    return QFileSystemEngine::homePath();
}

QString QAbstractFileEngine::rootPath()
{
    return QFileSystemEngine::rootPath();
}

QString QAbstractFileEngine::tempPath()
{
    return QFileSystemEngine::tempPath();
}

/*!
    \since 4.3
    \class QAbstractFileEngineIterator
    \brief The QAbstractFileEngineIterator class provides an iterator
    interface for file engines.

    If all you want is to iterate over entries in a directory, see
    QDirIterator instead. This class is only for internal file engines.

    QAbstractFileEngineIterator is a unidirectional single-use iterator
    that plugs into QDirIterator, providing transparent proxy iteration for
    file engines.

    QAbstractFileEngineIterator is associated with a path, name filters, and
    entry filters. The path is the directory that the iterator lists entries
    in. The name filters and entry filters are provided for file engines that
    can optimize directory listing at the iterator level (e.g., network file
    systems that need to minimize network traffic), but they can also be
    ignored by the iterator; QAbstractFileEngineIterator already provides the
    required filtering logics in the matchesFilters() function. You can call
    dirName() to get the directory name, nameFilters() to get a stringlist of
    name filters, and filters() to get the entry filters.

    The function hasNext() returns true if the current directory has at least
    one more entry (i.e., the directory name is valid and accessible, and we
    have not reached the end of the entry list), and false otherwise.
    next() seeks to the next entry.

    The function currentFileName() returns the name of the current entry
    without advancing the iterator. The currentFilePath() function is
    provided for convenience; it returns the full path of the current entry.

    Note: QAbstractFileEngineIterator does not deal with QDir::IteratorFlags;
    it simply returns entries for a single directory.

    \sa QDirIterator
*/

/*!
    \typedef QAbstractFileEngine::Iterator
    \since 4.3
    \relates QAbstractFileEngine

    Synonym for QAbstractFileEngineIterator.
*/

class QAbstractFileEngineIteratorPrivate
{
public:
    QAbstractFileEngineIteratorPrivate();
    ~QAbstractFileEngineIteratorPrivate();

    QString path;
    QDir::Filters filters;
    QStringList nameFilters;
    QFileInfo fileInfo;

#ifndef QT_NO_FILESYSTEMITERATOR
    void advance() const;

    mutable QFileSystemIterator* nativeIterator;
    mutable QFileInfo currentInfo;
    mutable QFileInfo nextInfo;
    mutable bool done;
#endif
};

QAbstractFileEngineIteratorPrivate::QAbstractFileEngineIteratorPrivate()
#ifndef QT_NO_FILESYSTEMITERATOR
    : nativeIterator(nullptr)
    , done(false)
#endif
{
}

QAbstractFileEngineIteratorPrivate::~QAbstractFileEngineIteratorPrivate()
{
#ifndef QT_NO_FILESYSTEMITERATOR
    delete nativeIterator;
#endif
}

#ifndef QT_NO_FILESYSTEMITERATOR
void QAbstractFileEngineIteratorPrivate::advance() const
{
    currentInfo = nextInfo;

    QFileSystemEntry entry;
    QFileSystemMetaData data;
    if (nativeIterator->advance(entry, data)) {
        nextInfo = QFileInfo(new QFileInfoPrivate(entry, data));
    } else {
        done = true;
        delete nativeIterator;
        nativeIterator = nullptr;
    }
}
#endif // QT_NO_FILESYSTEMITERATOR

/*!
    Constructs a QAbstractFileEngineIterator, using the entry filters \a
    filters, and wildcard name filters \a nameFilters.
*/
QAbstractFileEngineIterator::QAbstractFileEngineIterator(QDir::Filters filters,
                                                         const QStringList &nameFilters)
    : d(new QAbstractFileEngineIteratorPrivate())
{
    d->nameFilters = nameFilters;
    d->filters = filters;
}

/*!
    Destroys the QAbstractFileEngineIterator.

    \sa QDirIterator
*/
QAbstractFileEngineIterator::~QAbstractFileEngineIterator()
{
    delete d;
}

/*!
    Returns the path for this iterator. QDirIterator is responsible for
    assigning this path; it cannot change during the iterator's lifetime.

    \sa nameFilters(), filters()
*/
QString QAbstractFileEngineIterator::path() const
{
    return d->path;
}

/*!
    \internal

    Sets the iterator path to \a path. This function is called from within
    QDirIterator.
*/
void QAbstractFileEngineIterator::setPath(const QString &path)
{
    d->path = path;
}

/*!
    Returns the name filters for this iterator.

    \sa QDir::nameFilters(), filters(), path()
*/
QStringList QAbstractFileEngineIterator::nameFilters() const
{
    return d->nameFilters;
}

/*!
    Returns the entry filters for this iterator.

    \sa QDir::filter(), nameFilters(), path()
*/
QDir::Filters QAbstractFileEngineIterator::filters() const
{
    return d->filters;
}

/*!
    Returns the name of the current directory entry, excluding the path.

    \sa currentFilePath()
*/
QString QAbstractFileEngineIterator::currentFileName() const
{
#ifndef QT_NO_FILESYSTEMITERATOR
    return d->currentInfo.fileName();
#else
    return QString();
#endif
}

/*!
    Returns the path to the current directory entry. It's the same as
    prepending path() to the return value of currentFileName().

    \sa currentFileName()
*/
QString QAbstractFileEngineIterator::currentFilePath() const
{
    QString name = currentFileName();
    if (!name.isNull()) {
        QString tmp = path();
        if (!tmp.isEmpty()) {
            if (!tmp.endsWith(QLatin1Char('/')))
                tmp.append(QLatin1Char('/'));
            name.prepend(tmp);
        }
    }
    return name;
}

/*!
    Returns a QFileInfo for the current directory entry. This function is
    provided for convenience. It can also be slightly faster than creating a
    QFileInfo object yourself, as the object returned by this function might
    contain cached information that QFileInfo otherwise would have to access
    through the file engine.

    \sa currentFileName()
*/
QFileInfo QAbstractFileEngineIterator::currentFileInfo() const
{
#ifndef QT_NO_FILESYSTEMITERATOR
    return d->currentInfo;
#else
    return QFileInfo();
#endif
}

/*!
    Advances the iterator to the next directory entry, and returns the file
    path to the current entry.

    This function can optionally make use of nameFilters() and filters() to
    optimize its performance.

    \sa QDirIterator::next()
*/
QString QAbstractFileEngineIterator::next()
{
#ifndef QT_NO_FILESYSTEMITERATOR
    if (!hasNext())
        return QString();

    d->advance();
    return currentFilePath();
#else
    return QString();
#endif
}

/*!
    Returns true if there is at least one more entry in the current
    directory (i.e., the iterator path is valid and accessible, and the
    iterator has not reached the end of the entry list).

    \sa QDirIterator::hasNext()
*/
bool QAbstractFileEngineIterator::hasNext() const
{
#ifndef QT_NO_FILESYSTEMITERATOR
    if (!d->done && !d->nativeIterator) {
        d->nativeIterator = new QFileSystemIterator(QFileSystemEntry(path()));
        d->advance();
    }

    return !d->done;
#else
    return false;
#endif
}

QT_END_NAMESPACE
