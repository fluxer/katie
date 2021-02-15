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

#ifndef QDESIGNER_SETTINGS_H
#define QDESIGNER_SETTINGS_H

#include "designer_enums.h"
#include <shared_settings_p.h>
#include <QtCore/QMap>
#include <QtCore/QRect>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QDesignerSettingsInterface;
struct ToolWindowFontSettings;

class QDesignerSettings : public qdesigner_internal::QDesignerSharedSettings
{
public:
    QDesignerSettings(QDesignerFormEditorInterface *core);

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    void restoreGeometry(QWidget *w, QRect fallBack = QRect()) const;
    void saveGeometryFor(const QWidget *w);

    QStringList recentFilesList() const;
    void setRecentFilesList(const QStringList &list);

    void setShowNewFormOnStartup(bool showIt);
    bool showNewFormOnStartup() const;

    void setUiMode(UIMode mode);
    UIMode uiMode() const;

    void setToolWindowFont(const ToolWindowFontSettings &fontSettings);
    ToolWindowFontSettings toolWindowFont() const;

    QByteArray mainWindowState(UIMode mode) const;
    void setMainWindowState(UIMode mode, const QByteArray &mainWindowState);

    QByteArray toolBarsState(UIMode mode) const;
    void setToolBarsState(UIMode mode, const QByteArray &mainWindowState);

    void clearBackup();
    void setBackup(const QMap<QString, QString> &map);
    QMap<QString, QString> backup() const;
};

QT_END_NAMESPACE

#endif // QDESIGNER_SETTINGS_H
