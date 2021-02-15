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

#ifndef ABSTRACTBRUSHMANAGER_H
#define ABSTRACTBRUSHMANAGER_H

#include <QtCore/qobject.h>
#include <QtCore/qmap.h>
#include <QtGui/qbrush.h>


QT_BEGIN_NAMESPACE

class QObject;

class Q_DESIGNER_EXPORT QDesignerBrushManagerInterface : public QObject
{
    Q_OBJECT
public:
    QDesignerBrushManagerInterface(QObject *parentObject = 0) : QObject(parentObject) {}

    virtual QBrush brush(const QString &name) const = 0;
    virtual QMap<QString, QBrush> brushes() const = 0;
    virtual QString currentBrush() const = 0;

    virtual QString addBrush(const QString &name, const QBrush &brush) = 0;
    virtual void removeBrush(const QString &name) = 0;
    virtual void setCurrentBrush(const QString &name) = 0;

    virtual QPixmap brushPixmap(const QBrush &brush) const = 0;
Q_SIGNALS:
    void brushAdded(const QString &name, const QBrush &brush);
    void brushRemoved(const QString &name);
    void currentBrushChanged(const QString &name, const QBrush &brush);

};

QT_END_NAMESPACE


#endif
