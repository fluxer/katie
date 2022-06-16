#ifndef QGUICOMMON_P_H
#define QGUICOMMON_P_H

#include "qfactoryloader_p.h"
#include "qtabbar.h"
#include "qtabwidget.h"
#include "qmap.h"
#include "qdrawhelper_p.h"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_IMAGEFORMAT_KAT
static const char* const qt_imageformat = "kat";
static const char* const qt_imagemime = "image/katie";
#else
static const char* const qt_imageformat = "png";
static const char* const qt_imagemime = "image/png";
#endif

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

inline static QImage replaceColors(const QImage &source, const QMap<QRgb, QRgb> &colormap)
{
    QImage result(source);
    const int bpl = result.bytesPerLine();
    uchar* imagebits = result.bits();
    switch (result.format()) {
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied: {
            for (int h = 0; h < source.height(); h++) {
                uchar* scan = QFAST_SCAN_LINE(imagebits, bpl, h);
                for (int w = 0; w < source.width(); w++) {
                    const QRgb pixel = source.pixel(w, h);
                    ((uint *)scan)[w] = colormap.value(pixel, pixel);
                }
            }
            break;
        }
        default: {
            Q_ASSERT_X(false, "replaceColors", "internal error");
            break;
        }
    }
    return result;
}

Q_GLOBAL_STATIC(QWidget, globalStyleInfoWidget)

#if !defined(QT_NO_LIBRARY)
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, iconloader, (QString::fromLatin1("/iconengines")))
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, imageloader, (QString::fromLatin1("/imageformats")))
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

// use the same rounding as in qrasterizer.cpp (6 bit fixed point)
static const qreal aliasedCoordinateDelta = 0.5 - 0.015625;

QT_END_NAMESPACE

#endif // QGUICOMMON_P_H
