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

#include "qcolor.h"
#include "qcolor_p.h"
#include "qnamespace.h"
#include "qdatastream.h"
#include "qvariant.h"
#include "qmath.h"
#include "qdebug.h"
#include "qcorecommon_p.h"

#include <limits.h>

QT_BEGIN_NAMESPACE

#define QCOLOR_INT_RANGE_CHECK(fn, var) \
    do { \
        if (Q_UNLIKELY(var < 0 || var > 255)) { \
            qWarning(#fn": invalid value %d", var); \
            var = qMax(0, qMin(var, 255)); \
        } \
    } while (0)

#define QCOLOR_REAL_RANGE_CHECK(fn, var) \
    do { \
        if (Q_UNLIKELY(var < qreal(0.0) || var > qreal(1.0))) { \
            qWarning(#fn": invalid value %g", var); \
            var = qMax(qreal(0.0), qMin(var, qreal(1.0)));      \
        } \
    } while (0)

static bool qAllowX11ColorNames = false;

/*
* From the "The Palette Manager: How and Why" by Ron Gery,
* March 23, 1992, archived on MSDN:
*
*     The Windows system palette is broken up into two
*     sections, one with fixed colors and one with colors
*     that can be changed by applications. The system palette
*     predefines 20 entries; these colors are known as the
*     static or reserved colors and consist of the 16 colors
*     found in the Windows version 3.0 VGA driver and 4
*     additional colors chosen for their visual appeal.  The
*     DEFAULT_PALETTE stock object is, as the name implies,
*     the default palette selected into a device context (DC)
*     and consists of these static colors. Applications can
*     set the remaining 236 colors using the Palette Manager.
*
* The 20 reserved entries have indices in [0,9] and
* [246,255]. We reuse 17 of them.
*/
static const struct globalColorsData {
    const int red;
    const int green;
    const int blue;
    const int alpha;
} globalColorsTbl[] = {
    { 255, 255, 255, 255 }, // Qt::color0
    {   0,   0,   0, 255 }, // Qt::color1
    {   0,   0,   0, 255 }, // black
    { 255, 255, 255, 255 }, // white
    { 128, 128, 128, 255 }, // index 248   medium gray
    { 160, 160, 164, 255 }, // index 247   light gray
    { 192, 192, 192, 255 }, // index 7     light gray
    { 255,   0,   0, 255 }, // index 249   red
    {   0, 255,   0, 255 }, // index 250   green
    {   0,   0, 255, 255 }, // index 252   blue
    {   0, 255, 255, 255 }, // index 254   cyan
    { 255,   0, 255, 255 }, // index 253   magenta
    { 255, 255,   0, 255 }, // index 251   yellow
    { 128,   0,   0, 255 }, // index 1     dark red
    {   0, 128,   0, 255 }, // index 2     dark green
    {   0,   0, 128, 255 }, // index 4     dark blue
    {   0, 128, 128, 255 }, // index 6     dark cyan
    { 128,   0, 128, 255 }, // index 5     dark magenta
    { 128, 128,   0, 255 }, // index 3     dark yellow
    {   0,   0,   0,   0 }, //             transparent
};

/*!
    \class QColor
    \brief The QColor class provides colors based on RGB or HSV values.

    \ingroup painting
    \ingroup appearance


    A color is normally specified in terms of RGB (red, green, and
    blue) components, but it is also possible to specify it in terms
    of HSV (hue, saturation and value) components. In addition a color
    can be specified using a color name. The color name can be any of
    the SVG 1.0 color names.

    The QColor constructor creates the color based on RGB values. To
    create a QColor based on HSV values, use the toHsv() function.
    That function return a copy of the color using the desired format.
    In addition the static fromRgb() and fromHsv() functions create
    colors from the specified values. Alternatively, a color can be
    converted to any of the three formats using the convertTo()
    function (returning a copy of the color in the desired format), or
    any of the setRgb() and setHsv() functions altering \e this color's
    format. The spec() function tells how the color was specified.

    A color can be set by passing an RGB string (such as "#112233"),
    or a color name (such as "blue"), to the setNamedColor() function.
    The color names are taken from the SVG 1.0 color names. The name()
    function returns the name of the color in the format
    "#RRGGBB". Colors can also be set using setRgb() and setHsv(). To
    get a lighter or darker color use the lighter() and darker()
    functions respectively.

    The isValid() function indicates whether a QColor is legal at
    all. For example, a RGB color with RGB values out of range is
    illegal. For performance reasons, QColor mostly disregards illegal
    colors, and for that reason, the result of using an invalid color
    is undefined.

    The color components can be retrieved individually, e.g with
    red(), hue() and cyan(). The values of the color components can
    also be retrieved in one go using the getRgb() and getHsv()
    functions. Using the RGB color model, the color components can
    in addition be accessed with rgb().

    There are several related non-members: QRgb is a typdef for an
    unsigned int representing the RGB value triplet (r, g, b). Note
    that it also can hold a value for the alpha-channel (for more
    information, see the \l {QColor#Alpha-Blended
    Drawing}{Alpha-Blended Drawing} section). The qRed(), qBlue() and
    qGreen() functions return the respective component of the given
    QRgb value, while the qRgb() and qRgba() functions create and
    return the QRgb triplet based on the given component
    values. Finally, the qAlpha() function returns the alpha component
    of the provided QRgb, and the qGray() function calculates and
    return a gray value based on the given value.

    QColor is platform and device independent. The QColormap class
    maps the color to the hardware.

    For more information about painting in general, see the \l{Paint
    System} documentation.

    \tableofcontents

    \section1 Integer vs. Floating Point Precision

    QColor supports floating point precision and provides floating
    point versions of all the color components functions,
    e.g. getRgbF() and hueF(). Note that since the components are
    stored using 16-bit integers, there might be minor deviations
    between the values set using, for example, setRgbF() and the
    values returned by the getRgbF() function due to rounding.

    While the integer based functions take values in the range 0-255
    (except hue() which must have values within the range 0-359),
    the floating point functions accept values in the range 0.0 - 1.0.

    \section1 Alpha-Blended Drawing

    QColor also support alpha-blended outlining and filling. The
    alpha channel of a color specifies the transparency effect, 0
    represents a fully transparent color, while 255 represents a fully
    opaque color. For example:

    \snippet doc/src/snippets/code/src_gui_painting_qcolor.cpp 0

    The code above produces the following output:

    \img alphafill.png

    Alpha-blended drawing is supported on Windows, Mac OS X, and on
    X11 systems that have the X Render extension installed.

    The alpha channel of a color can be retrieved and set using the
    alpha() and setAlpha() functions if its value is an integer, and
    alphaF() and setAlphaF() if its value is qreal (double). By
    default, the alpha-channel is set to 255 (opaque). To retrieve and
    set \e all the RGB color components (including the alpha-channel)
    in one go, use the rgba() and setRgba() functions.

    \section1 Predefined Colors

    There are 20 predefined QColors described by the Qt::GlobalColor enum,
    including black, white, primary and secondary colors, darker versions
    of these colors and three shades of gray. QColor also recognizes a
    variety of color names; the static colorNames() function returns a
    QStringList color names that QColor knows about.

    \img qt-colors.png Qt Colors

    Additionally, the Qt::color0, Qt::color1 and Qt::transparent colors
    are used for special purposes.

    Qt::color0 (zero pixel value) and Qt::color1 (non-zero pixel value)
    are special colors for drawing in QBitmaps. Painting with Qt::color0
    sets the bitmap bits to 0 (transparent; i.e., background), and painting
    with Qt::color1 sets the bits to 1 (opaque; i.e., foreground).

    Qt::transparent is used to indicate a transparent pixel. When painting
    with this value, a pixel value will be used that is appropriate for the
    underlying pixel format in use.

    \section1 The HSV Color Model

    The RGB model is hardware-oriented. Its representation is close to
    what most monitors show. In contrast, HSV represents color in a way
    more suited to the human perception of color. For example, the
    relationships "stronger than", "darker than", and "the opposite of"
    are easily expressed in HSV but are much harder to express in RGB.

    HSV, like RGB, has three components:

    \list
    \o H, for hue, is in the range 0 to 359 if the color is chromatic (not
    gray), or meaningless if it is gray. It represents degrees on the
    color wheel familiar to most people. Red is 0 (degrees), green is
    120, and blue is 240.

    \inlineimage qcolor-hue.png

    \o S, for saturation, is in the range 0 to 255, and the bigger it is,
    the stronger the color is. Grayish colors have saturation near 0; very
    strong colors have saturation near 255.

    \inlineimage qcolor-saturation.png

    \o V, for value, is in the range 0 to 255 and represents lightness or
    brightness of the color. 0 is black; 255 is as far from black as
    possible.

    \inlineimage qcolor-value.png
    \endlist

    Here are some examples: pure red is H=0, S=255, V=255; a dark red,
    moving slightly towards the magenta, could be H=350 (equivalent to
    -10), S=255, V=180; a grayish light red could have H about 0 (say
    350-359 or 0-10), S about 50-100, and S=255.

    Qt returns a hue value of -1 for achromatic colors. If you pass a
    hue value that is too large, Qt forces it into range. Hue 360 or 720 is
    treated as 0; hue 540 is treated as 180.

    In addition to the standard HSV model, Qt provides an
    alpha-channel to feature \l {QColor#Alpha-Blended
    Drawing}{alpha-blended drawing}.

    \sa QPalette, QBrush, QApplication::setColorSpec()
*/

/*!
    \enum QColor::Spec

    The type of color specified, either RGB or HSV.

    \value Rgb
    \value Hsv
    \value Invalid

    \sa spec(), toRgb(), toHsv()
*/

/*!
    \fn Spec QColor::spec() const

    Returns how the color was specified.

    \sa Spec
*/


/*!
    \fn QColor::QColor()

    Constructs an invalid color with the RGB value (0, 0, 0). An
    invalid color is a color that is not properly set up for the
    underlying window system.

    The alpha value of an invalid color is unspecified.

    \sa isValid()
*/

/*!
    \overload

    Constructs a new color with a color value of \a color.

    \sa isValid(), {QColor#Predefined Colors}{Predefined Colors}
 */
QColor::QColor(Qt::GlobalColor color)
{
    setRgb(globalColorsTbl[color].red,
           globalColorsTbl[color].green,
           globalColorsTbl[color].blue,
           globalColorsTbl[color].alpha);
}

/*!
    \fn QColor::QColor(int r, int g, int b, int a = 255)

    Constructs a color with the RGB value \a r, \a g, \a b, and the
    alpha-channel (transparency) value of \a a.

    The color is left invalid if any of the arguments are invalid.

    \sa setRgba(), isValid()
*/

/*!
    Constructs a color with the value \a color. The alpha component is
    ignored and set to solid.

    \sa fromRgb(), isValid()
*/

QColor::QColor(QRgb color)
{
    cspec = QColor::Rgb;
    ct.argb.alpha = 0xffff;
    ct.argb.red   = qRed(color)   * 0x101;
    ct.argb.green = qGreen(color) * 0x101;
    ct.argb.blue  = qBlue(color)  * 0x101;
}

/*!
    \internal

    Constructs a color with the given \a spec.

    This function is primarly present to avoid that QColor::Invalid
    becomes a valid color by accident.
*/

QColor::QColor(Spec spec)
{
    switch (spec) {
        case QColor::Invalid: {
            invalidate();
            break;
        }
        case QColor::Rgb: {
            setRgb(0, 0, 0);
            break;
        }
        case QColor::Hsv: {
            setHsv(0, 0, 0);
            break;
        }
    }
}

/*!
    \fn QColor::QColor(const QString &name)

    Constructs a named color in the same way as setNamedColor() using
    the given \a name.

    The color is left invalid if the \a name cannot be parsed.

    \sa setNamedColor(), name(), isValid()
*/

/*!
    \fn QColor::QColor(const char *name)

    Constructs a named color in the same way as setNamedColor() using
    the given \a name.

    The color is left invalid if the \a name cannot be parsed.

    \sa setNamedColor(), name(), isValid()
*/

/*!
    \fn QColor::QColor(const QColor &color)

    Constructs a color that is a copy of \a color.

    \sa isValid()
*/

/*!
    \fn bool QColor::isValid() const

    Returns true if the color is valid; otherwise returns false.
*/

/*!
    Returns the name of the color in the format "#RRGGBB"; i.e. a "#"
    character followed by three two-digit hexadecimal numbers.

    \sa setNamedColor()
*/

QString QColor::name() const
{
    QSTACKARRAY(char, snprintfbuf, 8);
    ::snprintf(snprintfbuf, sizeof(snprintfbuf), "#%02x%02x%02x", red(), green(), blue());
    return QString::fromLatin1(snprintfbuf, sizeof(snprintfbuf) - 1);
}

/*!
    Sets the RGB value of this QColor to \a name, which may be in one
    of these formats:

    \list
    \i #RGB (each of R, G, and B is a single hex digit)
    \i #RRGGBB
    \i #RRRGGGBBB
    \i #RRRRGGGGBBBB
    \i A name from the list of colors defined in the list of \l{SVG color keyword names}
       provided by the World Wide Web Consortium; for example, "steelblue" or "gainsboro".
       These color names work on all platforms. Note that these color names are \e not the
       same as defined by the Qt::GlobalColor enums, e.g. "green" and Qt::green does not
       refer to the same color.
    \i \c transparent - representing the absence of a color.
    \i \e{X11 only}: If allowX11ColorNames() returns true, any valid X11 color name. See
       the documentation for \c XParseColor() for information about valid X11 color names.
    \endlist

    The color is invalid if \a name cannot be parsed.

    \sa QColor(), name(), isValid(), allowX11ColorNames()
*/

bool QColor::setNamedColor(const QString &name)
{
    if (name.isEmpty()) {
        invalidate();
        return false;
    }

    QByteArray latin = name.toLatin1();
    int spaceindex = latin.indexOf(' ');
    while (spaceindex >= 0) {
        latin.remove(spaceindex, 1);
        spaceindex = latin.indexOf(' ');
    }

    if (latin.startsWith('#')) {
        QRgb rgb;
        if (qt_get_hex_rgb(latin.constData(), latin.length(), &rgb)) {
            setRgb(rgb);
            return true;
        }
        invalidate();
        return false;
    }

#ifndef QT_NO_COLORNAMES
    for (qint16 i = 0; i < RGBTblSize; i++) {
        if (qstricmp(RGBTbl[i].name, latin.constData()) == 0) {
            setRgba(RGBTbl[i].value);
            return true;
        }
    }
#endif

#ifdef Q_WS_X11
    if (qAllowX11ColorNames) {
        for (qint16 i = 0; i < X11RGBTblSize; i++) {
            if (qstricmp(X11RGBTbl[i].name, latin.constData()) == 0) {
                setRgba(X11RGBTbl[i].value);
                return true;
            }
        }
    }
#endif

    invalidate();
    return false;
}

/*!
   \since 4.7

   Returns true if the \a name is a valid color name and can
   be used to construct a valid QColor object, otherwise returns
   false.

   It uses the same algorithm used in setNamedColor().

   \sa setNamedColor()
*/
bool QColor::isValidColor(const QString &name)
{
    if (name.isEmpty()) {
        return false;
    }

    QByteArray latin = name.toLatin1();
    int spaceindex = latin.indexOf(' ');
    while (spaceindex >= 0) {
        latin.remove(spaceindex, 1);
        spaceindex = latin.indexOf(' ');
    }

    if (latin.startsWith('#')) {
        QRgb rgb;
        if (qt_get_hex_rgb(latin.constData(), latin.length(), &rgb)) {
            return true;
        }
        return false;
    }

#ifndef QT_NO_COLORNAMES
    for (qint16 i = 0; i < RGBTblSize; i++) {
        if (qstricmp(RGBTbl[i].name, latin.constData()) == 0) {
            return true;
        }
    }
#endif

#ifdef Q_WS_X11
    if (qAllowX11ColorNames) {
        for (qint16 i = 0; i < X11RGBTblSize; i++) {
            if (qstricmp(X11RGBTbl[i].name, latin.constData()) == 0) {
                return true;
            }
        }
    }
#endif

    return false;
}

/*!
    Returns a QStringList containing the color names Qt knows about.

    \sa {QColor#Predefined Colors}{Predefined Colors}
*/
QStringList QColor::colorNames()
{
#ifndef QT_NO_COLORNAMES
    QStringList result;
    for (qint16 i = 0; i < RGBTblSize; i++) {
        result << QString::fromLatin1(RGBTbl[i].name);
    }
#ifdef Q_WS_X11
    if (qAllowX11ColorNames) {
        for (qint16 i = 0; i < X11RGBTblSize; i++) {
            const QString x11color = QString::fromLatin1(X11RGBTbl[i].name);
            if (!result.contains(x11color)) {
                result << x11color;
            }
        }
    }
#endif
    return result;
#else
    return QStringList();
#endif
}

/*!
    Sets the contents pointed to by \a h, \a s, \a v, and \a a, to the hue,
    saturation, value, and alpha-channel (transparency) components of the
    color's HSV value.

    These components can be retrieved individually using the hueF(),
    saturationF(), valueF() and alphaF() functions.

    \sa setHsv() {QColor#The HSV Color Model}{The HSV Color Model}
*/
void QColor::getHsvF(qreal *h, qreal *s, qreal *v, qreal *a) const
{
    if (!h || !s || !v)
        return;

    if (cspec != QColor::Invalid && cspec != QColor::Hsv) {
        toHsv().getHsvF(h, s, v, a);
        return;
    }

    *h = ct.ahsv.hue == USHRT_MAX ? qreal(-1.0) : ct.ahsv.hue / qreal(36000.0);
    *s = ct.ahsv.saturation / qreal(USHRT_MAX);
    *v = ct.ahsv.value / qreal(USHRT_MAX);

    if (a)
        *a = ct.ahsv.alpha / qreal(USHRT_MAX);
}

/*!
    Sets the contents pointed to by \a h, \a s, \a v, and \a a, to the hue,
    saturation, value, and alpha-channel (transparency) components of the
    color's HSV value.

    These components can be retrieved individually using the hue(),
    saturation(), value() and alpha() functions.

    \sa setHsv(), {QColor#The HSV Color Model}{The HSV Color Model}
*/
void QColor::getHsv(int *h, int *s, int *v, int *a) const
{
    if (!h || !s || !v)
        return;

    if (cspec != QColor::Invalid && cspec != QColor::Hsv) {
        toHsv().getHsv(h, s, v, a);
        return;
    }

    *h = ct.ahsv.hue == USHRT_MAX ? -1 : ct.ahsv.hue / 100;
    *s = ct.ahsv.saturation >> 8;
    *v = ct.ahsv.value      >> 8;

    if (a)
        *a = ct.ahsv.alpha >> 8;
}

/*!
    Sets a HSV color value; \a h is the hue, \a s is the saturation, \a v is
    the value and \a a is the alpha component of the HSV color.

    All the values must be in the range 0.0-1.0.

    \sa getHsvF(), setHsv(), {QColor#The HSV Color Model}{The HSV
    Color Model}
*/
void QColor::setHsvF(qreal h, qreal s, qreal v, qreal a)
{
    if (Q_UNLIKELY(((h < qreal(0.0) || h > qreal(1.0)) && h != qreal(-1.0))
        || (s < qreal(0.0) || s > qreal(1.0))
        || (v < qreal(0.0) || v > qreal(1.0))
        || (a < qreal(0.0) || a > qreal(1.0)))) {
        qWarning("QColor::setHsvF: HSV parameters out of range");
        invalidate();
        return;
    }

    cspec = QColor::Hsv;
    ct.ahsv.alpha      = qRound(a * USHRT_MAX);
    ct.ahsv.hue        = h == qreal(-1.0) ? USHRT_MAX : qRound(h * 36000);
    ct.ahsv.saturation = qRound(s * USHRT_MAX);
    ct.ahsv.value      = qRound(v * USHRT_MAX);
}

/*!
    Sets a HSV color value; \a h is the hue, \a s is the saturation, \a v is
    the value and \a a is the alpha component of the HSV color.

    The saturation, value and alpha-channel values must be in the range 0-255,
    and the hue value must be greater than -1.

    \sa getHsv(), setHsvF(), {QColor#The HSV Color Model}{The HSV
    Color Model}
*/
void QColor::setHsv(int h, int s, int v, int a)
{
    if (Q_UNLIKELY(h < -1 || (uint)s > 255 || (uint)v > 255 || (uint)a > 255)) {
        qWarning("QColor::setHsv: HSV parameters out of range");
        invalidate();
        return;
    }

    cspec = QColor::Hsv;
    ct.ahsv.alpha      = a * 0x101;
    ct.ahsv.hue        = h == -1 ? USHRT_MAX : (h % 360) * 100;
    ct.ahsv.saturation = s * 0x101;
    ct.ahsv.value      = v * 0x101;
}

/*!
    Sets the contents pointed to by \a r, \a g, \a b, and \a a, to the red,
    green, blue, and alpha-channel (transparency) components of the color's
    RGB value.

    These components can be retrieved individually using the redF(), greenF(),
    blueF() and alphaF() functions.

    \sa rgb(), setRgb()
*/
void QColor::getRgbF(qreal *r, qreal *g, qreal *b, qreal *a) const
{
    if (!r || !g || !b)
        return;

    if (cspec != QColor::Invalid && cspec != QColor::Rgb) {
        toRgb().getRgbF(r, g, b, a);
        return;
    }

    *r = ct.argb.red   / qreal(USHRT_MAX);
    *g = ct.argb.green / qreal(USHRT_MAX);
    *b = ct.argb.blue  / qreal(USHRT_MAX);

    if (a)
        *a = ct.argb.alpha / qreal(USHRT_MAX);

}

/*!
    Sets the contents pointed to by \a r, \a g, \a b, and \a a, to the red,
    green, blue, and alpha-channel (transparency) components of the color's
    RGB value.

    These components can be retrieved individually using the red(), green(),
    blue() and alpha() functions.

    \sa rgb(), setRgb()
*/
void QColor::getRgb(int *r, int *g, int *b, int *a) const
{
    if (!r || !g || !b)
        return;

    if (cspec != QColor::Invalid && cspec != QColor::Rgb) {
        toRgb().getRgb(r, g, b, a);
        return;
    }

    *r = ct.argb.red   >> 8;
    *g = ct.argb.green >> 8;
    *b = ct.argb.blue  >> 8;

    if (a)
        *a = ct.argb.alpha >> 8;
}

/*!
    \obsolete
    \fn void QColor::getRgba(int *r, int *g, int *b, int *a) const

    Use getRgb() instead.
*/

/*!
    \fn void QColor::setRgbF(qreal r, qreal g, qreal b, qreal a)

    Sets the color channels of this color to \a r (red), \a g (green),
    \a b (blue) and \a a (alpha, transparency).

    All values must be in the range 0.0-1.0.

    \sa rgb(), getRgbF(), setRgb()
*/
void QColor::setRgbF(qreal r, qreal g, qreal b, qreal a)
{
    if (Q_UNLIKELY(r < qreal(0.0) || r > qreal(1.0)
        || g < qreal(0.0) || g > qreal(1.0)
        || b < qreal(0.0) || b > qreal(1.0)
        || a < qreal(0.0) || a > qreal(1.0))) {
        qWarning("QColor::setRgbF: RGB parameters out of range");
        invalidate();
        return;
    }

    cspec = QColor::Rgb;
    ct.argb.alpha = qRound(a * USHRT_MAX);
    ct.argb.red   = qRound(r * USHRT_MAX);
    ct.argb.green = qRound(g * USHRT_MAX);
    ct.argb.blue  = qRound(b * USHRT_MAX);
}

/*!
    Sets the RGB value to \a r, \a g, \a b and the alpha value to \a a.

    All the values must be in the range 0-255.

    \sa rgb(), getRgb(), setRgbF()
*/
void QColor::setRgb(int r, int g, int b, int a)
{
    if (Q_UNLIKELY((uint)r > 255 || (uint)g > 255 || (uint)b > 255 || (uint)a > 255)) {
        qWarning("QColor::setRgb: RGB parameters out of range");
        invalidate();
        return;
    }

    cspec = QColor::Rgb;
    ct.argb.alpha = a * 0x101;
    ct.argb.red   = r * 0x101;
    ct.argb.green = g * 0x101;
    ct.argb.blue  = b * 0x101;
}

/*!
    \obsolete
    \fn void QColor::setRgba(int r, int g, int b, int a)

    Use setRgb() instead.
*/

/*!
    \fn QRgb QColor::rgba() const

    Returns the RGB value of the color, including its alpha.

    For an invalid color, the alpha value of the returned color is unspecified.

    \sa setRgba(), rgb()
*/

QRgb QColor::rgba() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Rgb)
        return toRgb().rgba();
    return qRgba(ct.argb.red >> 8, ct.argb.green >> 8, ct.argb.blue >> 8, ct.argb.alpha >> 8);
}

