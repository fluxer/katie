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

#ifndef NEWFORM_H
#define NEWFORM_H

#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {
    class DeviceProfile;
}

class QDesignerFormEditorInterface;
class QDesignerNewFormWidgetInterface;
class QDesignerWorkbench;

class QCheckBox;
class QAbstractButton;
class QPushButton;
class QDialogButtonBox;
class QImage;
class QIODevice;

class NewForm: public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(NewForm)

public:
    NewForm(QDesignerWorkbench *workbench,
            QWidget *parentWidget,
            // Use that file name instead of a temporary one
            const QString &fileName = QString());

    virtual ~NewForm();

    // Convenience for implementing file dialogs with preview
    static QImage grabForm(QDesignerFormEditorInterface *core,
                           QIODevice &file,
                           const QString &workingDir,
                           const qdesigner_internal::DeviceProfile &dp);

private slots:
    void slotButtonBoxClicked(QAbstractButton *btn);
    void recentFileChosen();
    void slotCurrentTemplateChanged(bool templateSelected);
    void slotTemplateActivated();

private:
    QDialogButtonBox *createButtonBox();
    bool openTemplate(QString *ptrToErrorMessage);

    QString m_fileName;
    QDesignerNewFormWidgetInterface *m_newFormWidget;
    QDesignerWorkbench *m_workbench;
    QCheckBox *m_chkShowOnStartup;
    QPushButton *m_createButton;
    QPushButton *m_recentButton;
    QDialogButtonBox *m_buttonBox;
};

QT_END_NAMESPACE

#endif // NEWFORM_H
