/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVECUSTOMPARSER_P_H
#define QDECLARATIVECUSTOMPARSER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qdeclarativecustomparser_p.h"

#include "qdeclarativeparser_p.h"

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

class QDeclarativeCustomParserNodePrivate
{
public:
    QByteArray name;
    QList<QDeclarativeCustomParserProperty> properties;
    QDeclarativeParser::Location location;

    static QDeclarativeCustomParserNode fromObject(QDeclarativeParser::Object *);
    static QDeclarativeCustomParserProperty fromProperty(QDeclarativeParser::Property *);
};

class QDeclarativeCustomParserPropertyPrivate
{
public:
    QDeclarativeCustomParserPropertyPrivate()
        : isList(false) {}

    QByteArray name;
    bool isList;
    QDeclarativeParser::Location location;
    QList<QVariant> values;
};

QT_END_NAMESPACE

#endif // QDECLARATIVECUSTOMPARSER_P_H
