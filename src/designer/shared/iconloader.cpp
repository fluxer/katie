/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#include "iconloader_p.h"

#include <QtCore/QFile>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

Q_DESIGNER_EXPORT QIcon createIconSet(const QString &name)
{
    const QStringList candidates = QStringList()
        << (QLatin1String(":/trolltech/formeditor/images/") + name)
        << (QLatin1String(":/trolltech/formeditor/images/win/") + name)
        << (QLatin1String(":/trolltech/formeditor/images/designer_") + name);

    foreach (const QString &f, candidates) {
        if (QFile::exists(f))
            return QIcon(f);
    }

    return QIcon();
}

Q_DESIGNER_EXPORT QIcon emptyIcon()
{
    return QIcon(QLatin1String(":/trolltech/formeditor/images/emptyicon.png"));
}

} // namespace qdesigner_internal

QT_END_NAMESPACE

