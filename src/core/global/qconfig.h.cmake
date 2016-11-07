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

#ifndef QCONFIG_H
#define QCONFIG_H

/* No longer supported */
#define QT_NO_OPENVG
#define QT_NO_PHONON
#define QT_NO_WIN_ACTIVEQT
#define QT_NO_DIRECTPAINTER
#define QT_NO_COP
#define QT_NO_BLITTABLE
#define QT_NO_PICTUREIO
#define QT_NO_SOFTKEYMANAGER
#define QT_NO_SXE
#define QT_NO_QWS_QPF2
#define QT_NO_QWS_INPUTMETHODS
#define QT_NO_QWSEMBEDWIDGET
#define QT_NO_QWS_CURSOR
#define QT_NO_QWS_DECORATION_DEFAULT
#define QT_NO_QWS_DECORATION_STYLED
#define QT_NO_QWS_DECORATION_WINDOWS
#define QT_NO_QWS_MANAGER
#define QT_NO_QWS_KEYBOARD
#define QT_NO_QWS_MOUSE
#define QT_NO_QWS_MOUSE_AUTO
#define QT_NO_QWS_MOUSE_MANUAL
#define QT_NO_QWS_MULTIPROCESS
#define QT_NO_QWS_SOUNDSERVER
#define QT_NO_QWS_PROPERTIES
#define QT_NO_QWS_PROXYSCREEN
#define QT_NO_QWS_DYNAMICSCREENTRANSFORMATION
#define QT_NO_QWS_TRANSFORMED
#define QT_NO_QWS_VNC
#define QT_NO_AUDIO_BACKEND
#define QT_NO_DIRECTWRITE
#define QT_NO_STYLE_WINDOWSCE
#define QT_NO_STYLE_WINDOWSMOBILE
#define QT_NO_STYLE_WINDOWSXP
#define QT_NO_STYLE_WINDOWSVISTA
#define QT_NO_STYLE_MAC
#define QT_NO_STYLE_S60
#define QT_NO_STATEMACHINE
#define QT_NO_XIM
#define QT_NO_IM

/* Qt build specs */
#ifndef QT_EDITION
#  define QT_EDITION QT_EDITION_DESKTOP
#endif
#ifndef QT_KATIE
#  define QT_KATIE
#endif

#ifndef QT_BUILD_KEY
#  define QT_BUILD_KEY "${KATIE_KEY}"
#endif

#cmakedefine QT_VISIBILITY_AVAILABLE
#cmakedefine QT_LARGEFILE_SUPPORT ${QT_LARGEFILE_SUPPORT}
#cmakedefine QT_POINTER_SIZE ${QT_POINTER_SIZE}

/* Data structures */
#cmakedefine QT_NO_QUUID_STRING
#cmakedefine QT_NO_STL_WCHAR
#cmakedefine QT_NO_TEXTDATE
#cmakedefine QT_NO_DATESTRING

/* Dialogs */
#cmakedefine QT_NO_COLORDIALOG
#cmakedefine QT_NO_ERRORMESSAGE
#cmakedefine QT_NO_FILEDIALOG
#cmakedefine QT_NO_FONTDIALOG
#cmakedefine QT_NO_INPUTDIALOG
#cmakedefine QT_NO_MESSAGEBOX
#cmakedefine QT_NO_PRINTDIALOG
#cmakedefine QT_NO_PRINTPREVIEWDIALOG
#cmakedefine QT_NO_PROGRESSDIALOG
#cmakedefine QT_NO_WIZARD

/* File I/O */
#cmakedefine QT_NO_DOM
#cmakedefine QT_NO_FILESYSTEMWATCHER
#cmakedefine QT_NO_FILESYSTEMMODEL
#cmakedefine QT_NO_PROCESS
#cmakedefine QT_NO_TEMPORARYFILE
#cmakedefine QT_NO_TEXTSTREAM
#cmakedefine QT_NO_SETTINGS
#cmakedefine QT_NO_STANDARDPATHS
#cmakedefine QT_NO_LIBRARY

