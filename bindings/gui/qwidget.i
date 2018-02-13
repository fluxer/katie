%{
#include "QtGui/qwidget.h"
QT_USE_NAMESPACE
%}

class QWidget : public QObject, public QPaintDevice
{
    Q_OBJECT

    Q_PROPERTY(bool modal READ isModal)
    Q_PROPERTY(Qt::WindowModality windowModality READ windowModality WRITE setWindowModality)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(QRect geometry READ geometry WRITE setGeometry)
    Q_PROPERTY(QRect frameGeometry READ frameGeometry)
    Q_PROPERTY(QRect normalGeometry READ normalGeometry)
    Q_PROPERTY(int x READ x)
    Q_PROPERTY(int y READ y)
    Q_PROPERTY(QPoint pos READ pos WRITE move DESIGNABLE false STORED false)
    Q_PROPERTY(QSize frameSize READ frameSize)
    Q_PROPERTY(QSize size READ size WRITE resize DESIGNABLE false STORED false)
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)
    Q_PROPERTY(QRect rect READ rect)
    Q_PROPERTY(QRect childrenRect READ childrenRect)
    Q_PROPERTY(QRegion childrenRegion READ childrenRegion)
    Q_PROPERTY(QSizePolicy sizePolicy READ sizePolicy WRITE setSizePolicy)
    Q_PROPERTY(QSize minimumSize READ minimumSize WRITE setMinimumSize)
    Q_PROPERTY(QSize maximumSize READ maximumSize WRITE setMaximumSize)
    Q_PROPERTY(int minimumWidth READ minimumWidth WRITE setMinimumWidth STORED false DESIGNABLE false)
    Q_PROPERTY(int minimumHeight READ minimumHeight WRITE setMinimumHeight STORED false DESIGNABLE false)
    Q_PROPERTY(int maximumWidth READ maximumWidth WRITE setMaximumWidth STORED false DESIGNABLE false)
    Q_PROPERTY(int maximumHeight READ maximumHeight WRITE setMaximumHeight STORED false DESIGNABLE false)
    Q_PROPERTY(QSize sizeIncrement READ sizeIncrement WRITE setSizeIncrement)
    Q_PROPERTY(QSize baseSize READ baseSize WRITE setBaseSize)
    Q_PROPERTY(QPalette palette READ palette WRITE setPalette)
    Q_PROPERTY(QFont font READ font WRITE setFont)
#ifndef QT_NO_CURSOR
    Q_PROPERTY(QCursor cursor READ cursor WRITE setCursor RESET unsetCursor)
#endif
    Q_PROPERTY(bool mouseTracking READ hasMouseTracking WRITE setMouseTracking)
    Q_PROPERTY(bool isActiveWindow READ isActiveWindow)
    Q_PROPERTY(Qt::FocusPolicy focusPolicy READ focusPolicy WRITE setFocusPolicy)
    Q_PROPERTY(bool focus READ hasFocus)
    Q_PROPERTY(Qt::ContextMenuPolicy contextMenuPolicy READ contextMenuPolicy WRITE setContextMenuPolicy)
    Q_PROPERTY(bool updatesEnabled READ updatesEnabled WRITE setUpdatesEnabled DESIGNABLE false)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible DESIGNABLE false)
    Q_PROPERTY(bool minimized READ isMinimized)
    Q_PROPERTY(bool maximized READ isMaximized)
    Q_PROPERTY(bool fullScreen READ isFullScreen)
    Q_PROPERTY(QSize sizeHint READ sizeHint)
    Q_PROPERTY(QSize minimumSizeHint READ minimumSizeHint)
    Q_PROPERTY(bool acceptDrops READ acceptDrops WRITE setAcceptDrops)
    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle DESIGNABLE isWindow)
    Q_PROPERTY(QIcon windowIcon READ windowIcon WRITE setWindowIcon DESIGNABLE isWindow)
    Q_PROPERTY(QString windowIconText READ windowIconText WRITE setWindowIconText DESIGNABLE isWindow)
    Q_PROPERTY(double windowOpacity READ windowOpacity WRITE setWindowOpacity DESIGNABLE isWindow)
    Q_PROPERTY(bool windowModified READ isWindowModified WRITE setWindowModified DESIGNABLE isWindow)
#ifndef QT_NO_TOOLTIP
    Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
#endif
#ifndef QT_NO_STATUSTIP
    Q_PROPERTY(QString statusTip READ statusTip WRITE setStatusTip)
#endif
#ifndef QT_NO_WHATSTHIS
    Q_PROPERTY(QString whatsThis READ whatsThis WRITE setWhatsThis)
#endif
#ifndef QT_NO_ACCESSIBILITY
    Q_PROPERTY(QString accessibleName READ accessibleName WRITE setAccessibleName)
    Q_PROPERTY(QString accessibleDescription READ accessibleDescription WRITE setAccessibleDescription)
