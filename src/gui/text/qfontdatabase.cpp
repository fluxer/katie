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

#define SMOOTH_SCALABLE 0xffff

static int getFontWeight(const QString &weightString)
{
    QString s = weightString.toLower();

    // Test in decreasing order of commonness
    if (s == QLatin1String("medium") ||
        s == QLatin1String("normal")
        || s.compare(QApplication::translate("QFontDatabase", "Normal"), Qt::CaseInsensitive) == 0)
        return QFont::Normal;
    if (s == QLatin1String("bold")
        || s.compare(QApplication::translate("QFontDatabase", "Bold"), Qt::CaseInsensitive) == 0)
        return QFont::Bold;
    if (s == QLatin1String("demibold") || s == QLatin1String("demi bold")
        || s.compare(QApplication::translate("QFontDatabase", "Demi Bold"), Qt::CaseInsensitive) == 0)
        return QFont::DemiBold;
    if (s == QLatin1String("black")
        || s.compare(QApplication::translate("QFontDatabase", "Black"), Qt::CaseInsensitive) == 0)
        return QFont::Black;
    if (s == QLatin1String("light"))
        return QFont::Light;

    if (s.contains(QLatin1String("bold"))
        || s.contains(QApplication::translate("QFontDatabase", "Bold"), Qt::CaseInsensitive)) {
        if (s.contains(QLatin1String("demi"))
            || s.compare(QApplication::translate("QFontDatabase", "Demi"), Qt::CaseInsensitive) == 0)
            return (int) QFont::DemiBold;
        return (int) QFont::Bold;
    }

    if (s.contains(QLatin1String("light"))
        || s.compare(QApplication::translate("QFontDatabase", "Light"), Qt::CaseInsensitive) == 0)
        return (int) QFont::Light;

    if (s.contains(QLatin1String("black"))
        || s.compare(QApplication::translate("QFontDatabase", "Black"), Qt::CaseInsensitive) == 0)
        return (int) QFont::Black;

    return (int) QFont::Normal;
}

struct QtFontEncoding
{
    signed int encoding : 16;
    uchar pitch   : 8;
};

struct  QtFontSize
{
#ifdef Q_WS_X11
    QtFontEncoding *encodings;
    QtFontEncoding *encodingID(int id, bool add = false);
    unsigned short count : 16;
#endif // Q_WS_X11

    unsigned short pixelSize : 16;
};


#ifdef Q_WS_X11
QtFontEncoding *QtFontSize::encodingID(int id, bool add)
{
    for (int i = 0; i < count; ++i) {
        if (encodings[i].encoding == id)
            return encodings + i;
    }

    if (!add) return 0;

    if (!(count % 4)) {
        QtFontEncoding *newEncodings = (QtFontEncoding *)
                    realloc(encodings,
                             (((count+4) >> 2) << 2) * sizeof(QtFontEncoding));
        Q_CHECK_PTR(newEncodings);
        encodings = newEncodings;
    }
    encodings[count].encoding = id;
    encodings[count].pitch = '*';
    return encodings + count++;
}
#endif // Q_WS_X11

struct QtFontStyle
{
    struct Key {
        Key(const QString &styleString);
        Key() : style(QFont::StyleNormal),
                weight(QFont::Normal), stretch(0) { }
        Key(const Key &o) : style(o.style), weight(o.weight), stretch(o.stretch) { }
        uint style : 2;
        signed int  weight : 8;
        signed int stretch : 12;

        bool operator==(const Key & other) const {
            return (style == other.style && weight == other.weight &&
                    (stretch == 0 || other.stretch == 0 || stretch == other.stretch));
        }
        bool operator!=(const Key &other) const {
            return !operator==(other);
        }
        bool operator <(const Key &o) const {
            int x = (style << 12) + (weight << 14) + stretch;
            int y = (o.style << 12) + (o.weight << 14) + o.stretch;
            return (x < y);
        }
    };

    QtFontStyle(const Key &k)
        : key(k), smoothScalable(false),
          count(0), pixelSizes(0)
    {
    }

    ~QtFontStyle() {
#ifdef Q_WS_X11
        while (count) {
            // bitfield count-- in while condition does not work correctly in mwccsym2
            count--;
            free(pixelSizes[count].encodings);
        }
#endif
        free(pixelSizes);
    }

    Key key;
    bool smoothScalable;
    int count;
    QtFontSize *pixelSizes;
    QString styleName;

    QtFontSize *pixelSize(unsigned short size, bool = false);
};

QtFontStyle::Key::Key(const QString &styleString)
    : style(QFont::StyleNormal), weight(QFont::Normal), stretch(0)
{
    weight = getFontWeight(styleString);

    if (styleString.contains(QLatin1String("Italic"))
        || styleString.contains(QApplication::translate("QFontDatabase", "Italic")))
        style = QFont::StyleItalic;
    else if (styleString.contains(QLatin1String("Oblique"))
             || styleString.contains(QApplication::translate("QFontDatabase", "Oblique")))
        style = QFont::StyleOblique;
}

