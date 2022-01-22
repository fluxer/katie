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

#include "qtextlayout.h"
#include "qtextengine_p.h"

#include "qfont.h"
#include "qapplication.h"
#include "qmath.h"
#include "qpainter.h"
#include "qvarlengtharray.h"
#include "qtextformat.h"
#include "qabstracttextdocumentlayout.h"
#include "qtextdocument_p.h"
#include "qtextformat_p.h"
#include "qstyleoption.h"
#include "qpainterpath.h"
#include "qdebug.h"
#include "qtextboundaryfinder.h"
#include "qfontengine_p.h"
#include "qfontengine_ft_p.h"
#include "qcorecommon_p.h"

#include <limits.h>

QT_BEGIN_NAMESPACE

#define ObjectSelectionBrush (QTextFormat::ForegroundBrush + 1)
#define SuppressText 0x5012
#define SuppressBackground 0x513

/*!
    \class QTextLayout::FormatRange
    \reentrant

    \brief The QTextLayout::FormatRange structure is used to apply extra formatting information
    for a specified area in the text layout's content.

    \sa QTextLayout::setAdditionalFormats(), QTextLayout::draw()
*/

/*!
    \variable QTextLayout::FormatRange::start
    Specifies the beginning of the format range within the text layout's text.
*/

/*!
    \variable QTextLayout::FormatRange::length
    Specifies the numer of characters the format range spans.
*/

/*!
    \variable QTextLayout::FormatRange::format
    Specifies the format to apply.
*/

/*!
    \class QTextInlineObject
    \reentrant

    \brief The QTextInlineObject class represents an inline object in
    a QTextLayout.

    \ingroup richtext-processing

    This class is only used if the text layout is used to lay out
    parts of a QTextDocument.

    The inline object has various attributes that can be set, for
    example using, setWidth(), setAscent(), and setDescent(). The
    rectangle it occupies is given by rect(), and its direction by
    isRightToLeft(). Its position in the text layout is given by at(),
    and its format is given by format().
*/

/*!
    \fn QTextInlineObject::QTextInlineObject(int i, QTextEngine *e)

    Creates a new inline object for the item at position \a i in the
    text engine \a e.
*/

/*!
    \fn QTextInlineObject::QTextInlineObject()

    \internal
*/

/*!
    \fn bool QTextInlineObject::isValid() const

    Returns true if this inline object is valid; otherwise returns
    false.
*/

/*!
    Returns the inline object's rectangle.

    \sa ascent(), descent(), width()
*/
QRectF QTextInlineObject::rect() const
{
    if (!eng || eng->lines.isEmpty()) {
        return QRectF();
    }
    return QRectF(
        eng->lines[i].x.toReal(), eng->lines[i].y.toReal(),
        eng->lines[i].width.toReal(), eng->lines[i].height.toReal()
    );
}

/*!
    Returns the inline object's width.

    \sa ascent(), descent(), rect()
*/
qreal QTextInlineObject::width() const
{
    return rect().width();
}

/*!
    Returns the inline object's ascent.

    \sa descent(), width(), rect()
*/
qreal QTextInlineObject::ascent() const
{
    if (!eng || eng->lines.isEmpty()) {
        return qreal(0.0);
    }
    return eng->lines[i].ascent.toReal();
}

/*!
    Returns the inline object's descent.

    \sa ascent(), width(), rect()
*/
qreal QTextInlineObject::descent() const
{
    if (!eng || eng->lines.isEmpty()) {
        return qreal(0.0);
    }
    return eng->lines[i].descent.toReal();
}

/*!
    Returns the inline object's total height. This is equal to
    ascent() + descent() + 1.

    \sa ascent(), descent(), width(), rect()
*/
qreal QTextInlineObject::height() const
{
    return rect().height();
}

/*!
    Sets the inline object's width to \a w.

    \sa width(), ascent(), descent(), rect()
*/
void QTextInlineObject::setWidth(qreal w)
{
    eng->lines[i].width = QFixed::fromReal(w);
    eng->updateLine(i);
}

/*!
    Sets the inline object's ascent to \a a.

    \sa ascent(), setDescent(), width(), rect()
*/
void QTextInlineObject::setAscent(qreal a)
{
    // TODO: implement
}

