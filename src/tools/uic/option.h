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

#ifndef OPTION_H
#define OPTION_H

#include <QtCore/QString>
#include <QtCore/QDir>

QT_BEGIN_NAMESPACE

struct Option
{
    bool headerProtection;
    bool copyrightHeader;
    bool generateImplemetation;
    bool generateNamespace;
    bool autoConnection;
    bool dependencies;
    bool implicitIncludes;

    QString inputFile;
    QString outputFile;
    QString qrcOutputFile;
    QString indent;
    QString prefix;
    QString postfix;
    QString translateFunction;

    Option()
        : headerProtection(true),
          copyrightHeader(true),
          generateImplemetation(false),
          generateNamespace(true),
          autoConnection(true),
          dependencies(false),
          implicitIncludes(true),
          prefix(QLatin1String("Ui_"))
    { indent.fill(QLatin1Char(' '), 4); }

    QString messagePrefix() const
    {
        return inputFile.isEmpty() ?
               QString(QLatin1String("stdin")) : inputFile;
    }
};

QT_END_NAMESPACE

#endif // OPTION_H
