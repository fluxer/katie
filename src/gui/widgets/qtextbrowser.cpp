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

#include "qtextbrowser.h"
#include "qtextedit_p.h"

#ifndef QT_NO_TEXTBROWSER

#include "qstack.h"
#include "qapplication.h"
#include "qevent.h"
#include "qdesktopwidget.h"
#include "qdebug.h"
#include "qabstracttextdocumentlayout.h"
#include "qtextdocumentlayout_p.h"
#include "qtextcodec.h"
#include "qpainter.h"
#include "qdir.h"
#include "qwhatsthis.h"
#include "qtextobject.h"
#include "qstandardpaths.h"
#include "qcore_unix_p.h"

QT_BEGIN_NAMESPACE

class QTextBrowserPrivate : public QTextEditPrivate
{
    Q_DECLARE_PUBLIC(QTextBrowser)
public:
    inline QTextBrowserPrivate()
        : textOrSourceChanged(false), forceLoadOnSourceChange(false), openExternalLinks(false),
          openLinks(true)
    {}

    void init();

    struct HistoryEntry {
        inline HistoryEntry()
            : hpos(0), vpos(0), focusIndicatorPosition(-1),
              focusIndicatorAnchor(-1) {}
        QUrl url;
        QString title;
        int hpos;
        int vpos;
        int focusIndicatorPosition, focusIndicatorAnchor;
    };

    HistoryEntry history(int i) const
    {
        if (i <= 0)
            if (-i < stack.count())
                return stack[stack.count()+i-1];
            else
                return HistoryEntry();
        else
            if (i <= forwardStack.count())
                return forwardStack[forwardStack.count()-i];
            else
                return HistoryEntry();
    }


    HistoryEntry createHistoryEntry() const;
    void restoreHistoryEntry(const HistoryEntry &entry);

    QStack<HistoryEntry> stack;
    QStack<HistoryEntry> forwardStack;
    QUrl home;
    QUrl currentURL;

    QStringList searchPaths;

    /*flag necessary to give the linkClicked() signal some meaningful
      semantics when somebody connected to it calls setText() or
      setSource() */
    bool textOrSourceChanged;
    bool forceLoadOnSourceChange;

    bool openExternalLinks;
    bool openLinks;

#ifndef QT_NO_CURSOR
    QCursor oldCursor;
#endif

    QString findFile(const QUrl &name) const;

    inline void _q_documentModified()
    {
        textOrSourceChanged = true;
        forceLoadOnSourceChange = !currentURL.path().isEmpty();
    }

    void _q_activateAnchor(const QString &href);
    void _q_highlightLink(const QString &href);

    void setSource(const QUrl &url);

    // re-imlemented from QTextEditPrivate
    virtual QUrl resolveUrl(const QUrl &url) const;
    inline QUrl resolveUrl(const QString &url) const
    { return resolveUrl(QUrl::fromEncoded(url.toUtf8())); }

};

QString QTextBrowserPrivate::findFile(const QUrl &name) const
{
    const QString fileName(name.toLocalFile());

    if (QFileInfo(fileName).isAbsolute())
        return fileName;

    foreach (QString path, searchPaths) {
        if (!path.endsWith(QLatin1Char('/')))
            path.append(QLatin1Char('/'));
        path.append(fileName);
        if (QStatInfo(path).isReadable())
            return path;
    }

    return fileName;
}

QUrl QTextBrowserPrivate::resolveUrl(const QUrl &url) const
{
    if (!url.isRelative())
        return url;

    // For the second case QUrl can merge "#someanchor" with "foo.html"
    // correctly to "foo.html#someanchor"
    if (!(currentURL.isRelative()
          || (currentURL.scheme() == QLatin1String("file")
              && !QFileInfo(currentURL.toLocalFile()).isAbsolute()))
          || (url.hasFragment() && url.path().isEmpty())) {
        return currentURL.resolved(url);
    }

    // this is our last resort when current url and new url are both relative
    // we try to resolve against the current working directory in the local
    // file system.
    QFileInfo fi(currentURL.toLocalFile());
    if (fi.exists()) {
        return QUrl::fromLocalFile(fi.absolutePath() + QDir::separator()).resolved(url);
    }

    return url;
}

