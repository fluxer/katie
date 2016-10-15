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

#include "qgtkstyle_p.h"

// This file is responsible for resolving all GTK functions we use
// dynamically. This is done to avoid link-time dependancy on GTK
// as well as crashes occurring due to usage of the GTK_QT engines
//
// Additionally we create a map of common GTK widgets that we can pass
// to the GTK theme engine as many engines resort to querying the
// actual widget pointers for details that are not covered by the
// state flags

#include <QtCore/qglobal.h>
#if !defined(QT_NO_STYLE_GTK)

#include <QtCore/QEvent>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtCore/QHash>
#include <QtCore/QUrl>
#include <QtCore/QLibrary>
#include <QtCore/QDebug>

#include <qapplication_p.h>
#include <qiconloader_p.h>

#include <QtGui/QMenu>
#include <QtGui/QStyle>
#include <QtGui/QApplication>
#include <QtGui/QPixmapCache>
#include <QtGui/QStatusBar>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/qx11info_x11.h>

#include <qt_x11_p.h>

#ifndef QT_NO_GCONF2
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#endif

#include <gdk/gdkx.h>

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QGtkStyleUpdateScheduler, styleScheduler)

Ptr_gnome_icon_lookup_sync QGtkStylePrivate::gnome_icon_lookup_sync = 0;
Ptr_gnome_vfs_init QGtkStylePrivate::gnome_vfs_init = 0;

typedef int (*x11ErrorHandler)(Display*, XErrorEvent*);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QGtkStylePrivate*)

QT_BEGIN_NAMESPACE

static void gtkStyleSetCallback(GtkWidget*)
{
    qRegisterMetaType<QGtkStylePrivate *>();

    // We have to let this function return and complete the event
    // loop to ensure that all gtk widgets have been styled before
    // updating
    QMetaObject::invokeMethod(styleScheduler(), "updateTheme", Qt::QueuedConnection);
}

static void update_toolbar_style(GtkWidget *gtkToolBar, GParamSpec *, gpointer)
{
    GtkToolbarStyle toolbar_style = GTK_TOOLBAR_ICONS;
    g_object_get(gtkToolBar, "toolbar-style", &toolbar_style, NULL);
    QWidgetList widgets = QApplication::allWidgets();
    for (int i = 0; i < widgets.size(); ++i) {
        QWidget *widget = widgets.at(i);
        if (qobject_cast<QToolButton*>(widget)) {
            QEvent event(QEvent::StyleChange);
            QApplication::sendEvent(widget, &event);
        }
    }
}

static QHashableLatin1Literal classPath(GtkWidget *widget)
{
    char *class_path;
    gtk_widget_path (widget, NULL, &class_path, NULL);

    char *copy = class_path;
    if (strncmp(copy, "GtkWindow.", 10) == 0)
        copy += 10;
    if (strncmp(copy, "GtkFixed.", 9) == 0)
        copy += 9;

    copy = strdup(copy);

    g_free(class_path);

    return QHashableLatin1Literal::fromData(copy);
}



bool QGtkStyleFilter::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::ApplicationPaletteChange) {
        // Only do this the first time since this will also
        // generate applicationPaletteChange events
        if (!qt_app_palettes_hash() ||  qt_app_palettes_hash()->isEmpty()) {
            stylePrivate->applyCustomPaletteHash();
        }
    }
    return QObject::eventFilter(obj, e);
}

QList<QGtkStylePrivate *> QGtkStylePrivate::instances;
QGtkStylePrivate::WidgetMap *QGtkStylePrivate::widgetMap = 0;

QGtkStylePrivate::QGtkStylePrivate()
  : QCleanlooksStylePrivate()
  , filter(this)
{
    instances.append(this);
}

QGtkStylePrivate::~QGtkStylePrivate()
{
    instances.removeOne(this);
}

void QGtkStylePrivate::init()
{
#warning make gnomeui linked
    gnome_icon_lookup_sync = (Ptr_gnome_icon_lookup_sync)QLibrary::resolve(QLS("gnomeui-2"), 0, "gnome_icon_lookup_sync");
    gnome_vfs_init= (Ptr_gnome_vfs_init)QLibrary::resolve(QLS("gnomevfs-2"), 0, "gnome_vfs_init");

    initGtkWidgets();
}

