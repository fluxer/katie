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

#ifndef QSETTINGS_P_H
#define QSETTINGS_P_H

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

#include "qfileinfo.h"
#include "qstringlist.h"
#include "qobject_p.h"

QT_BEGIN_NAMESPACE

class QSettingsPrivate
    : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QSettings)

public:
    QSettingsPrivate(QSettings::Format format);
    QSettingsPrivate(const QString &fileName, QSettings::Format format);
    ~QSettingsPrivate();

    void read();
    void write();
    void notify();

    QString toGroupKey(const QString &key) const;
    // INI parser functions
    static QString variantToString(const QVariant &v);
    static QVariant stringToVariant(const QString &s);
    static QStringList splitArgs(const QString &s, int idx);

    QSettings::Format format;
    QSettings::SettingsStatus status;
    QString filename;
    QSettings::SettingsMap map;
    QString group;

    QSettings::ReadFunc readFunc;
    QSettings::WriteFunc writeFunc;
    bool shouldwrite;
};

QT_END_NAMESPACE

#endif // QSETTINGS_P_H
