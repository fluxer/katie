/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the plugins of the Katie Toolkit.
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

#include "qsqldriverplugin.h"
#include "qstringlist.h"
#include "qsql_psql.h"

QT_BEGIN_NAMESPACE

class QPSQLDriverPlugin : public QSqlDriverPlugin
{
public:
    QPSQLDriverPlugin();

    QSqlDriver* create(const QString &);
    QStringList keys() const;
};

QPSQLDriverPlugin::QPSQLDriverPlugin()
    : QSqlDriverPlugin()
{
}

QSqlDriver* QPSQLDriverPlugin::create(const QString &name)
{
    if (name == QLatin1String("QPSQL") || name == QLatin1String("QPSQL7")) {
        QPSQLDriver* driver = new QPSQLDriver();
        return driver;
    }
    return 0;
}

QStringList QPSQLDriverPlugin::keys() const
{
    static const QStringList list = QStringList()
        << QLatin1String("QPSQL7")
        << QLatin1String("QPSQL");
    return list;
}

Q_EXPORT_PLUGIN2(qsqlpsql, QPSQLDriverPlugin)

QT_END_NAMESPACE
