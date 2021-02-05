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
#include <QtGui>

#include <qtest.h>


class tst_qapplication : public QObject
{
    Q_OBJECT
private slots:
    void ctor();
};

/*
    Test the performance of the QApplication constructor.

    Note: results from the second start on can be misleading,
    since all global statics are already initialized.
*/
void tst_qapplication::ctor()
{
    // simulate reasonable argc, argv
    int argc = 1;
    char *argv[] = { (char*)"tst_qapplication" };
    QBENCHMARK {
        QApplication app(argc, argv);
    }
}

QTEST_APPLESS_MAIN(tst_qapplication)

#include "moc_main.cpp"
