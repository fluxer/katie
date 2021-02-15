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

#ifndef __THEMEEVENT_H__
#define __THEMEEVENT_H__

#include <QEvent>
#include <QString>

static QEvent::Type ThemeEventType = (QEvent::Type) 1010;

class ThemeEvent : public QEvent
{
public:
    ThemeEvent(QString newTheme, QEvent::Type type = ThemeEventType );
    ~ThemeEvent();

public:
    inline QString getTheme() { return m_theme; }
    
private:
    QString m_theme;
};


#endif /* __THEMEEVENT_H__ */
