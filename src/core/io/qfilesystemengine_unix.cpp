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
#include "qfilesystemengine_p.h"
#include "qfsfileengine.h"
#include "qfile.h"
#include "qcore_unix_p.h"
#include "qcorecommon_p.h"

#include <errno.h>
#include <pwd.h>
#include <grp.h>

#ifdef Q_OS_LINUX
#  include <sys/sendfile.h>
#endif

#ifndef PATH_MAX
#  define PATH_MAX _POSIX_PATH_MAX
#endif

QT_BEGIN_NAMESPACE

const uint QFileSystemMetaData::nobodyID = (uint) -2;

//static
QFileSystemEntry QFileSystemEngine::getLinkTarget(const QFileSystemEntry &link, QFileSystemMetaData &data)
{
    QByteArray lpath = link.nativeFilePath();
    QSTACKARRAY(char, readlinkbuf, PATH_MAX);
    int len = ::readlink(lpath.constData(), readlinkbuf, sizeof(readlinkbuf));
    if (len > 0) {
        QString ret;
        if (!data.hasFlags(QFileSystemMetaData::DirectoryType))
            fillMetaData(link, data, QFileSystemMetaData::DirectoryType);
        if (data.isDirectory() && readlinkbuf[0] != '/') {
            QDir parent(link.filePath());
            parent.cdUp();
            ret = parent.path();
            if (!ret.isEmpty() && !ret.endsWith(QLatin1Char('/')))
                ret += QLatin1Char('/');
        }
        ret += QFile::decodeName(QByteArray(readlinkbuf, len));

        if (!ret.startsWith(QLatin1Char('/'))) {
            if (link.filePath().startsWith(QLatin1Char('/'))) {
                ret.prepend(link.filePath().left(link.filePath().lastIndexOf(QLatin1Char('/')))
                            + QLatin1Char('/'));
            } else {
                ret.prepend(QDir::currentPath() + QLatin1Char('/'));
            }
        }
        ret = QDir::cleanPath(ret);
        if (ret.size() > 1 && ret.endsWith(QLatin1Char('/')))
            ret.chop(1);
        return QFileSystemEntry(ret);
    }
    return QFileSystemEntry();
}

//static
QFileSystemEntry QFileSystemEngine::canonicalName(const QFileSystemEntry &entry, QFileSystemMetaData &data)
{
    if (entry.isEmpty() || entry.isRoot())
        return entry;

    QByteArray path = entry.nativeFilePath();
    QSTACKARRAY(char, realpathbuf, PATH_MAX);
    char *ret = ::realpath(path.constData(), realpathbuf);
    if (ret) {
        data.entryFlags |= QFileSystemMetaData::ExistsAttribute;
        QString canonicalPath = QDir::cleanPath(QString::fromLocal8Bit(ret));
        return QFileSystemEntry(canonicalPath);
    } else if (errno == ENOENT) { // file doesn't exist
        data.entryFlags &= ~(QFileSystemMetaData::ExistsAttribute);
        return QFileSystemEntry();
    }
    return entry;
}

//static
QFileSystemEntry QFileSystemEngine::absoluteName(const QFileSystemEntry &entry)
{
    if (entry.isAbsolute() && entry.isClean())
        return entry;

    QByteArray orig = entry.nativeFilePath();
    QByteArray result;
    if (orig.isEmpty() || !orig.startsWith('/')) {
        QFileSystemEntry cur(currentPath());
        result = cur.nativeFilePath();
    }
    if (!orig.isEmpty() && !(orig.length() == 1 && orig[0] == '.')) {
        if (!result.isEmpty() && !result.endsWith('/'))
            result.append('/');
        result.append(orig);
    }

    if (result.length() == 1 && result[0] == '/')
        return QFileSystemEntry(result, QFileSystemEntry::FromNativePath());
    const bool isDir = result.endsWith('/');

    /* as long as QDir::cleanPath() operates on a QString we have to convert to a string here.
     * ideally we never convert to a string since that loses information. Please fix after
     * we get a QByteArray version of QDir::cleanPath()
     */
    QFileSystemEntry resultingEntry(result, QFileSystemEntry::FromNativePath());
    QString stringVersion = QDir::cleanPath(resultingEntry.filePath());
    if (isDir)
        stringVersion.append(QLatin1Char('/'));
    return QFileSystemEntry(stringVersion);
}

