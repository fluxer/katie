/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QMARGINS_H
#define QMARGINS_H

#include <QtCore/qnamespace.h>


QT_BEGIN_NAMESPACE

class QDebug;

class QMargins
{
public:
    QMargins();
    QMargins(int left, int top, int right, int bottom);

    bool isNull() const;

    int left() const;
    int top() const;
    int right() const;
    int bottom() const;

    void setLeft(int left);
    void setTop(int top);
    void setRight(int right);
    void setBottom(int bottom);

private:
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;

    inline bool operator==(const QMargins &other) const;
    inline bool operator!=(const QMargins &other) const { return !operator==(other); };
};

Q_DECLARE_TYPEINFO(QMargins, Q_MOVABLE_TYPE);

/*****************************************************************************
  QMargins inline functions
 *****************************************************************************/

inline QMargins::QMargins()
{ m_top = m_bottom = m_left = m_right = 0; }

inline QMargins::QMargins(int aleft, int atop, int aright, int abottom)
    : m_left(aleft), m_top(atop), m_right(aright), m_bottom(abottom) {}

inline bool QMargins::isNull() const
{ return m_left==0 && m_top==0 && m_right==0 && m_bottom==0; }

inline int QMargins::left() const
{ return m_left; }

inline int QMargins::top() const
{ return m_top; }

inline int QMargins::right() const
{ return m_right; }

inline int QMargins::bottom() const
{ return m_bottom; }


inline void QMargins::setLeft(int aleft)
{ m_left = aleft; }

inline void QMargins::setTop(int atop)
{ m_top = atop; }

inline void QMargins::setRight(int aright)
{ m_right = aright; }

inline void QMargins::setBottom(int abottom)
{ m_bottom = abottom; }

inline bool QMargins::operator==(const QMargins &other) const
{
    return m_left == other.m_left &&
           m_top == other.m_top &&
           m_right == other.m_right &&
           m_bottom == other.m_bottom;
}

#ifndef QT_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug, const QMargins &);
#endif

QT_END_NAMESPACE


#endif // QMARGINS_H
