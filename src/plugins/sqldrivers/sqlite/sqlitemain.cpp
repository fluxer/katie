/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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
#include "qsql_sqlite.h"

QT_BEGIN_NAMESPACE

class QSQLiteDriverPlugin : public QSqlDriverPlugin
{
public:
    QSQLiteDriverPlugin();

    QSqlDriver* create(const QString &);
    QStringList keys() const;
};

QSQLiteDriverPlugin::QSQLiteDriverPlugin()
    : QSqlDriverPlugin()
{
}

QSqlDriver* QSQLiteDriverPlugin::create(const QString &name)
{
    if (name == QLatin1String("QSQLITE")) {
        QSQLiteDriver* driver = new QSQLiteDriver();
        return driver;
    }
    return 0;
}

QStringList QSQLiteDriverPlugin::keys() const
{
    const QStringList list(QLatin1String("QSQLITE"));
    return list;
}

Q_EXPORT_STATIC_PLUGIN(QSQLiteDriverPlugin)
Q_EXPORT_PLUGIN2(qsqlite, QSQLiteDriverPlugin)

QT_END_NAMESPACE