void QTextBrowserPrivate::_q_activateAnchor(const QString &href)
{
    if (href.isEmpty())
        return;
    Q_Q(QTextBrowser);

#ifndef QT_NO_CURSOR
    viewport->setCursor(oldCursor);
#endif

    const QUrl url = resolveUrl(href);

    if (!openLinks) {
        emit q->anchorClicked(url);
        return;
    }

    textOrSourceChanged = false;

    if ((openExternalLinks
         && url.scheme() != QLatin1String("file")
         && !url.isRelative())
        || (url.isRelative() && !currentURL.isRelative()
            && currentURL.scheme() != QLatin1String("file"))) {
        QStandardPaths::openUrl(url);
        return;
    }

    emit q->anchorClicked(url);

    if (textOrSourceChanged)
        return;

    q->setSource(url);
}

void QTextBrowserPrivate::_q_highlightLink(const QString &anchor)
{
    Q_Q(QTextBrowser);
    if (anchor.isEmpty()) {
#ifndef QT_NO_CURSOR
        if (viewport->cursor().shape() != Qt::PointingHandCursor)
            oldCursor = viewport->cursor();
        viewport->setCursor(oldCursor);
#endif
        emit q->highlighted(QUrl());
        emit q->highlighted(QString());
    } else {
#ifndef QT_NO_CURSOR
        viewport->setCursor(Qt::PointingHandCursor);
#endif

        const QUrl url = resolveUrl(anchor);
        emit q->highlighted(url);
        // convenience to ease connecting to QStatusBar::showMessage(const QString &)
        emit q->highlighted(url.toString());
    }
}

void QTextBrowserPrivate::setSource(const QUrl &url)
{
    Q_Q(QTextBrowser);
#ifndef QT_NO_CURSOR
    if (q->isVisible())
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textOrSourceChanged = true;

    QString txt;

    bool doSetText = false;

    QUrl currentUrlWithoutFragment = currentURL;
    currentUrlWithoutFragment.setFragment(QString());
    QUrl newUrlWithoutFragment = currentURL.resolved(url);
    newUrlWithoutFragment.setFragment(QString());

    if (url.isValid()
        && (newUrlWithoutFragment != currentUrlWithoutFragment || forceLoadOnSourceChange)) {
        QVariant data = q->loadResource(QTextDocument::HtmlResource, resolveUrl(url));
        if (data.type() == QVariant::String) {
            txt = data.toString();
        } else if (data.type() == QVariant::ByteArray) {
#ifndef QT_NO_TEXTCODEC
            QByteArray ba = data.toByteArray();
            QTextCodec *codec = QTextCodec::codecForHtml(ba);
            txt = codec->toUnicode(ba);
#else
	    txt = data.toString();
#endif
        }
        if (txt.isEmpty())
            qWarning("QTextBrowser: No document for %s", url.toString().toLatin1().constData());

        if (q->isVisible()) {
            QString firstTag = txt.left(txt.indexOf(QLatin1Char('>')) + 1);
            if (firstTag.startsWith(QLatin1String("<qt")) && firstTag.contains(QLatin1String("type")) && firstTag.contains(QLatin1String("detail"))) {
#ifndef QT_NO_CURSOR
                QApplication::restoreOverrideCursor();
#endif
#ifndef QT_NO_WHATSTHIS
                QWhatsThis::showText(QCursor::pos(), txt, q);
#endif
                return;
            }
        }

        currentURL = resolveUrl(url);
        doSetText = true;
    }

    if (!home.isValid())
        home = url;

    if (doSetText) {
#ifndef QT_NO_TEXTHTMLPARSER
        q->QTextEdit::setHtml(txt);
        q->document()->setMetaInformation(QTextDocument::DocumentUrl, currentURL.toString());
#else
        q->QTextEdit::setPlainText(txt);
#endif

    }

    forceLoadOnSourceChange = false;

    if (!url.fragment().isEmpty()) {
        q->scrollToAnchor(url.fragment());
    } else {
        hbar->setValue(0);
        vbar->setValue(0);
    }

#ifndef QT_NO_CURSOR
    if (q->isVisible())
        QApplication::restoreOverrideCursor();
#endif
    emit q->sourceChanged(url);
}


