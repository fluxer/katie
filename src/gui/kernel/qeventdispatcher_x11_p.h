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

#ifndef QEVENTDISPATCHER_X11_P_H
#define QEVENTDISPATCHER_X11_P_H

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

#include "qeventdispatcher_unix_p.h"

QT_BEGIN_NAMESPACE

class QEventDispatcherX11Private;

class QEventDispatcherX11 : public QEventDispatcherUNIX
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QEventDispatcherX11)

public:
    explicit QEventDispatcherX11(QObject *parent = nullptr);
    ~QEventDispatcherX11();

    bool processEvents(QEventLoop::ProcessEventsFlags flags);
    bool hasPendingEvents();

    void flush();

protected:
    int select(QEventDispatcherPollSet &pollset, int timeout) final;
};

QT_END_NAMESPACE

#endif // QEVENTDISPATCHER_X11_P_H
