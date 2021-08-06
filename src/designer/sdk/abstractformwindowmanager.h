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

#ifndef ABSTRACTFORMWINDOWMANAGER_H
#define ABSTRACTFORMWINDOWMANAGER_H

#include <QtDesigner/abstractformwindow.h>

#include <QtCore/QObject>


QT_BEGIN_NAMESPACE

class QAction;
class QActionGroup;
class QDesignerFormEditorInterface;
class DomUI;
class QWidget;
class QDesignerDnDItemInterface;

class Q_DESIGNER_EXPORT QDesignerFormWindowManagerInterface: public QObject
{
    Q_OBJECT
public:
    QDesignerFormWindowManagerInterface(QObject *parent = nullptr);
    virtual ~QDesignerFormWindowManagerInterface();

    virtual QAction *actionCut() const;
    virtual QAction *actionCopy() const;
    virtual QAction *actionPaste() const;
    virtual QAction *actionDelete() const;
    virtual QAction *actionSelectAll() const;
    virtual QAction *actionLower() const;
    virtual QAction *actionRaise() const;
    virtual QAction *actionUndo() const;
    virtual QAction *actionRedo() const;

    virtual QAction *actionHorizontalLayout() const;
    virtual QAction *actionVerticalLayout() const;
    virtual QAction *actionSplitHorizontal() const;
    virtual QAction *actionSplitVertical() const;
    virtual QAction *actionGridLayout() const;
    QAction *actionFormLayout() const;
    virtual QAction *actionBreakLayout() const;
    virtual QAction *actionAdjustSize() const;
    QAction *actionSimplifyLayout() const;

    virtual QDesignerFormWindowInterface *activeFormWindow() const;

    virtual int formWindowCount() const;
    virtual QDesignerFormWindowInterface *formWindow(int index) const;

    virtual QDesignerFormWindowInterface *createFormWindow(QWidget *parentWidget = 0, Qt::WindowFlags flags = 0);

    virtual QDesignerFormEditorInterface *core() const;

    virtual void dragItems(const QList<QDesignerDnDItemInterface*> &item_list) = 0;

Q_SIGNALS:
    void formWindowAdded(QDesignerFormWindowInterface *formWindow);
    void formWindowRemoved(QDesignerFormWindowInterface *formWindow);
    void activeFormWindowChanged(QDesignerFormWindowInterface *formWindow);

public Q_SLOTS:
    virtual void addFormWindow(QDesignerFormWindowInterface *formWindow);
    virtual void removeFormWindow(QDesignerFormWindowInterface *formWindow);
    virtual void setActiveFormWindow(QDesignerFormWindowInterface *formWindow);

protected:
    void setActionFormLayout(QAction *action);
    void setActionSimplifyLayout(QAction *action);

private:
    QDesignerFormWindowManagerInterface(const QDesignerFormWindowManagerInterface &other);
    QDesignerFormWindowManagerInterface &operator = (const QDesignerFormWindowManagerInterface &other);
};

QT_END_NAMESPACE


#endif // ABSTRACTFORMWINDOWMANAGER_H
