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

#ifndef QSYSTEMTRAYICON_H
#define QSYSTEMTRAYICON_H

#include <QtCore/qobject.h>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QtGui/qicon.h>


QT_BEGIN_NAMESPACE

class QSystemTrayIconPrivate;

class QMenu;
class QEvent;
class QWheelEvent;
class QMouseEvent;
class QPoint;

class Q_GUI_EXPORT QSystemTrayIcon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible DESIGNABLE false)

public:
    QSystemTrayIcon(QObject *parent = nullptr);
    QSystemTrayIcon(const QIcon &icon, QObject *parent = nullptr);
    ~QSystemTrayIcon();

    enum ActivationReason {
        Unknown,
        Context,
        DoubleClick,
        Trigger,
        MiddleClick
    };

#ifndef QT_NO_MENU
    void setContextMenu(QMenu *menu);
    QMenu *contextMenu() const;
#endif

    QIcon icon() const;
    void setIcon(const QIcon &icon);

    QString toolTip() const;
    void setToolTip(const QString &tip);

    static bool isSystemTrayAvailable();
    static bool supportsMessages();

    enum MessageIcon { NoIcon, Information, Warning, Critical };
    void showMessage(const QString &title, const QString &msg,
                     MessageIcon icon = Information, int msecs = 10000);

    QRect geometry() const;
    bool isVisible() const;

public Q_SLOTS:
    void setVisible(bool visible);
    inline void show() { setVisible(true); }
    inline void hide() { setVisible(false); }

Q_SIGNALS:
    void activated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();

protected:
    bool event(QEvent *event);

private:
    Q_DISABLE_COPY(QSystemTrayIcon)
    Q_DECLARE_PRIVATE(QSystemTrayIcon)

    friend class QSystemTrayIconSys;
    friend class QBalloonTip;
};

QT_END_NAMESPACE


#endif // QT_NO_SYSTEMTRAYICON
#endif // QSYSTEMTRAYICON_H
