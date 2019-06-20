/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the utils of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#include <QtCore>

static QString utf8encode(const QByteArray &array) // turns e.g. tranøy.no to tran\xc3\xb8y.no
{
    QString result;
    result.reserve(array.length() + array.length() / 3);
    for (int i = 0; i < array.length(); ++i) {
        char c = array.at(i);
        // if char is non-ascii, escape it
        if (c < 0x20 || uchar(c) >= 0x7f) {
            result += QLatin1String("\\x") + QString::number(uchar(c), 16);
        } else {
            // if previous char was escaped, we need to make sure the next char is not
            // interpreted as part of the hex value, e.g. "äc.com" -> "\xabc.com"; this
            // should be "\xab""c.com"
            QRegExp hexEscape(QLatin1String("\\\\x[a-fA-F0-9][a-fA-F0-9]$"));
            bool isHexChar = ((c >= '0' && c <= '9') ||
                             (c >= 'a' && c <= 'f') ||
                             (c >= 'A' && c <= 'F'));
            if (result.contains(hexEscape) && isHexChar)
                result += QLatin1String("\"\"");
            result += QChar::fromLatin1(c);
        }
    }
    return result;
}

int main(int argc, char **argv) {

    QCoreApplication app(argc, argv);
    if (argc < 3) {
        printf("\nusage: %s inputFile outputFile\n\n", argv[0]);
        printf("'inputFile' should be a list of effective TLDs, one per line,\n");
        printf("as obtained from http://publicsuffix.org . To create indices and data file\n");
        printf("file, do the following:\n\n");
        printf("       wget https://publicsuffix.org/list/effective_tld_names.dat -O effective_tld_names.dat\n");
        printf("       grep '^[^\\/\\/]' effective_tld_names.dat > effective_tld_names.dat.trimmed\n");
        printf("       %s effective_tld_names.dat.trimmed effective_tld_names.dat.txt\n\n", argv[0]);
        printf("Now copy the data from effective_tld_names.dat.txt to the file src/core/io/qurltlds_p.h in your Qt repo\n\n");
        exit(1);
    }
    QFile file(QString::fromLatin1(argv[1]));
    QFile outFile(QString::fromLatin1(argv[2]));
    file.open(QIODevice::ReadOnly);
    outFile.open(QIODevice::WriteOnly);

    QByteArray outDataBufferBA;
    QBuffer outDataBuffer(&outDataBufferBA);
    outDataBuffer.open(QIODevice::WriteOnly);

    int lineCount = 0;
    while (!file.atEnd()) {
        QString s = QString::fromUtf8(file.readLine());
        QString st = s.trimmed();

        QString utf8String = utf8encode(st.toUtf8());

        outDataBuffer.write("    \"");
        outDataBuffer.write(utf8String.toUtf8());
        outDataBuffer.write("\\0\",\n");

        lineCount++;
    }

    outDataBuffer.close();
    outFile.write("\nstatic const char* TLDTbl[");
//    outFile.write(QByteArray::number(charSize)); // not needed
    outFile.write("] = {\n");
    outFile.write(outDataBufferBA);
    outFile.write("};\n\n");
    outFile.write("static const qint32 TLDTblSize = ");
    outFile.write(QByteArray::number(lineCount));
    outFile.write(";\n");
    outFile.close();
    printf("data generated to %s . Now copy the data from this file to src/core/io/qurltlds_p.h in your Qt repo\n", argv[2]);
    exit(0);
}