/*!
    Sets the RGB value to \a rgba, including its alpha.

    \sa rgba(), rgb()
*/
void QColor::setRgba(QRgb rgba)
{
    cspec = QColor::Rgb;
    ct.argb.alpha = qAlpha(rgba) * 0x101;
    ct.argb.red   = qRed(rgba)   * 0x101;
    ct.argb.green = qGreen(rgba) * 0x101;
    ct.argb.blue  = qBlue(rgba)  * 0x101;
}

/*!
    \fn QRgb QColor::rgb() const

    Returns the RGB value of the color. The alpha value is opaque.

    \sa getRgb(), rgba()
*/
QRgb QColor::rgb() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Rgb)
        return toRgb().rgb();
    return qRgb(ct.argb.red >> 8, ct.argb.green >> 8, ct.argb.blue >> 8);
}

/*!
    \overload

    Sets the RGB value to \a rgb. The alpha value is set to opaque.
*/
void QColor::setRgb(QRgb rgb)
{
    cspec = QColor::Rgb;
    ct.argb.alpha = 0xffff;
    ct.argb.red   = qRed(rgb)   * 0x101;
    ct.argb.green = qGreen(rgb) * 0x101;
    ct.argb.blue  = qBlue(rgb)  * 0x101;
}

/*!
    Returns the alpha color component of this color.

    \sa setAlpha(), alphaF(), {QColor#Alpha-Blended
    Drawing}{Alpha-Blended Drawing}
*/
int QColor::alpha() const
{ return ct.argb.alpha >> 8; }


