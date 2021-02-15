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

#include "qdesigner_qsettings_p.h"

#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QtCore/QDebug>

QT_BEGIN_NAMESPACE

/*!
  \class QDesignerSettingsSimple

  \brief Implements QDesignerSettingsInterface by calls to QSettings
  */

QDesignerQSettings::QDesignerQSettings() :
    m_settings(settingsApplicationName(), QSettings::NativeFormat)
{
}

QString QDesignerQSettings::settingsApplicationName()
{
    return qApp->applicationName();
}

void QDesignerQSettings::beginGroup(const QString &prefix)
{
    Q_ASSERT(m_group.isEmpty()); // no sub-groups support
    m_settings.beginGroup(prefix);
}

void QDesignerQSettings::endGroup()
{
    m_settings.endGroup();
}

bool QDesignerQSettings::contains(const QString &key) const
{
    return m_settings.contains(key);
}

void QDesignerQSettings::setValue(const QString &key, const QVariant &value)
{
    if (m_group.isEmpty()) {
        m_settings.setValue(key, value);
    } else {
        m_settings.setValue(m_group + QLatin1Char('/') + key, value);
    }
}

QVariant QDesignerQSettings::value(const QString &key, const QVariant &defaultValue) const
{
    if (m_group.isEmpty()) {
        return m_settings.value(key, defaultValue);
    }
    return m_settings.value(m_group + QLatin1Char('/') + key, defaultValue);
}

void QDesignerQSettings::remove(const QString &key)
{
    m_settings.remove(key);
}

QSettings::SettingsStatus QDesignerQSettings::status() const
{
    return m_settings.status();
}

QT_END_NAMESPACE
