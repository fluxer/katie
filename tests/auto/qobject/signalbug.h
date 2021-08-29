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

#ifndef SIGNAL_BUG_H
#define SIGNAL_BUG_H


#include <QObject>


class Sender;


class Receiver : public QObject
{
    Q_OBJECT

public:
    Receiver ();
    virtual ~Receiver () {}

protected slots:
    void received ();

public:
    Sender *s;
};


class Disconnector : public QObject
{
    Q_OBJECT

public:
    Disconnector ();
    virtual ~Disconnector () {}

protected slots:
    void received ();

public:
    Sender *s;
};


class Sender : public QObject
{
Q_OBJECT

public:
    Sender (Receiver *r, Disconnector *d);
    virtual ~Sender () {}

    void fire ();

signals:
    void fired ();

public:
    Receiver *r;
    Disconnector *d;
};


#endif  // SIGNAL_BUG_H