Q_CORE_EXPORT QString qt_resolveUserName(uint userId)
{
#if !defined(QT_NO_THREAD)
    static long size_max = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (size_max == -1)
        size_max = 1024;
    QSTACKARRAY(char, buf, size_max);
    struct passwd entry;
    struct passwd *pw = nullptr;
    ::getpwuid_r(userId, &entry, buf, size_max, &pw);
#else
    struct passwd *pw = ::getpwuid(userId);
#endif
    if (pw)
        return QFile::decodeName(QByteArray(pw->pw_name));
    return QString();
}

//static
QString QFileSystemEngine::resolveUserName(uint userId)
{
    return qt_resolveUserName(userId);
}

//static
QString QFileSystemEngine::resolveGroupName(uint groupId)
{
#if !defined(QT_NO_THREAD)
    static long size_max = sysconf(_SC_GETGR_R_SIZE_MAX);
    if (size_max == -1)
        size_max = 1024;
    QSTACKARRAY(char, buf, size_max);
    struct group entry;
    struct group *gr = nullptr;
    ::getgrgid_r(groupId, &entry, buf, size_max, &gr);
#else
    struct group *gr = ::getgrgid(groupId);
#endif
    if (gr)
        return QFile::decodeName(QByteArray(gr->gr_name));
    return QString();
}

//static
bool QFileSystemEngine::fillMetaData(const QFileSystemEntry &entry, QFileSystemMetaData &data,
        QFileSystemMetaData::MetaDataFlags what)
{
    if (what & QFileSystemMetaData::ExistsAttribute) {
        //  FIXME:  Would other queries being performed provide this bit?
        what |= QFileSystemMetaData::PosixStatFlags;
    }

    data.entryFlags &= ~what;

    const QByteArray path = entry.nativeFilePath();

    QT_STATBUF statBuffer;
    bool statBufferValid = false;
    if (what & QFileSystemMetaData::LinkType) {
        if (QT_LSTAT(path.constData(), &statBuffer) == 0) {
            statBufferValid = true;
            if (S_ISLNK(statBuffer.st_mode))
                data.entryFlags |= QFileSystemMetaData::LinkType;
        }
    }

    // for compatibility obtain values from the file or link, not the resolved file
    if (!statBufferValid || (what & QFileSystemMetaData::PosixStatFlags))
        statBufferValid = (QT_STAT(path.constData(), &statBuffer) == 0);

    if (statBufferValid) {
        data.fillFromStatBuf(statBuffer);
    } else {
        data.creationTime_ = 0;
        data.modificationTime_ = 0;
        data.accessTime_ = 0;
        data.size_ = 0;
        data.userId_ = QFileSystemMetaData::nobodyID;
        data.groupId_ = QFileSystemMetaData::nobodyID;
    }

    if (what & QFileSystemMetaData::UserPermissions) {
        // calculate user permissions
        if (what & QFileSystemMetaData::UserReadPermission) {
            if (QT_ACCESS(path.constData(), R_OK) == 0)
                data.entryFlags |= QFileSystemMetaData::UserReadPermission;
        }
        if (what & QFileSystemMetaData::UserWritePermission) {
            if (QT_ACCESS(path.constData(), W_OK) == 0)
                data.entryFlags |= QFileSystemMetaData::UserWritePermission;
        }
        if (what & QFileSystemMetaData::UserExecutePermission) {
            if (QT_ACCESS(path.constData(), X_OK) == 0)
                data.entryFlags |= QFileSystemMetaData::UserExecutePermission;
        }
    }

    if (what & QFileSystemMetaData::HiddenAttribute) {
        const QString &fileName = entry.fileName();
        if (fileName.size() > 0 && fileName.at(0) == QLatin1Char('.'))
            data.entryFlags |= QFileSystemMetaData::HiddenAttribute;
    }

    return data.hasFlags(what);
}

//static
bool QFileSystemEngine::createDirectory(const QFileSystemEntry &entry, bool createParents)
{
    QString dirName = entry.filePath();
    if (createParents) {
        dirName = QDir::cleanPath(dirName);
        for (int oldslash = -1, slash=0; slash != -1; oldslash = slash) {
            slash = dirName.indexOf(QDir::separator(), oldslash+1);
            if (slash == -1) {
                if (oldslash == dirName.length())
                    break;
                slash = dirName.length();
            }
            if (slash) {
                const QByteArray chunk = QFile::encodeName(dirName.left(slash));
                QT_STATBUF st;
                if (QT_STAT(chunk.constData(), &st) == 0) {
                    if (!S_ISDIR(st.st_mode))
                        return false;
                } else if (QT_MKDIR(chunk.constData(), 0777) != 0) {
                    return false;
                }
            }
        }
        return true;
    }
    const QByteArray path = QFile::encodeName(dirName);
    return (QT_MKDIR(path.constData(), 0777) == 0);
}

