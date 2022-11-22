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

/*

| *property* | *Used for type* |
| period     | QEasingCurve::{In,Out,InOut,OutIn}Elastic |
| amplitude  | QEasingCurve::{In,Out,InOut,OutIn}Bounce, QEasingCurve::{In,Out,InOut,OutIn}Elastic |
| overshoot  | QEasingCurve::{In,Out,InOut,OutIn}Back   |

*/




/*!
    \class QEasingCurve
    \since 4.6
    \ingroup animation
    \brief The QEasingCurve class provides easing curves for controlling animation.

    Easing curves describe a function that controls how the speed of the interpolation
    between 0 and 1 should be. Easing curves allow transitions from
    one value to another to appear more natural than a simple constant speed would allow.
    The QEasingCurve class is usually used in conjunction with the QVariantAnimation and
    QPropertyAnimation classes but can be used on its own. It is usually used to accelerate
    the interpolation from zero velocity (ease in) or decelerate to zero velocity (ease out).
    Ease in and ease out can also be combined in the same easing curve.

    To calculate the speed of the interpolation, the easing curve provides the function
    valueForProgress(), where the \a progress argument specifies the progress of the
    interpolation: 0 is the start value of the interpolation, 1 is the end value of the
    interpolation. The returned value is the effective progress of the interpolation.
    If the returned value is the same as the input value for all input values the easing
    curve is a linear curve. This is the default behaviour.

    For example,
    \code
    QEasingCurve easing(QEasingCurve::InOutQuad);

    for(qreal t = 0.0; t < 1.0; t+=0.1)
        qWarning() << "Effective progress" << t << " is
                   << easing.valueForProgress(t);
    \endcode
    will print the effective progress of the interpolation between 0 and 1.

    When using a QPropertyAnimation, the associated easing curve will be used to control the
    progress of the interpolation between startValue and endValue:
    \code
    QPropertyAnimation animation;
    animation.setStartValue(0);
    animation.setEndValue(1000);
    animation.setDuration(1000);
    animation.setEasingCurve(QEasingCurve::InOutQuad);
    \endcode

    The ability to set an amplitude, overshoot, or period depends on
    the QEasingCurve type. Amplitude access is available to curves
    that behave as springs such as elastic and bounce curves. Changing
    the amplitude changes the height of the curve. Period access is
    only available to elastic curves and setting a higher period slows
    the rate of bounce. Only curves that have "boomerang" behaviors
    such as the InBack, OutBack, InOutBack, and OutInBack have
    overshoot settings. These curves will interpolate beyond the end
    points and return to the end point, acting similar to a boomerang.

    The \l{Easing Curves Example} contains samples of QEasingCurve
    types and lets you change the curve settings.

    \legalese
    Copyright © 2001 Robert Penner
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.
        * Neither the name of the author nor the names of contributors may be used
        to endorse or promote products derived from this software without specific
        prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    \endlegalese
*/

