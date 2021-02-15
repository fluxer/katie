/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtTest module of the Katie Toolkit.
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

#include "qbenchmarkevent_p.h"
#include "qbenchmark_p.h"
#include "qbenchmarkmetric_p.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

QAbstractEventDispatcher::EventFilter oldEventFilter = 0;
qint64 QBenchmarkEvent::eventCounter = 0;

QBenchmarkEvent::~QBenchmarkEvent()
{
}

void QBenchmarkEvent::start()
{
    QBenchmarkEvent::eventCounter = 0;
    QAbstractEventDispatcher *parent = QAbstractEventDispatcher::instance();
    oldEventFilter = parent->setEventFilter(QBenchmarkEvent::eventCountingMechanism);
}

qint64 QBenchmarkEvent::checkpoint()
{
    return QBenchmarkEvent::eventCounter;
}

qint64 QBenchmarkEvent::stop()
{
    QAbstractEventDispatcher *parent = QAbstractEventDispatcher::instance();
    parent->setEventFilter(oldEventFilter);
    return QBenchmarkEvent::eventCounter;
}

// It's very tempting to simply reject a measurement if 0 events
// where counted, however that is a possible situation and returning
// false here will create a infinite loop. Do not change this.
bool QBenchmarkEvent::isMeasurementAccepted(qint64 measurement)
{
    Q_UNUSED(measurement);
    return true;
}

QTest::QBenchmarkMetric QBenchmarkEvent::metricType()
{
    return QTest::Events;
}

// This could be done in a much better way, this is just the beginning.
bool QBenchmarkEvent::eventCountingMechanism(void *message)
{
    Q_UNUSED(message);
    QBenchmarkEvent::eventCounter++;
    return false;
}

QT_END_NAMESPACE
