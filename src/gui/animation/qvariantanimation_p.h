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

#ifndef QANIMATION_P_H
#define QANIMATION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of QIODevice. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qvariantanimation.h"
#include "qabstractanimation_p.h"
#include <QtCore/qeasingcurve.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qvector.h>

#ifndef QT_NO_ANIMATION

QT_BEGIN_NAMESPACE

class QVariantAnimationPrivate : public QAbstractAnimationPrivate
{
    Q_DECLARE_PUBLIC(QVariantAnimation)
public:

    QVariantAnimationPrivate();

    static QVariantAnimationPrivate *get(QVariantAnimation *q)
    {
        return q->d_func();
    }

    void setDefaultStartEndValue(const QVariant &value);


    QVariant currentValue;
    QVariant defaultStartEndValue;

    //this is used to keep track of the KeyValue interval in which we currently are
    struct
    {
        QVariantAnimation::KeyValue start, end;
    } currentInterval;

    QEasingCurve easing;
    int duration;
    QVariantAnimation::KeyValues keyValues;
    QVariantAnimation::Interpolator interpolator;

    void setCurrentValueForProgress(const qreal progress);
    void recalculateCurrentInterval(bool force=false);
    void setValueAt(qreal, const QVariant &);
    QVariant valueAt(qreal step) const;
    void convertValues(int t);

    void updateInterpolator();

    // this is needed by declarative animations
    static Q_GUI_EXPORT QVariantAnimation::Interpolator getInterpolator(int interpolationType);
};

//this should make the interpolation faster
template<typename T>
inline T _q_interpolate(const T &f, const T &t, qreal progress)
{
    return T(f + (t - f) * progress);
}

template<typename T >
inline QVariant _q_interpolateVariant(const T &from, const T &to, qreal progress)
{
    return _q_interpolate(from, to, progress);
}


QT_END_NAMESPACE

#endif //QT_NO_ANIMATION

#endif //QANIMATION_P_H
