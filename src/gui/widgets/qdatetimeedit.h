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

#ifndef QDATETIMEEDIT_H
#define QDATETIMEEDIT_H

#include <QtCore/qdatetime.h>
#include <QtCore/qvariant.h>
#include <QtGui/qabstractspinbox.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_DATETIMEEDIT

class QDateTimeEditPrivate;
class QStyleOptionSpinBox;
class QCalendarWidget;

class Q_GUI_EXPORT QDateTimeEdit : public QAbstractSpinBox
{
    Q_OBJECT

    Q_ENUMS(Section)
    Q_FLAGS(Sections)
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged USER true)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QTime time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(QDateTime maximumDateTime READ maximumDateTime WRITE setMaximumDateTime RESET clearMaximumDateTime)
    Q_PROPERTY(QDateTime minimumDateTime READ minimumDateTime WRITE setMinimumDateTime RESET clearMinimumDateTime)
    Q_PROPERTY(QDate maximumDate READ maximumDate WRITE setMaximumDate RESET clearMaximumDate)
    Q_PROPERTY(QDate minimumDate READ minimumDate WRITE setMinimumDate RESET clearMinimumDate)
    Q_PROPERTY(QTime maximumTime READ maximumTime WRITE setMaximumTime RESET clearMaximumTime)
    Q_PROPERTY(QTime minimumTime READ minimumTime WRITE setMinimumTime RESET clearMinimumTime)
    Q_PROPERTY(Section currentSection READ currentSection WRITE setCurrentSection)
    Q_PROPERTY(Sections displayedSections READ displayedSections)
    Q_PROPERTY(QString displayFormat READ displayFormat WRITE setDisplayFormat)
    Q_PROPERTY(bool calendarPopup READ calendarPopup WRITE setCalendarPopup)
    Q_PROPERTY(int currentSectionIndex READ currentSectionIndex WRITE setCurrentSectionIndex)
    Q_PROPERTY(int sectionCount READ sectionCount)
    Q_PROPERTY(Qt::TimeSpec timeSpec READ timeSpec WRITE setTimeSpec)
public:
    enum Section {
        NoSection = 0x0000,
        AmPmSection = 0x0001,
        MSecSection = 0x0002,
        SecondSection = 0x0004,
        MinuteSection = 0x0008,
        HourSection   = 0x0010,
        DaySection    = 0x0100,
        MonthSection  = 0x0200,
        YearSection   = 0x0400,
        TimeSections_Mask = AmPmSection|MSecSection|SecondSection|MinuteSection|HourSection,
        DateSections_Mask = DaySection|MonthSection|YearSection
    };

    Q_DECLARE_FLAGS(Sections, Section)

    explicit QDateTimeEdit(QWidget *parent = nullptr);
    explicit QDateTimeEdit(const QDateTime &dt, QWidget *parent = nullptr);
    explicit QDateTimeEdit(const QDate &d, QWidget *parent = nullptr);
    explicit QDateTimeEdit(const QTime &t, QWidget *parent = nullptr);

    QDateTime dateTime() const;
    QDate date() const;
    QTime time() const;

    QDateTime minimumDateTime() const;
    void clearMinimumDateTime();
    void setMinimumDateTime(const QDateTime &dt);

    QDateTime maximumDateTime() const;
    void clearMaximumDateTime();
    void setMaximumDateTime(const QDateTime &dt);

    void setDateTimeRange(const QDateTime &min, const QDateTime &max);

    QDate minimumDate() const;
    void setMinimumDate(const QDate &min);
    void clearMinimumDate();

    QDate maximumDate() const;
    void setMaximumDate(const QDate &max);
    void clearMaximumDate();

    void setDateRange(const QDate &min, const QDate &max);

    QTime minimumTime() const;
    void setMinimumTime(const QTime &min);
    void clearMinimumTime();

    QTime maximumTime() const;
    void setMaximumTime(const QTime &max);
    void clearMaximumTime();

    void setTimeRange(const QTime &min, const QTime &max);

    Sections displayedSections() const;
    Section currentSection() const;
    Section sectionAt(int index) const;
    void setCurrentSection(Section section);

    int currentSectionIndex() const;
    void setCurrentSectionIndex(int index);

    QCalendarWidget *calendarWidget() const;
    void setCalendarWidget(QCalendarWidget *calendarWidget);

    int sectionCount() const;

    void setSelectedSection(Section section);

    QString sectionText(Section section) const;

    QString displayFormat() const;
    void setDisplayFormat(const QString &format);

    bool calendarPopup() const;
    void setCalendarPopup(bool enable);

    Qt::TimeSpec timeSpec() const;
    void setTimeSpec(Qt::TimeSpec spec);

    QSize sizeHint() const;

    virtual void clear();
    virtual void stepBy(int steps);

    bool event(QEvent *event);
Q_SIGNALS:
    void dateTimeChanged(const QDateTime &date);
    void timeChanged(const QTime &date);
    void dateChanged(const QDate &date);

public Q_SLOTS:
    void setDateTime(const QDateTime &dateTime);
    void setDate(const QDate &date);
    void setTime(const QTime &time);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual bool focusNextPrevChild(bool next);
    virtual QValidator::State validate(QString &input, int &pos) const;
    virtual void fixup(QString &input) const;

    virtual QDateTime dateTimeFromText(const QString &text) const;
    virtual QString textFromDateTime(const QDateTime &dt) const;
    virtual StepEnabled stepEnabled() const;
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    void initStyleOption(QStyleOptionSpinBox *option) const;

    QDateTimeEdit(const QVariant &val, QVariant::Type parserType, QWidget *parent = nullptr);
private:
    Q_DECLARE_PRIVATE(QDateTimeEdit)
    Q_DISABLE_COPY(QDateTimeEdit)

    Q_PRIVATE_SLOT(d_func(), void _q_resetButton())
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

Q_DECLARE_OPERATORS_FOR_FLAGS(QDateTimeEdit::Sections)

#endif // QT_NO_DATETIMEEDIT

QT_END_NAMESPACE


#endif // QDATETIMEEDIT_H
