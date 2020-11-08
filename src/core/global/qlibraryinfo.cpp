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

    \table
    \header \o Function           \o Return value
    \row    \o buildKey()         \o A string that identifies the Katie version and
                                     the configuration. This key is used to ensure
                                     that \l{plugins} link against the same version
                                     of Katie as the application.
    \row    \o location()         \o The path to a certain Katie
                                     component (e.g., documentation, header files).
    \row    \o licensee(),
               licensedProducts() \o Licensing information.
    \endtable

    You can also use a \c Katie.conf file to override the hard-coded paths
    that are compiled into the Katie library. For more information, see
    the \l {Using Katie.conf} documentation.

    \sa QSysInfo, {Using Katie.conf}
*/

/*!
    \enum QLibraryInfo::LibraryLocation

    \keyword library location

    This enum type is used to specify a specific location
    specifier:

    \value PrefixPath The default prefix for all paths.
    \value HeadersPath The location for all headers.
    \value LibrariesPath The location of installed libraries.
    \value BinariesPath The location of installed Qt binaries (tools and applications).
    \value PluginsPath The location of installed Qt plugins.
    \value ImportsPath The location of installed QML extensions to import.
    \value DataPath The location of general Qt data.
    \value TranslationsPath The location of translation information for Qt strings.

    \sa location()
*/

/*!
  Returns the person to whom this build of Qt is licensed.

  \sa licensedProducts()
*/

QString QLibraryInfo::licensee()
{
    return QString::fromLocal8Bit(qt_configure_licensee_str);
}

/*!
  Returns the products that the license for this build of Qt has access to.

  \sa licensee()
*/

QString QLibraryInfo::licensedProducts()
{
    return QString::fromLatin1(qt_configure_licensed_products_str);
}

/*!
    Returns a unique key identifying this build of Qt and its
    configurations. This key is not globally unique, rather only useful
    for establishing of two configurations are compatible.

    \sa location()
*/

QString QLibraryInfo::buildKey()
{
    return QString::fromLatin1(qt_configure_build_key_str);
}

/*!
    \since 4.6
    Returns the installation date for this build of Qt. The install date will
    usually be the last time that Qt sources were configured.
*/
#ifndef QT_NO_DATESTRING
QDate QLibraryInfo::buildDate()
{
    return QDate::fromString(QString::fromLatin1(qt_configure_build_date_str), Qt::ISODate);
}
#endif //QT_NO_DATESTRING

/*!
  Returns the location specified by \a loc.

*/

QString QLibraryInfo::location(LibraryLocation loc)
{
    switch (loc) {
        case PrefixPath:
            return QString::fromLocal8Bit(qt_configure_prefix_path_str);
        case HeadersPath:
            return QString::fromLocal8Bit(qt_configure_headers_path_str);
        case LibrariesPath:
            return QString::fromLocal8Bit(qt_configure_libraries_path_str);
        case BinariesPath:
            return QString::fromLocal8Bit(qt_configure_binaries_path_str);
        case PluginsPath:
            return QString::fromLocal8Bit(qt_configure_plugins_path_str);
        case DataPath:
            return QString::fromLocal8Bit(qt_configure_data_path_str);
        case TranslationsPath:
            return QString::fromLocal8Bit(qt_configure_translations_path_str);
        case ImportsPath:
            return QString::fromLocal8Bit(qt_configure_imports_path_str);
    }

    Q_UNREACHABLE();
}

QT_END_NAMESPACE
