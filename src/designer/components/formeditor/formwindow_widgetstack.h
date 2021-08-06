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

#ifndef FORMWINDOW_WIDGETSTACK_H
#define FORMWINDOW_WIDGETSTACK_H

#include "formeditor_global.h"

#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowToolInterface;

class QStackedLayout;
class QWidget;

namespace qdesigner_internal {

class QT_FORMEDITOR_EXPORT FormWindowWidgetStack: public QObject
{
    Q_OBJECT
public:
    FormWindowWidgetStack(QObject *parent = nullptr);
    virtual ~FormWindowWidgetStack();

    QLayout *layout() const;

    int count() const;
    QDesignerFormWindowToolInterface *tool(int index) const;
    QDesignerFormWindowToolInterface *currentTool() const;
    int currentIndex() const;
    int indexOf(QDesignerFormWindowToolInterface *tool) const;

    void setMainContainer(QWidget *w = 0);

    // Return the widget containing the form which can be used to apply embedded design settings to.
    // These settings should not affect the other editing tools.
    QWidget *formContainer() const { return m_formContainer; }

signals:
    void currentToolChanged(int index);

public slots:
    void addTool(QDesignerFormWindowToolInterface *tool);
    void setCurrentTool(QDesignerFormWindowToolInterface *tool);
    void setCurrentTool(int index);
    void setSenderAsCurrentTool();

protected:
    QWidget *defaultEditor() const;

private:
    QList<QDesignerFormWindowToolInterface*> m_tools;
    QWidget *m_formContainer;
    QStackedLayout *m_formContainerLayout;
    QStackedLayout *m_layout;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // FORMWINDOW_WIDGETSTACK_H
