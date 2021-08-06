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

#ifndef CHIPTESTER_H
#define CHIPTESTER_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QEventLoop>
#include <QTime>

class ChipTester : public QGraphicsView
{
    Q_OBJECT
public:
    enum Operation {
        Rotate360,
        ZoomInOut,
        Translate
    };
    ChipTester(QWidget *parent = nullptr);

    void setAntialias(bool enabled);
    void setOpenGL(bool enabled);
    void runBenchmark();
    void setOperation(Operation operation);

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    void populateScene();

    QGraphicsView *view;
    QGraphicsScene *scene;
    int npaints;
    int timerId;
    QEventLoop eventLoop;
    QTime stopWatch;
    Operation operation;
};

#endif
