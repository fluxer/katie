/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QSESSIONMANAGER_H
#define QSESSIONMANAGER_H

#include <QtCore/qobject.h>
#include <QtGui/qwindowdefs.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>

#ifndef QT_NO_SESSIONMANAGER


QT_BEGIN_NAMESPACE

class QApplication;
class QSessionManagerPrivate;

class Q_GUI_EXPORT  QSessionManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSessionManager)
    QSessionManager(QApplication *app, QString &id, QString &key);
    ~QSessionManager();
public:
    QString sessionId() const;
    QString sessionKey() const;
#if defined(Q_WS_X11)
    void *handle() const;
#endif

    bool allowsInteraction();
    bool allowsErrorInteraction();
    void release();

    void cancel();

    enum RestartHint {
        RestartIfRunning,
        RestartAnyway,
        RestartImmediately,
        RestartNever
    };
    void setRestartHint(RestartHint);
    RestartHint restartHint() const;

    void setRestartCommand(const QStringList&);
    QStringList restartCommand() const;
    void setDiscardCommand(const QStringList&);
    QStringList discardCommand() const;

    void setManagerProperty(const QString& name, const QString& value);
    void setManagerProperty(const QString& name, const QStringList& value);

    bool isPhase2() const;
    void requestPhase2();

private:
    friend class QApplication;
    friend class QApplicationPrivate;
};

QT_END_NAMESPACE


#endif // QT_NO_SESSIONMANAGER

#endif // QSESSIONMANAGER_H
