/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#ifndef QGTKSTYLE_P_H
#define QGTKSTYLE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qglobal.h>
#if !defined(QT_NO_STYLE_GTK)

#include <QtCore/qstring.h>
#include <QtCore/qstringbuilder.h>
#include <QtCore/qcoreapplication.h>

#include <QtGui/QFileDialog>

#include <QtGui/QGtkStyle>
#include <qcleanlooksstyle_p.h>

#undef signals // Collides with GTK stymbols
#include <gtk/gtk.h>

typedef unsigned long XID;

#undef GTK_OBJECT_FLAGS
#define GTK_OBJECT_FLAGS(obj)(((GtkObject*)(obj))->flags)
#define Q_GTK_IS_WIDGET(widget) widget && GTK_CHECK_TYPE ((widget), gtk_widget_get_type())

#define QLS(x) QLatin1String(x)

QT_BEGIN_NAMESPACE

// ### Qt 4.7 - merge with QLatin1Literal
class QHashableLatin1Literal
{
public:
    int size() const { return m_size; }
    const char *data() const { return m_data; }

#ifdef __SUNPRO_CC
        QHashableLatin1Literal(const char* str)
        : m_size(strlen(str)), m_data(str) {}
#else
    template <int N>
        QHashableLatin1Literal(const char (&str)[N])
        : m_size(N - 1), m_data(str) {}
#endif

    QHashableLatin1Literal(const QHashableLatin1Literal &other)
        : m_size(other.m_size), m_data(other.m_data)
    {}

    QHashableLatin1Literal &operator=(const QHashableLatin1Literal &other)
    {
        if (this == &other)
            return *this;
        *const_cast<int *>(&m_size) = other.m_size;
        *const_cast<char **>(&m_data) = const_cast<char *>(other.m_data);
        return *this;
    }

    QString toString() const { return QString::fromLatin1(m_data, m_size); }

    static QHashableLatin1Literal fromData(const char *str)
    {
        return QHashableLatin1Literal(str, qstrlen(str));
    }

private:
    QHashableLatin1Literal(const char *str, int length)
        : m_size(length), m_data(str)
    {}

    const int m_size;
    const char *m_data;
};

bool operator==(const QHashableLatin1Literal &l1, const QHashableLatin1Literal &l2);
inline bool operator!=(const QHashableLatin1Literal &l1, const QHashableLatin1Literal &l2) { return !operator==(l1, l2); }
uint qHash(const QHashableLatin1Literal &key);

typedef QStringList (*_qt_filedialog_open_filenames_hook)(QWidget * parent, const QString &caption, const QString &dir,
                                                          const QString &filter, QString *selectedFilter, QFileDialog::Options options);
typedef QString (*_qt_filedialog_open_filename_hook)     (QWidget * parent, const QString &caption, const QString &dir,
                                                          const QString &filter, QString *selectedFilter, QFileDialog::Options options);
typedef QString (*_qt_filedialog_save_filename_hook)     (QWidget * parent, const QString &caption, const QString &dir,
                                                          const QString &filter, QString *selectedFilter, QFileDialog::Options options);
typedef QString (*_qt_filedialog_existing_directory_hook)(QWidget *parent, const QString &caption, const QString &dir,
                                                          QFileDialog::Options options);

extern Q_GUI_EXPORT _qt_filedialog_open_filename_hook qt_filedialog_open_filename_hook;
extern Q_GUI_EXPORT _qt_filedialog_open_filenames_hook qt_filedialog_open_filenames_hook;
extern Q_GUI_EXPORT _qt_filedialog_save_filename_hook qt_filedialog_save_filename_hook;
extern Q_GUI_EXPORT _qt_filedialog_existing_directory_hook qt_filedialog_existing_directory_hook;

class QGtkStylePrivate;

class QGtkStyleFilter : public QObject
{
public:
    QGtkStyleFilter(QGtkStylePrivate* sp)
        : stylePrivate(sp)
    {}
private:
    QGtkStylePrivate* stylePrivate;
    bool eventFilter(QObject *obj, QEvent *e);
};

typedef enum {
  GNOME_ICON_LOOKUP_FLAGS_NONE = 0,
  GNOME_ICON_LOOKUP_FLAGS_EMBEDDING_TEXT = 1<<0,
  GNOME_ICON_LOOKUP_FLAGS_SHOW_SMALL_IMAGES_AS_THEMSELVES = 1<<1,
  GNOME_ICON_LOOKUP_FLAGS_ALLOW_SVG_AS_THEMSELVES = 1<<2
} GnomeIconLookupFlags;

