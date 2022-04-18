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

#include "qtextoption.h"
#include "qapplication.h"
#include "qlist.h"

QT_BEGIN_NAMESPACE

/*!
    Constructs a text option with default properties for text.
    The text alignment property is set to Qt::AlignLeft. The
    word wrap property is set to QTextOption::WordWrap. The
    using of design metrics flag is set to false.
*/
QTextOption::QTextOption()
    : wordwrap(QTextOption::WordWrap),
    fflags(0),
    align(Qt::AlignLeft),
    direction(Qt::LayoutDirectionAuto),
    design(false),
    tab(-1)
{
}

/*!
    Constructs a text option with the given \a alignment for text.
    The word wrap property is set to QTextOption::WordWrap. The using
    of design metrics flag is set to false.
*/
QTextOption::QTextOption(Qt::Alignment alignment)
    : wordwrap(QTextOption::WordWrap),
    fflags(0),
    align(alignment),
    direction(QApplication::layoutDirection()),
    design(false),
    tab(-1)
{
}

/*!
    Destroys the text option.
*/
QTextOption::~QTextOption()
{
}

/*!
    \fn QTextOption::QTextOption(const QTextOption &other)

    Construct a copy of the \a other text option.
*/
QTextOption::QTextOption(const QTextOption &o)
    : wordwrap(o.wordwrap),
    fflags(o.fflags),
    align(o.align),
    direction(o.direction),
    design(o.design),
    tab(o.tab)
{
}

/*!
    \fn QTextOption &QTextOption::operator=(const QTextOption &other)

    Returns true if the text option is the same as the \a other text option;
    otherwise returns false.
*/
QTextOption &QTextOption::operator=(const QTextOption &o)
{
    if (this == &o)
        return *this;

    wordwrap = o.wordwrap;
    fflags = o.fflags;
    align = o.align;
    direction = o.direction;
    design = o.design;
    tab = o.tab;
    return *this;
}

/*!
    \class QTextOption
    \reentrant

    \brief The QTextOption class provides a description of general rich text
    properties.

    \ingroup richtext-processing

    QTextOption is used to encapsulate common rich text properties in a single
    object. It contains information about text alignment, layout direction,
    word wrapping, and other standard properties associated with text rendering
    and layout.

    \sa QTextEdit, QTextDocument, QTextCursor
*/

/*!
    \enum QTextOption::WrapMode

    This enum describes how text is wrapped in a document.

    \value NoWrap       Text is not wrapped at all.
    \value WordWrap     Text is wrapped at word boundaries.
    \value WrapAnywhere Text can be wrapped at any point on a line, even if
                        it occurs in the middle of a word.
    \value WrapAtWordBoundaryOrAnywhere If possible, wrapping occurs at a word
                        boundary; otherwise it will occur at the appropriate
                        point on the line, even in the middle of a word.
*/

/*!
  \fn Qt::Alignment QTextOption::alignment() const

  Returns the text alignment defined by the option.

  \sa setAlignment()
*/

/*!
  \fn void QTextOption::setAlignment(Qt::Alignment alignment);

  Sets the option's text alignment to the specified \a alignment.

  \sa alignment()
*/

/*!
  \fn Qt::LayoutDirection QTextOption::textDirection() const

  Returns the direction of the text layout defined by the option.

  \sa setTextDirection()
*/

/*!
  \fn void QTextOption::setTextDirection(Qt::LayoutDirection direction)

  Sets the direction of the text layout defined by the option to the
  given \a direction.

  \sa textDirection()
*/

/*!
  \fn WrapMode QTextOption::wrapMode() const

  Returns the text wrap mode defined by the option.

  \sa setWrapMode()
*/

/*!
  \fn void QTextOption::setWrapMode(WrapMode mode)

  Sets the option's text wrap mode to the given \a mode.
*/

/*!
  \enum QTextOption::Flag

  \value IncludeTrailingSpaces When this option is set, QTextLine::naturalTextWidth() and naturalTextRect() will
                               return a value that includes the width of trailing spaces in the text; otherwise
                               this width is excluded.
  \value SuppressColors Suppress all color changes in the character formats (except the main selection).
*/

/*!
  \fn Flags QTextOption::flags() const

  Returns the flags associated with the option.

  \sa setFlags()
*/

/*!
  \fn void QTextOption::setFlags(Flags flags)

  Sets the flags associated with the option to the given \a flags.

  \sa flags()
*/

/*!
  \fn qreal QTextOption::tabStop() const

  Returns the distance in device units between tab stops.
  Convenient function for the above method

  \sa setTabStop()
*/

/*!
  \fn void QTextOption::setTabStop(qreal tabStop)

  Sets the default distance in device units between tab stops to the value specified
  by \a tabStop.

  \sa tabStop()
*/

QT_END_NAMESPACE

#include "moc_qtextoption.h"

