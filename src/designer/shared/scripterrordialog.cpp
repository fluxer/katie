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

#include "scripterrordialog_p.h"

#include <QtGui/QTextEdit>
#include <QtGui/QTextCursor>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QPen>
#include <QtCore/QCoreApplication>

QT_BEGIN_NAMESPACE

static void formatError(const QFormScriptRunner::Error &error,
                        QTextCursor &cursor)
{
    const QTextCharFormat oldFormat = cursor.charFormat();
    // Message
    cursor.insertText(QCoreApplication::translate("ScriptErrorDialog", "An error occurred while running the scripts for \"%1\":\n").arg(error.objectName));

    QTextCharFormat format(oldFormat);

    // verbatim listing
    format.setFontFamily(QLatin1String("Courier"));
    cursor.insertText(error.script, format);

    const QString newLine(QLatin1Char('\n'));

    cursor.insertText(newLine);

    // red error
    format = oldFormat;
    format.setTextOutline(QPen(Qt::red));
    cursor.insertText(error.errorMessage, format);
    cursor.insertText(newLine);
    cursor.setCharFormat (oldFormat);
}

namespace qdesigner_internal {

    // ScriptErrorDialog
    ScriptErrorDialog::ScriptErrorDialog(const Errors& errors, QWidget *parent) :
        QDialog(parent),
        m_textEdit(new QTextEdit)
    {
        setWindowTitle(tr("Script errors"));
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
        setModal(true);

        QVBoxLayout *vboxLayout = new QVBoxLayout(this);

        m_textEdit->setReadOnly(true);
        m_textEdit->setMinimumSize(QSize(600, 400));
        vboxLayout->addWidget(m_textEdit);
        // button box
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
        connect(buttonBox , SIGNAL(rejected()), this, SLOT(reject()));
        vboxLayout->addWidget(buttonBox);

        // Generate text
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.movePosition (QTextCursor::End);
        foreach (const QFormScriptRunner::Error error, errors)
            formatError(error, cursor);
    }
} // namespace qdesigner_internal

QT_END_NAMESPACE
#include "moc_scripterrordialog_p.h"
