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

#ifndef SYSTEMSETTINGSDIALOG_H
#define SYSTEMSETTINGSDIALOG_H

#include <QtGui/QDialog>
#include <QtCore/QStringList>

QT_BEGIN_NAMESPACE

class Ui_DeviceProfileDialog;
class QDesignerDialogGuiInterface;

class QDialogButtonBox;

namespace qdesigner_internal {

class DeviceProfile;

/* DeviceProfileDialog: Widget to edit system settings for embedded design */

class DeviceProfileDialog : public QDialog
{
    Q_DISABLE_COPY(DeviceProfileDialog)
    Q_OBJECT
public:
    explicit DeviceProfileDialog(QDesignerDialogGuiInterface *dlgGui, QWidget *parent = nullptr);
    ~DeviceProfileDialog();

    DeviceProfile deviceProfile() const;
    void setDeviceProfile(const DeviceProfile &s);

    bool showDialog(const QStringList &existingNames);

private slots:
    void setOkButtonEnabled(bool);
    void nameChanged(const QString &name);
    void save();
    void open();

private:
    void critical(const QString &title, const QString &msg);
    Ui_DeviceProfileDialog *m_ui;
    QDesignerDialogGuiInterface *m_dlgGui;
    QStringList m_existingNames;
};
}

QT_END_NAMESPACE

#endif // SYSTEMSETTINGSDIALOG_H
