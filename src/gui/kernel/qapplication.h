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

#ifndef QAPPLICATION_H
#define QAPPLICATION_H

#include <QtCore/qcoreapplication.h>
#include <QtGui/qwindowdefs.h>
#include <QtCore/qpoint.h>
#include <QtCore/qsize.h>
#include <QtGui/qcursor.h>


QT_BEGIN_NAMESPACE

class QFont;
class QPalette;
class QFontMetrics;
class QClipboard;
class QSessionManager;
class QDesktopWidget;
class QStyle;
class QIcon;
class QLocale;

class QApplicationPrivate;

class Q_GUI_EXPORT QApplication : public QCoreApplication
{
    Q_OBJECT
    Q_PROPERTY(Qt::LayoutDirection layoutDirection READ layoutDirection WRITE setLayoutDirection)
    Q_PROPERTY(QIcon windowIcon READ windowIcon WRITE setWindowIcon)
    Q_PROPERTY(int cursorFlashTime READ cursorFlashTime WRITE setCursorFlashTime)
    Q_PROPERTY(int doubleClickInterval  READ doubleClickInterval WRITE setDoubleClickInterval)
    Q_PROPERTY(int keyboardInputInterval READ keyboardInputInterval WRITE setKeyboardInputInterval)
#ifndef QT_NO_WHEELEVENT
    Q_PROPERTY(int wheelScrollLines  READ wheelScrollLines WRITE setWheelScrollLines)
#endif
    Q_PROPERTY(QSize globalStrut READ globalStrut WRITE setGlobalStrut)
    Q_PROPERTY(int startDragTime  READ startDragTime WRITE setStartDragTime)
    Q_PROPERTY(int startDragDistance  READ startDragDistance WRITE setStartDragDistance)
    Q_PROPERTY(bool quitOnLastWindowClosed  READ quitOnLastWindowClosed WRITE setQuitOnLastWindowClosed)
#ifndef QT_NO_STYLE_STYLESHEET
    Q_PROPERTY(QString styleSheet READ styleSheet WRITE setStyleSheet)
#endif
    Q_PROPERTY(bool autoSipEnabled READ autoSipEnabled WRITE setAutoSipEnabled)

public:
    QApplication(int &argc, char **argv);
#if defined(Q_WS_X11)
    QApplication(Display* dpy, Qt::HANDLE visual = 0, Qt::HANDLE cmap = 0);
    QApplication(Display *dpy, int &argc, char **argv, Qt::HANDLE visual = 0, Qt::HANDLE cmap= 0);
#endif
    virtual ~QApplication();

    static QStyle *style();
    static void setStyle(QStyle*);
    static QStyle *setStyle(const QString&);

#ifndef QT_NO_CURSOR
    static QCursor *overrideCursor();
    static void setOverrideCursor(const QCursor &);
    static void changeOverrideCursor(const QCursor &);
    static void restoreOverrideCursor();
#endif
    static QPalette palette();
    static QPalette palette(const QWidget *);
    static QPalette palette(const char *className);
    static void setPalette(const QPalette &, const char* className = 0);
    static QFont font();
    static QFont font(const QWidget*);
    static QFont font(const char *className);
    static void setFont(const QFont &, const char* className = 0);
    static QFontMetrics fontMetrics();

    static void setWindowIcon(const QIcon &icon);
    static QIcon windowIcon();

    static QWidgetList allWidgets();
    static QWidgetList topLevelWidgets();

    static QDesktopWidget *desktop();

    static QWidget *activePopupWidget();
    static QWidget *activeModalWidget();
#ifndef QT_NO_CLIPBOARD
    static QClipboard *clipboard();
#endif
    static QWidget *focusWidget();

    static QWidget *activeWindow();
    static void setActiveWindow(QWidget* act);