/*!
    Sets the alpha of this color to \a alpha. Integer alpha is specified in the
    range 0-255.

    \sa alpha(), alphaF(), {QColor#Alpha-Blended
    Drawing}{Alpha-Blended Drawing}
*/

void QColor::setAlpha(int alpha)
{
    QCOLOR_INT_RANGE_CHECK("QColor::setAlpha", alpha);
    ct.argb.alpha = alpha * 0x101;
}

/*!
    Returns the alpha color component of this color.

    \sa setAlphaF(), alpha(),  {QColor#Alpha-Blended
    Drawing}{Alpha-Blended Drawing}
*/
qreal QColor::alphaF() const
{
    return ct.argb.alpha / qreal(USHRT_MAX);
}

/*!
    Sets the alpha of this color to \a alpha. qreal alpha is specified in the
    range 0.0-1.0.

    \sa alphaF(), alpha(), {QColor#Alpha-Blended
    Drawing}{Alpha-Blended Drawing}

*/
void QColor::setAlphaF(qreal alpha)
{
    QCOLOR_REAL_RANGE_CHECK("QColor::setAlphaF", alpha);
    ct.argb.alpha = qRound(alpha * USHRT_MAX);
}


/*!
    Returns the red color component of this color.

    \sa setRed(), redF(), getRgb()
*/
int QColor::red() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Rgb)
        return toRgb().red();
    return ct.argb.red >> 8;
}

