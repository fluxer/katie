/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#include "qguiplatformplugin.h"
#include "qdebug.h"
#include "qfile.h"
#include "qdir.h"
#include "qsettings.h"
#include "qfactoryloader_p.h"
#include "qstylefactory.h"
#include "qapplication.h"
#include "qplatformdefs.h"
#include "qicon.h"
#include "qstandardpaths.h"
#include "qcorecommon_p.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_LIBRARY
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, platformLoader, (QString::fromLatin1("/gui_platform")))
#endif

/*!
    \internal
    Return (and constructs if necesseray) the Gui Platform plugin.

    If no plugin can be loaded, the default one is returned.
 */
QGuiPlatformPlugin *qt_guiPlatformPlugin()
{
    static QGuiPlatformPlugin *appplugin = nullptr;
    if (appplugin) {
        return appplugin;
    }

    QGuiPlatformPlugin *plugin = nullptr;
#ifndef QT_NO_LIBRARY
    static const char* platformEnvTbl[] = {
        "QT_PLATFORM_PLUGIN",
        "XDG_CURRENT_DESKTOP",
        "DESKTOP_SESSION",
        nullptr
    };
    int counter = 0;
    while (platformEnvTbl[counter]) {
        QString key = qGetEnv(platformEnvTbl[counter]);
        if (!key.isEmpty()) {
            plugin = qobject_cast<QGuiPlatformPlugin*>(platformLoader()->instance(key));
            if (plugin) {
                break;
            }
        }
        counter++;
    }
#endif // QT_NO_LIBRARY

    if (!plugin) {
        static QGuiPlatformPlugin def;
        plugin = &def;
    }

    if (qApp) {
        appplugin = plugin;
    }

    return plugin;
}


/*!
    \class QGuiPlatformPlugin
    \brief The QGuiPlatformPlugin provides abstraction between several other
    classes and the platform defaults

    Applications that are built on top of Qt may provide a plugin so 3rd party
    Qt application running in the platform are integrated and behaviour is
    consistent across all applications. The plugin to be loaded can be
    specified with QT_PLATFORM_PLUGIN environment variable. If it is not set,
    DESKTOP_SESSION will be used as fallback on X11.

    \sa QIcon, QFileIconProvider, QStyle
 */

/*!
    The constructor can be used to install hooks in Qt
 */
QGuiPlatformPlugin::QGuiPlatformPlugin(QObject *parent)
    : QObject(parent)
{
}

QGuiPlatformPlugin::~QGuiPlatformPlugin()
{
}

/*!
    Returns the style string key to be used by application
*/
QString QGuiPlatformPlugin::styleName()
{
    return QString::fromLatin1("cleanlooks");
}

/*
    returns an additional palette (only work on X11)
*/
QPalette QGuiPlatformPlugin::palette()
{
    return QPalette();
}

/*!
    Returns the icon theme name for \p QIcon::fromTheme.
*/
QString QGuiPlatformPlugin::systemIconThemeName()
{
    static QString themename;

    if (themename.isEmpty()) {
        foreach (const QString &path, iconThemeSearchPaths()) {
            const QDir pathdir(path);
            const QStringList pathsubdirs = pathdir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
            foreach (const QString &subpath, pathsubdirs) {
                if (subpath == QLatin1String("hicolor")) {
                    continue;
                }
                QSettings indextheme(path + QLatin1Char('/') + subpath + QLatin1String("/index.theme"));
                const QStringList themedirectories = indextheme.stringList(QString::fromLatin1("Icon Theme/Directories"));
                if (!themedirectories.isEmpty()) {
                    themename = subpath;
                    return themename;
                }
            }
        }

        themename = QLatin1String("hicolor");
    }

    return themename;
}

/*!
    Returns list of system icon theme search paths. The environment
    variable XDG_DATA_DIRS affects the behaviour of the result
*/
QStringList QGuiPlatformPlugin::iconThemeSearchPaths()
{
    QStringList paths;

    // Add home directory first in search path
    QDir homeDir(QDir::homePath() + QLatin1String("/.icons"));
    if (homeDir.exists()) {
        paths.append(homeDir.path());
    }

    foreach (const QString &it, QStandardPaths::standardLocations(QStandardPaths::DataLocation)) {
        QDir dir(it);
        if (dir.exists()) {
            paths.append(dir.path() + QLatin1String("/icons"));
        }
    }

    return paths;
}

/*!
    Returns system icon. If the icon returned is null (default) the
    result from \p QIcon::fromTheme will be used instead
*/
QIcon QGuiPlatformPlugin::systemIcon(const QString &)
{
    return QIcon();
}

/*!
    Returns file icon. If the icon returned is null (default) the
    result from \p QFileIconProvider will be used instead
*/
QIcon QGuiPlatformPlugin::fileSystemIcon(const QFileInfo &)
{
    return QIcon();
}

/*!
    Like QStyle::styleHint
*/
int QGuiPlatformPlugin::platformHint(PlatformHint hint)
{
    int ret = 0;
    switch(hint) {
        case PH_ToolButtonStyle: {
            ret = Qt::ToolButtonIconOnly;
            break;
        }
        case PH_ToolBarIconSize: {
            // by default keep ret = 0 so QCommonStyle will use the style default
            break;
        }
        default: {
            break;
        }
    }
    return ret;
}

void QGuiPlatformPlugin::fileDialogDelete(QFileDialog *)
{
}

bool QGuiPlatformPlugin::fileDialogSetVisible(QFileDialog *, bool)
{
    return false;
}

QDialog::DialogCode QGuiPlatformPlugin::fileDialogResultCode(QFileDialog *)
{
    return QDialog::Rejected;
}

void QGuiPlatformPlugin::fileDialogSetDirectory(QFileDialog *, const QString &)
{
}

QString QGuiPlatformPlugin::fileDialogDirectory(const QFileDialog *) const
{
    return QString();
}

void QGuiPlatformPlugin::fileDialogSelectFile(QFileDialog *, const QString &)
{
}

QStringList QGuiPlatformPlugin::fileDialogSelectedFiles(const QFileDialog *) const
{
    return QStringList();
}

void QGuiPlatformPlugin::fileDialogSetFilter(QFileDialog *)
{
}

void QGuiPlatformPlugin::fileDialogSetNameFilters(QFileDialog *, const QStringList &)
{
}

void QGuiPlatformPlugin::fileDialogSelectNameFilter(QFileDialog *, const QString &)
{
}

QString QGuiPlatformPlugin::fileDialogSelectedNameFilter(const QFileDialog *) const
{
    return QString();
}

void QGuiPlatformPlugin::colorDialogDelete(QColorDialog *)
{
}

bool QGuiPlatformPlugin::colorDialogSetVisible(QColorDialog *, bool)
{
    return false;
}

void QGuiPlatformPlugin::colorDialogSetCurrentColor(QColorDialog *, const QColor &)
{
}

QT_END_NAMESPACE

#include "moc_qguiplatformplugin.h"
