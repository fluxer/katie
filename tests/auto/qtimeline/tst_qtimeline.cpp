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

#include <qtimeline.h>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QTimeLine : public QObject {
  Q_OBJECT

public:
    tst_QTimeLine();
    virtual ~tst_QTimeLine();

public Q_SLOTS:
    void init();
    void cleanup();

private slots:
    void range();
    void currentTime();
    void duration();
    void frameRate();
    void value();
    void currentFrame();
    void loopCount();
    void interpolation();
    void reverse_data();
    void reverse();
    //void toggle();
    //void reset(); ### todo
    void frameChanged();
    void stopped();
    void finished();
    void isRunning();
    void multipleTimeLines();
    void sineCurve();
    void cosineCurve();
    void outOfRange();
    void stateInFinishedSignal();
    void resume();
    void restart();
    void setPaused();

protected slots:
    void finishedSlot();

protected:
    QTimeLine::State state;
    QTimeLine * view;
};

tst_QTimeLine::tst_QTimeLine()
{
}

tst_QTimeLine::~tst_QTimeLine()
{
}

void tst_QTimeLine::init()
{
}

void tst_QTimeLine::cleanup()
{
}
#include <qdebug.h>

void tst_QTimeLine::range()
{
    QTimeLine timeLine(200);
    QCOMPARE(timeLine.startFrame(), 0);
    QCOMPARE(timeLine.endFrame(), 0);
    timeLine.setFrameRange(0, 1);
    QCOMPARE(timeLine.startFrame(), 0);
    QCOMPARE(timeLine.endFrame(), 1);
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.startFrame(), 10);
    QCOMPARE(timeLine.endFrame(), 20);

    timeLine.setStartFrame(6);
    QCOMPARE(timeLine.startFrame(), 6);
    timeLine.setEndFrame(16);
    QCOMPARE(timeLine.endFrame(), 16);

    // Verify that you can change the range in the timeLine
    timeLine.setFrameRange(10, 20);
    QSignalSpy spy(&timeLine, SIGNAL(frameChanged(int)));
    timeLine.start();
    QTest::qWait(100);
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    int oldValue = timeLine.currentFrame();
    timeLine.setFrameRange(0, 5);
    QVERIFY(timeLine.currentFrame() < oldValue);
    timeLine.setEndFrame(100);
    timeLine.setStartFrame(50);
    QVERIFY(timeLine.currentFrame() > oldValue);
    timeLine.setFrameRange(0, 5);
    QTest::qWait(50);
    QVERIFY(spy.count() > 1);
    QVERIFY(timeLine.currentFrame() < oldValue);
}

void tst_QTimeLine::currentTime()
{
    QTimeLine timeLine(2000);
    timeLine.setUpdateInterval((timeLine.duration()/2) / 33);
    QSignalSpy spy(&timeLine, SIGNAL(valueChanged(qreal)));
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.currentTime(), 0);
    timeLine.start();
    QTest::qWait(timeLine.duration()/2);
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    QVERIFY(timeLine.currentTime() > timeLine.duration()/2 - timeLine.duration()/10);
    QVERIFY(timeLine.currentTime() < timeLine.duration()/2 + timeLine.duration()/10);
    QTest::qWait(timeLine.duration()/4 + timeLine.duration());
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    QCOMPARE(timeLine.currentTime(), timeLine.duration());

    spy.clear();
    timeLine.setCurrentTime(timeLine.duration()/2);
    timeLine.setCurrentTime(timeLine.duration()/2);
    QCOMPARE(spy.count(), 1);
    spy.clear();
    QCOMPARE(timeLine.currentTime(), timeLine.duration()/2);
    timeLine.resume();
    // Let it update on its own
    QTest::qWait(timeLine.duration()/4);
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    QVERIFY(timeLine.currentTime() > timeLine.duration()/2);
    QVERIFY(timeLine.currentTime() < timeLine.duration());
    QTest::qWait(timeLine.duration()/4 + timeLine.duration());
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    QVERIFY(timeLine.currentTime() == timeLine.duration());

    // Reverse should decrease the currentTime
    timeLine.setCurrentTime(timeLine.duration()/2);
    timeLine.start();
    // Let it update on its own
    QTest::qWait(timeLine.duration()/4);
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    int currentTime = timeLine.currentTime();
    timeLine.setDirection(QTimeLine::Backward);
    QTest::qWait(timeLine.duration()/4);
    QVERIFY(timeLine.currentTime() < currentTime);
    timeLine.stop();
}

