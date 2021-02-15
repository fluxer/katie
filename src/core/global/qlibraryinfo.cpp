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

#include "qlibraryinfo.h"

#include "qconfig.cpp"

QT_BEGIN_NAMESPACE

/*!
    \class QLibraryInfo
    \brief The QLibraryInfo class provides information about the Katie library.

    Many pieces of information are established when Katie is configured.
    Installation paths, license information, and even a unique build
    key. This class provides an abstraction for accessing this
    information.

    You can also use a \c Katie.conf file to override the hard-coded paths
    that are compiled into the Katie library. For more information, see
    the \l {Using Katie.conf} documentation.

    \sa QSysInfo, {Using Katie.conf}
*/

/*!
    \enum QLibraryInfo::LibraryLocation

    \keyword library location

    This enum type is used to specify a specific location specifier:

    \value HeadersPath The location for all headers.
    \value LibrariesPath The location of installed libraries.
    \value BinariesPath The location of installed Qt binaries (tools and applications).
    \value PluginsPath The location of installed Qt plugins.
    \value ImportsPath The location of installed QML extensions to import.
    \value TranslationsPath The location of translation information for Qt strings.

    \sa location()
*/

/*!
  Returns the location specified by \a loc.

*/

QString QLibraryInfo::location(LibraryLocation loc)
{
    switch (loc) {
        case HeadersPath:
            return QString::fromLocal8Bit(qt_configure_headers_path_str);
        case LibrariesPath:
            return QString::fromLocal8Bit(qt_configure_libraries_path_str);
        case BinariesPath:
            return QString::fromLocal8Bit(qt_configure_binaries_path_str);
        case PluginsPath:
            return QString::fromLocal8Bit(qt_configure_plugins_path_str);
        case TranslationsPath:
            return QString::fromLocal8Bit(qt_configure_translations_path_str);
        case ImportsPath:
            return QString::fromLocal8Bit(qt_configure_imports_path_str);
    }

    Q_UNREACHABLE();
}

QT_END_NAMESPACE
