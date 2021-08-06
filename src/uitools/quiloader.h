/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#ifndef QUILOADER_H
#define QUILOADER_H

#include <QtCore/QObject>


QT_BEGIN_NAMESPACE

class QWidget;
class QLayout;
class QAction;
class QActionGroup;
class QString;
class QIODevice;
class QDir;

class QUiLoaderPrivate;
class Q_UITOOLS_EXPORT QUiLoader : public QObject
{
    Q_OBJECT
public:
    QUiLoader(QObject *parent = nullptr);
    virtual ~QUiLoader();

    QStringList pluginPaths() const;
    void clearPluginPaths();
    void addPluginPath(const QString &path);

    QWidget *load(QIODevice *device, QWidget *parentWidget = 0);
    QStringList availableWidgets() const;
    QStringList availableLayouts() const;

    virtual QWidget *createWidget(const QString &className, QWidget *parent = nullptr, const QString &name = QString());
    virtual QLayout *createLayout(const QString &className, QObject *parent = nullptr, const QString &name = QString());
    virtual QActionGroup *createActionGroup(QObject *parent = nullptr, const QString &name = QString());
    virtual QAction *createAction(QObject *parent = nullptr, const QString &name = QString());

    void setWorkingDirectory(const QDir &dir);
    QDir workingDirectory() const;

    void setScriptingEnabled(bool enabled);
    bool isScriptingEnabled() const;

    void setLanguageChangeEnabled(bool enabled);
    bool isLanguageChangeEnabled() const;

    void setTranslationEnabled(bool enabled);
    bool isTranslationEnabled() const;

private:
    QUiLoaderPrivate* d_ptr;
    Q_DECLARE_PRIVATE(QUiLoader)
    Q_DISABLE_COPY(QUiLoader)
};

QT_END_NAMESPACE


#endif // QUILOADER_H