QTextBrowserPrivate::HistoryEntry QTextBrowserPrivate::createHistoryEntry() const
{
    HistoryEntry entry;
    entry.url = q_func()->source();
    entry.title = q_func()->documentTitle();
    entry.hpos = hbar->value();
    entry.vpos = vbar->value();

    const QTextCursor cursor = control->textCursor();
    if (control->cursorIsFocusIndicator()
        && cursor.hasSelection()) {

        entry.focusIndicatorPosition = cursor.position();
        entry.focusIndicatorAnchor = cursor.anchor();
    }
    return entry;
}

void QTextBrowserPrivate::restoreHistoryEntry(const HistoryEntry &entry)
{
    setSource(entry.url);
    hbar->setValue(entry.hpos);
    vbar->setValue(entry.vpos);
    if (entry.focusIndicatorAnchor != -1 && entry.focusIndicatorPosition != -1) {
        QTextCursor cursor(control->document());
        cursor.setPosition(entry.focusIndicatorAnchor);
        cursor.setPosition(entry.focusIndicatorPosition, QTextCursor::KeepAnchor);
        control->setTextCursor(cursor);
        control->setCursorIsFocusIndicator(true);
    }
}

/*!
    \class QTextBrowser
    \brief The QTextBrowser class provides a rich text browser with hypertext navigation.

    \ingroup richtext-processing

    This class extends QTextEdit (in read-only mode), adding some navigation
    functionality so that users can follow links in hypertext documents.

    If you want to provide your users with an editable rich text editor,
    use QTextEdit. If you want a text browser without hypertext navigation
    use QTextEdit, and use QTextEdit::setReadOnly() to disable
    editing. If you just need to display a small piece of rich text
    use QLabel.

    \section1 Document Source and Contents

    The contents of QTextEdit are set with setHtml() or setPlainText(),
    but QTextBrowser also implements the setSource() function, making it
    possible to use a named document as the source text. The name is looked
    up in a list of search paths and in the directory of the current document
    factory.

    If a document name ends with
    an anchor (for example, "\c #anchor"), the text browser automatically
    scrolls to that position (using scrollToAnchor()). When the user clicks
    on a hyperlink, the browser will call setSource() itself with the link's
    \c href value as argument. You can track the current source by connecting
    to the sourceChanged() signal.

    \section1 Navigation

    QTextBrowser provides backward() and forward() slots which you can
    use to implement Back and Forward buttons. The home() slot sets
    the text to the very first document displayed. The anchorClicked()
    signal is emitted when the user clicks an anchor. To override the
    default navigation behavior of the browser, call the setSource()
    function to supply new document text in a slot connected to this
    signal.

    To access local files, use \c{file} as the scheme in the URL.

    \sa QTextEdit, QTextDocument
*/

/*!
    \property QTextBrowser::modified
    \brief whether the contents of the text browser have been modified
*/

/*!
    \property QTextBrowser::readOnly
    \brief whether the text browser is read-only

    By default, this property is true.
*/

/*!
    \property QTextBrowser::undoRedoEnabled
    \brief whether the text browser supports undo/redo operations

    By default, this property is false.
*/

void QTextBrowserPrivate::init()
{
    Q_Q(QTextBrowser);
    control->setTextInteractionFlags(Qt::TextBrowserInteraction);
#ifndef QT_NO_CURSOR
    viewport->setCursor(oldCursor);
#endif
    q->setUndoRedoEnabled(false);
    viewport->setMouseTracking(true);
    QObject::connect(q->document(), SIGNAL(contentsChanged()), q, SLOT(_q_documentModified()));
    QObject::connect(control, SIGNAL(linkActivated(QString)),
                     q, SLOT(_q_activateAnchor(QString)));
    QObject::connect(control, SIGNAL(linkHovered(QString)),
                     q, SLOT(_q_highlightLink(QString)));
}

/*!
    Constructs an empty QTextBrowser with parent \a parent.
*/
QTextBrowser::QTextBrowser(QWidget *parent)
    : QTextEdit(*new QTextBrowserPrivate, parent)
{
    Q_D(QTextBrowser);
    d->init();
}


