/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEPRIVATE_H
#define QDECLARATIVEPRIVATE_H

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

#include <QtCore/qglobal.h>
#include <QtCore/qvariant.h>
#include <QtCore/qurl.h>
#include <QtDeclarative/qdeclarativelist.h>
#include <QtDeclarative/QDeclarativeParserStatus>
#include <QtDeclarative/QDeclarativePropertyValueSource>
#include <QtDeclarative/QDeclarativePropertyValueInterceptor>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


typedef QObject *(*QDeclarativeAttachedPropertiesFunc)(QObject *);

template <typename TYPE>
class QDeclarativeTypeInfo
{
public:
    enum {
        hasAttachedProperties = 0
    };
};


class QDeclarativeCustomParser;
namespace QDeclarativePrivate
{
    void Q_DECLARATIVE_EXPORT qdeclarativeelement_destructor(QObject *);
    template<typename T>
    class QDeclarativeElement : public T
    {
    public:
        virtual ~QDeclarativeElement() { 
            QDeclarativePrivate::qdeclarativeelement_destructor(this); 
        }
    };

    template<typename T>
    void createInto(void *memory) { new (memory) QDeclarativeElement<T>; }

    template<typename T>
    QObject *createParent(QObject *p) { return new T(p); }

    template<class From, class To, int N>
    struct StaticCastSelectorClass
    {
        static inline int cast() { return -1; }
    };

    template<class From, class To>
    struct StaticCastSelectorClass<From, To, sizeof(int)>
    {
        static inline int cast() { return int(reinterpret_cast<quintptr>(static_cast<To *>(reinterpret_cast<From *>(0x10000000)))) - 0x10000000; }
    };

    template<class From, class To>
    struct StaticCastSelector
    {
        typedef int yes_type;
        typedef char no_type;

        static yes_type check(To *);
        static no_type check(...);

        static inline int cast()
        {
            return StaticCastSelectorClass<From, To, sizeof(check(reinterpret_cast<From *>(0)))>::cast();
        }
    };

    template <typename T>
    struct has_attachedPropertiesMember
    {
        static bool const value = QDeclarativeTypeInfo<T>::hasAttachedProperties;
    };

    template <typename T, bool hasMember>
    class has_attachedPropertiesMethod 
    {
    public:
        typedef int yes_type;
        typedef char no_type;

        template<typename ReturnType>
        static yes_type check(ReturnType *(*)(QObject *));
        static no_type check(...);

        static bool const value = sizeof(check(&T::qmlAttachedProperties)) == sizeof(yes_type);
    }; 

    template <typename T>
    class has_attachedPropertiesMethod<T, false>
    {
    public:
        static bool const value = false;
    };

    template<typename T, int N>
    class AttachedPropertySelector
    {
    public:
        static inline QDeclarativeAttachedPropertiesFunc func() { return 0; }
        static inline const QMetaObject *metaObject() { return 0; }
    };
    template<typename T>
    class AttachedPropertySelector<T, 1>
    {
        static inline QObject *attachedProperties(QObject *obj) {
            return T::qmlAttachedProperties(obj);
        }
        template<typename ReturnType>
        static inline const QMetaObject *attachedPropertiesMetaObject(ReturnType *(*)(QObject *)) {
            return &ReturnType::staticMetaObject;
        }
    public:
        static inline QDeclarativeAttachedPropertiesFunc func() {
            return &attachedProperties;
        }
        static inline const QMetaObject *metaObject() {
            return attachedPropertiesMetaObject(&T::qmlAttachedProperties);
        }
    };

    template<typename T>
    inline QDeclarativeAttachedPropertiesFunc attachedPropertiesFunc()
    {
        return AttachedPropertySelector<T, has_attachedPropertiesMethod<T, has_attachedPropertiesMember<T>::value>::value>::func();
    }

    template<typename T>
    inline const QMetaObject *attachedPropertiesMetaObject()
    {
        return AttachedPropertySelector<T, has_attachedPropertiesMethod<T, has_attachedPropertiesMember<T>::value>::value>::metaObject();
    }

    enum AutoParentResult { Parented, IncompatibleObject, IncompatibleParent };
    typedef AutoParentResult (*AutoParentFunction)(QObject *object, QObject *parent);