GtkWidget* QGtkStylePrivate::gtkWidget(const QHashableLatin1Literal &path)
{
    GtkWidget *widget = gtkWidgetMap()->value(path);
    if (!widget) {
        // Theme might have rearranged widget internals
        widget = gtkWidgetMap()->value(path);
    }
    return widget;
}

GtkStyle* QGtkStylePrivate::gtkStyle(const QHashableLatin1Literal &path)
{
    if (GtkWidget *w = gtkWidgetMap()->value(path))
        return w->style;
    return 0;
}

/* \internal
 * Initializes a number of gtk menu widgets.
 * The widgets are cached.
 */
void QGtkStylePrivate::initGtkMenu() const
{
    // Create menubar
    GtkWidget *gtkMenuBar = gtk_menu_bar_new();
    setupGtkWidget(gtkMenuBar);

    GtkWidget *gtkMenuBarItem = gtk_menu_item_new_with_label("X");
    gtk_menu_shell_append((GtkMenuShell*)(gtkMenuBar), gtkMenuBarItem);
    gtk_widget_realize(gtkMenuBarItem);

    // Create menu
    GtkWidget *gtkMenu = gtk_menu_new();
    gtk_menu_item_set_submenu((GtkMenuItem*)(gtkMenuBarItem), gtkMenu);
    gtk_widget_realize(gtkMenu);

    GtkWidget *gtkMenuItem = gtk_menu_item_new_with_label("X");
    gtk_menu_shell_append((GtkMenuShell*)gtkMenu, gtkMenuItem);
    gtk_widget_realize(gtkMenuItem);

    GtkWidget *gtkCheckMenuItem = gtk_check_menu_item_new_with_label("X");
    gtk_menu_shell_append((GtkMenuShell*)gtkMenu, gtkCheckMenuItem);
    gtk_widget_realize(gtkCheckMenuItem);

    GtkWidget *gtkMenuSeparator = gtk_separator_menu_item_new();
    gtk_menu_shell_append((GtkMenuShell*)gtkMenu, gtkMenuSeparator);

    addAllSubWidgets(gtkMenuBar);
    addAllSubWidgets(gtkMenu);
}


void QGtkStylePrivate::initGtkTreeview() const
{
    GtkWidget *gtkTreeView = gtk_tree_view_new();
    gtk_tree_view_append_column((GtkTreeView*)gtkTreeView, gtk_tree_view_column_new());
    gtk_tree_view_append_column((GtkTreeView*)gtkTreeView, gtk_tree_view_column_new());
    gtk_tree_view_append_column((GtkTreeView*)gtkTreeView, gtk_tree_view_column_new());
    addWidget(gtkTreeView);
}


/* \internal
 * Initializes a number of gtk widgets that we can later on use to determine some of our styles.
 * The widgets are cached.
 */