/*!
    \internal
*/
QTextBrowser::~QTextBrowser()
{
}

/*!
    \property QTextBrowser::source
    \brief the name of the displayed document.

    This is a an invalid url if no document is displayed or if the
    source is unknown.

    When setting this property QTextBrowser tries to find a document
    with the specified name in the paths of the searchPaths property
    and directory of the current source, unless the value is an absolute
    file path. It also checks for optional anchors and scrolls the document
    accordingly

    If the first tag in the document is \c{<qt type=detail>}, the
    document is displayed as a popup rather than as new document in
    the browser window itself. Otherwise, the document is displayed
    normally in the text browser with the text set to the contents of
    the named document with setHtml().

    By default, this property contains an empty URL.
*/
QUrl QTextBrowser::source() const
{
    Q_D(const QTextBrowser);
    if (d->stack.isEmpty())
        return QUrl();
    else
        return d->stack.top().url;
}

/*!
    \property QTextBrowser::searchPaths
    \brief the search paths used by the text browser to find supporting
    content

    QTextBrowser uses this list to locate images and documents.

    By default, this property contains an empty string list.
*/

QStringList QTextBrowser::searchPaths() const
{
    Q_D(const QTextBrowser);
    return d->searchPaths;
}

void QTextBrowser::setSearchPaths(const QStringList &paths)
{
    Q_D(QTextBrowser);
    d->searchPaths = paths;
}

/*!
    Reloads the current set source.
*/
void QTextBrowser::reload()
{
    Q_D(QTextBrowser);
    QUrl s = d->currentURL;
    d->currentURL = QUrl();
    setSource(s);
}

void QTextBrowser::setSource(const QUrl &url)
{
    Q_D(QTextBrowser);

    const QTextBrowserPrivate::HistoryEntry historyEntry = d->createHistoryEntry();

    d->setSource(url);

    if (!url.isValid())
        return;

    // the same url you are already watching?
    if (!d->stack.isEmpty() && d->stack.top().url == url)
        return;

    if (!d->stack.isEmpty())
        d->stack.top() = historyEntry;

    QTextBrowserPrivate::HistoryEntry entry;
    entry.url = url;
    entry.title = documentTitle();
    entry.hpos = 0;
    entry.vpos = 0;
    d->stack.push(entry);

    emit backwardAvailable(d->stack.count() > 1);

    if (!d->forwardStack.isEmpty() && d->forwardStack.top().url == url) {
        d->forwardStack.pop();
        emit forwardAvailable(d->forwardStack.count() > 0);
    } else {
        d->forwardStack.clear();
        emit forwardAvailable(false);
    }

    emit historyChanged();
}

/*!
    \fn void QTextBrowser::backwardAvailable(bool available)

    This signal is emitted when the availability of backward()
    changes. \a available is false when the user is at home();
    otherwise it is true.
*/

/*!
    \fn void QTextBrowser::forwardAvailable(bool available)

    This signal is emitted when the availability of forward() changes.
    \a available is true after the user navigates backward() and false
    when the user navigates or goes forward().
*/

/*!
    \fn void QTextBrowser::historyChanged()
    \since 4.4

    This signal is emitted when the history changes.

    \sa historyTitle(), historyUrl()
*/

/*!
    \fn void QTextBrowser::sourceChanged(const QUrl &src)

    This signal is emitted when the source has changed, \a src
    being the new source.

    Source changes happen both programmatically when calling
    setSource(), forward(), backword() or home() or when the user
    clicks on links or presses the equivalent key sequences.
*/

/*!  \fn void QTextBrowser::highlighted(const QUrl &link)

    This signal is emitted when the user has selected but not
    activated an anchor in the document. The URL referred to by the
    anchor is passed in \a link.
*/

/*!  \fn void QTextBrowser::highlighted(const QString &link)
     \overload

     Convenience signal that allows connecting to a slot
     that takes just a QString, like for example QStatusBar's
     message().
*/


/*!
    \fn void QTextBrowser::anchorClicked(const QUrl &link)

    This signal is emitted when the user clicks an anchor. The
    URL referred to by the anchor is passed in \a link.

    Note that the browser will automatically handle navigation to the
    location specified by \a link unless the openLinks property
    is set to false or you call setSource() in a slot connected.
    This mechanism is used to override the default navigation features of the browser.
*/

