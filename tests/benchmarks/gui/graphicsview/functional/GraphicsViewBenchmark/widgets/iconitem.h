/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the examples of the Katie Toolkit.
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

#ifndef ICONITEM_H
#define ICONITEM_H

#include <QPainter>

#include "gvbwidget.h"

#if (QT_VERSION >= 0x040600)
class QGraphicsOpacityEffect;
#endif
class QPainter;

class IconItem : public GvbWidget
{
    Q_OBJECT

public:

    IconItem(const QString &filename = "", QGraphicsItem *parent = 0);

    virtual ~IconItem();

    QString fileName() const;
    void setFileName(const QString &filename);

#if (QT_VERSION >= 0x040600)
    void setOpacityEffectEnabled(const bool enable);
    bool isOpacityEffectEnabled() const;
#endif
    void setRotation(const qreal rotation) { m_rotation = rotation; }
    qreal rotation() const { return m_rotation; }

    void setSmoothTransformationEnabled(const bool enable) { m_smoothTransformation = enable; }
    bool isSmoothTransformationEnabled() const { return m_smoothTransformation; }

private:

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget */*widget = 0*/);
    QSizeF sizeHint(Qt::SizeHint which, 
        const QSizeF &constraint = QSizeF()) const;

private:
    Q_DISABLE_COPY(IconItem)
    void reload();

    QString m_filename;
    QPixmap m_pixmap;
    qreal m_rotation;
#if (QT_VERSION >= 0x040600)
    QGraphicsOpacityEffect *m_opacityEffect;
#endif
    bool m_smoothTransformation;
};

#endif
