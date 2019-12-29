/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef FORMEDITOR_OPTIONSPAGE_H
#define FORMEDITOR_OPTIONSPAGE_H

#include <QtDesigner/abstractoptionspage_p.h>
#include <QtCore/QPointer>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;

namespace qdesigner_internal {

class PreviewConfigurationWidget;
class GridPanel;
class ZoomSettingsWidget;

class FormEditorOptionsPage : public QDesignerOptionsPageInterface
{
public:
    explicit FormEditorOptionsPage(QDesignerFormEditorInterface *core);

    QString name() const;
    QWidget *createPage(QWidget *parent);
    virtual void apply();
    virtual void finish();

private:
    QDesignerFormEditorInterface *m_core;
    QPointer<PreviewConfigurationWidget> m_previewConf;
    QPointer<GridPanel> m_defaultGridConf;
    QPointer<ZoomSettingsWidget> m_zoomSettingsWidget;
};

} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // FORMEDITOR_OPTIONSPAGE_H
