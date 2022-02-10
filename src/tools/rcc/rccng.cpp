/****************************************************************************
**
** Copyright (C) 2022 Ivailo Monev
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

#include <QDebug>
#include <QDir>
#include <QFile>

QT_BEGIN_NAMESPACE

static QByteArray createResourceData(const QByteArray &content, const QString &filename,
                                     const bool compressed, const qint64 uncompressedsize)
{
    const QString cppfilename = filename.toLower().simplified().replace(" ", "_");

    QByteArray cppcontent;
    static const char * const digits = "0123456789abcdef";
    int wrapcount = 0;
    int chopcount = 2;
    for (int i = 0; i < content.size(); i++) {
        cppcontent += "0x";
        const uchar c = content.at(i);
        if (c < 16) {
            cppcontent += digits[c];
        } else {
            cppcontent += digits[c >> 4];
            cppcontent += digits[c & 0xf];
        }

        wrapcount++;
        if (wrapcount == 20) {
            cppcontent += ",\n        ";
            chopcount = 10;
            wrapcount = 0;
        } else {
            chopcount = 2;
            cppcontent += ", ";
        }
    }
    cppcontent.chop(chopcount);

    QByteArray compressedbytes("static_qrc_data");
    if (compressed) {
        compressedbytes = "qUncompress(reinterpret_cast<const char*>(static_qrc_data, ";
        compressedbytes += QByteArray::number(content.size());
        compressedbytes += "))";
    }

    const QByteArray uncompressedbytessize = QByteArray::number(uncompressedsize);

    QString result = QString::fromLatin1(
"static const uchar* qrc_%1()\n"
"{\n"
"    static const uchar static_qrc_data[] = {\n"
"        %2\n"
"    };\n"
"    return %3;\n"
"};\n"
"static const qint64 qrc_%1_size = %4;\n"
    ).arg(cppfilename, cppcontent.constData(), compressedbytes.constData(), uncompressedbytessize.constData());

    return result.toLocal8Bit();
}

void showUsage()
{
    ::fprintf(stderr, "Usage:\n"
              "    rcc [options] <file> [<file>] ...\n\n"
              "Options:\n"
              "    -threshold <level>\n"
              "         Threshold to consider when compressing files.\n\n"
              "    -compress <level>\n"
              "         Compress input files by level.\n\n"
              "    -version\n"
              "         Display version.\n\n"
              "    -help\n"
              "         Display this information.\n");
    ::exit(1);
}

int runRcc(int argc, char *argv[])
{
    QStringList args;
    for (int i = 0; i != argc; i++) {
        args += QString::fromLocal8Bit(argv[i]);
    }

    if (args.size() <= 1) {
        showUsage();
    }

    // parse options
    QStringList input;
    qreal threshold = 2.0;
    int compresslevel = 1;
    for (int i = 1; i < args.count(); i++) {
        const QString arg = args.at(i);
        if (arg.isEmpty()) {
            continue;
        }

        if (arg == QLatin1String("-threshold")) {
            if ((i + 1) >= args.count()) {
                ::fprintf(stderr, "rcc: missing threshold value argument");
                ::exit(3);
            }
            threshold = args.at(i + 1).toDouble();
            i++;
        } else if (arg == QLatin1String("-compress")) {
            if ((i + 1) >= args.count()) {
                ::fprintf(stderr, "rcc: missing compress value argument");
                ::exit(4);
            }
            compresslevel = args.at(i + 1).toInt();
            i++;
        } else if (arg == QLatin1String("-version")) {
            ::fprintf(stderr, "Katie Resource Compiler version %s\n", QT_VERSION_STR);
            ::exit(5);
        } else if (QFile::exists(arg)) {
            input.append(arg);
        } else {
            showUsage();
        }
    }

    foreach (const QString &filepath, input) {
        QFile filehandle(filepath);
        if (!filehandle.open(QFile::ReadOnly)) {
            ::fprintf(stderr, "rcc: could not open %s\n", filepath.toLocal8Bit().constData());
            return 6;
        }
        QByteArray filecontent = filehandle.readAll();
        if (filecontent.isEmpty()) {
            ::fprintf(stderr, "rcc: file is empty %s\n", filepath.toLocal8Bit().constData());
            return 7;
        }

        const QString filename = QFileInfo(filepath).baseName();
        if (filename.isEmpty()) {
            ::fprintf(stderr, "rcc: could not determine file base name %s\n", filepath.toLocal8Bit().constData());
            return 8;
        }

        const qint64 uncompressedsize = filecontent.size();
        bool compressed = false;
        const QByteArray compressedcontent = qCompress(filecontent, compresslevel);
        if ((qreal(filecontent.size()) / qreal(compressedcontent.size())) >= threshold) {
            compressed = true;
            filecontent = compressedcontent;
        }

        const QByteArray resourcedata = createResourceData(filecontent, filename, compressed, uncompressedsize);
        ::fprintf(stdout, resourcedata.constData());
    }

    return 0;
}

QT_END_NAMESPACE

int main(int argc, char *argv[])
{
    return QT_PREPEND_NAMESPACE(runRcc)(argc, argv);
}
