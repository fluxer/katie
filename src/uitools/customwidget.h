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

#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QIcon>

QT_BEGIN_NAMESPACE

class QWidget;

class Q_UITOOLS_EXPORT QCustomWidget : public QObject
{
    Q_OBJECT
public:
    explicit QCustomWidget(QObject *parent = nullptr);
    virtual ~QCustomWidget();

    virtual QString name() const = 0;
    virtual QString group() const = 0;
    virtual QString toolTip() const = 0;
    virtual QString whatsThis() const = 0;
    virtual QString includeFile() const = 0;
    virtual QIcon icon() const = 0;

    virtual bool isContainer() const = 0;

    virtual QWidget *createWidget(QWidget *parent) = 0;

    virtual bool isInitialized() const { return false; }
    virtual void initialize() { }

    virtual QString domXml() const
    {
        return QString::fromUtf8("<widget class=\"%1\" name=\"%2\"/>")
            .arg(name()).arg(name().toLower());
    }

    virtual QString codeTemplate() const { return QString(); }
};

class Q_UITOOLS_EXPORT QCustomWidgetPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QCustomWidgetPlugin(QObject *parent = nullptr);
    virtual ~QCustomWidgetPlugin();

    virtual QList<QCustomWidget*> customWidgets() const = 0;
};

QT_END_NAMESPACE

#endif // CUSTOMWIDGET_H
