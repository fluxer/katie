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

#ifndef QACTIONGROUP_H
#define QACTIONGROUP_H

#include <QtGui/qaction.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_ACTION

class QActionGroupPrivate;

class Q_GUI_EXPORT QActionGroup : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QActionGroup)

    Q_PROPERTY(bool exclusive READ isExclusive WRITE setExclusive)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible)

public:
    explicit QActionGroup(QObject* parent);
    ~QActionGroup();

    QAction *addAction(QAction* a);
    QAction *addAction(const QString &text);
    QAction *addAction(const QIcon &icon, const QString &text);
    void removeAction(QAction *a);
    QList<QAction*> actions() const;

    QAction *checkedAction() const;
    bool isExclusive() const;
    bool isEnabled() const;
    bool isVisible() const;


public Q_SLOTS:
    void setEnabled(bool);
    inline void setDisabled(bool b) { setEnabled(!b); }
    void setVisible(bool);
    void setExclusive(bool);

Q_SIGNALS:
    void triggered(QAction *);
    void hovered(QAction *);

private:
    Q_DISABLE_COPY(QActionGroup)
    Q_PRIVATE_SLOT(d_func(), void _q_actionTriggered())
    Q_PRIVATE_SLOT(d_func(), void _q_actionChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_actionHovered())
};

#endif // QT_NO_ACTION

QT_END_NAMESPACE


#endif // QACTIONGROUP_H