QtFontSize *QtFontStyle::pixelSize(unsigned short size, bool add)
{
    for (int i = 0; i < count; i++) {
        if (pixelSizes[i].pixelSize == size)
            return pixelSizes + i;
    }
    if (!add)
        return 0;

    if (!pixelSizes) {
        // Most style have only one font size, we avoid waisting memory
        QtFontSize *newPixelSizes = (QtFontSize *)malloc(sizeof(QtFontSize));
        Q_CHECK_PTR(newPixelSizes);
        pixelSizes = newPixelSizes;
    } else if (!(count % 8) || count == 1) {
        QtFontSize *newPixelSizes = (QtFontSize *)
                     realloc(pixelSizes,
                              (((count+8) >> 3) << 3) * sizeof(QtFontSize));
        Q_CHECK_PTR(newPixelSizes);
        pixelSizes = newPixelSizes;
    }
    pixelSizes[count].pixelSize = size;
#ifdef Q_WS_X11
    pixelSizes[count].count = 0;
    pixelSizes[count].encodings = 0;
#endif
    return pixelSizes + (count++);
}

struct QtFontFoundry
{
    QtFontFoundry(const QString &n) : name(n), count(0), styles(0) {}
    ~QtFontFoundry() {
        while (count--)
            delete styles[count];
        free(styles);
    }

    QString name;

    int count;
    QtFontStyle **styles;
    QtFontStyle *style(const QtFontStyle::Key &, const QString & = QString(), bool = false);
};

QtFontStyle *QtFontFoundry::style(const QtFontStyle::Key &key, const QString &styleName, bool create)
{
    int pos = 0;
    for (; pos < count; pos++) {
        bool hasStyleName = !styleName.isEmpty(); // search styleName first if available
        if (hasStyleName && !styles[pos]->styleName.isEmpty()) {
            if (styles[pos]->styleName == styleName)
                return styles[pos];
        } else {
            if (styles[pos]->key == key)
                return styles[pos];
        }
    }
    if (!create)
        return 0;

    // qDebug("adding key (weight=%d, style=%d, stretch=%d) at %d", key.weight, key.style, key.stretch, pos);
    if (!(count % 8)) {
        QtFontStyle **newStyles = (QtFontStyle **)
                 realloc(styles, (((count+8) >> 3) << 3) * sizeof(QtFontStyle *));
        Q_CHECK_PTR(newStyles);
        styles = newStyles;
    }

    QtFontStyle *style = new QtFontStyle(key);
    style->styleName = styleName;
    styles[pos] = style;
    count++;
    return styles[pos];
}

struct  QtFontFamily
{
    QtFontFamily(const QString &n)
        :
#ifdef Q_WS_X11
        fixedPitch(true),
#else
        fixedPitch(false),
#endif
        name(n), count(0), foundries(0)
    {

    }
    ~QtFontFamily() {
        while (count--)
            delete foundries[count];
        free(foundries);
    }

    bool fixedPitch;
    QString name;
    int count;
    QtFontFoundry **foundries;

    QtFontFoundry *foundry(const QString &f, bool = false);
};

QtFontFoundry *QtFontFamily::foundry(const QString &f, bool create)
{
    if (f.isNull() && count == 1)
        return foundries[0];

    for (int i = 0; i < count; i++) {
        if (foundries[i]->name.compare(f, Qt::CaseInsensitive) == 0)
            return foundries[i];
    }
    if (!create)
        return 0;

    if (!(count % 8)) {
        QtFontFoundry **newFoundries = (QtFontFoundry **)
                    realloc(foundries,
                             (((count+8) >> 3) << 3) * sizeof(QtFontFoundry *));
        Q_CHECK_PTR(newFoundries);
        foundries = newFoundries;
    }

    foundries[count] = new QtFontFoundry(f);
    return foundries[count++];
}

class QFontDatabasePrivate
{
public:
    QFontDatabasePrivate()
        : count(0), families(0), reregisterAppFonts(false)
    { }

    ~QFontDatabasePrivate() {
        free();
    }
    QtFontFamily *family(const QString &f, bool = false);
    void free() {
        while (count--)
            delete families[count];
        ::free(families);
        families = 0;
        count = 0;
        // don't clear the memory fonts!
    }

    int count;
#if defined(Q_WS_X11) && !defined(QT_NO_FONTCONFIG)
    QString systemLang;
#endif
    QtFontFamily **families;

    struct ApplicationFont {
        QString fileName;
        QByteArray data;
        QStringList families;
    };
    QVector<ApplicationFont> applicationFonts;
    int addAppFont(const QByteArray &fontData, const QString &fileName);
    bool reregisterAppFonts;
    bool isApplicationFont(const QString &fileName);

    void invalidate();
};

void QFontDatabasePrivate::invalidate()
{
    QFontCache::instance()->clear();
    free();
    emit qApp->fontDatabaseChanged();
}

