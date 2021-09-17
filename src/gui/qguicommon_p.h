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

inline static const QVector<QRgb>& grayColorTable()
{
/*
    same as
    for (int i=0; i<256; i++) {
        colortable.append(qRgb(i, i, i));
    }

    generated via:
    for (int i=0; i<256; i++) {
        qDebug("%u,", qRgb(i, i, i));
    }
*/

    static const QVector<QRgb> colortable = {
        4278190080,
        4278255873,
        4278321666,
        4278387459,
        4278453252,
        4278519045,
        4278584838,
        4278650631,
        4278716424,
        4278782217,
        4278848010,
        4278913803,
        4278979596,
        4279045389,
        4279111182,
        4279176975,
        4279242768,
        4279308561,
        4279374354,
        4279440147,
        4279505940,
        4279571733,
        4279637526,
        4279703319,
        4279769112,
        4279834905,
        4279900698,
        4279966491,
        4280032284,
        4280098077,
        4280163870,
        4280229663,
        4280295456,
        4280361249,
        4280427042,
        4280492835,
        4280558628,
        4280624421,
        4280690214,
        4280756007,
        4280821800,
        4280887593,
        4280953386,
        4281019179,
        4281084972,
        4281150765,
        4281216558,
        4281282351,
        4281348144,
        4281413937,
        4281479730,
        4281545523,
        4281611316,
        4281677109,
        4281742902,
        4281808695,
        4281874488,
        4281940281,
        4282006074,
        4282071867,
        4282137660,
        4282203453,
        4282269246,
        4282335039,
        4282400832,
        4282466625,
        4282532418,
        4282598211,
        4282664004,
        4282729797,
        4282795590,
        4282861383,
        4282927176,
        4282992969,
        4283058762,
        4283124555,
        4283190348,
        4283256141,
        4283321934,
        4283387727,
        4283453520,
        4283519313,
        4283585106,
        4283650899,
        4283716692,
        4283782485,
        4283848278,
        4283914071,
        4283979864,
        4284045657,
        4284111450,
        4284177243,
        4284243036,
        4284308829,
        4284374622,
        4284440415,
        4284506208,
        4284572001,
        4284637794,
        4284703587,
        4284769380,
        4284835173,
        4284900966,
        4284966759,
        4285032552,
        4285098345,
        4285164138,
        4285229931,
        4285295724,
        4285361517,
        4285427310,
        4285493103,
        4285558896,
        4285624689,
        4285690482,
        4285756275,
        4285822068,
        4285887861,
        4285953654,
        4286019447,
        4286085240,
        4286151033,
        4286216826,
        4286282619,
        4286348412,
        4286414205,
        4286479998,
        4286545791,
        4286611584,
        4286677377,
        4286743170,
        4286808963,
        4286874756,
        4286940549,
        4287006342,
        4287072135,
        4287137928,
        4287203721,
        4287269514,
        4287335307,
        4287401100,
        4287466893,
        4287532686,
        4287598479,
        4287664272,
        4287730065,
        4287795858,
        4287861651,
        4287927444,
        4287993237,
        4288059030,
        4288124823,
        4288190616,
        4288256409,
        4288322202,
        4288387995,
        4288453788,
        4288519581,
        4288585374,
        4288651167,
        4288716960,
        4288782753,
        4288848546,
        4288914339,
        4288980132,
        4289045925,
        4289111718,
        4289177511,
        4289243304,
        4289309097,
        4289374890,
        4289440683,
        4289506476,
        4289572269,
        4289638062,
        4289703855,
        4289769648,
        4289835441,
        4289901234,
        4289967027,
        4290032820,
        4290098613,
        4290164406,
        4290230199,
        4290295992,
        4290361785,
        4290427578,
        4290493371,
        4290559164,
        4290624957,
        4290690750,
        4290756543,
        4290822336,
        4290888129,
        4290953922,
        4291019715,
        4291085508,
        4291151301,
        4291217094,
        4291282887,
        4291348680,
        4291414473,
        4291480266,
        4291546059,
        4291611852,
        4291677645,
        4291743438,
        4291809231,
        4291875024,
        4291940817,
        4292006610,
        4292072403,
        4292138196,
        4292203989,
        4292269782,
        4292335575,
        4292401368,
        4292467161,
        4292532954,
        4292598747,
        4292664540,
        4292730333,
        4292796126,
        4292861919,
        4292927712,
        4292993505,
        4293059298,
        4293125091,
        4293190884,
        4293256677,
        4293322470,
        4293388263,
        4293454056,
        4293519849,
        4293585642,
        4293651435,
        4293717228,
        4293783021,
        4293848814,
        4293914607,
        4293980400,
        4294046193,
        4294111986,
        4294177779,
        4294243572,
        4294309365,
        4294375158,
        4294440951,
        4294506744,
        4294572537,
        4294638330,
        4294704123,
        4294769916,
        4294835709,
        4294901502,
        4294967295
    };
    return colortable;
}

