/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtTest module of the Katie Toolkit.
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

#ifndef QABSTRACTTESTLOGGER_P_H
#define QABSTRACTTESTLOGGER_P_H

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

#include "qglobal.h"

#include <cstdlib>

QT_BEGIN_NAMESPACE

class QBenchmarkResult;

class QAbstractTestLogger
{
public:
    enum IncidentTypes {
        Pass,
        XFail,
        Fail,
        XPass
    };

    enum MessageTypes {
        Warn,
        QWarning,
        QDebug,
        QCritical,
        QFatal,
        Skip,
        Info
    };

    QAbstractTestLogger() {}
    virtual ~QAbstractTestLogger() {}

    virtual void startLogging();
    virtual void stopLogging();

    virtual void enterTestFunction(const char *function) = 0;
    virtual void leaveTestFunction() = 0;

    virtual void addIncident(IncidentTypes type, const char *description,
                             const char *file = 0, int line = 0) = 0;
    virtual void addBenchmarkResult(const QBenchmarkResult &result) = 0;

    virtual void addMessage(MessageTypes type, const char *message,
                            const char *file = 0, int line = 0) = 0;

    virtual void registerRandomSeed(unsigned int seed) = 0;

    static void outputString(const char *msg);
    static bool isTtyOutput();
};

struct QTestCharBuffer
{
    enum { InitialSize = 512 };

    inline QTestCharBuffer()
            : _size(InitialSize), buf(staticBuf)
    {
        staticBuf[0] = '\0';
    }

    inline ~QTestCharBuffer()
    {
        if (buf != staticBuf)
            free(buf);
    }

    inline char *data()
    {
        return buf;
    }

    inline char **buffer()
    {
        return &buf;
    }

    inline const char* constData() const
    {
        return buf;
    }

    inline int size() const
    {
        return _size;
    }

    inline bool reset(int newSize)
    {
        char *newBuf = 0;
        if (buf == staticBuf) {
            // if we point to our internal buffer, we need to malloc first
            newBuf = static_cast<char *>(::malloc(newSize));
        } else {
            // if we already malloc'ed, just realloc
            newBuf = static_cast<char *>(::realloc(buf, newSize));
        }

        // if the allocation went wrong (newBuf == 0), we leave the object as is
        if (!newBuf)
            return false;

        _size = newSize;
        buf = newBuf;
        return true;
    }

private:
    int _size;
    char* buf;
    char staticBuf[InitialSize];
};

namespace QTest
{
    int qt_asprintf(QTestCharBuffer *buf, const char *format, ...);
}


QT_END_NAMESPACE

#endif
