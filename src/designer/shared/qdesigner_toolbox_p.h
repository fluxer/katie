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

#ifndef QDESIGNER_TOOLBOX_H
#define QDESIGNER_TOOLBOX_H

#include "qdesigner_propertysheet_p.h"
#include "qdesigner_utils_p.h"
#include <QtGui/QPalette>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {
    class PromotionTaskMenu;
}

class QToolBox;

class QAction;
class QMenu;

class Q_DESIGNER_EXPORT QToolBoxHelper : public QObject
{
    Q_OBJECT

    explicit QToolBoxHelper(QToolBox *toolbox);
public:
    // Install helper on QToolBox
    static void install(QToolBox *toolbox);
    static QToolBoxHelper *helperOf(const QToolBox *toolbox);
    // Convenience to add a menu on a toolbox
    static QMenu *addToolBoxContextMenuActions(const QToolBox *toolbox, QMenu *popup);

    QPalette::ColorRole currentItemBackgroundRole() const;
    void setCurrentItemBackgroundRole(QPalette::ColorRole role);

    bool eventFilter(QObject *watched, QEvent *event);
    // Add context menu and return page submenu or 0.

    QMenu *addContextMenuActions(QMenu *popup) const;

private slots:
    void removeCurrentPage();
    void addPage();
    void addPageAfter();
    void changeOrder();

private:
    QToolBox *m_toolbox;
    QAction *m_actionDeletePage;
    QAction *m_actionInsertPage;
    QAction *m_actionInsertPageAfter;
    QAction *m_actionChangePageOrder;
    qdesigner_internal::PromotionTaskMenu* m_pagePromotionTaskMenu;
};

// PropertySheet to handle the page properties
class Q_DESIGNER_EXPORT QToolBoxWidgetPropertySheet : public QDesignerPropertySheet {
public:
    explicit QToolBoxWidgetPropertySheet(QToolBox *object, QObject *parent = nullptr);

    virtual void setProperty(int index, const QVariant &value);
    virtual QVariant property(int index) const;
    virtual bool reset(int index);
    virtual bool isEnabled(int index) const;

    // Check whether the property is to be saved. Returns false for the page
    // properties (as the property sheet has no concept of 'stored')
    static bool checkProperty(const QString &propertyName);

private:
    enum ToolBoxProperty { PropertyCurrentItemText, PropertyCurrentItemName, PropertyCurrentItemIcon,
                           PropertyCurrentItemToolTip,  PropertyTabSpacing, PropertyToolBoxNone };

    static ToolBoxProperty toolBoxPropertyFromName(const QString &name);
    QToolBox *m_toolBox;
    struct PageData
        {
        qdesigner_internal::PropertySheetStringValue text;
        qdesigner_internal::PropertySheetStringValue tooltip;
        qdesigner_internal::PropertySheetIconValue icon;
        };
    QMap<QWidget *, PageData> m_pageToData;
};

typedef QDesignerPropertySheetFactory<QToolBox, QToolBoxWidgetPropertySheet> QToolBoxWidgetPropertySheetFactory;

QT_END_NAMESPACE

#endif // QDESIGNER_TOOLBOX_H
