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

#include "qgenericengine.h"
#include "qbearerplugin_p.h"
#include "qdebug.h"

#ifndef QT_NO_BEARERMANAGEMENT

QT_BEGIN_NAMESPACE

class QGenericEnginePlugin : public QBearerEnginePlugin
{
public:
    QGenericEnginePlugin();
    ~QGenericEnginePlugin();

    QStringList keys() const;
    QBearerEngine *create(const QString &key) const;
};

QGenericEnginePlugin::QGenericEnginePlugin()
{
}

QGenericEnginePlugin::~QGenericEnginePlugin()
{
}

QStringList QGenericEnginePlugin::keys() const
{
    static const QStringList list = QStringList()
        << QLatin1String("generic");
    return list;
}

QBearerEngine *QGenericEnginePlugin::create(const QString &key) const
{
    if (key == QLatin1String("generic"))
        return new QGenericEngine;
    else
        return 0;
}

Q_EXPORT_PLUGIN2(qgenericbearer, QGenericEnginePlugin)

QT_END_NAMESPACE

#endif // QT_NO_BEARERMANAGEMENT
