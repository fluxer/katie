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

#ifndef TEXTEDIT_TASKMENU_H
#define TEXTEDIT_TASKMENU_H

#include <QtGui/QTextEdit>
#include <QtGui/QPlainTextEdit>

#include "qdesigner_taskmenu_p.h"
#include <extensionfactory_p.h>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;

namespace qdesigner_internal {

class TextEditTaskMenu: public QDesignerTaskMenu
{
    Q_OBJECT
public:
    explicit TextEditTaskMenu(QTextEdit *button, QObject *parent = nullptr);
    explicit TextEditTaskMenu(QPlainTextEdit *button, QObject *parent = nullptr);

    virtual ~TextEditTaskMenu();

    virtual QAction *preferredEditAction() const;
    virtual QList<QAction*> taskActions() const;

private slots:
    void editText();

private:
    void initialize();

    const Qt::TextFormat m_format;
    const QString m_property;
    const QString m_windowTitle;

    mutable QList<QAction*> m_taskActions;
    QAction *m_editTextAction;
};

typedef ExtensionFactory<QDesignerTaskMenuExtension, QTextEdit, TextEditTaskMenu> TextEditTaskMenuFactory;
typedef ExtensionFactory<QDesignerTaskMenuExtension, QPlainTextEdit, TextEditTaskMenu> PlainTextEditTaskMenuFactory;
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // TEXTEDIT_TASKMENU_H