/*!
    Sets the red color component of this color to \a red. Integer components
    are specified in the range 0-255.

    \sa red(), redF(), setRgb()
*/
void QColor::setRed(int red)
{
    QCOLOR_INT_RANGE_CHECK("QColor::setRed", red);
    if (cspec != QColor::Rgb)
        setRgb(red, green(), blue(), alpha());
    else
        ct.argb.red = red * 0x101;
}

/*!
    Returns the green color component of this color.

    \sa setGreen(), greenF(), getRgb()
*/
int QColor::green() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Rgb)
        return toRgb().green();
    return ct.argb.green >> 8;
}

/*!
    Sets the green color component of this color to \a green. Integer
    components are specified in the range 0-255.

    \sa green(), greenF(),  setRgb()
*/
void QColor::setGreen(int green)
{
    QCOLOR_INT_RANGE_CHECK("QColor::setGreen", green);
    if (cspec != QColor::Rgb)
        setRgb(red(), green, blue(), alpha());
    else
        ct.argb.green = green * 0x101;
}


/*!
    Returns the blue color component of this color.

    \sa setBlue(), blueF(), getRgb()
*/
int QColor::blue() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Rgb)
        return toRgb().blue();
    return ct.argb.blue >> 8;
}


/*!
    Sets the blue color component of this color to \a blue. Integer components
    are specified in the range 0-255.

    \sa blue(), blueF(), setRgb()
*/
void QColor::setBlue(int blue)
{
    QCOLOR_INT_RANGE_CHECK("QColor::setBlue", blue);
    if (cspec != QColor::Rgb)
        setRgb(red(), green(), blue, alpha());
    else
        ct.argb.blue = blue * 0x101;
}