/*!
    Sets the inline object's decent to \a d.

    \sa descent(), setAscent(), width(), rect()
*/
void QTextInlineObject::setDescent(qreal d)
{
    // TODO: implement
}

/*!
    The position of the inline object within the text layout.
*/
int QTextInlineObject::textPosition() const
{
    if (!eng || eng->lines.isEmpty()) {
        return qreal(0.0);
    }
    return eng->lines[i].position;
}

/*!
    Returns an integer describing the format of the inline object
    within the text layout.
*/
int QTextInlineObject::formatIndex() const
{
    // TODO: implement
    return 0;
}

/*!
    Returns format of the inline object within the text layout.
*/
QTextFormat QTextInlineObject::format() const
{
    // TODO: implement
    return QTextFormat();
}

/*!
    Returns if the object should be laid out right-to-left or left-to-right.
*/
Qt::LayoutDirection QTextInlineObject::textDirection() const
{
    // TODO: implement
    return Qt::LeftToRight;
}

/*!
    \class QTextLayout
    \reentrant

    \brief The QTextLayout class is used to lay out and render text.

    \ingroup richtext-processing

    It offers many features expected from a modern text layout
    engine, including Unicode compliant rendering, line breaking and
    handling of cursor positioning. It can also produce and render
    device independent layout, something that is important for WYSIWYG
    applications.

    The class has a rather low level API and unless you intend to
    implement your own text rendering for some specialized widget, you
    probably won't need to use it directly.

    QTextLayout can be used with both plain and rich text.

    QTextLayout can be used to create a sequence of QTextLine
    instances with given widths and can position them independently
    on the screen. Once the layout is done, these lines can be drawn
    on a paint device.

    The text to be laid out can be provided in the constructor or set with
    setText().

    The layout can be seen as a sequence of QTextLine objects; use createLine()
    to create a QTextLine instance, and lineAt() or lineForTextPosition() to retrieve
    created lines.

    Here is a code snippet that demonstrates the layout phase:
    \snippet doc/src/snippets/code/src_gui_text_qtextlayout.cpp 0

    The text can then be rendered by calling the layout's draw() function:
    \snippet doc/src/snippets/code/src_gui_text_qtextlayout.cpp 1

    For a given position in the text you can find a valid cursor position with
    isValidCursorPosition(), nextCursorPosition(), and previousCursorPosition().

    The QTextLayout itself can be positioned with setPosition(); it has a
    boundingRect(), and a minimumWidth() and a maximumWidth().
*/

/*!
    \enum QTextLayout::CursorMode

    \value SkipCharacters
    \value SkipWords
*/

/*!
    \fn QTextEngine *QTextLayout::engine() const
    \internal

    Returns the text engine used to render the text layout.
*/

/*!
    Constructs an empty text layout.

    \sa setText()
*/
QTextLayout::QTextLayout()
    : d(new QTextEngine())
{
}

/*!
    Constructs a text layout to lay out the given \a text.
*/
QTextLayout::QTextLayout(const QString& text)
    : d(new QTextEngine())
{
    d->text = text;
}

/*!
    Constructs a text layout to lay out the given \a text with the specified
    \a font.

    All the metric and layout calculations will be done in terms of
    the paint device, \a paintdevice. If \a paintdevice is 0 the
    calculations will be done in screen metrics.
*/
QTextLayout::QTextLayout(const QString& text, const QFont &font, QPaintDevice *paintdevice)
{
    QFont f(font);
    if (paintdevice)
        f = QFont(font, paintdevice);
    d = new QTextEngine(text, f);
}

/*!
    \internal
    Constructs a text layout to lay out the given \a block.
*/
QTextLayout::QTextLayout(const QTextBlock &block)
    : d(new QTextEngine())
{
    d->block = block;
    if (d->block.isValid()) {
        d->createLinesForBlock();
    }
    // qDebug() << Q_FUNC_INFO << d->text << block.isValid();
}

/*!
    Destructs the layout.
*/
QTextLayout::~QTextLayout()
{
    delete d;
}

/*!
    Sets the layout's font to the given \a font. The layout is
    invalidated and must be laid out again.

    \sa font()
*/
void QTextLayout::setFont(const QFont &font)
{
    d->fnt = font;
    d->invalidate();
    if (d->block.isValid()) {
        d->createLinesForBlock();
    }
}