/*!
    \enum QEasingCurve::Type

    The type of easing curve.

    \value Linear       \inlineimage qeasingcurve-linear.png
                        \br
                        Easing curve for a linear (t) function:
                        velocity is constant.
    \value InQuad       \inlineimage qeasingcurve-inquad.png
                        \br
                        Easing curve for a quadratic (t^2) function:
                        accelerating from zero velocity.
    \value OutQuad      \inlineimage qeasingcurve-outquad.png
                        \br
                        Easing curve for a quadratic (t^2) function:
                        decelerating to zero velocity.
    \value InOutQuad    \inlineimage qeasingcurve-inoutquad.png
                        \br
                        Easing curve for a quadratic (t^2) function:
                        acceleration until halfway, then deceleration.
    \value OutInQuad    \inlineimage qeasingcurve-outinquad.png
                        \br
                        Easing curve for a quadratic (t^2) function:
                        deceleration until halfway, then acceleration.
    \value InCubic      \inlineimage qeasingcurve-incubic.png
                        \br
                        Easing curve for a cubic (t^3) function:
                        accelerating from zero velocity.
    \value OutCubic     \inlineimage qeasingcurve-outcubic.png
                        \br
                        Easing curve for a cubic (t^3) function:
                        decelerating to zero velocity.
    \value InOutCubic   \inlineimage qeasingcurve-inoutcubic.png
                        \br
                        Easing curve for a cubic (t^3) function:
                        acceleration until halfway, then deceleration.
    \value OutInCubic   \inlineimage qeasingcurve-outincubic.png
                        \br
                        Easing curve for a cubic (t^3) function:
                        deceleration until halfway, then acceleration.
    \value InQuart      \inlineimage qeasingcurve-inquart.png
                        \br
                        Easing curve for a quartic (t^4) function:
                        accelerating from zero velocity.
    \value OutQuart     \inlineimage qeasingcurve-outquart.png
                        \br
                        Easing curve for a quartic (t^4) function:
                        decelerating to zero velocity.
    \value InOutQuart   \inlineimage qeasingcurve-inoutquart.png
                        \br
                        Easing curve for a quartic (t^4) function:
                        acceleration until halfway, then deceleration.
    \value OutInQuart   \inlineimage qeasingcurve-outinquart.png
                        \br
                        Easing curve for a quartic (t^4) function:
                        deceleration until halfway, then acceleration.
    \value InQuint      \inlineimage qeasingcurve-inquint.png
                        \br
                        Easing curve for a quintic (t^5) easing
                        in: accelerating from zero velocity.
    \value OutQuint     \inlineimage qeasingcurve-outquint.png
                        \br
                        Easing curve for a quintic (t^5) function:
                        decelerating to zero velocity.
    \value InOutQuint   \inlineimage qeasingcurve-inoutquint.png
                        \br
                        Easing curve for a quintic (t^5) function:
                        acceleration until halfway, then deceleration.
    \value OutInQuint   \inlineimage qeasingcurve-outinquint.png
                        \br
                        Easing curve for a quintic (t^5) function:
                        deceleration until halfway, then acceleration.
    \value InSine       \inlineimage qeasingcurve-insine.png
                        \br
                        Easing curve for a sinusoidal (sin(t)) function:
                        accelerating from zero velocity.
    \value OutSine      \inlineimage qeasingcurve-outsine.png
                        \br
                        Easing curve for a sinusoidal (sin(t)) function:
                        decelerating from zero velocity.
    \value InOutSine    \inlineimage qeasingcurve-inoutsine.png
                        \br
                        Easing curve for a sinusoidal (sin(t)) function:
                        acceleration until halfway, then deceleration.
    \value OutInSine    \inlineimage qeasingcurve-outinsine.png
                        \br
                        Easing curve for a sinusoidal (sin(t)) function:
                        deceleration until halfway, then acceleration.
    \value InExpo       \inlineimage qeasingcurve-inexpo.png
                        \br
                        Easing curve for an exponential (2^t) function:
                        accelerating from zero velocity.
    \value OutExpo      \inlineimage qeasingcurve-outexpo.png
                        \br
                        Easing curve for an exponential (2^t) function:
                        decelerating from zero velocity.
    \value InOutExpo    \inlineimage qeasingcurve-inoutexpo.png
                        \br
                        Easing curve for an exponential (2^t) function:
                        acceleration until halfway, then deceleration.
    \value OutInExpo    \inlineimage qeasingcurve-outinexpo.png
                        \br
                        Easing curve for an exponential (2^t) function:
                        deceleration until halfway, then acceleration.
    \value InCirc       \inlineimage qeasingcurve-incirc.png
                        \br
                        Easing curve for a circular (sqrt(1-t^2)) function:
                        accelerating from zero velocity.
    \value OutCirc      \inlineimage qeasingcurve-outcirc.png
                        \br
                        Easing curve for a circular (sqrt(1-t^2)) function:
                        decelerating from zero velocity.
    \value InOutCirc    \inlineimage qeasingcurve-inoutcirc.png
                        \br
                        Easing curve for a circular (sqrt(1-t^2)) function:
                        acceleration until halfway, then deceleration.
    \value OutInCirc    \inlineimage qeasingcurve-outincirc.png
                        \br
                        Easing curve for a circular (sqrt(1-t^2)) function:
                        deceleration until halfway, then acceleration.
    \value InElastic    \inlineimage qeasingcurve-inelastic.png
                        \br
                        Easing curve for an elastic
                        (exponentially decaying sine wave) function:
                        accelerating from zero velocity.  The peak amplitude
                        can be set with the \e amplitude parameter, and the
                        period of decay by the \e period parameter.
    \value OutElastic   \inlineimage qeasingcurve-outelastic.png
                        \br
                        Easing curve for an elastic
                        (exponentially decaying sine wave) function:
                        decelerating from zero velocity.  The peak amplitude
                        can be set with the \e amplitude parameter, and the
                        period of decay by the \e period parameter.
    \value InOutElastic \inlineimage qeasingcurve-inoutelastic.png
                        \br
                        Easing curve for an elastic
                        (exponentially decaying sine wave) function:
                        acceleration until halfway, then deceleration.
    \value OutInElastic \inlineimage qeasingcurve-outinelastic.png
                        \br
                        Easing curve for an elastic
                        (exponentially decaying sine wave) function:
                        deceleration until halfway, then acceleration.
    \value InBack       \inlineimage qeasingcurve-inback.png
                        \br
                        Easing curve for a back (overshooting
                        cubic function: (s+1)*t^3 - s*t^2) easing in:
                        accelerating from zero velocity.
    \value OutBack      \inlineimage qeasingcurve-outback.png
                        \br
                        Easing curve for a back (overshooting
                        cubic function: (s+1)*t^3 - s*t^2) easing out:
                        decelerating to zero velocity.
    \value InOutBack    \inlineimage qeasingcurve-inoutback.png
                        \br
                        Easing curve for a back (overshooting
                        cubic function: (s+1)*t^3 - s*t^2) easing in/out:
                        acceleration until halfway, then deceleration.
    \value OutInBack    \inlineimage qeasingcurve-outinback.png
                        \br
                        Easing curve for a back (overshooting
                        cubic easing: (s+1)*t^3 - s*t^2) easing out/in:
                        deceleration until halfway, then acceleration.
    \value InBounce     \inlineimage qeasingcurve-inbounce.png
                        \br
                        Easing curve for a bounce (exponentially
                        decaying parabolic bounce) function: accelerating
                        from zero velocity.
    \value OutBounce    \inlineimage qeasingcurve-outbounce.png
                        \br
                        Easing curve for a bounce (exponentially
                        decaying parabolic bounce) function: decelerating
                        from zero velocity.
    \value InOutBounce  \inlineimage qeasingcurve-inoutbounce.png
                        \br
                        Easing curve for a bounce (exponentially
                        decaying parabolic bounce) function easing in/out:
                        acceleration until halfway, then deceleration.
    \value OutInBounce  \inlineimage qeasingcurve-outinbounce.png
                        \br
                        Easing curve for a bounce (exponentially
                        decaying parabolic bounce) function easing out/in:
                        deceleration until halfway, then acceleration.
    \omitvalue InCurve
    \omitvalue OutCurve
    \omitvalue SineCurve
    \omitvalue CosineCurve
    \value Custom       This is returned if the user specified a custom curve type with
                        setCustomType(). Note that you cannot call setType() with this value,
                        but type() can return it.
    \omitvalue NCurveTypes
*/