/*!
    Returns the red color component of this color.

    \sa setRedF(), red(), getRgbF()
*/
qreal QColor::redF() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Rgb)
        return toRgb().redF();
    return ct.argb.red / qreal(USHRT_MAX);
}


/*!
    Sets the red color component of this color to \a red. Float components
    are specified in the range 0.0-1.0.

    \sa redF(), red(), setRgbF()
*/
void QColor::setRedF(qreal red)
{
    QCOLOR_REAL_RANGE_CHECK("QColor::setRedF", red);
    if (cspec != QColor::Rgb)
        setRgbF(red, greenF(), blueF(), alphaF());
    else
        ct.argb.red = qRound(red * USHRT_MAX);
}

/*!
    Returns the green color component of this color.

    \sa setGreenF(), green(), getRgbF()
*/
qreal QColor::greenF() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Rgb)
        return toRgb().greenF();
    return ct.argb.green / qreal(USHRT_MAX);
}


/*!
    Sets the green color component of this color to \a green. Float components
    are specified in the range 0.0-1.0.

    \sa greenF(), green(), setRgbF()
*/
void QColor::setGreenF(qreal green)
{
    QCOLOR_REAL_RANGE_CHECK("QColor::setGreenF", green);
    if (cspec != QColor::Rgb)
        setRgbF(redF(), green, blueF(), alphaF());
    else
        ct.argb.green = qRound(green * USHRT_MAX);
}

/*!
    Returns the blue color component of this color.

     \sa setBlueF(), blue(), getRgbF()
*/
qreal QColor::blueF() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Rgb)
        return toRgb().blueF();
    return ct.argb.blue / qreal(USHRT_MAX);
}

/*!
    Sets the blue color component of this color to \a blue. Float components
    are specified in the range 0.0-1.0.

    \sa blueF(), blue(), setRgbF()
*/
void QColor::setBlueF(qreal blue)
{
    QCOLOR_REAL_RANGE_CHECK("QColor::setBlueF", blue);
    if (cspec != QColor::Rgb)
        setRgbF(redF(), greenF(), blue, alphaF());
    else
        ct.argb.blue = qRound(blue * USHRT_MAX);
}

/*!
    Returns the hue color component of this color.

    The color is implicitly converted to HSV.

    \sa hsvHue(), hueF(), getHsv(), {QColor#The HSV Color Model}{The HSV Color
    Model}
*/

int QColor::hue() const
{
    return hsvHue();
}

/*!
    Returns the hue color component of this color.

    \sa hueF(), getHsv(), {QColor#The HSV Color Model}{The HSV Color
    Model}
*/
int QColor::hsvHue() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Hsv)
        return toHsv().hue();
    return ct.ahsv.hue == USHRT_MAX ? -1 : ct.ahsv.hue / 100;
}

/*!
    Returns the saturation color component of this color.

    The color is implicitly converted to HSV.

    \sa hsvSaturation(), saturationF(), getHsv(), {QColor#The HSV Color Model}{The HSV Color
    Model}
*/

int QColor::saturation() const
{
    return hsvSaturation();
}

/*!
    Returns the saturation color component of this color.

    \sa saturationF(), getHsv(), {QColor#The HSV Color Model}{The HSV Color
    Model}
*/
int QColor::hsvSaturation() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Hsv)
        return toHsv().saturation();
    return ct.ahsv.saturation >> 8;
}

/*!
    Returns the value color component of this color.

    \sa valueF(), getHsv(), {QColor#The HSV Color Model}{The HSV Color
    Model}
*/
int QColor::value() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Hsv)
        return toHsv().value();
    return ct.ahsv.value >> 8;
}

/*!
    Returns the hue color component of this color.

    The color is implicitly converted to HSV.

    \sa hsvHueF(), hue(), getHsvF(), {QColor#The HSV Color Model}{The HSV Color
    Model}
*/
qreal QColor::hueF() const
{
    return hsvHueF();
}

/*!
    Returns the hue color component of this color.

    \sa hue(), getHsvF(), {QColor#The HSV Color Model}{The HSV Color
    Model}
*/
qreal QColor::hsvHueF() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Hsv)
        return toHsv().hueF();
    return ct.ahsv.hue == USHRT_MAX ? qreal(-1.0) : ct.ahsv.hue / qreal(36000.0);
}

/*!
    Returns the saturation color component of this color.

     The color is implicitly converted to HSV.

    \sa hsvSaturationF(), saturation() getHsvF(), {QColor#The HSV Color Model}{The HSV Color
    Model}
*/
qreal QColor::saturationF() const
{
    return hsvSaturationF();
}

/*!
    Returns the saturation color component of this color.

    \sa saturation() getHsvF(), {QColor#The HSV Color Model}{The HSV Color
    Model}
*/
qreal QColor::hsvSaturationF() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Hsv)
        return toHsv().saturationF();
    return ct.ahsv.saturation / qreal(USHRT_MAX);
}