/*!
    Returns the current font that is used for the layout, or a default
    font if none is set.

    \sa setFont()
*/
QFont QTextLayout::font() const
{
    return d->fnt;
}

/*!
    Sets the layout's text to the given \a string. The layout is
    invalidated and must be laid out again.

    Notice that when using this QTextLayout as part of a QTextDocument this
    method will have no effect.

    \sa text()
*/
void QTextLayout::setText(const QString& string)
{
    d->text = string;
    d->invalidate();
    if (d->block.isValid()) {
        d->createLinesForBlock();
    }
}

/*!
    Returns the layout's text.

    \sa setText()
*/
QString QTextLayout::text() const
{
    return d->linesText();
}

/*!
    Sets the text option structure that controls the layout process to the
    given \a option.

    \sa textOption()
*/
void QTextLayout::setTextOption(const QTextOption &option)
{
    d->option = option;
    d->invalidate();
    if (d->block.isValid()) {
        d->createLinesForBlock();
    }
}

/*!
    Returns the current text option used to control the layout process.

    \sa setTextOption()
*/
QTextOption QTextLayout::textOption() const
{
    return d->option;
}

/*!
    Sets the additional formats supported by the text layout to \a formatList.

    \sa additionalFormats(), clearAdditionalFormats()
*/
void QTextLayout::setAdditionalFormats(const QList<FormatRange> &formatList)
{
    d->formats = formatList;
    if (d->block.isValid()) {
        d->block.docHandle()->documentChange(d->block.position(), d->block.length());
    }
}

/*!
    Returns the list of additional formats supported by the text layout.

    \sa setAdditionalFormats(), clearAdditionalFormats()
*/
QList<QTextLayout::FormatRange> QTextLayout::additionalFormats() const
{
    return d->formats;
}

/*!
    Clears the list of additional formats supported by the text layout.

    \sa additionalFormats(), setAdditionalFormats()
*/
void QTextLayout::clearAdditionalFormats()
{
    setAdditionalFormats(QList<FormatRange>());
}

/*!
    Enables caching of the complete layout information if \a enable is
    true; otherwise disables layout caching. Usually
    QTextLayout throws most of the layouting information away after a
    call to endLayout() to reduce memory consumption. If you however
    want to draw the laid out text directly afterwards enabling caching
    might speed up drawing significantly.

    \sa cacheEnabled()
*/
void QTextLayout::setCacheEnabled(bool enable)
{
    // TODO: implement
}

/*!
    Returns true if the complete layout information is cached; otherwise
    returns false.

    \sa setCacheEnabled()
*/
bool QTextLayout::cacheEnabled() const
{
    // TODO: implement
    return false;
}

/*!
    Begins the layout process.

    \sa endLayout()
*/
void QTextLayout::beginLayout()
{
}

/*!
    Ends the layout process.

    \sa beginLayout()
*/
void QTextLayout::endLayout()
{
}

/*!
    \since 4.4

    Clears the line information in the layout. After having called
    this function, lineCount() returns 0.
*/
void QTextLayout::clearLayout()
{
    d->invalidate();
}

/*!
    Returns the next valid cursor position after \a oldPos that
    respects the given cursor \a mode.
    Returns value of \a oldPos, if \a oldPos is not a valid cursor position.

    \sa isValidCursorPosition(), previousCursorPosition()
*/
int QTextLayout::nextCursorPosition(int oldPos, CursorMode mode) const
{
    if (oldPos < 0) {
        return 0;
    }

    const QString linestext = d->linesText();
    switch (mode) {
        case QTextLayout::SkipCharacters: {
            QTextBoundaryFinder finder(QTextBoundaryFinder::Grapheme, linestext);
            finder.setPosition(oldPos);
            oldPos = finder.toNextBoundary();
            if (oldPos == -1) {
                oldPos = qMax(linestext.size() - 1, 0);
            }
            break;
        }
        case QTextLayout::SkipWords: {
            QTextBoundaryFinder finder(QTextBoundaryFinder::Word, linestext);
            finder.setPosition(oldPos);
            oldPos = finder.toNextBoundary();
            if (oldPos == -1) {
                oldPos = qMax(linestext.size() - 1, 0);
            }
            break;
        }
    }
    return oldPos;
}