/*!
    Changes the document displayed to the previous document in the
    list of documents built by navigating links. Does nothing if there
    is no previous document.

    \sa forward(), backwardAvailable()
*/
void QTextBrowser::backward()
{
    Q_D(QTextBrowser);
    if (d->stack.count() <= 1)
        return;

    // Update the history entry
    d->forwardStack.push(d->createHistoryEntry());
    d->stack.pop(); // throw away the old version of the current entry
    d->restoreHistoryEntry(d->stack.top()); // previous entry
    emit backwardAvailable(d->stack.count() > 1);
    emit forwardAvailable(true);
    emit historyChanged();
}

/*!
    Changes the document displayed to the next document in the list of
    documents built by navigating links. Does nothing if there is no
    next document.

    \sa backward(), forwardAvailable()
*/
void QTextBrowser::forward()
{
    Q_D(QTextBrowser);
    if (d->forwardStack.isEmpty())
        return;
    if (!d->stack.isEmpty()) {
        // Update the history entry
        d->stack.top() = d->createHistoryEntry();
    }
    d->stack.push(d->forwardStack.pop());
    d->restoreHistoryEntry(d->stack.top());
    emit backwardAvailable(true);
    emit forwardAvailable(!d->forwardStack.isEmpty());
    emit historyChanged();
}

/*!
    Changes the document displayed to be the first document from
    the history.
*/
void QTextBrowser::home()
{
    Q_D(QTextBrowser);
    if (d->home.isValid())
        setSource(d->home);
}

/*!
    The event \a ev is used to provide the following keyboard shortcuts:
    \table
    \header \i Keypress            \i Action
    \row \i Alt+Left Arrow  \i \l backward()
    \row \i Alt+Right Arrow \i \l forward()
    \row \i Alt+Up Arrow    \i \l home()
    \endtable
*/
void QTextBrowser::keyPressEvent(QKeyEvent *ev)
{

    if (ev->modifiers() & Qt::AltModifier) {
        switch (ev->key()) {
        case Qt::Key_Right:
            forward();
            ev->accept();
            return;
        case Qt::Key_Left:
            backward();
            ev->accept();
            return;
        case Qt::Key_Up:
            home();
            ev->accept();
            return;
        }
    }
    QTextEdit::keyPressEvent(ev);
}

/*!
    \reimp
*/
void QTextBrowser::focusOutEvent(QFocusEvent *ev)
{
#ifndef QT_NO_CURSOR
    Q_D(QTextBrowser);
    d->viewport->setCursor((!(d->control->textInteractionFlags() & Qt::TextEditable)) ? d->oldCursor : Qt::IBeamCursor);
#endif
    QTextEdit::focusOutEvent(ev);
}

/*!
    \reimp
*/
bool QTextBrowser::focusNextPrevChild(bool next)
{
    Q_D(QTextBrowser);
    if (d->control->setFocusToNextOrPreviousAnchor(next)) {
        return true;
    } else {
    }
    return QTextEdit::focusNextPrevChild(next);
}

/*!
  \reimp
*/
void QTextBrowser::paintEvent(QPaintEvent *e)
{
    Q_D(QTextBrowser);
    QPainter p(d->viewport);
    d->paint(&p, e);
}

/*!
    This function is called when the document is loaded and for
    each image in the document. The \a type indicates the type of resource
    to be loaded. An invalid QVariant is returned if the resource cannot be
    loaded.

    The default implementation ignores \a type and tries to locate
    the resources by interpreting \a name as a file name. If it is
    not an absolute path it tries to find the file in the paths of
    the \l searchPaths property and in the same directory as the
    current source. On success, the result is a QVariant that stores
    a QByteArray with the contents of the file.

    If you reimplement this function, you can return other QVariant
    types. The table below shows which variant types are supported
    depending on the resource type:

    \table
    \header \i ResourceType  \i QVariant::Type
    \row    \i QTextDocument::HtmlResource  \i QString or QByteArray
    \row    \i QTextDocument::ImageResource \i QImage, QPixmap or QByteArray
    \row    \i QTextDocument::StyleSheetResource \i QString or QByteArray
    \endtable
*/
QVariant QTextBrowser::loadResource(int /*type*/, const QUrl &name)
{
    Q_D(QTextBrowser);

    QByteArray data;
    QString fileName = d->findFile(d->resolveUrl(name));
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        data = f.readAll();
        f.close();
    } else {
        return QVariant();
    }

    return data;
}

