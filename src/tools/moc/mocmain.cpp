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

#include "preprocessor.h"
#include "moc.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "qplatformdefs.h"

QT_BEGIN_NAMESPACE

/*
    This function looks at two file names and returns the name of the
    infile with a path relative to outfile.

    Examples:

        /tmp/abc, /tmp/bcd -> abc
        xyz/a/bc, xyz/b/ac -> ../a/bc
        /tmp/abc, xyz/klm -> /tmp/abc
 */

static QByteArray combinePath(const char *infile, const char *outfile)
{
    QFileInfo inFileInfo(QDir::current(), QFile::decodeName(infile));
    QFileInfo outFileInfo(QDir::current(), QFile::decodeName(outfile));
    return QFile::encodeName(outFileInfo.dir().relativeFilePath(inFileInfo.filePath()));
}


void error(const char *msg = "Invalid argument")
{
    if (msg)
        fprintf(stderr, "moc: %s\n", msg);
    fprintf(stderr, "Usage:\n"
            "    moc [options] <source-file|header-file> [<source-file|header-file>] ...\n\n"
            "moc reads one or more C++ class declarations from a C++ header or\n"
            "source file and generates one C++ source file containing meta\n"
            "object information for the classes. The C++ source file generated\n"
            "by the moc must be compiled and linked with the implementation of\n"
            "the class (or it can be #included into the class's source file).\n\n"
            "Options:\n"
            "    -o<file>\n"
            "         Write output to file rather than stdout.\n\n"
            "    -I<dir>\n"
            "         Add dir to the include path for header files.\n\n"
            "    -E\n"
            "         Preprocess only; do not generate meta object code.\n\n"
            "    -D<macro>[=<def>]\n"
            "         Define macro, with optional definition.\n\n"
            "    -U<macro>\n"
            "         Undefine macro.\n\n"
            "    -i\n"
            "         Do not generate an #include statement.\n\n"
            "    -p<path>\n"
            "         Path prefix for included file.\n\n"
            "    -f[<file>]\n"
            "         Force #include, optional file name.\n\n"
            "    -nn\n"
            "         Do not display notes.\n\n"
            "    -nw\n"
            "         Do not display warnings.\n\n"
            "    @<file>\n"
            "         Read additional options from file.\n\n"
            "    -v\n"
            "         Display version of moc and exit.\n");
    exit(1);
}

QByteArray composePreprocessorOutput(const Symbols &symbols) {
    QByteArray output;
    int lineNum = 1;
    Token last = PP_NOTOKEN;
    Token secondlast = last;
    int i = 0;
    while (i < symbols.size()) {
        Symbol sym = symbols.at(i++);
        switch (sym.token) {
        case PP_NEWLINE:
        case PP_WHITESPACE:
            if (last != PP_WHITESPACE) {
                secondlast = last;
                last = PP_WHITESPACE;
                output += ' ';
            }
            continue;
        case PP_STRING_LITERAL:
            if (last == PP_STRING_LITERAL)
                output.chop(1);
            else if (secondlast == PP_STRING_LITERAL && last == PP_WHITESPACE)
                output.chop(2);
            else
                break;
            output += sym.lexem().mid(1);
            secondlast = last;
            last = PP_STRING_LITERAL;
            continue;
        case MOC_INCLUDE_BEGIN:
            lineNum = 0;
            continue;
        case MOC_INCLUDE_END:
            lineNum = sym.lineNum;
            continue;
        default:
            break;
        }
        secondlast = last;
        last = sym.token;

        const int padding = sym.lineNum - lineNum;
        if (padding > 0) {
            output.resize(output.size() + padding);
            memset(output.data() + output.size() - padding, '\n', padding);
            lineNum = sym.lineNum;
        }

        output += sym.lexem();
    }

    return output;
}


