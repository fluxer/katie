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

#ifndef ABSTRACTOBJECTINSPECTOR_H
#define ABSTRACTOBJECTINSPECTOR_H

#include <QtGui/QWidget>


QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;

class Q_DESIGNER_EXPORT QDesignerObjectInspectorInterface: public QWidget
{
    Q_OBJECT
public:
    QDesignerObjectInspectorInterface(QWidget *parent, Qt::WindowFlags flags = 0);
    virtual ~QDesignerObjectInspectorInterface();

    virtual QDesignerFormEditorInterface *core() const;

public Q_SLOTS:
    virtual void setFormWindow(QDesignerFormWindowInterface *formWindow) = 0;
};

QT_END_NAMESPACE


#endif // ABSTRACTOBJECTINSPECTOR_H
