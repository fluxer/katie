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

#ifndef QEASINGCURVE_H
#define QEASINGCURVE_H

#include <QtCore/qobjectdefs.h>
#include <QtCore/qdebug.h>


QT_BEGIN_NAMESPACE

class QDataStream;

class QEasingCurvePrivate;
class Q_CORE_EXPORT QEasingCurve
{
    Q_GADGET
    Q_ENUMS(Type)
public:
    enum Type {
        Linear,
        InQuad, OutQuad, InOutQuad, OutInQuad,
        InCubic, OutCubic, InOutCubic, OutInCubic,
        InQuart, OutQuart, InOutQuart, OutInQuart,
        InQuint, OutQuint, InOutQuint, OutInQuint,
        InSine, OutSine, InOutSine, OutInSine,
        InExpo, OutExpo, InOutExpo, OutInExpo,
        InCirc, OutCirc, InOutCirc, OutInCirc,
        InElastic, OutElastic, InOutElastic, OutInElastic,
        InBack, OutBack, InOutBack, OutInBack,
        InBounce, OutBounce, InOutBounce, OutInBounce,
        InCurve, OutCurve, SineCurve, CosineCurve,
        Custom, NCurveTypes
    };

    QEasingCurve(Type type = Linear);
    QEasingCurve(const QEasingCurve &other);
    ~QEasingCurve();

    QEasingCurve &operator=(const QEasingCurve &other);
    bool operator==(const QEasingCurve &other) const;
    inline bool operator!=(const QEasingCurve &other) const
    { return !(this->operator==(other)); }

    qreal amplitude() const;
    void setAmplitude(qreal amplitude);

    qreal period() const;
    void setPeriod(qreal period);

    qreal overshoot() const;
    void setOvershoot(qreal overshoot);

    Type type() const;
    void setType(Type type);
    typedef qreal (*EasingFunction)(qreal progress);
    void setCustomType(EasingFunction func);
    EasingFunction customType() const;

    qreal valueForProgress(qreal progress) const;
private:
    QEasingCurvePrivate *d_ptr;
#ifndef QT_NO_DEBUG_STREAM
    friend Q_CORE_EXPORT QDebug operator<<(QDebug debug, const QEasingCurve &item);
#endif
#ifndef QT_NO_DATASTREAM
    friend Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QEasingCurve&);
    friend Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QEasingCurve &);
#endif
};

#ifndef QT_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug debug, const QEasingCurve &item);
#endif

#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QEasingCurve&);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QEasingCurve &);
#endif

QT_END_NAMESPACE


#endif