void QGtkStylePrivate::initGtkWidgets() const
{
    // From gtkmain.c
    uid_t ruid = getuid ();
    uid_t rgid = getgid ();
    uid_t euid = geteuid ();
    uid_t egid = getegid ();
    if (ruid != euid || rgid != egid) {
        qWarning("\nThis process is currently running setuid or setgid.\nGTK+ does not allow this "
                 "therefore Qt cannot use the GTK+ integration.\nTry launching your app using \'gksudo\', "
                 "\'kdesudo\' or a similar tool.\n\n"
                 "See http://www.gtk.org/setuid.html for more information.\n");
        return;
    }

    static QString themeName;
    if (!gtkWidgetMap()->contains("GtkWindow") && themeName.isEmpty()) {
        themeName = getThemeName();

        if (themeName == QLS("Qt") || themeName == QLS("Qt4")) {
            // Due to namespace conflicts with Qt3 and obvious recursion with Qt4,
            // we cannot support the GTK_Qt Gtk engine
            qWarning("QGtkStyle cannot be used together with the GTK_Qt engine.");
            return;
        }
    }

    // Gtk will set the Qt error handler so we have to reset it afterwards
    x11ErrorHandler qt_x_errhandler = XSetErrorHandler(0);
    gtk_init (NULL, NULL);
    XSetErrorHandler(qt_x_errhandler);

    // make a window
    GtkWidget* gtkWindow = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_widget_realize(gtkWindow);
    QHashableLatin1Literal widgetPath = QHashableLatin1Literal::fromData(strdup("GtkWindow"));
    removeWidgetFromMap(widgetPath);
    gtkWidgetMap()->insert(widgetPath, gtkWindow);


    // Make all other widgets. respect the text direction
    if (qApp->layoutDirection() == Qt::RightToLeft)
        gtk_widget_set_default_direction(GTK_TEXT_DIR_RTL);

    if (!gtkWidgetMap()->contains("GtkButton")) {
        GtkWidget *gtkButton = gtk_button_new();
        addWidget(gtkButton);
        g_signal_connect(gtkButton, "style-set", G_CALLBACK(gtkStyleSetCallback), 0);
#warning FIXME, add addWidget() overload
        // addWidget(gtk_tool_button_new(NULL, "Qt"));
        addWidget(gtk_arrow_new(GTK_ARROW_DOWN, GTK_SHADOW_NONE));
        addWidget(gtk_hbutton_box_new());
        addWidget(gtk_check_button_new());
        addWidget(gtk_radio_button_new(NULL));
        addWidget(gtk_combo_box_new());
        addWidget(gtk_combo_box_entry_new());
        addWidget(gtk_entry_new());
        addWidget(gtk_frame_new(NULL));
        addWidget(gtk_expander_new(""));
        addWidget(gtk_statusbar_new());
        addWidget(gtk_hscale_new((GtkAdjustment*)(gtk_adjustment_new(1, 0, 1, 0, 0, 0))));
        addWidget(gtk_hscrollbar_new(NULL));
        addWidget(gtk_scrolled_window_new(NULL, NULL));

        initGtkMenu();
        addWidget(gtk_notebook_new());
        addWidget(gtk_progress_bar_new());
        addWidget(gtk_spin_button_new((GtkAdjustment*)
                                            (gtk_adjustment_new(1, 0, 1, 0, 0, 0)), 0.1, 3));
        GtkWidget *toolbar = gtk_toolbar_new();
        g_signal_connect (toolbar, "notify::toolbar-style", G_CALLBACK (update_toolbar_style), toolbar);
        gtk_toolbar_insert((GtkToolbar*)toolbar, gtk_separator_tool_item_new(), -1);
        addWidget(toolbar);
        initGtkTreeview();
        addWidget(gtk_vscale_new((GtkAdjustment*)(gtk_adjustment_new(1, 0, 1, 0, 0, 0))));
        addWidget(gtk_vscrollbar_new(NULL));
    }
    else // Rebuild map
    {
        // When styles change subwidgets can get rearranged
        // as with the combo box. We need to update the widget map
        // to reflect this;
        QHash<QHashableLatin1Literal, GtkWidget*> oldMap = *gtkWidgetMap();
        gtkWidgetMap()->clear();
        QHashIterator<QHashableLatin1Literal, GtkWidget*> it(oldMap);
        while (it.hasNext()) {
            it.next();
            if (!strchr(it.key().data(), '.')) {
                addAllSubWidgets(it.value());
            }
            free(const_cast<char *>(it.key().data()));
        }
    }
}

/*! \internal
 * destroys all previously buffered widgets.
 */
void QGtkStylePrivate::cleanupGtkWidgets()
{
    if (!widgetMap)
        return;
    if (widgetMap->contains("GtkWindow")) // Gtk will destroy all children
        gtk_widget_destroy(widgetMap->value("GtkWindow"));
    for (QHash<QHashableLatin1Literal, GtkWidget *>::const_iterator it = widgetMap->constBegin();
         it != widgetMap->constEnd(); ++it)
        free(const_cast<char *>(it.key().data()));
}

QString QGtkStylePrivate::getGConfString(const QString &value, const QString &fallback)
{
    QString retVal = fallback;
#ifndef QT_NO_GCONF2
#if !GLIB_CHECK_VERSION(2,35,0)
    g_type_init ();
#endif
    GConfClient* client = gconf_client_get_default();
    GError *err = 0;
    char *str = gconf_client_get_string(client, qPrintable(value), &err);
    if (!err) {
        retVal = QString::fromUtf8(str);
        g_free(str);
    }
    g_object_unref(client);
    if (err)
        g_error_free (err);
#endif
    return retVal;
}

bool QGtkStylePrivate::getGConfBool(const QString &key, bool fallback)
{
    bool retVal = fallback;
#ifndef QT_NO_GCONF2
#if !GLIB_CHECK_VERSION(2,35,0)
    g_type_init ();
#endif
    GConfClient* client = gconf_client_get_default();
    GError *err = 0;
    bool result = gconf_client_get_bool(client, qPrintable(key), &err);
    g_object_unref(client);
    if (!err)
        retVal = result;
    else
        g_error_free (err);
#endif
    return retVal;
}