typedef enum {
  GNOME_ICON_LOOKUP_RESULT_FLAGS_NONE = 0,
  GNOME_ICON_LOOKUP_RESULT_FLAGS_THUMBNAIL = 1<<0
} GnomeIconLookupResultFlags;

struct GnomeThumbnailFactory;
typedef gboolean (*Ptr_gnome_vfs_init) (void);
typedef char* (*Ptr_gnome_icon_lookup_sync)  (
        GtkIconTheme *icon_theme,
        GnomeThumbnailFactory *,
        const char *file_uri,
        const char *custom_icon,
        GnomeIconLookupFlags flags,
        GnomeIconLookupResultFlags *result);

class QGtkStylePrivate : public QCleanlooksStylePrivate
{
    Q_DECLARE_PUBLIC(QGtkStyle)
public:
    QGtkStylePrivate();
    ~QGtkStylePrivate();

    QGtkStyleFilter filter;

    static GtkWidget* gtkWidget(const QHashableLatin1Literal &path);
    static GtkStyle* gtkStyle(const QHashableLatin1Literal &path = QHashableLatin1Literal("GtkWindow"));

    virtual void initGtkMenu() const;
    virtual void initGtkTreeview() const;
    virtual void initGtkWidgets() const;

    static void cleanupGtkWidgets();

    static bool isKDE4Session();
    void applyCustomPaletteHash();
    static QFont getThemeFont();
    static bool isThemeAvailable() { return gtkStyle() != 0; }

    static bool getGConfBool(const QString &key, bool fallback = 0);
    static QString getGConfString(const QString &key, const QString &fallback = QString());

    static QString getThemeName();
    static QString getIconThemeName();
    virtual int getSpinboxArrowSize() const;

    static void setupGtkFileChooser(GtkWidget* gtkFileChooser, QWidget *parent,
            const QString &dir, const QString &filter, QString *selectedFilter,
            QFileDialog::Options options, bool isSaveDialog = false,
            QMap<GtkFileFilter *, QString> *filterMap = 0);

    static QString openFilename(QWidget *parent, const QString &caption, const QString &dir, const QString &filter,
                                QString *selectedFilter, QFileDialog::Options options);
    static QString saveFilename(QWidget *parent, const QString &caption, const QString &dir, const QString &filter,
                                QString *selectedFilter, QFileDialog::Options options);
    static QString openDirectory(QWidget *parent, const QString &caption, const QString &dir, QFileDialog::Options options);
    static QStringList openFilenames(QWidget *parent, const QString &caption, const QString &dir, const QString &filter,
                                    QString *selectedFilter, QFileDialog::Options options);
    static QIcon getFilesystemIcon(const QFileInfo &);

    static Ptr_gnome_icon_lookup_sync gnome_icon_lookup_sync;
    static Ptr_gnome_vfs_init gnome_vfs_init;

    virtual QPalette gtkWidgetPalette(const QHashableLatin1Literal &gtkWidgetName) const;

protected:
    typedef QHash<QHashableLatin1Literal, GtkWidget*> WidgetMap;

    static inline void destroyWidgetMap()
    {
        cleanupGtkWidgets();
        delete widgetMap;
        widgetMap = 0;
    }

    static inline WidgetMap *gtkWidgetMap()
    {
        if (!widgetMap) {
            widgetMap = new WidgetMap();
            qAddPostRoutine(destroyWidgetMap);
        }
        return widgetMap;
    }

    static QStringList extract_filter(const QString &rawFilter);

    virtual GtkWidget* getTextColorWidget() const;
    static void setupGtkWidget(GtkWidget* widget);
    static void addWidgetToMap(GtkWidget* widget);
    static void addAllSubWidgets(GtkWidget *widget, gpointer v = 0);
    static void addWidget(GtkWidget *widget);
    static void removeWidgetFromMap(const QHashableLatin1Literal &path);

    virtual void init();

private:
    static QList<QGtkStylePrivate *> instances;
    static WidgetMap *widgetMap;
    friend class QGtkStyleUpdateScheduler;
};

// Helper to ensure that we have polished all our gtk widgets
// before updating our own palettes
class QGtkStyleUpdateScheduler : public QObject
{
    Q_OBJECT
public slots:
    void updateTheme();
};

QT_END_NAMESPACE

#endif // !QT_NO_STYLE_GTK
#endif // QGTKSTYLE_P_H
