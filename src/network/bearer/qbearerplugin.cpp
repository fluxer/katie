/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtNetwork module of the Katie Toolkit.
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

#include "qbearerplugin_p.h"

#ifndef QT_NO_BEARERMANAGEMENT

QT_BEGIN_NAMESPACE

QBearerEnginePlugin::QBearerEnginePlugin(QObject *parent)
    : QObject(parent)
{
}

QBearerEnginePlugin::~QBearerEnginePlugin()
{
}

QT_END_NAMESPACE

#endif // QT_NO_BEARERMANAGEMENT



#include "moc_qbearerplugin_p.h"
