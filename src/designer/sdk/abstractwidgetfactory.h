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

#ifndef ABSTRACTWIDGETFACTORY_H
#define ABSTRACTWIDGETFACTORY_H

#include <QtCore/QObject>


QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QWidget;
class QLayout;

class Q_DESIGNER_EXPORT QDesignerWidgetFactoryInterface: public QObject
{
    Q_OBJECT
public:
    QDesignerWidgetFactoryInterface(QObject *parent = nullptr);
    virtual ~QDesignerWidgetFactoryInterface();

    virtual QDesignerFormEditorInterface *core() const = 0;

    virtual QWidget* containerOfWidget(QWidget *w) const = 0;
    virtual QWidget* widgetOfContainer(QWidget *w) const = 0;

    virtual QWidget *createWidget(const QString &name, QWidget *parentWidget = 0) const = 0;
    virtual QLayout *createLayout(QWidget *widget, QLayout *layout, int type) const = 0;

    virtual bool isPassiveInteractor(QWidget *widget) = 0;
    virtual void initialize(QObject *object) const = 0;
};

QT_END_NAMESPACE


#endif // ABSTRACTWIDGETFACTORY_H
