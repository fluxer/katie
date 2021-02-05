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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
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