QtFontFamily *QFontDatabasePrivate::family(const QString &f, bool create)
{
    int low = 0;
    int high = count;
    int pos = count / 2;
    int res = 1;
    if (count) {
        while ((res = families[pos]->name.compare(f, Qt::CaseInsensitive)) && pos != low) {
            if (res > 0)
                high = pos;
            else
                low = pos;
            pos = (high + low) / 2;
        }
        if (!res)
            return families[pos];
    }
    if (!create)
        return 0;

    if (res < 0)
        pos++;

    // qDebug() << "adding family " << f.toLatin1() << " at " << pos << " total=" << count;
    if (!(count % 8)) {
        QtFontFamily **newFamilies = (QtFontFamily **)
                   realloc(families,
                            (((count+8) >> 3) << 3) * sizeof(QtFontFamily *));
        Q_CHECK_PTR(newFamilies);
        families = newFamilies;
    }

    QtFontFamily *family = new QtFontFamily(f);
    memmove(families + pos + 1, families + pos, (count-pos)*sizeof(QtFontFamily *));
    families[pos] = family;
    count++;
    return families[pos];
}

/*!
  \internal

  This makes sense of the font family name:

  if the family name contains a '[' and a ']', then we take the text
  between the square brackets as the foundry, and the text before the
  square brackets as the family (ie. "Arial [Monotype]")
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


struct QtFontDesc
{
    inline QtFontDesc() : family(0), foundry(0), style(0), size(0), encoding(0), familyIndex(-1) {}
    QtFontFamily *family;
    QtFontFoundry *foundry;
    QtFontStyle *style;
    QtFontSize *size;
    QtFontEncoding *encoding;
    int familyIndex;
};

static void match(int script, const QFontDef &request, const QString &family_name,
                  const QString &foundry_name, QtFontDesc *desc);

#if defined(Q_WS_X11)
static void getEngineData(const QFontPrivate *d, const QFontCache::Key &key)
{
    // look for the requested font in the engine data cache
    d->engineData = QFontCache::instance()->findEngineData(key);
    if (!d->engineData) {
        // create a new one
        d->engineData = new QFontEngineData;
        QFontCache::instance()->insertEngineData(key, d->engineData);
    } else {
        d->engineData->ref.ref();
    }
}
#endif

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

    // append the substitute list for each family in family_list
    QStringList subs_list;
    QStringList::ConstIterator it = family_list.constBegin(), end = family_list.constEnd();
    for (; it != end; ++it)
        subs_list += QFont::substitutes(*it);
//         qDebug() << "adding substs: " << subs_list;

    family_list += subs_list;

    return family_list;
}

Q_GLOBAL_STATIC(QFontDatabasePrivate, privateDb)
static std::recursive_mutex qGlobalFontDatabaseMutex;

// used in qfontengine_x11.cpp
std::recursive_mutex& qt_fontdatabase_mutex()
{
    return qGlobalFontDatabaseMutex;
}

QT_BEGIN_INCLUDE_NAMESPACE
#if defined(Q_WS_X11)
#  include "qfontdatabase_x11_p.h"
#endif
QT_END_INCLUDE_NAMESPACE

#if !defined(Q_WS_X11)
QString QFontDatabase::resolveFontFamilyAlias(const QString &family)
{
    return family;
}
#endif

static QtFontStyle *bestStyle(QtFontFoundry *foundry, const QtFontStyle::Key &styleKey,
                              const QString &styleName = QString())
{
    int best = 0;
    int dist = 0xffff;

    for ( int i = 0; i < foundry->count; i++ ) {
        QtFontStyle *style = foundry->styles[i];

        if (!styleName.isEmpty() && styleName == style->styleName) {
            dist = 0;
            best = i;
            break;
        }

        int d = qAbs( styleKey.weight - style->key.weight );

        if ( styleKey.stretch != 0 && style->key.stretch != 0 ) {
            d += qAbs( styleKey.stretch - style->key.stretch );
        }

        if (styleKey.style != style->key.style) {
            if (styleKey.style != QFont::StyleNormal && style->key.style != QFont::StyleNormal)
                // one is italic, the other oblique
                d += 0x0001;
            else
                d += 0x1000;
        }

        if ( d < dist ) {
            best = i;
            dist = d;
        }
    }

    FM_DEBUG( "          best style has distance 0x%x", dist );
    return foundry->styles[best];
}

#if defined(Q_WS_X11)
static inline QtFontEncoding *findEncoding(QtFontSize *size)
{
    return size->encodingID(-1); // -1 == prefer Freetype;
}
#endif // Q_WS_X11

static
unsigned int bestFoundry(unsigned int score, int styleStrategy,
                         const QtFontFamily *family, const QString &foundry_name,
                         QtFontStyle::Key styleKey, int pixelSize, char pitch,
                         QtFontDesc *desc)
{
    desc->foundry = 0;
    desc->style = 0;
    desc->size = 0;
    desc->encoding = 0;


    FM_DEBUG("  REMARK: looking for best foundry for family '%s' [%d]", family->name.toLatin1().constData(), family->count);

    for (int x = 0; x < family->count; ++x) {
        QtFontFoundry *foundry = family->foundries[x];
        if (!foundry_name.isEmpty() && foundry->name.compare(foundry_name, Qt::CaseInsensitive) != 0)
            continue;

        FM_DEBUG("          looking for matching style in foundry '%s' %d",
                 foundry->name.isEmpty() ? "-- none --" : foundry->name.toLatin1().constData(), foundry->count);

        QtFontStyle *style = bestStyle(foundry, styleKey);

        if (!style->smoothScalable && (styleStrategy & QFont::ForceOutline)) {
            FM_DEBUG("            ForceOutline set, but not smoothly scalable");
            continue;
        }

        int px = -1;
        QtFontSize *size = 0;

        // 1. see if we have an exact matching size
        if (!(styleStrategy & QFont::ForceOutline)) {
            size = style->pixelSize(pixelSize);
            if (size) {
                FM_DEBUG("          found exact size match (%d pixels)", size->pixelSize);
                px = size->pixelSize;
            }
        }

        // 2. see if we have a smoothly scalable font
        if (!size && style->smoothScalable && ! (styleStrategy & QFont::PreferBitmap)) {
            size = style->pixelSize(SMOOTH_SCALABLE);
            if (size) {
                FM_DEBUG("          found smoothly scalable font (%d pixels)", pixelSize);
                px = pixelSize;
            }
        }

#ifdef Q_WS_X11
        QtFontEncoding *encoding = 0;
#endif

        // 3. find closest size match
        if (! size) {
            unsigned int distance = ~0u;
            for (int x = 0; x < style->count; ++x) {
#ifdef Q_WS_X11
                encoding = findEncoding(style->pixelSizes + x);
                if (!encoding) {
                    FM_DEBUG("          size %3d does not support the script we want",
                             style->pixelSizes[x].pixelSize);
                    continue;
                }
#endif

                unsigned int d;
                if (style->pixelSizes[x].pixelSize < pixelSize) {
                    // penalize sizes that are smaller than the
                    // requested size, due to truncation from floating
                    // point to integer conversions
                    d = pixelSize - style->pixelSizes[x].pixelSize + 1;
                } else {
                    d = style->pixelSizes[x].pixelSize - pixelSize;
                }

                if (d < distance) {
                    distance = d;
                    size = style->pixelSizes + x;
                    FM_DEBUG("          best size so far: %3d (%d)", size->pixelSize, pixelSize);
                }
            }

            if (!size) {
                FM_DEBUG("          no size supports the script we want");
                continue;
            }

            px = size->pixelSize;
        }

#ifdef Q_WS_X11
        if (size) {
            encoding = findEncoding(size);
            if (!encoding) size = 0;
        }
        if (! encoding) {
            FM_DEBUG("          foundry doesn't support the script we want");
            continue;
        }
#endif // Q_WS_X11

        unsigned int this_score = 0x0000;
        enum {
            PitchMismatch       = 0x4000,
            StyleMismatch       = 0x2000,
            BitmapScaledPenalty = 0x1000,
            EncodingMismatch    = 0x0002,
            XLFDPenalty         = 0x0001
        };
#ifdef Q_WS_X11
        if (encoding->encoding != -1) {
            this_score += XLFDPenalty;
            if (encoding->encoding != QFontPrivate::defaultEncodingID)
                this_score += EncodingMismatch;
        }
        if (pitch != '*') {
            if (!(pitch == 'm' && encoding->pitch == 'c') && pitch != encoding->pitch)
                this_score += PitchMismatch;
        }
#else
        if (pitch != '*') {
            if ((pitch == 'm' && !family->fixedPitch)
                || (pitch == 'p' && family->fixedPitch))
                this_score += PitchMismatch;
        }
#endif
        if (styleKey != style->key)
            this_score += StyleMismatch;
        if (!style->smoothScalable && px != size->pixelSize) // bitmap scaled
            this_score += BitmapScaledPenalty;
        if (px != pixelSize) // close, but not exact, size match
            this_score += qAbs(px - pixelSize);

        if (this_score < score) {
            FM_DEBUG("          found a match: score %x best score so far %x",
                     this_score, score);

            score = this_score;
            desc->foundry = foundry;
            desc->style = style;
            desc->size = size;
#ifdef Q_WS_X11
            desc->encoding = encoding;
#endif // Q_WS_X11
        } else {
            FM_DEBUG("          score %x no better than best %x", this_score, score);
        }
    }

    return score;
}

/*!
    \internal

    Tries to find the best match for a given request and family/foundry
*/
static void match(int script, const QFontDef &request, const QString &family_name,
                  const QString &foundry_name, QtFontDesc *desc)
{
    QtFontStyle::Key styleKey;
    styleKey.style = request.style;
    styleKey.weight = request.weight;
    styleKey.stretch = request.stretch;
    char pitch = request.ignorePitch ? '*' : request.fixedPitch ? 'm' : 'p';


    FM_DEBUG("QFontDatabase::match\n"
             "  request:\n"
             "    family: %s [%s], script: %d\n"
             "    weight: %d, style: %d\n"
             "    stretch: %d\n"
             "    pixelSize: %g\n"
             "    pitch: %c",
             family_name.isEmpty() ? "-- first in script --" : family_name.toLatin1().constData(),
             foundry_name.isEmpty() ? "-- any --" : foundry_name.toLatin1().constData(),
             script, request.weight, request.style, request.stretch, request.pixelSize, pitch);

    desc->family = 0;
    desc->foundry = 0;
    desc->style = 0;
    desc->size = 0;
    desc->encoding = 0;
    desc->familyIndex = -1;

    unsigned int score = ~0u;

    if (qt_x11Data->has_fontconfig) {
        initializeFontDb();
    }

    QFontDatabasePrivate *db = privateDb();
    for (int x = 0; x < db->count; ++x) {
        QtFontDesc test;
        test.family = db->families[x];
        test.familyIndex = x;

        if (!family_name.isEmpty()
            && test.family->name.compare(family_name, Qt::CaseInsensitive) != 0)
            continue;

        // as we know the script is supported, we can be sure
        // to find a matching font here.
        unsigned int newscore =
            bestFoundry(score, request.styleStrategy,
                        test.family, foundry_name, styleKey, request.pixelSize, pitch,
                        &test);
        if (test.foundry == 0) {
            // the specific foundry was not found, so look for
            // any foundry matching our requirements
            newscore = bestFoundry(score, request.styleStrategy, test.family,
                                   QString(), styleKey, request.pixelSize,
                                   pitch, &test);
        }

        if (newscore < score) {
            score = newscore;
            *desc = test;
        }
        if (newscore < 10) // xlfd instead of FT... just accept it
            break;
    }
}

