/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#ifndef TREEWIDGETEDITOR_H
#define TREEWIDGETEDITOR_H

#include "ui_treewidgeteditor.h"

#include "listwidgeteditor.h"

#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class QTreeWidget;
class QDesignerFormWindowInterface;

namespace qdesigner_internal {

class FormWindowBase;
class PropertySheetIconValue;

class TreeWidgetEditor: public AbstractItemEditor
{
    Q_OBJECT
public:
    explicit TreeWidgetEditor(QDesignerFormWindowInterface *form, QDialog *dialog);

    TreeWidgetContents fillContentsFromTreeWidget(QTreeWidget *treeWidget);
    TreeWidgetContents contents() const;

private slots:
    void on_newItemButton_clicked();
    void on_newSubItemButton_clicked();
    void on_deleteItemButton_clicked();
    void on_moveItemUpButton_clicked();
    void on_moveItemDownButton_clicked();
    void on_moveItemRightButton_clicked();
    void on_moveItemLeftButton_clicked();

    void on_treeWidget_currentItemChanged();
    void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void on_columnEditor_indexChanged(int idx);
    void on_columnEditor_itemChanged(int idx, int role, const QVariant &v);

    void on_columnEditor_itemInserted(int idx);
    void on_columnEditor_itemDeleted(int idx);
    void on_columnEditor_itemMovedUp(int idx);
    void on_columnEditor_itemMovedDown(int idx);

    void togglePropertyBrowser();
    void cacheReloaded();

protected:
    virtual void setItemData(int role, const QVariant &v);
    virtual QVariant getItemData(int role) const;

private:
    void setPropertyBrowserVisible(bool v);
    QtVariantProperty *setupPropertyGroup(const QString &title, const PropertyDefinition *propDefs);
    void updateEditor();
    void moveColumnItems(const PropertyDefinition *propList, QTreeWidgetItem *item, int fromColumn, int toColumn, int step);
    void moveColumns(int fromColumn, int toColumn, int step);
    void moveColumnsLeft(int fromColumn, int toColumn);
    void moveColumnsRight(int fromColumn, int toColumn);
    void closeEditors();

    Ui_TreeWidgetEditor ui;
    ItemListEditor *m_columnEditor;
    bool m_updatingBrowser;
};

class TreeWidgetEditorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TreeWidgetEditorDialog(QDesignerFormWindowInterface *form, QWidget *parent);

    TreeWidgetContents fillContentsFromTreeWidget(QTreeWidget *treeWidget);
    TreeWidgetContents contents() const;

private:
    TreeWidgetEditor m_editor;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // TREEWIDGETEDITOR_H
