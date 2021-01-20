/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the qmake spec of the Katie Toolkit.
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

#ifndef QPLATFORMDEFS_H
#define QPLATFORMDEFS_H

// Get defines/settings
#include "qconfig.h"

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

// use LFS extension if required macros are defined, unless interface
// offset is set to 64 in which case both interfaces are the same.
// the host must support all of the functions, if that is the case
// QT_LARGEFILE_SUPPORT is defined
#if defined(_LARGEFILE64_SOURCE) && defined(_LARGEFILE_SOURCE) \
    && defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS != 64 \
    && defined(QT_LARGEFILE_SUPPORT)

#define QT_STATBUF              struct stat64
#define QT_FPOS_T               fpos64_t
#define QT_OFF_T                off64_t

#define QT_STAT                 ::stat64
#define QT_LSTAT                ::lstat64
#define QT_FSTAT                ::fstat64

#define QT_OPEN                 ::open64
#define QT_CREAT                ::creat64
#define QT_LSEEK                ::lseek64
#define QT_TRUNCATE             ::truncate64
#define QT_FOPEN                ::fopen64
#define QT_FSEEK                ::fseeko64
#define QT_FTELL                ::ftello64
#define QT_FGETPOS              ::fgetpos64
#define QT_FSETPOS              ::fsetpos64
#define QT_FTRUNCATE            ::ftruncate64
#define QT_MMAP                 ::mmap64

#define QT_DIRENT               struct dirent64
#define QT_READDIR              ::readdir64
#define QT_READDIR_R            ::readdir64_r

#define QT_OPEN_LARGEFILE       O_LARGEFILE

#else // QT_LARGEFILE_SUPPORT

#define QT_STATBUF              struct stat
#define QT_FPOS_T               fpos_t
#define QT_OFF_T                long

#define QT_STAT                 ::stat
#define QT_LSTAT                ::lstat
#define QT_FSTAT                ::fstat
#define QT_OPEN                 ::open
#define QT_CREAT                ::creat
#define QT_LSEEK                ::lseek
#define QT_TRUNCATE             ::truncate
#define QT_FOPEN                ::fopen
#define QT_FSEEK                ::fseek
#define QT_FTELL                ::ftell
#define QT_FGETPOS              ::fgetpos
#define QT_FSETPOS              ::fsetpos
#define QT_FTRUNCATE            ::ftruncate
#define QT_MMAP                 ::mmap

#define QT_DIRENT               struct dirent
#define QT_READDIR              ::readdir
#define QT_READDIR_R            ::readdir_r

// Posix extensions to C89
#if !defined(QT_NO_USE_FSEEKO)
#undef QT_OFF_T
#undef QT_FSEEK
#undef QT_FTELL

#define QT_OFF_T                off_t

#define QT_FSEEK                ::fseeko
#define QT_FTELL                ::ftello
#endif

#define QT_OPEN_LARGEFILE       0

#endif // QT_LARGEFILE_SUPPORT

#define QT_STAT_MASK            S_IFMT
#define QT_STAT_REG             S_IFREG
#define QT_STAT_DIR             S_IFDIR
#define QT_STAT_LNK             S_IFLNK

#define QT_ACCESS               ::access
#define QT_GETCWD               ::getcwd
#define QT_CHDIR                ::chdir
#define QT_MKDIR                ::mkdir
#define QT_RMDIR                ::rmdir
#define QT_CLOSE                ::close
#define QT_READ                 ::read
#define QT_WRITE                ::write

#define QT_OPEN_RDONLY          O_RDONLY
#define QT_OPEN_WRONLY          O_WRONLY
#define QT_OPEN_RDWR            O_RDWR
#define QT_OPEN_CREAT           O_CREAT
#define QT_OPEN_TRUNC           O_TRUNC
#define QT_OPEN_APPEND          O_APPEND

// Posix extensions to C89
#define QT_FILENO               fileno // defined as macro on OpenBSD

#define QT_DIR                  DIR

#define QT_OPENDIR              ::opendir
#define QT_CLOSEDIR             ::closedir

#define QT_SOCKLEN_T            socklen_t

#define QT_SOCKET_CONNECT       ::connect
#define QT_SOCKET_BIND          ::bind

#define QT_BUFFSIZE BUFSIZ

#endif // include guard