QString QGtkStylePrivate::getThemeName()
{
    QString themeName;
    // We try to parse the gtkrc file first
    // primarily to avoid resolving Gtk functions if
    // the KDE 3 "Qt" style is currently in use
    QString rcPaths = QString::fromLocal8Bit(qgetenv("GTK2_RC_FILES"));
    if (!rcPaths.isEmpty()) {
        QStringList paths = rcPaths.split(QLS(":"));
        foreach (const QString &rcPath, paths) {
            if (!rcPath.isEmpty()) {
                QFile rcFile(rcPath);
                if (rcFile.exists() && rcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&rcFile);
                    while(!in.atEnd()) {
                        QString line = in.readLine();
                        if (line.contains(QLS("gtk-theme-name"))) {
                            line = line.right(line.length() - line.indexOf(QLatin1Char('=')) - 1);
                            line.remove(QLatin1Char('\"'));
                            line = line.trimmed();
                            themeName = line;
                            break;
                        }
                    }
                }
            }
            if (!themeName.isEmpty())
                break;
        }
    }

#ifndef QT_NO_GCONF2
    // Fall back to gconf
    if (themeName.isEmpty())
        themeName = getGConfString(QLS("/desktop/gnome/interface/gtk_theme"));
#endif

    return themeName;
}

QString QGtkStylePrivate::getIconThemeName() {
    x11ErrorHandler qt_x_errhandler = XSetErrorHandler(0);
    gtk_init(NULL, NULL);
    XSetErrorHandler(qt_x_errhandler);
    GtkSettings *settings = gtk_settings_get_default();
    gchararray value;
    g_object_get(settings, "gtk-icon-theme-name", &value, NULL);
    QString result = QString::fromUtf8(value);
    g_free(value);
    return result;
}

// Get size of the arrow controls in a GtkSpinButton
int QGtkStylePrivate::getSpinboxArrowSize() const
{
    const int MIN_ARROW_WIDTH = 6;
    GtkWidget *spinButton = gtkWidget("GtkSpinButton");
    GtkStyle *style = spinButton->style;
    gint size = pango_font_description_get_size (style->font_desc);
    gint arrow_size;
    arrow_size = qMax(PANGO_PIXELS (size), MIN_ARROW_WIDTH) + style->xthickness;
    arrow_size += arrow_size%2 + 1;
    return arrow_size;
}


bool QGtkStylePrivate::isKDE4Session()
{
    static int version = -1;
    if (version == -1)
        version = qgetenv("KDE_SESSION_VERSION").toInt();
    return (version == 4);
}

void QGtkStylePrivate::applyCustomPaletteHash()
{
    QPalette menuPal = gtkWidgetPalette("GtkMenu");
    GdkColor gdkBg = gtkWidget("GtkMenu")->style->bg[GTK_STATE_NORMAL];
    QColor bgColor(gdkBg.red>>8, gdkBg.green>>8, gdkBg.blue>>8);
    menuPal.setBrush(QPalette::Base, bgColor);
    menuPal.setBrush(QPalette::Window, bgColor);
    qApp->setPalette(menuPal, "QMenu");

    QPalette toolbarPal = gtkWidgetPalette("GtkToolbar");
    qApp->setPalette(toolbarPal, "QToolBar");

    QPalette menuBarPal = gtkWidgetPalette("GtkMenuBar");
    qApp->setPalette(menuBarPal, "QMenuBar");
}

/*! \internal
 *  Returns the gtk Widget that should be used to determine text foreground and background colors.
*/
GtkWidget* QGtkStylePrivate::getTextColorWidget() const
{
    return  gtkWidget("GtkEntry");
}

void QGtkStylePrivate::setupGtkWidget(GtkWidget* widget)
{
    if (Q_GTK_IS_WIDGET(widget)) {
        static GtkWidget* protoLayout = 0;
        if (!protoLayout) {
            protoLayout = gtk_fixed_new();
            gtk_container_add((GtkContainer*)(gtkWidgetMap()->value("GtkWindow")), protoLayout);
        }
        Q_ASSERT(protoLayout);

        if (!widget->parent && !GTK_WIDGET_TOPLEVEL(widget))
            gtk_container_add((GtkContainer*)(protoLayout), widget);
        gtk_widget_realize(widget);
    }
}

