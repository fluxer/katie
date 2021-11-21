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

#ifndef QDECLARATIVEDIRPARSER_P_H
#define QDECLARATIVEDIRPARSER_P_H

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

#include <QtCore/QUrl>
#include <QtCore/QHash>

QT_BEGIN_NAMESPACE

class QDeclarativeError;
class QDeclarativeDirParser
{
    Q_DISABLE_COPY(QDeclarativeDirParser)

public:
    QDeclarativeDirParser();
    ~QDeclarativeDirParser();

    QUrl url() const;
    void setUrl(const QUrl &url);

    QString source() const;
    void setSource(const QString &source);

    QString fileSource() const;
    void setFileSource(const QString &filePath);

    bool isParsed() const;
    bool parse();

    bool hasError() const;
    QList<QDeclarativeError> errors(const QString &uri) const;

    struct Plugin
    {
        Plugin() {}

        Plugin(const QString &name, const QString &path)
            : name(name), path(path) {}

        QString name;
        QString path;
    };

    struct Component
    {
        Component()
            : majorVersion(0), minorVersion(0) {}

        Component(const QString &typeName, const QString &fileName, int majorVersion, int minorVersion)
            : typeName(typeName), fileName(fileName), majorVersion(majorVersion), minorVersion(minorVersion)
            {}

        QString typeName;
        QString fileName;
        int majorVersion;
        int minorVersion;
    };

    QList<Component> components() const;
    QList<Plugin> plugins() const;

private:
    void reportError(int line, int column, const QString &message);

private:
    QList<QDeclarativeError> _errors;
    QUrl _url;
    QString _source;
    QString _filePathSouce;
    QList<Component> _components;
    QList<Plugin> _plugins;
    bool _isParsed;
};

typedef QList<QDeclarativeDirParser::Component> QDeclarativeDirComponents;


QT_END_NAMESPACE

#endif // QDECLARATIVEDIRPARSER_P_H