/*!
    Returns the first valid cursor position before \a oldPos that
    respects the given cursor \a mode.
    Returns value of \a oldPos, if \a oldPos is not a valid cursor position.

    \sa isValidCursorPosition(), nextCursorPosition()
*/
int QTextLayout::previousCursorPosition(int oldPos, CursorMode mode) const
{
    if (oldPos < 0) {
        return 0;
    }

    const QString linestext = d->linesText();
    switch (mode) {
        case QTextLayout::SkipCharacters: {
            QTextBoundaryFinder finder(QTextBoundaryFinder::Grapheme, linestext);
            finder.setPosition(oldPos);
            oldPos = finder.toPreviousBoundary();
            break;
        }
        case QTextLayout::SkipWords: {
            QTextBoundaryFinder finder(QTextBoundaryFinder::Word, linestext);
            finder.setPosition(oldPos);
            oldPos = finder.toPreviousBoundary();
            break;
        }
    }
    return qMax(oldPos, 0);
}

/*!/
    Returns true if position \a pos is a valid cursor position.

    In a Unicode context some positions in the text are not valid
    cursor positions, because the position is inside a Unicode
    surrogate or a grapheme cluster.

    A grapheme cluster is a sequence of two or more Unicode characters
    that form one indivisible entity on the screen. For example the
    latin character `\Auml' can be represented in Unicode by two
    characters, `A' (0x41), and the combining diaresis (0x308). A text
    cursor can only validly be positioned before or after these two
    characters, never between them since that wouldn't make sense. In
    indic languages every syllable forms a grapheme cluster.
*/
bool QTextLayout::isValidCursorPosition(int pos) const
{
    const QString linestext = d->linesText();
    if (pos < 0 || pos >= linestext.size()) {
         return false;
    }

    const QChar positionchar = linestext.at(pos);
    return !positionchar.isLowSurrogate();
}

/*!
    Returns a new text line to be laid out if there is text to be
    inserted into the layout; otherwise returns an invalid text line.

    The text layout creates a new line object that starts after the
    last line in the layout, or at the beginning if the layout is empty.
    The layout maintains an internal cursor, and each line is filled
    with text from the cursor position onwards when the
    QTextLine::setLineWidth() function is called.

    Once QTextLine::setLineWidth() is called, a new line can be created and
    filled with text. Repeating this process will lay out the whole block
    of text contained in the QTextLayout. If there is no text left to be
    inserted into the layout, the QTextLine returned will not be valid
    (isValid() will return false).
*/
QTextLine QTextLayout::createLine()
{
    if (d->text.isEmpty() || d->state == QTextEngine::LayoutFailed) {
        return QTextLine();
    }

    int lineposition = 0;
    int linesize = 0;
    for (int i = 0; i < d->lines.size(); i++) {
        lineposition += d->lines[i].position;
        linesize += d->lines[i].size;
    }

    // qDebug() << Q_FUNC_INFO << lineposition << linesize;
    if ((d->text.size() - linesize) <= 0) {
        return QTextLine();
    }

    QTextItemInt line;
    line.position = lineposition;
    line.f = &d->fnt;
    if (d->fnt.underline()) {
        line.flags |= QTextItem::Underline;
        line.underlineStyle = QTextCharFormat::SingleUnderline;
    }
    line.fontEngine = d->fnt.d->engineForScript(QUnicodeTables::Common);
    d->lines.append(line);
    d->updateLine(d->lines.size() - 1);

    return QTextLine(d->lines.size() - 1, d);
}

/*!
    Returns the number of lines in this text layout.

    \sa lineAt()
*/
int QTextLayout::lineCount() const
{
    return d->lines.size();
}

/*!
    Returns the \a{i}-th line of text in this text layout.

    \sa lineCount(), lineForTextPosition()
*/
QTextLine QTextLayout::lineAt(int i) const
{
    return QTextLine(i, d);
}

/*!
    Returns the line that contains the cursor position specified by \a pos.

    \sa isValidCursorPosition(), lineAt()
*/
QTextLine QTextLayout::lineForTextPosition(int pos) const
{
    // qDebug() << Q_FUNC_INFO << pos;
    for (int i = 0; i < d->lines.size(); i++) {
        const int lineposition = d->lines[i].position;
        if ((lineposition >= pos && pos <= d->text.size()) || pos == d->text.size()) {
            return QTextLine(i, d);
        }
    }
    return QTextLine();
}

