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
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <QFile>

class tst_compilation : public QObject
{
    Q_OBJECT
public:
    tst_compilation();

private slots:
    void boomblock();

private:
    QDeclarativeEngine engine;
};

tst_compilation::tst_compilation()
{
}

void tst_compilation::boomblock()
{
    QFile f(QLatin1String(SRCDIR) + QLatin1String("/data/BoomBlock.qml"));
    QVERIFY(f.open(QIODevice::ReadOnly));
    QByteArray data = f.readAll();

    //get rid of initialization effects
    {
        QDeclarativeComponent c(&engine);
        c.setData(data, QUrl());
    }

    QBENCHMARK {
        QDeclarativeComponent c(&engine);
        c.setData(data, QUrl());
//        QVERIFY(c.isReady());
    }
}

QTEST_MAIN(tst_compilation)

#include "moc_tst_compilation.cpp"
