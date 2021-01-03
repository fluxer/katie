/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#include "qfileiconprovider.h"

#ifndef QT_NO_FILEICONPROVIDER

#include "qfileinfo.h"
#include "qapplication.h"
#include "qdir.h"
#include "qpixmapcache.h"
#include "qguiplatformplugin.h"
#include "qstyle.h"

QT_BEGIN_NAMESPACE

/*!
  \class QFileIconProvider

  \brief The QFileIconProvider class provides file icons for the QDirModel and the QFileSystemModel classes.
*/

/*!
  \enum QFileIconProvider::IconType
  \value Computer
  \value Desktop
  \value Trashcan
  \value Network
  \value Drive
  \value Folder
  \value File
*/

/*!
  Constructs a file icon provider.
*/

QFileIconProvider::QFileIconProvider()
{
}

/*!
  Destroys the file icon provider.

*/

QFileIconProvider::~QFileIconProvider()
{
}

/*!
  Returns an icon set for the given \a type.
*/

QIcon QFileIconProvider::icon(IconType type) const
{
    switch (type) {
        case Computer:
            return QApplication::style()->standardIcon(QStyle::SP_ComputerIcon);
        case Desktop:
            return QApplication::style()->standardIcon(QStyle::SP_DesktopIcon);
        case Trashcan:
            return QApplication::style()->standardIcon(QStyle::SP_TrashIcon);
        case Network:
            return QApplication::style()->standardIcon(QStyle::SP_DriveNetIcon);
        case Drive:
            return QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon);
        case Folder:
            return QApplication::style()->standardIcon(QStyle::SP_DirIcon);
        case File:
            return QApplication::style()->standardIcon(QStyle::SP_FileIcon);
    };
    return QIcon();
}

/*!
  Returns an icon for the file described by \a info.
*/

QIcon QFileIconProvider::icon(const QFileInfo &info) const
{
    QIcon platformIcon = qt_guiPlatformPlugin()->fileSystemIcon(info);
    if (!platformIcon.isNull())
        return platformIcon;

    if (info.isRoot()) {
        return QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon);
    } else if (info.isFile()) {
        if (info.isSymLink()) {
            return QApplication::style()->standardIcon(QStyle::SP_FileLinkIcon);
        } else {
            return QApplication::style()->standardIcon(QStyle::SP_FileIcon);
        }
    } else if (info.isDir()) {
        static QString homePath = QDir::home().absolutePath();

        if (info.isSymLink()) {
            return QApplication::style()->standardIcon(QStyle::SP_DirLinkIcon);
        } else if (info.absoluteFilePath() == homePath) {
            return QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon);
        } else {
            return QApplication::style()->standardIcon(QStyle::SP_DirIcon);
        }
    }
    return QIcon();
}

/*!
  Returns the type of the file described by \a info.
*/

QString QFileIconProvider::type(const QFileInfo &info) const
{
    if (info.isRoot())
        return QApplication::translate("QFileDialog", "Drive");
    if (info.isFile()) {
        if (!info.suffix().isEmpty())
            return info.suffix() + QLatin1Char(' ') + QApplication::translate("QFileDialog", "File");
        return QApplication::translate("QFileDialog", "File");
    }

    if (info.isDir())
        return QApplication::translate("QFileDialog", "Folder");
    // Konqueror - "Folder"
    // Nautilus  - "folder"

    if (info.isSymLink())
        return QApplication::translate("QFileDialog", "Shortcut");
    // Konqueror - "Folder" or "TXT File" i.e. what it is pointing to
    // Nautilus  - "link to folder" or "link to object file", same as Konqueror

    return QApplication::translate("QFileDialog", "Unknown");
}

QT_END_NAMESPACE

#endif
