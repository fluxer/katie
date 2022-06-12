/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QSETTINGS_H
#define QSETTINGS_H

#include <QtCore/qvariant.h>


#ifndef QT_NO_SETTINGS

QT_BEGIN_NAMESPACE

class QSettingsPrivate;

class Q_CORE_EXPORT QSettings
{
    Q_DECLARE_PRIVATE(QSettings)

public:
    enum SettingsStatus {
        NoError = 0,
        AccessError,
        FormatError
    };

    enum Format {
        NativeFormat = 0,
        IniFormat
    };

    typedef QMap<QString, QVariant> SettingsMap;

    QSettings();
    QSettings(Format format);
    QSettings(const QString &fileName, Format format = NativeFormat);
    ~QSettings();

    void clear();
    void sync();
    SettingsStatus status() const;

    SettingsMap map() const;
    QStringList keys() const;
    QString group() const;
    void beginGroup(const QString &prefix);
    void endGroup();
    QStringList groupKeys() const;

    bool isWritable() const;

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    void remove(const QString &key);
    bool contains(const QString &key) const;

    QString fileName() const;
    Format format() const;

private:
    Q_DISABLE_COPY(QSettings)
    QSettingsPrivate *d_ptr;
};

QT_END_NAMESPACE

#endif // QT_NO_SETTINGS


#endif // QSETTINGS_H
