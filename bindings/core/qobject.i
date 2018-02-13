%{
#include "QtCore/qobject.h"
QT_USE_NAMESPACE
%}

#ifndef QT_NO_QOBJECT

class QObject
{
/*
    Q_OBJECT
    Q_PROPERTY(QString objectName READ objectName WRITE setObjectName)
*/
public:
    Q_INVOKABLE explicit QObject(QObject *parent = Q_NULLPTR);
    virtual ~QObject();

    virtual bool event(QEvent *);
    virtual bool eventFilter(QObject *, QEvent *);

#ifdef QT_NO_TRANSLATION
    static QString tr(const char *sourceText, const char * = Q_NULLPTR, int = -1);
#ifndef QT_NO_TEXTCODEC
    static QString trUtf8(const char *sourceText, const char * = Q_NULLPTR, int = -1);
#endif
#endif //QT_NO_TRANSLATION

    QString objectName() const;
    void setObjectName(const QString &name);

    bool isWidgetType() const;

    bool signalsBlocked() const;
    bool blockSignals(bool b);

    QThread *thread() const;
    void moveToThread(QThread *thread);

    int startTimer(int interval);
    void killTimer(int id);

    template<typename T>
    findChild(const QString &aName = QString()) const;

    template<typename T>
    QList<T> findChildren(const QString &aName = QString()) const;

#ifndef QT_NO_REGEXP
    template<typename T>
    QList<T> findChildren(const QRegExp &re) const;
#endif

    const QObjectList &children() const;

    void setParent(QObject *);
    void installEventFilter(QObject *);
    void removeEventFilter(QObject *);


    static bool connect(const QObject *sender, const char *signal,
                        const QObject *receiver, const char *member,
                        Qt::ConnectionType type = Qt::AutoConnection
        );

    static bool connect(const QObject *sender, const QMetaMethod &signal,
                        const QObject *receiver, const QMetaMethod &method,
                        Qt::ConnectionType type = Qt::AutoConnection
        );

    bool connect(const QObject *sender, const char *signal,
                 const char *member,
                 Qt::ConnectionType type = Qt::AutoConnection
        ) const;


    static bool disconnect(const QObject *sender, const char *signal,
                           const QObject *receiver, const char *member);
    static bool disconnect(const QObject *sender, const QMetaMethod &signal,
                           const QObject *receiver, const QMetaMethod &member);
    bool disconnect(const char *signal = Q_NULLPTR,
                    const QObject *receiver = Q_NULLPTR, const char *member = Q_NULLPTR);
    bool disconnect(const QObject *receiver, const char *member = Q_NULLPTR);

    void dumpObjectTree();
    void dumpObjectInfo();

#ifndef QT_NO_PROPERTIES
    bool setProperty(const char *name, const QVariant &value);
    QVariant property(const char *name) const;
    QList<QByteArray> dynamicPropertyNames() const;
#endif // QT_NO_PROPERTIES

Q_SIGNALS:
    void destroyed(QObject * = Q_NULLPTR);

public:
    QObject *parent() const;

    bool inherits(const char *classname) const;

public Q_SLOTS:
    void deleteLater();

protected:
    QObject *sender() const;
    int senderSignalIndex() const;
    int receivers(const char* signal) const;

    virtual void timerEvent(QTimerEvent *);
    virtual void childEvent(QChildEvent *);
    virtual void customEvent(QEvent *);

    virtual void connectNotify(const char *signal);
    virtual void disconnectNotify(const char *signal);

    QObject(QObjectPrivate &dd, QObject *parent = Q_NULLPTR);

    QObjectData* d_ptr;

    static const QMetaObject staticQtMetaObject;
};

/* TODO
template <class T>
T qobject_cast(QObject *object);

template <class T>
T qobject_cast(const QObject *object);

template <class T> const char * qobject_interface_iid()
{ return Q_NULLPTR; }

#ifndef Q_MOC_RUN
#  define Q_DECLARE_INTERFACE(IFace, IId) \
    QT_BEGIN_NAMESPACE \
    template <> const char *qobject_interface_iid<IFace *>() \
    { return IId; } \
    template <> IFace *qobject_cast<IFace *>(QObject *object) \
    { return reinterpret_cast<IFace *>((object ? object->qt_metacast(IId) : Q_NULLPTR)); } \
    template <> IFace *qobject_cast<IFace *>(const QObject *object) \
    { return reinterpret_cast<IFace *>((object ? const_cast<QObject *>(object)->qt_metacast(IId) : Q_NULLPTR)); } \
    QT_END_NAMESPACE
#endif // Q_MOC_RUN
*/

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const QObject *);
#endif

#endif // QT_NO_QOBJECT


void qt_qFindChildren_helper(const QObject *parent, const QString &name, const QRegExp *re,
                                           const QMetaObject &mo, QList<void *> *list);
QObject *qt_qFindChild_helper(const QObject *parent, const QString &name, const QMetaObject &mo);
