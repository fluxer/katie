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

#include <QtCore>

int main(int argc, char **argv)
{
    QCoreApplication ca(argc, argv);
    QFile f;
    f.open(stdin, QIODevice::ReadOnly);
    QString input;
    char buf[1024];
    qint64 len;
    while ((len = f.read(buf, 1024)) > 0)
        input += QByteArray(buf, len);
    f.close();
    QFile f2("fileWriterProcess.txt");
    f2.open(QIODevice::WriteOnly | QIODevice::Truncate);
    f2.write(input.toLatin1());
    f2.close();
    return 0;
}
