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

#include <QFile>
#include <QDebug>

#include <gettext-po.h>

QT_BEGIN_NAMESPACE

static void gettext_xerror(int severity,
                           po_message_t message,
                           const char *filename, size_t lineno, size_t column,
                           int multiline_p, const char *message_text)
{
    switch (severity) {
        case PO_SEVERITY_WARNING: {
            qWarning("%s", message_text);
            break;
        }
        case PO_SEVERITY_ERROR: {
            qCritical("%s", message_text);
            break;
        }
        case PO_SEVERITY_FATAL_ERROR: {
            qFatal("%s", message_text);
            break;
        }
        default: {
            Q_ASSERT(false);
            break;
        }
    }
}

static void gettext_xerror2(int severity,
                            po_message_t message1,
                            const char *filename1, size_t lineno1, size_t column1,
                            int multiline_p1, const char *message_text1,
                            po_message_t message2,
                            const char *filename2, size_t lineno2, size_t column2,
                            int multiline_p2, const char *message_text2)
{
    switch (severity) {
        case PO_SEVERITY_WARNING: {
            qWarning("%s, %s", message_text1, message_text2);
            break;
        }
        case PO_SEVERITY_ERROR: {
            qCritical("%s, %s", message_text1, message_text2);
            break;
        }
        case PO_SEVERITY_FATAL_ERROR: {
            qFatal("%s, %s", message_text1, message_text2);
            break;
        }
        default: {
            Q_ASSERT(false);
            break;
        }
    }
}

static const struct po_xerror_handler gettext_handler
{
    gettext_xerror,
    gettext_xerror2
};

void showHelp()
{
    fprintf(stderr, "Usage:\n"
            "    trc [options] <translation-file>\n\n"
            "Options:\n"
            "    -f\n"
            "    -fuzzy\n"
            "         Process fuzzy translations.\n\n"
            "    -b\n"
            "    -obsolete\n"
            "         Process obsolete translations.\n\n"
            "    -h\n"
            "    -help\n"
            "         Display this help and exit.\n\n"
            "    -v\n"
            "    -version\n"
            "         Display version.\n\n"
            "    -o <file>\n"
            "         Place the output into <file>.\n");
}

int runTrc(int argc, char *argv[])
{
    QByteArray inputfilepath;
    QString outputfilepath;

    int arg = 1;
    bool fuzzy = false;
    bool obsolete = false;
    while (arg < argc) {
        const QString opt = QString::fromLocal8Bit(argv[arg]);
        if (opt == QLatin1String("-h") || opt == QLatin1String("-help")) {
            showHelp();
            return 0;
        } else if (opt == QLatin1String("-v") || opt == QLatin1String("-version")) {
            fprintf(stderr, "Katie Translation Compiler version %s\n", QT_VERSION_STR);
            return 0;
        } else if (opt == QLatin1String("-f") || opt == QLatin1String("-fuzzy")) {
            fuzzy = true;
        } else if (opt == QLatin1String("-b") || opt == QLatin1String("-obsolete")) {
            obsolete = true;
        } else if (opt == QLatin1String("-o") || opt == QLatin1String("-output")) {
            ++arg;
            if (!argv[arg]) {
                showHelp();
                return 1;
            }
            outputfilepath = QFile::decodeName(argv[arg]);
        } else if (inputfilepath.isEmpty()) {
            inputfilepath = argv[arg];
        } else {
            showHelp();
            return 1;
        }

        ++arg;
    }

    if (outputfilepath.isEmpty()) {
        outputfilepath = inputfilepath;
        if (outputfilepath.endsWith(QLatin1String(".po"))) {
            outputfilepath.chop(3);
        }
        outputfilepath.append(QLatin1String(".tr"));
    }

    if (inputfilepath.isEmpty()) {
        showHelp();
        return 1;
    }

    QFile outputfile(outputfilepath);
    if (!outputfile.open(QFile::WriteOnly)) {
        fprintf(stderr, "trc: Coult not open output %s\n", qPrintable(outputfilepath));
        return 2;
    }

    po_file_t gettext_file = po_file_read(
        inputfilepath.constData(),
        &gettext_handler
    );
    if (gettext_file == NULL) {
        fprintf(stderr, "trc: Coult not open input %s\n", inputfilepath.constData());
        return 3;
    }

    const char* gettext_domain_header = po_file_domain_header(gettext_file, NULL);

    QByteArray trdata;
    QDataStream trdatastream(&trdata, QIODevice::WriteOnly);
    po_message_iterator_t gettext_iterator = po_message_iterator(gettext_file, NULL);
    po_message_t gettext_message = po_next_message(gettext_iterator);
    while (gettext_message != NULL) {
        if (po_message_is_obsolete(gettext_message) && !obsolete) {
            gettext_message = po_next_message(gettext_iterator);
            continue;
        }
        if (po_message_is_fuzzy(gettext_message) && !fuzzy) {
            gettext_message = po_next_message(gettext_iterator);
            continue;
        }

        const QByteArray po_msgctxt = po_message_msgctxt(gettext_message);
        const QByteArray po_msgid = po_message_msgid(gettext_message);
        const QByteArray po_msgstr = po_message_msgstr(gettext_message);
        if (po_msgstr == gettext_domain_header) {
            gettext_message = po_next_message(gettext_iterator);
            continue;
        }
        const QByteArray po_msgid_plural = po_message_msgid_plural(gettext_message);
        const QByteArray po_msgstr_plural = po_message_msgstr_plural(gettext_message, 1);

        trdatastream << po_msgctxt;
        trdatastream << po_msgid;
        trdatastream << po_msgstr;
        trdatastream << po_msgid_plural;
        trdatastream << po_msgstr_plural;

        gettext_message = po_next_message(gettext_iterator);
    }
    po_message_iterator_free(gettext_iterator);
    po_file_free(gettext_file);

    QByteArray trheaderdata;
    QDataStream trheaderstream(&trheaderdata, QIODevice::WriteOnly);
    trheaderstream << QByteArray("KATIE_TRANSLATION");
    // both Katie and Katana assume that the strings are UTF-8
    trheaderstream << QByteArray("UTF-8");

    if (outputfile.write(trheaderdata.constData(), trheaderdata.size()) != trheaderdata.size()) {
        fprintf(stderr, "trc: Coult not write header %s\n", qPrintable(outputfile.errorString()));
        return 4;
    }

    if (outputfile.write(trdata.constData(), trdata.size()) != trdata.size()) {
        fprintf(stderr, "trc: Coult not write data %s\n", qPrintable(outputfile.errorString()));
        return 5;
    }

    return 0;
}

QT_END_NAMESPACE

int main(int argc, char *argv[])
{
    return QT_PREPEND_NAMESPACE(runTrc)(argc, argv);
}