    struct RegisterType {
        int version;

        int typeId;
        int listId;
        int objectSize;
        void (*create)(void *);
        QString noCreationReason;

        const char *uri;
        int versionMajor;
        int versionMinor;
        const char *elementName;
        const QMetaObject *metaObject;

        QDeclarativeAttachedPropertiesFunc attachedPropertiesFunction;
        const QMetaObject *attachedPropertiesMetaObject;

        int parserStatusCast;
        int valueSourceCast;
        int valueInterceptorCast;

        QObject *(*extensionObjectCreate)(QObject *);
        const QMetaObject *extensionMetaObject;

        QDeclarativeCustomParser *customParser;
        int revision;
        // If this is extended ensure "version" is bumped!!!
    };

    struct RegisterInterface {
        int version;

        int typeId;
        int listId;

        const char *iid;
    };

    struct RegisterAutoParent {
        int version;

        AutoParentFunction function;
    };

    struct RegisterComponent {
        const QUrl &url;
        const char *uri;
        const char *typeName;
        int majorVersion;
        int minorVersion;
    };

    enum RegistrationType {
        TypeRegistration       = 0, 
        InterfaceRegistration  = 1,
        AutoParentRegistration = 2,
        ComponentRegistration  = 3
    };

    int Q_DECLARATIVE_EXPORT qmlregister(RegistrationType, void *);


    /*!
      \internal
      \fn int qmlRegisterType(const char *url, const char *uri, int versionMajor, int versionMinor, const char *qmlName);
      \relates QDeclarativeEngine

      This function registers a type in the QML system with the name \a qmlName, in the library imported from \a uri having the
      version number composed from \a versionMajor and \a versionMinor. The type is defined by the QML file located at \a url.

      Normally QML files can be loaded as types directly from other QML files, or using a qmldir file. This function allows
      registration of files to types from a C++ module, such as when the type mapping needs to be procedurally determined at startup.

      Returns non-zero if the registration was sucessful.

      This function is added to QtQuick 1 in Qt 5, and is here as private API for developers needing compatibility.
    */
    inline int qmlRegisterType(const QUrl &url, const char *uri, int versionMajor, int versionMinor, const char *qmlName)
    {
        RegisterComponent type = {
            url,
            uri,
            qmlName,
            versionMajor,
            versionMinor
        };

        return qmlregister(QDeclarativePrivate::ComponentRegistration, &type);
    }
    /*!
      \internal
      \fn int qmlRegisterUncreatableType(const char *url, const char *uri, int versionMajor, int versionMinor, const char *qmlName);
      \relates QDeclarativeEngine

      This overload is backported from Qt5, and allows uncreatable types to be versioned.
  */
    template<typename T, int metaObjectRevision>
    int qmlRegisterUncreatableType(const char *uri, int versionMajor, int versionMinor, const char *qmlName, const QString& reason)
    {
        QByteArray name(T::staticMetaObject.className());

        QByteArray pointerName(name + '*');
        QByteArray listName("QDeclarativeListProperty<" + name + ">");

        QDeclarativePrivate::RegisterType type = {
            1,

            qRegisterMetaType<T *>(pointerName.constData()),
            qRegisterMetaType<QDeclarativeListProperty<T> >(listName.constData()),
            0, 0,
            reason,

            uri, versionMajor, versionMinor, qmlName, &T::staticMetaObject,

            QDeclarativePrivate::attachedPropertiesFunc<T>(),
            QDeclarativePrivate::attachedPropertiesMetaObject<T>(),

            QDeclarativePrivate::StaticCastSelector<T,QDeclarativeParserStatus>::cast(),
            QDeclarativePrivate::StaticCastSelector<T,QDeclarativePropertyValueSource>::cast(),
            QDeclarativePrivate::StaticCastSelector<T,QDeclarativePropertyValueInterceptor>::cast(),

            0, 0,

            0,
            metaObjectRevision
        };

        return QDeclarativePrivate::qmlregister(QDeclarativePrivate::TypeRegistration, &type);
    }


}

QT_END_NAMESPACE

QT_END_HEADER

#endif // QDECLARATIVEPRIVATE_H
