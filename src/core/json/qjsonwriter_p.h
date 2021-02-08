/****************************************************************************
**
** Copyright (c) 2012-2015 Barbara Geller
** Copyright (c) 2012-2015 Ansel Sermersheim
** Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
** Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QJSONWRITER_P_H
#define QJSONWRITER_P_H

#include "qjsonvalue.h"

QT_BEGIN_NAMESPACE

namespace QJsonPrivate {

class Writer
{
 public:
   static void objectToJson(const QJsonPrivate::Object *o, QByteArray &json, int indent, bool compact = false);
   static void arrayToJson(const QJsonPrivate::Array *a, QByteArray &json, int indent, bool compact = false);
};

}

QT_END_NAMESPACE

#endif
