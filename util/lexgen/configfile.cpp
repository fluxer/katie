/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
#include "configfile.h"

#include <QFile>

QT_BEGIN_NAMESPACE

ConfigFile::SectionMap ConfigFile::parse(const QString &fileName)
{
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly))
        return ConfigFile::SectionMap();
    return parse(&f);
}

ConfigFile::SectionMap ConfigFile::parse(QIODevice *dev)
{
    SectionMap sections;
    SectionMap::Iterator currentSection = sections.end();

    ConfigFile::SectionMap result;
    int currentLineNumber = 0;
    while (!dev->atEnd()) {
        QString line = QString::fromUtf8(dev->readLine()).trimmed();
        ++currentLineNumber;

        if (line.isEmpty() || line.startsWith(QLatin1Char('#')))
            continue;

        if (line.startsWith(QLatin1Char('['))) {
            if (!line.endsWith(QLatin1Char('['))) {
                qWarning("Syntax error at line %d: Missing ']' at start of new section.", currentLineNumber);
                return SectionMap();
            }
            line.remove(0, 1);
            line.chop(1);
            const QString sectionName = line;
            currentSection = sections.insert(sectionName, Section());
            continue;
        }

        if (currentSection == sections.end()) {
            qWarning("Syntax error at line %d: Entry found outside of any section.", currentLineNumber);
            return SectionMap();
        }

        Entry e;
        e.lineNumber = currentLineNumber;

        int equalPos = line.indexOf(QLatin1Char('='));
        if (equalPos == -1) {
            e.key = line;
        } else {
            e.key = line;
            e.key.truncate(equalPos);
            e.key = e.key.trimmed();
            e.value = line.mid(equalPos + 1).trimmed();
        }
        currentSection->append(e);
    }
    return sections;
}

QT_END_NAMESPACE
