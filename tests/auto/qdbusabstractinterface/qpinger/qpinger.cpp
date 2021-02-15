/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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
#include "../interface.h"

static const char serviceName[] = "com.trolltech.autotests.qpinger";
static const char objectPath[] = "/com/trolltech/qpinger";
//static const char *interfaceName = serviceName;

class PingerServer : public QDBusServer
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.trolltech.autotests.qpinger")
public:
    PingerServer(QString addr = "unix:tmpdir=/tmp", QObject *parent = Q_NULLPTR)
        : QDBusServer(addr, parent),
          m_conn("none")
    {
        connect(this, SIGNAL(newConnection(const QDBusConnection&)), SLOT(handleConnection(const QDBusConnection&)));
        reset();
    }

public slots:
    QString address() const
    {
        return QDBusServer::address();
    }

    bool isConnected() const
    {
        return m_conn.isConnected();
    }

    void reset()
    {
        targetObj.m_stringProp = "This is a test";
        targetObj.m_variantProp = QDBusVariant(QVariant(42));
        targetObj.m_complexProp = RegisteredType("This is a test");
    }

    void voidSignal()
    {
        emit targetObj.voidSignal();
    }

    void stringSignal(const QString& value)
    {
        emit targetObj.stringSignal(value);
    }

    void complexSignal(const QString& value)
    {
        RegisteredType reg(value);
        emit targetObj.complexSignal(reg);
    }

private slots:
    void handleConnection(const QDBusConnection& con)
    {
        m_conn = con;
        m_conn.registerObject("/", &targetObj, QDBusConnection::ExportScriptableContents);
    }

private:
    Interface targetObj;
    QDBusConnection m_conn;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // register the meta types
    qDBusRegisterMetaType<RegisteredType>();
    qRegisterMetaType<UnregisteredType>();

    QDBusConnection con = QDBusConnection::sessionBus();
    if (!con.isConnected())
        exit(1);

    if (!con.registerService(serviceName))
        exit(2);

    PingerServer server;
    con.registerObject(objectPath, &server, QDBusConnection::ExportAllSlots);

    printf("ready.\n");

    return app.exec();
}

#include "moc_qpinger.cpp"
