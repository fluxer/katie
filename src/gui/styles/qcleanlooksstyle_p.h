/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QCLEANLOOKSSTYLE_P_H
#define QCLEANLOOKSSTYLE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp and qfiledialog.cpp.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#include "qwindowsstyle.h"
#include "qwindowsstyle_p.h"

#ifndef QT_NO_STYLE_CLEANLOOKS

QT_BEGIN_NAMESPACE

class QCleanlooksStylePrivate : public QWindowsStylePrivate
{
    Q_DECLARE_PUBLIC(QCleanlooksStyle)
public:
    QCleanlooksStylePrivate()
        : QWindowsStylePrivate() {
        animationFps = 24;
    }

    ~QCleanlooksStylePrivate() {
    }
};

QT_END_NAMESPACE

#endif // QT_NO_STYLE_CLEANLOOKS

#endif //QCLEANLOOKSSTYLE_P_H