void QGtkStylePrivate::removeWidgetFromMap(const QHashableLatin1Literal &path)
{
    WidgetMap *map = gtkWidgetMap();
    WidgetMap::iterator it = map->find(path);
    if (it != map->end()) {
        free(const_cast<char *>(it.key().data()));
        map->erase(it);
    }
}

void QGtkStylePrivate::addWidgetToMap(GtkWidget *widget)
{
    if (Q_GTK_IS_WIDGET(widget)) {
        gtk_widget_realize(widget);
        QHashableLatin1Literal widgetPath = classPath(widget);

        removeWidgetFromMap(widgetPath);
        gtkWidgetMap()->insert(widgetPath, widget);
#ifdef DUMP_GTK_WIDGET_TREE
        qWarning("Inserted Gtk Widget: %s", widgetPath.data());
#endif
    }
 }

void QGtkStylePrivate::addAllSubWidgets(GtkWidget *widget, gpointer v)
{
    Q_UNUSED(v);
    addWidgetToMap(widget);
    if (GTK_CHECK_TYPE ((widget), gtk_container_get_type()))
        gtk_container_forall((GtkContainer*)widget, addAllSubWidgets, NULL);
}

// Updates window/windowtext palette based on the indicated gtk widget
QPalette QGtkStylePrivate::gtkWidgetPalette(const QHashableLatin1Literal &gtkWidgetName) const
{
    GtkWidget *gtkWidget = QGtkStylePrivate::gtkWidget(gtkWidgetName);
    Q_ASSERT(gtkWidget);
    QPalette pal = QApplication::palette();
    GdkColor gdkBg = gtkWidget->style->bg[GTK_STATE_NORMAL];
    GdkColor gdkText = gtkWidget->style->fg[GTK_STATE_NORMAL];
    GdkColor gdkDisabledText = gtkWidget->style->fg[GTK_STATE_INSENSITIVE];
    QColor bgColor(gdkBg.red>>8, gdkBg.green>>8, gdkBg.blue>>8);
    QColor textColor(gdkText.red>>8, gdkText.green>>8, gdkText.blue>>8);
    QColor disabledTextColor(gdkDisabledText.red>>8, gdkDisabledText.green>>8, gdkDisabledText.blue>>8);
    pal.setBrush(QPalette::Window, bgColor);
    pal.setBrush(QPalette::Button, bgColor);
    pal.setBrush(QPalette::All, QPalette::WindowText, textColor);
    pal.setBrush(QPalette::Disabled, QPalette::WindowText, disabledTextColor);
    pal.setBrush(QPalette::All, QPalette::ButtonText, textColor);
    pal.setBrush(QPalette::Disabled, QPalette::ButtonText, disabledTextColor);
    return pal;
}


void QGtkStyleUpdateScheduler::updateTheme()
{
    static QString oldTheme(QLS("qt_not_set"));
    QPixmapCache::clear();

    QFont font = QGtkStylePrivate::getThemeFont();
    if (QApplication::font() != font)
        qApp->setFont(font);

      if (oldTheme != QGtkStylePrivate::getThemeName()) {
          oldTheme = QGtkStylePrivate::getThemeName();
          QPalette newPalette = qApp->style()->standardPalette();
          QApplicationPrivate::setSystemPalette(newPalette);
          QApplication::setPalette(newPalette);
          if (!QGtkStylePrivate::instances.isEmpty()) {
              QGtkStylePrivate::instances.last()->initGtkWidgets();
              QGtkStylePrivate::instances.last()->applyCustomPaletteHash();
          }
          QList<QWidget*> widgets = QApplication::allWidgets();
          // Notify all widgets that size metrics might have changed
          foreach (QWidget *widget, widgets) {
              QEvent e(QEvent::StyleChange);
              QApplication::sendEvent(widget, &e);
          }
      }
    QIconLoader::instance()->updateSystemTheme();
}

void QGtkStylePrivate::addWidget(GtkWidget *widget)
{
    if (widget) {
        setupGtkWidget(widget);
        addAllSubWidgets(widget);
    }
}