    static QWidget *widgetAt(const QPoint &p);
    static inline QWidget *widgetAt(int x, int y) { return widgetAt(QPoint(x, y)); }
    static QWidget *topLevelAt(const QPoint &p);
    static inline QWidget *topLevelAt(int x, int y)  { return topLevelAt(QPoint(x, y)); }

    static void syncX();
    static void beep();
    static void alert(QWidget *widget, int duration = 0);

    static Qt::KeyboardModifiers keyboardModifiers();
    static Qt::KeyboardModifiers queryKeyboardModifiers();
    static Qt::MouseButtons mouseButtons();

    static void setDesktopSettingsAware(bool);
    static bool desktopSettingsAware();

    static void setCursorFlashTime(int);
    static int cursorFlashTime();

    static void setDoubleClickInterval(int);
    static int doubleClickInterval();

    static void setKeyboardInputInterval(int);
    static int keyboardInputInterval();

#ifndef QT_NO_WHEELEVENT
    static void setWheelScrollLines(int);
    static int wheelScrollLines();
#endif
    static void setGlobalStrut(const QSize &);
    static QSize globalStrut();

    static void setStartDragTime(int ms);
    static int startDragTime();
    static void setStartDragDistance(int l);
    static int startDragDistance();

    static void setLayoutDirection(Qt::LayoutDirection direction);
    static Qt::LayoutDirection layoutDirection();

    static inline bool isRightToLeft() { return layoutDirection() == Qt::RightToLeft; }
    static inline bool isLeftToRight() { return layoutDirection() == Qt::LeftToRight; }

    static bool isEffectEnabled(Qt::UIEffect);
    static void setEffectEnabled(Qt::UIEffect, bool enable = true);

#if defined(Q_WS_X11)
    virtual bool x11EventFilter(XEvent *);
    int x11ClientMessage(QWidget*, XEvent*, bool passive_only);
    int x11ProcessEvent(XEvent*);
#endif

#ifndef QT_NO_SESSIONMANAGER
    // session management
    bool isSessionRestored() const;
    QString sessionId() const;
    QString sessionKey() const;
    virtual void commitData(QSessionManager& sm);
    virtual void saveState(QSessionManager& sm);
#endif

    static QLocale keyboardInputLocale();
    static Qt::LayoutDirection keyboardInputDirection();

    static int exec();
    bool notify(QObject *, QEvent *);

    static void setQuitOnLastWindowClosed(bool quit);
    static bool quitOnLastWindowClosed();


Q_SIGNALS:
    void lastWindowClosed();
    void focusChanged(QWidget *old, QWidget *now);
    void fontDatabaseChanged();
#ifndef QT_NO_SESSIONMANAGER
    void commitDataRequest(QSessionManager &sessionManager);
    void saveStateRequest(QSessionManager &sessionManager);
#endif

public:
    QString styleSheet() const;
public Q_SLOTS:
#ifndef QT_NO_STYLE_STYLESHEET
    void setStyleSheet(const QString& sheet);
#endif
    void setAutoSipEnabled(const bool enabled);
    bool autoSipEnabled() const;
    static void closeAllWindows();
    static void aboutQt();

protected:
    bool event(QEvent *);

private:
    Q_DISABLE_COPY(QApplication)
    Q_DECLARE_PRIVATE(QApplication)

    friend class QGraphicsWidget;
    friend class QGraphicsItem;
    friend class QGraphicsScene;
    friend class QGraphicsScenePrivate;
    friend class QWidget;
    friend class QWidgetPrivate;
    friend class QETWidget;
    friend class QWidgetAnimator;
#ifndef QT_NO_SHORTCUT
    friend class QShortcut;
    friend class QLineEdit;
    friend class QTextControl;
#endif
    friend class QAction;
    friend class QFontDatabasePrivate;

#if defined(Q_WS_X11)
    Q_PRIVATE_SLOT(d_func(), void _q_alertTimeOut())
#endif
};

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<QApplication *>(QCoreApplication::instance()))

QT_END_NAMESPACE


#endif // QAPPLICATION_H
