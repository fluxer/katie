/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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
#include "qfileiconprovider_p.h"

#ifndef QT_NO_FILEICONPROVIDER

#include "qfileinfo.h"
#include "qapplication.h"
#include "qdir.h"
#include "qpixmapcache.h"
#include "qguiplatformplugin.h"

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

QFileIconProviderPrivate::QFileIconProviderPrivate() :
    homePath(QDir::home().absolutePath())
{
}

QIcon QFileIconProviderPrivate::getIcon(QStyle::StandardPixmap name) const
{
    switch (name) {
        case QStyle::SP_FileIcon:
        case QStyle::SP_FileLinkIcon:
        case QStyle::SP_DirIcon:
        case QStyle::SP_DirLinkIcon:
        case QStyle::SP_DriveHDIcon:
        case QStyle::SP_DriveFDIcon:
        case QStyle::SP_DriveCDIcon:
        case QStyle::SP_DriveNetIcon:
        case QStyle::SP_ComputerIcon:
        case QStyle::SP_DesktopIcon:
        case QStyle::SP_TrashIcon:
        case QStyle::SP_DirHomeIcon:
            return QApplication::style()->standardIcon(name);
        default:
            return QIcon();
    }
    return QIcon();
}

/*!
  Constructs a file icon provider.
*/

QFileIconProvider::QFileIconProvider()
    : d_ptr(new QFileIconProviderPrivate)
{
}

/*!
  Destroys the file icon provider.

*/

QFileIconProvider::~QFileIconProvider()
{
    delete d_ptr;
}

/*!
  Returns an icon set for the given \a type.
*/

QIcon QFileIconProvider::icon(IconType type) const
{
    Q_D(const QFileIconProvider);
    switch (type) {
        case Computer:
            return d->getIcon(QStyle::SP_ComputerIcon);
        case Desktop:
            return d->getIcon(QStyle::SP_DesktopIcon);
        case Trashcan:
            return d->getIcon(QStyle::SP_TrashIcon);
        case Network:
            return d->getIcon(QStyle::SP_DriveNetIcon);
        case Drive:
            return d->getIcon(QStyle::SP_DriveHDIcon);
        case Folder:
            return d->getIcon(QStyle::SP_DirIcon);
        case File:
            return d->getIcon(QStyle::SP_FileIcon);
        default:
            break;
    };
    return QIcon();
}

/*!
  Returns an icon for the file described by \a info.
*/

QIcon QFileIconProvider::icon(const QFileInfo &info) const
{
    Q_D(const QFileIconProvider);

    QIcon platformIcon = qt_guiPlatformPlugin()->fileSystemIcon(info);
    if (!platformIcon.isNull())
        return platformIcon;

    if (info.isRoot()) {
        return d->getIcon(QStyle::SP_DriveHDIcon);
    } else if (info.isFile()) {
        if (info.isSymLink()) {
            return d->getIcon(QStyle::SP_FileLinkIcon);
        } else {
            return d->getIcon(QStyle::SP_FileIcon);
        }
    } else if (info.isDir()) {
        if (info.isSymLink()) {
            return d->getIcon(QStyle::SP_DirLinkIcon);
        } else if (info.absoluteFilePath() == d->homePath) {
            return d->getIcon(QStyle::SP_DirHomeIcon);
        } else {
            return d->getIcon(QStyle::SP_DirIcon);
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
        return QApplication::translate("QFileDialog", "Folder", "All other platforms");
    // Konqueror - "Folder"
    // Nautilus  - "folder"

    if (info.isSymLink())
        return QApplication::translate("QFileDialog", "Shortcut", "All other platforms");
    // Konqueror - "Folder" or "TXT File" i.e. what it is pointing to
    // Nautilus  - "link to folder" or "link to object file", same as Konqueror

    return QApplication::translate("QFileDialog", "Unknown");
}

QT_END_NAMESPACE

#endif