static QString styleStringHelper(int weight, QFont::Style style)
{
    QString result;
    if (weight >= QFont::Black)
        result = QApplication::translate("QFontDatabase", "Black");
    else if (weight >= QFont::Bold)
        result = QApplication::translate("QFontDatabase", "Bold");
    else if (weight >= QFont::DemiBold)
        result = QApplication::translate("QFontDatabase", "Demi Bold");
    else if (weight < QFont::Normal)
        result = QApplication::translate("QFontDatabase", "Light");

    if (style == QFont::StyleItalic)
        result += QLatin1Char(' ') + QApplication::translate("QFontDatabase", "Italic");
    else if (style == QFont::StyleOblique)
        result += QLatin1Char(' ') + QApplication::translate("QFontDatabase", "Oblique");

    if (result.isEmpty())
        result = QApplication::translate("QFontDatabase", "Normal");

    return result.simplified();
}

/*!
    Returns a string that describes the style of the \a font. For
    example, "Bold Italic", "Bold", "Italic" or "Normal". An empty
    string may be returned.
*/
QString QFontDatabase::styleString(const QFont &font)
{
    return font.styleName().isEmpty() ? styleStringHelper(font.weight(), font.style())
                                      : font.styleName();
}

/*!
    Returns a string that describes the style of the \a fontInfo. For
    example, "Bold Italic", "Bold", "Italic" or "Normal". An empty
    string may be returned.
*/
QString QFontDatabase::styleString(const QFontInfo &fontInfo)
{
    return fontInfo.styleName().isEmpty() ? styleStringHelper(fontInfo.weight(), fontInfo.style())
                                          : fontInfo.styleName();
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
    family, you can either use the old hyphenated "foundry-family"
    format or the bracketed "family [foundry]" format; for example:
    "Cronyx-Helvetica" or "Helvetica [Cronyx]". If the family has a
    foundry it is always returned using the bracketed format, as is
    the case with the value returned by families().

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
    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);
    createDatabase();
    d = privateDb();
}

