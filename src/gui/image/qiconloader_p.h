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

#ifndef QDESKTOPICON_P_H
#define QDESKTOPICON_P_H

#ifndef QT_NO_ICON
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

#include <QtCore/QHash>
#include <QtGui/QIcon>
#include <QtGui/QIconEngine>
#include <QtGui/QPixmapCache>
#include "qicon_p.h"

QT_BEGIN_NAMESPACE

class QIconLoader;

struct QIconDirInfo
{
    enum Type {
        Fixed,
        Scalable,
        Threshold
    };

    QIconDirInfo(const QString &_path = QString())
        : path(_path),
        size(0),
        maxSize(0),
        minSize(0),
        threshold(0),
        type(Threshold) { }
    QString path;
    short size;
    short maxSize;
    short minSize;
    short threshold;
    Type type;
};

class QIconLoaderEngineEntry
{
public:
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state);

    QString filename;
    QIconDirInfo dir;

private:
    QPixmap m_basePixmap;
};

typedef QList<QIconLoaderEngineEntry*> QThemeIconEntries;

class QIconLoaderEngine : public QIconEngine
{
public:
    QIconLoaderEngine(const QString& iconName = QString());
    ~QIconLoaderEngine();

    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) final;
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) final;
    QSize actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state) final;
    QList<QSize> availableSizes(QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::Off) const final;
    QString iconName() const final;
    QIconEngine *clone() const final;
    bool read(QDataStream &in) final;
    bool write(QDataStream &out) const final;
    QString key() const final;

private:
    void ensureLoaded();
    QIconLoaderEngineEntry *entryForSize(const QSize &size);
    QIconLoaderEngine(const QIconLoaderEngine &other);

    QThemeIconEntries m_entries;
    QString m_iconName;
    uint m_key;
};

class QIconTheme
{
public:
    QIconTheme();
    QIconTheme(const QString &name, const bool supportsSvg);
    QStringList parents() { return m_parents; }
    QList <QIconDirInfo> keyList() { return m_keyList; }
    QString contentDir() { return m_contentDir; }
    bool isValid() { return m_valid; }

private:
    QString m_contentDir;
    QList <QIconDirInfo> m_keyList;
    QStringList m_parents;
    bool m_valid;
    bool m_supportsSvg;
};

class QIconLoader
{
public:
    QIconLoader();
    QThemeIconEntries loadIcon(const QString &iconName);
    uint themeKey() const { return m_themeKey; }

    QString themeName() const { return m_userTheme.isEmpty() ? m_systemTheme : m_userTheme; }
    void setThemeName(const QString &themeName);
    QIconTheme theme() const { return m_themeList.value(themeName()); }
    void setThemeSearchPath(const QStringList &searchPaths);
    QStringList themeSearchPaths() const;
    static QIconLoader *instance();
    void updateSystemTheme();
    void invalidateKey() { m_themeKey++; }

private:
    QThemeIconEntries findIconHelper(const QString &themeName,
                                     const QString &iconName,
                                     QStringList &visited);
    uint m_themeKey;
    bool m_supportsSvg;

    QString m_userTheme;
    QString m_systemTheme;
    mutable QStringList m_iconDirs;
    QHash <QString, QIconTheme> m_themeList;
};

QT_END_NAMESPACE

#endif // QDESKTOPICON_P_H

#endif //QT_NO_ICON