/*!
    Returns the value color component of this color.

    \sa value() getHsvF(), {QColor#The HSV Color Model}{The HSV Color
    Model}
*/
qreal QColor::valueF() const
{
    if (cspec != QColor::Invalid && cspec != QColor::Hsv)
        return toHsv().valueF();
    return ct.ahsv.value / qreal(USHRT_MAX);
}

/*!
    Create and returns an RGB QColor based on this color.

    \sa fromRgb(), toHsv(), isValid()
*/
QColor QColor::toRgb() const
{
    if (!isValid() || cspec == QColor::Rgb) {
        return *this;
    }

    QColor color;
    color.cspec = QColor::Rgb;
    color.ct.argb.alpha = ct.argb.alpha;

    switch (cspec) {
    case QColor::Hsv: {
        if (ct.ahsv.saturation == 0 || ct.ahsv.hue == USHRT_MAX) {
            // achromatic case
            color.ct.argb.red = color.ct.argb.green = color.ct.argb.blue = ct.ahsv.value;
            break;
        }

        // chromatic case
        const qreal h = ct.ahsv.hue == 36000 ? 0 : ct.ahsv.hue / qreal(6000.);
        const qreal s = ct.ahsv.saturation / qreal(USHRT_MAX);
        const qreal v = ct.ahsv.value / qreal(USHRT_MAX);
        const int i = int(h);
        const qreal f = h - i;
        const qreal p = v * (qreal(1.0) - s);

        if (i & 1) {
            const qreal q = v * (qreal(1.0) - (s * f));

            switch (i) {
            case 1:
                color.ct.argb.red   = qRound(q * USHRT_MAX);
                color.ct.argb.green = qRound(v * USHRT_MAX);
                color.ct.argb.blue  = qRound(p * USHRT_MAX);
                break;
            case 3:
                color.ct.argb.red   = qRound(p * USHRT_MAX);
                color.ct.argb.green = qRound(q * USHRT_MAX);
                color.ct.argb.blue  = qRound(v * USHRT_MAX);
                break;
            case 5:
                color.ct.argb.red   = qRound(v * USHRT_MAX);
                color.ct.argb.green = qRound(p * USHRT_MAX);
                color.ct.argb.blue  = qRound(q * USHRT_MAX);
                break;
            }
        } else {
            const qreal t = v * (qreal(1.0) - (s * (qreal(1.0) - f)));

            switch (i) {
            case 0:
                color.ct.argb.red   = qRound(v * USHRT_MAX);
                color.ct.argb.green = qRound(t * USHRT_MAX);
                color.ct.argb.blue  = qRound(p * USHRT_MAX);
                break;
            case 2:
                color.ct.argb.red   = qRound(p * USHRT_MAX);
                color.ct.argb.green = qRound(v * USHRT_MAX);
                color.ct.argb.blue  = qRound(t * USHRT_MAX);
                break;
            case 4:
                color.ct.argb.red   = qRound(t * USHRT_MAX);
                color.ct.argb.green = qRound(p * USHRT_MAX);
                color.ct.argb.blue  = qRound(v * USHRT_MAX);
                break;
            }
        }
        break;
    }
    default:
        break;
    }

    return color;
}


#define Q_MAX_3(a, b, c) ( ( a > b && a > c) ? a : (b > c ? b : c) )
#define Q_MIN_3(a, b, c) ( ( a < b && a < c) ? a : (b < c ? b : c) )


/*!
    Creates and returns an HSV QColor based on this color.

    \sa fromHsv(), toRgb(), isValid(), {QColor#The HSV Color
    Model}{The HSV Color Model}
*/
QColor QColor::toHsv() const
{
    if (!isValid() || cspec == QColor::Hsv)
        return *this;

    if (cspec != QColor::Rgb)
        return toRgb().toHsv();

    QColor color;
    color.cspec = QColor::Hsv;
    color.ct.ahsv.alpha = ct.argb.alpha;

    const qreal r = ct.argb.red   / qreal(USHRT_MAX);
    const qreal g = ct.argb.green / qreal(USHRT_MAX);
    const qreal b = ct.argb.blue  / qreal(USHRT_MAX);
    const qreal max = Q_MAX_3(r, g, b);
    const qreal min = Q_MIN_3(r, g, b);
    const qreal delta = max - min;
    color.ct.ahsv.value = qRound(max * USHRT_MAX);
    if (qFuzzyIsNull(delta)) {
        // achromatic case, hue is undefined
        color.ct.ahsv.hue = USHRT_MAX;
        color.ct.ahsv.saturation = 0;
    } else {
        // chromatic case
        qreal hue = 0;
        color.ct.ahsv.saturation = qRound((delta / max) * USHRT_MAX);
        if (qFuzzyCompare(r, max)) {
            hue = ((g - b) /delta);
        } else if (qFuzzyCompare(g, max)) {
            hue = (qreal(2.0) + (b - r) / delta);
        } else if (qFuzzyCompare(b, max)) {
            hue = (qreal(4.0) + (r - g) / delta);
        } else {
            Q_ASSERT_X(false, "QColor::toHsv", "internal error");
        }
        hue *= qreal(60.0);
        if (hue < qreal(0.0))
            hue += qreal(360.0);
        color.ct.ahsv.hue = qRound(hue * 100);
    }

    return color;
}

/*!
    Static convenience function that returns a QColor constructed from the
    given QRgb value \a rgb.

    The alpha component of \a rgb is ignored (i.e. it is automatically set to
    255), use the fromRgba() function to include the alpha-channel specified by
    the given QRgb value.

    \sa fromRgba(), fromRgbF(), toRgb(), isValid()
*/

QColor QColor::fromRgb(QRgb rgb)
{
    return fromRgb(qRed(rgb), qGreen(rgb), qBlue(rgb));
}


/*!
    Static convenience function that returns a QColor constructed from the
    given QRgb value \a rgba.

    Unlike the fromRgb() function, the alpha-channel specified by the given
    QRgb value is included.

    \sa fromRgb(), isValid()
*/

QColor QColor::fromRgba(QRgb rgba)
{
    return fromRgb(qRed(rgba), qGreen(rgba), qBlue(rgba), qAlpha(rgba));
}

/*!
    Static convenience function that returns a QColor constructed from the RGB
    color values, \a r (red), \a g (green), \a b (blue), and \a a
    (alpha-channel, i.e. transparency).

    All the values must be in the range 0-255.

    \sa toRgb(), fromRgbF(), isValid()
*/
QColor QColor::fromRgb(int r, int g, int b, int a)
{
    if (Q_UNLIKELY(r < 0 || r > 255
        || g < 0 || g > 255
        || b < 0 || b > 255
        || a < 0 || a > 255)) {
        qWarning("QColor::fromRgb: RGB parameters out of range");
        return QColor();
    }

    QColor color;
    color.cspec = QColor::Rgb;
    color.ct.argb.alpha = a * 0x101;
    color.ct.argb.red   = r * 0x101;
    color.ct.argb.green = g * 0x101;
    color.ct.argb.blue  = b * 0x101;
    return color;
}

