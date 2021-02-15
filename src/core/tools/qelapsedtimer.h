/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QTIMESTAMP_H
#define QTIMESTAMP_H

#include <QtCore/qglobal.h>


QT_BEGIN_NAMESPACE


class Q_CORE_EXPORT QElapsedTimer
{
public:
    enum ClockType {
        SystemTime,
        MonotonicClock
    };
    static ClockType clockType();
    static bool isMonotonic();

    void start();
    qint64 restart();
    void invalidate();
    bool isValid() const;

    qint64 nsecsElapsed() const;
    qint64 elapsed() const;
    bool hasExpired(qint64 timeout) const;

    qint64 msecsSinceReference() const;
    qint64 msecsTo(const QElapsedTimer &other) const;
    qint64 secsTo(const QElapsedTimer &other) const;

    bool operator==(const QElapsedTimer &other) const
    { return t1 == other.t1 && t2 == other.t2; }
    bool operator!=(const QElapsedTimer &other) const
    { return !(*this == other); }

    friend bool Q_CORE_EXPORT operator<(const QElapsedTimer &v1, const QElapsedTimer &v2);

private:
    qint64 t1;
    qint64 t2;
};

QT_END_NAMESPACE


#endif // QTIMESTAMP_H
