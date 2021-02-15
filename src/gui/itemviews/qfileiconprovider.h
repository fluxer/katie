/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QFILEICONPROVIDER_H
#define QFILEICONPROVIDER_H

#include <QtCore/qfileinfo.h>
#include <QtGui/qicon.h>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_FILEICONPROVIDER

class Q_GUI_EXPORT QFileIconProvider
{
public:
    QFileIconProvider();
    virtual ~QFileIconProvider();
    enum IconType { Computer, Desktop, Trashcan, Network, Drive, Folder, File };
    virtual QIcon icon(IconType type) const;
    virtual QIcon icon(const QFileInfo &info) const;
    virtual QString type(const QFileInfo &info) const;
};

#endif // QT_NO_FILEICONPROVIDER

QT_END_NAMESPACE


#endif // QFILEICONPROVIDER_H

