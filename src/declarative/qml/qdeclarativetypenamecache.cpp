/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeclarativetypenamecache_p.h"

#include "qdeclarativeengine_p.h"

QT_BEGIN_NAMESPACE

QDeclarativeTypeNameCache::QDeclarativeTypeNameCache(QDeclarativeEngine *e)
: QDeclarativeCleanup(e), engine(e)
{
}

QDeclarativeTypeNameCache::~QDeclarativeTypeNameCache()
{
    clear();
}

void QDeclarativeTypeNameCache::clear()
{
    qDeleteAll(stringCache);
    stringCache.clear();
    identifierCache.clear();
    engine = 0;
}

void QDeclarativeTypeNameCache::add(const QString &name, int importedScriptIndex)
{
    if (stringCache.contains(name))
        return;

    QDeclarativeEnginePrivate *ep = QDeclarativeEnginePrivate::get(engine);

    RData *data = new RData;
    // ### Use typename class
    data->identifier = ep->objectClass->createPersistentIdentifier(name);
    data->importedScriptIndex = importedScriptIndex;
    stringCache.insert(name, data);
    identifierCache.insert(data->identifier.identifier, data);
}

void QDeclarativeTypeNameCache::add(const QString &name, QDeclarativeType *type)
{
    if (stringCache.contains(name))
        return;

    QDeclarativeEnginePrivate *ep = QDeclarativeEnginePrivate::get(engine);

    RData *data = new RData;
    // ### Use typename class
    data->identifier = ep->objectClass->createPersistentIdentifier(name);
    data->type = type;
    stringCache.insert(name, data);
    identifierCache.insert(data->identifier.identifier, data);
}

void QDeclarativeTypeNameCache::add(const QString &name, QDeclarativeTypeNameCache *typeNamespace)
{
    if (stringCache.contains(name))
        return;

    QDeclarativeEnginePrivate *ep = QDeclarativeEnginePrivate::get(engine);

    RData *data = new RData;
    // ### Use typename class
    data->identifier = ep->objectClass->createPersistentIdentifier(name);
    data->typeNamespace = typeNamespace;
    stringCache.insert(name, data);
    identifierCache.insert(data->identifier.identifier, data);
    typeNamespace->addref();
}

QDeclarativeTypeNameCache::Data *QDeclarativeTypeNameCache::data(const QString &id) const
{
    return stringCache.value(id);
}

QT_END_NAMESPACE

