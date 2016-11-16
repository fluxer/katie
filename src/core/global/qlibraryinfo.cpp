/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#include "qdir.h"
#include "qfile.h"
#include "qconfig.h"
#include "qsettings.h"
#include "qlibraryinfo.h"
#include "qscopedpointer.h"

#ifdef QT_BOOTSTRAPPED
QT_BEGIN_NAMESPACE
extern QString qmake_libraryInfoFile();
QT_END_NAMESPACE
#else
# include "qcoreapplication.h"
#endif

#include "qconfig.cpp"

QT_BEGIN_NAMESPACE

extern void qDumpCPUFeatures(); // in qsimd.cpp

#ifndef QT_NO_SETTINGS

/*!
    \class QLibraryInfo
    \brief The QLibraryInfo class provides information about the Qt library.

    Many pieces of information are established when Qt is configured.
    Installation paths, license information, and even a unique build
    key. This class provides an abstraction for accessing this
    information.

    \table
    \header \o Function           \o Return value
    \row    \o buildKey()         \o A string that identifies the Qt version and
                                     the configuration. This key is used to ensure
                                     that \l{plugins} link against the same version
                                     of Qt as the application.
    \row    \o location()         \o The path to a certain Qt
                                     component (e.g., documentation, header files).
    \row    \o licensee(),
               licensedProducts() \o Licensing information.
    \endtable

    You can also use a \c qt.conf file to override the hard-coded paths
    that are compiled into the Qt library. For more information, see
    the \l {Using qt.conf} documentation.

    \sa QSysInfo, {Using qt.conf}
*/

/*! \internal

   You cannot create a QLibraryInfo, instead only the static functions are available to query
   information.
*/

QLibraryInfo::QLibraryInfo()
{ }

/*!
  Returns the person to whom this build of Qt is licensed.

  \sa licensedProducts()
*/

QString
QLibraryInfo::licensee()
{
    return QString::fromLocal8Bit(qt_configure_licensee_str);
}

/*!
  Returns the products that the license for this build of Qt has access to.

  \sa licensee()
*/

QString
QLibraryInfo::licensedProducts()
{
    return QString::fromLatin1(qt_configure_licensed_products_str);
}

/*!
    Returns a unique key identifying this build of Qt and its
    configurations. This key is not globally unique, rather only useful
    for establishing of two configurations are compatible. This can be
    used to compare with the \c QT_BUILD_KEY preprocessor symbol.

    \sa location()
*/

QString
QLibraryInfo::buildKey()
{
    return QString::fromLatin1(QT_BUILD_KEY);
}

/*!
    \since 4.6
    Returns the installation date for this build of Qt. The install date will
    usually be the last time that Qt sources were configured.
*/
#ifndef QT_NO_DATESTRING
QDate
QLibraryInfo::buildDate()
{
    return QDate::fromString(QString::fromLatin1(qt_configure_installation), Qt::ISODate);
}
#endif //QT_NO_DATESTRING

/*!
  Returns the location specified by \a loc.

*/

QString
QLibraryInfo::location(LibraryLocation loc)
{
    QString ret;
    switch (loc) {
        case PrefixPath:
            ret = QString::fromLocal8Bit(qt_configure_prefix_path_str);
            break;
        case HeadersPath:
            ret = QString::fromLocal8Bit(qt_configure_headers_path_str);
            break;
        case LibrariesPath:
            ret = QString::fromLocal8Bit(qt_configure_libraries_path_str);
            break;
        case BinariesPath:
            ret = QString::fromLocal8Bit(qt_configure_binaries_path_str);
            break;
        case PluginsPath:
            ret = QString::fromLocal8Bit(qt_configure_plugins_path_str);
            break;
        case ImportsPath:
            ret = QString::fromLocal8Bit(qt_configure_imports_path_str);
            break;
        case DataPath:
            ret = QString::fromLocal8Bit(qt_configure_data_path_str);
            break;
        case TranslationsPath:
            ret = QString::fromLocal8Bit(qt_configure_translations_path_str);
            break;
        case SettingsPath:
            ret = QString::fromLocal8Bit(qt_configure_settings_path_str);
            break;
        default:
            break;
    }

    if (QDir::isRelativePath(ret)) {
        QString baseDir;
        if (loc == PrefixPath) {
            // we make the prefix path absolute to the executable's directory
#ifdef QT_BOOTSTRAPPED
            baseDir = QFileInfo(qmake_libraryInfoFile()).absolutePath();
#else
            if (QCoreApplication::instance()) {
                baseDir = QCoreApplication::applicationDirPath();
            } else {
                baseDir = QDir::currentPath();
            }
#endif
        } else {
            // we make any other path absolute to the prefix directory
            baseDir = location(PrefixPath);
        }
        ret = QDir::cleanPath(baseDir + QLatin1Char('/') + ret);
    }
    return ret;
}

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
    \value SettingsPath The location for Qt settings.

    \sa location()
*/

#endif // QT_NO_SETTINGS

QT_END_NAMESPACE

#if defined(Q_CC_GNU) && defined(ELF_INTERPRETER)
#  include <stdio.h>
#  include <stdlib.h>

extern const char qt_core_interpreter[] __attribute__((section(".interp")))
    = ELF_INTERPRETER;

extern "C" void qt_core_boilerplate();
void qt_core_boilerplate()
{
    printf("This is the QtCore library version " QT_VERSION_STR "\n"
           "Copyright (C) 2015 The Qt Company Ltd.\n"
           "Contact: http://www.qt.io/licensing/\n"
           "\n"
           "Build key:           " QT_BUILD_KEY "\n"
           "|\n"
           "Build date:          %s\n"
           "Installation prefix: %s\n"
           "Library path:        %s\n"
           "Include path:        %s\n",
           qt_configure_installation,
           qt_configure_prefix_path_str,
           qt_configure_libraries_path_str,
           qt_configure_headers_path_str);

    QT_PREPEND_NAMESPACE(qDumpCPUFeatures)();

    exit(0);
}

#endif