/* Fonts */
#cmakedefine QT_NO_FREETYPE

/* Images */
#cmakedefine QT_NO_IMAGEFORMATPLUGIN
#cmakedefine QT_NO_IMAGEFORMAT_BMP
#cmakedefine QT_NO_IMAGEFORMAT_JPEG
#cmakedefine QT_NO_IMAGEFORMAT_PNG
#cmakedefine QT_NO_IMAGEFORMAT_PPM
#cmakedefine QT_NO_IMAGEFORMAT_XBM
#cmakedefine QT_NO_IMAGEFORMAT_XPM
#cmakedefine QT_NO_IMAGE_HEURISTIC_MASK
#cmakedefine QT_NO_IMAGE_TEXT
#cmakedefine QT_NO_MOVIE

/* Internationalization */
#cmakedefine QT_NO_BIG_CODECS
#cmakedefine QT_NO_TEXTCODEC
#cmakedefine QT_NO_CODECS
#cmakedefine QT_NO_TEXTCODECPLUGIN
#cmakedefine QT_NO_TRANSLATION
#cmakedefine QT_NO_TRANSLATION_UTF8

/* ItemViews */
#cmakedefine QT_NO_ITEMVIEWS
#cmakedefine QT_NO_DATAWIDGETMAPPER
#cmakedefine QT_NO_DIRMODEL
#cmakedefine QT_NO_LISTVIEW
#cmakedefine QT_NO_COLUMNVIEW
#cmakedefine QT_NO_PROXYMODEL
#cmakedefine QT_NO_SORTFILTERPROXYMODEL
#cmakedefine QT_NO_STANDARDITEMMODEL
#cmakedefine QT_NO_STRINGLISTMODEL
#cmakedefine QT_NO_TABLEVIEW
#cmakedefine QT_NO_TREEVIEW

/* Kernel */
#cmakedefine QT_NO_ACTION
#cmakedefine QT_NO_CLIPBOARD
#cmakedefine QT_NO_CSSPARSER
#cmakedefine QT_NO_CURSOR
#cmakedefine QT_NO_DRAGANDDROP
#cmakedefine QT_NO_EFFECTS
#cmakedefine QT_NO_PROPERTIES
#cmakedefine QT_NO_SESSIONMANAGER
#cmakedefine QT_NO_SHAREDMEMORY
#cmakedefine QT_NO_SHORTCUT
#cmakedefine QT_NO_SOUND
#cmakedefine QT_NO_SYSTEMSEMAPHORE
#cmakedefine QT_NO_TABLETEVENT
#cmakedefine QT_NO_TEXTHTMLPARSER
#cmakedefine QT_NO_CONCURRENT
#cmakedefine QT_NO_WHEELEVENT
#cmakedefine QT_NO_XMLSTREAM
#cmakedefine QT_NO_XMLSTREAMREADER
#cmakedefine QT_NO_XMLSTREAMWRITER

/* Networking */
#cmakedefine QT_NO_COP
#cmakedefine QT_NO_HOSTINFO
#cmakedefine QT_NO_HTTP
#cmakedefine QT_NO_NETWORKPROXY
#cmakedefine QT_NO_SOCKS5
#cmakedefine QT_NO_UDPSOCKET
#cmakedefine QT_NO_URLINFO
#cmakedefine QT_NO_FTP
#cmakedefine QT_NO_RESOLV

/* Painting */
#cmakedefine QT_NO_COLORNAMES
#cmakedefine QT_NO_PAINTONSCREEN
#cmakedefine QT_NO_PAINT_DEBUG
#cmakedefine QT_NO_PICTURE
#cmakedefine QT_NO_PRINTER
#cmakedefine QT_NO_CUPS

