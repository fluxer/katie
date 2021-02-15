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

#ifndef CUSTOMWIDGETSINFO_H
#define CUSTOMWIDGETSINFO_H

#include "treewalker.h"
#include <QtCore/QStringList>
#include <QtCore/QMap>

QT_BEGIN_NAMESPACE

class Driver;
class DomScript;

class CustomWidgetsInfo : public TreeWalker
{
public:
    CustomWidgetsInfo();

    void acceptUI(DomUI *node);
    void acceptCustomWidget(DomCustomWidget *node);

    inline QStringList customWidgets() const
    { return m_customWidgets.keys(); }

    inline bool hasCustomWidget(const QString &name) const
    { return m_customWidgets.contains(name); }

    inline DomCustomWidget *customWidget(const QString &name) const
    { return m_customWidgets.value(name); }

    DomScript *customWidgetScript(const QString &name) const;

    QString customWidgetAddPageMethod(const QString &name) const;

    QString realClassName(const QString &className) const;

    bool extends(const QString &className, const QLatin1String &baseClassName) const;

    bool isCustomWidgetContainer(const QString &className) const;

private:
    typedef QMap<QString, DomCustomWidget*> NameCustomWidgetMap;
    NameCustomWidgetMap m_customWidgets;
};

QT_END_NAMESPACE

#endif // CUSTOMWIDGETSINFO_H