int runMoc(int _argc, char **_argv)
{
    bool autoInclude = true;
    Preprocessor pp;
    Moc moc;
    pp.macros["Q_MOC_RUN"];
    pp.macros["__cplusplus"];

    // Workaround a bug while parsing the boost/type_traits/has_operator.hpp header. See QTBUG-22829
    pp.macros["BOOST_TT_HAS_OPERATOR_HPP_INCLUDED"];

    QByteArray filename;
    QByteArray output;
    FILE *in = 0;
    FILE *out = 0;
    bool ignoreConflictingOptions = false;

    QVector<QByteArray> argv;
    argv.resize(_argc - 1);
    for (int n = 1; n < _argc; ++n)
        argv[n - 1] = _argv[n];

    for (int n = 0; n < argv.count(); ++n) {
        if (argv.at(n).startsWith('@')) {
            QByteArray optionsFile = argv.at(n);
            optionsFile.remove(0, 1);
            if (optionsFile.isEmpty())
                error("The @ option requires an input file");
            QFile f(QString::fromLatin1(optionsFile.constData()));
            if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
                error("Cannot open options file specified with @");
            argv.remove(n);
            while (!f.atEnd()) {
                QByteArray line = f.readLine().trimmed();
                if (!line.isEmpty())
                    argv.insert(n++, line);
            }
        }
    }

    int argc = argv.count();

    for (int n = 0; n < argc; ++n) {
        QByteArray arg(argv[n]);
        if (arg[0] != '-') {
            if (filename.isEmpty()) {
                filename = arg;
                continue;
            }
            error("Too many input files specified");
        }
        QByteArray opt = arg.mid(1);
        bool more = (opt.size() > 1);
        switch (opt[0]) {
        case 'o': // output redirection
            if (!more) {
                if (!(n < argc-1))
                    error("Missing output file name");
                output = argv[++n];
            } else
                output = opt.mid(1);
            break;
        case 'E': // only preprocessor
            pp.preprocessOnly = true;
            break;
        case 'i': // no #include statement
            if (more)
                error();
            moc.noInclude        = true;
            autoInclude = false;
            break;
        case 'f': // produce #include statement
            if (ignoreConflictingOptions)
                break;
            moc.noInclude        = false;
            autoInclude = false;
            if (opt[1])                        // -fsomething.h
                moc.includeFiles.append(opt.mid(1));
            break;
        case 'p': // include file path
            if (ignoreConflictingOptions)
                break;
            if (!more) {
                if (!(n < argc-1))
                    error("Missing path name for the -p option.");
                moc.includePath = argv[++n];
            } else {
                moc.includePath = opt.mid(1);
            }
            break;
        case 'I': // produce #include statement
            if (!more) {
                if (!(n < argc-1))
                    error("Missing path name for the -I option.");
                pp.includes += IncludePath(argv[++n]);
            } else {
                pp.includes += IncludePath(opt.mid(1));
            }
            break;
        case 'D': // define macro
            {
                QByteArray name;
                QByteArray value("1");
                if (!more) {
                    if (n < argc-1)
                        name = argv[++n];
                } else
                    name = opt.mid(1);
                int eq = name.indexOf('=');
                if (eq >= 0) {
                    value = name.mid(eq + 1);
                    name = name.left(eq);
                }
                if (name.isEmpty())
                    error("Missing macro name");
                Macro macro;
                macro.symbols += Symbol(0, PP_IDENTIFIER, value);
                pp.macros.insert(name, macro);

            }
            break;
        case 'U':
            {
                QByteArray macro;
                if (!more) {
                    if (n < argc-1)
                        macro = argv[++n];
                } else
                    macro = opt.mid(1);
                if (macro.isEmpty())
                    error("Missing macro name");
                pp.macros.remove(macro);

            }
            break;
        case 'v':  // version number
            if (more && opt != "version")
                error();
            fprintf(stderr, "Qt Meta Object Compiler version %d (Qt %s)\n",
                    Q_MOC_OUTPUT_REVISION, QT_VERSION_STR);
            return 1;
        case 'n': // don't display warnings
            if (ignoreConflictingOptions)
                break;
            if (opt == "nw")
                moc.displayWarnings = moc.displayNotes = false;
            else if (opt == "nn")
                moc.displayNotes = false;
            else
                error();
            break;
        case 'h': // help
            if (more && opt != "help")
                error();
            else
                error(0); // 0 means usage only
            break;
        case '-':
            if (more && arg == "--ignore-option-clashes") {
                // -- ignore all following moc specific options that conflict
                // with for example gcc, like -pthread conflicting with moc's
                // -p option.
                ignoreConflictingOptions = true;
                break;
            }
            // fall through
        default:
            error();
        }
    }


    if (autoInclude) {
        int spos = filename.lastIndexOf(QDir::separator().toLatin1());
        int ppos = filename.lastIndexOf('.');
        // spos >= -1 && ppos > spos => ppos >= 0
        moc.noInclude = (ppos > spos && tolower(filename[ppos + 1]) != 'h');
    }
    if (moc.includeFiles.isEmpty()) {
        if (moc.includePath.isEmpty()) {
            if (filename.size()) {
                if (output.size())
                    moc.includeFiles.append(combinePath(filename.constData(), output.data()));
                else
                    moc.includeFiles.append(filename);
            }
        } else {
            moc.includeFiles.append(combinePath(filename.constData(), filename.data()));
        }
    }

    if (filename.isEmpty()) {
        filename = "standard input";
        in = stdin;
    } else {
        in = QT_FOPEN(filename.data(), "rb");
        if (!in) {
            fprintf(stderr, "moc: %s: No such file\n", filename.constData());
            return 1;
        }
        moc.filename = filename;
    }

    moc.currentFilenames.push(filename);

    // 1. preprocess
    moc.symbols = pp.preprocessed(moc.filename, in);
    ::fclose(in);

    if (!pp.preprocessOnly) {
        // 2. parse
        moc.parse();
    }

    // 3. and output meta object code

    if (output.size()) { // output file specified
        out = QT_FOPEN(output.data(), "w"); // create output file
        if (!out)
        {
            fprintf(stderr, "moc: Cannot create %s\n", output.constData());
            return 1;
        }
    } else { // use stdout
        out = stdout;
    }

    if (pp.preprocessOnly) {
        fprintf(out, "%s\n", composePreprocessorOutput(moc.symbols).constData());
    } else {
        if (moc.classList.isEmpty())
            moc.note("No relevant classes found. No output generated.");
        else
            moc.generate(out);
    }

    if (output.size())
        ::fclose(out);

    return 0;
}

QT_END_NAMESPACE

int main(int _argc, char **_argv)
{
    return QT_PREPEND_NAMESPACE(runMoc)(_argc, _argv);
}
