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

#include "qdebug.h"
#include "qtextformat.h"
#include "qtextformat_p.h"
#include "qtextengine_p.h"
#include "qabstracttextdocumentlayout.h"
#include "qtextlayout.h"
#include "qvarlengtharray.h"
#include "qfont.h"
#include "qfontengine_p.h"
#include "qunicodetables_p.h"
#include "qtextdocument_p.h"
#include "qfontmetrics.h"
#include "qcorecommon_p.h"
#include "qx11info_x11.h"

#if defined(Q_WS_X11)
#   include "qfontengine_ft_p.h"
#endif

QT_BEGIN_NAMESPACE

QTextEngine::QTextEngine()
    : state(QTextEngine::LayoutEmpty)
{
}

QTextEngine::QTextEngine(const QString &str, const QFont &f)
    : text(str), fnt(f), state(QTextEngine::LayoutEmpty)
{
}

QTextEngine::~QTextEngine()
{
}

void QTextEngine::invalidate()
{
    // qDebug() << Q_FUNC_INFO;
    state = QTextEngine::LayoutEmpty;
    lines.clear();
}

void QTextEngine::updateLine(const int line)
{
    // TODO: apply text options
    QString result(text.unicode() + lines[line].position, text.size() - lines[line].position);

    lines[line].text = result;
    lines[line].size = (text.size() - lines[line].position);

    // TODO: glyphs
    lines[line].glyphs.numGlyphs = 1;

    const QFontMetricsF fontmetrics(fnt);
    lines[line].width = QFixed::fromReal(fontmetrics.width(QLatin1Char('x')) * result.size());
    lines[line].ascent = QFixed::fromReal(fontmetrics.ascent());
    lines[line].descent = QFixed::fromReal(fontmetrics.descent());
    lines[line].leading = QFixed::fromReal(fontmetrics.leading());
    if (lines[line].leadingincluded) {
        lines[line].height = (lines[line].ascent + lines[line].descent + lines[line].leading + 1);
    } else {
        lines[line].height = (lines[line].ascent + lines[line].descent + 1);
    }

    // qDebug() << Q_FUNC_INFO << line << lines[line].position << result.size();
}

void QTextEngine::createLinesForBlock()
{
    Q_ASSERT(block.isValid());
    text.clear();
    const QTextDocument* blockdocument = block.document();
    if (blockdocument) {
        qreal xoff = 0.0;
        qreal yoff = 0.0;
        for (int i = 0; i < blockdocument->blockCount(); i++) {
            QTextBlock documentblock = blockdocument->findBlockByNumber(i);

            QTextItemInt line;
            line.position = documentblock.position();
            line.f = &fnt;
            if (line.f->underline()) {
                line.flags |= QTextItem::Underline;
                line.underlineStyle = QTextCharFormat::SingleUnderline;
            }
            line.fontEngine = line.f->d->engineForScript(QUnicodeTables::Common);
            lines.append(line);

            lines[i].x = QFixed::fromReal(xoff);
            lines[i].y = QFixed::fromReal(yoff);

            // TODO: apply text options
            lines[i].text = documentblock.text();
            lines[i].size = (lines[i].text.size() - lines[i].position);

            // TODO: glyphs
            lines[i].glyphs.numGlyphs = 1;

            const QFontMetricsF fontmetrics(*line.f);
            lines[i].width = QFixed::fromReal(fontmetrics.width(QLatin1Char('x')) * lines[i].text.size());
            lines[i].ascent = QFixed::fromReal(fontmetrics.ascent());
            lines[i].descent = QFixed::fromReal(fontmetrics.descent());
            lines[i].leading = QFixed::fromReal(fontmetrics.leading());
            if (lines[i].leadingincluded) {
                lines[i].height = (lines[i].ascent + lines[i].descent + lines[i].leading + 1);
            } else {
                lines[i].height = (lines[i].ascent + lines[i].descent + 1);
            }

            lines[i].charFormat = documentblock.charFormat();

            xoff += 0.0;
            yoff += lines[i].height.toReal();
        }
    }
    // qDebug() << Q_FUNC_INFO << lines.size();
}

bool QTextEngine::atWordSeparator(int position) const
{
    const QString linestext = linesText();
    if (position < 0 || position >= linestext.size()) {
        return false;
    }

    const QChar c = linestext.at(position);
    switch (c.toLatin1()) {
    case '.':
    case ',':
    case '?':
    case '!':
    case '@':
    case '#':
    case '$':
    case ':':
    case ';':
    case '-':
    case '<':
    case '>':
    case '[':
    case ']':
    case '(':
    case ')':
    case '{':
    case '}':
    case '=':
    case '/':
    case '+':
    case '%':
    case '&':
    case '^':
    case '*':
    case '\'':
    case '"':
    case '`':
    case '~':
    case '|':
        return true;
    default:
        return false;
    }
}

