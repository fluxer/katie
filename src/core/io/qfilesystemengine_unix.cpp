/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qplatformdefs.h"
#include "qfilesystemengine_p.h"
#include "qfsfileengine.h"
#include "qfile.h"
#include "qfileinfo.h"
#include "qcore_unix_p.h"

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

//static
QFileSystemEntry QFileSystemEngine::getLinkTarget(const QFileSystemEntry &link, QFileSystemMetaData &data)
{
    char readlinkbuf[PATH_MAX];
    int len = ::readlink(link.nativeFilePath().constData(), readlinkbuf, sizeof(readlinkbuf));
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
        readlinkbuf[len] = '\0';
        ret += QFile::decodeName(QByteArray(readlinkbuf));

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

    char *ret = ::realpath(entry.nativeFilePath().constData(), (char*)0);
    if (ret) {
        data.knownFlagsMask |= QFileSystemMetaData::ExistsAttribute;
        data.entryFlags |= QFileSystemMetaData::ExistsAttribute;
        QString canonicalPath = QDir::cleanPath(QString::fromLocal8Bit(ret));
        ::free(ret);
        return QFileSystemEntry(canonicalPath);
    } else if (errno == ENOENT) { // file doesn't exist
        data.knownFlagsMask |= QFileSystemMetaData::ExistsAttribute;
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

//static
QString QFileSystemEngine::resolveUserName(uint userId)
{
#if !defined(QT_NO_THREAD) && defined(QT_HAVE_GETPWUID_R)
    static int size_max = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (size_max == -1)
        size_max = 1024;
    char buf[size_max];
    struct passwd entry;
    struct passwd *pw = Q_NULLPTR;
    ::getpwuid_r(userId, &entry, buf, size_max, &pw);
#else
    struct passwd *pw = ::getpwuid(userId);
#endif
    if (pw)
        return QFile::decodeName(QByteArray(pw->pw_name));
    return QString();
}

//static
QString QFileSystemEngine::resolveGroupName(uint groupId)
{
#if !defined(QT_NO_THREAD) && defined(QT_HAVE_GETGRGID_R)
    static int size_max = sysconf(_SC_GETGR_R_SIZE_MAX);
    if (size_max == -1)
        size_max = 1024;
    char buf[size_max];
    struct group entry;
    struct group *gr = Q_NULLPTR;
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

    const QByteArray &path = entry.nativeFilePath();
    bool entryExists = true; // innocent until proven otherwise

    QT_STATBUF statBuffer;
    bool statBufferValid = false;
    if (what & QFileSystemMetaData::LinkType) {
        if (QT_LSTAT(path.constData(), &statBuffer) == 0) {
            if (S_ISLNK(statBuffer.st_mode)) {
                data.entryFlags |= QFileSystemMetaData::LinkType;
            } else {
                statBufferValid = true;
                data.entryFlags &= ~QFileSystemMetaData::PosixStatFlags;
            }
        } else {
            entryExists = false;
        }

        data.knownFlagsMask |= QFileSystemMetaData::LinkType;
    }

    if (statBufferValid || (what & QFileSystemMetaData::PosixStatFlags)) {
        if (entryExists && !statBufferValid)
            statBufferValid = (QT_STAT(path.constData(), &statBuffer) == 0);

        if (statBufferValid)
            data.fillFromStatBuf(statBuffer);
        else {
            entryExists = false;
            data.creationTime_ = 0;
            data.modificationTime_ = 0;
            data.accessTime_ = 0;
            data.size_ = 0;
            data.userId_ = (uint) -2;
            data.groupId_ = (uint) -2;
        }

        // reset the mask
        data.knownFlagsMask |= QFileSystemMetaData::PosixStatFlags
            | QFileSystemMetaData::ExistsAttribute;
    }

    if (what & QFileSystemMetaData::UserPermissions) {
        // calculate user permissions

        if (entryExists) {
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
        data.knownFlagsMask |= (what & QFileSystemMetaData::UserPermissions);
    }

    if (what & QFileSystemMetaData::HiddenAttribute && !data.isHidden()) {
        const QString &fileName = entry.fileName();
        if (fileName.size() > 0 && fileName.at(0) == QLatin1Char('.'))
            data.entryFlags |= QFileSystemMetaData::HiddenAttribute;
        data.knownFlagsMask |= QFileSystemMetaData::HiddenAttribute;
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
                    if ((st.st_mode & S_IFMT) != S_IFDIR)
                        return false;
                } else if (QT_MKDIR(chunk.constData(), 0777) != 0) {
                    return false;
                }
            }
        }
        return true;
    }
    const QByteArray eDirName = QFile::encodeName(dirName);
    return (QT_MKDIR(eDirName.constData(), 0777) == 0);
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
                if ((st.st_mode & S_IFMT) != S_IFDIR)
                    return false;
                if (::rmdir(chunk.constData()) != 0)
                    return oldslash != 0;
            } else {
                return false;
            }
            slash = dirName.lastIndexOf(QDir::separator(), oldslash-1);
        }
        return true;
    }
    const QByteArray eDirName = QFile::encodeName(entry.filePath());
    return ::rmdir(eDirName.constData()) == 0;
}