void tst_QTimeLine::duration()
{
    QTimeLine timeLine(200);
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.duration(), 200);
    timeLine.setDuration(1000);
    QCOMPARE(timeLine.duration(), 1000);

    timeLine.start();
    QTest::qWait(999);
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    QVERIFY(timeLine.currentTime() > 0.9);
    QTest::qWait(50);
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    // The duration shouldn't change
    QCOMPARE(timeLine.duration(), 1000);
}

void tst_QTimeLine::frameRate()
{
    QTimeLine timeLine;
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.updateInterval(), 1000 / 25);
    timeLine.setUpdateInterval(1000 / 60);
    QCOMPARE(timeLine.updateInterval(), 1000 / 60);

    // Default speed
    timeLine.setUpdateInterval(1000 / 33);
    QSignalSpy spy(&timeLine, SIGNAL(frameChanged(int)));
    timeLine.start();
    QTest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    int slowCount = spy.count();

    // Faster!!
    timeLine.setUpdateInterval(1000 / 100);
    spy.clear();
    timeLine.setCurrentTime(0);
    timeLine.start();
    QTest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    QVERIFY(slowCount < spy.count());
}

void tst_QTimeLine::value()
{
    QTimeLine timeLine(2000);
    QVERIFY(timeLine.currentValue() == 0.0);

    // Default speed
    QSignalSpy spy(&timeLine, SIGNAL(valueChanged(qreal)));
    timeLine.start();
    QTest::qWait(timeLine.duration()/3);
    QVERIFY(timeLine.currentValue() > 0);
    QTest::qWait(timeLine.duration());
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    qreal currentValue = timeLine.currentValue();
    QVERIFY(currentValue == 1);
    QVERIFY(spy.count() > 0);

    // Reverse should decrease the value
    timeLine.setCurrentTime(100);
    timeLine.start();
    // Let it update on its own
    QTest::qWait(500);
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    qreal value = timeLine.currentValue();
    timeLine.setDirection(QTimeLine::Backward);
    QTest::qWait(1000);
    QVERIFY(timeLine.currentValue() < value);
    timeLine.stop();
}

void tst_QTimeLine::currentFrame()
{
    QTimeLine timeLine(2000);
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.currentFrame(), 10);

    // Default speed
    QSignalSpy spy(&timeLine, SIGNAL(frameChanged(int)));
    timeLine.start();
    QTest::qWait(timeLine.duration()/3);
    QVERIFY(timeLine.currentFrame() > 10);
    QTest::qWait(timeLine.duration());
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    QCOMPARE(timeLine.currentFrame(), 20);

    // Reverse should decrease the value
    timeLine.setCurrentTime(timeLine.duration()/2);
    timeLine.start();
    // Let it update on its own
    QTest::qWait(timeLine.duration()/4);
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    int value = timeLine.currentFrame();
    timeLine.setDirection(QTimeLine::Backward);
    QTest::qWait(timeLine.duration()/2);
    QVERIFY(timeLine.currentFrame() < value);
    timeLine.stop();
}

