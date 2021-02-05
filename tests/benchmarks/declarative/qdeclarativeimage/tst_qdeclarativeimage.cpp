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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qtest.h>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <qdeclarativeimage_p.h>

class tst_qmlgraphicsimage : public QObject
{
    Q_OBJECT
public:
    tst_qmlgraphicsimage() {}

private slots:
    void qmlgraphicsimage();
    void qmlgraphicsimage_file();
    void qmlgraphicsimage_url();

private:
    QDeclarativeEngine engine;
};

void tst_qmlgraphicsimage::qmlgraphicsimage()
{
    int x = 0;
    QUrl url(QLatin1String(SRCDIR "/image.png"));
    QBENCHMARK {
        QUrl url2(QLatin1String("http://localhost/image") + QString::number(x++) + QLatin1String(".png"));
        QDeclarativeImage *image = new QDeclarativeImage;
        QDeclarativeEngine::setContextForObject(image, engine.rootContext());
        delete image;
    }
}

void tst_qmlgraphicsimage::qmlgraphicsimage_file()
{
    int x = 0;
    QUrl url(QLatin1String(SRCDIR "/image.png"));
    //get rid of initialization effects
    {
        QDeclarativeImage *image = new QDeclarativeImage;
        QDeclarativeEngine::setContextForObject(image, engine.rootContext());
        image->setSource(url);
    }
    QBENCHMARK {
        QUrl url2(QLatin1String("http://localhost/image") + QString::number(x++) + QLatin1String(".png"));
        QDeclarativeImage *image = new QDeclarativeImage;
        QDeclarativeEngine::setContextForObject(image, engine.rootContext());
        image->setSource(url);
        delete image;
    }
}

void tst_qmlgraphicsimage::qmlgraphicsimage_url()
{
    int x = 0;
    QUrl url(QLatin1String(SRCDIR "/image.png"));
    QBENCHMARK {
        QUrl url2(QLatin1String("http://localhost/image") + QString::number(x++) + QLatin1String(".png"));
        QDeclarativeImage *image = new QDeclarativeImage;
        QDeclarativeEngine::setContextForObject(image, engine.rootContext());
        image->setSource(url2);
        delete image;
    }
}

QTEST_MAIN(tst_qmlgraphicsimage)

#include "moc_tst_qdeclarativeimage.cpp"