/* SVG */
#cmakedefine QT_NO_SVG
#cmakedefine QT_NO_GRAPHICSSVGITEM
#cmakedefine QT_NO_SVGGENERATOR
#cmakedefine QT_NO_SVGRENDERER
#cmakedefine QT_NO_SVGWIDGET

/* Styles */
#cmakedefine QT_NO_STYLE_MOTIF
#cmakedefine QT_NO_STYLE_CDE
#cmakedefine QT_NO_STYLE_CLEANLOOKS
#cmakedefine QT_NO_STYLE_PLASTIQUE
#cmakedefine QT_NO_STYLE_STYLESHEET
#cmakedefine QT_NO_GCONF2

/* Utilities */
#cmakedefine QT_NO_ACCESSIBILITY
#cmakedefine QT_NO_COMPLETER
#cmakedefine QT_NO_DESKTOPSERVICES
#cmakedefine QT_NO_SCRIPT
#cmakedefine QT_NO_SYSTEMTRAYICON
#cmakedefine QT_NO_UNDOCOMMAND
#cmakedefine QT_NO_UNDOGROUP
#cmakedefine QT_NO_UNDOSTACK
#cmakedefine QT_NO_UNDOVIEW
#cmakedefine QT_NO_GESTURES

/* Widgets */
#cmakedefine QT_NO_GROUPBOX
#cmakedefine QT_NO_BUTTONGROUP
#cmakedefine QT_NO_LCDNUMBER
#cmakedefine QT_NO_LINEEDIT
#cmakedefine QT_NO_COMBOBOX
#cmakedefine QT_NO_FONTCOMBOBOX
#cmakedefine QT_NO_SPINBOX
#cmakedefine QT_NO_CALENDARWIDGET
#cmakedefine QT_NO_DATETIMEEDIT
#cmakedefine QT_NO_LISTWIDGET
#cmakedefine QT_NO_MENU
#cmakedefine QT_NO_CONTEXTMENU
#cmakedefine QT_NO_MAINWINDOW
#cmakedefine QT_NO_DOCKWIDGET
#cmakedefine QT_NO_TOOLBAR
#cmakedefine QT_NO_MENUBAR
#cmakedefine QT_NO_WORKSPACE
#cmakedefine QT_NO_PROGRESSBAR
#cmakedefine QT_NO_RESIZEHANDLER
#cmakedefine QT_NO_RUBBERBAND
#cmakedefine QT_NO_SPLITTER
#cmakedefine QT_NO_SIGNALMAPPER
#cmakedefine QT_NO_SIZEGRIP
#cmakedefine QT_NO_SLIDER
#cmakedefine QT_NO_DIAL
#cmakedefine QT_NO_SCROLLBAR
#cmakedefine QT_NO_SCROLLAREA
#cmakedefine QT_NO_GRAPHICSVIEW
#cmakedefine QT_NO_PRINTPREVIEWWIDGET
#cmakedefine QT_NO_MDIAREA
#cmakedefine QT_NO_TEXTEDIT
#cmakedefine QT_NO_SYNTAXHIGHLIGHTER
#cmakedefine QT_NO_TEXTBROWSER
#cmakedefine QT_NO_SPINWIDGET
#cmakedefine QT_NO_SPLASHSCREEN
#cmakedefine QT_NO_STACKEDWIDGET
#cmakedefine QT_NO_TABWIDGET
#cmakedefine QT_NO_STATUSBAR
#cmakedefine QT_NO_STATUSTIP
#cmakedefine QT_NO_TABLEWIDGET
#cmakedefine QT_NO_TOOLBUTTON
#cmakedefine QT_NO_TABBAR
#cmakedefine QT_NO_TOOLBOX
#cmakedefine QT_NO_WHATSTHIS
#cmakedefine QT_NO_TOOLTIP
#cmakedefine QT_NO_TREEWIDGET
#cmakedefine QT_NO_VALIDATOR

#endif
