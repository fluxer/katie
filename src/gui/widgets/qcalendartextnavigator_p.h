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

#ifndef QCALENDARTEXTNAVIGATOR_P_H
#define QCALENDARTEXTNAVIGATOR_P_H

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

#include <QtCore/qobject.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qbasictimer.h>

#ifndef QT_NO_CALENDARWIDGET

QT_BEGIN_NAMESPACE

class QLabel;
class QCalendarDateValidator;
class QFrame;

class QCalendarTextNavigator: public QObject
{
    Q_OBJECT
public:
    QCalendarTextNavigator(QObject *parent = Q_NULLPTR)
        : QObject(parent), m_dateText(0), m_dateFrame(0), m_dateValidator(0), m_widget(0), m_editDelay(1500), m_date(QDate::currentDate()) { }

    QWidget *widget() const;
    void setWidget(QWidget *widget);

    int dateEditAcceptDelay() const;
    void setDateEditAcceptDelay(int delay);

    QDate date() const;
    void setDate(const QDate &date);

    bool eventFilter(QObject *o, QEvent *e);
    void timerEvent(QTimerEvent *e);

signals:
    void dateChanged(const QDate &date);
    void editingFinished();

private:
    void applyDate();
    void updateDateLabel();
    void createDateLabel();
    void removeDateLabel();

    QLabel *m_dateText;
    QFrame *m_dateFrame;
    QBasicTimer m_acceptTimer;
    QCalendarDateValidator *m_dateValidator;
    QWidget *m_widget;
    int m_editDelay;

    QDate m_date;
};

QT_END_NAMESPACE

#endif // QT_NO_CALENDARWIDGET

#endif

