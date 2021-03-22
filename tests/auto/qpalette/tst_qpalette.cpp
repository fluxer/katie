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

#include "qpalette.h"
#include <qapplication.h>

//TESTED_CLASS=QPalette
//TESTED_FILES=qpalette.h qpalette.cpp

class tst_QPalette : public QObject
{
Q_OBJECT

public:
    tst_QPalette();
    virtual ~tst_QPalette();

private slots:
    void roleValues_data();
    void roleValues();
};

tst_QPalette::tst_QPalette()
{
}

tst_QPalette::~tst_QPalette()
{
}

void tst_QPalette::roleValues_data()
{
    QTest::addColumn<int>("role");
    QTest::addColumn<int>("value");

    QTest::newRow("QPalette::WindowText") << int(QPalette::WindowText) << 0;
    QTest::newRow("QPalette::Button") << int(QPalette::Button) << 1;
    QTest::newRow("QPalette::Light") << int(QPalette::Light) << 2;
    QTest::newRow("QPalette::Midlight") << int(QPalette::Midlight) << 3;
    QTest::newRow("QPalette::Dark") << int(QPalette::Dark) << 4;
    QTest::newRow("QPalette::Mid") << int(QPalette::Mid) << 5;
    QTest::newRow("QPalette::Text") << int(QPalette::Text) << 6;
    QTest::newRow("QPalette::BrightText") << int(QPalette::BrightText) << 7;
    QTest::newRow("QPalette::ButtonText") << int(QPalette::ButtonText) << 8;
    QTest::newRow("QPalette::Base") << int(QPalette::Base) << 9;
    QTest::newRow("QPalette::Window") << int(QPalette::Window) << 10;
    QTest::newRow("QPalette::Shadow") << int(QPalette::Shadow) << 11;
    QTest::newRow("QPalette::Highlight") << int(QPalette::Highlight) << 12;
    QTest::newRow("QPalette::HighlightedText") << int(QPalette::HighlightedText) << 13;
    QTest::newRow("QPalette::Link") << int(QPalette::Link) << 14;
    QTest::newRow("QPalette::LinkVisited") << int(QPalette::LinkVisited) << 15;
    QTest::newRow("QPalette::AlternateBase") << int(QPalette::AlternateBase) << 16;
    QTest::newRow("QPalette::NoRole") << int(QPalette::NoRole) << 17;
    QTest::newRow("QPalette::ToolTipBase") << int(QPalette::ToolTipBase) << 18;
    QTest::newRow("QPalette::ToolTipText") << int(QPalette::ToolTipText) << 19;

    // Change this value as you add more roles.
    QTest::newRow("QPalette::NColorRoles") << int(QPalette::NColorRoles) << 20;
}

void tst_QPalette::roleValues()
{
    QFETCH(int, role);
    QFETCH(int, value);
    QCOMPARE(role, value);
}

QTEST_MAIN(tst_QPalette)

#include "moc_tst_qpalette.cpp"
