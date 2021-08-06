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

#ifndef QMAINWINDOW_CONTAINER_H
#define QMAINWINDOW_CONTAINER_H

#include <QtDesigner/container.h>
#include <QtDesigner/default_extensionfactory.h>

#include <extensionfactory_p.h>

#include <QtGui/QMainWindow>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class QMainWindowContainer: public QObject, public QDesignerContainerExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerContainerExtension)
public:
    explicit QMainWindowContainer(QMainWindow *widget, QObject *parent = nullptr);

    virtual int count() const;
    virtual QWidget *widget(int index) const;
    virtual int currentIndex() const;
    virtual void setCurrentIndex(int index);
    virtual void addWidget(QWidget *widget);
    virtual void insertWidget(int index, QWidget *widget);
    virtual void remove(int index);

private:
    QMainWindow *m_mainWindow;
    QList<QWidget*> m_widgets;
};

typedef ExtensionFactory<QDesignerContainerExtension, QMainWindow, QMainWindowContainer> QMainWindowContainerFactory;
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // QMAINWINDOW_CONTAINER_H
