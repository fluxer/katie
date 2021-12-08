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
#ifndef QT_NO_ICON

#include "qiconloader_p.h"
#include "qapplication_p.h"
#include "qicon_p.h"
#include "qguiplatformplugin.h"
#include "qfactoryloader_p.h"
#include "qstylehelper_p.h"
#include "qguicommon_p.h"
#include "qcore_unix_p.h"

#include <QtGui/QIconEnginePlugin>
#include <QtGui/QPixmapCache>
#include <QtGui/QIconEngine>
#include <QtGui/QStyleOption>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtGui/QPainter>

#include <limits.h>

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QIconLoader, iconLoaderInstance)

/* Theme to use in last resort, if the theme does not have the icon, neither the parents  */
static const QString fallbackTheme = QLatin1String("hicolor");

QIconLoader::QIconLoader() :
        m_themeKey(1), m_supportsSvg(false)
{
    Q_ASSERT(qApp);

    m_systemTheme = qt_guiPlatformPlugin()->systemIconThemeName();
    if (m_systemTheme.isEmpty())
        m_systemTheme = fallbackTheme;
#ifndef QT_NO_LIBRARY
    if (iconloader()->keys().contains(QLatin1String("svg")))
        m_supportsSvg = true;
#endif //QT_NO_LIBRARY
}

QIconLoader *QIconLoader::instance()
{
   return iconLoaderInstance();
}

// Queries the system theme and invalidates existing
// icons if the theme has changed.
void QIconLoader::updateSystemTheme()
{
    // Only change if this is not explicitly set by the user
    if (m_userTheme.isEmpty()) {
        QString theme = qt_guiPlatformPlugin()->systemIconThemeName();
        if (theme.isEmpty())
            theme = fallbackTheme;
        if (theme != m_systemTheme) {
            m_systemTheme = theme;
            invalidateKey();
        }
    }
}

void QIconLoader::setThemeName(const QString &themeName)
{
    m_userTheme = themeName;
    invalidateKey();
}

void QIconLoader::setThemeSearchPath(const QStringList &searchPaths)
{
    m_iconDirs = searchPaths;
    themeList.clear();
    invalidateKey();
}

QStringList QIconLoader::themeSearchPaths() const
{
    if (m_iconDirs.isEmpty()) {
        m_iconDirs = qt_guiPlatformPlugin()->iconThemeSearchPaths();
        // Always add resource directory as search path
        m_iconDirs.append(QLatin1String(":/icons"));
    }
    return m_iconDirs;
}

QIconTheme::QIconTheme(const QString &themeName)
        : m_valid(false)
{
    foreach (const QString &it, QIcon::themeSearchPaths()) {
        QString themeDir = it + QLatin1Char('/') + themeName;
        const QStatInfo statinfo(themeDir + QLatin1String("/index.theme"));
        if (statinfo.isFile()) {
            m_contentDir = themeDir;
            m_valid = true;
            break;
        }
    }
    if (m_valid) {
        const QSettings indexReader(m_contentDir + QLatin1String("/index.theme"), QSettings::IniFormat);
        foreach (const QString &key, indexReader.keys()) {
            if (key.endsWith(QLatin1String("/Size"))) {
                // Note the QSettings ini-format does not accept
                // slashes in key names, hence we have to cheat
                if (int size = indexReader.value(key).toInt()) {
                    QString directoryKey = key.left(key.size() - 5);
                    QIconDirInfo dirInfo(directoryKey);
                    dirInfo.size = size;
                    QString type = indexReader.value(directoryKey +
                                                     QLatin1String("/Type")
                                                     ).toString();

                    if (type == QLatin1String("Fixed"))
                        dirInfo.type = QIconDirInfo::Fixed;
                    else if (type == QLatin1String("Scalable"))
                        dirInfo.type = QIconDirInfo::Scalable;
                    else
                        dirInfo.type = QIconDirInfo::Threshold;

                    dirInfo.threshold = indexReader.value(directoryKey +
                                                        QLatin1String("/Threshold"),
                                                        2).toInt();

                    dirInfo.minSize = indexReader.value(directoryKey +
                                                         QLatin1String("/MinSize"),
                                                         size).toInt();

                    dirInfo.maxSize = indexReader.value(directoryKey +
                                                        QLatin1String("/MaxSize"),
                                                        size).toInt();
                    m_keyList.append(dirInfo);
                }
            }
        }

        // Parent themes provide fallbacks for missing icons
        m_parents = indexReader.value(QLatin1String("Icon Theme/Inherits")).toStringList();

        // Ensure that all themes fall back to hicolor
        if (!m_parents.contains(fallbackTheme))
            m_parents.append(fallbackTheme);
    }
}

