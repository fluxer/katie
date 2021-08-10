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

#include <QCoreApplication>
#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QDir>

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QStringList args = app.arguments();
    if (args.count() != 2) {
        fprintf(stderr, "Usage: testDetached filename.txt\n");
        return 128;
    }

    QFile f(args.at(1));
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        fprintf(stderr, "Cannot open %s for writing", qPrintable(f.fileName()));
        return 1;
    }

    f.write(QDir::currentPath().toUtf8());
    f.putChar('\n');
    f.write(QByteArray::number(quint64(::getpid())));
    f.putChar('\n');

    f.close();

    return 0;
}
