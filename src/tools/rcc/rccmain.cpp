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

#include "rcc.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

QT_BEGIN_NAMESPACE

void showHelp( const QString &error)
{
    if (!error.isEmpty())
        fprintf(stderr, "rcc: %s\n", qPrintable(error));
    fprintf(stderr, "Usage:\n"
        "    rcc [options] <resource-file> [<resource-file>] ...\n\n"
        "Options:\n"
        "    -o <file>\n"
        "         Write output to file rather than stdout.\n\n"
        "    -name <name>\n"
        "         Create an external initialization function with name.\n\n"
        "    -threshold <level>\n"
        "         Threshold to consider compressing files.\n\n"
        "    -compress <level>\n"
        "         Compress input files by level.\n\n"
        "    -root <path>\n"
        "         Prefix resource access path with root path.\n\n"
        "    -no-compress\n"
        "         Disable all compression.\n\n"
        "    -binary\n"
        "         Output a binary file for use as a dynamic resource.\n\n"
        "    -namespace\n"
        "         Turn off namespace macros.\n\n"
        "    -project\n"
        "         Output a resource file containing all files from the\n"
        "         current directory.\n\n"
        "    -list\n"
        "         Lists .qrc file entries.\n\n"
        "    -version\n"
        "         Display version.\n\n"
        "    -help\n"
        "         Display this information.\n");
}

void dumpRecursive(const QDir &dir, QTextStream &out)
{
    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot
                                              | QDir::NoSymLinks);
    foreach (const QFileInfo &entry, entries) {
        if (entry.isDir()) {
            dumpRecursive(entry.filePath(), out);
        } else {
            out << QLatin1String("<file>")
                << entry.filePath()
                << QLatin1String("</file>\n");
        }
    }
}

int createProject(const QString &outFileName)
{
    QDir currentDir = QDir::current();
    QString currentDirName = currentDir.dirName();
    if (currentDirName.isEmpty())
        currentDirName = QLatin1String("root");

    QFile file;
    bool isOk = false;
    if (outFileName.isEmpty()) {
        isOk = file.open(stdout, QFile::WriteOnly | QFile::Text);
    } else {
        file.setFileName(outFileName);
        isOk = file.open(QFile::WriteOnly | QFile::Text);
    }
    if (!isOk) {
        fprintf(stderr, "Unable to open %s: %s\n",
                outFileName.isEmpty() ? qPrintable(outFileName) : "standard output",
                qPrintable(file.errorString()));
        return 1;
    }

    QTextStream out(&file);
    out << QLatin1String("<!DOCTYPE RCC><RCC version=\"1.0\">\n"
                         "<qresource>\n");

    // use "." as dir to get relative file pathes
    dumpRecursive(QDir(QLatin1String(".")), out);

    out << QLatin1String("</qresource>\n"
                         "</RCC>\n");

    return 0;
}

int runRcc(int argc, char *argv[])
{
    QString outFilename;
    bool helpRequested = false;
    bool list = false;
    bool projectRequested = false;
    QStringList filenamesIn;

    QStringList args;
    for (int i = 0; i != argc; ++i)
        args += QString::fromLocal8Bit(argv[i]);

    RCCResourceLibrary library;

    // parse options
    QString errorMsg;
    for (int i = 1; i < args.count() && errorMsg.isEmpty(); i++) {
        if (args[i].isEmpty())
            continue;
        if (args[i][0] == QLatin1Char('-')) {   // option
            QString opt = args[i];
            if (opt == QLatin1String("-o")) {
                if (!(i < argc-1)) {
                    errorMsg = QLatin1String("Missing output name");
                    break;
                }
                outFilename = args[++i];
            } else if (opt == QLatin1String("-name")) {
                if (!(i < argc-1)) {
                    errorMsg = QLatin1String("Missing target name");
                    break;
                }
                library.setInitName(args[++i]);
            } else if (opt == QLatin1String("-root")) {
                if (!(i < argc-1)) {
                    errorMsg = QLatin1String("Missing root path");
                    break;
                }
                library.setResourceRoot(QDir::cleanPath(args[++i]));
                if (library.resourceRoot().isEmpty()
                        || library.resourceRoot().at(0) != QLatin1Char('/'))
                    errorMsg = QLatin1String("Root must start with a /");
            } else if (opt == QLatin1String("-compress")) {
                if (!(i < argc-1)) {
                    errorMsg = QLatin1String("Missing compression level");
                    break;
                }
                library.setCompressLevel(args[++i].toInt());
            } else if (opt == QLatin1String("-threshold")) {
                if (!(i < argc-1)) {
                    errorMsg = QLatin1String("Missing compression threshold");
                    break;
                }
                library.setCompressThreshold(args[++i].toInt());
            } else if (opt == QLatin1String("-binary")) {
                library.setFormat(RCCResourceLibrary::Binary);
            } else if (opt == QLatin1String("-namespace")) {
                library.setUseNameSpace(!library.useNameSpace());
            } else if (opt == QLatin1String("-verbose")) {
                library.setVerbose(true);
            } else if (opt == QLatin1String("-list")) {
                list = true;
            } else if (opt == QLatin1String("-version") || opt == QLatin1String("-v")) {
                fprintf(stderr, "Katie Resource Compiler version %s\n", QT_VERSION_STR);
                return 1;
            } else if (opt == QLatin1String("-help") || opt == QLatin1String("-h")) {
                helpRequested = true;
            } else if (opt == QLatin1String("-no-compress")) {
                library.setCompressLevel(-2);
            } else if (opt == QLatin1String("-project")) {
                projectRequested = true;
            } else {
                errorMsg = QString::fromLatin1("Unknown option: '%1'").arg(args[i]);
            }
        } else {
            if (!QFile::exists(args[i])) {
                qWarning("%s: File does not exist '%s'",
                    qPrintable(args[0]), qPrintable(args[i]));
                return 1;
            }
            filenamesIn.append(args[i]);
        }
    }

    if (projectRequested && !helpRequested) {
        return createProject(outFilename);
    }

    if (!filenamesIn.size() || !errorMsg.isEmpty() || helpRequested) {
        showHelp(errorMsg);
        return 1;
    }
    QFile errorDevice;
    errorDevice.open(stderr, QIODevice::WriteOnly|QIODevice::Text);
    
    if (library.verbose())
        errorDevice.write("Katie resource compiler\n");

    library.setInputFiles(filenamesIn);

    if (!library.readFiles(list, errorDevice))
        return 1;

    // open output
    QFile out;
    QIODevice::OpenMode mode = QIODevice::WriteOnly;
    if (library.format() == RCCResourceLibrary::C_Code)
        mode |= QIODevice::Text;

    if (outFilename.isEmpty() || outFilename == QLatin1String("-")) {
        // using this overload close() only flushes.
        out.open(stdout, mode);
    } else {
        out.setFileName(outFilename);
        if (!out.open(mode)) {
            const QString msg = QString::fromLatin1("Unable to open %1 for writing: %2\n").arg(outFilename).arg(out.errorString());
            errorDevice.write(msg.toUtf8());
            return 1;
        }
    }

    // do the task
    if (list) {
        foreach (const QString &path, library.dataFiles()) {
            out.write(qPrintable(QDir::cleanPath(path)));
            out.write("\n");
        }
        return 0;
    }

    return library.output(out, errorDevice) ? 0 : 1;
}

QT_END_NAMESPACE

int main(int argc, char *argv[])
{
    return QT_PREPEND_NAMESPACE(runRcc)(argc, argv);
}