#endif
    Q_PROPERTY(Qt::LayoutDirection layoutDirection READ layoutDirection WRITE setLayoutDirection RESET unsetLayoutDirection)
    QDOC_PROPERTY(Qt::WindowFlags windowFlags READ windowFlags WRITE setWindowFlags)
    Q_PROPERTY(bool autoFillBackground READ autoFillBackground WRITE setAutoFillBackground)
#ifndef QT_NO_STYLE_STYLESHEET
    Q_PROPERTY(QString styleSheet READ styleSheet WRITE setStyleSheet)
#endif
    Q_PROPERTY(QLocale locale READ locale WRITE setLocale RESET unsetLocale)
    Q_PROPERTY(QString windowFilePath READ windowFilePath WRITE setWindowFilePath DESIGNABLE isWindow)

public:
    enum RenderFlags {
        DrawWindowBackground = 0x1,
        DrawChildren = 0x2,
        IgnoreMask = 0x4
    };

    QWidget(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = 0);
    ~QWidget();

    int devType() const;

    WId winId() const;
    void createWinId(); // internal, going away
    WId internalWinId() const;
    WId effectiveWinId() const;

    // GUI style setting
    QStyle *style() const;
    void setStyle(QStyle *);
    // Widget types and states

    bool isTopLevel() const;
    bool isWindow() const;

    bool isModal() const;
    Qt::WindowModality windowModality() const;
    void setWindowModality(Qt::WindowModality windowModality);

    bool isEnabled() const;
    bool isEnabledTo(QWidget*) const;
    bool isEnabledToTLW() const;

public Q_SLOTS:
    void setEnabled(bool);
    void setDisabled(bool);
    void setWindowModified(bool);

    // Widget coordinates

public:
    QRect frameGeometry() const;
    const QRect &geometry() const;
    QRect normalGeometry() const;

    int x() const;
    int y() const;
    QPoint pos() const;
    QSize frameSize() const;
    QSize size() const;
    int width() const;
    int height() const;
    QRect rect() const;
    QRect childrenRect() const;
    QRegion childrenRegion() const;

    QSize minimumSize() const;
    QSize maximumSize() const;
    int minimumWidth() const;
    int minimumHeight() const;
    int maximumWidth() const;
    int maximumHeight() const;
    void setMinimumSize(const QSize &);
    void setMinimumSize(int minw, int minh);
    void setMaximumSize(const QSize &);
    void setMaximumSize(int maxw, int maxh);
    void setMinimumWidth(int minw);
    void setMinimumHeight(int minh);
    void setMaximumWidth(int maxw);
    void setMaximumHeight(int maxh);


    QSize sizeIncrement() const;
    void setSizeIncrement(const QSize &);
    void setSizeIncrement(int w, int h);
    QSize baseSize() const;
    void setBaseSize(const QSize &);
    void setBaseSize(int basew, int baseh);

    void setFixedSize(const QSize &);
    void setFixedSize(int w, int h);
    void setFixedWidth(int w);
    void setFixedHeight(int h);

    // Widget coordinate mapping

    QPoint mapToGlobal(const QPoint &) const;
    QPoint mapFromGlobal(const QPoint &) const;
    QPoint mapToParent(const QPoint &) const;
    QPoint mapFromParent(const QPoint &) const;
    QPoint mapTo(QWidget *, const QPoint &) const;
    QPoint mapFrom(QWidget *, const QPoint &) const;

    QWidget *window() const;
    QWidget *nativeParentWidget() const;
    QWidget *topLevelWidget() const;

    // Widget appearance functions
    const QPalette &palette() const;
    void setPalette(const QPalette &);

    void setBackgroundRole(QPalette::ColorRole);
    QPalette::ColorRole backgroundRole() const;

    void setForegroundRole(QPalette::ColorRole);
    QPalette::ColorRole foregroundRole() const;

    const QFont &font() const;
    void setFont(const QFont &);
    QFontMetrics fontMetrics() const;
    QFontInfo fontInfo() const;

#ifndef QT_NO_CURSOR
    QCursor cursor() const;
    void setCursor(const QCursor &);
    void unsetCursor();
#endif

    void setMouseTracking(bool enable);
    bool hasMouseTracking() const;
    bool underMouse() const;

    void setMask(const QBitmap &);
    void setMask(const QRegion &);
    QRegion mask() const;
    void clearMask();

    void render(QPaintDevice *target, const QPoint &targetOffset = QPoint(),
                const QRegion &sourceRegion = QRegion(),
                RenderFlags renderFlags = RenderFlags(DrawWindowBackground | DrawChildren));

    void render(QPainter *painter, const QPoint &targetOffset = QPoint(),
                const QRegion &sourceRegion = QRegion(),
                RenderFlags renderFlags = RenderFlags(DrawWindowBackground | DrawChildren));

