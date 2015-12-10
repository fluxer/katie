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

/* Installation date */
static const char qt_configure_installation          [12+11]    = "${KATIE_DATE}";

/* License Info */
static const char qt_configure_licensee_str          [256 + 12] = "${QT_LICENSE_STR}";
static const char qt_configure_licensed_products_str [256 + 12] = "${QT_PRODUCT_STR}";

static const char qt_configure_prefix_path_str       [256 + 12] = "${QT_PREFIX_PATH_STR}";
static const char qt_configure_headers_path_str      [256 + 12] = "${QT_HEADERS_PATH_STR}";
static const char qt_configure_libraries_path_str    [256 + 12] = "${QT_LIBRARIES_PATH_STR}";
static const char qt_configure_binaries_path_str     [256 + 12] = "${QT_BINARIES_PATH_STR}";
static const char qt_configure_plugins_path_str      [256 + 12] = "${QT_PLUGINS_PATH_STR}";
static const char qt_configure_imports_path_str      [256 + 12] = "${QT_IMPORTS_PATH_STR}";
static const char qt_configure_data_path_str         [256 + 12] = "${QT_DATA_PATH_STR}";
static const char qt_configure_translations_path_str [256 + 12] = "${QT_TRANSLATIONS_PATH_STR}";
static const char qt_configure_settings_path_str     [256 + 12] = "${QT_SETTINGS_PATH_STR}";
static const char qt_configure_documentation_path_str[256 + 12] = "${QT_DOCUMENTATION_PATH_STR}";
static const char qt_configure_examples_path_str     [256 + 12] = "${QT_EXAMPLES_PATH_STR}";
static const char qt_configure_demos_path_str        [256 + 12] = "${QT_DEMOS_PATH_STR}";

/* strlen( "qt_lcnsxxxx" ) == 12 */
#define QT_CONFIGURE_LICENSEE qt_configure_licensee_str + 12;
#define QT_CONFIGURE_LICENSED_PRODUCTS qt_configure_licensed_products_str + 12;
#define QT_CONFIGURE_PREFIX_PATH qt_configure_prefix_path_str + 12;
#define QT_CONFIGURE_HEADERS_PATH qt_configure_headers_path_str + 12;
#define QT_CONFIGURE_LIBRARIES_PATH qt_configure_libraries_path_str + 12;
#define QT_CONFIGURE_BINARIES_PATH qt_configure_binaries_path_str + 12;
#define QT_CONFIGURE_PLUGINS_PATH qt_configure_plugins_path_str + 12;
#define QT_CONFIGURE_IMPORTS_PATH qt_configure_imports_path_str + 12;
#define QT_CONFIGURE_DATA_PATH qt_configure_data_path_str + 12;
#define QT_CONFIGURE_TRANSLATIONS_PATH qt_configure_translations_path_str + 12;
#define QT_CONFIGURE_SETTINGS_PATH qt_configure_settings_path_str + 12;
#define QT_CONFIGURE_DOCUMENTATION_PATH qt_configure_documentation_path_str + 12;
#define QT_CONFIGURE_EXAMPLES_PATH qt_configure_examples_path_str + 12;
#define QT_CONFIGURE_DEMOS_PATH qt_configure_demos_path_str + 12;
