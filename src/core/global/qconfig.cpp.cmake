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

/* Build information */
static const char* qt_configure_build_key_str          = "${KATIE_GIT}\0";
static const char* qt_configure_build_date_str         = "${KATIE_DATE}\0";
static const char* qt_configure_licensee_str           = "${KATIE_LICENSE}\0";
static const char* qt_configure_licensed_products_str  = "${KATIE_PRODUCT}\0";

/* Runtime paths */
static const char* qt_configure_prefix_path_str        = "${KATIE_PREFIX_FULL}\0";
static const char* qt_configure_headers_path_str       = "${KATIE_HEADERS_FULL}\0";
static const char* qt_configure_libraries_path_str     = "${KATIE_LIBRARIES_FULL}\0";
static const char* qt_configure_binaries_path_str      = "${KATIE_BINARIES_FULL}\0";
static const char* qt_configure_plugins_path_str       = "${KATIE_PLUGINS_FULL}\0";
static const char* qt_configure_imports_path_str       = "${KATIE_IMPORTS_FULL}\0";
static const char* qt_configure_data_path_str          = "${KATIE_DATA_FULL}\0";
static const char* qt_configure_translations_path_str  = "${KATIE_TRANSLATIONS_FULL}\0";
static const char* qt_configure_settings_path_str      = "${KATIE_SETTINGS_FULL}\0";
