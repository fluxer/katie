/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#include "qcore_unix_p.h"
#include "qelapsedtimer.h"

#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>

QT_BEGIN_NAMESPACE

static inline bool time_update(struct timeval *tv, const struct timeval &start,
                               const struct timeval &timeout)
{
    // clock source is (hopefully) monotonic, so we can recalculate how much timeout is left;
    // if it isn't monotonic, we'll simply hope that it hasn't jumped, because we have no alternative
    struct timeval now = qt_gettime();
    *tv = timeout + start - now;
    return tv->tv_sec >= 0;
}

int qt_safe_select(int nfds, fd_set *fdread, fd_set *fdwrite, fd_set *fdexcept,
                   const struct timeval *orig_timeout)
{
    if (!orig_timeout) {
        // no timeout -> block forever
        int ret;
        EINTR_LOOP(ret, select(nfds, fdread, fdwrite, fdexcept, 0));
        return ret;
    }

    timeval start = qt_gettime();
    timeval timeout = *orig_timeout;

    // loop and recalculate the timeout as needed
    int ret;
    forever {
        ret = ::select(nfds, fdread, fdwrite, fdexcept, &timeout);
        if (ret != -1 || errno != EINTR)
            return ret;

        // recalculate the timeout
        if (!time_update(&timeout, start, *orig_timeout)) {
            // timeout during update
            // or clock reset, fake timeout error
            return 0;
        }
    }
}

QT_END_NAMESPACE
