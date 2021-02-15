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

#include <QtDeclarative/qdeclarativeextensionplugin.h>
#include <QtDeclarative/qdeclarative.h>

#include "qdeclarativefolderlistmodel.h"

QT_BEGIN_NAMESPACE

class QmlFolderListModelPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Qt.labs.folderlistmodel"));
#ifndef QT_NO_DIRMODEL
        qmlRegisterType<QDeclarativeFolderListModel>(uri,1,0,"FolderListModel");
#endif
    }
};

QT_END_NAMESPACE

#include "moc_folderlistmodel.cpp"

Q_EXPORT_PLUGIN2(qmlfolderlistmodelplugin, QT_PREPEND_NAMESPACE(QmlFolderListModelPlugin))
