/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

QT_USE_NAMESPACE

int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(qtconfig);

    QApplication app(argc, argv);

#ifndef QT_NO_TRANSLATION
    QTranslator translator;
    QTranslator qtTranslator;
    if (translator.load(QLatin1String("qt_tools"))
        && qtTranslator.load(QLatin1String("qt"))) {
        app.installTranslator(&translator);
        app.installTranslator(&qtTranslator);
    }
#endif

    MainWindow mw;
    mw.show();
    return app.exec();
}

#include "qrc_qtconfig.cpp"
