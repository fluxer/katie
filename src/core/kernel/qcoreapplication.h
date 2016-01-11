/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#ifndef QCOREAPPLICATION_H
#define QCOREAPPLICATION_H

#include <QtCore/qobject.h>
#include <QtCore/qcoreevent.h>
#include <QtCore/qeventloop.h>

#ifdef QT_INCLUDE_COMPAT
#include <QtCore/qstringlist.h>
#endif

#if defined(Q_WS_WIN) && !defined(tagMSG)
typedef struct tagMSG MSG;
#endif

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QCoreApplicationPrivate;
class QTextCodec;
class QTranslator;
class QPostEventList;
class QStringList;

#define qApp QCoreApplication::instance()

class Q_CORE_EXPORT QCoreApplication : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString applicationName READ applicationName WRITE setApplicationName)
    Q_PROPERTY(QString applicationVersion READ applicationVersion WRITE setApplicationVersion)
    Q_PROPERTY(QString organizationName READ organizationName WRITE setOrganizationName)
    Q_PROPERTY(QString organizationDomain READ organizationDomain WRITE setOrganizationDomain)

    Q_DECLARE_PRIVATE(QCoreApplication)
public:
    enum { ApplicationFlags = QT_VERSION
        | 0x01000000
    };

    QCoreApplication(int &argc, char **argv, int
#if !defined(QT_BUILD_CORE_LIB)
        = ApplicationFlags
#endif
        );

    ~QCoreApplication();

    static QStringList arguments();

    static void setAttribute(Qt::ApplicationAttribute attribute, bool on = true);
    static bool testAttribute(Qt::ApplicationAttribute attribute);

    static void setOrganizationDomain(const QString &orgDomain);
    static QString organizationDomain();
    static void setOrganizationName(const QString &orgName);
    static QString organizationName();
    static void setApplicationName(const QString &application);
    static QString applicationName();
    static void setApplicationVersion(const QString &version);
    static QString applicationVersion();

    static QCoreApplication *instance() { return self; }

    static int exec();
    static void processEvents(QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents);
    static void processEvents(QEventLoop::ProcessEventsFlags flags, int maxtime);
    static void exit(int retcode=0);

    static bool sendEvent(QObject *receiver, QEvent *event);
    static void postEvent(QObject *receiver, QEvent *event);
    static void postEvent(QObject *receiver, QEvent *event, int priority);
    static void sendPostedEvents(QObject *receiver, int event_type);
    static void sendPostedEvents();
    static void removePostedEvents(QObject *receiver);
    static void removePostedEvents(QObject *receiver, int eventType);
    static bool hasPendingEvents();

    virtual bool notify(QObject *, QEvent *);

    static bool startingUp();
    static bool closingDown();

    static QString applicationDirPath();
    static QString applicationFilePath();
    static qint64 applicationPid();

#ifndef QT_NO_LIBRARY
    static void setLibraryPaths(const QStringList &);
    static QStringList libraryPaths();
    static void addLibraryPath(const QString &);
    static void removeLibraryPath(const QString &);
#endif // QT_NO_LIBRARY

#ifndef QT_NO_TRANSLATION
    static void installTranslator(QTranslator * messageFile);
    static void removeTranslator(QTranslator * messageFile);
#endif
    enum Encoding { CodecForTr, UnicodeUTF8, DefaultCodec = CodecForTr };
    static QString translate(const char * context,
                             const char * sourceText,
                             const char * disambiguation = 0,
                             Encoding encoding = CodecForTr,
                             int n = -1);

    static void flush();


#if defined(Q_WS_WIN)
    virtual bool winEventFilter(MSG *message, long *result);
#endif

#if defined(Q_OS_UNIX)
    static void watchUnixSignal(int signal, bool watch);
#endif

    typedef bool (*EventFilter)(void *message, long *result);
    EventFilter setEventFilter(EventFilter filter);
    bool filterEvent(void *message, long *result);

public Q_SLOTS:
    static void quit();

Q_SIGNALS:
    void aboutToQuit();
    void unixSignal(int);

protected:
    bool event(QEvent *);

    virtual bool compressEvent(QEvent *, QObject *receiver, QPostEventList *);

protected:
    QCoreApplication(QCoreApplicationPrivate &p);

private:
    static bool sendSpontaneousEvent(QObject *receiver, QEvent *event);
    bool notifyInternal(QObject *receiver, QEvent *event);

    void init();

    static QCoreApplication *self;
    
    Q_DISABLE_COPY(QCoreApplication)

    friend class QEventDispatcherUNIXPrivate;
    friend class QApplication;
    friend class QApplicationPrivate;
    friend class QETWidget;
    friend class QShortcutMap;
    friend class QWidget;
    friend class QWidgetPrivate;
    friend bool qt_sendSpontaneousEvent(QObject*, QEvent*);
    friend Q_CORE_EXPORT QString qAppName();
    friend class QClassFactory;
};

inline bool QCoreApplication::sendEvent(QObject *receiver, QEvent *event)
{  if (event) event->spont = false; return self ? self->notifyInternal(receiver, event) : false; }

inline bool QCoreApplication::sendSpontaneousEvent(QObject *receiver, QEvent *event)
{ if (event) event->spont = true; return self ? self->notifyInternal(receiver, event) : false; }

inline void QCoreApplication::sendPostedEvents() { sendPostedEvents(0, 0); }

#ifdef QT_NO_TRANSLATION
// Simple versions
inline QString QCoreApplication::translate(const char *, const char *sourceText,
                                           const char *, Encoding encoding = CodecForTr, int = -1)
{
#ifndef QT_NO_TEXTCODEC
    if (encoding == UnicodeUTF8)
        return QString::fromUtf8(sourceText);
#else
    Q_UNUSED(encoding)
#endif
    return QString::fromLatin1(sourceText);
}
#endif

#define Q_DECLARE_TR_FUNCTIONS(context) \
public: \
    static inline QString tr(const char *sourceText, const char *disambiguation = 0, int n = -1) \
        { return QCoreApplication::translate(#context, sourceText, disambiguation, \
                                             QCoreApplication::CodecForTr, n); } \
    static inline QString trUtf8(const char *sourceText, const char *disambiguation = 0, int n = -1) \
        { return QCoreApplication::translate(#context, sourceText, disambiguation, \
                                             QCoreApplication::UnicodeUTF8, n); } \
private:

typedef void (*QtCleanUpFunction)();

Q_CORE_EXPORT void qAddPostRoutine(QtCleanUpFunction);
Q_CORE_EXPORT void qRemovePostRoutine(QtCleanUpFunction);
Q_CORE_EXPORT QString qAppName();                // get application name

#if defined(Q_WS_WIN) && !defined(QT_NO_DEBUG_STREAM)
Q_CORE_EXPORT QString decodeMSG(const MSG &);
Q_CORE_EXPORT QDebug operator<<(QDebug, const MSG &);
#endif

QT_END_NAMESPACE

QT_END_HEADER

#endif // QCOREAPPLICATION_H
