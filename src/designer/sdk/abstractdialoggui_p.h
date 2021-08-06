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

#ifndef ABSTRACTDIALOGGUI_H
#define ABSTRACTDIALOGGUI_H

#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>


QT_BEGIN_NAMESPACE

class Q_DESIGNER_EXPORT QDesignerDialogGuiInterface
{
    Q_DISABLE_COPY(QDesignerDialogGuiInterface)
public:
    QDesignerDialogGuiInterface();
    virtual ~QDesignerDialogGuiInterface();

    enum Message { FormLoadFailureMessage, UiVersionMismatchMessage, ResourceLoadFailureMessage,
                   TopLevelSpacerMessage, PropertyEditorMessage, SignalSlotEditorMessage, FormEditorMessage,
                   PreviewFailureMessage, PromotionErrorMessage, ResourceEditorMessage,
                   ScriptDialogMessage, SignalSlotDialogMessage, OtherMessage, FileChangedMessage };

    virtual QMessageBox::StandardButton
        message(QWidget *parent, Message context, QMessageBox::Icon icon,
                const QString &title, const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                QMessageBox::StandardButton defaultButton = QMessageBox::NoButton) = 0;

    virtual QMessageBox::StandardButton
        message(QWidget *parent, Message context, QMessageBox::Icon icon,
                const QString &title, const QString &text, const QString &informativeText,
                QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                QMessageBox::StandardButton defaultButton = QMessageBox::NoButton) = 0;

    virtual QMessageBox::StandardButton
        message(QWidget *parent, Message context, QMessageBox::Icon icon,
                const QString &title, const QString &text, const QString &informativeText, const QString &detailedText,
                QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                QMessageBox::StandardButton defaultButton = QMessageBox::NoButton) = 0;

    virtual QString getExistingDirectory(QWidget *parent = nullptr, const QString &caption = QString(), const QString &dir = QString(), QFileDialog::Options options = QFileDialog::ShowDirsOnly)= 0;
    virtual QString getOpenFileName(QWidget *parent = nullptr, const QString &caption = QString(), const QString &dir = QString(), const QString &filter = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0)= 0;
    virtual QString getOpenImageFileName(QWidget *parent = nullptr, const QString &caption = QString(), const QString &dir = QString(), const QString &filter = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0);
    virtual QStringList getOpenFileNames(QWidget *parent = nullptr, const QString &caption = QString(), const QString &dir = QString(), const QString &filter = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0)= 0;
    virtual QStringList getOpenImageFileNames(QWidget *parent = nullptr, const QString &caption = QString(), const QString &dir = QString(), const QString &filter = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0);
    virtual QString getSaveFileName(QWidget *parent = nullptr, const QString &caption = QString(), const QString &dir = QString(), const QString &filter = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0)= 0;
};

QT_END_NAMESPACE


#endif // ABSTRACTDIALOGGUI_H
