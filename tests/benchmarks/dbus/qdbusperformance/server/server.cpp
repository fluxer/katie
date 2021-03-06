/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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
#include <QtCore/QtCore>
#include <QtDBus/QtDBus>

#include "../serverobject.h"

static const char serviceName[] = "com.trolltech.autotests.performance";
static const char objectPath[] = "/";

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QDBusConnection con = QDBusConnection::sessionBus();
    if (!con.isConnected())
        exit(1);

    if (!con.registerService(QLatin1String(serviceName)))
        exit(2);

    ServerObject obj(QLatin1String(objectPath), con);
    printf("ready.\n");
    return app.exec();
}

#include "moc_serverobject.h"
