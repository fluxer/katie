/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the plugins of the Katie Toolkit.
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

#include "qiconengineplugin.h"
#include "qstringlist.h"

#if !defined(QT_NO_SVG)

#include "qsvgiconengine.h"
#include "qiodevice.h"
#include "qbytearray.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

class QSvgIconPlugin : public QIconEnginePlugin
{
public:
    QStringList keys() const;
    QIconEngine *create(const QString &filename = QString());
};

QStringList QSvgIconPlugin::keys() const
{
    static const QStringList list = QStringList()
        << QLatin1String("svg")
        << QLatin1String("svgz")
        << QLatin1String("svg.gz");
    return list;
}

QIconEngine *QSvgIconPlugin::create(const QString &file)
{
    QSvgIconEngine *engine = new QSvgIconEngine;
    if (!file.isNull())
        engine->addFile(file, QSize(), QIcon::Normal, QIcon::Off);
    return engine;
}

Q_EXPORT_PLUGIN2(qsvgicon, QSvgIconPlugin)

QT_END_NAMESPACE

#endif // !QT_NO_SVG
