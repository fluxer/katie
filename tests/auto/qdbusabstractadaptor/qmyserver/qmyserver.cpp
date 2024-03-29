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

#include "../myobject.h"

static const char serviceName[] = "com.trolltech.autotests.qmyserver";
static const char objectPath[] = "/com/trolltech/qmyserver";
//static const char *interfaceName = serviceName;

const char *slotSpy;
QString valueSpy;

Q_DECLARE_METATYPE(QDBusConnection::RegisterOptions)

class MyServer : public QDBusServer
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.trolltech.autotests.qmyserver")

public:
    MyServer(QString addr = "unix:tmpdir=/tmp", QObject *parent = nullptr)
        : QDBusServer(addr, parent),
          m_conn("none"),
          obj(NULL)
    {
        connect(this, SIGNAL(newConnection(const QDBusConnection&)), SLOT(handleConnection(const QDBusConnection&)));
    }

    ~MyServer()
    {
        if (obj)
            obj->deleteLater();
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

    void emitSignal(const QString& interface, const QString& name, const QDBusVariant& parameter)
    {
        if (interface.endsWith('2'))
            obj->if2->emitSignal(name, parameter.variant());
        else if (interface.endsWith('3'))
            obj->if3->emitSignal(name, parameter.variant());
        else if (interface.endsWith('4'))
            obj->if4->emitSignal(name, parameter.variant());
        else
            obj->emitSignal(name, parameter.variant());
    }

    void emitSignal2(const QString& interface, const QString& name)
    {
        if (interface.endsWith('2'))
            obj->if2->emitSignal(name, QVariant());
        else if (interface.endsWith('3'))
            obj->if3->emitSignal(name, QVariant());
        else if (interface.endsWith('4'))
            obj->if4->emitSignal(name, QVariant());
        else
            obj->emitSignal(name, QVariant());
    }

    void newMyObject(int nInterfaces = 4)
    {
        if (obj)
            obj->deleteLater();
        obj = new MyObject(nInterfaces);
    }

    void registerMyObject(const QString & path, int options)
    {
        m_conn.registerObject(path, obj, (QDBusConnection::RegisterOptions)options);
    }

    QString slotSpyServer()
    {
        return QLatin1String(slotSpy);
    }

    QString valueSpyServer()
    {
        return valueSpy;
    }

    void clearValueSpy()
    {
        valueSpy.clear();
    }

private slots:
    void handleConnection(const QDBusConnection& con)
    {
        m_conn = con;
    }

private:
    QDBusConnection m_conn;
    MyObject* obj;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QDBusConnection con = QDBusConnection::sessionBus();
    if (!con.isConnected())
        exit(1);

    if (!con.registerService(serviceName))
        exit(2);

    MyServer server;
    con.registerObject(objectPath, &server, QDBusConnection::ExportAllSlots);

    printf("ready.\n");

    return app.exec();
}

#include "moc_qmyserver.cpp"
#include "moc_myobject.h"
