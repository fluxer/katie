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

#ifndef FORMWINDOWSETTINGS_H
#define FORMWINDOWSETTINGS_H

#include <QDialog>

QT_BEGIN_NAMESPACE

class Ui_FormWindowSettings;

class QDesignerFormWindowInterface;

namespace qdesigner_internal {

struct FormWindowData;
class FormWindowBase;

/* Dialog to edit the settings of a QDesignerFormWindowInterface.
 * It sets the dirty flag on the form window if something was changed. */

class FormWindowSettings: public QDialog
{
    Q_DISABLE_COPY(FormWindowSettings)
    Q_OBJECT
public:
    explicit FormWindowSettings(QDesignerFormWindowInterface *formWindow);
    virtual ~FormWindowSettings();

    virtual void accept();

private:
    FormWindowData data() const;
    void setData(const FormWindowData&);

    Ui_FormWindowSettings *m_ui;
    FormWindowBase *m_formWindow;
    FormWindowData *m_oldData;
};
}

QT_END_NAMESPACE

#endif // FORMWINDOWSETTINGS_H
