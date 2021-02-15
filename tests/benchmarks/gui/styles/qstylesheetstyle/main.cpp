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
// This file contains benchmarks for QRect/QRectF functions.

#include <QtGui>
#include <qtest.h>

class tst_qstylesheetstyle : public QObject
{
    Q_OBJECT
private slots:
    void empty();
    void empty_events();

    void simple();
    void simple_events();

    void grid_data();
    void grid();

private:
    QWidget *buildSimpleWidgets();

};


QWidget *tst_qstylesheetstyle::buildSimpleWidgets()
{
    QWidget *w = new QWidget();
    QGridLayout *layout = new QGridLayout(w);
    w->setLayout(layout);
    layout->addWidget(new QPushButton(QLatin1String("pushButton"))     ,0,0);
    layout->addWidget(new QComboBox()                                  ,0,1);
    layout->addWidget(new QCheckBox(QLatin1String("checkBox"))         ,0,2);
    layout->addWidget(new QRadioButton(QLatin1String("radioButton"))   ,0,3);
    layout->addWidget(new QLineEdit()                                  ,1,0);
    layout->addWidget(new QLabel(QLatin1String("label"))               ,1,1);
    layout->addWidget(new QSpinBox()                                   ,1,2);
    layout->addWidget(new QProgressBar()                               ,1,3);
    return w;
}

void tst_qstylesheetstyle::empty()
{
    QWidget *w = buildSimpleWidgets();
    w->setStyleSheet(QLatin1String("/* */"));
    QApplication::processEvents();
    int i = 0;
    QBENCHMARK {
        w->setStyleSheet(QLatin1String("/*") + QString::number(i) + QLatin1String("*/"));
        i++; // we want a different string in case we have severals iterations
    }
    delete w;
}

void tst_qstylesheetstyle::empty_events()
{
    QWidget *w = buildSimpleWidgets();
    w->setStyleSheet(QLatin1String("/* */"));
    QApplication::processEvents();
    int i = 0;
    QBENCHMARK {
        w->setStyleSheet(QLatin1String("/*") + QString::number(i) + QLatin1String("*/"));
        i++; // we want a different string in case we have severals iterations
        qApp->processEvents();
    }
    delete w;
}

static const char *simple_css =
   " QLineEdit { background: red; }   QPushButton { border: 1px solid yellow; color: pink; }  \n"
   " QCheckBox { margin: 3px 5px; background-color:red; } QAbstractButton { background-color: #456; } \n"
   " QFrame { padding: 3px; } QLabel { color: black } QSpinBox:hover { background-color:blue; }  ";

void tst_qstylesheetstyle::simple()
{
    QWidget *w = buildSimpleWidgets();
    w->setStyleSheet(QLatin1String("/* */"));
    QApplication::processEvents();
    int i = 0;
    QBENCHMARK {
        w->setStyleSheet(QLatin1String(simple_css) + QLatin1String("/*") + QString::number(i) + QLatin1String("*/"));
        i++; // we want a different string in case we have severals iterations
    }
    delete w;
}

void tst_qstylesheetstyle::simple_events()
{
    QWidget *w = buildSimpleWidgets();
    w->setStyleSheet(QLatin1String("/* */"));
    QApplication::processEvents();
    int i = 0;
    QBENCHMARK {
        w->setStyleSheet(QLatin1String(simple_css) + QLatin1String("/*") + QString::number(i) + QLatin1String("*/"));
        i++; // we want a different string in case we have severals iterations
        qApp->processEvents();
    }
    delete w;
}

void tst_qstylesheetstyle::grid_data()
{
        QTest::addColumn<bool>("events");
        QTest::addColumn<bool>("show");
        QTest::addColumn<int>("N");
        for (int n = 5; n <= 25; n += 5) {
           const QByteArray nString = QByteArray::number(n*n);
            QTest::newRow(QByteArray("simple--" + nString).constData()) << false << false << n;
            QTest::newRow(QByteArray("events--" + nString).constData()) << true << false << n;
            QTest::newRow(QByteArray("show--" + nString).constData()) << true << true << n;
        }
}


void tst_qstylesheetstyle::grid()
{
    QFETCH(bool, events);
    QFETCH(bool, show);
    QFETCH(int, N);

    QWidget *w = new QWidget();
    QGridLayout *layout = new QGridLayout(w);
    w->setLayout(layout);
    QString stylesheet;
    for(int x=0; x<N ;x++)
        for(int y=0; y<N ;y++) {
        QLabel *label = new QLabel(QString::number(y * N + x));
        layout->addWidget(label ,x,y);
        label->setObjectName(QString::fromLatin1("label%1").arg(y * N + x));
        stylesheet += QString::fromLatin1("#label%1 { background-color: rgb(0,%2,%3); color: rgb(%2,%3,255);  } ").arg(y*N+x).arg(y*255/N).arg(x*255/N);
    }

    w->setStyleSheet(QLatin1String("/* */"));
    if(show) {
        w->show();
        QTest::qWaitForWindowShown(w);
        QApplication::flush();
        QApplication::processEvents();
        QTest::qWait(30);
        QApplication::processEvents();
    }
    QApplication::processEvents();
    int i = 0;
    QBENCHMARK {
        w->setStyleSheet(stylesheet + QLatin1String("/*") + QString::number(i) + QLatin1String("*/"));
        i++; // we want a different string in case we have severals iterations
        if(events)
            qApp->processEvents();
    }
    delete w;
}

QTEST_MAIN(tst_qstylesheetstyle)

#include "moc_main.cpp"
