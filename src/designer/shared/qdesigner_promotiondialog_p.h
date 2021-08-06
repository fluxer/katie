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

#ifndef PROMOTIONEDITORDIALOG_H
#define PROMOTIONEDITORDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QGroupBox>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;
class QDesignerPromotionInterface;
class QDesignerWidgetDataBaseItemInterface;

class QTreeView;
class QPushButton;
class QItemSelection;
class QDialogButtonBox;
class QComboBox;
class QLineEdit;
class QCheckBox;

namespace qdesigner_internal {
    struct PromotionParameters;
    class PromotionModel;


    // Panel for adding a new promoted class. Separate class for code cleanliness.
    class NewPromotedClassPanel : public QGroupBox {
        Q_OBJECT
    public:
        explicit NewPromotedClassPanel(const QStringList &baseClasses,
                                       int selectedBaseClass = -1,
                                       QWidget *parent = nullptr);

        QString promotedHeaderSuffix() const           { return m_promotedHeaderSuffix; }
        void setPromotedHeaderSuffix(const QString &s) { m_promotedHeaderSuffix = s; }

        bool isPromotedHeaderLowerCase() const    { return m_promotedHeaderLowerCase; }
        void setPromotedHeaderLowerCase(bool l) { m_promotedHeaderLowerCase = l; }

    signals:
        void newPromotedClass(const PromotionParameters &, bool *ok);

    public slots:
        void grabFocus();
        void chooseBaseClass(const QString &);
    private slots:
        void slotNameChanged(const QString &);
        void slotIncludeFileChanged(const QString &);
        void slotAdd();
        void slotReset();

    private:
        PromotionParameters promotionParameters() const;
        void enableButtons();

        QString m_promotedHeaderSuffix;
        bool m_promotedHeaderLowerCase;

        QComboBox *m_baseClassCombo;
        QLineEdit *m_classNameEdit;
        QLineEdit *m_includeFileEdit;
        QCheckBox *m_globalIncludeCheckBox;
        QPushButton *m_addButton;
    };

    // Dialog for editing promoted classes.
    class QDesignerPromotionDialog : public QDialog {
        Q_OBJECT

    public:
        enum Mode { ModeEdit, ModeEditChooseClass };

        explicit QDesignerPromotionDialog(QDesignerFormEditorInterface *core,
                                          QWidget *parent = nullptr,
                                          const QString &promotableWidgetClassName = QString(),
                                          QString *promoteTo = 0);
        // Return an alphabetically ordered list of base class names for adding new classes.
        static const QStringList &baseClassNames(const QDesignerPromotionInterface *promotion);

        signals:
        void selectedBaseClassChanged(const QString &);
    private slots:
        void slotRemove();
        void slotAcceptPromoteTo();
        void slotSelectionChanged(const QItemSelection &, const QItemSelection &);
        void slotNewPromotedClass(const PromotionParameters &, bool *ok);

        void slotIncludeFileChanged(QDesignerWidgetDataBaseItemInterface *, const QString &includeFile);
        void slotClassNameChanged(QDesignerWidgetDataBaseItemInterface *, const QString &newName);
        void slotUpdateFromWidgetDatabase();
        void slotTreeViewContextMenu(const QPoint &);
        void slotEditSignalsSlots();

    private:
        QDialogButtonBox *createButtonBox();
        void delayedUpdateFromWidgetDatabase();
        // Return item at model index and a combination of flags or 0.
        enum { Referenced = 1, CanPromote = 2 };
        QDesignerWidgetDataBaseItemInterface *databaseItemAt(const QItemSelection &, unsigned &flags) const;
        void displayError(const QString &message);

        const Mode m_mode;
        const QString m_promotableWidgetClassName;
        QDesignerFormEditorInterface *m_core;
        QString *m_promoteTo;
        QDesignerPromotionInterface *m_promotion;
        PromotionModel *m_model;
        QTreeView *m_treeView;
        QDialogButtonBox *m_buttonBox;
        QPushButton *m_removeButton;
        QString m_lastSelectedBaseClass;
    };
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // PROMOTIONEDITORDIALOG_H
