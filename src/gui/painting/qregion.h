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

#ifndef QREGION_H
#define QREGION_H

#include <QtCore/qatomic.h>
#include <QtCore/qrect.h>
#include <QtGui/qwindowdefs.h>

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif


QT_BEGIN_NAMESPACE

template <class T> class QVector;
class QVariant;
class QPolygon;
class QBitmap;

#if defined(Q_WS_X11)
struct QRegionPrivate;
#endif

class Q_GUI_EXPORT QRegion
{
public:
    enum RegionType { Rectangle, Ellipse };

    QRegion();
    QRegion(int x, int y, int w, int h, RegionType t = Rectangle);
    QRegion(const QRect &r, RegionType t = Rectangle);
    QRegion(const QPolygon &pa, Qt::FillRule fillRule = Qt::OddEvenFill);
    QRegion(const QRegion &region);
    QRegion(const QBitmap &bitmap);
    ~QRegion();
    QRegion &operator=(const QRegion &);
#ifdef Q_COMPILER_RVALUE_REFS
    inline QRegion &operator=(QRegion &&other)
    { qSwap(d, other.d); return *this; }
#endif
    inline void swap(QRegion &other) { qSwap(d, other.d); }
    bool isEmpty() const;

    bool contains(const QPoint &p) const;
    bool contains(const QRect &r) const;

    void translate(int dx, int dy);
    inline void translate(const QPoint &p) { translate(p.x(), p.y()); }
    QRegion translated(int dx, int dy) const;
    inline QRegion translated(const QPoint &p) const { return translated(p.x(), p.y()); }

    // ### Qt 5: make these four functions QT4_SUPPORT
    QRegion unite(const QRegion &r) const;
    QRegion unite(const QRect &r) const;
    QRegion intersect(const QRegion &r) const;
    QRegion intersect(const QRect &r) const;
    QRegion subtract(const QRegion &r) const;
    QRegion eor(const QRegion &r) const;

    inline QRegion united(const QRegion &r) const { return unite(r); }
    inline QRegion united(const QRect &r) const { return unite(r); }
    inline QRegion intersected(const QRegion &r) const { return intersect(r); }
    inline QRegion intersected(const QRect &r) const { return intersect(r); }
    inline QRegion subtracted(const QRegion &r) const { return subtract(r); }
    inline QRegion xored(const QRegion &r) const { return eor(r); }

    bool intersects(const QRegion &r) const;
    bool intersects(const QRect &r) const;

    QRect boundingRect() const;
    QVector<QRect> rects() const;
    void setRects(const QRect *rect, int num);
    int rectCount() const;

    const QRegion operator|(const QRegion &r) const;
    const QRegion operator+(const QRegion &r) const;
    const QRegion operator+(const QRect &r) const;
    const QRegion operator&(const QRegion &r) const;
    const QRegion operator&(const QRect &r) const;
    const QRegion operator-(const QRegion &r) const;
    const QRegion operator^(const QRegion &r) const;
    QRegion& operator|=(const QRegion &r);
    QRegion& operator+=(const QRegion &r);
    QRegion& operator+=(const QRect &r);
    QRegion& operator&=(const QRegion &r);
    QRegion& operator&=(const QRect &r);
    QRegion& operator-=(const QRegion &r);
    QRegion& operator^=(const QRegion &r);

    bool operator==(const QRegion &r) const;
    inline bool operator!=(const QRegion &r) const { return !(operator==(r)); }
    operator QVariant() const;

#if defined(Q_WS_X11)
    inline Region handle() const { if(!d->rgn) updateX11Region(); return d->rgn; }
#endif

#ifndef QT_NO_DATASTREAM
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QRegion &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QRegion &);
#endif
private:
    QRegion copy() const;   // helper of detach.
    void detach();
#if defined(Q_WS_X11)
    void updateX11Region() const;
    void *clipRectangles(int &num) const;
    friend void *qt_getClipRects(const QRegion &r, int &num);
#endif
    friend bool qt_region_strictContains(const QRegion &region,
                                         const QRect &rect);

    struct QRegionData {
        QAtomicInt ref;
#if defined(Q_WS_X11)
        Region rgn;
        void *xrectangles;
        QRegionPrivate *qt_rgn;
#endif
    };
    QRegionData *d;
    static QRegionData shared_empty;
    static void cleanUp(QRegionData *x);
};

/*****************************************************************************
  QRegion stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QRegion &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QRegion &);
#endif

#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QRegion &);
#endif

QT_END_NAMESPACE


#endif // QREGION_H
