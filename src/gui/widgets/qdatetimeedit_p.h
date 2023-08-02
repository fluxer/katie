/****************************************************************************
**
** Copyright (C) 2023 Ivailo Monev
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

#ifndef QDATETIMEEDIT_P_H
#define QDATETIMEEDIT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qwidget_p.h"
#include "qdatetimeedit.h"
#include "qboxlayout.h"
#include "qspinbox.h"
#include "qtoolbutton.h"
#include "qmenu.h"
#include "qwidgetaction.h"
#include "qdebug.h"

#ifndef QT_NO_DATETIMEEDIT

QT_BEGIN_NAMESPACE

class QDateTimeBox : public QSpinBox
{
public:
    QDateTimeBox(QWidget *parent);

protected:
    QValidator::State validate(QString &input, int &pos) const final;
    int valueFromText(const QString &text) const final;
    QString textFromValue(int value) const final;
};

class QDateTimeEditPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QDateTimeEdit)
public:
    QDateTimeEditPrivate();

    QDateTime minimumdate;
    QDateTime maximumdate;
    QCalendarWidget *calendarwidget;

    void init(const QDateTime &datetime, const bool showdate, const bool showtime);
    void updateWidgets(const QDateTime &datetime);
    void updateButton(const QDate &date);
    void setCalendar(QCalendarWidget *calendar);
    QDateTime currentDateTime() const;

    void _q_dateChanged();
    void _q_timeChanged();
    void _q_selectDate();

private:
    bool m_showdate;
    bool m_showtime;
    QHBoxLayout *m_layout;
    QDateTimeBox *m_timebox;
    QToolButton *m_datebutton;
    QMenu* m_datemenu;
    QWidgetAction* m_dateaction;
};

QT_END_NAMESPACE

#endif // QT_NO_DATETIMEEDIT

#endif // QDATETIMEEDIT_P_H