void tst_QTimeLine::loopCount()
{
    QTimeLine timeLine(200);
    QCOMPARE(timeLine.loopCount(), 1);
    timeLine.setFrameRange(10, 20);
    QCOMPARE(timeLine.loopCount(), 1);
    timeLine.setLoopCount(0);
    QCOMPARE(timeLine.loopCount(), 0);

    // Default speed infiniti looping
    QSignalSpy spy(&timeLine, SIGNAL(frameChanged(int)));
    timeLine.start();
    QTest::qWait(timeLine.duration());
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    //QCOMPARE(timeLine.currentFrame(), 20);
    QTest::qWait(timeLine.duration()*6);
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    QVERIFY(timeLine.currentTime() >= 0);
    QVERIFY(timeLine.currentFrame() >= 10);
    QVERIFY(timeLine.currentFrame() <= 20);
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    timeLine.stop();

    timeLine.setDuration(2500); // ### some platforms have a very low resolution timer
    timeLine.setFrameRange(0, 2);
    timeLine.setLoopCount(4);

    QSignalSpy finishedSpy(&timeLine, SIGNAL(finished()));
    QSignalSpy frameChangedSpy(&timeLine, SIGNAL(frameChanged(int)));
    QEventLoop loop;
    connect(&timeLine, SIGNAL(finished()), &loop, SLOT(quit()));


    for(int i=0;i<2;i++) {

        timeLine.start();
        //we clear te list after the start so we don't catch
        //a frameChanged signal for the frame 0 at the beginning
        finishedSpy.clear();
        frameChangedSpy.clear();

        loop.exec();

        QCOMPARE(finishedSpy.count(), 1);
        QCOMPARE(frameChangedSpy.count(), 11);
        for (int i = 0; i < 11; ++i) {
            QCOMPARE(frameChangedSpy.at(i).at(0).toInt(), (i+1) % 3);
        }
    }

    timeLine.setDirection(QTimeLine::Backward);
    timeLine.start();
    loop.exec();

    QCOMPARE(finishedSpy.count(), 2);
    QCOMPARE(frameChangedSpy.count(), 22);
    for (int i = 11; i < 22; ++i) {
        QCOMPARE(frameChangedSpy.at(i).at(0).toInt(), 2 - (i+2) % 3);
    }
}

void tst_QTimeLine::interpolation()
{
    QTimeLine timeLine(400);
    QCOMPARE(timeLine.easingCurve().type(), QEasingCurve::InOutSine);
    timeLine.setFrameRange(100, 200);
    timeLine.setEasingCurve(QEasingCurve(QEasingCurve::Linear));
    QCOMPARE(timeLine.easingCurve().type(), QEasingCurve::Linear);

    // smooth
    timeLine.setEasingCurve(QEasingCurve(QEasingCurve::InOutSine));
    timeLine.start();
    QTest::qWait(100);
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    int firstValue = timeLine.currentFrame();
    QTest::qWait(200);
    int endValue = timeLine.currentFrame();
    timeLine.stop(); // ### todo reset?
    timeLine.setCurrentTime(0); // ### todo reset?

    // linear
    timeLine.setEasingCurve(QEasingCurve(QEasingCurve::Linear));
    timeLine.start();
    QTest::qWait(100);
    QCOMPARE(timeLine.state(), QTimeLine::Running);

    // Smooth accellerates slowly so in the beginning so it is farther behind
    QVERIFY(firstValue < timeLine.currentFrame());
    QTest::qWait(200);
    QVERIFY(endValue > timeLine.currentFrame());
    timeLine.stop();
}

void tst_QTimeLine::reverse_data()
{
    QTest::addColumn<int>("duration");
    QTest::addColumn<int>("start");
    QTest::addColumn<int>("end");
    QTest::addColumn<int>("direction");
    QTest::addColumn<int>("direction2");
    QTest::addColumn<int>("direction3");
    QTest::addColumn<int>("startTime");
    QTest::addColumn<int>("currentFrame");
    QTest::addColumn<qreal>("currentValue");
    QTest::addColumn<int>("wait");
    QTest::addColumn<int>("state");
    QTest::addColumn<int>("wait2");

    QTest::newRow("start at end") << 200 << 1000 << 2000 << (int)QTimeLine::Backward << (int)QTimeLine::Forward << (int)QTimeLine::Backward << 200 << 2000 << qreal(1.0) << 40 << (int)QTimeLine::Running << 140;
    QTest::newRow("start at half") << 200 << 1000 << 2000 << (int)QTimeLine::Backward << (int)QTimeLine::Forward << (int)QTimeLine::Backward << 100 << 1500 << qreal(0.5) << 40 << (int)QTimeLine::Running << 140;
    QTest::newRow("start at quarter") << 200 << 1000 << 2000 << (int)QTimeLine::Backward << (int)QTimeLine::Forward << (int)QTimeLine::Backward << 50 << 1250 << qreal(0.25) << 40 << (int)QTimeLine::Running << 140;
}

