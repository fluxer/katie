#ifndef QDECLARATIVECOMMON_P_H
#define QDECLARATIVECOMMON_P_H

#include "qobject.h"
#include "qdeclarativeglobal_p.h"
#include "qdeclarative.h"
#include "qlistmodelinterface_p.h"
#include "qscriptvalue.h"

Q_DECLARE_METATYPE(QScriptValue)
QML_DECLARE_TYPE(QListModelInterface)

QT_BEGIN_NAMESPACE

struct StaticQtMetaObject : public QObject
{
    static const QMetaObject *get()
        { return &static_cast<StaticQtMetaObject*> (0)->staticQtMetaObject; }
};

inline static int QMetaObject_methods(const QMetaObject *metaObject)
{
    struct Private
    {
        int revision;
        int className;
        int classInfoCount, classInfoData;
        int methodCount, methodData;
        int propertyCount, propertyData;
    };

    return reinterpret_cast<const Private *>(metaObject->d.data)->methodCount;
}

inline static int qRoundDouble(const double d)
{
    return d >= double(0.0) ? int(d + double(0.5)) : int(d - int(d-1) + double(0.5)) + int(d-1);
}

DEFINE_BOOL_CONFIG_OPTION(stateChangeDebug, STATECHANGE_DEBUG)
DEFINE_BOOL_CONFIG_OPTION(bindingsDump, QML_BINDINGS_DUMP)

QT_END_NAMESPACE

#endif // QDECLARATIVECOMMON_P_H
