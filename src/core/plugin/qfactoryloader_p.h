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

#ifndef QFACTORYLOADER_P_H
#define QFACTORYLOADER_P_H

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

#include "QtCore/qobject.h"
#include "QtCore/qstringlist.h"

#ifndef QT_NO_LIBRARY

QT_BEGIN_NAMESPACE

class QFactoryLoaderPrivate;

class Q_CORE_EXPORT QFactoryLoader
{
public:
    QFactoryLoader(const QString &suffix);
    ~QFactoryLoader();

    QStringList keys() const;
    QObject *instance(const QString &key);

    void update();

    static void refreshAll();

private:
    QFactoryLoaderPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QFactoryLoader)
};

QT_END_NAMESPACE

#endif // QT_NO_LIBRARY

#endif // QFACTORYLOADER_P_H
