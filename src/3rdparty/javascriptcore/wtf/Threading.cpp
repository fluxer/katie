/*
 * Copyright (C) 2008, 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Platform.h"
#include "Threading.h"
#include "HashMap.h"

#include <string.h>

namespace WTF {

QT_USE_NAMESPACE

class ThreadPrivate : public QThread {
public:
    ThreadPrivate(ThreadFunction entryPoint, void* data);
    void run();
private:
    void* m_data;
    ThreadFunction m_entryPoint;
};

ThreadPrivate::ThreadPrivate(ThreadFunction entryPoint, void* data)
    : m_data(data)
    , m_entryPoint(entryPoint)
{
}

void ThreadPrivate::run()
{
    (void)m_entryPoint(m_data);
}

static bool threadInitialized = false;

static QMutex threadMapMutex;

static HashMap<ThreadIdentifier, QThread*>& threadMap()
{
    static HashMap<ThreadIdentifier, QThread*> map;
    return map;
}

static ThreadIdentifier identifierByQthreadHandle(QThread*& thread)
{
    QMutexLocker locker(&threadMapMutex);

    HashMap<ThreadIdentifier, QThread*>::iterator i = threadMap().begin();
    for (; i != threadMap().end(); ++i) {
        if (i->second == thread)
            return i->first;
    }

    return 0;
}

static ThreadIdentifier establishIdentifierForThread(QThread*& thread)
{
    Q_ASSERT(!identifierByQthreadHandle(thread));

    QMutexLocker locker(&threadMapMutex);

    static ThreadIdentifier identifierCount = 1;

    threadMap().add(identifierCount, thread);

    return identifierCount++;
}

void initializeThreading()
{
    if (!threadInitialized) {
        threadInitialized = true;
        QThread* mainThread = QCoreApplication::instance()->thread();
        (void)establishIdentifierForThread(mainThread);
    }
}

int waitForThreadCompletion(ThreadIdentifier threadID)
{
    Q_ASSERT(threadID);

    QMutexLocker locker(&threadMapMutex);
    QThread* thread = threadMap().get(threadID);
    locker.unlock();

    bool res = thread->wait();

    locker.relock();
    Q_ASSERT(threadMap().contains(threadID));
    threadMap().remove(threadID);

    return !res;
}

ThreadIdentifier currentThread()
{
    QThread* currentThread = QThread::currentThread();
    if (ThreadIdentifier id = identifierByQthreadHandle(currentThread))
        return id;
    return establishIdentifierForThread(currentThread);
}

struct NewThreadContext : FastAllocBase {
    NewThreadContext(ThreadFunction entryPoint, void* data, const char* name)
        : entryPoint(entryPoint)
        , data(data)
        , name(name)
        , creationMutex(new QMutex)
    {
    }

    ThreadFunction entryPoint;
    void* data;
    const char* name;

    QMutex* creationMutex;
};

ThreadIdentifier createThread(ThreadFunction entryPoint, void* data, const char* name)
{
    NewThreadContext* context = new NewThreadContext(entryPoint, data, name);

    // Prevent the thread body from executing until we've established the thread identifier.
    QMutexLocker locker(context->creationMutex);

    ThreadPrivate* thread = new ThreadPrivate(entryPoint, data);
    if (!thread) {
        qCritical("Failed to create thread at entry point %p with data %p", entryPoint, data);
        return 0;
    }

    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();

    QThread* threadRef = static_cast<QThread*>(thread);

    return establishIdentifierForThread(threadRef);
}

} // namespace WTF
