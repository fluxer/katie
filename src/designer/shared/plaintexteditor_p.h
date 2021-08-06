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

#ifndef PLAINTEXTEDITOR_H
#define PLAINTEXTEDITOR_H

#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class QPlainTextEdit;
class QDesignerFormEditorInterface;

namespace qdesigner_internal {

class Q_DESIGNER_EXPORT PlainTextEditorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlainTextEditorDialog(QDesignerFormEditorInterface *core, QWidget *parent = nullptr);
    ~PlainTextEditorDialog();

    int showDialog();

    void setDefaultFont(const QFont &font);

    void setText(const QString &text);
    QString text() const;

private:
    QPlainTextEdit *m_editor;
    QDesignerFormEditorInterface *m_core;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // RITCHTEXTEDITOR_H
