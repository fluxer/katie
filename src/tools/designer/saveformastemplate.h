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

#ifndef SAVEFORMASTEMPLATE_H
#define SAVEFORMASTEMPLATE_H

#include "ui_saveformastemplate.h"

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;

class SaveFormAsTemplate: public QDialog
{
    Q_OBJECT
public:
    explicit SaveFormAsTemplate(QDesignerFormEditorInterface *m_core,
                                QDesignerFormWindowInterface *formWindow,
                                QWidget *parent = nullptr);
    virtual ~SaveFormAsTemplate();

private slots:
    void accept();
    void updateOKButton(const QString &str);
    void checkToAddPath(int itemIndex);

private:
    static QString chooseTemplatePath(QWidget *parent);

    Ui::SaveFormAsTemplate ui;
    QDesignerFormEditorInterface *m_core;
    QDesignerFormWindowInterface *m_formWindow;
    int m_addPathIndex;
};

QT_END_NAMESPACE

#endif // SAVEFORMASTEMPLATE_H
