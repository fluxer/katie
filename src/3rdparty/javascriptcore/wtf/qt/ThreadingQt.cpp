/*
 * Copyright (C) 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Justin Haygood (jhaygood@reaktix.com)
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
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
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
#include "config.h"
#include "Threading.h"

#ifdef QT_NO_THREAD

namespace WTF {

void initializeThreading() { }
inline ThreadIdentifier createThreadInternal(ThreadFunction, void*, const char*) { return ThreadIdentifier(); }
int waitForThreadCompletion(ThreadIdentifier, void**) { return 0; }
void detachThread(ThreadIdentifier) { }
inline ThreadIdentifier currentThread() { return ThreadIdentifier(); }

void lockAtomicallyInitializedStaticMutex() { }
void unlockAtomicallyInitializedStaticMutex() { }

} // namespace WTF

#else // QT_NO_THREAD

#include "CurrentTime.h"
#include "HashMap.h"

namespace WTF {

QT_USE_NAMESPACE

class ThreadPrivate : public QThread {
public:
    ThreadPrivate(ThreadFunction entryPoint, void* data);
    void run();
    void* getReturnValue() { return m_returnValue; }
private:
    void* m_data;
    ThreadFunction m_entryPoint;
    void* m_returnValue;
};

ThreadPrivate::ThreadPrivate(ThreadFunction entryPoint, void* data) 
    : m_data(data)
    , m_entryPoint(entryPoint)
    , m_returnValue(0)
{
}

void ThreadPrivate::run()
{
    m_returnValue = m_entryPoint(m_data);
}

class ThreadMonitor : public QObject {
    Q_OBJECT
public:
    static ThreadMonitor * instance()
    {
        static ThreadMonitor *instance = new ThreadMonitor();
        return instance;
    }

public Q_SLOTS:
    void threadFinished()
    {
        sender()->deleteLater();
    }
};

static Mutex* atomicallyInitializedStaticMutex;

static ThreadIdentifier mainThreadIdentifier;

static Mutex* threadMapMutex()
{
    static Mutex* mutex;
    if (!mutex)
        mutex = new Mutex;
    return mutex;
}

static HashMap<ThreadIdentifier, QThread*>& threadMap()
{
    static HashMap<ThreadIdentifier, QThread*> map;
    return map;
}

static ThreadIdentifier identifierByQthreadHandle(QThread*& thread)
{
    QMutexLocker locker(threadMapMutex());

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

    QMutexLocker locker(threadMapMutex());

    static ThreadIdentifier identifierCount = 1;

    threadMap().add(identifierCount, thread);

    return identifierCount++;
}

static void clearThreadForIdentifier(ThreadIdentifier id)
{
    QMutexLocker locker(threadMapMutex());

    Q_ASSERT(threadMap().contains(id));

    threadMap().remove(id);
}

static QThread* threadForIdentifier(ThreadIdentifier id)
{
    QMutexLocker locker(threadMapMutex());

    return threadMap().get(id);
}

void initializeThreading()
{
    if (!atomicallyInitializedStaticMutex) {
        atomicallyInitializedStaticMutex = new Mutex;
        threadMapMutex();
        QThread* mainThread = QCoreApplication::instance()->thread();
        mainThreadIdentifier = identifierByQthreadHandle(mainThread);
        if (!mainThreadIdentifier)
            mainThreadIdentifier = establishIdentifierForThread(mainThread);
    }
}

void lockAtomicallyInitializedStaticMutex()
{
    Q_ASSERT(atomicallyInitializedStaticMutex);
    atomicallyInitializedStaticMutex->lock();
}

void unlockAtomicallyInitializedStaticMutex()
{
    atomicallyInitializedStaticMutex->unlock();
}

ThreadIdentifier createThreadInternal(ThreadFunction entryPoint, void* data, const char*)
{
    ThreadPrivate* thread = new ThreadPrivate(entryPoint, data);
    if (!thread) {
        qCritical("Failed to create thread at entry point %p with data %p", entryPoint, data);
        return 0;
    }

    QObject::connect(thread, SIGNAL(finished()), ThreadMonitor::instance(), SLOT(threadFinished()));

    thread->start();

    QThread* threadRef = static_cast<QThread*>(thread);

    return establishIdentifierForThread(threadRef);
}

int waitForThreadCompletion(ThreadIdentifier threadID, void** result)
{
    Q_ASSERT(threadID);

    QThread* thread = threadForIdentifier(threadID);

    bool res = thread->wait();

    clearThreadForIdentifier(threadID);
    if (result)
        *result = static_cast<ThreadPrivate*>(thread)->getReturnValue();

    return !res;
}

void detachThread(ThreadIdentifier threadID)
{
    Q_ASSERT(threadID);
    clearThreadForIdentifier(threadID);
}

ThreadIdentifier currentThread()
{
    QThread* currentThread = QThread::currentThread();
    if (ThreadIdentifier id = identifierByQthreadHandle(currentThread))
        return id;
    return establishIdentifierForThread(currentThread);
}

} // namespace WTF

#include "moc_ThreadingQt.cpp"

#endif
