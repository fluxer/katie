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

#ifndef STYLESHEETEDITOR_H
#define STYLESHEETEDITOR_H

#include <QtGui/QTextEdit>
#include <QtGui/QDialog>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;
class QDesignerFormEditorInterface;

class QDialogButtonBox;

namespace qdesigner_internal {

class Q_DESIGNER_EXPORT StyleSheetEditor : public QTextEdit
{
    Q_OBJECT
public:
    StyleSheetEditor(QWidget *parent = nullptr);
};

// Edit a style sheet.
class Q_DESIGNER_EXPORT StyleSheetEditorDialog : public QDialog
{
    Q_OBJECT
public:
    enum Mode {
        ModeGlobal, // resources are disabled (we don't have current resource set loaded), used e.g. in configuration dialog context
        ModePerForm // resources are available
    };

    StyleSheetEditorDialog(QDesignerFormEditorInterface *core, QWidget *parent, Mode mode = ModePerForm);
    ~StyleSheetEditorDialog();
    QString text() const;
    void setText(const QString &t);

    static bool isStyleSheetValid(const QString &styleSheet);


private slots:
    void validateStyleSheet();
    void slotContextMenuRequested(const QPoint &pos);
    void slotAddResource(const QString &property);
    void slotAddGradient(const QString &property);
    void slotAddColor(const QString &property);
    void slotAddFont();
    void slotRequestHelp();

protected:
    QDialogButtonBox *buttonBox() const;
    void setOkButtonEnabled(bool v);

private:
    void insertCssProperty(const QString &name, const QString &value);

    QDialogButtonBox *m_buttonBox;
    StyleSheetEditor *m_editor;
    QLabel *m_validityLabel;
    QDesignerFormEditorInterface *m_core;
    QAction *m_addResourceAction;
    QAction *m_addGradientAction;
    QAction *m_addColorAction;
    QAction *m_addFontAction;
};

// Edit the style sheet property of the designer selection.
// Provides an "Apply" button.

class Q_DESIGNER_EXPORT StyleSheetPropertyEditorDialog : public StyleSheetEditorDialog
{
    Q_OBJECT
public:
    StyleSheetPropertyEditorDialog(QWidget *parent, QDesignerFormWindowInterface *fw, QWidget *widget);

    static bool isStyleSheetValid(const QString &styleSheet);

private slots:
    void applyStyleSheet();

private:
    QDesignerFormWindowInterface *m_fw;
    QWidget *m_widget;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // STYLESHEETEDITOR_H
