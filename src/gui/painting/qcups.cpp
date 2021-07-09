/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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
#include "qdebug.h"
#include "qcups_p.h"
#include "qcorecommon_p.h"

#ifndef QT_NO_CUPS

#include "qtextcodec.h"

QT_BEGIN_NAMESPACE

static bool cupsLoaded = false;
static int qt_cups_num_printers = 0;

static inline void resolveCups()
{
    cups_dest_t *printers;
    int num_printers = cupsGetDests(&printers);
    if (num_printers)
        cupsFreeDests(num_printers, printers);
    qt_cups_num_printers = num_printers;
    cupsLoaded = true;
}

// ================ CUPS Support class ========================

QCUPSSupport::QCUPSSupport()
    :
    prnCount(0),
    printers(0),
    page_sizes(0),
    currPrinterIndex(0),
    currPPD(0)
{
    if (!cupsLoaded)
        resolveCups();

    // getting all available printers
    if (!isAvailable())
        return;

    // Update the available printer count
    qt_cups_num_printers = prnCount = cupsGetDests(&printers);

    for (int i = 0; i <  prnCount; ++i) {
        if (printers[i].is_default) {
            currPrinterIndex = i;
            setCurrentPrinter(i);
            break;
        }
    }

#ifndef QT_NO_TEXTCODEC
    cups_lang_t *cupsLang = cupsLangGet(0);
    codec = QTextCodec::codecForName(cupsLangEncoding(cupsLang));
    if (!codec)
        codec = QTextCodec::codecForLocale();
#endif
}

QCUPSSupport::~QCUPSSupport()
{
     if (currPPD)
        ppdClose(currPPD);
     if (prnCount)
         cupsFreeDests(prnCount, printers);
}

int QCUPSSupport::availablePrintersCount() const
{
    return prnCount;
}

const cups_dest_t* QCUPSSupport::availablePrinters() const
{
    return printers;
}

const ppd_file_t* QCUPSSupport::currentPPD() const
{
    return currPPD;
}

const ppd_file_t* QCUPSSupport::setCurrentPrinter(int index)
{
    Q_ASSERT(index >= 0 && index <= prnCount);
    if (index == prnCount)
        return 0;

    currPrinterIndex = index;

    if (currPPD)
        ppdClose(currPPD);
    currPPD = 0;
    page_sizes = 0;

    const char *ppdFile = cupsGetPPD(printers[index].name);

    if (!ppdFile)
      return 0;

    currPPD = ppdOpenFile(ppdFile);
    unlink(ppdFile);

    // marking default options
    ppdMarkDefaults(currPPD);

    // marking options explicitly set
    cupsMarkOptions(currPPD, printers[currPrinterIndex].num_options, printers[currPrinterIndex].options);

    // getting pointer to page sizes
    page_sizes = ppdOption("PageSize");

    return currPPD;
}

int QCUPSSupport::currentPrinterIndex() const
{
    return currPrinterIndex;
}

bool QCUPSSupport::isAvailable()
{
    if(!cupsLoaded)
        resolveCups();

    return (qt_cups_num_printers > 0);
}

const ppd_option_t* QCUPSSupport::ppdOption(const char *key) const
{
    if (currPPD) {
        for (int gr = 0; gr < currPPD->num_groups; ++gr) {
            for (int opt = 0; opt < currPPD->groups[gr].num_options; ++opt) {
                if (qstrcmp(currPPD->groups[gr].options[opt].keyword, key) == 0)
                    return &currPPD->groups[gr].options[opt];
            }
        }
    }
    return 0;
}

const cups_option_t* QCUPSSupport::printerOption(const QString &key) const
{
    for (int i = 0; i < printers[currPrinterIndex].num_options; ++i) {
        if (QLatin1String(printers[currPrinterIndex].options[i].name) == key)
            return &printers[currPrinterIndex].options[i];
    }
    return 0;
}

const ppd_option_t* QCUPSSupport::pageSizes() const
{
    return page_sizes;
}

int QCUPSSupport::markOption(const char* name, const char* value)
{
    return ppdMarkOption(currPPD, name, value);
}