//static
bool QFileSystemEngine::removeDirectory(const QFileSystemEntry &entry, bool removeEmptyParents)
{
    if (removeEmptyParents) {
        QString dirName = QDir::cleanPath(entry.filePath());
        for (int oldslash = 0, slash=dirName.length(); slash > 0; oldslash = slash) {
            const QByteArray chunk = QFile::encodeName(dirName.left(slash));
            QT_STATBUF st;
            if (QT_STAT(chunk.constData(), &st) == 0) {
                if (!S_ISDIR(st.st_mode))
                    return false;
                if (QT_RMDIR(chunk.constData()) != 0)
                    return oldslash != 0;
            } else {
                return false;
            }
            slash = dirName.lastIndexOf(QDir::separator(), oldslash-1);
        }
        return true;
    }
    const QByteArray path = QFile::encodeName(entry.filePath());
    return QT_RMDIR(path.constData()) == 0;
}

//static
bool QFileSystemEngine::createLink(const QFileSystemEntry &source, const QFileSystemEntry &target, int *error)
{
    const QByteArray spath = source.nativeFilePath();
    const QByteArray tpath = target.nativeFilePath();
    if (::symlink(spath.constData(), tpath.constData()) == 0)
        return true;
    *error = errno;
    return false;
}

//static
bool QFileSystemEngine::copyFile(const QFileSystemEntry &source, const QFileSystemEntry &target, int *error)
{
    const QByteArray spath = source.nativeFilePath();
    const QByteArray tpath = target.nativeFilePath();
    QT_STATBUF st;
    if (QT_STAT(spath.constData(), &st) == 0) {
        if (!S_ISREG(st.st_mode))
            return false;
    }

    const int sourcefd = qt_safe_open(spath.constData(), O_RDONLY);
    if (sourcefd == -1) {
        *error = errno;
        return false;
    }

    const int targetfd = qt_safe_creat(tpath.constData(), st.st_mode);
    if (targetfd == -1) {
        *error = errno;
        qt_safe_close(sourcefd);
        return false;
    }

    QT_OFF_T tocopy = st.st_size;
    QT_OFF_T totalwrite = 0;
#ifdef Q_OS_LINUX
    while (totalwrite != tocopy) {
        // sendfile64() may use internal types (different from off_t), do not use it
        const ssize_t sendresult = ::sendfile(targetfd, sourcefd, &totalwrite, tocopy - totalwrite);
        if (sendresult == -1) {
            *error = errno;
            qt_safe_close(sourcefd);
            qt_safe_close(targetfd);
            return false;
        }
    }
#else
    QSTACKARRAY(char, copybuffer, QT_BUFFSIZE);
    while (totalwrite != tocopy) {
        const qint64 readresult = qt_safe_read(sourcefd, copybuffer, sizeof(copybuffer));
        if (readresult == -1) {
            *error = errno;
            qt_safe_close(sourcefd);
            qt_safe_close(targetfd);
            return false;
        }

        const qint64 writeresult = qt_safe_write(targetfd, copybuffer, readresult);
        if (writeresult != readresult) {
            *error = errno;
            qt_safe_close(sourcefd);
            qt_safe_close(targetfd);
            return false;
        }

        totalwrite += readresult;
    }
#endif

    qt_safe_close(sourcefd);
    qt_safe_close(targetfd);
    return true;
}

//static
bool QFileSystemEngine::renameFile(const QFileSystemEntry &source, const QFileSystemEntry &target, int *error)
{
    const QByteArray spath = source.nativeFilePath();
    const QByteArray tpath = target.nativeFilePath();
#ifdef QT_HAVE_RENAMEAT2
    if (::renameat2(AT_FDCWD, spath.constData(), AT_FDCWD, tpath.constData(), RENAME_NOREPLACE) == 0)
        return true;
#else
    if (::rename(spath.constData(), tpath.constData()) == 0)
        return true;
#endif
    *error = errno;
    return false;
}

