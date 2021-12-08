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

#include "qfile.h"
#include "qstringlist.h"
#include "qlibrary_p.h"
#include "qcoreapplication.h"

#ifndef QT_NO_LIBRARY

#include <dlfcn.h>

QT_BEGIN_NAMESPACE

static inline QString qdlerror()
{
    return QString::fromLatin1(::dlerror());
}

bool QLibraryPrivate::load_sys()
{
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

    pHnd = ::dlopen(QFile::encodeName(fileName).constData(), dlFlags);
    if (!pHnd) {
        errorString = QLibrary::tr("Cannot load library %1: %2").arg(fileName).arg(qdlerror());
    } else {
        errorString.clear();
    }
    return (pHnd != 0);
}

bool QLibraryPrivate::unload_sys()
{
    if (::dlclose(pHnd)) {
        errorString = QLibrary::tr("Cannot unload library %1: %2").arg(fileName).arg(qdlerror());
        return false;
    }
    errorString.clear();
    return true;
}

void* QLibraryPrivate::resolve_sys(const char* symbol)
{
    void* address = ::dlsym(pHnd, symbol);
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
