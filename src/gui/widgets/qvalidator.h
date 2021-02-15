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

#ifndef QVALIDATOR_H
#define QVALIDATOR_H

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qregexp.h>
#include <QtCore/qlocale.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_VALIDATOR

class QValidatorPrivate;

class Q_GUI_EXPORT QValidator : public QObject
{
    Q_OBJECT
public:
    explicit QValidator(QObject *parent = Q_NULLPTR);
    ~QValidator();

    enum State {
        Invalid,
        Intermediate,
        Acceptable

    };

    void setLocale(const QLocale &locale);
    QLocale locale() const;

    virtual State validate(QString &, int &) const = 0;
    virtual void fixup(QString &) const;

protected:
    QValidator(QObjectPrivate &d, QObject *parent);
    QValidator(QValidatorPrivate &d, QObject *parent);

private:
    Q_DISABLE_COPY(QValidator)
    Q_DECLARE_PRIVATE(QValidator)
};

class Q_GUI_EXPORT QIntValidator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(int bottom READ bottom WRITE setBottom)
    Q_PROPERTY(int top READ top WRITE setTop)

public:
    explicit QIntValidator(QObject *parent = Q_NULLPTR);
    QIntValidator(int bottom, int top, QObject *parent = Q_NULLPTR);
    ~QIntValidator();

    QValidator::State validate(QString &, int &) const;
    void fixup(QString &input) const;

    void setBottom(int);
    void setTop(int);
    virtual void setRange(int bottom, int top);

    int bottom() const { return b; }
    int top() const { return t; }


private:
    Q_DISABLE_COPY(QIntValidator)

    int b;
    int t;
};


class QDoubleValidatorPrivate;

class Q_GUI_EXPORT QDoubleValidator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(double bottom READ bottom WRITE setBottom)
    Q_PROPERTY(double top READ top WRITE setTop)
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals)
    Q_ENUMS(Notation)
    Q_PROPERTY(Notation notation READ notation WRITE setNotation)

public:
    explicit QDoubleValidator(QObject *parent = Q_NULLPTR);
    QDoubleValidator(double bottom, double top, int decimals, QObject *parent = Q_NULLPTR);
    ~QDoubleValidator();

    enum Notation {
        StandardNotation,
        ScientificNotation
    };

    QValidator::State validate(QString &, int &) const;

    virtual void setRange(double bottom, double top, int decimals = 0);
    void setBottom(double);
    void setTop(double);
    void setDecimals(int);
    void setNotation(Notation);

    double bottom() const { return b; }
    double top() const { return t; }
    int decimals() const { return dec; }
    Notation notation() const;

private:
    Q_DECLARE_PRIVATE(QDoubleValidator)
    Q_DISABLE_COPY(QDoubleValidator)

    double b;
    double t;
    int dec;
};


class Q_GUI_EXPORT QRegExpValidator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(QRegExp regExp READ regExp WRITE setRegExp)

public:
    explicit QRegExpValidator(QObject *parent = Q_NULLPTR);
    QRegExpValidator(const QRegExp& rx, QObject *parent = Q_NULLPTR);
    ~QRegExpValidator();

    virtual QValidator::State validate(QString& input, int& pos) const;

    void setRegExp(const QRegExp& rx);
    const QRegExp& regExp() const { return r; }


private:
    Q_DISABLE_COPY(QRegExpValidator)

    QRegExp r;
};


#endif // QT_NO_VALIDATOR

QT_END_NAMESPACE


#endif // QVALIDATOR_H
