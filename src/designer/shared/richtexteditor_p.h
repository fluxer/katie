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

#ifndef RICHTEXTEDITOR_H
#define RICHTEXTEDITOR_H

#include <QtGui/QTextEdit>
#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class QTabWidget;
class QToolBar;

class QDesignerFormEditorInterface;

namespace qdesigner_internal {

class RichTextEditor;

class Q_DESIGNER_EXPORT RichTextEditorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RichTextEditorDialog(QDesignerFormEditorInterface *core, QWidget *parent = nullptr);
    ~RichTextEditorDialog();

    int showDialog();
    void setDefaultFont(const QFont &font);
    void setText(const QString &text);
    QString text(Qt::TextFormat format = Qt::AutoText) const;

private slots:
    void tabIndexChanged(int newIndex);
    void richTextChanged();
    void sourceChanged();

private:
    enum TabIndex { RichTextIndex, SourceIndex };
    enum State { Clean, RichTextChanged, SourceChanged };
    RichTextEditor *m_editor;
    QTextEdit      *m_text_edit;
    QTabWidget     *m_tab_widget;
    State m_state;
    QDesignerFormEditorInterface *m_core;
    int m_initialTab;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // RITCHTEXTEDITOR_H