/*!
    Static convenience function that returns a QColor constructed from the RGB
    color values, \a r (red), \a g (green), \a b (blue), and \a a
    (alpha-channel, i.e. transparency).

    All the values must be in the range 0.0-1.0.

    \sa fromRgb(), toRgb(), isValid()
*/
QColor QColor::fromRgbF(qreal r, qreal g, qreal b, qreal a)
{
    if (Q_UNLIKELY(r < qreal(0.0) || r > qreal(1.0)
        || g < qreal(0.0) || g > qreal(1.0)
        || b < qreal(0.0) || b > qreal(1.0)
        || a < qreal(0.0) || a > qreal(1.0))) {
        qWarning("QColor::fromRgbF: RGB parameters out of range");
        return QColor();
    }

    QColor color;
    color.cspec = QColor::Rgb;
    color.ct.argb.alpha = qRound(a * USHRT_MAX);
    color.ct.argb.red   = qRound(r * USHRT_MAX);
    color.ct.argb.green = qRound(g * USHRT_MAX);
    color.ct.argb.blue  = qRound(b * USHRT_MAX);
    return color;
}

/*!
    Static convenience function that returns a QColor constructed from the HSV
    color values, \a h (hue), \a s (saturation), \a v (value), and \a a
    (alpha-channel, i.e. transparency).

    The value of \a s, \a v, and \a a must all be in the range 0-255; the value
    of \a h must be in the range 0-359.

    \sa toHsv(), fromHsvF(), isValid(), {QColor#The HSV Color
    Model}{The HSV Color Model}
*/
QColor QColor::fromHsv(int h, int s, int v, int a)
{
    if (Q_UNLIKELY(((h < 0 || h >= 360) && h != -1)
        || s < 0 || s > 255
        || v < 0 || v > 255
        || a < 0 || a > 255)) {
        qWarning("QColor::fromHsv: HSV parameters out of range");
        return QColor();
    }

    QColor color;
    color.cspec = QColor::Hsv;
    color.ct.ahsv.alpha      = a * 0x101;
    color.ct.ahsv.hue        = h == -1 ? USHRT_MAX : (h % 360) * 100;
    color.ct.ahsv.saturation = s * 0x101;
    color.ct.ahsv.value      = v * 0x101;
    return color;
}

/*!
    \overload

    Static convenience function that returns a QColor constructed from the HSV
    color values, \a h (hue), \a s (saturation), \a v (value), and \a a
    (alpha-channel, i.e. transparency).

    All the values must be in the range 0.0-1.0.

    \sa toHsv(), fromHsv(), isValid(), {QColor#The HSV Color
    Model}{The HSV Color Model}
*/
QColor QColor::fromHsvF(qreal h, qreal s, qreal v, qreal a)
{
    if (Q_UNLIKELY(((h < qreal(0.0) || h > qreal(1.0)) && h != qreal(-1.0))
        || (s < qreal(0.0) || s > qreal(1.0))
        || (v < qreal(0.0) || v > qreal(1.0))
        || (a < qreal(0.0) || a > qreal(1.0)))) {
        qWarning("QColor::fromHsvF: HSV parameters out of range");
        return QColor();
    }

    QColor color;
    color.cspec = QColor::Hsv;
    color.ct.ahsv.alpha      = qRound(a * USHRT_MAX);
    color.ct.ahsv.hue        = h == qreal(-1.0) ? USHRT_MAX : qRound(h * 36000);
    color.ct.ahsv.saturation = qRound(s * USHRT_MAX);
    color.ct.ahsv.value      = qRound(v * USHRT_MAX);
    return color;
}

/*!
    \fn QColor QColor::lighter(int factor) const
    \since 4.3

    Returns a lighter (or darker) color, but does not change this object.

    If the \a factor is greater than 100, this functions returns a lighter
    color. Setting \a factor to 150 returns a color that is 50% brighter. If
    the \a factor is less than 100, the return color is darker, but we
    recommend using the darker() function for this purpose. If the \a factor
    is 0 or negative, the return value is unspecified.

    The function converts the current RGB color to HSV, multiplies the value
    (V) component by \a factor and converts the color back to RGB.

    \sa darker(), isValid()
*/

/*!
    \obsolete

    Use lighter(\a factor) instead.
*/
QColor QColor::light(int factor) const
{
    if (cspec == QColor::Invalid) {
        return *this;
    } else if (factor <= 0) {
        // invalid lightness factor
        return *this;
    } else if (factor < 100) {
        // makes color darker
        return darker(10000 / factor);
    }

    QColor hsv = toHsv();
    int s = hsv.ct.ahsv.saturation;
    int v = hsv.ct.ahsv.value;

    v = (factor*v)/100;
    if (v > USHRT_MAX) {
        // overflow... adjust saturation
        s -= v - USHRT_MAX;
        if (s < 0)
            s = 0;
        v = USHRT_MAX;
    }

    hsv.ct.ahsv.saturation = s;
    hsv.ct.ahsv.value = v;

    // convert back to same color spec as original color
    switch (cspec) {
        case QColor::Rgb: {
            return hsv.toRgb();
        }
        case QColor::Hsv: {
            return hsv;
        }
        default: {
            Q_ASSERT(false);
            return QColor();
        }
    }
    Q_UNREACHABLE();
}

/*!
    \fn QColor QColor::darker(int factor) const
    \since 4.3

    Returns a darker (or lighter) color, but does not change this object.

    If the \a factor is greater than 100, this functions returns a darker
    color. Setting \a factor to 300 returns a color that has one-third the
    brightness. If the \a factor is less than 100, the return color is lighter,
    but we recommend using the lighter() function for this purpose. If the
    \a factor is 0 or negative, the return value is unspecified.

    The function converts the current RGB color to HSV, divides the value (V)
    component by \a factor and converts the color back to RGB.

    \sa lighter(), isValid()
*/

/*!
    \obsolete

    Use darker(\a factor) instead.
*/
QColor QColor::dark(int factor) const
{
    if (cspec == QColor::Invalid) {
        return *this;
    } else if (factor <= 0) {
        // invalid darkness factor
        return *this;
    } else if (factor < 100) {
        // makes color lighter
        return lighter(10000 / factor);
    }

    QColor hsv = toHsv();
    hsv.ct.ahsv.value = (hsv.ct.ahsv.value * 100) / factor;

    // convert back to same color spec as original color
    switch (cspec) {
        case QColor::Rgb: {
            return hsv.toRgb();
        }
        case QColor::Hsv: {
            return hsv;
        }
        default: {
            Q_ASSERT(false);
            return QColor();
        }
    }
    Q_UNREACHABLE();
}

/*!
    Assigns a copy of \a color to this color, and returns a reference to it.
*/
QColor &QColor::operator=(const QColor &color)
{
    cspec = color.cspec;
    ct.argb = color.ct.argb;
    return *this;
}

/*! \overload
    Assigns a copy of \a color and returns a reference to this color.
 */
