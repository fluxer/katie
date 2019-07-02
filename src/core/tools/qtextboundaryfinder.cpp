/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
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
#include <QtCore/qtextboundaryfinder.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE

class QTextBoundaryFinderPrivate
{
public:
    QTextBoundaryFinder::BoundaryType type;
    int pos;
    int length;
    QString string;
};

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
    : d(0)
{
}

/*!
  Copies the QTextBoundaryFinder object, \a other.
*/
QTextBoundaryFinder::QTextBoundaryFinder(const QTextBoundaryFinder &other)
    : d(other.d)
{
}

/*!
  Assigns the object, \a other, to another QTextBoundaryFinder object.
*/
QTextBoundaryFinder &QTextBoundaryFinder::operator=(const QTextBoundaryFinder &other)
{
    // this object was constructed as invalid
    if (!d)
        d = new QTextBoundaryFinderPrivate();
    if (this != &other)
        memcpy(d, other.d, sizeof(QTextBoundaryFinderPrivate));
    return *this;
}

/*!
  Destructs the QTextBoundaryFinder object.
*/
QTextBoundaryFinder::~QTextBoundaryFinder()
{
    delete d;
    d = 0;
}

/*!
  Creates a QTextBoundaryFinder object of \a type operating on \a string.
*/
QTextBoundaryFinder::QTextBoundaryFinder(BoundaryType type, const QString &string)
    : d(new QTextBoundaryFinderPrivate)
{
    d->type = type;
    d->pos = 0;
    d->length = string.size();
    d->string = QString(string);
}

/*!
  Creates a QTextBoundaryFinder object of \a type operating on \a chars
  with \a length.
*/
QTextBoundaryFinder::QTextBoundaryFinder(BoundaryType type, const QChar *chars, const int length)
    : d(new QTextBoundaryFinderPrivate)
{
    d->type = type;
    d->pos = 0;
    d->length = length;
    d->string = QString::fromRawData(chars, length);
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
    d->pos = d->length;
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
    d->pos = qBound(0, position, d->length);
}

/*! \fn QTextBoundaryFinder::BoundaryType QTextBoundaryFinder::type() const

  Returns the type of the QTextBoundaryFinder.
*/
QTextBoundaryFinder::BoundaryType QTextBoundaryFinder::type() const
{
    if (!d)
        return QTextBoundaryFinder::Grapheme;
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
    return QString(d->string);
}


/*!
  Moves the QTextBoundaryFinder to the next boundary position and returns that position.

  Returns -1 if there is no next boundary.
*/
int QTextBoundaryFinder::toNextBoundary()
{
    if (!d) {
        d->pos = -1;
        return d->pos;
    }

    if (d->pos < 0 || d->pos >= d->length) {
        d->pos = -1;
        return d->pos;
    }
    ++d->pos;
    if (d->pos == d->length)
        return d->pos;

    switch(d->type) {
        case QTextBoundaryFinder::Grapheme: {
            while (d->pos < d->length && d->string[d->pos].isLetterOrNumber())
                ++d->pos;
            break;
        }
        case QTextBoundaryFinder::Word: {
            while (d->pos < d->length && !d->string[d->pos].isSpace())
                ++d->pos;
            break;
        }
        case QTextBoundaryFinder::Sentence: {
            while (d->pos < d->length && !d->string[d->pos].isPunct())
                ++d->pos;
            break;
        }
        case QTextBoundaryFinder::Line: {
            while (d->pos < d->length && d->string[d->pos].category() != QChar::Separator_Line)
                ++d->pos;
            break;
        }
    }

    return d->pos;
}

/*!
  Moves the QTextBoundaryFinder to the previous boundary position and returns that position.

  Returns -1 if there is no previous boundary.
*/
int QTextBoundaryFinder::toPreviousBoundary()
{
    if (!d) {
        d->pos = -1;
        return d->pos;
    }

    if (d->pos <= 0 || d->pos >= d->length) {
        d->pos = -1;
        return d->pos;
    }
    --d->pos;
    if (d->pos == 0)
        return d->pos;

    switch(d->type) {
        case QTextBoundaryFinder::Grapheme: {
            while (d->pos > 0 && d->string[d->pos].isLetterOrNumber())
                --d->pos;
            break;
        }
        case QTextBoundaryFinder::Word: {
            while (d->pos > 0 && !d->string[d->pos].isSpace())
                --d->pos;
            break;
        }
        case QTextBoundaryFinder::Sentence: {
            while (d->pos > 0 && !d->string[d->pos].isPunct())
                --d->pos;
            break;
        }
        case QTextBoundaryFinder::Line: {
            while (d->pos > 0 && d->string[d->pos].category() != QChar::Separator_Line)
                --d->pos;
            break;
        }
    }

    return d->pos;
}

/*!
  Returns true if the object's position() is currently at a valid text boundary.
*/
bool QTextBoundaryFinder::isAtBoundary() const
{
    if (!d || d->pos < 0)
        return false;

    if (d->pos == d->length)
        return true;

    switch(d->type) {
        case QTextBoundaryFinder::Grapheme:
            return !d->string[d->pos].isLetterOrNumber();
        case QTextBoundaryFinder::Word:
            return d->string[d->pos].isSpace();
        case QTextBoundaryFinder::Line:
            return (d->pos > 0) ? d->string[d->pos].category() == QChar::Separator_Line : true;
        case QTextBoundaryFinder::Sentence:
            return d->string[d->pos].isPunct();
    }
    return false;
}

/*!
  Returns the reasons for the boundary finder to have chosen the current position as a boundary.
*/
QTextBoundaryFinder::BoundaryReasons QTextBoundaryFinder::boundaryReasons() const
{
    if (!d || !isAtBoundary())
        return QTextBoundaryFinder::NotAtBoundary;
    if (d->pos == 0) {
        if (d->string[d->pos].isSpace())
            return QTextBoundaryFinder::NotAtBoundary;
        return QTextBoundaryFinder::StartWord;
    }
    if (d->pos == d->length) {
        if (d->string[d->length-1].isSpace())
            return QTextBoundaryFinder::NotAtBoundary;
        return QTextBoundaryFinder::EndWord;
    }

    const bool nextIsSpace = d->string[d->pos].isSpace();
    const bool prevIsSpace = d->string[d->pos - 1].isSpace();

    if (prevIsSpace && !nextIsSpace)
        return QTextBoundaryFinder::StartWord;
    else if (!prevIsSpace && nextIsSpace)
        return QTextBoundaryFinder::EndWord;
    else if (!prevIsSpace && !nextIsSpace)
        return BoundaryReasons(QTextBoundaryFinder::StartWord | QTextBoundaryFinder::EndWord);
    else
        return QTextBoundaryFinder::NotAtBoundary;
}

QT_END_NAMESPACE