void tst_QTimeLine::reverse()
{
    QFETCH(int, duration);
    QFETCH(int, start);
    QFETCH(int, end);
    QFETCH(int, direction);
    QFETCH(int, direction2);
    QFETCH(int, direction3);
    QFETCH(int, startTime);
    QFETCH(int, currentFrame);
    QFETCH(qreal, currentValue);
    QFETCH(int, wait);
    QFETCH(int, state);
    QFETCH(int, wait2);

    QTimeLine timeLine(duration);
    timeLine.setEasingCurve(QEasingCurve(QEasingCurve::Linear));
    timeLine.setFrameRange(start, end);

    timeLine.setDirection((QTimeLine::Direction)direction);
    timeLine.setDirection((QTimeLine::Direction)direction2);
    timeLine.setDirection((QTimeLine::Direction)direction3);
    QCOMPARE(timeLine.direction(), ((QTimeLine::Direction)direction));

    timeLine.setCurrentTime(startTime);
    timeLine.setDirection((QTimeLine::Direction)direction);
    timeLine.setDirection((QTimeLine::Direction)direction2);
    timeLine.setDirection((QTimeLine::Direction)direction3);

    QCOMPARE(timeLine.currentFrame(), currentFrame);
    QCOMPARE(timeLine.currentValue(), currentValue);
    timeLine.start();

    QTest::qWait(wait);
    QCOMPARE(timeLine.state(), (QTimeLine::State)state);
    int firstValue = timeLine.currentFrame();
    timeLine.setDirection((QTimeLine::Direction)direction2);
    timeLine.setDirection((QTimeLine::Direction)direction3);
    timeLine.setDirection((QTimeLine::Direction)direction2);
    timeLine.setDirection((QTimeLine::Direction)direction3);
    QTest::qWait(wait2);
    int endValue = timeLine.currentFrame();
    QVERIFY(endValue < firstValue);


}

/*
void tst_QTimeLine::toggle()
{
    QTimeLine timeLine;
    QCOMPARE(timeLine.isReverse(), false);
    timeLine.toggle();
    QCOMPARE(timeLine.isReverse(), true);
    timeLine.toggle();
    QCOMPARE(timeLine.isReverse(), false);
}
*/
/*
void tst_QTimeLine::reset()
{
    QTimeLine timeLine;
    timeLine.setFrameRange(10,100);

    timeLine.setLoopCount(-1);
    QSignalSpy spy(&timeLine, SIGNAL(frameChanged(int)));
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    timeLine.start();
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    int wait = timeLine.duration()*5/3;
    QTest::qWait(wait);
    QVERIFY(spy.count() >= 1 );
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    timeLine.setDirection(QTimeLine::Backward);
    QVERIFY(timeLine.currentFrame() != 10);
    QVERIFY(timeLine.currentTime() != 0);
    QVERIFY(timeLine.state() != QTimeLine::Forward);
    QVERIFY(timeLine.loopCount() != 0);

    timeLine.reset();
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    QCOMPARE(timeLine.currentFrame(), timeLine.startFrame());
    QCOMPARE(timeLine.currentTime(), 0);
    timeLine.setDirection(QTimeLine::Backward);
    QCOMPARE(timeLine.loopCount(), 1);
    QCOMPARE(timeLine.startFrame(), 10);
    QCOMPARE(timeLine.endFrame(), 100);
}
*/
void tst_QTimeLine::frameChanged()
{
    QTimeLine timeLine;
    timeLine.setFrameRange(0,9);
    timeLine.setUpdateInterval(1000);
    QSignalSpy spy(&timeLine, SIGNAL(frameChanged(int)));
    timeLine.start();
    QTest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    // Probably 10
    QVERIFY(spy.count() <= 10 && spy.count() > 0);

    //timeLine.reset(); ### todo
    timeLine.setUpdateInterval(5);
    spy.clear();
    timeLine.setCurrentTime(0);
    timeLine.start();
    QTest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    // Probably 1
    QVERIFY(spy.count() <= 10 && spy.count() > 0);
}

