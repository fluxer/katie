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

#ifndef QMOTIFSTYLE_P_H
#define QMOTIFSTYLE_P_H
#include "qlist.h"
#include "qdatetime.h"
#include "qprogressbar.h"
#include "qmotifstyle.h"
#include "qcommonstyle_p.h"

QT_BEGIN_NAMESPACE

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

// Private class
class QMotifStylePrivate : public QCommonStylePrivate
{
    Q_DECLARE_PUBLIC(QMotifStyle)
public:
    explicit QMotifStylePrivate(bool useHighlightCols = false);
    ~QMotifStylePrivate();

public:
    QPointer<QFocusFrame> focus;
    bool highlightCols;

#ifndef QT_NO_PROGRESSBAR
    QList<QProgressBar *> bars;
    int animationFps;
    int animateTimer;
    QTime startTime;
    int animateStep;
#endif // QT_NO_PROGRESSBAR
};

QT_END_NAMESPACE

#endif //QMOTIFSTYLE_P_H
