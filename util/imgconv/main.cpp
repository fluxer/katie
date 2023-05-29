/****************************************************************************
**
** Copyright (C) 2022 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qfileinfo.h>
#include <QtGui/qimagereader.h>

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const QStringList args = app.arguments().mid(1);
    if (args.size() != 2) {
        qWarning() << "Usage: imgconv <input> <output>";
        return 1;
    }

    const QString inputpath(args.at(0));
    QImageReader imagereader(inputpath);
    QImage inputimage = imagereader.read();
    if (inputimage.isNull()) {
        qWarning() << "Cannot not read" << inputpath << ":" << imagereader.errorString();
        return 2;
    }

    const QString outputpath(args.at(1));
    const QFileInfo outputinfo(outputpath);
    QByteArray outputformat(outputinfo.suffix().toLocal8Bit());
    if (outputpath.endsWith(".svg.gz")) {
        // the only exception I am aware of
        outputformat = "svgz";
    }
    if (outputformat.isEmpty()) {
        qWarning() << "Could not determine format for" << outputpath;
        return 3;
    }
    const bool outputraw32 = (outputformat == "raw32");
    const bool outputraw16 = (outputformat == "raw16");
    if (outputraw32 || outputraw16) {
        qWarning() << "Use" << outputpath << "with caution, you have been warned!";
        if (outputraw32) {
            inputimage = inputimage.convertToFormat(QImage::Format_ARGB32);
        } else {
            inputimage = inputimage.convertToFormat(QImage::Format_RGB16);
        }
        QFile outputfile(outputpath);
        if (!outputfile.open(QFile::WriteOnly)) {
            qWarning() << "Could not open" << outputpath << outputfile.errorString();
            return 4;
        }
        QDataStream outputstream(&outputfile);
        outputstream << inputimage;
        if (outputstream.status() != QDataStream::Ok) {
            qWarning() << "Could not stream" << outputpath;
            return 5;
        }
        return 0;
    }

    if (inputimage.save(outputpath, outputformat, 100) == false) {
        qWarning() << "Could not save" << outputpath;
        return 6;
    }

    return 0;
}

