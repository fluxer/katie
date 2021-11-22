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

// There are two incompatible versions of strerror_r:
// a) the XSI/POSIX.1 version, which returns an int,
//    indicating success or not
// b) the GNU version, which returns a char*, which may or may not
//    be the beginning of the buffer we used
// The GNU libc manpage for strerror_r says you should use the the XSI
// version in portable code.
//
// to get the XSI/POSIX.1 version _GNU_SOURCE must not be defined
#undef _GNU_SOURCE

#include "qcorecommon_p.h"

#include <string.h>
#include <errno.h>

QT_BEGIN_NAMESPACE

QString qt_error_string(int errorCode)
{
#if !defined(QT_NO_THREAD)
    QSTACKARRAY(char, errbuf, 1024);
    ::strerror_r(errorCode, errbuf, sizeof(errbuf));
    return QString::fromLocal8Bit(errbuf);
#else
    return QString::fromLocal8Bit(::strerror(errorCode));
#endif
}

QT_END_NAMESPACE