/*!
    Returns a sorted list of the available font families.

    If a family exists in several foundries, the returned name for
    that font is in the form "family [foundry]". Examples: "Times
    [Adobe]", "Times [Cronyx]", "Palatino".
*/
QStringList QFontDatabase::families() const
{
    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

    createDatabase();

    QStringList flist;
    for (int i = 0; i < d->count; i++) {
        QtFontFamily *f = d->families[i];
        if (f->count == 0)
            continue;
        if (f->count == 1) {
            flist.append(f->name);
        } else {
            for (int j = 0; j < f->count; j++) {
                QString str = f->name;
                QString foundry = f->foundries[j]->name;
                if (!foundry.isEmpty()) {
                    str += QLatin1String(" [");
                    str += foundry;
                    str += QLatin1Char(']');
                }
                flist.append(str);
            }
        }
    }
    return flist;
}

/*!
    Returns a list of the styles available for the font family \a
    family. Some example styles: "Light", "Light Italic", "Bold",
    "Oblique", "Demi". The list may be empty.

    \sa families()
*/
QStringList QFontDatabase::styles(const QString &family) const
{
    QString familyName, foundryName;
    parseFontName(family, foundryName, familyName);

    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

    createDatabase();

    QStringList l;
    QtFontFamily *f = d->family(familyName);
    if (!f)
        return l;

    QtFontFoundry allStyles(foundryName);
    for (int j = 0; j < f->count; j++) {
        QtFontFoundry *foundry = f->foundries[j];
        if (foundryName.isEmpty() || foundry->name.compare(foundryName, Qt::CaseInsensitive) == 0) {
            for (int k = 0; k < foundry->count; k++) {
                QtFontStyle::Key ke(foundry->styles[k]->key);
                ke.stretch = 0;
                allStyles.style(ke, foundry->styles[k]->styleName, true);
            }
        }
    }

    for (int i = 0; i < allStyles.count; i++) {
        l.append(allStyles.styles[i]->styleName.isEmpty() ?
                 styleStringHelper(allStyles.styles[i]->key.weight,
                                   (QFont::Style)allStyles.styles[i]->key.style) :
                 allStyles.styles[i]->styleName);
    }
    return l;
}

