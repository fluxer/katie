/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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

#include "qtextboundaryfinder.h"
#include "qlocale.h"

#include <unicode/ubrk.h>

QT_BEGIN_NAMESPACE

static inline UBreakIteratorType getBreakType(const QTextBoundaryFinder::BoundaryType type)
{
    switch (type) {
        case QTextBoundaryFinder::Grapheme:
            return UBRK_CHARACTER;
        case QTextBoundaryFinder::Word:
            return UBRK_WORD;
        case QTextBoundaryFinder::Line:
            return UBRK_LINE;
        case QTextBoundaryFinder::Sentence:
            return UBRK_SENTENCE;
    }
    Q_UNREACHABLE();
}

static const char* getBreakLocale() {
    QByteArray bcp = QLocale::system().bcp47Name().toLatin1();
    for(int i = 0; i < ubrk_countAvailable(); i++) {
        const char* locale = ubrk_getAvailable(i);
        if (qstrcmp(locale, bcp.constData()) == 0) {
            return locale;
        }
    }
    return "en";
}

class QTextBoundaryFinderPrivate
{
public:
    QTextBoundaryFinderPrivate();
    QTextBoundaryFinderPrivate(const QTextBoundaryFinder::BoundaryType type, const QString &text);
    ~QTextBoundaryFinderPrivate();

    QTextBoundaryFinder::BoundaryType type;
    int pos;
    QString string;
    mutable UBreakIterator *breakiter; // ubrk_isBoundary() takes non-const argument

private:
    Q_DISABLE_COPY(QTextBoundaryFinderPrivate);
};

QTextBoundaryFinderPrivate::QTextBoundaryFinderPrivate()
    : type(QTextBoundaryFinder::Grapheme), pos(0), breakiter(nullptr)
{
}

QTextBoundaryFinderPrivate::QTextBoundaryFinderPrivate(const QTextBoundaryFinder::BoundaryType type, const QString &text)
    : type(type), pos(0), string(text), breakiter(nullptr)
{
    if (Q_LIKELY(!string.isEmpty())) {
        UErrorCode error = U_ZERO_ERROR;
        breakiter = ubrk_open(getBreakType(type), getBreakLocale(),
            reinterpret_cast<const UChar*>(string.unicode()), string.size(), &error);
        if (Q_UNLIKELY(U_FAILURE(error))) {
            qWarning("QTextBoundaryFinder::QTextBoundaryFinder: ubrk_open() failed %s", u_errorName(error));
            breakiter = nullptr;
        }
    }
}

QTextBoundaryFinderPrivate::~QTextBoundaryFinderPrivate()
{
    if (breakiter) {
        ubrk_close(breakiter);
    }
}

/*! 
    \class QTextBoundaryFinder

    \brief The QTextBoundaryFinder class provides a way of finding Unicode text boundaries in a string.

    \since 4.4
    \ingroup tools
    \ingroup shared
    \ingroup string-processing
    \reentrant

    QTextBoundaryFinder allows to find Unicode text boundaries in a
    string, similar to the Unicode text boundary specification (see
    http://www.unicode.org/reports/tr29/tr29-11.html).

    QTextBoundaryFinder can operate on a QString in four possible
    modes depending on the value of \a BoundaryType.

    Units of Unicode characters that make up what the user thinks of
    as a character or basic unit of the language are here called
    Grapheme clusters. The two unicode characters 'A' + diaeresis do
    for example form one grapheme cluster as the user thinks of them
    as one character, yet it is in this case represented by two
    unicode code points.

    Word boundaries are there to locate the start and end of what a
    language considers to be a word.

    Line break boundaries give possible places where a line break
    might happen and sentence boundaries will show the beginning and
    end of whole sentences.

    The first position in a string is always a valid boundary and
    refers to the position before the first character. The last
    position at the length of the string is also valid and refers
    to the position after the last character.

    Boundaries are resolved based on the current locale.
*/

/*!
    \enum QTextBoundaryFinder::BoundaryType

    \value Grapheme Finds a grapheme which is the smallest boundary. It
    including letters, punctation marks, numerals and more.
    \value Word Finds a word.
    \value Line Finds possible positions for breaking the text into multiple
    lines.
    \value Sentence Finds sentence boundaries. These include periods, question
    marks etc.
*/

/*!
  \enum QTextBoundaryFinder::BoundaryReason

  \value NotAtBoundary  The boundary finder is not at a boundary position.
  \value StartWord  The boundary finder is at the start of a word.
  \value EndWord  The boundary finder is at the end of a word.
*/

/*!
  Constructs an invalid QTextBoundaryFinder object.
*/
QTextBoundaryFinder::QTextBoundaryFinder()
    : d(new QTextBoundaryFinderPrivate())
{
}

/*!
  Copies the QTextBoundaryFinder object, \a other.
*/
QTextBoundaryFinder::QTextBoundaryFinder(const QTextBoundaryFinder &other)
    : d(new QTextBoundaryFinderPrivate(other.d->type, other.d->string))
{
}

