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

#ifndef CONTAINER_H
#define CONTAINER_H

#include <QtDesigner/extension.h>
#include <QtCore/QObject>


QT_BEGIN_NAMESPACE

class QWidget;

class QDesignerContainerExtension
{
public:
    virtual ~QDesignerContainerExtension() {}

    virtual int count() const = 0;
    virtual QWidget *widget(int index) const = 0;

    virtual int currentIndex() const = 0;
    virtual void setCurrentIndex(int index) = 0;

    virtual void addWidget(QWidget *widget) = 0;
    virtual void insertWidget(int index, QWidget *widget) = 0;
    virtual void remove(int index) = 0;
};

QT_END_NAMESPACE

Q_DECLARE_EXTENSION_INTERFACE(QDesignerContainerExtension, "Katie.Designer.Container")


#endif // CONTAINER_H