/*!
    Returns true if the font that has family \a family and style \a
    style is fixed pitch; otherwise returns false.
*/

bool QFontDatabase::isFixedPitch(const QString &family,
                                 const QString &style) const
{
    Q_UNUSED(style);

    QString familyName, foundryName;
    parseFontName(family, foundryName, familyName);

    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

    createDatabase();

    QtFontFamily *f = d->family(familyName);
    return (f && f->fixedPitch);
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
    QString familyName, foundryName;
    parseFontName(family, foundryName, familyName);

    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

    createDatabase();

    QtFontStyle::Key styleKey(style);

    QtFontFamily *f = d->family(familyName);
    if (!f) return false;

    for (int j = 0; j < f->count; j++) {
        QtFontFoundry *foundry = f->foundries[j];
        if (foundryName.isEmpty() || foundry->name.compare(foundryName, Qt::CaseInsensitive) == 0) {
            for (int k = 0; k < foundry->count; k++)
                if ((style.isEmpty() ||
                     foundry->styles[k]->styleName == style ||
                     foundry->styles[k]->key == styleKey) && foundry->styles[k]->smoothScalable) {
                    return true;
                }
        }
    }
    return false;
}

/*!
    Returns true if the font that has family \a family and style \a
    style is scalable; otherwise returns false.

    \sa isSmoothlyScalable()
*/
bool  QFontDatabase::isScalable(const QString &family,
                                 const QString &style) const
{
    return isSmoothlyScalable(family, style);
}


/*!
    \fn QList<int> QFontDatabase::pointSizes(const QString &family, const QString &style)
    Returns a list of the point sizes available for the font with the
    given \a family and \a style. The list may be empty.

    \sa smoothSizes(), standardSizes()
*/
QList<int> QFontDatabase::pointSizes(const QString &family,
                                           const QString &styleName)
{
    QString familyName, foundryName;
    parseFontName(family, foundryName, familyName);

    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

    createDatabase();

    QtFontStyle::Key styleKey(styleName);

    QList<int> sizes;

    QtFontFamily *fam = d->family(familyName);
    if (!fam) return sizes;


    const int dpi = QX11Info::appDpiY();

    for (int j = 0; j < fam->count; j++) {
        QtFontFoundry *foundry = fam->foundries[j];
        if (foundryName.isEmpty() || foundry->name.compare(foundryName, Qt::CaseInsensitive) == 0) {
            QtFontStyle *style = foundry->style(styleKey, styleName);
            if (!style) continue;

            if (style->smoothScalable) {
                return standardSizes();
            }
            for (int l = 0; l < style->count; l++) {
                const QtFontSize *size = style->pixelSizes + l;

                if (size->pixelSize != 0 && size->pixelSize != USHRT_MAX) {
                    const uint pointSize = qRound(size->pixelSize * qreal(72.0) / dpi);
                    if (! sizes.contains(pointSize))
                        sizes.append(pointSize);
                }
            }
        }
    }

    qSort(sizes);
    return sizes;
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
    QString familyName, foundryName;
    parseFontName(family, foundryName, familyName);

    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

    createDatabase();

    QtFontFoundry allStyles(foundryName);
    QtFontFamily *f = d->family(familyName);
    if (!f) return QApplication::font();

    for (int j = 0; j < f->count; j++) {
        QtFontFoundry *foundry = f->foundries[j];
        if (foundryName.isEmpty() || foundry->name.compare(foundryName, Qt::CaseInsensitive) == 0) {
            for (int k = 0; k < foundry->count; k++)
                allStyles.style(foundry->styles[k]->key, foundry->styles[k]->styleName, true);
        }
    }

    QtFontStyle::Key styleKey(style);
    QtFontStyle *s = bestStyle(&allStyles, styleKey, style);

    if (!s) // no styles found?
        return QApplication::font();

    QFont fnt(family, pointSize, s->key.weight);
    fnt.setStyle((QFont::Style)s->key.style);
    if (!s->styleName.isEmpty())
        fnt.setStyleName(s->styleName);
    return fnt;
}


