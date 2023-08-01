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

#ifndef QDATETIMEEDIT_H
#define QDATETIMEEDIT_H

#include <QtCore/qdatetime.h>
#include <QtGui/qcalendarwidget.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_DATETIMEEDIT

class QDateTimeEditPrivate;

class Q_GUI_EXPORT QDateTimeEdit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged USER true)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QTime time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(QDateTime maximumDateTime READ maximumDateTime)
    Q_PROPERTY(QDateTime minimumDateTime READ minimumDateTime)
    Q_PROPERTY(QDate maximumDate READ maximumDate)
    Q_PROPERTY(QDate minimumDate READ minimumDate)
    Q_PROPERTY(QTime maximumTime READ maximumTime)
    Q_PROPERTY(QTime minimumTime READ minimumTime)

public:

    explicit QDateTimeEdit(QWidget *parent = nullptr);
    explicit QDateTimeEdit(const QDateTime &dt, QWidget *parent = nullptr);
    explicit QDateTimeEdit(const QDate &d, QWidget *parent = nullptr);
    explicit QDateTimeEdit(const QTime &t, QWidget *parent = nullptr);

    QDateTime dateTime() const;
    QDate date() const;
    QTime time() const;

    QDateTime minimumDateTime() const;
    QDateTime maximumDateTime() const;
    void setDateTimeRange(const QDateTime &min, const QDateTime &max);

    QDate minimumDate() const;
    QDate maximumDate() const;
    void setDateRange(const QDate &min, const QDate &max);

    QTime minimumTime() const;
    QTime maximumTime() const;
    void setTimeRange(const QTime &min, const QTime &max);

    QCalendarWidget* calendarWidget() const;
    void setCalendarWidget(QCalendarWidget *calendarWidget);

Q_SIGNALS:
    void dateTimeChanged(const QDateTime &date);
    void timeChanged(const QTime &date);
    void dateChanged(const QDate &date);

public Q_SLOTS:
    void setDateTime(const QDateTime &dateTime);
    void setDate(const QDate &date);
    void setTime(const QTime &time);

private:
    Q_DECLARE_PRIVATE(QDateTimeEdit)
    Q_DISABLE_COPY(QDateTimeEdit)

    Q_PRIVATE_SLOT(d_func(), void _q_dateChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_timeChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_selectDate())
};

class Q_GUI_EXPORT QTimeEdit : public QDateTimeEdit
{
    Q_OBJECT
public:
    QTimeEdit(QWidget *parent = nullptr);
    QTimeEdit(const QTime &time, QWidget *parent = nullptr);
};

class Q_GUI_EXPORT QDateEdit : public QDateTimeEdit
{
    Q_OBJECT
public:
    QDateEdit(QWidget *parent = nullptr);
    QDateEdit(const QDate &date, QWidget *parent = nullptr);
};

#endif // QT_NO_DATETIMEEDIT

QT_END_NAMESPACE


#endif // QDATETIMEEDIT_H
