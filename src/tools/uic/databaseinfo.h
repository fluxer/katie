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

#ifndef DATABASEINFO_H
#define DATABASEINFO_H

#include "treewalker.h"
#include <QtCore/QStringList>
#include <QtCore/QMap>

QT_BEGIN_NAMESPACE

class DatabaseInfo : public TreeWalker
{
public:
    DatabaseInfo();

    void acceptUI(DomUI *node);
    void acceptWidget(DomWidget *node);

    inline QStringList connections() const
    { return m_connections; }

    inline QStringList cursors(const QString &connection) const
    { return m_cursors.value(connection); }

    inline QStringList fields(const QString &connection) const
    { return m_fields.value(connection); }

private:
    QStringList m_connections;
    QMap<QString, QStringList> m_cursors;
    QMap<QString, QStringList> m_fields;
};

QT_END_NAMESPACE

#endif // DATABASEINFO_H
