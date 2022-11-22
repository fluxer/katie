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

/*!
    \internal

    The QOpacityEffect object sets the widget opacity
*/
class QOpacityEffect: public QObject, private QEffects
{
    Q_OBJECT
public:
    QOpacityEffect(QWidget* w);
    ~QOpacityEffect();

    void run();

private slots:
    void fade();
    void cancel();

private:
    double alpha;
    QPointer<QWidget> widget;
    int duration;
    int elapsed;
    QTimer anim;
    QElapsedTimer checkTime;
};

static QOpacityEffect* q_opacity = nullptr;

QOpacityEffect::QOpacityEffect(QWidget* w)
    : QObject(w),
    alpha(0.0),
    widget(w),
    duration(150),
    elapsed(0)
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
    int tempel = checkTime.elapsed();
    if (elapsed >= tempel) {
        elapsed++;
    } else {
        elapsed = tempel;
    }

    if (duration != 0) {
        alpha = tempel / double(duration);
    } else {
        alpha = 1.0;
    }

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
    \internal

    The QRollEffect widget is shown while the animation lasts and displays a
    scrolling pixmap
*/
class QRollEffect : public QWidget, private QEffects
{
    Q_OBJECT
public:
    QRollEffect(QWidget* w, Qt::WindowFlags f, DirFlags orient);

    void run();

protected:
    void paintEvent(QPaintEvent*);
    void closeEvent(QCloseEvent*);

private slots:
    void scroll();

private:
    QPointer<QWidget> widget;

    int currentHeight;
    int currentWidth;
    int totalHeight;
    int totalWidth;

    int duration;
    int elapsed;
    bool done;
    bool showWidget;
    int orientation;

    QTimer anim;
    QElapsedTimer checkTime;

    QPixmap pm;
};

static QRollEffect* q_roll = nullptr;

QRollEffect::QRollEffect(QWidget* w, Qt::WindowFlags f, DirFlags orient)
    : QWidget(0, f),
    widget(w),
    elapsed(0),
    done(false),
    orientation(orient)
{
    Q_ASSERT(widget);

    setEnabled(false);
    setAttribute(Qt::WA_NoSystemBackground, true);

    if (widget->testAttribute(Qt::WA_Resized)) {
        totalWidth = widget->width();
        totalHeight = widget->height();
    } else {
        totalWidth = widget->sizeHint().width();
        totalHeight = widget->sizeHint().height();
    }

    currentHeight = totalHeight;
    currentWidth = totalWidth;

    if (orientation & (RightScroll|LeftScroll)) {
        currentWidth = 0;
    }
    if (orientation & (DownScroll|UpScroll)) {
        currentHeight = 0;
    }

    pm = QPixmap::grabWidget(widget);

    int dist = 0;
    if (orientation & (RightScroll|LeftScroll)) {
        dist += totalWidth - currentWidth;
    }
    if (orientation & (DownScroll|UpScroll)) {
        dist += totalHeight - currentHeight;
    }
    duration = qMin(qMax(dist/3, 50), 120);

    connect(&anim, SIGNAL(timeout()), this, SLOT(scroll()));

    move(widget->geometry().x(),widget->geometry().y());
    resize(qMin(currentWidth, totalWidth), qMin(currentHeight, totalHeight));

    // This is roughly equivalent to calling setVisible(true) without actually showing the widget
    widget->setAttribute(Qt::WA_WState_ExplicitShowHide, true);
    widget->setAttribute(Qt::WA_WState_Hidden, false);

    show();
    setEnabled(false);

    qApp->installEventFilter(this);

    showWidget = true;
    anim.start(1);
    checkTime.start();
}

/*!
    \reimp
*/
void QRollEffect::paintEvent(QPaintEvent*)
{
    int x = orientation & RightScroll ? qMin(0, currentWidth - totalWidth) : 0;
    int y = orientation & DownScroll ? qMin(0, currentHeight - totalHeight) : 0;

    QPainter p(this);
    p.drawPixmap(x, y, pm);
}

/*!
    \reimp
*/
void QRollEffect::closeEvent(QCloseEvent *e)
{
    e->accept();
    if (done) {
        return;
    }

    showWidget = false;
    done = true;
    scroll();

    QWidget::closeEvent(e);
}

/*
    Roll according to the time elapsed.
*/
void QRollEffect::scroll()
{
    if (!done && widget) {
        int tempel = checkTime.elapsed();
        if (elapsed >= tempel) {
            elapsed++;
        } else {
            elapsed = tempel;
        }

        if (currentWidth != totalWidth) {
            currentWidth = totalWidth * (elapsed/duration)
                + (2 * totalWidth * (elapsed%duration) + duration)
                / (2 * duration);
            // equiv. to int((totalWidth*elapsed) / duration + 0.5)
            done = (currentWidth >= totalWidth);
        }
        if (currentHeight != totalHeight) {
            currentHeight = totalHeight * (elapsed/duration)
                + (2 * totalHeight * (elapsed%duration) + duration)
                / (2 * duration);
            // equiv. to int((totalHeight*elapsed) / duration + 0.5)
            done = (currentHeight >= totalHeight);
        }
        done = (currentHeight >= totalHeight) &&
               (currentWidth >= totalWidth);

        int w = totalWidth;
        int h = totalHeight;
        int x = widget->geometry().x();
        int y = widget->geometry().y();

        if (orientation & RightScroll || orientation & LeftScroll)
            w = qMin(currentWidth, totalWidth);
        if (orientation & DownScroll || orientation & UpScroll)
            h = qMin(currentHeight, totalHeight);

        setUpdatesEnabled(false);
        if (orientation & UpScroll) {
            y = widget->geometry().y() + qMax(0, totalHeight - currentHeight);
        }
        if (orientation & LeftScroll) {
            x = widget->geometry().x() + qMax(0, totalWidth - currentWidth);
        }
        if (orientation & UpScroll || orientation & LeftScroll) {
            move(x, y);
        }

        resize(w, h);
        setUpdatesEnabled(true);
        repaint();
    }
    if (done || !widget) {
        anim.stop();
        qApp->removeEventFilter(this);
        if (widget) {
            if (!showWidget) {
                widget->hide();
            } else {
                // Since we are faking the visibility of the widget
                // we need to unset the hidden state on it before calling show
                widget->setAttribute(Qt::WA_WState_Hidden, true);
                widget->show();
                lower();
            }
        }
        q_roll = nullptr;
        deleteLater();
    }
}

/*!
    Scroll widget \a w. \a orient may be 1 (vertical), 2 (horizontal) or 3 (diagonal).
*/
void qScrollEffect(QWidget* w, QEffects::DirFlags orient)
{
    if (q_roll) {
        q_roll->deleteLater();
        q_roll = nullptr;
    }

    if (!w) {
        return;
    }

    QApplication::sendPostedEvents(w, QEvent::Move);
    QApplication::sendPostedEvents(w, QEvent::Resize);

    // those can be popups - they would steal the focus, but are disabled
    q_roll = new QRollEffect(w, Qt::ToolTip, orient);
}

/*!
    Fade in widget \a w.
*/
void qFadeEffect(QWidget* w)
{
    if (q_opacity) {
        q_opacity->deleteLater();
        q_opacity = nullptr;
    }

    if (!w) {
        return;
    }

    QApplication::sendPostedEvents(w, QEvent::Move);
    QApplication::sendPostedEvents(w, QEvent::Resize);

    // those can be popups - they would steal the focus, but are disabled
    q_opacity = new QOpacityEffect(w);
}

QT_END_NAMESPACE

#include "moc_qeffects.cpp"

#endif //QT_NO_EFFECTS
