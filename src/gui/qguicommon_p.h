#ifndef QGUICOMMON_P_H
#define QGUICOMMON_P_H

#include "qfactoryloader_p.h"
#include "qiconengineplugin.h"
#include "qimageiohandler.h"
#include "qtabbar.h"
#include "qtabwidget.h"

QT_BEGIN_NAMESPACE

static QColor mergedColors(const QColor &colorA, const QColor &colorB, int factor = 50)
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
#endif

#if !defined(QT_NO_LIBRARY)
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, imageloader,
    (QImageIOHandlerFactoryInterface_iid, QLatin1String("/imageformats")))
#endif

const uchar qt_pixmap_bit_mask[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

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
static Qt::DockWidgetArea toDockWidgetArea(QInternal::DockPosition pos)
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
static inline QTabBar::Shape tabBarShapeFrom(QTabWidget::TabShape shape, QTabWidget::TabPosition position)
{
    const bool rounded = (shape == QTabWidget::Rounded);
    if (position == QTabWidget::North)
        return rounded ? QTabBar::RoundedNorth : QTabBar::TriangularNorth;
    if (position == QTabWidget::South)
        return rounded ? QTabBar::RoundedSouth : QTabBar::TriangularSouth;
    if (position == QTabWidget::East)
        return rounded ? QTabBar::RoundedEast : QTabBar::TriangularEast;
    if (position == QTabWidget::West)
        return rounded ? QTabBar::RoundedWest : QTabBar::TriangularWest;
    return QTabBar::RoundedNorth;
}
#endif // QT_NO_TABWIDGET

QT_END_NAMESPACE

#endif // QGUICOMMON_P_H
