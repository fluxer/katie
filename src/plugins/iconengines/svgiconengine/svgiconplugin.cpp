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
#include "qsvgiconengine.h"
#include "qiodevice.h"
#include "qbytearray.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

class QSvgIconPlugin : public QIconEnginePlugin
{
public:
    QIconEngine *create(const QString &filename = QString()) final;
};

QIconEngine *QSvgIconPlugin::create(const QString &file)
{
    QSvgIconEngine *engine = new QSvgIconEngine;
    if (!file.isNull())
        engine->addFile(file, QSize(), QIcon::Normal, QIcon::Off);
    return engine;
}

Q_EXPORT_PLUGIN(QSvgIconPlugin)

QT_END_NAMESPACE