void tst_QTimeLine::stopped()
{
    QTimeLine timeLine;
    timeLine.setFrameRange(0, 9);
    qRegisterMetaType<QTimeLine::State>("QTimeLine::State");
    QSignalSpy spy(&timeLine, SIGNAL(stateChanged(QTimeLine::State)));
    timeLine.start();
    QTest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    QCOMPARE(spy.count(), 2);
    // timeLine.reset(); ### todo
    spy.clear();
    //int currentFrame = timeLine.currentFrame();
    //int currentCurrentTime = timeLine.currentTime();
    timeLine.start();
    timeLine.stop();
    QCOMPARE(spy.count(), 2);
    //QCOMPARE(timeLine.currentFrame(), currentFrame); ### Behavioral change
    //QCOMPARE(timeLine.currentTime(), currentCurrentTime);
    timeLine.setDirection(QTimeLine::Backward);
    QCOMPARE(timeLine.loopCount(), 1);
}

void tst_QTimeLine::finished()
{
    QTimeLine timeLine;
    timeLine.setFrameRange(0,9);
    QSignalSpy spy(&timeLine, SIGNAL(finished()));
    timeLine.start();
    QTest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    QCOMPARE(spy.count(), 1);

    spy.clear();
    timeLine.start();
    timeLine.stop();
    QCOMPARE(spy.count(), 0);
}

void tst_QTimeLine::isRunning()
{
    QTimeLine timeLine;
    timeLine.setFrameRange(0,9);
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
    timeLine.start();
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    timeLine.stop();
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);

    timeLine.start();
    QTest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);
}

void tst_QTimeLine::multipleTimeLines()
{
    // Stopping a timer shouldn't affect the other timers
    QTimeLine timeLine(200);
    timeLine.setFrameRange(0,99);
    QSignalSpy spy(&timeLine, SIGNAL(finished()));

    QTimeLine timeLineKiller;
    timeLineKiller.setFrameRange(0,99);

    timeLineKiller.start();
    timeLine.start();
    timeLineKiller.stop();
    QTest::qWait(timeLine.duration()*2);
    QCOMPARE(spy.count(), 1);
}

void tst_QTimeLine::sineCurve()
{
    QTimeLine timeLine(1000);
    timeLine.setEasingCurve(QEasingCurve(QEasingCurve::SineCurve));
    QCOMPARE(timeLine.valueForTime(0), qreal(0));
    QCOMPARE(timeLine.valueForTime(250), qreal(0.5));
    QCOMPARE(timeLine.valueForTime(500), qreal(1));
    QCOMPARE(timeLine.valueForTime(750), qreal(0.5));
    QCOMPARE(timeLine.valueForTime(1000), qreal(0));
}

void tst_QTimeLine::cosineCurve()
{
    QTimeLine timeLine(1000);
    timeLine.setEasingCurve(QEasingCurve(QEasingCurve::CosineCurve));
    QCOMPARE(timeLine.valueForTime(0), qreal(0.5));
    QCOMPARE(timeLine.valueForTime(250), qreal(1));
    QCOMPARE(timeLine.valueForTime(500), qreal(0.5));
    QCOMPARE(timeLine.valueForTime(750), qreal(0));
    QCOMPARE(timeLine.valueForTime(1000), qreal(0.5));
}

void tst_QTimeLine::outOfRange()
{
    QTimeLine timeLine(1000);
    QCOMPARE(timeLine.valueForTime(-100), qreal(0));
    QCOMPARE(timeLine.valueForTime(2000), qreal(1));

    timeLine.setEasingCurve(QEasingCurve(QEasingCurve::SineCurve));
    QCOMPARE(timeLine.valueForTime(2000), qreal(0));
}

