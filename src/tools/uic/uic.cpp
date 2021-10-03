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

#include "uic.h"
#include "ui4.h"
#include "driver.h"
#include "option.h"
#include "treewalker.h"
#include "cppwriteincludes.h"
#include "cppwritedeclaration.h"

#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtXml/qxmlstream.h>

QT_BEGIN_NAMESPACE

Uic::Uic(Driver *d)
     : drv(d),
       out(d->output()),
       opt(d->option()),
       externalPix(true)
{
}

Uic::~Uic()
{
}

bool Uic::printDependencies()
{
    QString fileName = opt.inputFile;

    QFile f;
    if (fileName.isEmpty())
        f.open(stdin, QIODevice::ReadOnly);
    else {
        f.setFileName(fileName);
        if (!f.open(QIODevice::ReadOnly))
            return false;
    }

    DomUI *ui = 0;
    {
        QXmlStreamReader reader;
        reader.setDevice(&f);
        ui = parseUiFile(reader);
        if (!ui)
            return false;
    }

    if (DomIncludes *includes = ui->elementIncludes()) {
        foreach (DomInclude *incl, includes->elementInclude()) {
            QString file = incl->text();
            if (file.isEmpty())
                continue;

            fprintf(stdout, "%s\n", file.toLocal8Bit().constData());
        }
    }

    if (DomCustomWidgets *customWidgets = ui->elementCustomWidgets()) {
        foreach (DomCustomWidget *customWidget, customWidgets->elementCustomWidget()) {
            if (DomHeader *header = customWidget->elementHeader()) {
                QString file = header->text();
                if (file.isEmpty())
                    continue;

                fprintf(stdout, "%s\n", file.toLocal8Bit().constData());
            }
        }
    }

    delete ui;

    return true;
}

void Uic::writeCopyrightHeader(DomUI *ui)
{
    QString comment = ui->elementComment();
    if (comment.size())
        out << "/*\n" << comment << "\n*/\n\n";

    out << "/********************************************************************************\n";
    out << "** Form generated from reading UI file '" << QFileInfo(opt.inputFile).fileName() << "'\n";
    out << "**\n";
    out << "** Created by: Qt User Interface Compiler version " << QLatin1String(QT_VERSION_STR) << "\n";
    out << "**\n";
    out << "** WARNING! All changes made in this file will be lost when recompiling UI file!\n";
    out << "********************************************************************************/\n\n";
}

// Check the version with a stream reader at the <ui> element.

static double versionFromUiAttribute(QXmlStreamReader &reader)
{
    const QXmlStreamAttributes attributes = reader.attributes();
    const QString versionAttribute = QLatin1String("version");
    if (!attributes.hasAttribute(versionAttribute))
        return 4.0;
    const QString version = attributes.value(versionAttribute).toString();
    return version.toDouble();
}

DomUI *Uic::parseUiFile(QXmlStreamReader &reader)
{
    DomUI *ui = 0;

    const QString uiElement = QLatin1String("ui");
    while (!reader.atEnd()) {
        if (reader.readNext() == QXmlStreamReader::StartElement) {
            if (reader.name().compare(uiElement, Qt::CaseInsensitive) == 0
                && !ui) {
                const double version = versionFromUiAttribute(reader);
                if (version < 4.0) {
                    const QString msg = QString::fromLatin1("uic: File generated with too old version of Katie Designer (%1)").arg(version);
                    fprintf(stderr, "%s\n", qPrintable(msg));
                    return 0;
                }

                ui = new DomUI();
                ui->read(reader);
            } else {
                reader.raiseError(QLatin1String("Unexpected element ") + reader.name().toString());
            }
        }
    }
    if (reader.hasError()) {
        delete ui;
        ui = 0;
        fprintf(stderr, "%s\n", qPrintable(QString::fromLatin1("uic: Error in line %1, column %2 : %3")
                                    .arg(reader.lineNumber()).arg(reader.columnNumber())
                                    .arg(reader.errorString())));
    }

    return ui;
}

