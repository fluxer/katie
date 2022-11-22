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

#include "qapplication.h"
#ifndef QT_NO_EFFECTS
#include "qdesktopwidget.h"
#include "qeffects_p.h"
#include "qevent.h"
#include "qimage.h"
#include "qpainter.h"
#include "qpixmap.h"
#include "qpointer.h"
#include "qtimer.h"
#include "qelapsedtimer.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

static const double s_duration = 150.0;

/*!
    \internal

    The QOpacityEffect object sets the widget opacity
*/
class QOpacityEffect: public QObject
{
    Q_OBJECT
public:
    QOpacityEffect(QWidget* w);
    ~QOpacityEffect();

private slots:
    void fade();
    void cancel();

private:
    QPointer<QWidget> widget;
    QTimer anim;
    QElapsedTimer checkTime;
};

static QOpacityEffect* q_opacity = nullptr;

QOpacityEffect::QOpacityEffect(QWidget* w)
    : QObject(w),
    widget(w)
{
    connect(widget, SIGNAL(destroyed()), this, SLOT(cancel()));

    checkTime.start();
    widget->setWindowOpacity(0.0);
    widget->show();
    connect(&anim, SIGNAL(timeout()), this, SLOT(fade()));
    anim.start(1);
}

QOpacityEffect::~QOpacityEffect()
{
    // Restore the opacity value
    if (widget) {
        widget->setWindowOpacity(1);
    }
}

/*
    Sets the widget opacity for the time elapsed
*/
void QOpacityEffect::fade()
{
    const double alpha = (double(checkTime.elapsed()) / s_duration);

    if (alpha >= 1.0 || !widget) {
        anim.stop();
        if (widget) {
            widget->setWindowOpacity(1.0);
        }
        q_opacity = nullptr;
        deleteLater();
    } else {
        widget->setWindowOpacity(alpha);
    }
}

void QOpacityEffect::cancel()
{
    anim.stop();
    q_opacity = nullptr;
    deleteLater();
}

/*!
    Fade in widget \a w.
*/
void qFadeEffect(QWidget* w)
{
    if (q_opacity) {
        q_opacity = nullptr;
        q_opacity->deleteLater();
    }

    if (!w) {
        return;
    }

    q_opacity = new QOpacityEffect(w);
}

QT_END_NAMESPACE

#include "moc_qeffects.cpp"

#endif //QT_NO_EFFECTS
