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

#include "qfile.h"
#include "qstringlist.h"
#include "qlibrary_p.h"
#include "qcoreapplication.h"
#include "qfilesystementry_p.h"

#ifndef QT_NO_LIBRARY

#if defined (Q_OS_NACL)
#define QT_NO_DYNAMIC_LIBRARY
#endif

#if !defined(QT_NO_DYNAMIC_LIBRARY)
#include <dlfcn.h>
#endif

QT_BEGIN_NAMESPACE

static QString qdlerror()
{
#if defined(QT_NO_DYNAMIC_LIBRARY)
    return QLatin1String("This platform does not support dynamic libraries.");
#else
    return QString::fromLatin1(::dlerror());
#endif
}

bool QLibraryPrivate::load_sys()
{
    QString attempt;
#if !defined(QT_NO_DYNAMIC_LIBRARY)
    QFileSystemEntry fsEntry(fileName);

    QString path = fsEntry.path();
    QString name = fsEntry.fileName();
    if (path == QLatin1String(".") && !fileName.startsWith(path))
        path.clear();
    else
        path += QLatin1Char('/');

    QStringList suffixes;
    QStringList prefixes;
    if (pluginState != IsAPlugin) {
        prefixes << QLatin1String("lib");
#if defined(Q_OS_HPUX)
        // according to
        // http://docs.hp.com/en/B2355-90968/linkerdifferencesiapa.htm

        // In PA-RISC (PA-32 and PA-64) shared libraries are suffixed
        // with .sl. In IPF (32-bit and 64-bit), the shared libraries
        // are suffixed with .so. For compatibility, the IPF linker
        // also supports the .sl suffix.

        // But since we don't know if we are built on HPUX or HPUXi,
        // we support both .sl (and .<version>) and .so suffixes but
        // .so is preferred.
# if defined(QT_ARCH_IA64)
        if (!fullVersion.isEmpty()) {
            suffixes << QString::fromLatin1(".so.%1").arg(fullVersion);
        } else {
            suffixes << QLatin1String(".so");
        }
# endif
        if (!fullVersion.isEmpty()) {
            suffixes << QString::fromLatin1(".sl.%1").arg(fullVersion);
            suffixes << QString::fromLatin1(".%1").arg(fullVersion);
        } else {
            suffixes << QLatin1String(".sl");
        }
#else
#ifdef Q_OS_AIX
        suffixes << ".a";
#endif // Q_OS_AIX
        if (!fullVersion.isEmpty()) {
            suffixes << QString::fromLatin1(".so.%1").arg(fullVersion);
        } else {
            suffixes << QLatin1String(".so");
        }
#endif
    }
    int dlFlags = 0;
    if (loadHints & QLibrary::ResolveAllSymbolsHint) {
        dlFlags |= RTLD_NOW;
    } else {
        dlFlags |= RTLD_LAZY;
    }
    if (loadHints & QLibrary::ExportExternalSymbolsHint) {
        dlFlags |= RTLD_GLOBAL;
    } else {
        dlFlags |= RTLD_LOCAL;
    }
#if defined(Q_OS_AIX)	// Not sure if any other platform actually support this thing.
    if (loadHints & QLibrary::LoadArchiveMemberHint) {
        dlFlags |= RTLD_MEMBER;
    }
#endif
    // If using the new search heuristics we do:
    //
    //   If the filename is an absolute path then we want to try that first as it is most likely
    //   what the callee wants. If we have been given a non-absolute path then lets try the
    //   native library name first to avoid unnecessary calls to dlopen().
    //
    // otherwise:
    //
    //   We use the old behaviour which is to always try the specified filename first
    if ((loadHints & QLibrary::ImprovedSearchHeuristics) && !fsEntry.isAbsolute()) {
        suffixes.append(QLatin1String(""));
        prefixes.append(QLatin1String(""));
    } else {
        suffixes.prepend(QLatin1String(""));
        prefixes.prepend(QLatin1String(""));
    }

    bool retry = true;
    for(int prefix = 0; retry && !pHnd && prefix < prefixes.size(); prefix++) {
        for(int suffix = 0; retry && !pHnd && suffix < suffixes.size(); suffix++) {
            if (!prefixes.at(prefix).isEmpty() && name.startsWith(prefixes.at(prefix)))
                continue;
            if (!suffixes.at(suffix).isEmpty() && name.endsWith(suffixes.at(suffix)))
                continue;
            if (loadHints & QLibrary::LoadArchiveMemberHint) {
                attempt = name;
                int lparen = attempt.indexOf(QLatin1Char('('));
                if (lparen == -1)
                    lparen = attempt.count();
                attempt = path + prefixes.at(prefix) + attempt.insert(lparen, suffixes.at(suffix));
            } else {
                attempt = path + prefixes.at(prefix) + name + suffixes.at(suffix);
            }
            pHnd = ::dlopen(QFile::encodeName(attempt).constData(), dlFlags);

            if (!pHnd && fileName.startsWith(QLatin1Char('/')) && QFile::exists(attempt)) {
                // We only want to continue if dlopen failed due to that the shared library did not exist.
                // However, we are only able to apply this check for absolute filenames (since they are
                // not influenced by the content of LD_LIBRARY_PATH, /etc/ld.so.cache, DT_RPATH etc...)
                // This is all because dlerror is flawed and cannot tell us the reason why it failed.
                retry = false;
            }
        }
    }

#endif // QT_NO_DYNAMIC_LIBRARY
    if (!pHnd) {
        errorString = QLibrary::tr("Cannot load library %1: %2").arg(fileName).arg(qdlerror());
    }
    if (pHnd) {
        qualifiedFileName = attempt;
        errorString.clear();
    }
    return (pHnd != 0);
}

bool QLibraryPrivate::unload_sys()
{
#if !defined(QT_NO_DYNAMIC_LIBRARY)
    if (::dlclose(pHnd)) {
        errorString = QLibrary::tr("Cannot unload library %1: %2").arg(fileName).arg(qdlerror());
        return false;
    }
#endif
    errorString.clear();
    return true;
}

void* QLibraryPrivate::resolve_sys(const char* symbol)
{
#if defined (QT_NO_DYNAMIC_LIBRARY)
    void *address = Q_NULLPTR;
#else
    void* address = ::dlsym(pHnd, symbol);
#endif
    if (!address) {
        errorString = QLibrary::tr("Cannot resolve symbol \"%1\" in %2: %3").arg(
            QString::fromAscii(symbol)).arg(fileName).arg(qdlerror());
    } else {
        errorString.clear();
    }
    return address;
}

QT_END_NAMESPACE

#endif // QT_NO_LIBRARY