/*!
    \fn QList<int> QFontDatabase::smoothSizes(const QString &family, const QString &style)
    Returns the point sizes of a font with the given \a family and \a style
    that will look attractive. The list may be empty.
    For non-scalable fonts and bitmap scalable fonts, this function
    is equivalent to pointSizes().

  \sa pointSizes(), standardSizes()
*/
QList<int> QFontDatabase::smoothSizes(const QString &family,
                                            const QString &styleName)
{
    QString familyName, foundryName;
    parseFontName(family, foundryName, familyName);

    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

    createDatabase();

    QtFontStyle::Key styleKey(styleName);

    QList<int> sizes;

    QtFontFamily *fam = d->family(familyName);
    if (!fam)
        return sizes;

#ifdef Q_WS_X11
    const int dpi = QX11Info::appDpiY();
#else
    const int dpi = QX11Info::appDpiY(); // embedded
#endif

    for (int j = 0; j < fam->count; j++) {
        QtFontFoundry *foundry = fam->foundries[j];
        if (foundryName.isEmpty() || foundry->name.compare(foundryName, Qt::CaseInsensitive) == 0) {
            QtFontStyle *style = foundry->style(styleKey, styleName);
            if (!style) continue;

            if (style->smoothScalable) {
                return QFontDatabase::standardSizes();
            }
            for (int l = 0; l < style->count; l++) {
                const QtFontSize *size = style->pixelSizes + l;

                if (size->pixelSize != 0 && size->pixelSize != USHRT_MAX) {
                    const uint pointSize = qRound(size->pixelSize * qreal(72.0) / dpi);
                    if (! sizes.contains(pointSize))
                        sizes.append(pointSize);
                }
            }
        }
    }

    qSort(sizes);
    return sizes;
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
    QString familyName, foundryName;
    parseFontName(family, foundryName, familyName);

    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

    createDatabase();

    QtFontFoundry allStyles(foundryName);
    QtFontFamily *f = d->family(familyName);
    if (!f) return false;

    for (int j = 0; j < f->count; j++) {
        QtFontFoundry *foundry = f->foundries[j];
        if (foundryName.isEmpty() || foundry->name.compare(foundryName, Qt::CaseInsensitive) == 0) {
            for (int k = 0; k < foundry->count; k++)
                allStyles.style(foundry->styles[k]->key, foundry->styles[k]->styleName, true);
        }
    }

    QtFontStyle::Key styleKey(style);
    QtFontStyle *s = allStyles.style(styleKey, style);
    return s && s->key.style == QFont::StyleItalic;
}


/*!
    Returns true if the font that has family \a family and style \a
    style is bold; otherwise returns false.

    \sa italic(), weight()
*/
bool QFontDatabase::bold(const QString &family,
                          const QString &style) const
{
    QString familyName, foundryName;
    parseFontName(family, foundryName, familyName);

    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

    createDatabase();

    QtFontFoundry allStyles(foundryName);
    QtFontFamily *f = d->family(familyName);
    if (!f) return false;

    for (int j = 0; j < f->count; j++) {
        QtFontFoundry *foundry = f->foundries[j];
        if (foundryName.isEmpty() ||
            foundry->name.compare(foundryName, Qt::CaseInsensitive) == 0) {
            for (int k = 0; k < foundry->count; k++)
                allStyles.style(foundry->styles[k]->key, foundry->styles[k]->styleName, true);
        }
    }

    QtFontStyle::Key styleKey(style);
    QtFontStyle *s = allStyles.style(styleKey, style);
    return s && s->key.weight >= QFont::Bold;
}


/*!
    Returns the weight of the font that has family \a family and style
    \a style. If there is no such family and style combination,
    returns -1.

    \sa italic(), bold()
*/
int QFontDatabase::weight(const QString &family,
                           const QString &style) const
{
    QString familyName, foundryName;
    parseFontName(family, foundryName, familyName);

    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

    createDatabase();

    QtFontFoundry allStyles(foundryName);
    QtFontFamily *f = d->family(familyName);
    if (!f) return -1;

    for (int j = 0; j < f->count; j++) {
        QtFontFoundry *foundry = f->foundries[j];
        if (foundryName.isEmpty() ||
            foundry->name.compare(foundryName, Qt::CaseInsensitive) == 0) {
            for (int k = 0; k < foundry->count; k++)
                allStyles.style(foundry->styles[k]->key, foundry->styles[k]->styleName, true);
        }
    }

    QtFontStyle::Key styleKey(style);
    QtFontStyle *s = allStyles.style(styleKey, style);
    return s ? s->key.weight : -1;
}


