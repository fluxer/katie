/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#ifndef ABSTRACTICONCACHE_H
#define ABSTRACTICONCACHE_H

#include <QtCore/QObject>


QT_BEGIN_NAMESPACE

class QIcon;
class QPixmap;
class QString;

class Q_DESIGNER_EXPORT QDesignerIconCacheInterface : public QObject
{
    Q_OBJECT
public:
    QDesignerIconCacheInterface(QObject *parent_)
        : QObject(parent_) {}

    virtual QIcon nameToIcon(const QString &filePath, const QString &qrcPath = QString()) = 0;
    virtual QPixmap nameToPixmap(const QString &filePath, const QString &qrcPath = QString()) = 0;

    virtual QString iconToFilePath(const QIcon &pm) const = 0;
    virtual QString iconToQrcPath(const QIcon &pm) const = 0;

    virtual QString pixmapToFilePath(const QPixmap &pm) const = 0;
    virtual QString pixmapToQrcPath(const QPixmap &pm) const = 0;

    virtual QList<QPixmap> pixmapList() const = 0;
    virtual QList<QIcon> iconList() const = 0;

    virtual QString resolveQrcPath(const QString &filePath, const QString &qrcPath, const QString &workingDirectory = QString()) const = 0;
};

QT_END_NAMESPACE


#endif // ABSTRACTICONCACHE_H
