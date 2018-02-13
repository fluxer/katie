%{
#include "QtCore/qcoreapplication.h"
QT_USE_NAMESPACE
%}

class QCoreApplication : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString applicationName READ applicationName WRITE setApplicationName)
    Q_PROPERTY(QString applicationVersion READ applicationVersion WRITE setApplicationVersion)
    Q_PROPERTY(QString organizationName READ organizationName WRITE setOrganizationName)
    Q_PROPERTY(QString organizationDomain READ organizationDomain WRITE setOrganizationDomain)

public:
    QCoreApplication(int &argc, char **argv);

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

    static QCoreApplication *instance();

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
                             const char * disambiguation = Q_NULLPTR,
                             Encoding encoding = CodecForTr,
                             int n = -1);

    static void flush();

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

    QCoreApplication(QCoreApplicationPrivate &p);
};

#ifdef QT_NO_TRANSLATION
// Simple versions
QString QCoreApplication::translate(const char *, const char *sourceText,
                                    const char *, Encoding encoding = CodecForTr, int = -1);
#endif

typedef void (*QtCleanUpFunction)();

void qAddPostRoutine(QtCleanUpFunction);
void qRemovePostRoutine(QtCleanUpFunction);