QThemeIconEntries QIconLoader::findIconHelper(const QString &themeName,
                                 const QString &iconName,
                                 QStringList &visited) const
{
    QThemeIconEntries entries;
    Q_ASSERT(!themeName.isEmpty());

    QPixmap pixmap;

    // Used to protect against potential recursions
    visited << themeName;

    QIconTheme theme = themeList.value(themeName);
    if (!theme.isValid()) {
        theme = QIconTheme(themeName);
        if (!theme.isValid())
            theme = QIconTheme(fallbackTheme);

        themeList.insert(themeName, theme);
    }

    QString contentDir = theme.contentDir() + QLatin1Char('/');

    // Add all relevant files
    foreach (const QIconDirInfo &dirInfo, theme.keyList()) {
        const QString subDir = contentDir + dirInfo.path + QLatin1Char('/');
        const QString pngPath = subDir + iconName + QLatin1String(".png");
        const QStatInfo pnginfo(pngPath);
        if (pnginfo.isFile()) {
            PixmapEntry *iconEntry = new PixmapEntry;
            iconEntry->dir = dirInfo;
            iconEntry->filename = pngPath;
            // Notice we ensure that pixmap entries always come before
            // scalable to preserve search order afterwards
            entries.prepend(iconEntry);
        } else if (m_supportsSvg) {
            const QString svgPath = subDir + iconName + QLatin1String(".svg");
            const QStatInfo svginfo(pngPath);
            if (svginfo.isFile()) {
                ScalableEntry *iconEntry = new ScalableEntry;
                iconEntry->dir = dirInfo;
                iconEntry->filename = svgPath;
                entries.append(iconEntry);
            }
        }
    }

    if (entries.isEmpty()) {
        // Search recursively through inherited themes
        foreach (const QString &parentTheme, theme.parents()) {
            if (!visited.contains(parentTheme)) // guard against recursion
                entries = findIconHelper(parentTheme, iconName, visited);

            if (!entries.isEmpty()) // success
                break;
        }
    }
    return entries;
}

QThemeIconEntries QIconLoader::loadIcon(const QString &name) const
{
    if (!themeName().isEmpty()) {
        QStringList visited;
        return findIconHelper(themeName(), name, visited);
    }

    return QThemeIconEntries();
}


// -------- Icon Loader Engine -------- //


QIconLoaderEngine::QIconLoaderEngine(const QString& iconName)
        : m_iconName(iconName), m_key(0)
{
    ensureLoaded();
}

QIconLoaderEngine::~QIconLoaderEngine()
{
    while (!m_entries.isEmpty())
        delete m_entries.takeLast();
    Q_ASSERT(m_entries.size() == 0);
}

QIconLoaderEngine::QIconLoaderEngine(const QIconLoaderEngine &other)
        : QIconEngine(other),
        m_iconName(other.m_iconName),
        m_key(0)
{
    ensureLoaded();
}

QIconEngine *QIconLoaderEngine::clone() const
{
    return new QIconLoaderEngine(*this);
}

bool QIconLoaderEngine::read(QDataStream &in) {
    in >> m_iconName;
    return true;
}

bool QIconLoaderEngine::write(QDataStream &out) const
{
    out << m_iconName;
    return true;
}

void QIconLoaderEngine::ensureLoaded()
{
    if (!(iconLoaderInstance()->themeKey() == m_key)) {

        while (!m_entries.isEmpty())
            delete m_entries.takeLast();

        Q_ASSERT(m_entries.size() == 0);
        m_entries = iconLoaderInstance()->loadIcon(m_iconName);
        m_key = iconLoaderInstance()->themeKey();
    }
}

void QIconLoaderEngine::paint(QPainter *painter, const QRect &rect,
                             QIcon::Mode mode, QIcon::State state)
{
    painter->drawPixmap(rect, pixmap(rect.size(), mode, state));
}

/*
 * This algorithm is defined by the freedesktop spec:
 * https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html#icon_lookup
 */
static bool directoryMatchesSize(const QIconDirInfo &dir, int iconsize)
{
    if (dir.type == QIconDirInfo::Fixed) {
        return dir.size == iconsize;

    } else if (dir.type == QIconDirInfo::Scalable) {
        return iconsize <= dir.maxSize &&
                iconsize >= dir.minSize;

    } else if (dir.type == QIconDirInfo::Threshold) {
        return iconsize >= dir.size - dir.threshold &&
                iconsize <= dir.size + dir.threshold;
    }

    Q_ASSERT(1); // Not a valid value
    return false;
}

