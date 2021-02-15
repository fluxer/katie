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

#include <qtest.h>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
//TESTED_FILES=

class tst_QGraphicsWidget : public QObject
{
    Q_OBJECT

public:
    tst_QGraphicsWidget();
    virtual ~tst_QGraphicsWidget();

public slots:
    void init();
    void cleanup();

private slots:
    void move();
};

tst_QGraphicsWidget::tst_QGraphicsWidget()
{
}

tst_QGraphicsWidget::~tst_QGraphicsWidget()
{
}

void tst_QGraphicsWidget::init()
{
}

void tst_QGraphicsWidget::cleanup()
{
}

void tst_QGraphicsWidget::move()
{
    QGraphicsScene scene;
    QGraphicsWidget *widget = new QGraphicsWidget();
    scene.addItem(widget);
    QGraphicsView view(&scene);
    view.show();
    QBENCHMARK {
        widget->setPos(qrand(),qrand());
    }
}

QTEST_MAIN(tst_QGraphicsWidget)

#include "moc_tst_qgraphicswidget.cpp"
