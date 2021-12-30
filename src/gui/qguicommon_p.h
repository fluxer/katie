#ifndef QGUICOMMON_P_H
#define QGUICOMMON_P_H

#include "qfactoryloader_p.h"
#include "qiconengineplugin.h"
#include "qimageiohandler.h"
#include "qtabbar.h"
#include "qtabwidget.h"

QT_BEGIN_NAMESPACE

#ifdef QT_NO_FPU
static const qreal qt_epsilon = qreal(1e-12);
#else
static const qreal qt_epsilon = qreal(1e-5);
#endif

// same as QColor(Qt::color0).rgba() and QColor(Qt::white).rgba()
static const QRgb qt_whitergba = qRgba(255, 255, 255, 255);
// same as QColor(Qt::color0).rgb() and QColor(Qt::white).rgb()
static const QRgb qt_whitergb = qRgb(255, 255, 255);
// same as QColor(Qt::color1).rgba() and QColor(Qt::black).rgba()
static const QRgb qt_blackrgba = qRgba(0, 0, 0, 255);
// same as QColor(Qt::color1).rgb() and QColor(Qt::black).rgb()
static const QRgb qt_blackrgb = qRgb(0, 0, 0);
// same as QColor(Qt::transparent).rgba()
static const QRgb qt_transparentrgba = qRgba(0, 0, 0, 0);
// same as QColor(Qt::transparent).rgb()
static const QRgb qt_transparentrgb = qRgb(0, 0, 0);


inline static const QVector<QRgb>& monoColorTable()
{
    static const QVector<QRgb> colortable = { qt_whitergba, qt_blackrgba };
    return colortable;
}

inline static QColor mergedColors(const QColor &colorA, const QColor &colorB, int factor = 50)
{
    const int maxFactor = 100;
    QColor tmp = colorA;
    tmp.setRed((tmp.red() * factor) / maxFactor + (colorB.red() * (maxFactor - factor)) / maxFactor);
    tmp.setGreen((tmp.green() * factor) / maxFactor + (colorB.green() * (maxFactor - factor)) / maxFactor);
    tmp.setBlue((tmp.blue() * factor) / maxFactor + (colorB.blue() * (maxFactor - factor)) / maxFactor);
    return tmp;
}

Q_GLOBAL_STATIC(QWidget, globalStyleInfoWidget)

#if !defined(QT_NO_LIBRARY)
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, iconloader,
    (QIconEngineFactoryInterface_iid, QLatin1String("/iconengines"), Qt::CaseInsensitive))

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, imageloader,
    (QImageIOHandlerFactoryInterface_iid, QLatin1String("/imageformats")))
#endif

#ifndef QT_NO_TABBAR
inline static bool verticalTabs(QTabBar::Shape shape)
{
    return shape == QTabBar::RoundedWest
           || shape == QTabBar::RoundedEast
           || shape == QTabBar::TriangularWest
           || shape == QTabBar::TriangularEast;
}
#endif // QT_NO_TABBAR

#ifndef QT_NO_DOCKWIDGET
inline static Qt::DockWidgetArea toDockWidgetArea(QInternal::DockPosition pos)
{
    switch (pos) {
        case QInternal::LeftDock:   return Qt::LeftDockWidgetArea;
        case QInternal::RightDock:  return Qt::RightDockWidgetArea;
        case QInternal::TopDock:    return Qt::TopDockWidgetArea;
        case QInternal::BottomDock: return Qt::BottomDockWidgetArea;
        default: break;
    }
    return Qt::NoDockWidgetArea;
}
#endif

#ifndef QT_NO_TABWIDGET
inline static QTabBar::Shape tabBarShapeFrom(QTabWidget::TabShape shape, QTabWidget::TabPosition position)
{
    const bool rounded = (shape == QTabWidget::Rounded);
    switch (position) {
        case QTabWidget::North:
            return (rounded ? QTabBar::RoundedNorth : QTabBar::TriangularNorth);
        case QTabWidget::South:
            return (rounded ? QTabBar::RoundedSouth : QTabBar::TriangularSouth);
        case QTabWidget::East:
            return (rounded ? QTabBar::RoundedEast : QTabBar::TriangularEast);
        case QTabWidget::West:
            return (rounded ? QTabBar::RoundedWest : QTabBar::TriangularWest);
    }
    Q_UNREACHABLE();
}
#endif // QT_NO_TABWIDGET