//static
bool QFileSystemEngine::removeFile(const QFileSystemEntry &entry, int *error)
{
    const QByteArray path = entry.nativeFilePath();
    if (unlink(path.constData()) == 0)
        return true;
    *error = errno;
    return false;

}

//static
bool QFileSystemEngine::setPermissions(const QFileSystemEntry &entry, QFile::Permissions permissions, int *error)
{
    mode_t mode = 0;
    if (permissions & QFile::ReadOwner)
        mode |= S_IRUSR;
    if (permissions & QFile::WriteOwner)
        mode |= S_IWUSR;
    if (permissions & QFile::ExeOwner)
        mode |= S_IXUSR;
    if (permissions & QFile::ReadUser)
        mode |= S_IRUSR;
    if (permissions & QFile::WriteUser)
        mode |= S_IWUSR;
    if (permissions & QFile::ExeUser)
        mode |= S_IXUSR;
    if (permissions & QFile::ReadGroup)
        mode |= S_IRGRP;
    if (permissions & QFile::WriteGroup)
        mode |= S_IWGRP;
    if (permissions & QFile::ExeGroup)
        mode |= S_IXGRP;
    if (permissions & QFile::ReadOther)
        mode |= S_IROTH;
    if (permissions & QFile::WriteOther)
        mode |= S_IWOTH;
    if (permissions & QFile::ExeOther)
        mode |= S_IXOTH;

    const QByteArray path = entry.nativeFilePath();
    if (::chmod(path.constData(), mode) == 0)
        return true;
    *error = errno;
    return false;
}

QString QFileSystemEngine::homePath()
{
    const QString home = QFile::decodeName(qgetenv("HOME"));
    if (Q_LIKELY(!home.isEmpty()))
        return QDir::cleanPath(home);
    return rootPath();
}

QString QFileSystemEngine::rootPath()
{
    return QLatin1String("/");
}

QString QFileSystemEngine::tempPath()
{
    const QString temp = QFile::decodeName(qgetenv("TMPDIR"));
    if (!temp.isEmpty())
        return QDir::cleanPath(temp);
    return QLatin1String("/tmp");
}

bool QFileSystemEngine::setCurrentPath(const QFileSystemEntry &entry)
{
    const QByteArray path = entry.nativeFilePath();
    return (QT_CHDIR(path.constData()) >= 0);
}

QFileSystemEntry QFileSystemEngine::currentPath()
{
    QFileSystemEntry result;
#ifdef QT_HAVE_GET_CURRENT_DIR_NAME
#define GETCWDFUNCNAME "get_current_dir_name"
    char *currentdir = ::get_current_dir_name();
    if (currentdir) {
        result = QFileSystemEntry(QByteArray(currentdir), QFileSystemEntry::FromNativePath());
        ::free(currentdir);
    }
#else
#define GETCWDFUNCNAME "getcwd"
    QSTACKARRAY(char, getcwdbuffer, PATH_MAX);
    if (QT_GETCWD(getcwdbuffer, sizeof(getcwdbuffer))) {
        result = QFileSystemEntry(QByteArray(getcwdbuffer), QFileSystemEntry::FromNativePath());
    }
#endif // QT_HAVE_GET_CURRENT_DIR_NAME

#ifndef QT_NO_DEBUG
    if (result.isEmpty())
        qWarning("QFSFileEngine::currentPath: " GETCWDFUNCNAME "() failed");
#endif
#undef GETCWDFUNCNAME

    return result;
}

bool QFileSystemEngine::fillMetaData(int fd, QFileSystemMetaData &data)
{
    data.entryFlags &= ~QFileSystemMetaData::PosixStatFlags;

    QT_STATBUF statBuffer;
    if (QT_FSTAT(fd, &statBuffer) == 0) {
        data.fillFromStatBuf(statBuffer);
        return true;
    }

    return false;
}