//static
bool QFileSystemEngine::createLink(const QFileSystemEntry &source, const QFileSystemEntry &target, int *error)
{
    if (::symlink(source.nativeFilePath().constData(), target.nativeFilePath().constData()) == 0)
        return true;
    *error = errno;
    return false;
}

//static
bool QFileSystemEngine::copyFile(const QFileSystemEntry &source, const QFileSystemEntry &target, int *error)
{
    QT_STATBUF st;
    if (QT_STAT(source.nativeFilePath().constData(), &st) == 0) {
        if (!S_ISREG(st.st_mode))
            return false;
    }

    const int sourcefd = QT_OPEN(source.nativeFilePath().constData(), O_RDONLY);
    if (sourcefd == -1) {
        *error = errno;
        return false;
    }

    const int targetfd = QT_CREAT(target.nativeFilePath().constData(), st.st_mode);
    if (targetfd == -1) {
        *error = errno;
        ::close(sourcefd);
        return false;
    }

    QT_OFF_T tocopy = st.st_size;
#ifdef Q_OS_LINUX
// not in qplatformdefs.h since it is platform specific
#if defined(QT_LARGEFILE_SUPPORT)
#  define QT_SENDFILE ::sendfile64
#else
#  define QT_SENDFILE ::sendfile
#endif
    ssize_t sendresult = QT_SENDFILE(targetfd, sourcefd, Q_NULLPTR, tocopy);
    while (sendresult != tocopy) {
        if (sendresult == -1) {
            *error = errno;
            ::close(sourcefd);
            ::close(targetfd);
            return false;
        }
        tocopy -= sendresult;
        sendresult = QT_SENDFILE(targetfd, sourcefd, &tocopy, tocopy);
    }
#undef QT_SENDFILE
#elif defined(Q_OS_FREEBSD)
    QT_OFF_T totalwrite = 0;
    int sendresult = ::sendfile(sourcefd, targetfd, QT_OFF_T(0), size_t(0), Q_NULLPTR, &totalwrite, SF_SYNC);
    if (QT_OFF_T(sendresult) != totalwrite) {
        *error = errno;
        ::close(sourcefd);
        ::close(targetfd);
        return false;
    }
#else
    size_t totalwrite = 0;
    char copybuffer[QT_BUFFSIZE];
    while (QT_OFF_T(totalwrite) != tocopy) {
        const size_t readresult = QT_READ(sourcefd, copybuffer, sizeof(copybuffer));
        if (readresult == -1) {
            *error = errno;
            ::close(sourcefd);
            ::close(targetfd);
            return false;
        }

        const size_t writeresult = QT_WRITE(targetfd, copybuffer, readresult);
        if (writeresult != readresult) {
            *error = errno;
            ::close(sourcefd);
            ::close(targetfd);
            return false;
        }

        totalwrite += readresult;
    }
#endif

    ::close(sourcefd);
    ::close(targetfd);
    return true;
}

//static
bool QFileSystemEngine::renameFile(const QFileSystemEntry &source, const QFileSystemEntry &target, int *error)
{
    if (::rename(source.nativeFilePath().constData(), target.nativeFilePath().constData()) == 0)
        return true;
    *error = errno;
    return false;
}

//static
bool QFileSystemEngine::removeFile(const QFileSystemEntry &entry, int *error)
{
    if (unlink(entry.nativeFilePath().constData()) == 0)
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

    if (::chmod(entry.nativeFilePath().constData(), mode) == 0)
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

bool QFileSystemEngine::setCurrentPath(const QFileSystemEntry &path)
{
    const char* cPath = path.nativeFilePath().constData();
    return (QT_CHDIR(cPath) >= 0);
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
    char getcwdbuffer[PATH_MAX];
    if (::getcwd(getcwdbuffer, sizeof(getcwdbuffer))) {
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
QT_END_NAMESPACE
