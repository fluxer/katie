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

#include "qdir.h"
#include "qfontdatabase.h"
#include "qdebug.h"
#include "qalgorithms.h"
#include "qapplication.h"
#include "qthread.h"
#include "qmutex.h"
#include "qunicodetables_p.h"
#include "qfontengine_p.h"
#include "qfontinfo.h"
#include "qfontmetrics.h"

#include <stdlib.h>
#include <limits.h>

// #define QFONTDATABASE_DEBUG
#ifdef QFONTDATABASE_DEBUG
#  define FD_DEBUG qDebug
#else
#  define FD_DEBUG if (false) qDebug
#endif

// #define FONT_MATCH_DEBUG
#ifdef FONT_MATCH_DEBUG
#  define FM_DEBUG qDebug
#else
#  define FM_DEBUG if (false) qDebug
#endif

QT_BEGIN_NAMESPACE

struct QtFontFamily {
    QString family;
    QString foundry;
    QString style;
    bool fixedpitch;
    bool scalable;
    bool italic;
    bool bold;
    int weight;
    int pointsize;

    bool operator<(const QtFontFamily &other) const;
};

bool QtFontFamily::operator<(const QtFontFamily &other) const
{
    if (family == other.family) {
        // qDebug() << Q_FUNC_INFO << family << weight << other.weight;
        if (style == QLatin1String("Normal") && other.style != QLatin1String("Normal")) {
            return false;
        }
        if (weight < other.weight) {
            return false;
        }
    }
    return (family < other.family || foundry < other.foundry
            || style < other.style || fixedpitch < other.fixedpitch
            || scalable < other.scalable || italic < other.italic
            || bold < other.bold || pointsize < other.pointsize);
}

class QFontDatabasePrivate
{
public:
#if defined(Q_WS_X11) && !defined(QT_NO_FONTCONFIG)
    QString systemLang;
#endif
    QMap<QtFontFamily, bool> families;
};



Q_GLOBAL_STATIC(QFontDatabasePrivate, qGlobalFontDatabase)
Q_GLOBAL_STATIC(QMutex, qGlobalFontDatabaseMutex)

/*!
  \internal

  This makes sense of the font family name:

  if the family name contains a '[' and a ']', then we take the text
  between the square brackets as the foundry, and the text before the
  square brackets as the family (ie. "FreeSans [GNU]")
*/
static void parseFontName(const QString &name, QString &foundry, QString &family)
{
    int i = name.indexOf(QLatin1Char('['));
    int li = name.lastIndexOf(QLatin1Char(']'));
    if (i >= 0 && li >= 0 && i < li) {
        foundry = name.mid(i + 1, li - i - 1);
        if (i > 0 && name[i - 1] == QLatin1Char(' '))
            i--;
        family = name.left(i);
    } else {
        foundry.clear();
        family = name;
    }

    // capitalize the family/foundry names
    bool space = true;
    QChar *s = family.data();
    int len = family.length();
    while(len--) {
        if (space) *s = s->toUpper();
        space = s->isSpace();
        ++s;
    }

    space = true;
    s = foundry.data();
    len = foundry.length();
    while(len--) {
        if (space) *s = s->toUpper();
        space = s->isSpace();
        ++s;
    }
}

static QStringList familyList(const QFontDef &req)
{
    // list of families to try
    QStringList family_list;
    if (req.family.isEmpty())
        return family_list;

    QStringList list = req.family.split(QLatin1Char(','));
    for (int i = 0; i < list.size(); ++i) {
        QString str = list.at(i).trimmed();
        if ((str.startsWith(QLatin1Char('"')) && str.endsWith(QLatin1Char('"')))
            || (str.startsWith(QLatin1Char('\'')) && str.endsWith(QLatin1Char('\''))))
            str = str.mid(1, str.length() - 2);
        family_list << str;
    }

    return family_list;
}