/*!
    \typedef QEasingCurve::EasingFunction

    This is a typedef for a pointer to a function with the following
    signature:

    \snippet doc/src/snippets/code/src_corelib_tools_qeasingcurve.cpp 0
*/

#include "qeasingcurve.h"

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#endif

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif

#include "../../3rdparty/easing/easing.cpp"

QT_BEGIN_NAMESPACE

class QEasingCurvePrivate
{
public:
    QEasingCurvePrivate()
        : type(QEasingCurve::Linear),
          func(nullptr),
          per(0.3), amp(1.0), over(1.70158)
    { }
    QEasingCurvePrivate(const QEasingCurvePrivate &other)
        : type(other.type),
          func(other.func),
          per(other.per), amp(other.amp), over(other.over)
    { }

    QEasingCurve::Type type;
    QEasingCurve::EasingFunction func;
    qreal per;
    qreal amp;
    qreal over;
};

/*!
    Constructs an easing curve of the given \a type.
*/
QEasingCurve::QEasingCurve(Type type)
    : d_ptr(new QEasingCurvePrivate())
{
    setType(type);
}

/*!
    Construct a copy of \a other.
 */
QEasingCurve::QEasingCurve(const QEasingCurve &other)
    : d_ptr(new QEasingCurvePrivate(*other.d_ptr))
{
}

