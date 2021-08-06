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

#ifndef QCOREAPPLICATION_H
#define QCOREAPPLICATION_H

#include <QtCore/qcoreevent.h>
#include <QtCore/qeventloop.h>


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
    enum Type { Tty, Gui };

    QCoreApplication(int &argc, char **argv);

    ~QCoreApplication();

    static Type type();

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
    static QString translate(const char *context,
                             const char *sourceText,
                             Encoding encoding = CodecForTr);

    static void flush();

    typedef bool (*EventFilter)(void *message, long *result);
    EventFilter setEventFilter(EventFilter filter);
    bool filterEvent(void *message, long *result);

public Q_SLOTS:
    static void quit();

Q_SIGNALS:
    void aboutToQuit();

protected:
    bool event(QEvent *);
    bool compressEvent(QEvent *, QObject *receiver, QPostEventList *);

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
};

inline bool QCoreApplication::sendEvent(QObject *receiver, QEvent *event)
{  if (event) event->spont = false; return self ? self->notifyInternal(receiver, event) : false; }

inline bool QCoreApplication::sendSpontaneousEvent(QObject *receiver, QEvent *event)
{ if (event) event->spont = true; return self ? self->notifyInternal(receiver, event) : false; }

inline void QCoreApplication::sendPostedEvents() { sendPostedEvents(nullptr, 0); }

#ifdef QT_NO_TRANSLATION
// Simple versions
inline QString QCoreApplication::translate(const char *, const char *sourceText, Encoding encoding)
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
    static inline QString tr(const char *sourceText) \
        { return QCoreApplication::translate(#context, sourceText, QCoreApplication::CodecForTr); } \
    static inline QString trUtf8(const char *sourceText) \
        { return QCoreApplication::translate(#context, sourceText, QCoreApplication::UnicodeUTF8); } \
private:

typedef void (*QtCleanUpFunction)();

Q_CORE_EXPORT void qAddPostRoutine(QtCleanUpFunction);
Q_CORE_EXPORT void qRemovePostRoutine(QtCleanUpFunction);

QT_END_NAMESPACE


#endif // QCOREAPPLICATION_H