// Fetch the application font from the pango font description
// contained in the theme.
QFont QGtkStylePrivate::getThemeFont()
{
    QFont font;
    GtkStyle *style = gtkStyle();
    if (style && qApp->desktopSettingsAware())
    {
        PangoFontDescription *gtk_font = style->font_desc;
        font.setPointSizeF((float)(pango_font_description_get_size(gtk_font))/PANGO_SCALE);

        QString family = QString::fromLatin1(pango_font_description_get_family(gtk_font));
        if (!family.isEmpty())
            font.setFamily(family);

        int weight = pango_font_description_get_weight(gtk_font);
        if (weight >= PANGO_WEIGHT_HEAVY)
            font.setWeight(QFont::Black);
        else if (weight >= PANGO_WEIGHT_BOLD)
            font.setWeight(QFont::Bold);
        else if (weight >= PANGO_WEIGHT_SEMIBOLD)
            font.setWeight(QFont::DemiBold);
        else if (weight >= PANGO_WEIGHT_NORMAL)
            font.setWeight(QFont::Normal);
        else
            font.setWeight(QFont::Light);

        PangoStyle fontstyle = pango_font_description_get_style(gtk_font);
        if (fontstyle == PANGO_STYLE_ITALIC)
            font.setStyle(QFont::StyleItalic);
        else if (fontstyle == PANGO_STYLE_OBLIQUE)
            font.setStyle(QFont::StyleOblique);
        else
            font.setStyle(QFont::StyleNormal);
    }
    return font;
}


// ----------- Native file dialogs -----------

// Extract filter list from expressions of type: foo (*.a *.b *.c)"
QStringList QGtkStylePrivate::extract_filter(const QString &rawFilter)
{
    QString result = rawFilter;
    QRegExp r(QString::fromLatin1("^([^()]*)\\(([a-zA-Z0-9_.*? +;#\\-\\[\\]@\\{\\}/!<>\\$%&=^~:\\|]*)\\)$"));
    int index = r.indexIn(result);
    if (index >= 0)
        result = r.cap(2);
    return result.split(QLatin1Char(' '));
}

extern QStringList qt_make_filter_list(const QString &filter);

void QGtkStylePrivate::setupGtkFileChooser(GtkWidget* gtkFileChooser, QWidget *parent,
                                const QString &dir, const QString &filter, QString *selectedFilter,
                                QFileDialog::Options options, bool isSaveDialog,
                                QMap<GtkFileFilter *, QString> *filterMap)
{
    g_object_set(gtkFileChooser, "do-overwrite-confirmation", gboolean(!(options & QFileDialog::DontConfirmOverwrite)), NULL);
    g_object_set(gtkFileChooser, "local_only", gboolean(true), NULL);
    if (!filter.isEmpty()) {
        QStringList filters = qt_make_filter_list(filter);
        foreach (const QString &rawfilter, filters) {
            GtkFileFilter *gtkFilter = gtk_file_filter_new ();
            QString name = rawfilter.left(rawfilter.indexOf(QLatin1Char('(')));
            QStringList extensions = extract_filter(rawfilter);
            gtk_file_filter_set_name(gtkFilter, qPrintable(name.isEmpty() ? extensions.join(QLS(", ")) : name));

            foreach (const QString &fileExtension, extensions) {
                // Note Gtk file dialogs are by default case sensitive
                // and only supports basic glob syntax so we
                // rewrite .xyz to .[xX][yY][zZ]
                QString caseInsensitive;
                for (int i = 0 ; i < fileExtension.length() ; ++i) {
                    QChar ch = fileExtension.at(i);
                    if (ch.isLetter()) {
                        caseInsensitive.append(
                                QLatin1Char('[') +
                                ch.toLower() +
                                ch.toUpper() +
                                QLatin1Char(']'));
                    } else {
                        caseInsensitive.append(ch);
                    }
                }
                gtk_file_filter_add_pattern (gtkFilter, qPrintable(caseInsensitive));

            }
            if (filterMap)
                filterMap->insert(gtkFilter, rawfilter);
            gtk_file_chooser_add_filter((GtkFileChooser*)gtkFileChooser, gtkFilter);
            if (selectedFilter && (rawfilter == *selectedFilter))
                gtk_file_chooser_set_filter((GtkFileChooser*)gtkFileChooser, gtkFilter);
        }
    }

    // Using the currently active window is not entirely correct, however
    // it gives more sensible behavior for applications that do not provide a
    // parent
    QWidget *modalFor = parent ? parent->window() : qApp->activeWindow();
    if (modalFor) {
        gtk_widget_realize(gtkFileChooser); // Creates X window
        XSetTransientForHint(gdk_x11_drawable_get_xdisplay(gtkFileChooser->window),
                             gdk_x11_drawable_get_xid(gtkFileChooser->window),
                             modalFor->winId());
        gdk_x11_window_set_user_time (gtkFileChooser->window, QX11Info::appUserTime());

    }

    QFileInfo fileinfo(dir);
    if (dir.isEmpty())
        fileinfo.setFile(QDir::currentPath());
    fileinfo.makeAbsolute();
    if (fileinfo.isDir()) {
        gtk_file_chooser_set_current_folder((GtkFileChooser*)gtkFileChooser, qPrintable(dir));
    } else if (isSaveDialog) {
        gtk_file_chooser_set_current_folder((GtkFileChooser*)gtkFileChooser, qPrintable(fileinfo.absolutePath()));
        gtk_file_chooser_set_current_name((GtkFileChooser*)gtkFileChooser, qPrintable(fileinfo.fileName()));
    } else {
        gtk_file_chooser_set_filename((GtkFileChooser*)gtkFileChooser, qPrintable(dir));
    }
}

