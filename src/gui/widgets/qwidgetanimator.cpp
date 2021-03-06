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

#include "qpropertyanimation.h"
#include "qwidget.h"
#include "qmainwindowlayout_p.h"
#include "qwidgetanimator_p.h"

QT_BEGIN_NAMESPACE

QWidgetAnimator::QWidgetAnimator(QMainWindowLayout *layout)
#ifndef QT_NO_MAINWINDOW
    : m_mainWindowLayout(layout)
#endif
{
#ifdef QT_NO_MAINWINDOW
    Q_UNUSED(m_mainWindowLayout);
#endif
}

void QWidgetAnimator::abort(QWidget *w)
{
#ifndef QT_NO_ANIMATION
    QPropertyAnimation *anim = m_animation_map.take(w);
    if (anim) {
        anim->stop();
        anim->deleteLater();
    }
#ifndef QT_NO_MAINWINDOW
    m_mainWindowLayout->animationFinished(w);
#endif
#else
    Q_UNUSED(w); //there is no animation to abort
#endif //QT_NO_ANIMATION
}

#ifndef QT_NO_ANIMATION
void QWidgetAnimator::animationFinished()
{
    QPropertyAnimation *anim = qobject_cast<QPropertyAnimation*>(sender());
    QWidgetAnimator::abort(static_cast<QWidget*>(anim->targetObject()));
}
#endif //QT_NO_ANIMATION

void QWidgetAnimator::animate(QWidget *widget, const QRect &_final_geometry, bool animate)
{
    QRect r = widget->geometry();
    if (r.right() < 0 || r.bottom() < 0)
        r = QRect();

#ifndef QT_NO_ANIMATION
    bool animate_widget = animate && !r.isNull() && !_final_geometry.isNull();
#else
    bool animate_widget = false;
#endif

    // might make the wigdet go away by sending it to negative space
    const QRect final_geometry = _final_geometry.isValid() || widget->isWindow() ? _final_geometry :
        QRect(QPoint(-500 - widget->width(), -500 - widget->height()), widget->size());

    if (!animate_widget) {
        // we do it in one shot
        widget->setGeometry(final_geometry);
#ifndef QT_NO_MAINWINDOW
        m_mainWindowLayout->animationFinished(widget);
#endif // QT_NO_MAINWINDOW
        return;
    }

#ifndef QT_NO_ANIMATION
    QPropertyAnimation* anim = m_animation_map.value(widget);
    if (anim && anim->endValue().toRect() == final_geometry)
        return;

    anim = new QPropertyAnimation(widget, "geometry", widget);
    anim->setDuration(200);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->setEndValue(final_geometry);
    m_animation_map.insert(widget, anim);
    connect(anim, SIGNAL(finished()), this, SLOT(animationFinished()));
    anim->start();
#endif // QT_NO_ANIMATION
}

bool QWidgetAnimator::animating() const
{
    return !m_animation_map.isEmpty();
}

QT_END_NAMESPACE


#include "moc_qwidgetanimator_p.h"
