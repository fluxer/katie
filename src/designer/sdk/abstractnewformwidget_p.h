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

#ifndef ABSTRACTNEWFORMWIDGET_H
#define ABSTRACTNEWFORMWIDGET_H

#include <QtGui/QWidget>


QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;

class Q_DESIGNER_EXPORT QDesignerNewFormWidgetInterface : public QWidget
{
    Q_DISABLE_COPY(QDesignerNewFormWidgetInterface)
    Q_OBJECT
public:
    explicit QDesignerNewFormWidgetInterface(QWidget *parent = nullptr);
    virtual ~QDesignerNewFormWidgetInterface();

    virtual bool hasCurrentTemplate() const = 0;
    virtual QString currentTemplate(QString *errorMessage = 0) = 0;

    static QDesignerNewFormWidgetInterface *createNewFormWidget(QDesignerFormEditorInterface *core, QWidget *parent = nullptr);

Q_SIGNALS:
    void templateActivated();
    void currentTemplateChanged(bool templateSelected);
};

QT_END_NAMESPACE


#endif // ABSTRACTNEWFORMWIDGET_H