QString QGtkStylePrivate::openFilename(QWidget *parent, const QString &caption, const QString &dir, const QString &filter,
                            QString *selectedFilter, QFileDialog::Options options)
{
    QMap<GtkFileFilter *, QString> filterMap;
    GtkWidget *gtkFileChooser = gtk_file_chooser_dialog_new (qPrintable(caption),
                                                             NULL,
                                                             GTK_FILE_CHOOSER_ACTION_OPEN,
                                                             GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                             GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                                             NULL);

    setupGtkFileChooser(gtkFileChooser, parent, dir, filter, selectedFilter, options, false, &filterMap);

    QWidget modal_widget;
    modal_widget.setAttribute(Qt::WA_NoChildEventsForParent, true);
    modal_widget.setParent(parent, Qt::Window);
    QApplicationPrivate::enterModal(&modal_widget);

    QString filename;
    if (gtk_dialog_run ((GtkDialog*)gtkFileChooser) == GTK_RESPONSE_ACCEPT) {
        char *gtk_filename = gtk_file_chooser_get_filename ((GtkFileChooser*)gtkFileChooser);
        filename = QString::fromUtf8(gtk_filename);
        g_free (gtk_filename);
        if (selectedFilter) {
            GtkFileFilter *gtkFilter = gtk_file_chooser_get_filter ((GtkFileChooser*)gtkFileChooser);
            *selectedFilter = filterMap.value(gtkFilter);
        }
    }

    QApplicationPrivate::leaveModal(&modal_widget);
    gtk_widget_destroy (gtkFileChooser);
    return filename;
}


QString QGtkStylePrivate::openDirectory(QWidget *parent, const QString &caption, const QString &dir, QFileDialog::Options options)
{
    QMap<GtkFileFilter *, QString> filterMap;
    GtkWidget *gtkFileChooser = gtk_file_chooser_dialog_new (qPrintable(caption),
                                                             NULL,
                                                             GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                                             GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                             GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                                             NULL);

    setupGtkFileChooser(gtkFileChooser, parent, dir, QString(), 0, options);
    QWidget modal_widget;
    modal_widget.setAttribute(Qt::WA_NoChildEventsForParent, true);
    modal_widget.setParent(parent, Qt::Window);
    QApplicationPrivate::enterModal(&modal_widget);

    QString filename;
    if (gtk_dialog_run ((GtkDialog*)gtkFileChooser) == GTK_RESPONSE_ACCEPT) {
        char *gtk_filename = gtk_file_chooser_get_filename ((GtkFileChooser*)gtkFileChooser);
        filename = QString::fromUtf8(gtk_filename);
        g_free (gtk_filename);
    }

    QApplicationPrivate::leaveModal(&modal_widget);
    gtk_widget_destroy (gtkFileChooser);
    return filename;
}