/*!
    Destructor.
*/
QEasingCurve::~QEasingCurve()
{
    delete d_ptr;
}

/*!
    Copy \a other.
*/
QEasingCurve &QEasingCurve::operator=(const QEasingCurve &other)
{
    if (*this != other) {
        d_ptr->type = other.d_ptr->type;
        d_ptr->func = other.d_ptr->func;
        d_ptr->per = other.d_ptr->per;
        d_ptr->amp = other.d_ptr->amp;
        d_ptr->over = other.d_ptr->over;
    }
    return *this;
}

/*!
    Compare this easing curve with \a other and returns true if they are
    equal. It will also compare the properties of a curve.
*/
bool QEasingCurve::operator==(const QEasingCurve &other) const
{
    bool res = d_ptr->type == other.d_ptr->type
            && d_ptr->func == other.d_ptr->func;
    if (res) {
        res = qFuzzyCompare(period(), other.period()) &&
              qFuzzyCompare(amplitude(), other.amplitude()) &&
              qFuzzyCompare(overshoot(), other.overshoot());
    }
    return res;
}

/*!
    \fn bool QEasingCurve::operator!=(const QEasingCurve &other) const
    Compare this easing curve with \a other and returns true if they are not equal.
    It will also compare the properties of a curve.

    \sa operator==()
*/

/*!
    Returns the amplitude. This is not applicable for all curve types.
    It is only applicable for bounce and elastic curves (curves of type()
    QEasingCurve::InBounce, QEasingCurve::OutBounce, QEasingCurve::InOutBounce,
    QEasingCurve::OutInBounce, QEasingCurve::InElastic, QEasingCurve::OutElastic,
    QEasingCurve::InOutElastic or QEasingCurve::OutInElastic).
*/
qreal QEasingCurve::amplitude() const
{
    return d_ptr->amp;
}

/*!
    Sets the amplitude to \a amplitude.

    This will set the amplitude of the bounce or the amplitude of the
    elastic "spring" effect. The higher the number, the higher the amplitude.
    \sa amplitude()
*/
void QEasingCurve::setAmplitude(qreal amplitude)
{
    d_ptr->amp = amplitude;
}

/*!
    Returns the period. This is not applicable for all curve types.
    It is only applicable if type() is QEasingCurve::InElastic, QEasingCurve::OutElastic,
    QEasingCurve::InOutElastic or QEasingCurve::OutInElastic.
 */
qreal QEasingCurve::period() const
{
    return d_ptr->per;
}

/*!
    Sets the period to \a period.
    Setting a small period value will give a high frequency of the curve. A
    large period will give it a small frequency.

    \sa period()
*/
void QEasingCurve::setPeriod(qreal period)
{
    d_ptr->per = period;
}

/*!
    Returns the overshoot. This is not applicable for all curve types.
    It is only applicable if type() is QEasingCurve::InBack, QEasingCurve::OutBack,
    QEasingCurve::InOutBack or QEasingCurve::OutInBack.
*/
qreal QEasingCurve::overshoot() const
{
    return d_ptr->over;
}

/*!
    Sets the overshoot to \a overshoot.

    0 produces no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent.

    \sa overshoot()
*/
void QEasingCurve::setOvershoot(qreal overshoot)
{
    d_ptr->over = overshoot;
}

/*!
    Returns the type of the easing curve.
*/
QEasingCurve::Type QEasingCurve::type() const
{
    return d_ptr->type;
}

