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
#include <qmargins.h>

#include <limits.h>

Q_DECLARE_METATYPE(QMargins)

//TESTED_CLASS=
//TESTED_FILES=

class tst_QMargins : public QObject
{
    Q_OBJECT

public:
    tst_QMargins();
    virtual ~tst_QMargins();


public slots:
    void init();
    void cleanup();
private slots:
    void getSetCheck();
};

// Testing get/set functions
void tst_QMargins::getSetCheck()
{
    QMargins margins;
    // int QMargins::width()
    // void QMargins::setWidth(int)
    margins.setLeft(0);
    QCOMPARE(0, margins.left());
    margins.setTop(INT_MIN);
    QCOMPARE(INT_MIN, margins.top());
    margins.setBottom(INT_MAX);
    QCOMPARE(INT_MAX, margins.bottom());
    margins.setRight(INT_MAX);
    QCOMPARE(INT_MAX, margins.right());

    margins = QMargins();
    QVERIFY(margins.isNull());
    margins.setLeft(5);
    margins.setRight(5);
    QVERIFY(!margins.isNull());
    QCOMPARE(margins, QMargins(5, 0, 5, 0));
} 

tst_QMargins::tst_QMargins()
{
}

tst_QMargins::~tst_QMargins()
{
}

void tst_QMargins::init()
{
}

void tst_QMargins::cleanup()
{
}

QTEST_APPLESS_MAIN(tst_QMargins)

#include "moc_tst_qmargins.cpp"