#ifndef QT_NO_GRAPHICSEFFECT
    QGraphicsEffect *graphicsEffect() const;
    void setGraphicsEffect(QGraphicsEffect *effect);
#endif //QT_NO_GRAPHICSEFFECT

#ifndef QT_NO_GESTURES
    void grabGesture(Qt::GestureType type, Qt::GestureFlags flags = Qt::GestureFlags());
    void ungrabGesture(Qt::GestureType type);
#endif

public Q_SLOTS:
    void setWindowTitle(const QString &);
#ifndef QT_NO_STYLE_STYLESHEET
    void setStyleSheet(const QString& styleSheet);
#endif
public:
#ifndef QT_NO_STYLE_STYLESHEET
    QString styleSheet() const;
#endif
    QString windowTitle() const;
    void setWindowIcon(const QIcon &icon);
    QIcon windowIcon() const;
    void setWindowIconText(const QString &);
    QString windowIconText() const;
    void setWindowRole(const QString &);
    QString windowRole() const;
    void setWindowFilePath(const QString &filePath);
    QString windowFilePath() const;

    void setWindowOpacity(qreal level);
    qreal windowOpacity() const;

    bool isWindowModified() const;
#ifndef QT_NO_TOOLTIP
    void setToolTip(const QString &);
    QString toolTip() const;
#endif
#ifndef QT_NO_STATUSTIP
    void setStatusTip(const QString &);
    QString statusTip() const;
#endif
#ifndef QT_NO_WHATSTHIS
    void setWhatsThis(const QString &);
    QString whatsThis() const;
#endif
#ifndef QT_NO_ACCESSIBILITY
    QString accessibleName() const;
    void setAccessibleName(const QString &name);
    QString accessibleDescription() const;
    void setAccessibleDescription(const QString &description);
#endif

    void setLayoutDirection(Qt::LayoutDirection direction);
    Qt::LayoutDirection layoutDirection() const;
    void unsetLayoutDirection();

    void setLocale(const QLocale &locale);
    QLocale locale() const;
    void unsetLocale();

    bool isRightToLeft() const;
    bool isLeftToRight() const;

public Q_SLOTS:
    void setFocus();

public:
    bool isActiveWindow() const;
    void activateWindow();
    void clearFocus();

    void setFocus(Qt::FocusReason reason);
    Qt::FocusPolicy focusPolicy() const;
    void setFocusPolicy(Qt::FocusPolicy policy);
    bool hasFocus() const;
    static void setTabOrder(QWidget *, QWidget *);
    void setFocusProxy(QWidget *);
    QWidget *focusProxy() const;
    Qt::ContextMenuPolicy contextMenuPolicy() const;
    void setContextMenuPolicy(Qt::ContextMenuPolicy policy);

    // Grab functions
    void grabMouse();
#ifndef QT_NO_CURSOR
    void grabMouse(const QCursor &);
#endif
    void releaseMouse();
    void grabKeyboard();
    void releaseKeyboard();
#ifndef QT_NO_SHORTCUT
    int grabShortcut(const QKeySequence &key, Qt::ShortcutContext context = Qt::WindowShortcut);
    void releaseShortcut(int id);
    void setShortcutEnabled(int id, bool enable = true);
    void setShortcutAutoRepeat(int id, bool enable = true);
#endif
    static QWidget *mouseGrabber();
    static QWidget *keyboardGrabber();

    // Update/refresh functions
    bool updatesEnabled() const;
    void setUpdatesEnabled(bool enable);

#ifndef QT_NO_GRAPHICSVIEW
    QGraphicsProxyWidget *graphicsProxyWidget() const;
#endif

public Q_SLOTS:
    void update();
    void repaint();

public:
    void update(int x, int y, int w, int h);
    void update(const QRect&);
    void update(const QRegion&);

    void repaint(int x, int y, int w, int h);
    void repaint(const QRect &);
    void repaint(const QRegion &);

public Q_SLOTS:
    // Widget management functions

    virtual void setVisible(bool visible);
    void setHidden(bool hidden);
    void show();
    void hide();

    void showMinimized();
    void showMaximized();
    void showFullScreen();
    void showNormal();

    bool close();
    void raise();
    void lower();

