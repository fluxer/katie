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

#ifndef QTEXTOPTION_H
#define QTEXTOPTION_H

#include <QtCore/qnamespace.h>
#include <QtCore/qchar.h>
#include <QtCore/qmetatype.h>



QT_BEGIN_NAMESPACE


template <typename T> class QList;

class Q_GUI_EXPORT QTextOption
{
    Q_GADGET
public:
    QTextOption();
    QTextOption(Qt::Alignment alignment);
    ~QTextOption();

    QTextOption(const QTextOption &o);
    QTextOption &operator=(const QTextOption &o);

    inline void setAlignment(Qt::Alignment alignment);
    inline Qt::Alignment alignment() const { return align; }

    inline void setTextDirection(Qt::LayoutDirection aDirection) { this->direction = aDirection; }
    inline Qt::LayoutDirection textDirection() const { return direction; }

    enum WrapMode {
        NoWrap,
        WordWrap,
        ManualWrap,
        WrapAnywhere,
        WrapAtWordBoundaryOrAnywhere
    };
    Q_ENUMS(WrapMode)
    inline void setWrapMode(WrapMode wrap) { wordwrap = wrap; }
    inline WrapMode wrapMode() const { return wordwrap; }

    enum Flag {
        ShowTabsAndSpaces = 0x1,
        ShowLineAndParagraphSeparators = 0x2,
        AddSpaceForLineAndParagraphSeparators = 0x4,
        SuppressColors = 0x8,
        IncludeTrailingSpaces = 0x80000000
    };
    Q_DECLARE_FLAGS(Flags, Flag)
    inline void setFlags(Flags flags);
    inline Flags flags() const { return fflags; }

    inline void setTabStop(qreal tabStop);
    inline qreal tabStop() const { return tab; }

    void setUseDesignMetrics(bool b) { design = b; }
    bool useDesignMetrics() const { return design; }

private:
    WrapMode wordwrap;
    Flags fflags;
    Qt::Alignment align;
    Qt::LayoutDirection direction;
    bool design;
    qreal tab;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QTextOption::Flags)

inline void QTextOption::setAlignment(Qt::Alignment aalignment)
{ align = aalignment; }

inline void QTextOption::setFlags(Flags aflags)
{ fflags = aflags; }

inline void QTextOption::setTabStop(qreal atabStop)
{ tab = atabStop; }

QT_END_NAMESPACE

#endif // QTEXTOPTION_H