// Returns position of lowest set bit in 'v' as an integer (0-31), or -1
inline static int lowest_bit(uint v)
{
    int i;
    ulong lb = 1;
    for (i=0; ((v & lb) == 0) && i<32;  i++, lb<<=1) {}
    return i==32 ? -1 : i;
}

static const qreal inv_dist_to_plane = 1. / 1024.;

#ifndef QT_NO_XPM
// Message box icons, from page 210 of the Windows style guide.

// Hand-drawn to resemble Microsoft's icons, but in the Mac/Netscape palette.
// Thanks to TrueColor displays, it is slightly more efficient to have
// them duplicated.
/* XPM */
static const char * const qt_information_xpm[]={
    "32 32 5 1",
    ". c None",
    "c c #000000",
    "* c #999999",
    "a c #ffffff",
    "b c #0000ff",
    "...........********.............",
    "........***aaaaaaaa***..........",
    "......**aaaaaaaaaaaaaa**........",
    ".....*aaaaaaaaaaaaaaaaaa*.......",
    "....*aaaaaaaabbbbaaaaaaaac......",
    "...*aaaaaaaabbbbbbaaaaaaaac.....",
    "..*aaaaaaaaabbbbbbaaaaaaaaac....",
    ".*aaaaaaaaaaabbbbaaaaaaaaaaac...",
    ".*aaaaaaaaaaaaaaaaaaaaaaaaaac*..",
    "*aaaaaaaaaaaaaaaaaaaaaaaaaaaac*.",
    "*aaaaaaaaaabbbbbbbaaaaaaaaaaac*.",
    "*aaaaaaaaaaaabbbbbaaaaaaaaaaac**",
    "*aaaaaaaaaaaabbbbbaaaaaaaaaaac**",
    "*aaaaaaaaaaaabbbbbaaaaaaaaaaac**",
    "*aaaaaaaaaaaabbbbbaaaaaaaaaaac**",
    "*aaaaaaaaaaaabbbbbaaaaaaaaaaac**",
    ".*aaaaaaaaaaabbbbbaaaaaaaaaac***",
    ".*aaaaaaaaaaabbbbbaaaaaaaaaac***",
    "..*aaaaaaaaaabbbbbaaaaaaaaac***.",
    "...caaaaaaabbbbbbbbbaaaaaac****.",
    "....caaaaaaaaaaaaaaaaaaaac****..",
    ".....caaaaaaaaaaaaaaaaaac****...",
    "......ccaaaaaaaaaaaaaacc****....",
    ".......*cccaaaaaaaaccc*****.....",
    "........***cccaaaac*******......",
    "..........****caaac*****........",
    ".............*caaac**...........",
    "...............caac**...........",
    "................cac**...........",
    ".................cc**...........",
    "..................***...........",
    "...................**..........."};

/* XPM */
static const char* const qt_warning_xpm[]={
    "32 32 4 1",
    ". c None",
    "a c #ffff00",
    "* c #000000",
    "b c #999999",
    ".............***................",
    "............*aaa*...............",
    "...........*aaaaa*b.............",
    "...........*aaaaa*bb............",
    "..........*aaaaaaa*bb...........",
    "..........*aaaaaaa*bb...........",
    ".........*aaaaaaaaa*bb..........",
    ".........*aaaaaaaaa*bb..........",
    "........*aaaaaaaaaaa*bb.........",
    "........*aaaa***aaaa*bb.........",
    ".......*aaaa*****aaaa*bb........",
    ".......*aaaa*****aaaa*bb........",
    "......*aaaaa*****aaaaa*bb.......",
    "......*aaaaa*****aaaaa*bb.......",
    ".....*aaaaaa*****aaaaaa*bb......",
    ".....*aaaaaa*****aaaaaa*bb......",
    "....*aaaaaaaa***aaaaaaaa*bb.....",
    "....*aaaaaaaa***aaaaaaaa*bb.....",
    "...*aaaaaaaaa***aaaaaaaaa*bb....",
    "...*aaaaaaaaaa*aaaaaaaaaa*bb....",
    "..*aaaaaaaaaaa*aaaaaaaaaaa*bb...",
    "..*aaaaaaaaaaaaaaaaaaaaaaa*bb...",
    ".*aaaaaaaaaaaa**aaaaaaaaaaa*bb..",
    ".*aaaaaaaaaaa****aaaaaaaaaa*bb..",
    "*aaaaaaaaaaaa****aaaaaaaaaaa*bb.",
    "*aaaaaaaaaaaaa**aaaaaaaaaaaa*bb.",
    "*aaaaaaaaaaaaaaaaaaaaaaaaaaa*bbb",
    "*aaaaaaaaaaaaaaaaaaaaaaaaaaa*bbb",
    ".*aaaaaaaaaaaaaaaaaaaaaaaaa*bbbb",
    "..*************************bbbbb",
    "....bbbbbbbbbbbbbbbbbbbbbbbbbbb.",
    ".....bbbbbbbbbbbbbbbbbbbbbbbbb.."};

