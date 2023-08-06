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

#include <QStringList>


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

    QSettings();
    QSettings(const QString &fileNamet);
    ~QSettings();

    void clear();
    void sync();
    SettingsStatus status() const;

    QStringList keys() const;
    QString group() const;
    void beginGroup(const QString &prefix);
    void endGroup();
    QStringList groupKeys() const;

    bool isWritable() const;

    void setString(const QString &key, const QString &value);
    QString string(const QString &key, const QString &defaultValue = QString()) const;
    void setStringList(const QString &key, const QStringList &value);
    QStringList stringList(const QString &key, const QStringList &defaultValue = QStringList()) const;
    void setInteger(const QString &key, const qlonglong value);
    qlonglong integer(const QString &key, const qlonglong defaultValue = 0) const;
    void setBoolean(const QString &key, const bool value);
    bool boolean(const QString &key, const bool defaultValue = false) const;

    void remove(const QString &key);
    bool contains(const QString &key) const;

    QString fileName() const;

private:
    Q_DISABLE_COPY(QSettings)
    QSettingsPrivate *d_ptr;
};

QT_END_NAMESPACE

#endif // QT_NO_SETTINGS


#endif // QSETTINGS_H
