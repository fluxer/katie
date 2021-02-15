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

#ifndef QFUTRUESYNCHRONIZER_H
#define QFUTRUESYNCHRONIZER_H

#include <QtCore/qfuture.h>

#ifndef QT_NO_CONCURRENT

QT_BEGIN_NAMESPACE


template <typename T>
class QFutureSynchronizer
{
    Q_DISABLE_COPY(QFutureSynchronizer)

public:
    QFutureSynchronizer() : m_cancelOnWait(false) { }
    explicit QFutureSynchronizer(const QFuture<T> &future)
        : m_cancelOnWait(false)
    { addFuture(future); }
    ~QFutureSynchronizer()  { waitForFinished(); }

    void setFuture(const QFuture<T> &future)
    {
        waitForFinished();
        m_futures.clear();
        addFuture(future);
    }

    void addFuture(const QFuture<T> &future)
    {
        m_futures.append(future);
    }

    void waitForFinished()
    {
        if (m_cancelOnWait) {
            foreach (QFuture<T> future, m_futures) {
                 future.cancel();
            }
        }

        foreach (QFuture<T> future, m_futures) {
             future.waitForFinished();
         }
    }

    void clearFutures()
    {
        m_futures.clear();
    }

    QList<QFuture<T> > futures() const
    {
        return m_futures;
    }

    void setCancelOnWait(bool enabled)
    {
        m_cancelOnWait = enabled;
    }

    bool cancelOnWait() const
    {
        return m_cancelOnWait;
    }

private:
    QList<QFuture<T> > m_futures;
    bool m_cancelOnWait;
};

QT_END_NAMESPACE

#endif // QT_NO_CONCURRENT

#endif // QFUTRUESYNCHRONIZER_H
