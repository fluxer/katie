/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#ifndef ABSTRACTFORMWINDOWTOOL_H
#define ABSTRACTFORMWINDOWTOOL_H

#include <QtCore/QObject>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;
class QWidget;
class QAction;
class DomUI;

class Q_DESIGNER_EXPORT QDesignerFormWindowToolInterface: public QObject
{
    Q_OBJECT
public:
    QDesignerFormWindowToolInterface(QObject *parent = Q_NULLPTR);
    virtual ~QDesignerFormWindowToolInterface();

    virtual QDesignerFormEditorInterface *core() const = 0;
    virtual QDesignerFormWindowInterface *formWindow() const = 0;
    virtual QWidget *editor() const = 0;

    virtual QAction *action() const = 0;

    virtual void activated() = 0;
    virtual void deactivated() = 0;

    virtual void saveToDom(DomUI*, QWidget*) {}
    virtual void loadFromDom(DomUI*, QWidget*) {}

    virtual bool handleEvent(QWidget *widget, QWidget *managedWidget, QEvent *event) = 0;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // ABSTRACTFORMWINDOWTOOL_H
