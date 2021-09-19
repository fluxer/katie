/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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

#include <QtTest/QtTest>
#include <qmath.h>

static const qreal PI = 3.14159265358979323846264338327950288;

class tst_QMath : public QObject
{
    Q_OBJECT
private slots:
    void sinCos();
};

void tst_QMath::sinCos()
{
    // Test evenly spaced angles from 0 to 2pi radians.
    const int LOOP_COUNT = 100000;
    for (int i = 0; i < LOOP_COUNT; ++i) {
        qreal angle = i * 2 * PI / (LOOP_COUNT - 1);
        QVERIFY(qAbs(qSin(angle) - qSin(angle)) < 1e-5);
        QVERIFY(qAbs(qCos(angle) - qCos(angle)) < 1e-5);
    }
}

QTEST_APPLESS_MAIN(tst_QMath)

#include "moc_tst_qmath.cpp"