void tst_QTimeLine::stateInFinishedSignal()
{
    QTimeLine timeLine(50);

    connect(&timeLine, SIGNAL(finished()), this, SLOT(finishedSlot()));
    state = QTimeLine::State(-1);

    timeLine.start();
    QTest::qWait(250);

    QCOMPARE(state, QTimeLine::NotRunning);
}

void tst_QTimeLine::finishedSlot()
{
    QTimeLine *timeLine = qobject_cast<QTimeLine *>(sender());
    if (timeLine)
        state = timeLine->state();
}

void tst_QTimeLine::resume()
{
    QTimeLine timeLine(1000);
    {
        QCOMPARE(timeLine.currentTime(), 0);
        timeLine.start();
        QTest::qWait(250);
        timeLine.stop();
        int oldCurrentTime = timeLine.currentTime();
        QVERIFY(oldCurrentTime > 0);
        QVERIFY(oldCurrentTime < 1000);
        timeLine.resume();
        QTest::qWait(250);
        timeLine.stop();
        int currentTime = timeLine.currentTime();
        QVERIFY(currentTime > oldCurrentTime);
        QVERIFY(currentTime < 1000);
    }
    timeLine.setDirection(QTimeLine::Backward);
    {
        timeLine.setCurrentTime(1000);
        QCOMPARE(timeLine.currentTime(), 1000);
        timeLine.start();
        QTest::qWait(250);
        timeLine.stop();
        int oldCurrentTime = timeLine.currentTime();
        QVERIFY(oldCurrentTime < 1000);
        QVERIFY(oldCurrentTime > 0);
        timeLine.resume();
        QTest::qWait(250);
        timeLine.stop();
        int currentTime = timeLine.currentTime();
        QVERIFY(currentTime < oldCurrentTime);
        QVERIFY(currentTime > 0);
    }
}

void tst_QTimeLine::restart()
{
    QTimeLine timeLine(100);
    timeLine.setFrameRange(0,9);

    timeLine.start();
    QTest::qWait(timeLine.duration()*2);
    QCOMPARE(timeLine.currentFrame(), timeLine.endFrame());
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);

    // A restart with the same duration
    timeLine.start();
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    QCOMPARE(timeLine.currentFrame(), timeLine.startFrame());
    QCOMPARE(timeLine.currentTime(), 0);
    QTest::qWait(250);
    QCOMPARE(timeLine.currentFrame(), timeLine.endFrame());
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);

    // Set a smaller duration and restart
    timeLine.setDuration(50);
    timeLine.start();
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    QCOMPARE(timeLine.currentFrame(), timeLine.startFrame());
    QCOMPARE(timeLine.currentTime(), 0);
    QTest::qWait(250);
    QCOMPARE(timeLine.currentFrame(), timeLine.endFrame());
    QCOMPARE(timeLine.state(), QTimeLine::NotRunning);

    // Set a longer duration and restart
    timeLine.setDuration(150);
    timeLine.start();
    QCOMPARE(timeLine.state(), QTimeLine::Running);
    QCOMPARE(timeLine.currentFrame(), timeLine.startFrame());
    QCOMPARE(timeLine.currentTime(), 0);
}

void tst_QTimeLine::setPaused()
{
    QTimeLine timeLine(1000);
    {
        QCOMPARE(timeLine.currentTime(), 0);
        timeLine.start();
        QTest::qWait(250);
        timeLine.setPaused(true);
        int oldCurrentTime = timeLine.currentTime();
        QVERIFY(oldCurrentTime > 0);
        QVERIFY(oldCurrentTime < 1000);
        QTest::qWait(1000);
        timeLine.setPaused(false);
        QTest::qWait(250);
        int currentTime = timeLine.currentTime();
        QVERIFY(currentTime > 0);
        QVERIFY(currentTime > oldCurrentTime);
        QVERIFY(currentTime < 1000);
        timeLine.stop();
    }
}

QTEST_MAIN(tst_QTimeLine)

#include "moc_tst_qtimeline.cpp"
