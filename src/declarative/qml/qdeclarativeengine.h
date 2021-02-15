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

#ifndef QDECLARATIVEENGINE_H
#define QDECLARATIVEENGINE_H

#include <QtCore/qurl.h>
#include <QtCore/qobject.h>
#include <QtCore/qmap.h>
#include <QtScript/qscriptvalue.h>
#include <QtDeclarative/qdeclarativeerror.h>


QT_BEGIN_NAMESPACE


class QDeclarativeComponent;
class QDeclarativeEnginePrivate;
class QDeclarativeImportsPrivate;
class QDeclarativeExpression;
class QDeclarativeContext;
class QDeclarativeType;
class QUrl;
class QScriptEngine;
class QScriptContext;
class QDeclarativeImageProvider;
class QNetworkAccessManager;
class QDeclarativeNetworkAccessManagerFactory;
class Q_DECLARATIVE_EXPORT QDeclarativeEngine : public QObject
{
    Q_OBJECT
public:
    QDeclarativeEngine(QObject *p = 0);
    virtual ~QDeclarativeEngine();

    QDeclarativeContext *rootContext() const;

    void clearComponentCache();

    QStringList importPathList() const;
    void setImportPathList(const QStringList &paths);
    void addImportPath(const QString& dir);

    QStringList pluginPathList() const;
    void setPluginPathList(const QStringList &paths);
    void addPluginPath(const QString& dir);

    bool importPlugin(const QString &filePath, const QString &uri, QString *errorString);

    void setNetworkAccessManagerFactory(QDeclarativeNetworkAccessManagerFactory *);
    QDeclarativeNetworkAccessManagerFactory *networkAccessManagerFactory() const;

    QNetworkAccessManager *networkAccessManager() const;

    void addImageProvider(const QString &id, QDeclarativeImageProvider *);
    QDeclarativeImageProvider *imageProvider(const QString &id) const;
    void removeImageProvider(const QString &id);

    QUrl baseUrl() const;
    void setBaseUrl(const QUrl &);

    bool outputWarningsToStandardError() const;
    void setOutputWarningsToStandardError(bool);

    static QDeclarativeContext *contextForObject(const QObject *);
    static void setContextForObject(QObject *, QDeclarativeContext *);

    enum ObjectOwnership { CppOwnership, JavaScriptOwnership };
    static void setObjectOwnership(QObject *, ObjectOwnership);
    static ObjectOwnership objectOwnership(QObject *);

Q_SIGNALS:
    void quit();
    void warnings(const QList<QDeclarativeError> &warnings);

private:
    Q_DISABLE_COPY(QDeclarativeEngine)
    Q_DECLARE_PRIVATE(QDeclarativeEngine)
};

QT_END_NAMESPACE


#endif // QDECLARATIVEENGINE_H