static inline bool isStyleMatch(const QString &style, const QString &otherstyle)
{
    const QString lowerstyle(style.toLower());
    const QString lowerother(otherstyle.toLower());
    if (lowerstyle == lowerother) {
        return true;
    }

    if (lowerstyle.contains(QLatin1String("italic")) && lowerother.contains(QLatin1String("oblique"))) {
        return true;
    }
    return (lowerstyle.contains(QLatin1String("oblique")) && lowerother.contains(QLatin1String("italic")));
}

QT_BEGIN_INCLUDE_NAMESPACE
#if defined(Q_WS_X11)
#  include "qfontdatabase_x11_p.h"
#endif
QT_END_INCLUDE_NAMESPACE

QString QFontDatabase::resolveFontFamilyAlias(const QString &family)
{
#if defined(QT_NO_FONTCONFIG)
    return family;
#else
    if (!qt_x11Data->has_fontconfig)
        return family;

    FcPattern *pattern = FcPatternCreate();
    if (!pattern)
        return family;

    QByteArray cs = family.toUtf8();
    FcPatternAddString(pattern, FC_FAMILY, (const FcChar8 *) cs.constData());
    FcConfigSubstitute(0, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    QString resolved;
    FcChar8 *familyAfterSubstitution = nullptr;
    FcResult unused;
    FcPattern *match = FcFontMatch(0, pattern, &unused);
    if (match) {
        FcPatternGetString(match, FC_FAMILY, 0, &familyAfterSubstitution);
        resolved = QString::fromUtf8((const char *) familyAfterSubstitution);
        FcPatternDestroy(match);
    } else {
        FcPatternGetString(pattern, FC_FAMILY, 0, &familyAfterSubstitution);
        resolved = QString::fromUtf8((const char *) familyAfterSubstitution);
    }
    FcPatternDestroy(pattern);

    return resolved;
#endif
}

static inline QString styleStringHelper(const QString &family, int weight, QFont::Style style)
{
    QString result;

    if (weight >= QFont::Black) {
        result = QApplication::translate("QFontDatabase", "Black");
    } else if (weight >= QFont::Bold) {
        result = QApplication::translate("QFontDatabase", "Bold");
    } else if (weight >= QFont::DemiBold) {
        result = QApplication::translate("QFontDatabase", "Demi Bold");
    } else if (weight < QFont::Normal) {
        result = QApplication::translate("QFontDatabase", "Light");
    }

    if (style == QFont::StyleItalic) {
        result += QLatin1Char(' ') + QApplication::translate("QFontDatabase", "Italic");
    } else if (style == QFont::StyleOblique) {
        result += QLatin1Char(' ') + QApplication::translate("QFontDatabase", "Oblique");
    }

    if (result.isEmpty()) {
        foreach (const QtFontFamily &fontfamily, qGlobalFontDatabase()->families.keys()) {
            if (fontfamily.family.compare(family, Qt::CaseInsensitive) != 0) {
                continue;
            }
            result = fontfamily.style;
            break;
        }
    }

    return result.simplified();
}

/*!
    Returns a string that describes the style of the \a font. For
    example, "Bold Italic", "Bold", "Italic" or "Normal". An empty
    string may be returned.
*/
QString QFontDatabase::styleString(const QFont &font)
{
    QString result = font.styleName();
    if (result.isEmpty()) {
        result = styleStringHelper(font.family(), font.weight(), font.style());
    }
    return result;
}

/*!
    Returns a string that describes the style of the \a fontInfo. For
    example, "Bold Italic", "Bold", "Italic" or "Normal". An empty
    string may be returned.
*/
QString QFontDatabase::styleString(const QFontInfo &fontInfo)
{
    QString result = fontInfo.styleName();
    if (result.isEmpty()) {
        result = styleStringHelper(fontInfo.family(), fontInfo.weight(), fontInfo.style());
    }
    return result;
}


/*!
    \class QFontDatabase
    \threadsafe

    \brief The QFontDatabase class provides information about the fonts available in the underlying window system.

    \ingroup appearance

    The most common uses of this class are to query the database for
    the list of font families() and for the pointSizes() and styles()
    that are available for each family. An alternative to pointSizes()
    is smoothSizes() which returns the sizes at which a given family
    and style will look attractive.

    If the font family is available from two or more foundries the
    foundry name is included in the family name; for example:
    "Helvetica [Adobe]" and "Helvetica [Cronyx]". When you specify a
    family, you can use "family [foundry]" format; for example:
    "Helvetica [Cronyx]". If the family has a foundry it is always
    returned using the bracketed format, as is the case with the
    value returned by families().

    The font() function returns a QFont given a family, style and
    point size.

    A family and style combination can be checked to see if it is
    italic() or bold(), and to retrieve its weight(). Similarly we can
    call isSmoothlyScalable(), isScalable() and isFixedPitch().

    Use the styleString() to obtain a text version of a style.

    The QFontDatabase class also supports some static functions, for
    example, standardSizes().

    Example:

    \snippet doc/src/snippets/qfontdatabase/main.cpp 0
    \snippet doc/src/snippets/qfontdatabase/main.cpp 1

    This example gets the list of font families, the list of
    styles for each family, and the point sizes that are available for
    each combination of family and style, displaying this information
    in a tree view.

    \sa QFont, QFontInfo, QFontMetrics, QFontComboBox, {Character Map Example}
*/

/*!
    Creates a font database object.
*/
QFontDatabase::QFontDatabase()
{
    {
        QMutexLocker locker(qGlobalFontDatabaseMutex());

        d = qGlobalFontDatabase();
        if (!d || d->families.size() > 0)
            return;

#ifndef QT_NO_FONTCONFIG
#ifdef QFONTDATABASE_DEBUG
        QElapsedTimer elapsedtimer;
        elapsedtimer.start();
#endif

        Q_ASSERT_X(qt_x11Data, "QFontDatabase",
                "A QApplication object needs to be constructed before FontConfig is used.");
        if (!qt_x11Data->has_fontconfig)
            return;

        Q_ASSERT_X(int(QUnicodeTables::ScriptCount) == SpecialLanguageCount,
                "QFontDatabase", "New scripts have been added.");
        Q_ASSERT_X(int(QUnicodeTables::ScriptCount) == SpecialCharCount,
                "QFontDatabase", "New scripts have been added.");

        FcFontSet *fonts;

        FcPattern *pattern = FcPatternCreate();
        FcDefaultSubstitute(pattern);
        FcChar8 *lang = nullptr;
        if (FcPatternGetString(pattern, FC_LANG, 0, &lang) == FcResultMatch)
            d->systemLang = QString::fromUtf8((const char *) lang);
        FcPatternDestroy(pattern);

        {
            FcObjectSet *os = FcObjectSetCreate();
            FcPattern *pattern = FcPatternCreate();
            for (qint16 i = 0; i < PatternPropertiesTblSize; i++) {
                FcObjectSetAdd(os, PatternPropertiesTbl[i]);
            }
            fonts = FcFontList(0, pattern, os);
            FcObjectSetDestroy(os);
            FcPatternDestroy(pattern);
        }

        for (int i = 0; i < fonts->nfont; i++) {
            FcChar8 *family_value = nullptr;
            FcChar8 *foundry_value = nullptr;
            int weight_value = FC_WEIGHT_MEDIUM;
            int spacing_value = FC_PROPORTIONAL;
            FcChar8 *style_value = nullptr;
            FcBool scalable = FcTrue;
            double pixel_size = 0;
            int slant_value = FC_SLANT_ROMAN;

            if (FcPatternGetString(fonts->fonts[i], FC_FAMILY, 0, &family_value) != FcResultMatch)
                continue;

            if (FcPatternGetString(fonts->fonts[i], FC_FOUNDRY, 0, &foundry_value) != FcResultMatch)
                foundry_value = nullptr;
            if (FcPatternGetInteger(fonts->fonts[i], FC_WEIGHT, 0, &weight_value) != FcResultMatch)
                weight_value = FC_WEIGHT_MEDIUM;
            if (FcPatternGetInteger (fonts->fonts[i], FC_SPACING, 0, &spacing_value) != FcResultMatch)
                spacing_value = FC_PROPORTIONAL;
            if (FcPatternGetBool(fonts->fonts[i], FC_SCALABLE, 0, &scalable) != FcResultMatch)
                scalable = FcTrue;
            if (FcPatternGetString(fonts->fonts[i], FC_STYLE, 0, &style_value) != FcResultMatch)
                style_value = nullptr;
            FcPatternGetDouble(fonts->fonts[i], FC_PIXEL_SIZE, 0, &pixel_size);
            if (FcPatternGetInteger(fonts->fonts[i], FC_SLANT, 0, &slant_value) != FcResultMatch)
                slant_value = FC_SLANT_ROMAN;

            QString fontstyle = QString::fromUtf8((const char *)style_value);
            if (fontstyle.compare(QLatin1String("Regular"), Qt::CaseInsensitive) == 0) {
                fontstyle = QLatin1String("Normal");
            }

            QtFontFamily fontfamily;
            fontfamily.family = QString::fromUtf8((const char *)family_value);
            fontfamily.foundry = QString::fromUtf8((const char *)foundry_value);
            fontfamily.style = fontstyle;
            fontfamily.fixedpitch = (spacing_value >= FC_MONO);
            fontfamily.scalable = scalable;
            fontfamily.italic = (slant_value >= FC_SLANT_ITALIC);
            fontfamily.bold = (weight_value >= FC_WEIGHT_BOLD); // or FC_WEIGHT_DEMIBOLD?
            fontfamily.weight = weight_value;
            fontfamily.pointsize = qRound(pixel_size);
            d->families.insert(fontfamily, true);
        }

        FcFontSetDestroy(fonts);

#ifdef QFONTDATABASE_DEBUG
        FD_DEBUG("QFontDatabase: loaded FontConfig: %d ms", int(elapsedtimer.elapsed()));
#endif
    }

    emit qApp->fontDatabaseChanged();
#endif // QT_NO_FONTCONFIG
}

/*!
    Returns a sorted list of the available font families.

    If a family exists in several foundries, the returned name for
    that font is in the form "family [foundry]". Examples: "Times
    [Adobe]", "Times [Cronyx]", "Palatino".
*/
QStringList QFontDatabase::families() const
{
    QStringList result;
    foreach (const QtFontFamily &fontfamily, d->families.keys()) {
        QString familyandfoundry;
        if (fontfamily.foundry.isEmpty()) {
            familyandfoundry = fontfamily.family;
        } else {
            familyandfoundry = QString::fromLatin1("%1 [%2]").arg(fontfamily.family, fontfamily.foundry);
        }
        if (result.contains(familyandfoundry)) {
            continue;
        }
        result.append(familyandfoundry);
    }
    return result;
}

/*!
    Returns a list of the styles available for the font family \a
    family. Some example styles: "Light", "Light Italic", "Bold",
    "Oblique", "Demi". The list may be empty.

    \sa families()
*/
QStringList QFontDatabase::styles(const QString &family) const
{
    QString parsedfamily, parsedfoundry;
    parseFontName(family, parsedfoundry, parsedfamily);

    QStringList result;
    foreach (const QtFontFamily &fontfamily, d->families.keys()) {
        if (fontfamily.family.compare(parsedfamily, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)) {
            continue;
        }
        result.append(fontfamily.style);
    }
    const int normalstyleindex = result.indexOf(QLatin1String("Normal"));
    if (normalstyleindex != -1) {
        result.move(normalstyleindex, 0);
    }
    return result;
}

/*!
    Returns true if the font that has family \a family and style \a
    style is fixed pitch; otherwise returns false.
*/

bool QFontDatabase::isFixedPitch(const QString &family, const QString &style) const
{
    QString parsedfamily, parsedfoundry;
    parseFontName(family, parsedfoundry, parsedfamily);

    bool result = false;
    foreach (const QtFontFamily &fontfamily, d->families.keys()) {
        if (fontfamily.family.compare(parsedfamily, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || (!style.isEmpty() && !isStyleMatch(fontfamily.style, style))) {
            continue;
        }
        result = fontfamily.fixedpitch;
        break;
    }
    return result;
}

/*!
    Returns true if the font that has family \a family and style \a
    style is smoothly scalable; otherwise returns false. If this
    function returns true, it's safe to scale this font to any size,
    and the result will always look attractive.

    \sa isScalable()
*/
bool QFontDatabase::isSmoothlyScalable(const QString &family, const QString &style) const
{
    QString parsedfamily, parsedfoundry;
    parseFontName(family, parsedfoundry, parsedfamily);

    bool result = false;
    foreach (const QtFontFamily &fontfamily, d->families.keys()) {
        if (fontfamily.family.compare(parsedfamily, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || (!style.isEmpty() && !isStyleMatch(fontfamily.style, style))) {
            continue;
        }
        result = fontfamily.scalable;
        break;
    }
    return result;
}

/*!
    Returns true if the font that has family \a family and style \a
    style is scalable; otherwise returns false.

    \sa isSmoothlyScalable()
*/
bool  QFontDatabase::isScalable(const QString &family, const QString &style) const
{
    return isSmoothlyScalable(family, style);
}


/*!
    \fn QList<int> QFontDatabase::pointSizes(const QString &family, const QString &style)
    Returns a list of the point sizes available for the font with the
    given \a family and \a style. The list may be empty.

    \sa smoothSizes(), standardSizes()
*/
QList<int> QFontDatabase::pointSizes(const QString &family, const QString &style)
{
    QString parsedfamily, parsedfoundry;
    parseFontName(family, parsedfoundry, parsedfamily);

    QList<int> result;
    foreach (const QtFontFamily &fontfamily, d->families.keys()) {
        if (fontfamily.family.compare(parsedfamily, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || !isStyleMatch(fontfamily.style, style)) {
            continue;
        }
        result.append(fontfamily.pointsize);
        break;
    }
    qSort(result);
    return result;
}

/*!
    Returns a QFont object that has family \a family, style \a style
    and point size \a pointSize. If no matching font could be created,
    a QFont object that uses the application's default font is
    returned.
*/
QFont QFontDatabase::font(const QString &family, const QString &style,
                          int pointSize) const
{
    QString parsedfamily, parsedfoundry;
    parseFontName(family, parsedfoundry, parsedfamily);

    QFont result = QApplication::font();
    foreach (const QtFontFamily &fontfamily, d->families.keys()) {
        if (fontfamily.family.compare(parsedfamily, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || !isStyleMatch(fontfamily.style, style)) {
            continue;
        }
        if (fontfamily.pointsize != pointSize && !fontfamily.scalable) {
            continue;
        }

        result = QFont(fontfamily.family, pointSize, fontfamily.weight, fontfamily.italic);
        result.setStyleName(fontfamily.style);
        result.setBold(fontfamily.bold);
        break;
    }
    return result;
}


/*!
    \fn QList<int> QFontDatabase::smoothSizes(const QString &family, const QString &style)
    Returns the point sizes of a font with the given \a family and \a style
    that will look attractive. The list may be empty.
    For non-scalable fonts and bitmap scalable fonts, this function
    is equivalent to pointSizes().

  \sa pointSizes(), standardSizes()
*/
QList<int> QFontDatabase::smoothSizes(const QString &family, const QString &style)
{
    QString parsedfamily, parsedfoundry;
    parseFontName(family, parsedfoundry, parsedfamily);

    QList<int> result;
    foreach (const QtFontFamily &fontfamily, d->families.keys()) {
        if (fontfamily.family.compare(parsedfamily, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || !isStyleMatch(fontfamily.style, style)) {
            continue;
        }
        if (fontfamily.scalable) {
            result = standardSizes();
        } else {
            result.append(fontfamily.pointsize);
        }
        break;
    }
    qSort(result);
    return result;
}


/*!
    Returns a list of standard font sizes.

    \sa smoothSizes(), pointSizes()
*/
QList<int> QFontDatabase::standardSizes()
{
    static const QList<int> sizes = QList<int>()
        << 6
        << 7
        << 8
        << 9
        << 10
        << 11
        << 12
        << 14
        << 16
        << 18
        << 20
        << 22
        << 24
        << 26
        << 28
        << 36
        << 48
        << 72;
    return sizes;
}


/*!
    Returns true if the font that has family \a family and style \a
    style is italic; otherwise returns false.

    \sa weight(), bold()
*/
bool QFontDatabase::italic(const QString &family, const QString &style) const
{
    QString parsedfamily, parsedfoundry;
    parseFontName(family, parsedfoundry, parsedfamily);

    bool result = false;
    foreach (const QtFontFamily &fontfamily, d->families.keys()) {
        if (fontfamily.family.compare(parsedfamily, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || !isStyleMatch(fontfamily.style, style)) {
            continue;
        }
        result = fontfamily.italic;
        break;
    }
    return result;
}


/*!
    Returns true if the font that has family \a family and style \a
    style is bold; otherwise returns false.

    \sa italic(), weight()
*/
bool QFontDatabase::bold(const QString &family, const QString &style) const
{
    QString parsedfamily, parsedfoundry;
    parseFontName(family, parsedfoundry, parsedfamily);

    bool result = false;
    foreach (const QtFontFamily &fontfamily, d->families.keys()) {
        if (fontfamily.family.compare(parsedfamily, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || !isStyleMatch(fontfamily.style, style)) {
            continue;
        }
        result = fontfamily.bold;
        break;
    }
    return result;
}


/*!
    Returns the weight of the font that has family \a family and style
    \a style. If there is no such family and style combination,
    returns -1.

    \sa italic(), bold()
*/
int QFontDatabase::weight(const QString &family, const QString &style) const
{
    QString parsedfamily, parsedfoundry;
    parseFontName(family, parsedfoundry, parsedfamily);

    int result = -1;
    foreach (const QtFontFamily &fontfamily, d->families.keys()) {
        if (fontfamily.family.compare(parsedfamily, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || !isStyleMatch(fontfamily.style, style)) {
            continue;
        }
        result = fontfamily.weight;
        break;
    }
    return result;
}


/*!
    Returns true if font \a family is available, false otherwise. Aliases
    such as "Sans Serif" are resolved.
*/
bool QFontDatabase::hasFamily(const QString &family) const
{
    QString parsedfamily, parsedfoundry;
    parseFontName(family, parsedfoundry, parsedfamily);

    const QString familyalias = resolveFontFamilyAlias(parsedfamily);

    bool result = false;
    foreach (const QtFontFamily &fontfamily, d->families.keys()) {
        if (fontfamily.family.compare(familyalias, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)) {
            continue;
        }
        result = true;
        break;
    }
    return result;
}

void QFontDatabase::parseFontName(const QString &name, QString &foundry, QString &family)
{
    QT_PREPEND_NAMESPACE(parseFontName)(name, foundry, family);
}

/*!
    \since 4.4

    Returns true if font rendering is supported outside the GUI
    thread, false otherwise. In other words, a return value of false
    means that all QPainter::drawText() calls outside the GUI thread
    will not produce readable output.

    \sa {Thread-Support in Qt Modules#Painting In Threads}{Painting In Threads}
*/
bool QFontDatabase::supportsThreadedFontRendering()
{
#if defined(QT_NO_FONTCONFIG)
    return false;
#else
    return qt_x11Data->has_fontconfig;
#endif
}

QT_END_NAMESPACE

#include "moc_qfontdatabase.h"

