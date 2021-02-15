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

#ifndef QDECLARATIVECOMPONENT_P_H
#define QDECLARATIVECOMPONENT_P_H

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

#include "qdeclarativecomponent.h"

#include "qdeclarativeengine_p.h"
#include "qdeclarativetypeloader_p.h"
#include "qdeclarativeerror.h"
#include "qdeclarative.h"

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>

#include "qobject_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativeComponent;
class QDeclarativeEngine;
class QDeclarativeCompiledData;

class QDeclarativeComponentAttached;
class Q_AUTOTEST_EXPORT QDeclarativeComponentPrivate : public QObjectPrivate, public QDeclarativeTypeData::TypeDataCallback
{
    Q_DECLARE_PUBLIC(QDeclarativeComponent)
        
public:
    QDeclarativeComponentPrivate() : typeData(0), progress(0.), start(-1), count(-1), cc(0), engine(0), creationContext(0) {}

    QObject *beginCreate(QDeclarativeContextData *);
    void completeCreate();

    QDeclarativeTypeData *typeData;
    virtual void typeDataReady(QDeclarativeTypeData *);
    virtual void typeDataProgress(QDeclarativeTypeData *, qreal);
    
    void fromTypeData(QDeclarativeTypeData *data);

    QUrl url;
    qreal progress;

    int start;
    int count;
    QDeclarativeCompiledData *cc;

    struct ConstructionState {
        ConstructionState() : componentAttached(0), completePending(false) {}
        QList<QDeclarativeEnginePrivate::SimpleList<QDeclarativeAbstractBinding> > bindValues;
        QList<QDeclarativeEnginePrivate::SimpleList<QDeclarativeParserStatus> > parserStatus;
        QList<QPair<QDeclarativeGuard<QObject>, int> > finalizedParserStatus;
        QDeclarativeComponentAttached *componentAttached;
        QList<QDeclarativeError> errors;
        bool completePending;
    };
    ConstructionState state;

    static QObject *begin(QDeclarativeContextData *parentContext, QDeclarativeContextData *componentCreationContext,
                          QDeclarativeCompiledData *component, int start, int count, 
                          ConstructionState *state, QList<QDeclarativeError> *errors);
    static void beginDeferred(QDeclarativeEnginePrivate *enginePriv, QObject *object, 
                              ConstructionState *state);
    static void complete(QDeclarativeEnginePrivate *enginePriv, ConstructionState *state);

    QScriptValue createObject(QObject *publicParent, const QScriptValue valuemap);

    QDeclarativeEngine *engine;
    QDeclarativeGuardedContextData creationContext;

    void clear();

    static QDeclarativeComponentPrivate *get(QDeclarativeComponent *c) {
        return static_cast<QDeclarativeComponentPrivate *>(QObjectPrivate::get(c));
    }
};

class QDeclarativeComponentAttached : public QObject
{
    Q_OBJECT
public:
    QDeclarativeComponentAttached(QObject *parent = Q_NULLPTR);
    ~QDeclarativeComponentAttached();

    void add(QDeclarativeComponentAttached **a) {
        prev = a; next = *a; *a = this;
        if (next) next->prev = &next;
    }
    void rem() {
        if (next) next->prev = prev;
        *prev = next;
        next = 0; prev = 0;
    }
    QDeclarativeComponentAttached **prev;
    QDeclarativeComponentAttached *next;

Q_SIGNALS:
    void completed();
    void destruction();

private:
    friend class QDeclarativeContextData;
    friend class QDeclarativeComponentPrivate;
};

QT_END_NAMESPACE

#endif // QDECLARATIVECOMPONENT_P_H
