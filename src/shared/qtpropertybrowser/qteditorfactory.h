/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

#ifndef QTEDITORFACTORY_H
#define QTEDITORFACTORY_H

#include "qtpropertymanager.h"

QT_BEGIN_NAMESPACE

class QtSpinBoxFactoryPrivate;

class QtSpinBoxFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    QtSpinBoxFactory(QObject *parent = nullptr);
    ~QtSpinBoxFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager *manager);
    QWidget *createEditor(QtIntPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtIntPropertyManager *manager);
private:
    QScopedPointer<QtSpinBoxFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSpinBoxFactory)
    Q_DISABLE_COPY(QtSpinBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtSliderFactoryPrivate;

class QtSliderFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    QtSliderFactory(QObject *parent = nullptr);
    ~QtSliderFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager *manager);
    QWidget *createEditor(QtIntPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtIntPropertyManager *manager);
private:
    QScopedPointer<QtSliderFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSliderFactory)
    Q_DISABLE_COPY(QtSliderFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtScrollBarFactoryPrivate;

class QtScrollBarFactory : public QtAbstractEditorFactory<QtIntPropertyManager>
{
    Q_OBJECT
public:
    QtScrollBarFactory(QObject *parent = nullptr);
    ~QtScrollBarFactory();
protected:
    void connectPropertyManager(QtIntPropertyManager *manager);
    QWidget *createEditor(QtIntPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtIntPropertyManager *manager);
private:
    QScopedPointer<QtScrollBarFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtScrollBarFactory)
    Q_DISABLE_COPY(QtScrollBarFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, int, int))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtCheckBoxFactoryPrivate;

class QtCheckBoxFactory : public QtAbstractEditorFactory<QtBoolPropertyManager>
{
    Q_OBJECT
public:
    QtCheckBoxFactory(QObject *parent = nullptr);
    ~QtCheckBoxFactory();
protected:
    void connectPropertyManager(QtBoolPropertyManager *manager);
    QWidget *createEditor(QtBoolPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtBoolPropertyManager *manager);
private:
    QScopedPointer<QtCheckBoxFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCheckBoxFactory)
    Q_DISABLE_COPY(QtCheckBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, bool))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(bool))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtDoubleSpinBoxFactoryPrivate;

class QtDoubleSpinBoxFactory : public QtAbstractEditorFactory<QtDoublePropertyManager>
{
    Q_OBJECT
public:
    QtDoubleSpinBoxFactory(QObject *parent = nullptr);
    ~QtDoubleSpinBoxFactory();
protected:
    void connectPropertyManager(QtDoublePropertyManager *manager);
    QWidget *createEditor(QtDoublePropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtDoublePropertyManager *manager);
private:
    QScopedPointer<QtDoubleSpinBoxFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDoubleSpinBoxFactory)
    Q_DISABLE_COPY(QtDoubleSpinBoxFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *, double, double))
    Q_PRIVATE_SLOT(d_func(), void slotSingleStepChanged(QtProperty *, double))
    Q_PRIVATE_SLOT(d_func(), void slotDecimalsChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(double))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtLineEditFactoryPrivate;

