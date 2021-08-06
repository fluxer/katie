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

#ifndef PROMOTIONTASKMENU_H
#define PROMOTIONTASKMENU_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QList>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;
class QDesignerFormEditorInterface;

class QAction;
class QMenu;
class QWidget;
class QSignalMapper;

namespace qdesigner_internal {

// A helper class for creating promotion context menus and handling promotion actions.

class Q_DESIGNER_EXPORT PromotionTaskMenu: public QObject
{
    Q_OBJECT
public:
    enum Mode {
        ModeSingleWidget,
        ModeManagedMultiSelection,
        ModeUnmanagedMultiSelection
    };

    explicit PromotionTaskMenu(QWidget *widget,Mode mode = ModeManagedMultiSelection, QObject *parent = nullptr);

    Mode mode() const;
    void setMode(Mode m);

    void setWidget(QWidget *widget);

    // Set menu labels
    void setPromoteLabel(const QString &promoteLabel);
    void setEditPromoteToLabel(const QString &promoteEditLabel);
    // Defaults to "Demote to %1".arg(class).
    void setDemoteLabel(const QString &demoteLabel);

    typedef QList<QAction*> ActionList;

    enum AddFlags { LeadingSeparator = 1, TrailingSeparator = 2, SuppressGlobalEdit = 4};

    // Adds a list of promotion actions according to the current promotion state of the widget.
    void addActions(QDesignerFormWindowInterface *fw, unsigned flags, ActionList &actionList);
    // Convenience that finds the form window.
    void addActions(unsigned flags, ActionList &actionList);

    void addActions(QDesignerFormWindowInterface *fw, unsigned flags, QMenu *menu);
    void addActions(unsigned flags, QMenu *menu);

    // Pop up the editor in a global context.
    static void editPromotedWidgets(QDesignerFormEditorInterface *core, QWidget* parent);

private slots:
    void slotPromoteToCustomWidget(const QString &customClassName);
    void slotDemoteFromCustomWidget();
    void slotEditPromotedWidgets();
    void slotEditPromoteTo();
    void slotEditSignalsSlots();

private:
    void promoteTo(QDesignerFormWindowInterface *fw, const QString &customClassName);

    enum PromotionState { NotApplicable, NoHomogenousSelection, CanPromote, CanDemote };
    PromotionState createPromotionActions(QDesignerFormWindowInterface *formWindow);
    QDesignerFormWindowInterface *formWindow() const;

    typedef QList<QPointer<QWidget> > PromotionSelectionList;
    PromotionSelectionList promotionSelectionList(QDesignerFormWindowInterface *formWindow) const;

    Mode m_mode;

    QPointer<QWidget> m_widget;

    QSignalMapper *m_promotionMapper;
    // Per-Widget actions
    QList<QAction *> m_promotionActions;

    QAction *m_globalEditAction;
    QAction *m_EditPromoteToAction;
    QAction *m_EditSignalsSlotsAction;

    QString m_promoteLabel;
    QString m_demoteLabel;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // PROMOTIONTASKMENU_H
