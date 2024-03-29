/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEVALUETYPE_P_H
#define QDECLARATIVEVALUETYPE_P_H

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

#include "qdeclarativeproperty.h"
#include "qdeclarativeproperty_p.h"
#include "qdeclarativenullablevalue_p_p.h"

#include <QtCore/qobject.h>
#include <QtCore/qrect.h>
#include <QtCore/qeasingcurve.h>
#include <QtCore/qvariant.h>
#include <QtGui/qvector2d.h>
#include <QtGui/qvector3d.h>
#include <QtGui/qvector4d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qfont.h>

QT_BEGIN_NAMESPACE

class Q_DECLARATIVE_PRIVATE_EXPORT QDeclarativeValueType : public QObject
{
    Q_OBJECT
public:
    QDeclarativeValueType(QObject *parent = nullptr);
    virtual void read(QObject *, int) = 0;
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags flags) = 0;
    virtual QVariant value() = 0;
    virtual void setValue(QVariant) = 0;
};

class Q_DECLARATIVE_PRIVATE_EXPORT QDeclarativeValueTypeFactory
{
public:
    QDeclarativeValueTypeFactory();
    ~QDeclarativeValueTypeFactory();
    static bool isValueType(int);
    static QDeclarativeValueType *valueType(int);

    static void registerValueTypes();

    QDeclarativeValueType *operator[](int idx) const {
        if (idx < 0 || idx >= (int)QVariant::UserType) return 0;
        else return valueTypes[idx];
    }

private:
    QDeclarativeValueType *valueTypes[QVariant::UserType - 1]; 
};

class Q_AUTOTEST_EXPORT QDeclarativePointFValueType : public QDeclarativeValueType
{
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_OBJECT
public:
    QDeclarativePointFValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    qreal x() const;
    qreal y() const;
    void setX(qreal);
    void setY(qreal);

private:
    QPointF point;
};

class Q_AUTOTEST_EXPORT QDeclarativePointValueType : public QDeclarativeValueType
{
    Q_PROPERTY(int x READ x WRITE setX)
    Q_PROPERTY(int y READ y WRITE setY)
    Q_OBJECT
public:
    QDeclarativePointValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    int x() const;
    int y() const;
    void setX(int);
    void setY(int);

private:
    QPoint point;
};

class Q_AUTOTEST_EXPORT QDeclarativeSizeFValueType : public QDeclarativeValueType
{
    Q_PROPERTY(qreal width READ width WRITE setWidth)
    Q_PROPERTY(qreal height READ height WRITE setHeight)
    Q_OBJECT
public:
    QDeclarativeSizeFValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    qreal width() const;
    qreal height() const;
    void setWidth(qreal);
    void setHeight(qreal);

private:
    QSizeF size;
};

class Q_AUTOTEST_EXPORT QDeclarativeSizeValueType : public QDeclarativeValueType
{
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_OBJECT
public:
    QDeclarativeSizeValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    int width() const;
    int height() const;
    void setWidth(int);
    void setHeight(int);

private:
    QSize size;
};

class Q_AUTOTEST_EXPORT QDeclarativeRectFValueType : public QDeclarativeValueType
{
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_PROPERTY(qreal width READ width WRITE setWidth)
    Q_PROPERTY(qreal height READ height WRITE setHeight)
    Q_OBJECT
public:
    QDeclarativeRectFValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    qreal x() const;
    qreal y() const;
    void setX(qreal);
    void setY(qreal);
    
    qreal width() const;
    qreal height() const;
    void setWidth(qreal);
    void setHeight(qreal);

private:
    QRectF rect;
};

class Q_AUTOTEST_EXPORT QDeclarativeRectValueType : public QDeclarativeValueType
{
    Q_PROPERTY(int x READ x WRITE setX)
    Q_PROPERTY(int y READ y WRITE setY)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_OBJECT
public:
    QDeclarativeRectValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    int x() const;
    int y() const;
    void setX(int);
    void setY(int);
    
    int width() const;
    int height() const;
    void setWidth(int);
    void setHeight(int);

private:
    QRect rect;
};

#ifndef QT_NO_VECTOR2D
class Q_AUTOTEST_EXPORT QDeclarativeVector2DValueType : public QDeclarativeValueType
{
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_OBJECT
public:
    QDeclarativeVector2DValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    qreal x() const;
    qreal y() const;
    void setX(qreal);
    void setY(qreal);

private:
    QVector2D vector;
};
#endif // QT_NO_VECTOR2D

#ifndef QT_NO_VECTOR3D
class Q_AUTOTEST_EXPORT QDeclarativeVector3DValueType : public QDeclarativeValueType
{
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_PROPERTY(qreal z READ z WRITE setZ)
    Q_OBJECT
public:
    QDeclarativeVector3DValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    qreal x() const;
    qreal y() const;
    qreal z() const;
    void setX(qreal);
    void setY(qreal);
    void setZ(qreal);

private:
    QVector3D vector;
};
#endif // QT_NO_VECTOR3D

#ifndef QT_NO_VECTOR4D
class Q_AUTOTEST_EXPORT QDeclarativeVector4DValueType : public QDeclarativeValueType
{
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_PROPERTY(qreal z READ z WRITE setZ)
    Q_PROPERTY(qreal w READ w WRITE setW)
    Q_OBJECT
public:
    QDeclarativeVector4DValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    qreal x() const;
    qreal y() const;
    qreal z() const;
    qreal w() const;
    void setX(qreal);
    void setY(qreal);
    void setZ(qreal);
    void setW(qreal);

private:
    QVector4D vector;
};
#endif // QT_NO_VECTOR4D

class Q_AUTOTEST_EXPORT QDeclarativeMatrix4x4ValueType : public QDeclarativeValueType
{
    Q_PROPERTY(qreal m11 READ m11 WRITE setM11)
    Q_PROPERTY(qreal m12 READ m12 WRITE setM12)
    Q_PROPERTY(qreal m13 READ m13 WRITE setM13)
    Q_PROPERTY(qreal m14 READ m14 WRITE setM14)
    Q_PROPERTY(qreal m21 READ m21 WRITE setM21)
    Q_PROPERTY(qreal m22 READ m22 WRITE setM22)
    Q_PROPERTY(qreal m23 READ m23 WRITE setM23)
    Q_PROPERTY(qreal m24 READ m24 WRITE setM24)
    Q_PROPERTY(qreal m31 READ m31 WRITE setM31)
    Q_PROPERTY(qreal m32 READ m32 WRITE setM32)
    Q_PROPERTY(qreal m33 READ m33 WRITE setM33)
    Q_PROPERTY(qreal m34 READ m34 WRITE setM34)
    Q_PROPERTY(qreal m41 READ m41 WRITE setM41)
    Q_PROPERTY(qreal m42 READ m42 WRITE setM42)
    Q_PROPERTY(qreal m43 READ m43 WRITE setM43)
    Q_PROPERTY(qreal m44 READ m44 WRITE setM44)
    Q_OBJECT
public:
    QDeclarativeMatrix4x4ValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    qreal m11() const { return matrix(0, 0); }
    qreal m12() const { return matrix(0, 1); }
    qreal m13() const { return matrix(0, 2); }
    qreal m14() const { return matrix(0, 3); }
    qreal m21() const { return matrix(1, 0); }
    qreal m22() const { return matrix(1, 1); }
    qreal m23() const { return matrix(1, 2); }
    qreal m24() const { return matrix(1, 3); }
    qreal m31() const { return matrix(2, 0); }
    qreal m32() const { return matrix(2, 1); }
    qreal m33() const { return matrix(2, 2); }
    qreal m34() const { return matrix(2, 3); }
    qreal m41() const { return matrix(3, 0); }
    qreal m42() const { return matrix(3, 1); }
    qreal m43() const { return matrix(3, 2); }
    qreal m44() const { return matrix(3, 3); }

    void setM11(qreal value) { matrix(0, 0) = value; }
    void setM12(qreal value) { matrix(0, 1) = value; }
    void setM13(qreal value) { matrix(0, 2) = value; }
    void setM14(qreal value) { matrix(0, 3) = value; }
    void setM21(qreal value) { matrix(1, 0) = value; }
    void setM22(qreal value) { matrix(1, 1) = value; }
    void setM23(qreal value) { matrix(1, 2) = value; }
    void setM24(qreal value) { matrix(1, 3) = value; }
    void setM31(qreal value) { matrix(2, 0) = value; }
    void setM32(qreal value) { matrix(2, 1) = value; }
    void setM33(qreal value) { matrix(2, 2) = value; }
    void setM34(qreal value) { matrix(2, 3) = value; }
    void setM41(qreal value) { matrix(3, 0) = value; }
    void setM42(qreal value) { matrix(3, 1) = value; }
    void setM43(qreal value) { matrix(3, 2) = value; }
    void setM44(qreal value) { matrix(3, 3) = value; }

private:
    QMatrix4x4 matrix;
};

class Q_AUTOTEST_EXPORT QDeclarativeEasingValueType : public QDeclarativeValueType
{
    Q_OBJECT
    Q_ENUMS(Type)

