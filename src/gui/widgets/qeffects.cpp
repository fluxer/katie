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

#include "qeffects_p.h"

#ifndef QT_NO_EFFECTS
#include "qwidget.h"
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
    QPointer<QWidget> m_widget;
    QTimer m_fadetimer;
    QElapsedTimer m_elapsedtimer;
};

static QOpacityEffect* q_opacity = nullptr;

QOpacityEffect::QOpacityEffect(QWidget* w)
    : QObject(w),
    m_widget(w)
{
    connect(m_widget, SIGNAL(destroyed()), this, SLOT(cancel()));

    m_widget->setWindowOpacity(0.0);
    m_widget->show();
    connect(&m_fadetimer, SIGNAL(timeout()), this, SLOT(fade()));
    m_elapsedtimer.start();
    m_fadetimer.start(1);
}

QOpacityEffect::~QOpacityEffect()
{
    // Restore the opacity value
    if (m_widget) {
        m_widget->setWindowOpacity(1);
    }
}

/*
    Sets the widget opacity for the time elapsed
*/
void QOpacityEffect::fade()
{
    const double alpha = (double(m_elapsedtimer.elapsed()) / s_duration);

    if (alpha >= 1.0 || !m_widget) {
        m_fadetimer.stop();
        if (m_widget) {
            m_widget->setWindowOpacity(1.0);
        }
        q_opacity = nullptr;
        deleteLater();
    } else {
        m_widget->setWindowOpacity(alpha);
    }
}

void QOpacityEffect::cancel()
{
    m_fadetimer.stop();
    q_opacity = nullptr;
    deleteLater();
}

/*!
    Fade in widget \a widget, if \a widget is null the effect is canceled.
*/
void qFadeEffect(QWidget* widget)
{
    if (q_opacity) {
        q_opacity = nullptr;
        q_opacity->deleteLater();
    }

    if (!widget) {
        return;
    }

    q_opacity = new QOpacityEffect(widget);
}

QT_END_NAMESPACE

#include "moc_qeffects.cpp"

#endif // QT_NO_EFFECTS