void QFileSystemMetaData::fillFromStatBuf(const QT_STATBUF &statBuffer)
{
    // Permissions
    if (statBuffer.st_mode & S_IRUSR)
        entryFlags |= QFileSystemMetaData::OwnerReadPermission;
    if (statBuffer.st_mode & S_IWUSR)
        entryFlags |= QFileSystemMetaData::OwnerWritePermission;
    if (statBuffer.st_mode & S_IXUSR)
        entryFlags |= QFileSystemMetaData::OwnerExecutePermission;

    if (statBuffer.st_mode & S_IRGRP)
        entryFlags |= QFileSystemMetaData::GroupReadPermission;
    if (statBuffer.st_mode & S_IWGRP)
        entryFlags |= QFileSystemMetaData::GroupWritePermission;
    if (statBuffer.st_mode & S_IXGRP)
        entryFlags |= QFileSystemMetaData::GroupExecutePermission;

    if (statBuffer.st_mode & S_IROTH)
        entryFlags |= QFileSystemMetaData::OtherReadPermission;
    if (statBuffer.st_mode & S_IWOTH)
        entryFlags |= QFileSystemMetaData::OtherWritePermission;
    if (statBuffer.st_mode & S_IXOTH)
        entryFlags |= QFileSystemMetaData::OtherExecutePermission;

    // Type
    if (S_ISREG(statBuffer.st_mode))
        entryFlags |= QFileSystemMetaData::FileType;
    else if (S_ISDIR(statBuffer.st_mode))
        entryFlags |= QFileSystemMetaData::DirectoryType;
    else
        entryFlags |= QFileSystemMetaData::SequentialType;

    // Attributes
    entryFlags |= QFileSystemMetaData::ExistsAttribute;
    entryFlags |= QFileSystemMetaData::SizeAttribute;
    size_ = statBuffer.st_size;

    // Times
    entryFlags |= QFileSystemMetaData::Times;
    creationTime_ = statBuffer.st_ctime;
    modificationTime_ = statBuffer.st_mtime;
    accessTime_ = statBuffer.st_atime;
    userId_ = statBuffer.st_uid;
    groupId_ = statBuffer.st_gid;
}

void QFileSystemMetaData::fillFromDirEnt(const QT_DIRENT &entry, const QFileSystemEntry::NativePath &nativePath)
{
    // ### This will clear all entry flags
#ifdef QT_HAVE_DIRENT_D_TYPE
    switch (entry.d_type) {
        case DT_DIR: {
            entryFlags = QFileSystemMetaData::DirectoryType
                | QFileSystemMetaData::ExistsAttribute;
            break;
        }
        case DT_BLK:
        case DT_CHR:
        case DT_FIFO:
        case DT_SOCK: {
            entryFlags = QFileSystemMetaData::SequentialType
                | QFileSystemMetaData::ExistsAttribute;
            break;
        }
        case DT_LNK: {
            entryFlags = QFileSystemMetaData::LinkType;
            break;
        }
        case DT_REG: {
            entryFlags = QFileSystemMetaData::FileType
                | QFileSystemMetaData::ExistsAttribute;
            break;
        }
        case DT_UNKNOWN:
        default: {
            clear();
        }
    }
    Q_UNUSED(nativePath);
#else
    Q_UNUSED(entry);
    QT_STATBUF statBuffer;
    if (QT_STAT(nativePath.constData(), &statBuffer) == 0) {
        if (S_ISREG(statBuffer.st_mode)) {
            entryFlags |= QFileSystemMetaData::FileType
                | QFileSystemMetaData::ExistsAttribute;
        } else if (S_ISDIR(statBuffer.st_mode)) {
            entryFlags |= QFileSystemMetaData::DirectoryType
                | QFileSystemMetaData::ExistsAttribute;
        } else if (S_ISLNK(statBuffer.st_mode)) {
            entryFlags |= QFileSystemMetaData::LinkType;
        } else if (S_ISBLK(statBuffer.st_mode) || S_ISCHR(statBuffer.st_mode)
            || S_ISFIFO(statBuffer.st_mode) || S_ISSOCK(statBuffer.st_mode)) {
            entryFlags |= QFileSystemMetaData::SequentialType
                | QFileSystemMetaData::ExistsAttribute;
        } else {
            clear();
        }
    } else {
        clear();
    }
#endif
}

//static
QString QFileSystemEngine::resolveUserName(const QFileSystemEntry &entry, QFileSystemMetaData &metaData)
{
    if (!metaData.hasFlags(QFileSystemMetaData::UserId))
        QFileSystemEngine::fillMetaData(entry, metaData, QFileSystemMetaData::UserId);
    return resolveUserName(metaData.userId());
}

//static
QString QFileSystemEngine::resolveGroupName(const QFileSystemEntry &entry, QFileSystemMetaData &metaData)
{
    if (!metaData.hasFlags(QFileSystemMetaData::GroupId))
        QFileSystemEngine::fillMetaData(entry, metaData, QFileSystemMetaData::GroupId);
    return resolveGroupName(metaData.groupId());
}

QT_END_NAMESPACE