QColor &QColor::operator=(Qt::GlobalColor color)
{
    return operator=(QColor(color));
}

/*!
    Returns true if this color has the same RGB and alpha values as \a color;
    otherwise returns false.
*/
bool QColor::operator==(const QColor &color) const
{
    return (cspec == color.cspec
            && ct.argb.alpha == color.ct.argb.alpha
            && (((cspec == QColor::Hsv)
                    && ((ct.ahsv.hue % 36000) == (color.ct.ahsv.hue % 36000)))
                || (ct.ahsv.hue == color.ct.ahsv.hue))
            && ct.argb.green == color.ct.argb.green
            && ct.argb.blue  == color.ct.argb.blue);
}

/*!
    Returns true if this color has a different RGB and alpha values from
    \a color; otherwise returns false.
*/
bool QColor::operator!=(const QColor &color) const
{ return !operator==(color); }


/*!
    Returns the color as a QVariant
*/
QColor::operator QVariant() const
{
    return QVariant(QVariant::Color, this);
}

#ifdef Q_WS_X11
/*!
    Returns true if setNamedColor() is allowed to look up colors in the X11
    color database. By default, this function returns false.

    \note This function is only available on the X11 platform.

    \sa setAllowX11ColorNames()
*/
bool QColor::allowX11ColorNames()
{
    return qAllowX11ColorNames;
}

/*!
    Allow setNamedColor() to look up colors in the X11 color database if
    \a enabled. By default, setNamedColor() does \e not look up colors in the
    X11 color database.

    \note This function is only available on the X11 platform.

    \sa setNamedColor(), allowX11ColorNames()
*/
void QColor::setAllowX11ColorNames(bool enabled)
{
    qAllowX11ColorNames = enabled;
}
#endif

/*! \internal

    Marks the color as invalid and sets all components to zero (alpha is set
    to fully opaque for compatibility with Qt 3).
*/
void QColor::invalidate()
{
    cspec = QColor::Invalid;
    ct.argb.alpha = USHRT_MAX;
    ct.argb.red = 0;
    ct.argb.green = 0;
    ct.argb.blue = 0;
}


/*****************************************************************************
  QColor stream functions
 *****************************************************************************/

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QColor &c)
{
#ifndef Q_BROKEN_DEBUG_STREAM
    if (!c.isValid())
        dbg.nospace() << "QColor(Invalid)";
    else if (c.spec() == QColor::Rgb)
        dbg.nospace() << "QColor(ARGB " << c.alphaF() << ", " << c.redF() << ", " << c.greenF() << ", " << c.blueF() << ')';
    else if (c.spec() == QColor::Hsv)
        dbg.nospace() << "QColor(AHSV " << c.alphaF() << ", " << c.hueF() << ", " << c.saturationF() << ", " << c.valueF() << ')';

    return dbg.space();
#else
    qWarning("This compiler doesn't support streaming QColor to QDebug");
    return dbg;
    Q_UNUSED(c);
#endif
}
#endif

#ifndef QT_NO_DATASTREAM
/*!
    \fn QDataStream &operator<<(QDataStream &stream, const QColor &color)
    \relates QColor

    Writes the \a color to the \a stream.

    \sa {Serializing Qt Data Types}
*/
QDataStream &operator<<(QDataStream &stream, const QColor &color)
{
    qint8   s = color.cspec;
    quint16 a = color.ct.argb.alpha;
    quint16 r = color.ct.argb.red;
    quint16 g = color.ct.argb.green;
    quint16 b = color.ct.argb.blue;

    stream << s;
    stream << a;
    stream << r;
    stream << g;
    stream << b;

    return stream;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, QColor &color)
    \relates QColor

    Reads the \a color from the \a stream.

    \sa {Serializing Qt Data Types}
*/
QDataStream &operator>>(QDataStream &stream, QColor &color)
{
    qint8 s;
    quint16 a, r, g, b;
    stream >> s;
    stream >> a;
    stream >> r;
    stream >> g;
    stream >> b;

    color.cspec = QColor::Spec(s);
    color.ct.argb.alpha = a;
    color.ct.argb.red   = r;
    color.ct.argb.green = g;
    color.ct.argb.blue  = b;

    return stream;
}
#endif // QT_NO_DATASTREAM


/*****************************************************************************
  QColor global functions (documentation only)
 *****************************************************************************/

/*!
    \fn int qRed(QRgb rgb)
    \relates QColor

    Returns the red component of the ARGB quadruplet \a rgb.

    \sa qRgb(), QColor::red()
*/

/*!
    \fn int qGreen(QRgb rgb)
    \relates QColor

    Returns the green component of the ARGB quadruplet \a rgb.

    \sa qRgb(), QColor::green()
*/

/*!
    \fn int qBlue(QRgb rgb)
    \relates QColor

    Returns the blue component of the ARGB quadruplet \a rgb.

    \sa qRgb(), QColor::blue()
*/

/*!
    \fn int qAlpha(QRgb rgba)
    \relates QColor

    Returns the alpha component of the ARGB quadruplet \a rgba.

    \sa qRgb(), QColor::alpha()
*/

/*!
    \fn QRgb qRgb(int r, int g, int b)
    \relates QColor

    Returns the ARGB quadruplet (255, \a{r}, \a{g}, \a{b}).

    \sa qRgba(), qRed(), qGreen(), qBlue()
*/

/*!
    \fn QRgb qRgba(int r, int g, int b, int a)
    \relates QColor

    Returns the ARGB quadruplet (\a{a}, \a{r}, \a{g}, \a{b}).

    \sa qRgb(), qRed(), qGreen(), qBlue()
*/

/*!
    \fn int qGray(int r, int g, int b)
    \relates QColor

    Returns a gray value (0 to 255) from the (\a r, \a g, \a b)
    triplet.

    The gray value is calculated using the formula (\a r * 11 + \a g * 16 +
    \a b * 5)/32.
*/

/*!
    \fn int qGray(QRgb rgb)
    \overload
    \relates QColor

    Returns a gray value (0 to 255) from the given ARGB quadruplet \a rgb.

    The gray value is calculated using the formula (R * 11 + G * 16 + B * 5)/32;
    the alpha-channel is ignored.
*/

/*!
    \fn QColor::QColor(int x, int y, int z, Spec colorSpec)

    Use one of the other QColor constructors, or one of the static convenience
    functions, instead.
*/

/*!
    \fn QColor::rgb(int *r, int *g, int *b) const

    Use getRgb() instead.
*/

/*!
    \fn QColor::hsv(int *h, int *s, int *v) const

    Use getHsv() instead.
*/

/*!
    \typedef QRgb
    \relates QColor

    An ARGB quadruplet on the format #AARRGGBB, equivalent to an unsigned int.

    The type also holds a value for the alpha-channel. The default alpha
    channel is \c ff, i.e opaque. For more information, see the
    \l{QColor#Alpha-Blended Drawing}{Alpha-Blended Drawing} section.

    \sa QColor::rgb(), QColor::rgba()
*/

QT_END_NAMESPACE
