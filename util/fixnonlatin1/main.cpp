/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the utils of the Katie Toolkit.
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
#include <QtCore/QtCore>

QT_USE_NAMESPACE

// Scans files for characters >127 and replaces them with the \nnn octal representation

int main(int argc, char *argv[])
{
    if (argc <= 1)
        qFatal("Usage: %s FILES", argc ? argv[0] : "fixnonlatin1");
    for (int i = 1; i < argc; ++i) {

        QString fileName = QString::fromLocal8Bit(argv[i]);
        if (   fileName.endsWith(QLatin1String(".gif"))
            || fileName.endsWith(QLatin1String(".jpg"))
            || fileName.endsWith(QLatin1String(".tif"))
            || fileName.endsWith(QLatin1String(".tiff"))
            || fileName.endsWith(QLatin1String(".png"))
            || fileName.endsWith(QLatin1String(".mng"))
            || fileName.endsWith(QLatin1String(".ico"))
            || fileName.endsWith(QLatin1String(".zip"))
            || fileName.endsWith(QLatin1String(".gz"))
            || fileName.endsWith(QLatin1String(".qpf"))
            || fileName.endsWith(QLatin1String(".ttf"))
            || fileName.endsWith(QLatin1String(".pfb"))
            || fileName.endsWith(QLatin1String(".exe"))
            || fileName.endsWith(QLatin1String(".nib"))
            || fileName.endsWith(QLatin1String(".o"))
            )
            continue;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            qFatal("Cannot open '%s': %s", argv[i], qPrintable(file.errorString()));

        QByteArray ba = file.readAll();
        bool mod = false;
        for (int j = 0; j < ba.count(); ++j) {
            uchar c = ba.at(j);
            if (c > 127) {
                ba[j] = '\\';
                ba.insert(j + 1, QByteArray::number(c, 8).rightJustified(3, '0', true));
                j += 3;
                mod = true;
            }
        }
        file.close();

        if (!mod)
            continue;

        qWarning("found non-latin1 characters in '%s'", argv[i]);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning("Cannot open '%s' for writing: %s", argv[i], qPrintable(file.errorString()));
            continue;
        }
        if (file.write(ba) < 0)
            qFatal("Error while writing into '%s': %s", argv[i], qPrintable(file.errorString()));
        file.close();
    }

    return 0;
}

