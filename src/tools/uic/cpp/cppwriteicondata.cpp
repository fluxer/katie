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

#include "cppwriteicondata.h"
#include "driver.h"
#include "ui4.h"
#include "uic.h"
#include "qcorecommon_p.h"

#include <QtCore/QTextStream>

QT_BEGIN_NAMESPACE

namespace CPP {

static QByteArray transformImageData(const QString &data)
{
    int baSize = data.length() / 2;
    QSTACKARRAY(uchar, ba, baSize);
    for (int i = 0; i < baSize; ++i) {
        char h = data[2 * i].toLatin1();
        char l = data[2 * i + 1].toLatin1();
        uchar r = 0;
        if (h <= '9')
            r += h - '0';
        else
            r += h - 'a' + 10;
        r = r << 4;
        if (l <= '9')
            r += l - '0';
        else
            r += l - 'a' + 10;
        ba[i] = r;
    }
    return QByteArray(reinterpret_cast<const char *>(ba), baSize);
}

WriteIconData::WriteIconData(Uic *uic)
    : driver(uic->driver()), output(uic->output()), option(uic->option())
{
}

void WriteIconData::acceptImage(DomImage *image)
{
    writeImage(output, option.indent, image);
}

void WriteIconData::writeImage(QTextStream &output, const QString &indent, const DomImage *image)
{
    QString img = image->attributeName() + QLatin1String("_data");
    QString data = image->elementData()->text();

    output << indent << "static const unsigned char " << img << "[] = { \n";
    output << indent;
    int a;
    for (a = 0; a < (int) (data.length()/2)-1; a++) {
        output << "0x" << QString(data[2*a]) << QString(data[2*a+1]) << ',';
        if (a % 12 == 11)
            output << '\n' << indent;
        else
            output << ' ';
    }
    output << "0x" << QString(data[2*a]) << QString(data[2*a+1]) << '\n';
    output << "};\n\n";
}

void WriteIconData::writeImage(QIODevice &output, DomImage *image)
{
    QByteArray array = transformImageData(image->elementData()->text());
    output.write(array, array.size());
}

} // namespace CPP

QT_END_NAMESPACE