/*!
    \since 4.2

    Returns true if the text browser can go backward in the document history
    using backward().

    \sa backwardAvailable(), backward()
*/
bool QTextBrowser::isBackwardAvailable() const
{
    Q_D(const QTextBrowser);
    return d->stack.count() > 1;
}

/*!
    \since 4.2

    Returns true if the text browser can go forward in the document history
    using forward().

    \sa forwardAvailable(), forward()
*/
bool QTextBrowser::isForwardAvailable() const
{
    Q_D(const QTextBrowser);
    return !d->forwardStack.isEmpty();
}

/*!
    \since 4.2

    Clears the history of visited documents and disables the forward and
    backward navigation.

    \sa backward(), forward()
*/
void QTextBrowser::clearHistory()
{
    Q_D(QTextBrowser);
    d->forwardStack.clear();
    if (!d->stack.isEmpty()) {
        QTextBrowserPrivate::HistoryEntry historyEntry = d->stack.top();
        d->stack.resize(0);
        d->stack.push(historyEntry);
        d->home = historyEntry.url;
    }
    emit forwardAvailable(false);
    emit backwardAvailable(false);
    emit historyChanged();
}

/*!
   Returns the url of the HistoryItem.

    \table
    \header \i Input            \i Return
    \row \i \a{i} < 0  \i \l backward() history
    \row \i\a{i} == 0 \i current, see QTextBrowser::source()
    \row \i \a{i} > 0  \i \l forward() history
    \endtable

    \since 4.4
*/
QUrl QTextBrowser::historyUrl(int i) const
{
    Q_D(const QTextBrowser);
    return d->history(i).url;
}

/*!
    Returns the documentTitle() of the HistoryItem.

    \table
    \header \i Input            \i Return
    \row \i \a{i} < 0  \i \l backward() history
    \row \i \a{i} == 0 \i current, see QTextBrowser::source()
    \row \i \a{i} > 0  \i \l forward() history
    \endtable

    \snippet doc/src/snippets/code/src_gui_widgets_qtextbrowser.cpp 0

    \since 4.4
*/
QString QTextBrowser::historyTitle(int i) const
{
    Q_D(const QTextBrowser);
    return d->history(i).title;
}


/*!
    Returns the number of locations forward in the history.

    \since 4.4
*/
int QTextBrowser::forwardHistoryCount() const
{
    Q_D(const QTextBrowser);
    return d->forwardStack.count();
}

/*!
    Returns the number of locations backward in the history.

    \since 4.4
*/
int QTextBrowser::backwardHistoryCount() const
{
    Q_D(const QTextBrowser);
    return d->stack.count()-1;
}

/*!
    \property QTextBrowser::openExternalLinks
    \since 4.2

    Specifies whether QTextBrowser should automatically open links to external
    sources using QStandardPaths::openUrl() instead of emitting the
    anchorClicked signal. Links are considered external if their scheme is
    not file.

    The default value is false.
*/
bool QTextBrowser::openExternalLinks() const
{
    Q_D(const QTextBrowser);
    return d->openExternalLinks;
}

void QTextBrowser::setOpenExternalLinks(bool open)
{
    Q_D(QTextBrowser);
    d->openExternalLinks = open;
}

/*!
   \property QTextBrowser::openLinks
   \since 4.3

   This property specifies whether QTextBrowser should automatically open links the user tries to
   activate by mouse or keyboard.

   Regardless of the value of this property the anchorClicked signal is always emitted.

   The default value is true.
*/

bool QTextBrowser::openLinks() const
{
    Q_D(const QTextBrowser);
    return d->openLinks;
}

void QTextBrowser::setOpenLinks(bool open)
{
    Q_D(QTextBrowser);
    d->openLinks = open;
}

QT_END_NAMESPACE


#include "moc_qtextbrowser.h"

#endif // QT_NO_TEXTBROWSER
