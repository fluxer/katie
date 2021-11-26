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
#include <QPainterPathStroker>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QPainterPathStroker : public QObject
{
    Q_OBJECT

public:

private slots:
    void strokeEmptyPath();
};

void tst_QPainterPathStroker::strokeEmptyPath()
{
    QPainterPath path;
    path.moveTo(10, 10);
    path.lineTo(10, 10);
    QPainterPathStroker stroker;
    QCOMPARE(stroker.createStroke(path), path);
}

QTEST_APPLESS_MAIN(tst_QPainterPathStroker)

#include "moc_tst_qpainterpathstroker.cpp"
