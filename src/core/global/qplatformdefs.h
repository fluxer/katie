/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QPLATFORMDEFS_H
#define QPLATFORMDEFS_H

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

// Definitions
#define QT_STAT_MASK            S_IFMT
#define QT_STAT_REG             S_IFREG
#define QT_STAT_DIR             S_IFDIR
#define QT_STAT_LNK             S_IFLNK
#define QT_OPEN_RDONLY          O_RDONLY
#define QT_OPEN_WRONLY          O_WRONLY
#define QT_OPEN_RDWR            O_RDWR
#define QT_OPEN_CREAT           O_CREAT
#define QT_OPEN_TRUNC           O_TRUNC
#define QT_OPEN_APPEND          O_APPEND
#define QT_BUFFSIZE             BUFSIZ

// Types
#define QT_FPOS_T               fpos_t
#define QT_OFF_T                off_t
#define QT_SOCKLEN_T            socklen_t

// Structures
#define QT_STATBUF              struct stat
#define QT_DIR                  DIR
#define QT_DIRENT               struct dirent

// Functions (could be defined as macros)
#define QT_STAT                 ::stat
#define QT_LSTAT                ::lstat
#define QT_FSTAT                ::fstat
#define QT_OPEN                 ::open
#define QT_CREAT                ::creat
#define QT_LSEEK                ::lseek
#define QT_TRUNCATE             ::truncate
#define QT_FOPEN                ::fopen
#define QT_FSEEK                ::fseeko
#define QT_FTELL                ::ftello
#define QT_FGETPOS              ::fgetpos
#define QT_FSETPOS              ::fsetpos
#define QT_FTRUNCATE            ::ftruncate
#define QT_MMAP                 ::mmap
#define QT_READDIR              ::readdir
#define QT_READDIR_R            ::readdir_r
#define QT_ACCESS               ::access
#define QT_GETCWD               ::getcwd
#define QT_CHDIR                ::chdir
#define QT_MKDIR                ::mkdir
#define QT_RMDIR                ::rmdir
#define QT_CLOSE                ::close
#define QT_READ                 ::read
#define QT_WRITE                ::write
#define QT_OPENDIR              ::opendir
#define QT_CLOSEDIR             ::closedir
#define QT_FILENO               fileno // defined as macro on OpenBSD
#define QT_SOCKET_CONNECT       ::connect
#define QT_SOCKET_BIND          ::bind

#endif // QPLATFORMDEFS_H
