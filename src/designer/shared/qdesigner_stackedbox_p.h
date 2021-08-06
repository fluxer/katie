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

#ifndef QDESIGNER_STACKEDBOX_H
#define QDESIGNER_STACKEDBOX_H

#include "qdesigner_propertysheet_p.h"

QT_BEGIN_NAMESPACE

class QStackedWidget;
class QWidget;
class QAction;
class QMenu;
class QToolButton;

namespace qdesigner_internal {
    class PromotionTaskMenu;
}

// Event filter to be installed on a QStackedWidget in preview mode.
// Create two buttons to switch pages.

class Q_DESIGNER_EXPORT QStackedWidgetPreviewEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit QStackedWidgetPreviewEventFilter(QStackedWidget *parent);

    // Install helper on QStackedWidget
    static void install(QStackedWidget *stackedWidget);
    bool eventFilter(QObject *watched, QEvent *event);

    void setButtonToolTipEnabled(bool v) { m_buttonToolTipEnabled = v; }
    bool buttonToolTipEnabled() const    { return m_buttonToolTipEnabled; }

public slots:
    void updateButtons();
    void prevPage();
    void nextPage();

protected:
    QStackedWidget *stackedWidget() const { return m_stackedWidget; }
    virtual void gotoPage(int page);

private:
    void updateButtonToolTip(QObject *o);

    bool m_buttonToolTipEnabled;
    QStackedWidget *m_stackedWidget;
    QToolButton *m_prev;
    QToolButton *m_next;
};

// Event filter to be installed on a QStackedWidget in editing mode.
//  In addition to the browse buttons, handles context menu and everything

class Q_DESIGNER_EXPORT QStackedWidgetEventFilter : public QStackedWidgetPreviewEventFilter
{
    Q_OBJECT
public:
    explicit QStackedWidgetEventFilter(QStackedWidget *parent);

    // Install helper on QStackedWidget
    static void install(QStackedWidget *stackedWidget);
    static QStackedWidgetEventFilter *eventFilterOf(const QStackedWidget *stackedWidget);
    // Convenience to add a menu on a tackedWidget
    static QMenu *addStackedWidgetContextMenuActions(const QStackedWidget *stackedWidget, QMenu *popup);

    // Add context menu and return page submenu or 0.
    QMenu *addContextMenuActions(QMenu *popup);

private slots:
    void removeCurrentPage();
    void addPage();
    void addPageAfter();
    void changeOrder();

protected:
    virtual void gotoPage(int page);

private:
    QAction *m_actionPreviousPage;
    QAction *m_actionNextPage;
    QAction *m_actionDeletePage;
    QAction *m_actionInsertPage;
    QAction *m_actionInsertPageAfter;
    QAction *m_actionChangePageOrder;
    qdesigner_internal::PromotionTaskMenu* m_pagePromotionTaskMenu;
};

// PropertySheet to handle the "currentPageName" property
class Q_DESIGNER_EXPORT QStackedWidgetPropertySheet : public QDesignerPropertySheet {
public:
    explicit QStackedWidgetPropertySheet(QStackedWidget *object, QObject *parent = nullptr);

    virtual void setProperty(int index, const QVariant &value);
    virtual QVariant property(int index) const;
    virtual bool reset(int index);
    virtual bool isEnabled(int index) const;

    // Check whether the property is to be saved. Returns false for the page
    // properties (as the property sheet has no concept of 'stored')
    static bool checkProperty(const QString &propertyName);

private:
    QStackedWidget *m_stackedWidget;
};

typedef QDesignerPropertySheetFactory<QStackedWidget, QStackedWidgetPropertySheet> QStackedWidgetPropertySheetFactory;

QT_END_NAMESPACE

#endif // QDESIGNER_STACKEDBOX_H