/*!
    \since 4.2

    The global position of the layout. This is independent of the
    bounding rectangle and of the layout process.

    \sa setPosition()
*/
QPointF QTextLayout::position() const
{
    return d->position;
}

/*!
    Moves the text layout to point \a p.

    \sa position()
*/
void QTextLayout::setPosition(const QPointF &p)
{
    d->position = p;
}

/*!
    The smallest rectangle that contains all the lines in the layout.
*/
QRectF QTextLayout::boundingRect() const
{
    QRectF result;
    for (int i = 0; i < d->lines.size(); i++) {
        result = result.united(
            QRectF(
                d->lines[i].x.toReal(), d->lines[i].y.toReal(),
                d->lines[i].width.toReal(), d->lines[i].height.toReal()
            )
        );
    }
    return result;
}

/*!
    The minimum width the layout needs. This is the width of the
    layout's smallest non-breakable substring.

    \warning This function only returns a valid value after the layout
    has been done.

    \sa maximumWidth()
*/
qreal QTextLayout::minimumWidth() const
{
    // TODO: implement
    return maximumWidth();
}

/*!
    The maximum width the layout could expand to; this is essentially
    the width of the entire text.

    \warning This function only returns a valid value after the layout
    has been done.

    \sa minimumWidth()
*/
qreal QTextLayout::maximumWidth() const
{
    return d->width(0, d->linesText().size()).toReal();
}

/*!
    \internal
*/
void QTextLayout::setFlags(int flags)
{
    // TODO: implement
}

/*!
    Draws the whole layout on the painter \a p at the position specified by \a pos.
    The rendered layout includes the given \a selections and is clipped within
    the rectangle specified by \a clip.
*/
void QTextLayout::draw(QPainter *p, const QPointF &pos, const QVector<FormatRange> &selections, const QRectF &clip) const
{
    // TODO: implement clip?
    for (int i = 0; i < d->lines.size(); i++) {
        QTextLine(i, d).draw(p, pos, selections.data());
    }
}

/*!
    \fn void QTextLayout::drawCursor(QPainter *painter, const QPointF &position, int cursorPosition) const
    \overload

    Draws a text cursor with the current pen at the given \a position using the
    \a painter specified.
    The corresponding position within the text is specified by \a cursorPosition.
*/
void QTextLayout::drawCursor(QPainter *p, const QPointF &pos, int cursorPosition) const
{
    drawCursor(p, pos, cursorPosition, 1);
}

/*!
    \fn void QTextLayout::drawCursor(QPainter *painter, const QPointF &position, int cursorPosition, int width) const

    Draws a text cursor with the current pen and the specified \a width at the given \a position using the
    \a painter specified.
    The corresponding position within the text is specified by \a cursorPosition.
*/
void QTextLayout::drawCursor(QPainter *p, const QPointF &pos, int cursorPosition, int width) const
{
    const QTextLine line = lineForTextPosition(cursorPosition);
    // qDebug() << Q_FUNC_INFO << cursorPosition << line.isValid();
    if (line.isValid()) {
        bool toggleantialiasing = (!(p->renderHints() & QPainter::Antialiasing));
        if (toggleantialiasing) {
            p->setRenderHint(QPainter::Antialiasing, true);
        }
        const QPointF lineposition = line.position();
        p->fillRect(QRectF(pos.x() + lineposition.x(), pos.y() + lineposition.y(), qreal(width), line.height()), p->pen().brush());
        if (toggleantialiasing) {
            p->setRenderHint(QPainter::Antialiasing, false);
        }
    }
}

/*!
    \class QTextLine
    \reentrant

    \brief The QTextLine class represents a line of text inside a QTextLayout.

    \ingroup richtext-processing

    A text line is usually created by QTextLayout::createLine().

    After being created, the line can be filled using the setLineWidth()
    or setNumColumns() functions. A line has a number of attributes including the
    rectangle it occupies, rect(), its coordinates, x() and y(), its
    textLength(), width() and naturalTextWidth(), and its ascent() and decent()
    relative to the text. The position of the cursor in terms of the
    line is available from cursorToX() and its inverse from
    xToCursor(). A line can be moved with setPosition().
*/

/*!
    \enum QTextLine::Edge

    \value Leading
    \value Trailing
*/

/*!
    \enum QTextLine::CursorPosition

    \value CursorBetweenCharacters
    \value CursorOnCharacter
*/

