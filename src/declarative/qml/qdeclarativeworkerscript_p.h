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

#ifndef QDECLARATIVEWORKERSCRIPT_P_H
#define QDECLARATIVEWORKERSCRIPT_P_H

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

#include "qdeclarative.h"
#include "qdeclarativeparserstatus.h"

#include <QtCore/qthread.h>
#include <QtScript/qscriptvalue.h>
#include <QtCore/qurl.h>


QT_BEGIN_NAMESPACE


class QDeclarativeWorkerScript;
class QDeclarativeWorkerScriptEnginePrivate;
class QDeclarativeWorkerScriptEngine : public QThread
{
Q_OBJECT
public:
    QDeclarativeWorkerScriptEngine(QDeclarativeEngine *parent = 0);
    virtual ~QDeclarativeWorkerScriptEngine();

    int registerWorkerScript(QDeclarativeWorkerScript *);
    void removeWorkerScript(int);
    void executeUrl(int, const QUrl &);
    void sendMessage(int, const QVariant &);

protected:
    virtual void run();

private:
    QDeclarativeWorkerScriptEnginePrivate *d;
};

class Q_AUTOTEST_EXPORT QDeclarativeWorkerScript : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

    Q_INTERFACES(QDeclarativeParserStatus)
public:
    QDeclarativeWorkerScript(QObject *parent = nullptr);
    virtual ~QDeclarativeWorkerScript();

    QUrl source() const;
    void setSource(const QUrl &);

public slots:
    void sendMessage(const QScriptValue &);

signals:
    void sourceChanged();
    void message(const QScriptValue &messageObject);

protected:
    virtual void classBegin();
    virtual void componentComplete();
    virtual bool event(QEvent *);

private:
    QDeclarativeWorkerScriptEngine *engine();
    QDeclarativeWorkerScriptEngine *m_engine;
    int m_scriptId;
    QUrl m_source;
    bool m_componentComplete;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeWorkerScript)


#endif // QDECLARATIVEWORKERSCRIPT_P_H