/*!
  Assigns the object, \a other, to another QTextBoundaryFinder object.
*/
QTextBoundaryFinder &QTextBoundaryFinder::operator=(const QTextBoundaryFinder &other)
{
    d->type = other.d->type;
    d->pos = other.d->pos;
    d->string = other.d->string;

    if (d->breakiter) {
        ubrk_close(d->breakiter);
    }

    UErrorCode error = U_ZERO_ERROR;
    d->breakiter = ubrk_safeClone(other.d->breakiter, nullptr, nullptr, &error);
    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("QTextBoundaryFinder: ubrk_safeClone() failed %s", u_errorName(error));
        d->breakiter = nullptr;
    }
    error = U_ZERO_ERROR;
    ubrk_setText(d->breakiter, reinterpret_cast<const UChar*>(d->string.unicode()), d->string.size(), &error);
    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("QTextBoundaryFinder: ubrk_setText() failed %s", u_errorName(error));
        ubrk_close(d->breakiter);
        d->breakiter = nullptr;
    }

    return *this;
}

/*!
  Destructs the QTextBoundaryFinder object.
*/
QTextBoundaryFinder::~QTextBoundaryFinder()
{
    delete d;
}

/*!
  Creates a QTextBoundaryFinder object of \a type operating on \a string.
*/
QTextBoundaryFinder::QTextBoundaryFinder(BoundaryType type, const QString &string)
    : d(new QTextBoundaryFinderPrivate(type, string))
{
}

/*!
  Creates a QTextBoundaryFinder object of \a type operating on \a chars
  with \a length.
*/
QTextBoundaryFinder::QTextBoundaryFinder(BoundaryType type, const QChar *chars, const int length)
    : d(new QTextBoundaryFinderPrivate(type, QString::fromRawData(chars, length)))
{
}

/*!
  Moves the finder to the start of the string. This is equivalent to setPosition(0).

  \sa setPosition(), position()
*/
void QTextBoundaryFinder::toStart()
{
    d->pos = 0;
}

/*!
  Moves the finder to the end of the string. This is equivalent to setPosition(string.length()).

  \sa setPosition(), position()
*/
void QTextBoundaryFinder::toEnd()
{
    d->pos = d->string.size();
}

/*!
  Returns the current position of the QTextBoundaryFinder.

  The range is from 0 (the beginning of the string) to the length of
  the string inclusive.

  \sa setPosition()
*/
int QTextBoundaryFinder::position() const
{
    return d->pos;
}

/*!
  Sets the current position of the QTextBoundaryFinder to \a position.

  If \a position is out of bounds, it will be bound to only valid
  positions. In this case, valid positions are from 0 to the length of
  the string inclusive.

  \sa position()
*/
void QTextBoundaryFinder::setPosition(const int position)
{
    d->pos = qBound(0, position, d->string.size());
}

/*! \fn QTextBoundaryFinder::BoundaryType QTextBoundaryFinder::type() const

  Returns the type of the QTextBoundaryFinder.
*/
QTextBoundaryFinder::BoundaryType QTextBoundaryFinder::type() const
{
    return d->type;
}

/*! \fn bool QTextBoundaryFinder::isValid() const

   Returns true if the text boundary finder is valid; otherwise returns false.
   A default QTextBoundaryFinder is invalid.
*/

/*!
  Returns the string the QTextBoundaryFinder object operates on.
*/
QString QTextBoundaryFinder::string() const
{
    return d->string;
}

/*!
  Moves the QTextBoundaryFinder to the next boundary position and returns that position.

  Returns -1 if there is no next boundary.
*/
int QTextBoundaryFinder::toNextBoundary()
{
    if (!d->breakiter) {
        return -1;
    }
    if (d->pos != -1) {
        ubrk_following(d->breakiter, d->pos - 1);
    }
    d->pos = ubrk_next(d->breakiter);
    return d->pos;
}

/*!
  Moves the QTextBoundaryFinder to the previous boundary position and returns that position.

  Returns -1 if there is no previous boundary.
*/
int QTextBoundaryFinder::toPreviousBoundary()
{
    if (!d->breakiter) {
        return -1;
    }
    if (d->pos != -1) {
        ubrk_preceding(d->breakiter, d->pos + 1);
    }
    d->pos = ubrk_previous(d->breakiter);
    return d->pos;
}

/*!
  Returns true if the object's position() is currently at a valid text boundary.
*/
bool QTextBoundaryFinder::isAtBoundary() const
{
    if (!d->breakiter) {
        return false;
    }
    return ubrk_isBoundary(d->breakiter, d->pos);
}

/*!
  Returns the reasons for the boundary finder to have chosen the current position as a boundary.
*/
QTextBoundaryFinder::BoundaryReasons QTextBoundaryFinder::boundaryReasons() const
{
    if (!isAtBoundary()) {
        return QTextBoundaryFinder::NotAtBoundary;
    } else if (d->pos == 0) {
        return QTextBoundaryFinder::StartWord;
    } else if (d->pos == d->string.size()) {
        return QTextBoundaryFinder::EndWord;
    }

    QTextBoundaryFinder::BoundaryReasons reasons;
    if (ubrk_isBoundary(d->breakiter, d->pos - 1)) {
        reasons |= QTextBoundaryFinder::StartWord;
    }
    if (ubrk_isBoundary(d->breakiter, d->pos + 1)) {
        reasons |= QTextBoundaryFinder::EndWord;
    }
    return reasons;
}

QT_END_NAMESPACE