void QCUPSSupport::saveOptions(QList<const ppd_option_t*> options, QList<const char*> markedOptions)
{
    int oldOptionCount = printers[currPrinterIndex].num_options;
    cups_option_t* oldOptions = printers[currPrinterIndex].options;

    int newOptionCount = 0;
    cups_option_t* newOptions = 0;

    // copying old options that are not on the new list
    for (int i = 0; i < oldOptionCount; ++i) {
        bool contains = false;
        for (int j = 0; j < options.count(); ++j) {
            if (qstrcmp(options.at(j)->keyword, oldOptions[i].name) == 0) {
                contains = true;
                break;
            }
        }

        if (!contains) {
            newOptionCount = cupsAddOption(oldOptions[i].name, oldOptions[i].value, newOptionCount, &newOptions);
        }
    }

    // we can release old option list
     cupsFreeOptions(oldOptionCount, oldOptions);

    // adding marked options
    for (int i = 0; i < markedOptions.count(); ++i) {
        const char* name = markedOptions.at(i);
        ++i;
        newOptionCount = cupsAddOption(name, markedOptions.at(i), newOptionCount, &newOptions);
    }

    // placing the new option list
    printers[currPrinterIndex].num_options = newOptionCount;
    printers[currPrinterIndex].options = newOptions;

    // saving new default values
    cupsSetDests(prnCount, printers);
}

QRect QCUPSSupport::paperRect(const char *choice) const
{
    if (!currPPD)
        return QRect();
    for (int i = 0; i < currPPD->num_sizes; ++i) {
        if (qstrcmp(currPPD->sizes[i].name, choice) == 0)
            return QRect(0, 0, qRound(currPPD->sizes[i].width), qRound(currPPD->sizes[i].length));
    }
    return QRect();
}

QRect QCUPSSupport::pageRect(const char *choice) const
{
    if (!currPPD)
        return QRect();
    for (int i = 0; i < currPPD->num_sizes; ++i) {
        if (qstrcmp(currPPD->sizes[i].name, choice) == 0)
            return QRect(qRound(currPPD->sizes[i].left),
                         qRound(currPPD->sizes[i].length - currPPD->sizes[i].top),
                         qRound(currPPD->sizes[i].right - currPPD->sizes[i].left),
                         qRound(currPPD->sizes[i].top - currPPD->sizes[i].bottom));
    }
    return QRect();
}

QStringList QCUPSSupport::options() const
{
    QStringList list;
    collectMarkedOptions(list);
    return list;
}

bool QCUPSSupport::printerHasPPD(const char *printerName)
{
    if (!isAvailable())
        return false;
    const char *ppdFile = cupsGetPPD(printerName);
    if (ppdFile)
        unlink(ppdFile);
    return (ppdFile != 0);
}

QString QCUPSSupport::unicodeString(const char *s)
{
#ifndef QT_NO_TEXTCODEC
    return codec->toUnicode(s);
#else
    return QLatin1String(s);
#endif
}

void QCUPSSupport::collectMarkedOptions(QStringList& list, const ppd_group_t* group) const
{
    if (group == 0) {
        if (!currPPD)
            return;
        for (int i = 0; i < currPPD->num_groups; ++i) {
            collectMarkedOptions(list, &currPPD->groups[i]);
            collectMarkedOptionsHelper(list, &currPPD->groups[i]);
        }
    } else {
        for (int i = 0; i < group->num_subgroups; ++i)
            collectMarkedOptionsHelper(list, &group->subgroups[i]);
    }
}

void QCUPSSupport::collectMarkedOptionsHelper(QStringList& list, const ppd_group_t* group) const
{
    for (int i = 0; i < group->num_options; ++i) {
        for (int j = 0; j < group->options[i].num_choices; ++j) {
            if (group->options[i].choices[j].marked == 1 && qstrcmp(group->options[i].choices[j].choice, group->options[i].defchoice) != 0)
                list << QString::fromLocal8Bit(group->options[i].keyword) << QString::fromLocal8Bit(group->options[i].choices[j].choice);
        }
    }
}

QPair<int, QString> QCUPSSupport::tempFd()
{
    QSTACKARRAY(char, filename, 512);
    int fd = cupsTempFd(filename, sizeof(filename));
    return QPair<int, QString>(fd, QString::fromLocal8Bit(filename));
}

// Prints the given file and returns a job id.
int QCUPSSupport::printFile(const char * printerName, const char * filename, const char * title,
                            int num_options, cups_option_t * options)
{
    return cupsPrintFile(printerName, filename, title, num_options, options);
}

QT_END_NAMESPACE

#endif // QT_NO_CUPS




