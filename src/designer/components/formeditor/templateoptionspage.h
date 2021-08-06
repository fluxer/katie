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

#ifndef QDESIGNER_TEMPLATEOPTIONS_H
#define QDESIGNER_TEMPLATEOPTIONS_H

#include "abstractoptionspage_p.h"

#include <QtCore/QPointer>
#include <QtCore/QStringList>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;

namespace qdesigner_internal {

class Ui_TemplateOptionsWidget;

/* Present the user with a list of form template paths to save
 * form templates. */
class TemplateOptionsWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(TemplateOptionsWidget)
public:
    explicit TemplateOptionsWidget(QDesignerFormEditorInterface *core,
                                              QWidget *parent = nullptr);
    ~TemplateOptionsWidget();


    QStringList templatePaths() const;
    void setTemplatePaths(const QStringList &l);

    static QString chooseTemplatePath(QDesignerFormEditorInterface *core, QWidget *parent);

private slots:
    void addTemplatePath();
    void removeTemplatePath();
    void templatePathSelectionChanged();

private:
    QDesignerFormEditorInterface *m_core;
    Ui_TemplateOptionsWidget *m_ui;
};

class TemplateOptionsPage : public QDesignerOptionsPageInterface
{
     Q_DISABLE_COPY(TemplateOptionsPage)
public:
    explicit TemplateOptionsPage(QDesignerFormEditorInterface *core);

    virtual QString name() const;
    virtual QWidget *createPage(QWidget *parent);
    virtual void apply();
    virtual void finish();

private:
    QDesignerFormEditorInterface *m_core;
    QStringList m_initialTemplatePaths;
    QPointer<TemplateOptionsWidget> m_widget;
};

}

QT_END_NAMESPACE

#endif // QDESIGNER_TEMPLATEOPTIONS_H
