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

#include "qdeclarativeview_plugin.h"

#include <QtCore/qplugin.h>
#include <QtDeclarative/QDeclarativeView>

static const char toolTipC[] = "QtDeclarative view widget";

QT_BEGIN_NAMESPACE

QDeclarativeViewWidget::QDeclarativeViewWidget(QObject *parent)
    : QCustomWidget(parent),
    m_initialized(false)
{
}

QString QDeclarativeViewWidget::name() const
{
    return QLatin1String("QDeclarativeView");
}

QString QDeclarativeViewWidget::group() const
{
    return QLatin1String("Display Widgets");
}

QString QDeclarativeViewWidget::toolTip() const
{
    return tr(toolTipC);
}

QString QDeclarativeViewWidget::whatsThis() const
{
    return tr(toolTipC);
}

QString QDeclarativeViewWidget::includeFile() const
{
    return QLatin1String("QtDeclarative/QDeclarativeView");
}

QIcon QDeclarativeViewWidget::icon() const
{
    return QIcon();
}

bool QDeclarativeViewWidget::isContainer() const
{
    return false;
}

QWidget *QDeclarativeViewWidget::createWidget(QWidget *parent)
{
    return new QDeclarativeView(parent);
}

bool QDeclarativeViewWidget::isInitialized() const
{
    return m_initialized;
}

void QDeclarativeViewWidget::initialize()
{
    if (m_initialized)
        return;

    m_initialized = true;
}

QString QDeclarativeViewWidget::domXml() const
{
    return QLatin1String("\
    <ui language=\"c++\">\
        <widget class=\"QDeclarativeView\" name=\"declarativeView\">\
            <property name=\"geometry\">\
                <rect>\
                    <x>0</x>\
                    <y>0</y>\
                    <width>300</width>\
                    <height>200</height>\
                </rect>\
            </property>\
        </widget>\
    </ui>");
}


QDeclarativeViewPlugin::QDeclarativeViewPlugin(QObject *parent)
    : QCustomWidgetPlugin(parent)
{
    m_plugins.append(new QDeclarativeViewWidget(this));
}

QDeclarativeViewPlugin::~QDeclarativeViewPlugin()
{
}

QList<QCustomWidget*> QDeclarativeViewPlugin::customWidgets() const
{
    return m_plugins;
}

Q_EXPORT_PLUGIN(QDeclarativeViewPlugin)

QT_END_NAMESPACE

#include "moc_qdeclarativeview_plugin.h"
