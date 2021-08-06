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

#ifndef QANIMATION_H
#define QANIMATION_H

#include <QtCore/qeasingcurve.h>
#include <QtCore/qvector.h>
#include <QtCore/qvariant.h>
#include <QtCore/qpair.h>
#include <QtGui/qabstractanimation.h>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_ANIMATION

class QVariantAnimationPrivate;
class Q_GUI_EXPORT QVariantAnimation : public QAbstractAnimation
{
    Q_OBJECT
    Q_PROPERTY(QVariant startValue READ startValue WRITE setStartValue)
    Q_PROPERTY(QVariant endValue READ endValue WRITE setEndValue)
    Q_PROPERTY(QVariant currentValue READ currentValue NOTIFY valueChanged)
    Q_PROPERTY(int duration READ duration WRITE setDuration)
    Q_PROPERTY(QEasingCurve easingCurve READ easingCurve WRITE setEasingCurve)

public:
    typedef QPair<qreal, QVariant> KeyValue;
    typedef QVector<KeyValue> KeyValues;

    QVariantAnimation(QObject *parent = nullptr);
    ~QVariantAnimation();

    QVariant startValue() const;
    void setStartValue(const QVariant &value);

    QVariant endValue() const;
    void setEndValue(const QVariant &value);

    QVariant keyValueAt(qreal step) const;
    void setKeyValueAt(qreal step, const QVariant &value);

    KeyValues keyValues() const;
    void setKeyValues(const KeyValues &values);

    QVariant currentValue() const;

    int duration() const;
    void setDuration(int msecs);

    QEasingCurve easingCurve() const;
    void setEasingCurve(const QEasingCurve &easing);

    typedef QVariant (*Interpolator)(const void *from, const void *to, qreal progress);

Q_SIGNALS:
    void valueChanged(const QVariant &value);

protected:
    QVariantAnimation(QVariantAnimationPrivate &dd, QObject *parent = nullptr);

    void updateCurrentTime(int);
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

    virtual void updateCurrentValue(const QVariant &value) = 0;
    virtual QVariant interpolated(const QVariant &from, const QVariant &to, qreal progress) const;

private:
    template <typename T> friend void qRegisterAnimationInterpolator(QVariant (*func)(const T &, const T &, qreal));
    static void registerInterpolator(Interpolator func, int interpolationType);

    Q_DISABLE_COPY(QVariantAnimation)
    Q_DECLARE_PRIVATE(QVariantAnimation)
};

template <typename T>
void qRegisterAnimationInterpolator(QVariant (*func)(const T &from, const T &to, qreal progress)) {
    QVariantAnimation::registerInterpolator(reinterpret_cast<QVariantAnimation::Interpolator>(func), qMetaTypeId<T>());
}

#endif //QT_NO_ANIMATION

QT_END_NAMESPACE


#endif //QANIMATION_H