bool QTextEngine::atSpace(int position) const
{
    const QString linestext = linesText();
    if (position < 0 || position >= linestext.size()) {
        return false;
    }

    const QChar c = linestext.at(position);

    return c == QLatin1Char(' ')
        || c == QChar::Nbsp
        || c == QChar::LineSeparator
        || c == QLatin1Char('\t')
        ;
}

QFixed QTextEngine::width(int charFrom, int numChars) const
{
    const QFontMetricsF fontmetrics(fnt);
    return QFixed::fromReal(numChars * fontmetrics.width(QLatin1Char('x')));
}

glyph_metrics_t QTextEngine::boundingBox(int from,  int len) const
{
    const QString linestext = linesText();

    // qDebug() << Q_FUNC_INFO << linestext << from << len;

    QPainterPath textpath;
    textpath.setFillRule(Qt::WindingFill);
    textpath.addText(QPointF(), fnt, linestext.mid(from, len));

    const QRectF brect = textpath.boundingRect();
    glyph_metrics_t gm;
    gm.x = QFixed::fromReal(brect.x());
    gm.y = QFixed::fromReal(brect.y());
    gm.width = QFixed::fromReal(brect.width());
    gm.height = QFixed::fromReal(brect.height());
    gm.xoff = QFixed::fromReal(0.0);
    return gm;
}

QString QTextEngine::elidedText(Qt::TextElideMode mode, const QFixed &width, int flags) const
{
    // TODO: flags
    const QString linestext = linesText();
    const QFontMetricsF fontmetrics(fnt);
    const int elidedlength = (width.toReal() / fontmetrics.width(QLatin1Char('x')));
    // qDebug() << Q_FUNC_INFO << linestext << width.toReal() << elidedlength;

    if (elidedlength <= 0) {
        return QString();
    } else if (elidedlength <= 3 && linestext.size() > 0) {
        return (QString(linestext.at(0)) + QChar(0x2026));
    } else if (linestext.size() <= elidedlength) {
        return linestext;
    }

    switch (mode) {
        case Qt::ElideLeft: {
            return (QLatin1String("...") + linestext.mid(0, elidedlength - 3));
        }
        case Qt::ElideRight: {
            return (linestext.mid(0, elidedlength - 3) + QLatin1String("..."));
        }
        case Qt::ElideMiddle: {
            if (elidedlength <= 5) {
                return linestext;
            }
            const int midlength = (elidedlength / 2);
            return (linestext.left(midlength - 2) + QLatin1String("...") + linestext.right(midlength - 1));
        }
        case Qt::ElideNone: {
            return linestext.mid(0, elidedlength);
        }
    }
    Q_UNREACHABLE();
}

QString QTextEngine::linesText() const
{
    QString result;
    if (state == LayoutFailed) {
        return result;
    }

    for (int i = 0; i < lines.size(); i++) {
        result += lines[i].text;
        if (i != (lines.size() - 1)) {
             result += QLatin1Char('\n');
        }
        // qDebug() << Q_FUNC_INFO << i << lines[i].text;
    }

    // FIXME: temporary until itemization is implemented
    if (result.isEmpty() && lines.size() == 0) {
        result = text;
    }

    // qDebug() << Q_FUNC_INFO << result;
    return result;
}

QTextItemInt::QTextItemInt()
    : flags(0), f(nullptr), fontEngine(nullptr),
    position(0), size(0), x(QFixed::fromReal(0.0)), y(QFixed::fromReal(0.0)),
    width(QFixed::fromReal(0.0)), height(QFixed::fromReal(0.0)),
    ascent(QFixed::fromReal(0.0)), descent(QFixed::fromReal(0.0)),
    leading(QFixed::fromReal(0.0)), leadingincluded(false),
    underlineStyle(QTextCharFormat::NoUnderline)
{
}

QTextItemInt::QTextItemInt(const QTextItemInt &other)
    : flags(other.flags), f(other.f), text(other.text), fontEngine(other.fontEngine),
    position(other.position), size(other.size), x(other.x), y(other.y),
    width(other.width), height(other.height),
    ascent(other.ascent), descent(other.descent),
    leading(other.leading), leadingincluded(other.leadingincluded),
    underlineStyle(other.underlineStyle)
{
}

QT_END_NAMESPACE
