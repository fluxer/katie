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

#ifndef QDBUSVIEWER_H
#define QDBUSVIEWER_H

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtGui/QtGui>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

QT_BEGIN_NAMESPACE

struct BusSignature
{
    QString mService, mPath, mInterface, mName;
    QString mTypeSig;
};

class QDBusViewer: public QWidget
{
    Q_OBJECT
public:
    QDBusViewer(const QDBusConnection &connection, QWidget *parent = nullptr);

public slots:
    void refresh();
    void about();

private slots:
    void serviceChanged(const QModelIndex &index);
    void showContextMenu(const QPoint &);
    void connectionRequested(const BusSignature &sig);
    void callMethod(const BusSignature &sig);
    void getProperty(const BusSignature &sig);
    void setProperty(const BusSignature &sig);
    void dumpMessage(const QDBusMessage &msg);
    void refreshChildren();

    void serviceRegistered(const QString &service);
    void serviceUnregistered(const QString &service);
    void serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner);

    void activate(const QModelIndex &item);

    void logError(const QString &msg);
    void anchorClicked(const QUrl &url);

private:
    void logMessage(const QString &msg);

    QDBusConnection c;
    QString currentService;
    QTreeView *tree;
    QAction *refreshAction;
    QStringListModel *servicesModel;
    QSortFilterProxyModel *servicesFilterModel;
    QLineEdit *serviceFilterLine;
    QListView *servicesView;
    QTextBrowser *log;
    QRegExp objectPathRegExp;
};

QT_END_NAMESPACE

#endif