/*! \internal */
bool QFontDatabase::hasFamily(const QString &family) const
{
    QString parsedFamily, foundry;
    parseFontName(family, foundry, parsedFamily);
    const QString familyAlias = resolveFontFamilyAlias(parsedFamily);
    return families().contains(familyAlias, Qt::CaseInsensitive);
}

void QFontDatabase::parseFontName(const QString &name, QString &foundry, QString &family)
{
    QT_PREPEND_NAMESPACE(parseFontName)(name, foundry, family);
}

void QFontDatabase::createDatabase()
{ initializeFontDb(); }

// used from qfontengine_ft.cpp
Q_GUI_EXPORT QByteArray qt_fontdata_from_index(int index)
{
    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);
    return privateDb()->applicationFonts.value(index).data;
}

int QFontDatabasePrivate::addAppFont(const QByteArray &fontData, const QString &fileName)
{
    QFontDatabasePrivate::ApplicationFont font;
    font.data = fontData;
    font.fileName = fileName;

    int i;
    for (i = 0; i < applicationFonts.count(); ++i)
        if (applicationFonts.at(i).families.isEmpty())
            break;
    if (i >= applicationFonts.count()) {
        applicationFonts.append(ApplicationFont());
        i = applicationFonts.count() - 1;
    }

    if (font.fileName.isEmpty() && !fontData.isEmpty())
        font.fileName = QString::fromLatin1(":qmemoryfonts/") + QString::number(i);

    registerFont(&font);
    if (font.families.isEmpty())
        return -1;

    applicationFonts[i] = font;

    invalidate();
    return i;
}

bool QFontDatabasePrivate::isApplicationFont(const QString &fileName)
{
    for (int i = 0; i < applicationFonts.count(); ++i)
        if (applicationFonts.at(i).fileName == fileName)
            return true;
    return false;
}

/*!
    \since 4.2

    Loads the font from the file specified by \a fileName and makes it available to
    the application. An ID is returned that can be used to remove the font again
    with removeApplicationFont() or to retrieve the list of family names contained
    in the font.

    The function returns -1 if the font could not be loaded.

    Currently only TrueType fonts, TrueType font collections, and OpenType fonts are
    supported.

    \note Adding application fonts on Unix/X11 platforms without fontconfig is
    currently not supported.

    \note On Symbian, the font family names get truncated to a length of 20 characters.

    \sa addApplicationFontFromData(), applicationFontFamilies(), removeApplicationFont()
*/
int QFontDatabase::addApplicationFont(const QString &fileName)
{
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly))
        return -1;
    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);
    return privateDb()->addAppFont(f.readAll(), fileName);
}

/*!
    \since 4.2

    Loads the font from binary data specified by \a fontData and makes it available to
    the application. An ID is returned that can be used to remove the font again
    with removeApplicationFont() or to retrieve the list of family names contained
    in the font.

    The function returns -1 if the font could not be loaded.

    Currently only TrueType fonts and TrueType font collections are supported.

    \bold{Note:} Adding application fonts on Unix/X11 platforms without fontconfig is
    currently not supported.

    \note On Symbian, the font family names get truncated to a length of 20 characters.

    \sa addApplicationFont(), applicationFontFamilies(), removeApplicationFont()
*/
int QFontDatabase::addApplicationFontFromData(const QByteArray &fontData)
{
    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);
    return privateDb()->addAppFont(fontData, QString() /* fileName */);
}

/*!
    \since 4.2

    Returns a list of font families for the given application font identified by
    \a id.

    \sa addApplicationFont(), addApplicationFontFromData()
*/
QStringList QFontDatabase::applicationFontFamilies(int id)
{
    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);
    return privateDb()->applicationFonts.value(id).families;
}

/*!
    \fn bool QFontDatabase::removeApplicationFont(int id)
    \since 4.2

    Removes the previously loaded application font identified by \a
    id. Returns true if unloading of the font succeeded; otherwise
    returns false.

    \sa removeAllApplicationFonts(), addApplicationFont(),
        addApplicationFontFromData()
*/

/*!
    \fn bool QFontDatabase::removeAllApplicationFonts()
    \since 4.2

    Removes all application-local fonts previously added using addApplicationFont()
    and addApplicationFontFromData().

    Returns true if unloading of the fonts succeeded; otherwise
    returns false.

    \sa removeApplicationFont(), addApplicationFont(), addApplicationFontFromData()
*/

/*!
    \fn bool QFontDatabase::supportsThreadedFontRendering()
    \since 4.4

    Returns true if font rendering is supported outside the GUI
    thread, false otherwise. In other words, a return value of false
    means that all QPainter::drawText() calls outside the GUI thread
    will not produce readable output.

    \sa {Thread-Support in Qt Modules#Painting In Threads}{Painting In Threads}
*/


QT_END_NAMESPACE

#include "moc_qfontdatabase.h"

