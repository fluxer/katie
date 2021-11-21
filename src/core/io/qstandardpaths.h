/****************************************************************************
**
** Copyright (C) 2021 Ivailo Monev
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

#ifndef QSTANDARDPATHS_H
#define QSTANDARDPATHS_H

#include <QtCore/qstringlist.h>
#include <QtCore/qurl.h>

QT_BEGIN_NAMESPACE

class Q_CORE_EXPORT QStandardPaths
{
public:
    enum StandardLocation {
        DesktopLocation,
        DocumentsLocation,
        DownloadLocation,
        VideosLocation,
        MusicLocation,
        PicturesLocation,
        PublicLocation,
        TemplatesLocation,

        DataLocation,
        CacheLocation,
        ConfigLocation,
        RuntimeLocation,
        StateLocation,
        ExecutableLocation
    };

    enum LocateOption {
        LocateFile = 0x0,
        LocateDirectory = 0x1
    };

    static QString writableLocation(StandardLocation type);
    static QStringList standardLocations(StandardLocation type);

    static QString locate(StandardLocation type, const QString &fileName, LocateOption options = LocateFile);
    static QStringList locateAll(StandardLocation type, const QString &fileName, LocateOption options = LocateFile);
    static QString displayName(StandardLocation type);

    static QString findExecutable(const QString &executableName, const QStringList &paths = QStringList());

    static bool openUrl(const QUrl &url);

private:
    Q_DISABLE_COPY(QStandardPaths)
};


QT_END_NAMESPACE

#endif // QSTANDARDPATHS_H