/*!
    Sets the type of the easing curve to \a type.
*/
void QEasingCurve::setType(Type type)
{
    if (d_ptr->type == type)
        return;
    if (Q_UNLIKELY(type < Linear || type >= QEasingCurve::NCurveTypes - 1)) {
        qWarning("QEasingCurve: Invalid curve type %d", type);
        return;
    }

    d_ptr->type = type;
    d_ptr->func = nullptr;
}

/*!
    Sets a custom easing curve that is defined by the user in the function \a func.
    The signature of the function is qreal myEasingFunction(qreal progress),
    where \e progress and the return value is considered to be normalized between 0 and 1.
    (In some cases the return value can be outside that range)
    After calling this function type() will return QEasingCurve::Custom.
    \a func cannot be zero.

    \sa customType()
    \sa valueForProgress()
*/
void QEasingCurve::setCustomType(EasingFunction func)
{
    if (Q_UNLIKELY(!func)) {
        qWarning("QEasingCurve: Function pointer must not be null");
        return;
    }
    d_ptr->type = QEasingCurve::Custom;
    d_ptr->func = func;
}

/*!
    Returns the function pointer to the custom easing curve.
    If type() does not return QEasingCurve::Custom, this function
    will return 0.
*/
QEasingCurve::EasingFunction QEasingCurve::customType() const
{
    return d_ptr->type == QEasingCurve::Custom ? d_ptr->func : nullptr;
}

/*!
    Return the effective progress for the easing curve at \a progress.
    While  \a progress must be between 0 and 1, the returned effective progress
    can be outside those bounds. For instance, QEasingCurve::InBack will
    return negative values in the beginning of the function.
*/
qreal QEasingCurve::valueForProgress(qreal progress) const
{
    progress = qBound<qreal>(0, progress, 1);

    if (d_ptr->func) {
        return d_ptr->func(progress);
    }

    switch (d_ptr->type) {
        case QEasingCurve::Linear: {
            return easeNone(progress);
        }
        case QEasingCurve::InQuad: {
            return easeInQuad(progress);
        }
        case QEasingCurve::OutQuad: {
            return easeOutQuad(progress);
        }
        case QEasingCurve::InOutQuad: {
            return easeInOutQuad(progress);
        }
        case QEasingCurve::OutInQuad: {
            return easeOutInQuad(progress);
        }
        case QEasingCurve::InCubic: {
            return easeInCubic(progress);
        }
        case QEasingCurve::OutCubic: {
            return easeOutCubic(progress);
        }
        case QEasingCurve::InOutCubic: {
            return easeInOutCubic(progress);
        }
        case QEasingCurve::OutInCubic: {
            return easeOutInCubic(progress);
        }
        case QEasingCurve::InQuart: {
            return easeInQuart(progress);
        }
        case QEasingCurve::OutQuart: {
            return easeOutQuart(progress);
        }
        case QEasingCurve::InOutQuart: {
            return easeInOutQuart(progress);
        }
        case QEasingCurve::OutInQuart: {
            return easeOutInQuart(progress);
        }
        case QEasingCurve::InQuint: {
            return easeInQuint(progress);
        }
        case QEasingCurve::OutQuint: {
            return easeOutQuint(progress);
        }
        case QEasingCurve::InOutQuint: {
            return easeInOutQuint(progress);
        }
        case QEasingCurve::OutInQuint: {
            return easeOutInQuint(progress);
        }
        case QEasingCurve::InSine: {
            return easeInSine(progress);
        }
        case QEasingCurve::OutSine: {
            return easeOutSine(progress);
        }
        case QEasingCurve::InOutSine: {
            return easeInOutSine(progress);
        }
        case QEasingCurve::OutInSine: {
            return easeOutInSine(progress);
        }
        case QEasingCurve::InExpo: {
            return easeInExpo(progress);
        }
        case QEasingCurve::OutExpo: {
            return easeOutExpo(progress);
        }
        case QEasingCurve::InOutExpo: {
            return easeInOutExpo(progress);
        }
        case QEasingCurve::OutInExpo: {
            return easeOutInExpo(progress);
        }
        case QEasingCurve::InCirc: {
            return easeInCirc(progress);
        }
        case QEasingCurve::OutCirc: {
            return easeOutCirc(progress);
        }
        case QEasingCurve::InOutCirc: {
            return easeInOutCirc(progress);
        }
        case QEasingCurve::OutInCirc: {
            return easeOutInCirc(progress);
        }
        // Internal for, compatibility with QTimeLine only ??
        case QEasingCurve::InCurve: {
            return easeInCurve(progress);
        }
        case QEasingCurve::OutCurve: {
            return easeOutCurve(progress);
        }
        case QEasingCurve::SineCurve: {
            return easeSineCurve(progress);
        }
        case QEasingCurve::CosineCurve: {
            return easeCosineCurve(progress);
        }
        default: {
            break;
        }
    }

    qreal p = (d_ptr->per < 0) ? qreal(0.3) : d_ptr->per;
    qreal a = (d_ptr->amp < 0) ? qreal(1.0) : d_ptr->amp;
    qreal o = (d_ptr->over < 0) ? qreal(1.70158) : d_ptr->over;
    switch (d_ptr->type) {
        case QEasingCurve::InElastic: {
            return easeInElastic(progress, a, p);
        }
        case QEasingCurve::OutElastic: {
            return easeOutElastic(progress, a, p);
        }
        case QEasingCurve::InOutElastic: {
            return easeInOutElastic(progress, a, p);
        }
        case QEasingCurve::OutInElastic: {
            return easeOutInElastic(progress, a, p);
        }
        case QEasingCurve::InBounce: {
            return easeInBounce(progress, a);
        }
        case QEasingCurve::OutBounce: {
            return easeOutBounce(progress, a);
        }
        case QEasingCurve::InOutBounce: {
            return easeInOutBounce(progress, a);
        }
        case QEasingCurve::OutInBounce: {
            return easeOutInBounce(progress, a);
        }
        case QEasingCurve::InBack: {
            return easeInBack(progress, o);
        }
        case QEasingCurve::OutBack: {
            return easeOutBack(progress, o);
        }
        case QEasingCurve::InOutBack: {
            return easeInOutBack(progress, o);
        }
        case QEasingCurve::OutInBack: {
            return easeOutInBack(progress, o);
        }
        default: {
            break;
        }
    }

    return progress;
}

