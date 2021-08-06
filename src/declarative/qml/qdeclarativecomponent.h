/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVECOMPONENT_H
#define QDECLARATIVECOMPONENT_H

#include <QtDeclarative/qdeclarative.h>
#include <QtDeclarative/qdeclarativeerror.h>

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtScript/qscriptvalue.h>


QT_BEGIN_NAMESPACE


class QDeclarativeCompiledData;
class QByteArray;
class QDeclarativeComponentPrivate;
class QDeclarativeEngine;
class QDeclarativeComponentAttached;
class Q_DECLARATIVE_EXPORT QDeclarativeComponent : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeComponent)

    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QUrl url READ url CONSTANT)

public:
    QDeclarativeComponent(QObject *parent = nullptr);
    QDeclarativeComponent(QDeclarativeEngine *, QObject *parent = nullptr);
    QDeclarativeComponent(QDeclarativeEngine *, const QString &fileName, QObject *parent = nullptr);
    QDeclarativeComponent(QDeclarativeEngine *, const QUrl &url, QObject *parent = nullptr);
    virtual ~QDeclarativeComponent();

    Q_ENUMS(Status)
    enum Status { Null, Ready, Loading, Error };
    Status status() const;

    inline bool isNull() const
        { return status() == Null; }
    inline bool isReady() const
        { return status() == Ready; }
    inline bool isError() const
        { return status() == Error; }
    inline bool isLoading() const
        { return status() == Loading; }

    QList<QDeclarativeError> errors() const;
    Q_INVOKABLE QString errorString() const;

    qreal progress() const;

    QUrl url() const;

    virtual QObject *create(QDeclarativeContext *context = 0);
    virtual QObject *beginCreate(QDeclarativeContext *);
    virtual void completeCreate();

    void loadUrl(const QUrl &url);
    void setData(const QByteArray &, const QUrl &baseUrl);

    QDeclarativeContext *creationContext() const;

    static QDeclarativeComponentAttached *qmlAttachedProperties(QObject *);

Q_SIGNALS:
    void statusChanged(QDeclarativeComponent::Status);
    void progressChanged(qreal);

protected:
    QDeclarativeComponent(QDeclarativeComponentPrivate &dd, QObject* parent);
    Q_INVOKABLE QScriptValue createObject(QObject* parent);
    Q_INVOKABLE Q_REVISION(1) QScriptValue createObject(QObject* parent, const QScriptValue& valuemap); //XXX Versioning

private:
    QDeclarativeComponent(QDeclarativeEngine *, QDeclarativeCompiledData *, int, int, QObject *parent);

    Q_DISABLE_COPY(QDeclarativeComponent)
    friend class QDeclarativeVME;
    friend class QDeclarativeCompositeTypeData;
    friend class QDeclarativeTypeData;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QDeclarativeComponent::Status)
QML_DECLARE_TYPE(QDeclarativeComponent)
QML_DECLARE_TYPEINFO(QDeclarativeComponent, QML_HAS_ATTACHED_PROPERTIES)


#endif // QDECLARATIVECOMPONENT_H
