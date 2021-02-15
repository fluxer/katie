/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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

#include <QtGui>

#ifndef DUMMYOBJECT_H
#define DUMMYOBJECT_H

class DummyObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QRect rect READ rect WRITE setRect)
    Q_PROPERTY(float opacity READ opacity WRITE setOpacity)
public:
    DummyObject();
    QRect rect() const;
    void setRect(const QRect &r);
    float opacity() const;
    void setOpacity(float);

private:
    QRect m_rect;
    float m_opacity;
};

#endif