QStringList QGtkStylePrivate::openFilenames(QWidget *parent, const QString &caption, const QString &dir, const QString &filter,
                                 QString *selectedFilter, QFileDialog::Options options)
{
    QStringList filenames;
    QMap<GtkFileFilter *, QString> filterMap;
    GtkWidget *gtkFileChooser = gtk_file_chooser_dialog_new (qPrintable(caption),
                                                             NULL,
                                                             GTK_FILE_CHOOSER_ACTION_OPEN,
                                                             GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                             GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                                             NULL);

    setupGtkFileChooser(gtkFileChooser, parent, dir, filter, selectedFilter, options, false, &filterMap);
    g_object_set(gtkFileChooser, "select-multiple", gboolean(true), NULL);

    QWidget modal_widget;
    modal_widget.setAttribute(Qt::WA_NoChildEventsForParent, true);
    modal_widget.setParent(parent, Qt::Window);
    QApplicationPrivate::enterModal(&modal_widget);

    if (gtk_dialog_run ((GtkDialog*)gtkFileChooser) == GTK_RESPONSE_ACCEPT) {
        GSList *gtk_file_names = gtk_file_chooser_get_filenames((GtkFileChooser*)gtkFileChooser);
        for (GSList *iterator  = gtk_file_names ; iterator; iterator = iterator->next)
            filenames << QString::fromUtf8((const char*)iterator->data);
        g_slist_free(gtk_file_names);
        if (selectedFilter) {
            GtkFileFilter *gtkFilter = gtk_file_chooser_get_filter ((GtkFileChooser*)gtkFileChooser);
            *selectedFilter = filterMap.value(gtkFilter);
        }
    }

    QApplicationPrivate::leaveModal(&modal_widget);
    gtk_widget_destroy (gtkFileChooser);
    return filenames;
}

QString QGtkStylePrivate::saveFilename(QWidget *parent, const QString &caption, const QString &dir, const QString &filter,
                           QString *selectedFilter, QFileDialog::Options options)
{
    QMap<GtkFileFilter *, QString> filterMap;
    GtkWidget *gtkFileChooser = gtk_file_chooser_dialog_new (qPrintable(caption),
                                                             NULL,
                                                             GTK_FILE_CHOOSER_ACTION_SAVE,
                                                             GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                             GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                                             NULL);
    setupGtkFileChooser(gtkFileChooser, parent, dir, filter, selectedFilter, options, true, &filterMap);

    QWidget modal_widget;
    modal_widget.setAttribute(Qt::WA_NoChildEventsForParent, true);
    modal_widget.setParent(parent, Qt::Window);
    QApplicationPrivate::enterModal(&modal_widget);

    QString filename;
    if (gtk_dialog_run ((GtkDialog*)gtkFileChooser) == GTK_RESPONSE_ACCEPT) {
        char *gtk_filename = gtk_file_chooser_get_filename ((GtkFileChooser*)gtkFileChooser);
        filename = QString::fromUtf8(gtk_filename);
        g_free (gtk_filename);
        if (selectedFilter) {
            GtkFileFilter *gtkFilter = gtk_file_chooser_get_filter ((GtkFileChooser*)gtkFileChooser);
            *selectedFilter = filterMap.value(gtkFilter);
        }
    }

    QApplicationPrivate::leaveModal(&modal_widget);
    gtk_widget_destroy (gtkFileChooser);
    return filename;
}

QIcon QGtkStylePrivate::getFilesystemIcon(const QFileInfo &info)
{
    QIcon icon;
    if (gnome_vfs_init && gnome_icon_lookup_sync) {
        gnome_vfs_init();
        GtkIconTheme *theme = gtk_icon_theme_get_default();
        QByteArray fileurl = QUrl::fromLocalFile(info.absoluteFilePath()).toEncoded();
        char * icon_name = gnome_icon_lookup_sync(theme,
                                                  NULL,
                                                  fileurl.data(),
                                                  NULL,
                                                  GNOME_ICON_LOOKUP_FLAGS_NONE,
                                                  NULL);
        QString iconName = QString::fromUtf8(icon_name);
        g_free(icon_name);
        if (iconName.startsWith(QLatin1Char('/')))
            return QIcon(iconName);
        return QIcon::fromTheme(iconName);
    }
    return icon;
}

bool operator==(const QHashableLatin1Literal &l1, const QHashableLatin1Literal &l2)
{
    return l1.size() == l2.size() || qstrcmp(l1.data(), l2.data()) == 0;
}

// copied from qHash.cpp
uint qHash(const QHashableLatin1Literal &key)
{
    int n = key.size();
    const uchar *p = reinterpret_cast<const uchar *>(key.data());
    uint h = 0;
    uint g;

    while (n--) {
        h = (h << 4) + *p++;
        if ((g = (h & 0xf0000000)) != 0)
            h ^= g >> 23;
        h &= ~g;
    }
    return h;
}

QT_END_NAMESPACE

#include "moc_qgtkstyle_p.h"

#endif // !defined(QT_NO_STYLE_GTK)

