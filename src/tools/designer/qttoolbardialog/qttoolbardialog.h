/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Katie Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QTTOOLBARDIALOG_H
#define QTTOOLBARDIALOG_H

#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class QMainWindow;
class QAction;
class QToolBar;

class QtToolBarManagerPrivate;

class QtToolBarManager : public QObject
{
    Q_OBJECT
public:

    explicit QtToolBarManager(QObject *parent = Q_NULLPTR);
    ~QtToolBarManager();

    void setMainWindow(QMainWindow *mainWindow);
    QMainWindow *mainWindow() const;

    void addAction(QAction *action, const QString &category);
    void removeAction(QAction *action);

    void addToolBar(QToolBar *toolBar, const QString &category);
    void removeToolBar(QToolBar *toolBar);

    QList<QToolBar *> toolBars() const;

    QByteArray saveState(int version = 0) const;
    bool restoreState(const QByteArray &state, int version = 0);

private:

    friend class QtToolBarDialog;
    QtToolBarManagerPrivate* d_ptr;
    Q_DECLARE_PRIVATE(QtToolBarManager)
    Q_DISABLE_COPY(QtToolBarManager)
};

class QtToolBarDialogPrivate;

class QtToolBarDialog : public QDialog
{
    Q_OBJECT
public:

    explicit QtToolBarDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
    ~QtToolBarDialog();

    void setToolBarManager(QtToolBarManager *toolBarManager);

protected:

    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private:

    QtToolBarDialogPrivate* d_ptr;
    Q_DECLARE_PRIVATE(QtToolBarDialog)
    Q_DISABLE_COPY(QtToolBarDialog)

    Q_PRIVATE_SLOT(d_func(), void newClicked())
    Q_PRIVATE_SLOT(d_func(), void removeClicked())
    Q_PRIVATE_SLOT(d_func(), void defaultClicked())
    Q_PRIVATE_SLOT(d_func(), void okClicked())
    Q_PRIVATE_SLOT(d_func(), void applyClicked())
    Q_PRIVATE_SLOT(d_func(), void cancelClicked())
    Q_PRIVATE_SLOT(d_func(), void upClicked())
    Q_PRIVATE_SLOT(d_func(), void downClicked())
    Q_PRIVATE_SLOT(d_func(), void leftClicked())
    Q_PRIVATE_SLOT(d_func(), void rightClicked())
    Q_PRIVATE_SLOT(d_func(), void renameClicked())
    Q_PRIVATE_SLOT(d_func(), void toolBarRenamed(QListWidgetItem *))
    Q_PRIVATE_SLOT(d_func(), void currentActionChanged(QTreeWidgetItem *))
    Q_PRIVATE_SLOT(d_func(), void currentToolBarChanged(QListWidgetItem *))
    Q_PRIVATE_SLOT(d_func(), void currentToolBarActionChanged(QListWidgetItem *))
};

QT_END_NAMESPACE

#endif
