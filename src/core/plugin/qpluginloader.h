/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
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

#ifndef QPLUGINLOADER_H
#define QPLUGINLOADER_H

#include <QtCore/qlibrary.h>

#ifndef QT_NO_LIBRARY


QT_BEGIN_NAMESPACE

class Q_CORE_EXPORT QPluginLoader : public QLibrary
{
    Q_OBJECT
public:
    explicit QPluginLoader(QObject *parent = nullptr);
    explicit QPluginLoader(const QString &fileName, QObject *parent = nullptr);
    ~QPluginLoader();

    QObject *instance();

private:
    Q_DISABLE_COPY(QPluginLoader)
};

QT_END_NAMESPACE


#endif // QT_NO_LIBRARY

#endif //QPLUGINLOADER_H
