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

#ifndef QTBRUSHMANAGER_H
#define QTBRUSHMANAGER_H

#include <QtDesigner/abstractbrushmanager.h>
#include "formeditor_global.h"

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtGui/QBrush>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class QtBrushManagerPrivate;

class QT_FORMEDITOR_EXPORT QtBrushManager : public QDesignerBrushManagerInterface
{
    Q_OBJECT
public:
    QtBrushManager(QObject *parent = nullptr);
    ~QtBrushManager();

    QBrush brush(const QString &name) const;
    QMap<QString, QBrush> brushes() const;
    QString currentBrush() const;

    QString addBrush(const QString &name, const QBrush &brush);
    void removeBrush(const QString &name);
    void setCurrentBrush(const QString &name);

    QPixmap brushPixmap(const QBrush &brush) const;

private:
    QScopedPointer<QtBrushManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtBrushManager)
    Q_DISABLE_COPY(QtBrushManager)
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif
