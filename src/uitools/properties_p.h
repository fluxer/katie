/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Katie Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef UILIBPROPERTIES_H
#define UILIBPROPERTIES_H

#include <QtCore/QObject>
#include <QtCore/qmetaobject.h>
#include <QtCore/QLocale>
#include <QtCore/QCoreApplication>
#include <QtGui/QWidget>

#include "formbuilderextra_p.h"

QT_BEGIN_NAMESPACE

class QAbstractFormBuilder;
class DomProperty;

Q_UITOOLS_EXPORT DomProperty *variantToDomProperty(QAbstractFormBuilder *abstractFormBuilder, const QMetaObject *meta, const QString &propertyName, const QVariant &value);


Q_UITOOLS_EXPORT QVariant domPropertyToVariant(const DomProperty *property);
Q_UITOOLS_EXPORT QVariant domPropertyToVariant(QAbstractFormBuilder *abstractFormBuilder, const QMetaObject *meta, const  DomProperty *property);

// This class exists to provide meta information
// for enumerations only.
class QAbstractFormBuilderGadget: public QWidget
{
    Q_OBJECT
    Q_PROPERTY(Qt::ItemFlags itemFlags READ fakeItemFlags)
    Q_PROPERTY(Qt::CheckState checkState READ fakeCheckState)
    Q_PROPERTY(Qt::Alignment textAlignment READ fakeAlignment)
    Q_PROPERTY(Qt::Orientation orientation READ fakeOrientation)
    Q_PROPERTY(QSizePolicy::Policy sizeType READ fakeSizeType)
    Q_PROPERTY(QPalette::ColorRole colorRole READ fakeColorRole)
    Q_PROPERTY(QPalette::ColorGroup colorGroup READ fakeColorGroup)
    Q_PROPERTY(Qt::CursorShape cursorShape READ fakeCursorShape)
    Q_PROPERTY(Qt::BrushStyle brushStyle READ fakeBrushStyle)
    Q_PROPERTY(Qt::ToolBarArea toolBarArea READ fakeToolBarArea)
    Q_PROPERTY(QGradient::Type gradientType READ fakeGradientType)
    Q_PROPERTY(QGradient::Spread gradientSpread READ fakeGradientSpread)
    Q_PROPERTY(QGradient::CoordinateMode gradientCoordinate READ fakeGradientCoordinate)
    Q_PROPERTY(QLocale::Language language READ fakeLanguage)
    Q_PROPERTY(QLocale::Country country READ fakeCountry)
public:
    QAbstractFormBuilderGadget() { Q_ASSERT(false); }

    Qt::Orientation fakeOrientation() const     { Q_ASSERT(false); return Qt::Horizontal; }
    QSizePolicy::Policy fakeSizeType() const    { Q_ASSERT(false); return QSizePolicy::Expanding; }
    QPalette::ColorGroup fakeColorGroup() const { Q_ASSERT(false); return static_cast<QPalette::ColorGroup>(0); }
    QPalette::ColorRole fakeColorRole() const   { Q_ASSERT(false); return static_cast<QPalette::ColorRole>(0); }
    Qt::CursorShape fakeCursorShape() const     { Q_ASSERT(false); return Qt::ArrowCursor; }
    Qt::BrushStyle fakeBrushStyle() const       { Q_ASSERT(false); return Qt::NoBrush; }
    Qt::ToolBarArea fakeToolBarArea() const {  Q_ASSERT(false); return Qt::NoToolBarArea; }
    QGradient::Type fakeGradientType() const    { Q_ASSERT(false); return QGradient::NoGradient; }
    QGradient::Spread fakeGradientSpread() const  { Q_ASSERT(false); return QGradient::PadSpread; }
    QGradient::CoordinateMode fakeGradientCoordinate() const  { Q_ASSERT(false); return QGradient::LogicalMode; }
    QLocale::Language fakeLanguage() const  { Q_ASSERT(false); return QLocale::C; }
    QLocale::Country fakeCountry() const  { Q_ASSERT(false); return QLocale::AnyCountry; }
    Qt::ItemFlags fakeItemFlags() const         { Q_ASSERT(false); return Qt::NoItemFlags; }
    Qt::CheckState fakeCheckState() const       { Q_ASSERT(false); return Qt::Unchecked; }
    Qt::Alignment fakeAlignment() const         { Q_ASSERT(false); return Qt::AlignLeft; }
};

// Convert key to value for a given QMetaEnum
template <class EnumType>
inline EnumType enumKeyToValue(const QMetaEnum &metaEnum,const char *key, const EnumType* = 0)
{
    int val = metaEnum.keyToValue(key);
    if (val == -1) {

        uiLibWarning(QCoreApplication::translate("QFormBuilder", "The enumeration-value '%1' is invalid. The default value '%2' will be used instead.")
                    .arg(QString::fromUtf8(key)).arg(QString::fromUtf8(metaEnum.key(0))));
        val = metaEnum.value(0);
    }
    return static_cast<EnumType>(val);
}

// Convert keys to value for a given QMetaEnum
template <class EnumType>
inline EnumType enumKeysToValue(const QMetaEnum &metaEnum,const char *keys, const EnumType* = 0)
{
    int val = metaEnum.keysToValue(keys);
    if (val == -1) {

        uiLibWarning(QCoreApplication::translate("QFormBuilder", "The flag-value '%1' is invalid. Zero will be used instead.")
                    .arg(QString::fromUtf8(keys)));
        val = 0;
    }
    return static_cast<EnumType>(QFlag(val));
}

// Access meta enumeration object of a qobject
template <class QObjectType>
inline QMetaEnum metaEnum(const char *name, const QObjectType* = 0)
{
    const int e_index = QObjectType::staticMetaObject.indexOfProperty(name);
    Q_ASSERT(e_index != -1);
    return QObjectType::staticMetaObject.property(e_index).enumerator();
}

// Convert key to value for enumeration by name
template <class QObjectType, class EnumType>
inline EnumType enumKeyOfObjectToValue(const char *enumName, const char *key, const QObjectType* = 0, const EnumType* = 0)
{
    const QMetaEnum me = metaEnum<QObjectType>(enumName);
    return enumKeyToValue<EnumType>(me, key);
}

QT_END_NAMESPACE

#endif // UILIBPROPERTIES_H
