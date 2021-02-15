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

#ifndef QDIALOG_P_H
#define QDIALOG_P_H

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

#include "qwidget_p.h"
#include "QtCore/qeventloop.h"
#include "QtCore/qpointer.h"
#include "QtGui/qdialog.h"
#include "QtGui/qpushbutton.h"

QT_BEGIN_NAMESPACE

class QSizeGrip;

class QDialogPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QDialog)
public:

    QDialogPrivate()
        : mainDef(0),
#ifndef QT_NO_SIZEGRIP
          resizer(0),
          sizeGripEnabled(false),
#endif
          rescode(0), resetModalityTo(-1), wasModalitySet(true), eventLoop(0)
        {}

    QPointer<QPushButton> mainDef;
    QSize size, min, max;
#ifndef QT_NO_SIZEGRIP
    QSizeGrip *resizer;
    bool sizeGripEnabled;
#endif

    void setDefault(QPushButton *);
    void setMainDefault(QPushButton *);
    void hideDefault();
    void resetModalitySetByOpen();

    int rescode;
    int resetModalityTo;
    bool wasModalitySet;

    QPointer<QEventLoop> eventLoop;
};

QT_END_NAMESPACE

#endif // QDIALOG_P_H