public:
    void stackUnder(QWidget*);
    void move(int x, int y);
    void move(const QPoint &);
    void resize(int w, int h);
    void resize(const QSize &);
    void setGeometry(int x, int y, int w, int h);
    void setGeometry(const QRect &);
    QByteArray saveGeometry() const;
    bool restoreGeometry(const QByteArray &geometry);
    void adjustSize();
    bool isVisible() const;
    bool isVisibleTo(const QWidget*) const;
    bool isHidden() const;

    bool isMinimized() const;
    bool isMaximized() const;
    bool isFullScreen() const;

    Qt::WindowStates windowState() const;
    void setWindowState(Qt::WindowStates state);
    void overrideWindowState(Qt::WindowStates state);

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    QSizePolicy sizePolicy() const;
    void setSizePolicy(QSizePolicy);
    void setSizePolicy(QSizePolicy::Policy horizontal, QSizePolicy::Policy vertical);
    virtual int heightForWidth(int) const;

    QRegion visibleRegion() const;

    void setContentsMargins(int left, int top, int right, int bottom);
    void setContentsMargins(const QMargins &margins);
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
    QMargins contentsMargins() const;

    QRect contentsRect() const;

public:
    QLayout *layout() const;
    void setLayout(QLayout *);
    void updateGeometry();

    void setParent(QWidget *parent);
    void setParent(QWidget *parent, Qt::WindowFlags f);

    void scroll(int dx, int dy);
    void scroll(int dx, int dy, const QRect&);

    // Misc. functions

    QWidget *focusWidget() const;
    QWidget *nextInFocusChain() const;
    QWidget *previousInFocusChain() const;

    // drag and drop
    bool acceptDrops() const;
    void setAcceptDrops(bool on);

#ifndef QT_NO_ACTION
    //actions
    void addAction(QAction *action);
    void addActions(QList<QAction*> actions);
    void insertAction(QAction *before, QAction *action);
    void insertActions(QAction *before, QList<QAction*> actions);
    void removeAction(QAction *action);
    QList<QAction*> actions() const;
#endif

    QWidget *parentWidget() const;

    void setWindowFlags(Qt::WindowFlags type);
    Qt::WindowFlags windowFlags() const;
    void overrideWindowFlags(Qt::WindowFlags type);

    Qt::WindowType windowType() const;

    static QWidget *find(WId);
    QWidget *childAt(int x, int y) const;
    QWidget *childAt(const QPoint &p) const;

#if defined(Q_WS_X11)
    const QX11Info &x11Info() const;
    Qt::HANDLE x11PictureHandle() const;
#endif

    Qt::HANDLE handle() const;

    void setAttribute(Qt::WidgetAttribute, bool on = true);
    bool testAttribute(Qt::WidgetAttribute) const;

    QPaintEngine *paintEngine() const;

    void ensurePolished() const;
    bool isAncestorOf(const QWidget *child) const;

#ifdef QT_KEYPAD_NAVIGATION
    bool hasEditFocus() const;
    void setEditFocus(bool on);
#endif

    bool autoFillBackground() const;
    void setAutoFillBackground(bool enabled);

    void setWindowSurface(QWindowSurface *surface);
    QWindowSurface *windowSurface() const;

Q_SIGNALS:
    void customContextMenuRequested(const QPoint &pos);

protected:
    // Event handlers
    bool event(QEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
#ifndef QT_NO_WHEELEVENT
    virtual void wheelEvent(QWheelEvent *);
#endif
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void moveEvent(QMoveEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual void closeEvent(QCloseEvent *);
#ifndef QT_NO_CONTEXTMENU
    virtual void contextMenuEvent(QContextMenuEvent *);
#endif
#ifndef QT_NO_ACTION
    virtual void actionEvent(QActionEvent *);
#endif

#ifndef QT_NO_DRAGANDDROP
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dragMoveEvent(QDragMoveEvent *);
    virtual void dragLeaveEvent(QDragLeaveEvent *);
    virtual void dropEvent(QDropEvent *);
#endif

    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

#if defined(Q_WS_X11)
    virtual bool x11Event(XEvent *);
#endif

    // Misc. protected functions
    virtual void changeEvent(QEvent *);

    int metric(PaintDeviceMetric) const;

protected Q_SLOTS:
    void updateMicroFocus();

protected:
    void create(WId = 0, bool initializeWindow = true,
                         bool destroyOldWindow = true);
    void destroy(bool destroyWindow = true,
                 bool destroySubWindows = true);

    virtual bool focusNextPrevChild(bool next);
    bool focusNextChild();
    bool focusPreviousChild();

    QWidget(QWidgetPrivate &d, QWidget* parent, Qt::WindowFlags f);

    virtual void styleChange(QStyle&); // compat
    virtual void enabledChange(bool);  // compat
    virtual void paletteChange(const QPalette &);  // compat
    virtual void fontChange(const QFont &); // compat
    virtual void windowActivationChange(bool);  // compat
    virtual void languageChange();  // compat
};

template <> QWidget *qobject_cast<QWidget*>(QObject *o);
template <> const QWidget *qobject_cast<const QWidget*>(const QObject *o);