/*!
    \fn QTextLine::QTextLine(int line, QTextEngine *e)
    \internal

    Constructs a new text line using the line at position \a line in
    the text engine \a e.
*/

/*!
    \fn QTextLine::QTextLine()

    Creates an invalid line.
*/

/*!
    \fn bool QTextLine::isValid() const

    Returns true if this text line is valid; otherwise returns false.
*/

/*!
    \fn int QTextLine::lineNumber() const

    Returns the position of the line in the text engine.
*/


/*!
    Returns the line's bounding rectangle.

    \sa x(), y(), textLength(), width()
*/
QRectF QTextLine::rect() const
{
    if (!eng || eng->lines.isEmpty()) {
        return QRectF();
    }
    return QRectF(
        eng->lines[i].x.toReal(), eng->lines[i].y.toReal(),
        eng->lines[i].width.toReal(), eng->lines[i].height.toReal()
    );
}

/*!
    Returns the rectangle covered by the line.
*/
QRectF QTextLine::naturalTextRect() const
{
    // TODO: implement
    return rect();
}

/*!
    Returns the line's x position.

    \sa rect(), y(), textLength(), width()
*/
qreal QTextLine::x() const
{
    return rect().x();
}

/*!
    Returns the line's y position.

    \sa x(), rect(), textLength(), width()
*/
qreal QTextLine::y() const
{
    return rect().y();
}

/*!
    Returns the line's width as specified by the layout() function.

    \sa naturalTextWidth(), x(), y(), textLength(), rect()
*/
qreal QTextLine::width() const
{
    return rect().width();
}

/*!
    Returns the line's ascent.

    \sa descent(), height()
*/
qreal QTextLine::ascent() const
{
    if (!eng || eng->lines.isEmpty()) {
        return qreal(0.0);
    }
    return eng->lines[i].ascent.toReal();
}

/*!
    Returns the line's descent.

    \sa ascent(), height()
*/
qreal QTextLine::descent() const
{
    if (!eng || eng->lines.isEmpty()) {
        return qreal(0.0);
    }
    return eng->lines[i].descent.toReal();
}

/*!
    Returns the line's height. This is equal to ascent() + descent() + 1
    if leading is not included. If leading is included, this equals to
    ascent() + descent() + leading() + 1.

    \sa ascent(), descent(), leading(), setLeadingIncluded()
*/
qreal QTextLine::height() const
{
    return rect().height();
}

/*!
    \since 4.6

    Returns the line's leading.

    \sa ascent(), descent(), height()
*/
qreal QTextLine::leading() const
{
    if (!eng || eng->lines.isEmpty()) {
        return qreal(0.0);
    }
    return eng->lines[i].leading.toReal();
}

/*!
    \since 4.6

    Includes positive leading into the line's height if \a included is true;
    otherwise does not include leading.

    By default, leading is not included.

    Note that negative leading is ignored, it must be handled
    in the code using the text lines by letting the lines overlap.

    \sa leadingIncluded()

*/
void QTextLine::setLeadingIncluded(bool included)
{
    eng->lines[i].leadingincluded = included;
    if (included) {
        eng->lines[i].height = (eng->lines[i].ascent + eng->lines[i].descent + eng->lines[i].leading + 1);
    } else {
        eng->lines[i].height = (eng->lines[i].ascent + eng->lines[i].descent + 1);
    }
}

/*!
    \since 4.6

    Returns true if positive leading is included into the line's height;
    otherwise returns false.

    By default, leading is not included.

    \sa setLeadingIncluded()
*/
bool QTextLine::leadingIncluded() const
{
    return eng->lines[i].leadingincluded;
}

/*!
    Returns the width of the line that is occupied by text. This is
    always \<= to width(), and is the minimum width that could be used
    by layout() without changing the line break position.
*/
qreal QTextLine::naturalTextWidth() const
{
    if (!eng || eng->lines.isEmpty()) {
        return qreal(0.0);
    }
    const QFontMetricsF fontmetrics(*eng->lines[i].f);
    return (fontmetrics.width(QLatin1Char('x')) * eng->lines[i].text.trimmed().size());
}