#ifndef QT_NO_DEBUG_STREAM
static const char* const easingTypeName(const QEasingCurve::Type type)
{
    switch (type) {
        case QEasingCurve::Linear: {
            return "Linear";
        }
        case QEasingCurve::InQuad: {
            return "InQuad";
        }
        case QEasingCurve::OutQuad: {
            return "OutQuad";
        }
        case QEasingCurve::InOutQuad: {
            return "InOutQuad";
        }
        case QEasingCurve::OutInQuad: {
            return "OutInQuad";
        }
        case QEasingCurve::InCubic: {
            return "InCubic";
        }
        case QEasingCurve::OutCubic: {
            return "OutCubic";
        }
        case QEasingCurve::InOutCubic: {
            return "InOutCubic";
        }
        case QEasingCurve::OutInCubic: {
            return "OutInCubic";
        }
        case QEasingCurve::InQuart: {
            return "InQuart";
        }
        case QEasingCurve::OutQuart: {
            return "OutQuart";
        }
        case QEasingCurve::InOutQuart: {
            return "InOutQuart";
        }
        case QEasingCurve::OutInQuart: {
            return "OutInQuart";
        }
        case QEasingCurve::InQuint: {
            return "InQuint";
        }
        case QEasingCurve::OutQuint: {
            return "OutQuint";
        }
        case QEasingCurve::InOutQuint: {
            return "InOutQuint";
        }
        case QEasingCurve::OutInQuint: {
            return "OutInQuint";
        }
        case QEasingCurve::InSine: {
            return "InSine";
        }
        case QEasingCurve::OutSine: {
            return "OutSine";
        }
        case QEasingCurve::InOutSine: {
            return "InOutSine";
        }
        case QEasingCurve::OutInSine: {
            return "OutInSine";
        }
        case QEasingCurve::InExpo: {
            return "InExpo";
        }
        case QEasingCurve::OutExpo: {
            return "OutExpo";
        }
        case QEasingCurve::InOutExpo: {
            return "InOutExpo";
        }
        case QEasingCurve::OutInExpo: {
            return "OutInExpo";
        }
        case QEasingCurve::InCirc: {
            return "InCirc";
        }
        case QEasingCurve::OutCirc: {
            return "OutCirc";
        }
        case QEasingCurve::InOutCirc: {
            return "InOutCirc";
        }
        case QEasingCurve::OutInCirc: {
            return "OutInCirc";
        }
        case QEasingCurve::InCurve: {
            return "InCurve";
        }
        case QEasingCurve::OutCurve: {
            return "OutCurve";
        }
        case QEasingCurve::SineCurve: {
            return "SineCurve";
        }
        case QEasingCurve::CosineCurve: {
            return "CosineCurve";
        }
        case QEasingCurve::InElastic: {
            return "InElastic";
        }
        case QEasingCurve::OutElastic: {
            return "OutElastic";
        }
        case QEasingCurve::InOutElastic: {
            return "InOutElastic";
        }
        case QEasingCurve::OutInElastic: {
            return "OutInElastic";
        }
        case QEasingCurve::InBounce: {
            return "InBounce";
        }
        case QEasingCurve::OutBounce: {
            return "OutBounce";
        }
        case QEasingCurve::InOutBounce: {
            return "InOutBounce";
        }
        case QEasingCurve::OutInBounce: {
            return "OutInBounce";
        }
        case QEasingCurve::InBack: {
            return "InBack";
        }
        case QEasingCurve::OutBack: {
            return "OutBack";
        }
        case QEasingCurve::InOutBack: {
            return "InOutBack";
        }
        case QEasingCurve::OutInBack: {
            return "OutInBack";
        }
        case QEasingCurve::Custom: {
            return "Custom";
        }
        default: {
            Q_ASSERT(false);
            return "";
        }
    }
    Q_UNREACHABLE();
}