inline static const QVector<QRgb>& alphaColorTable()
{
/*
    same as
    for (int i=0; i<256; i++) {
        colortable.append(qRgba(0, 0, 0, i));
    }

    generated via:
    for (int i=0; i<256; i++) {
        qDebug("%u,", qRgba(0, 0, 0, i));
    }
*/

    static const QVector<QRgb> colortable = {
        0,
        16777216,
        33554432,
        50331648,
        67108864,
        83886080,
        100663296,
        117440512,
        134217728,
        150994944,
        167772160,
        184549376,
        201326592,
        218103808,
        234881024,
        251658240,
        268435456,
        285212672,
        301989888,
        318767104,
        335544320,
        352321536,
        369098752,
        385875968,
        402653184,
        419430400,
        436207616,
        452984832,
        469762048,
        486539264,
        503316480,
        520093696,
        536870912,
        553648128,
        570425344,
        587202560,
        603979776,
        620756992,
        637534208,
        654311424,
        671088640,
        687865856,
        704643072,
        721420288,
        738197504,
        754974720,
        771751936,
        788529152,
        805306368,
        822083584,
        838860800,
        855638016,
        872415232,
        889192448,
        905969664,
        922746880,
        939524096,
        956301312,
        973078528,
        989855744,
        1006632960,
        1023410176,
        1040187392,
        1056964608,
        1073741824,
        1090519040,
        1107296256,
        1124073472,
        1140850688,
        1157627904,
        1174405120,
        1191182336,
        1207959552,
        1224736768,
        1241513984,
        1258291200,
        1275068416,
        1291845632,
        1308622848,
        1325400064,
        1342177280,
        1358954496,
        1375731712,
        1392508928,
        1409286144,
        1426063360,
        1442840576,
        1459617792,
        1476395008,
        1493172224,
        1509949440,
        1526726656,
        1543503872,
        1560281088,
        1577058304,
        1593835520,
        1610612736,
        1627389952,
        1644167168,
        1660944384,
        1677721600,
        1694498816,
        1711276032,
        1728053248,
        1744830464,
        1761607680,
        1778384896,
        1795162112,
        1811939328,
        1828716544,
        1845493760,
        1862270976,
        1879048192,
        1895825408,
        1912602624,
        1929379840,
        1946157056,
        1962934272,
        1979711488,
        1996488704,
        2013265920,
        2030043136,
        2046820352,
        2063597568,
        2080374784,
        2097152000,
        2113929216,
        2130706432,
        2147483648,
        2164260864,
        2181038080,
        2197815296,
        2214592512,
        2231369728,
        2248146944,
        2264924160,
        2281701376,
        2298478592,
        2315255808,
        2332033024,
        2348810240,
        2365587456,
        2382364672,
        2399141888,
        2415919104,
        2432696320,
        2449473536,
        2466250752,
        2483027968,
        2499805184,
        2516582400,
        2533359616,
        2550136832,
        2566914048,
        2583691264,
        2600468480,
        2617245696,
        2634022912,
        2650800128,
        2667577344,
        2684354560,
        2701131776,
        2717908992,
        2734686208,
        2751463424,
        2768240640,
        2785017856,
        2801795072,
        2818572288,
        2835349504,
        2852126720,
        2868903936,
        2885681152,
        2902458368,
        2919235584,
        2936012800,
        2952790016,
        2969567232,
        2986344448,
        3003121664,
        3019898880,
        3036676096,
        3053453312,
        3070230528,
        3087007744,
        3103784960,
        3120562176,
        3137339392,
        3154116608,
        3170893824,
        3187671040,
        3204448256,
        3221225472,
        3238002688,
        3254779904,
        3271557120,
        3288334336,
        3305111552,
        3321888768,
        3338665984,
        3355443200,
        3372220416,
        3388997632,
        3405774848,
        3422552064,
        3439329280,
        3456106496,
        3472883712,
        3489660928,
        3506438144,
        3523215360,
        3539992576,
        3556769792,
        3573547008,
        3590324224,
        3607101440,
        3623878656,
        3640655872,
        3657433088,
        3674210304,
        3690987520,
        3707764736,
        3724541952,
        3741319168,
        3758096384,
        3774873600,
        3791650816,
        3808428032,
        3825205248,
        3841982464,
        3858759680,
        3875536896,
        3892314112,
        3909091328,
        3925868544,
        3942645760,
        3959422976,
        3976200192,
        3992977408,
        4009754624,
        4026531840,
        4043309056,
        4060086272,
        4076863488,
        4093640704,
        4110417920,
        4127195136,
        4143972352,
        4160749568,
        4177526784,
        4194304000,
        4211081216,
        4227858432,
        4244635648,
        4261412864,
        4278190080
    };
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

static const uchar qt_pixmap_bit_mask[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

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

#ifndef QT_NO_IMAGEFORMAT_XPM
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
