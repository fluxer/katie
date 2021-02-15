/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

#include "customwidgetsinfo.h"
#include "driver.h"
#include "ui4.h"
#include "utils.h"

QT_BEGIN_NAMESPACE

CustomWidgetsInfo::CustomWidgetsInfo()
{
}

void CustomWidgetsInfo::acceptUI(DomUI *node)
{
    m_customWidgets.clear();

    if (node->elementCustomWidgets())
        acceptCustomWidgets(node->elementCustomWidgets());
}

void CustomWidgetsInfo::acceptCustomWidget(DomCustomWidget *node)
{
    if (node->elementClass().isEmpty())
        return;

    m_customWidgets.insert(node->elementClass(), node);
}

bool CustomWidgetsInfo::extends(const QString &classNameIn, const QLatin1String &baseClassName) const
{
    if (classNameIn == baseClassName)
        return true;

    QString className = classNameIn;
    while (const DomCustomWidget *c = customWidget(className)) {
        const QString extends = c->elementExtends();
        if (className == extends) // Faulty legacy custom widget entries exist.
            return false;
        if (extends == baseClassName)
            return true;
        className = extends;
    }
    return false;
}

bool CustomWidgetsInfo::isCustomWidgetContainer(const QString &className) const
{
    if (const DomCustomWidget *dcw = m_customWidgets.value(className, 0))
        if (dcw->hasElementContainer())
            return dcw->elementContainer() != 0;
    return false;
}

QString CustomWidgetsInfo::realClassName(const QString &className) const
{
    if (className == QLatin1String("Line"))
        return QLatin1String("QFrame");

    return className;
}

DomScript *CustomWidgetsInfo::customWidgetScript(const QString &name) const
{
    if (m_customWidgets.empty())
        return 0;

    const NameCustomWidgetMap::const_iterator it = m_customWidgets.constFind(name);
    if (it == m_customWidgets.constEnd())
        return 0;

    return it.value()->elementScript();
}

QString CustomWidgetsInfo::customWidgetAddPageMethod(const QString &name) const
{
    if (DomCustomWidget *dcw = m_customWidgets.value(name, 0))
        return dcw->elementAddPageMethod();
    return QString();
}


QT_END_NAMESPACE