QDebug operator<<(QDebug debug, const QEasingCurve &easing)
{
#ifndef Q_BROKEN_DEBUG_STREAM
    debug.nospace() << "QEasingCurve("
        << easingTypeName(easing.type()) << ", "
        << easing.period() << ", "
        << easing.amplitude() << ", "
        << easing.overshoot();
    debug.nospace() << ')';
    return debug.space();
#else
    qWarning("This compiler doesn't support streaming QEasingCurve to QDebug");
    return debug;
    Q_UNUSED(v);
#endif
}
#endif // QT_NO_DEBUG_STREAM

#ifndef QT_NO_DATASTREAM
/*!
    \relates QEasingCurve

    Writes the given \a easing curve to the given \a stream and returns a
    reference to the stream.

    \sa {Serializing Katie Data Types}
*/
QDataStream &operator<<(QDataStream &stream, const QEasingCurve &easing)
{
    stream << quint8(easing.d_ptr->type);
    stream << quint64(quintptr(easing.d_ptr->func));
    stream << easing.d_ptr->per;
    stream << easing.d_ptr->amp;
    stream << easing.d_ptr->over;
    return stream;
}

/*!
    \relates QEasingCurve

    Reads an easing curve from the given \a stream into the given \a
    easing curve and returns a reference to the stream.

    \sa {Serializing Katie Data Types}
*/
QDataStream &operator>>(QDataStream &stream, QEasingCurve &easing)
{
    quint8 int_type;
    stream >> int_type;
    easing.setType(static_cast<QEasingCurve::Type>(int_type));

    quint64 ptr_func;
    stream >> ptr_func;
    easing.d_ptr->func = QEasingCurve::EasingFunction(quintptr(ptr_func));

    stream >> easing.d_ptr->per;
    stream >> easing.d_ptr->amp;
    stream >> easing.d_ptr->over;
    return stream;
}
#endif // QT_NO_DATASTREAM

QT_END_NAMESPACE

#include "moc_qeasingcurve.h"