/*
 * This algorithm is defined by the freedesktop spec:
 * https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html#icon_lookup
 */
static int directorySizeDistance(const QIconDirInfo &dir, int iconsize)
{
    if (dir.type == QIconDirInfo::Fixed) {
        return qAbs(dir.size - iconsize);

    } else if (dir.type == QIconDirInfo::Scalable) {
        if (iconsize < dir.minSize)
            return dir.minSize - iconsize;
        else if (iconsize > dir.maxSize)
            return iconsize - dir.maxSize;
        else
            return 0;

    } else if (dir.type == QIconDirInfo::Threshold) {
        if (iconsize < dir.size - dir.threshold)
            return dir.minSize - iconsize;
        else if (iconsize > dir.size + dir.threshold)
            return iconsize - dir.maxSize;
        else return 0;
    }

    Q_ASSERT(1); // Not a valid value
    return INT_MAX;
}

QIconLoaderEngineEntry *QIconLoaderEngine::entryForSize(const QSize &size)
{
    int iconsize = qMin(size.width(), size.height());

    // Note that m_entries are sorted so that png-files
    // come first

    // Search for exact matches first
    for (int i = 0; i < m_entries.count(); ++i) {
        QIconLoaderEngineEntry *entry = m_entries.at(i);
        if (directoryMatchesSize(entry->dir, iconsize)) {
            return entry;
        }
    }

    // Find the minimum distance icon
    int minimalSize = INT_MAX;
    QIconLoaderEngineEntry *closestMatch = 0;
    for (int i = 0; i < m_entries.count(); ++i) {
        QIconLoaderEngineEntry *entry = m_entries.at(i);
        int distance = directorySizeDistance(entry->dir, iconsize);
        if (distance < minimalSize) {
            minimalSize  = distance;
            closestMatch = entry;
        }
    }
    return closestMatch;
}

/*
 * Returns the actual icon size. For scalable svg's this is equivalent
 * to the requested size. Otherwise the closest match is returned but
 * we can never return a bigger size than the requested size.
 *
 */
QSize QIconLoaderEngine::actualSize(const QSize &size, QIcon::Mode mode,
                                   QIcon::State state)
{
    QIconLoaderEngineEntry *entry = entryForSize(size);
    if (entry) {
        const QIconDirInfo &dir = entry->dir;
        if (dir.type == QIconDirInfo::Scalable)
            return size;
        else {
            int result = qMin<int>(dir.size, qMin(size.width(), size.height()));
            return QSize(result, result);
        }
    }
    return QIconEngineV2::actualSize(size, mode, state);
}

QList<QSize> QIconLoaderEngine::availableSizes(QIcon::Mode mode, QIcon::State state) const
{
    QList<QSize> result;
    // Gets all sizes from the DirectoryInfo entries
    for (int i = 0 ; i < m_entries.size(); i++) {
        int size = m_entries.at(i)->dir.size;
        result.append(QSize(size, size));
    }
    return result;
}

QString QIconLoaderEngine::iconName() const
{
    return m_iconName;
}

QPixmap PixmapEntry::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    Q_UNUSED(state);

    // Ensure that basePixmap is lazily initialized before generating the
    // key, otherwise the cache key is not unique
    if (basePixmap.isNull())
        basePixmap.load(filename);

    int actualSize = qMin(size.width(), size.height());

    QString key = QLatin1String("$qt_theme_")
                  + HexString<qint64>(basePixmap.cacheKey())
                  + HexString<int>(mode)
                  + HexString<qint64>(qApp->palette().cacheKey())
                  + HexString<int>(actualSize);

    QPixmap cachedPixmap;
    if (QPixmapCache::find(key, &cachedPixmap)) {
        return cachedPixmap;
    } else {
        QStyleOption opt(0);
        opt.palette = qApp->palette();
        cachedPixmap = qApp->style()->generatedIconPixmap(mode, basePixmap, &opt);
        QPixmapCache::insert(key, cachedPixmap);
    }
    return cachedPixmap;
}

QPixmap ScalableEntry::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    if (svgIcon.isNull())
        svgIcon = QIcon(filename);

    // Simply reuse svg icon engine
    return svgIcon.pixmap(size, mode, state);
}

QPixmap QIconLoaderEngine::pixmap(const QSize &size, QIcon::Mode mode,
                                 QIcon::State state)
{
    QIconLoaderEngineEntry *entry = entryForSize(size);
    if (entry)
        return entry->pixmap(size, mode, state);

    return QPixmap();
}

QString QIconLoaderEngine::key() const
{
    return QLatin1String("QIconLoaderEngine");
}

QT_END_NAMESPACE

#endif //QT_NO_ICON