class QtLineEditFactory : public QtAbstractEditorFactory<QtStringPropertyManager>
{
    Q_OBJECT
public:
    QtLineEditFactory(QObject *parent = nullptr);
    ~QtLineEditFactory();
protected:
    void connectPropertyManager(QtStringPropertyManager *manager);
    QWidget *createEditor(QtStringPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtStringPropertyManager *manager);
private:
    QScopedPointer<QtLineEditFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtLineEditFactory)
    Q_DISABLE_COPY(QtLineEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QString &))
    Q_PRIVATE_SLOT(d_func(), void slotRegExpChanged(QtProperty *, const QRegExp &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QString &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtDateEditFactoryPrivate;

class QtDateEditFactory : public QtAbstractEditorFactory<QtDatePropertyManager>
{
    Q_OBJECT
public:
    QtDateEditFactory(QObject *parent = nullptr);
    ~QtDateEditFactory();
protected:
    void connectPropertyManager(QtDatePropertyManager *manager);
    QWidget *createEditor(QtDatePropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtDatePropertyManager *manager);
private:
    QScopedPointer<QtDateEditFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDateEditFactory)
    Q_DISABLE_COPY(QtDateEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QDate &))
    Q_PRIVATE_SLOT(d_func(), void slotRangeChanged(QtProperty *,
                        const QDate &, const QDate &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QDate &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtTimeEditFactoryPrivate;

class QtTimeEditFactory : public QtAbstractEditorFactory<QtTimePropertyManager>
{
    Q_OBJECT
public:
    QtTimeEditFactory(QObject *parent = nullptr);
    ~QtTimeEditFactory();
protected:
    void connectPropertyManager(QtTimePropertyManager *manager);
    QWidget *createEditor(QtTimePropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtTimePropertyManager *manager);
private:
    QScopedPointer<QtTimeEditFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTimeEditFactory)
    Q_DISABLE_COPY(QtTimeEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QTime &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QTime &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtDateTimeEditFactoryPrivate;

class QtDateTimeEditFactory : public QtAbstractEditorFactory<QtDateTimePropertyManager>
{
    Q_OBJECT
public:
    QtDateTimeEditFactory(QObject *parent = nullptr);
    ~QtDateTimeEditFactory();
protected:
    void connectPropertyManager(QtDateTimePropertyManager *manager);
    QWidget *createEditor(QtDateTimePropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtDateTimePropertyManager *manager);
private:
    QScopedPointer<QtDateTimeEditFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDateTimeEditFactory)
    Q_DISABLE_COPY(QtDateTimeEditFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QDateTime &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QDateTime &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtKeySequenceEditorFactoryPrivate;

class QtKeySequenceEditorFactory : public QtAbstractEditorFactory<QtKeySequencePropertyManager>
{
    Q_OBJECT
public:
    QtKeySequenceEditorFactory(QObject *parent = nullptr);
    ~QtKeySequenceEditorFactory();
protected:
    void connectPropertyManager(QtKeySequencePropertyManager *manager);
    QWidget *createEditor(QtKeySequencePropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtKeySequencePropertyManager *manager);
private:
    QScopedPointer<QtKeySequenceEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtKeySequenceEditorFactory)
    Q_DISABLE_COPY(QtKeySequenceEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QKeySequence &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QKeySequence &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtCharEditorFactoryPrivate;

class QtCharEditorFactory : public QtAbstractEditorFactory<QtCharPropertyManager>
{
    Q_OBJECT
public:
    QtCharEditorFactory(QObject *parent = nullptr);
    ~QtCharEditorFactory();
protected:
    void connectPropertyManager(QtCharPropertyManager *manager);
    QWidget *createEditor(QtCharPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtCharPropertyManager *manager);
private:
    QScopedPointer<QtCharEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCharEditorFactory)
    Q_DISABLE_COPY(QtCharEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QChar &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QChar &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtEnumEditorFactoryPrivate;

class QtEnumEditorFactory : public QtAbstractEditorFactory<QtEnumPropertyManager>
{
    Q_OBJECT
public:
    QtEnumEditorFactory(QObject *parent = nullptr);
    ~QtEnumEditorFactory();
protected:
    void connectPropertyManager(QtEnumPropertyManager *manager);
    QWidget *createEditor(QtEnumPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtEnumPropertyManager *manager);
private:
    QScopedPointer<QtEnumEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtEnumEditorFactory)
    Q_DISABLE_COPY(QtEnumEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotEnumNamesChanged(QtProperty *,
                        const QStringList &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumIconsChanged(QtProperty *,
                        const QMap<int, QIcon> &))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtCursorEditorFactoryPrivate;

class QtCursorEditorFactory : public QtAbstractEditorFactory<QtCursorPropertyManager>
{
    Q_OBJECT
public:
    QtCursorEditorFactory(QObject *parent = nullptr);
    ~QtCursorEditorFactory();
protected:
    void connectPropertyManager(QtCursorPropertyManager *manager);
    QWidget *createEditor(QtCursorPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtCursorPropertyManager *manager);
private:
    QScopedPointer<QtCursorEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCursorEditorFactory)
    Q_DISABLE_COPY(QtCursorEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QCursor &))
    Q_PRIVATE_SLOT(d_func(), void slotEnumChanged(QtProperty *, int))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
};

class QtColorEditorFactoryPrivate;

class QtColorEditorFactory : public QtAbstractEditorFactory<QtColorPropertyManager>
{
    Q_OBJECT
public:
    QtColorEditorFactory(QObject *parent = nullptr);
    ~QtColorEditorFactory();
protected:
    void connectPropertyManager(QtColorPropertyManager *manager);
    QWidget *createEditor(QtColorPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtColorPropertyManager *manager);
private:
    QScopedPointer<QtColorEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtColorEditorFactory)
    Q_DISABLE_COPY(QtColorEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QColor &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QColor &))
};

class QtFontEditorFactoryPrivate;

class QtFontEditorFactory : public QtAbstractEditorFactory<QtFontPropertyManager>
{
    Q_OBJECT
public:
    QtFontEditorFactory(QObject *parent = nullptr);
    ~QtFontEditorFactory();
protected:
    void connectPropertyManager(QtFontPropertyManager *manager);
    QWidget *createEditor(QtFontPropertyManager *manager, QtProperty *property,
                QWidget *parent);
    void disconnectPropertyManager(QtFontPropertyManager *manager);
private:
    QScopedPointer<QtFontEditorFactoryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtFontEditorFactory)
    Q_DISABLE_COPY(QtFontEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const QFont &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const QFont &))
};

QT_END_NAMESPACE

#endif
