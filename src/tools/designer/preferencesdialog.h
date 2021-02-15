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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE

class QPushButton;
class QDesignerFormEditorInterface;
class QDesignerOptionsPageInterface;

class Ui_PreferencesDialog;

class PreferencesDialog: public QDialog
{
    Q_OBJECT
public:
    explicit PreferencesDialog(QDesignerFormEditorInterface *core, QWidget *parentWidget = 0);
    ~PreferencesDialog();


private slots:
    void slotAccepted();
    void slotRejected();
    void slotApply();
    void slotUiModeChanged(bool modified);

private:
    QPushButton *applyButton() const;
    void closeOptionPages();

    Ui_PreferencesDialog *m_ui;
    QList<QDesignerOptionsPageInterface*> m_optionsPages;
};

QT_END_NAMESPACE

#endif // PREFERENCESDIALOG_H