/*!
    \since 4.7
    Returns the horizontal advance of the text. The advance of the text
    is the distance from its position to the next position at which
    text would naturally be drawn.

    By adding the advance to the position of the text line and using this
    as the position of a second text line, you will be able to position
    the two lines side-by-side without gaps in-between.
*/
qreal QTextLine::horizontalAdvance() const
{
    const QFontMetricsF fontmetrics(*eng->lines[i].f);
    return fontmetrics.width(QLatin1Char('x'));
}

/*!
    Lays out the line with the given \a width. The line is filled from
    its starting position with as many characters as will fit into
    the line. In case the text cannot be split at the end of the line,
    it will be filled with additional characters to the next whitespace
    or end of the text.
*/
void QTextLine::setLineWidth(qreal width)
{
    eng->lines[i].width = QFixed::fromReal(width);
    eng->updateLine(i);
}

/*!
    Lays out the line. The line is filled from its starting position
    with as many characters as are specified by \a numColumns. In case
    the text cannot be split until \a numColumns characters, the line
    will be filled with as many characters to the next whitespace or
    end of the text.
*/
void QTextLine::setNumColumns(int numColumns)
{
    // TODO: implement
}

/*!
    Lays out the line. The line is filled from its starting position
    with as many characters as are specified by \a numColumns. In case
    the text cannot be split until \a numColumns characters, the line
    will be filled with as many characters to the next whitespace or
    end of the text. The provided \a alignmentWidth is used as reference
    width for alignment.
*/
void QTextLine::setNumColumns(int numColumns, qreal alignmentWidth)
{
    // TODO: implement
}

/*!
    Moves the line to position \a pos.
*/
void QTextLine::setPosition(const QPointF &pos)
{
    eng->lines[i].x = QFixed::fromReal(pos.x());
    eng->lines[i].y = QFixed::fromReal(pos.y());
}

/*!
    Returns the line's position relative to the text layout's position.
*/
QPointF QTextLine::position() const
{
    return QPointF(eng->lines[i].x.toReal(), eng->lines[i].y.toReal());
}

/*!
    Returns the start of the line from the beginning of the string
    passed to the QTextLayout.
*/
int QTextLine::textStart() const
{
    return eng->lines[i].position;
}

/*!
    Returns the length of the text in the line.

    \sa naturalTextWidth()
*/
int QTextLine::textLength() const
{
    return eng->lines[i].text.size();
}

/*!
    \fn void QTextLine::draw(QPainter *painter, const QPointF &position, const QTextLayout::FormatRange *selection) const

    Draws a line on the given \a painter at the specified \a position.
    The \a selection is reserved for internal use.
*/
void QTextLine::draw(QPainter *p, const QPointF &pos, const QTextLayout::FormatRange *selection) const
{
    // TODO: implement selection?
    // qDebug() << Q_FUNC_INFO << p << pos << selection;
    // qDebug() << Q_FUNC_INFO << eng->lines[i].text;
    p->drawTextItem(pos + QPointF(position().x(), position().y() + (height() * 0.8)), eng->lines[i]);
}

/*!
    \fn int QTextLine::cursorToX(int cursorPos, Edge edge) const

    \overload
*/

/*!
    Converts the cursor position \a cursorPos to the corresponding x position
    inside the line, taking account of the \a edge.

    \sa xToCursor()
*/
qreal QTextLine::cursorToX(int *cursorPos, Edge edge) const
{
    // qDebug() << Q_FUNC_INFO << cursorPos << edge;
    if (!eng || eng->lines.isEmpty()) {
        return qreal(0.0);
    }
    switch (edge) {
        case QTextLine::Leading: {
            return (eng->lines[i].width.toReal() / *cursorPos);
        }
        case QTextLine::Trailing: {
            return (eng->lines[i].width.toReal() / (*cursorPos + 1));
        }
    }
    Q_UNREACHABLE();
}

/*!
    \fn int QTextLine::xToCursor(qreal x, CursorPosition cpos) const

    Converts the x-coordinate \a x, to the nearest matching cursor
    position, depending on the cursor position type, \a cpos.

    \sa cursorToX()
*/
int QTextLine::xToCursor(qreal x, CursorPosition cpos) const
{
    // qDebug() << Q_FUNC_INFO << x << cpos;
    // TODO: implement
    switch (cpos) {
        case QTextLine::CursorBetweenCharacters: {
            return 0;
        }
        case QTextLine::CursorOnCharacter: {
            return 0;
        }
    }
    Q_UNREACHABLE();
}

QT_END_NAMESPACE