/* XPM */
static const char* const qt_critical_xpm[]={
    "32 32 4 1",
    ". c None",
    "a c #999999",
    "* c #ff0000",
    "b c #ffffff",
    "...........********.............",
    ".........************...........",
    ".......****************.........",
    "......******************........",
    ".....********************a......",
    "....**********************a.....",
    "...************************a....",
    "..*******b**********b*******a...",
    "..******bbb********bbb******a...",
    ".******bbbbb******bbbbb******a..",
    ".*******bbbbb****bbbbb*******a..",
    "*********bbbbb**bbbbb*********a.",
    "**********bbbbbbbbbb**********a.",
    "***********bbbbbbbb***********aa",
    "************bbbbbb************aa",
    "************bbbbbb************aa",
    "***********bbbbbbbb***********aa",
    "**********bbbbbbbbbb**********aa",
    "*********bbbbb**bbbbb*********aa",
    ".*******bbbbb****bbbbb*******aa.",
    ".******bbbbb******bbbbb******aa.",
    "..******bbb********bbb******aaa.",
    "..*******b**********b*******aa..",
    "...************************aaa..",
    "....**********************aaa...",
    "....a********************aaa....",
    ".....a******************aaa.....",
    "......a****************aaa......",
    ".......aa************aaaa.......",
    ".........aa********aaaaa........",
    "...........aaaaaaaaaaa..........",
    ".............aaaaaaa............"};

/* XPM */
static const char *const qt_question_xpm[] = {
    "32 32 5 1",
    ". c None",
    "c c #000000",
    "* c #999999",
    "a c #ffffff",
    "b c #0000ff",
    "...........********.............",
    "........***aaaaaaaa***..........",
    "......**aaaaaaaaaaaaaa**........",
    ".....*aaaaaaaaaaaaaaaaaa*.......",
    "....*aaaaaaaaaaaaaaaaaaaac......",
    "...*aaaaaaaabbbbbbaaaaaaaac.....",
    "..*aaaaaaaabaaabbbbaaaaaaaac....",
    ".*aaaaaaaabbaaaabbbbaaaaaaaac...",
    ".*aaaaaaaabbbbaabbbbaaaaaaaac*..",
    "*aaaaaaaaabbbbaabbbbaaaaaaaaac*.",
    "*aaaaaaaaaabbaabbbbaaaaaaaaaac*.",
    "*aaaaaaaaaaaaabbbbaaaaaaaaaaac**",
    "*aaaaaaaaaaaaabbbaaaaaaaaaaaac**",
    "*aaaaaaaaaaaaabbaaaaaaaaaaaaac**",
    "*aaaaaaaaaaaaabbaaaaaaaaaaaaac**",
    "*aaaaaaaaaaaaaaaaaaaaaaaaaaaac**",
    ".*aaaaaaaaaaaabbaaaaaaaaaaaac***",
    ".*aaaaaaaaaaabbbbaaaaaaaaaaac***",
    "..*aaaaaaaaaabbbbaaaaaaaaaac***.",
    "...caaaaaaaaaabbaaaaaaaaaac****.",
    "....caaaaaaaaaaaaaaaaaaaac****..",
    ".....caaaaaaaaaaaaaaaaaac****...",
    "......ccaaaaaaaaaaaaaacc****....",
    ".......*cccaaaaaaaaccc*****.....",
    "........***cccaaaac*******......",
    "..........****caaac*****........",
    ".............*caaac**...........",
    "...............caac**...........",
    "................cac**...........",
    ".................cc**...........",
    "..................***...........",
    "...................**...........",
};
#endif

// use the same rounding as in qrasterizer.cpp (6 bit fixed point)
static const qreal aliasedCoordinateDelta = 0.5 - 0.015625;

QT_END_NAMESPACE

#endif // QGUICOMMON_P_H
