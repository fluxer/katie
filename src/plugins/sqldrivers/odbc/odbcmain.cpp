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
#include "qsql_odbc.h"

QT_BEGIN_NAMESPACE

class QODBCDriverPlugin : public QSqlDriverPlugin
{
public:
    QODBCDriverPlugin();

    QSqlDriver* create(const QString &);
    QStringList keys() const;
};

QODBCDriverPlugin::QODBCDriverPlugin()
    : QSqlDriverPlugin()
{
}

QSqlDriver* QODBCDriverPlugin::create(const QString &name)
{
    if (name == QLatin1String("QODBC") || name == QLatin1String("QODBC3")) {
        QODBCDriver* driver = new QODBCDriver();
        return driver;
    }
    return 0;
}

QStringList QODBCDriverPlugin::keys() const
{
    static const QStringList list = QStringList()
        << QLatin1String("QODBC3")
        << QLatin1String("QODBC");
    return list;
}

Q_EXPORT_PLUGIN2(qsqlodbc, QODBCDriverPlugin)

QT_END_NAMESPACE
