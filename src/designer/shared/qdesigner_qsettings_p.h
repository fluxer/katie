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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Katie Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QDESIGNER_QSETTINGS_H
#define QDESIGNER_QSETTINGS_H

#include "abstractsettings_p.h"

#include <QtCore/QSettings>

QT_BEGIN_NAMESPACE

//  Implements QDesignerSettingsInterface by calls to QSettings
class Q_DESIGNER_EXPORT QDesignerQSettings : public QDesignerSettingsInterface
{
public:
    QDesignerQSettings();

    virtual void beginGroup(const QString &prefix);
    virtual void endGroup();

    virtual bool contains(const QString &key) const;
    virtual void setValue(const QString &key, const QVariant &value);
    virtual QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    virtual void remove(const QString &key);

    virtual QSettings::SettingsStatus status() const;

    // The application name to be used for settings. Allows for including
    // the Qt version to prevent settings of different Qt versions from
    // interfering.
    static QString settingsApplicationName();

private:
    QString m_group;
    QSettings m_settings;
};

QT_END_NAMESPACE

#endif // QDESIGNER_QSETTINGS_H