    Q_PROPERTY(QDeclarativeEasingValueType::Type type READ type WRITE setType)
    Q_PROPERTY(qreal amplitude READ amplitude WRITE setAmplitude)
    Q_PROPERTY(qreal overshoot READ overshoot WRITE setOvershoot)
    Q_PROPERTY(qreal period READ period WRITE setPeriod)
public:
    enum Type {
        Linear = QEasingCurve::Linear,
        InQuad = QEasingCurve::InQuad, OutQuad = QEasingCurve::OutQuad,
        InOutQuad = QEasingCurve::InOutQuad, OutInQuad = QEasingCurve::OutInQuad,
        InCubic = QEasingCurve::InCubic, OutCubic = QEasingCurve::OutCubic,
        InOutCubic = QEasingCurve::InOutCubic, OutInCubic = QEasingCurve::OutInCubic,
        InQuart = QEasingCurve::InQuart, OutQuart = QEasingCurve::OutQuart,
        InOutQuart = QEasingCurve::InOutQuart, OutInQuart = QEasingCurve::OutInQuart,
        InQuint = QEasingCurve::InQuint, OutQuint = QEasingCurve::OutQuint,
        InOutQuint = QEasingCurve::InOutQuint, OutInQuint = QEasingCurve::OutInQuint,
        InSine = QEasingCurve::InSine, OutSine = QEasingCurve::OutSine,
        InOutSine = QEasingCurve::InOutSine, OutInSine = QEasingCurve::OutInSine,
        InExpo = QEasingCurve::InExpo, OutExpo = QEasingCurve::OutExpo,
        InOutExpo = QEasingCurve::InOutExpo, OutInExpo = QEasingCurve::OutInExpo,
        InCirc = QEasingCurve::InCirc, OutCirc = QEasingCurve::OutCirc,
        InOutCirc = QEasingCurve::InOutCirc, OutInCirc = QEasingCurve::OutInCirc,
        InElastic = QEasingCurve::InElastic, OutElastic = QEasingCurve::OutElastic,
        InOutElastic = QEasingCurve::InOutElastic, OutInElastic = QEasingCurve::OutInElastic,
        InBack = QEasingCurve::InBack, OutBack = QEasingCurve::OutBack,
        InOutBack = QEasingCurve::InOutBack, OutInBack = QEasingCurve::OutInBack,
        InBounce = QEasingCurve::InBounce, OutBounce = QEasingCurve::OutBounce,
        InOutBounce = QEasingCurve::InOutBounce, OutInBounce = QEasingCurve::OutInBounce,
        InCurve = QEasingCurve::InCurve, OutCurve = QEasingCurve::OutCurve,
        SineCurve = QEasingCurve::SineCurve, CosineCurve = QEasingCurve::CosineCurve
    };

    QDeclarativeEasingValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    Type type() const;
    qreal amplitude() const;
    qreal overshoot() const;
    qreal period() const;
    void setType(Type);
    void setAmplitude(qreal);
    void setOvershoot(qreal);
    void setPeriod(qreal);

private:
    QEasingCurve easing;
};

class Q_AUTOTEST_EXPORT QDeclarativeFontValueType : public QDeclarativeValueType
{
    Q_OBJECT
    Q_ENUMS(FontWeight)

    Q_PROPERTY(QString family READ family WRITE setFamily)
    Q_PROPERTY(bool bold READ bold WRITE setBold)
    Q_PROPERTY(FontWeight weight READ weight WRITE setWeight)
    Q_PROPERTY(bool italic READ italic WRITE setItalic)
    Q_PROPERTY(bool underline READ underline WRITE setUnderline)
    Q_PROPERTY(bool overline READ overline WRITE setOverline)
    Q_PROPERTY(bool strikeout READ strikeout WRITE setStrikeout)
    Q_PROPERTY(qreal pointSize READ pointSize WRITE setPointSize)
    Q_PROPERTY(int pixelSize READ pixelSize WRITE setPixelSize)

public:
    enum FontWeight { Light = QFont::Light,
                       Normal = QFont::Normal,
                       DemiBold = QFont::DemiBold,
                       Bold = QFont::Bold,
                       Black = QFont::Black };

    QDeclarativeFontValueType(QObject *parent = nullptr);

    virtual void read(QObject *, int);
    virtual void write(QObject *, int, QDeclarativePropertyPrivate::WriteFlags);
    virtual QVariant value();
    virtual void setValue(QVariant value);

    QString family() const;
    void setFamily(const QString &);

    bool bold() const;
    void setBold(bool b);

    FontWeight weight() const;
    void setWeight(FontWeight);

    bool italic() const;
    void setItalic(bool b);

    bool underline() const;
    void setUnderline(bool b);

    bool overline() const;
    void setOverline(bool b);

    bool strikeout() const;
    void setStrikeout(bool b);

    qreal pointSize() const;
    void setPointSize(qreal size);

    int pixelSize() const;
    void setPixelSize(int size);

private:
    QFont font;
    bool pixelSizeSet;
    bool pointSizeSet;
    mutable QDeclarativeNullableValue<int> dpi;
};

QT_END_NAMESPACE

#endif  // QDECLARATIVEVALUETYPE_P_H