bool Uic::write(QIODevice *in)
{
    DomUI *ui = 0;
    {
        QXmlStreamReader reader;
        reader.setDevice(in);
        ui = parseUiFile(reader);

        if (!ui)
            return false;
    }

    double version = ui->attributeVersion().toDouble();
    if (version < 4.0) {
        delete ui;

        fprintf(stderr, "uic: File generated with too old version of Katie Designer\n");
        return false;
    }

    QString language = ui->attributeLanguage();

    if (!language.isEmpty() && language.toLower() != QLatin1String("c++")) {
        fprintf(stderr, "uic: File is not a 'c++' ui file, language=%s\n", qPrintable(language));
        return false;
    }

    bool rtn = write (ui);

    delete ui;

    return rtn;
}

bool Uic::write(DomUI *ui)
{
    using namespace CPP;

    if (!ui || !ui->elementWidget())
        return false;

    if (opt.copyrightHeader)
        writeCopyrightHeader(ui);

    if (opt.headerProtection) {
        writeHeaderProtectionStart();
        out << "\n";
    }

    pixFunction = ui->elementPixmapFunction();
    if (pixFunction == QLatin1String("QPixmap::fromMimeSource"))
        pixFunction = QLatin1String("qPixmapFromMimeSource");

    externalPix = ui->elementImages() == 0;

    cWidgetsInfo.acceptUI(ui);
    WriteIncludes writeIncludes(this);
    writeIncludes.acceptUI(ui);

    WriteDeclaration(this, writeIncludes.scriptsActivated()).acceptUI(ui);

    if (opt.headerProtection)
        writeHeaderProtectionEnd();

    return true;
}

void Uic::writeHeaderProtectionStart()
{
    QString h = drv->headerFileName();
    out << "#ifndef " << h << "\n"
        << "#define " << h << "\n";
}

void Uic::writeHeaderProtectionEnd()
{
    QString h = drv->headerFileName();
    out << "#endif // " << h << "\n";
}

bool Uic::isMainWindow(const QString &className) const
{
    return customWidgetsInfo()->extends(className, QLatin1String("QMainWindow"));
}

bool Uic::isToolBar(const QString &className) const
{
    return customWidgetsInfo()->extends(className, QLatin1String("QToolBar"));
}

bool Uic::isButton(const QString &className) const
{
    return customWidgetsInfo()->extends(className, QLatin1String("QRadioButton"))
        || customWidgetsInfo()->extends(className, QLatin1String("QToolButton"))
        || customWidgetsInfo()->extends(className, QLatin1String("QCheckBox"))
        || customWidgetsInfo()->extends(className, QLatin1String("QPushButton"))
        || customWidgetsInfo()->extends(className, QLatin1String("QCommandLinkButton"));
}

bool Uic::isContainer(const QString &className) const
{
    return customWidgetsInfo()->extends(className, QLatin1String("QStackedWidget"))
        || customWidgetsInfo()->extends(className, QLatin1String("QToolBox"))
        || customWidgetsInfo()->extends(className, QLatin1String("QTabWidget"))
        || customWidgetsInfo()->extends(className, QLatin1String("QScrollArea"))
        || customWidgetsInfo()->extends(className, QLatin1String("QMdiArea"))
        || customWidgetsInfo()->extends(className, QLatin1String("QWizard"))
        || customWidgetsInfo()->extends(className, QLatin1String("QDockWidget"));
}

bool Uic::isCustomWidgetContainer(const QString &className) const
{
    return customWidgetsInfo()->isCustomWidgetContainer(className);
}

bool Uic::isStatusBar(const QString &className) const
{
    return customWidgetsInfo()->extends(className, QLatin1String("QStatusBar"));
}

bool Uic::isMenuBar(const QString &className) const
{
    return customWidgetsInfo()->extends(className, QLatin1String("QMenuBar"));
}

bool Uic::isMenu(const QString &className) const
{
    return customWidgetsInfo()->extends(className, QLatin1String("QMenu"))
        || customWidgetsInfo()->extends(className, QLatin1String("QPopupMenu"));
}

QT_END_NAMESPACE
