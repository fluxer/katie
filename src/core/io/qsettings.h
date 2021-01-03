/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSETTINGS_H
#define QSETTINGS_H

#include <QtCore/qvariant.h>

QT_BEGIN_HEADER

#ifndef QT_NO_SETTINGS

QT_BEGIN_NAMESPACE

class QIODevice;
class QSettingsPrivate;

class Q_CORE_EXPORT QSettings : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSettings)

public:
    enum SettingsStatus {
        NoError = 0,
        AccessError,
        FormatError
    };

    enum Format {
        NativeFormat,
        IniFormat,

        InvalidFormat = 16,
        CustomFormat1,
        CustomFormat2,
        CustomFormat3,
        CustomFormat4,
        CustomFormat5,
        CustomFormat6,
        CustomFormat7,
        CustomFormat8,
        CustomFormat9,
        CustomFormat10,
        CustomFormat11,
        CustomFormat12,
        CustomFormat13,
        CustomFormat14,
        CustomFormat15,
        CustomFormat16
    };

    enum Scope {
        UserScope,
        SystemScope
    };

    typedef QMap<QString, QVariant> SettingsMap;

    explicit QSettings(QObject *parent = Q_NULLPTR);
    QSettings(Scope scope, QObject *parent = Q_NULLPTR);
    QSettings(Format format, Scope scope, QObject *parent = Q_NULLPTR);
    QSettings(const QString &fileName, Format format = NativeFormat, QObject *parent = Q_NULLPTR);
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
    Scope scope() const;

    typedef bool (*ReadFunc)(QIODevice &device, SettingsMap &map);
    typedef bool (*WriteFunc)(QIODevice &device, const SettingsMap &map);

    static Format registerFormat(const QString &extension, ReadFunc readFunc, WriteFunc writeFunc);

private:
    Q_DISABLE_COPY(QSettings)
};

QT_END_NAMESPACE

#endif // QT_NO_SETTINGS

QT_END_HEADER

#endif // QSETTINGS_H
