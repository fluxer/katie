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

#include <qcoreapplication.h>
#include <qdebug.h>
#include <qscrollarea.h>
#include <qlayout.h>

// TESTED_CLASS=
// TESTED_FILES=

class tst_QScrollArea : public QObject
{
Q_OBJECT

public:
    tst_QScrollArea();
    virtual ~tst_QScrollArea();

private slots:
    void getSetCheck();
    void checkHFW_Task_197736();
};

tst_QScrollArea::tst_QScrollArea()
{
}

tst_QScrollArea::~tst_QScrollArea()
{
}

// Testing get/set functions
void tst_QScrollArea::getSetCheck()
{
    QScrollArea obj1;
    // QWidget * QScrollArea::widget()
    // void QScrollArea::setWidget(QWidget *)
    QWidget *var1 = new QWidget();
    obj1.setWidget(var1);
    QCOMPARE(var1, obj1.widget());
    obj1.setWidget((QWidget *)0);
    QCOMPARE(var1, obj1.widget()); // Cannot set a 0-widget. Old widget returned
    // delete var1; // No delete, since QScrollArea takes ownership

    // bool QScrollArea::widgetResizable()
    // void QScrollArea::setWidgetResizable(bool)
    obj1.setWidgetResizable(false);
    QCOMPARE(false, obj1.widgetResizable());
    obj1.setWidgetResizable(true);
    QCOMPARE(true, obj1.widgetResizable());
}

class HFWWidget : public QWidget
{
    public:
        HFWWidget();
        int heightForWidth(int w) const;
};

HFWWidget::HFWWidget()
    : QWidget()
{
    setMinimumSize(QSize(100,50));
    QSizePolicy s = sizePolicy();
    s.setHeightForWidth(true);
    setSizePolicy(s);
}

int HFWWidget::heightForWidth(int w) const
{
    // Mimic a label - the narrower we are, the taller we have to be
    if (w > 0)
        return 40000 / w;
    else
        return 40000;
}

void tst_QScrollArea::checkHFW_Task_197736()
{
    QScrollArea scrollArea;
    HFWWidget *w = new HFWWidget;
    scrollArea.resize(200,100);
    scrollArea.show();
    scrollArea.setWidgetResizable(true);
    scrollArea.setWidget(w);

    // at 200x100px, we expect HFW to be 200px tall, not 100px
    QVERIFY(w->height() >= 200);

    // at 200x300px, we expect HFW to be 300px tall (the heightForWidth is a min, not prescribed)
    scrollArea.resize(QSize(200, 300));
    QVERIFY(w->height() >= 250); // 50px for a fudge factor (size of frame margins/scrollbars etc)

    // make sure this only happens with widget resizable
    scrollArea.setWidgetResizable(false);
    scrollArea.resize(QSize(100,100));
    w->resize(QSize(200,200));
    QVERIFY(w->width() == 200);
    QVERIFY(w->height() == 200);
}

QTEST_MAIN(tst_QScrollArea)

#include "moc_tst_qscrollarea.cpp"
