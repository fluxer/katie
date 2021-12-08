/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QVARIANT_H
#define QVARIANT_H

#include <QtCore/qmetatype.h>
#include <QtCore/qhash.h>
#include <QtCore/qmap.h>


QT_BEGIN_NAMESPACE


class QBitArray;
class QDataStream;
class QDate;
class QDateTime;
#ifndef QT_BOOTSTRAPPED
class QEasingCurve;
class QJsonDocument;
#endif
class QLine;
class QLineF;
class QLocale;
class QMatrix;
class QTransform;
class QStringList;
class QTime;
class QPoint;
class QPointF;
class QSize;
class QSizeF;
class QRect;
class QRectF;
#ifndef QT_NO_REGEXP
class QRegExp;
#endif
class QTextFormat;
class QTextLength;
class QUrl;
class QVariant;
class QDebug;

template <typename T>
inline QVariant qVariantFromValue(const T &);

template<typename T>
inline T qvariant_cast(const QVariant &);

class Q_CORE_EXPORT QVariant
{
 public:
    enum Type {
        Invalid = QMetaType::Void,

        Bool = QMetaType::Bool,
        Int = QMetaType::Int,
        UInt = QMetaType::UInt,
        LongLong = QMetaType::LongLong,
        ULongLong = QMetaType::ULongLong,
        Double = QMetaType::Double,
        Float = QMetaType::Float,
        Char = QMetaType::QChar,
        Map = QMetaType::QVariantMap,
        List = QMetaType::QVariantList,
        String = QMetaType::QString,
        StringList = QMetaType::QStringList,
        ByteArray = QMetaType::QByteArray,
        BitArray = QMetaType::QBitArray,
        Date = QMetaType::QDate,
        Time = QMetaType::QTime,
        DateTime = QMetaType::QDateTime,
        Url = QMetaType::QUrl,
        Locale = QMetaType::QLocale,
        Rect = QMetaType::QRect,
        RectF = QMetaType::QRectF,
        Size = QMetaType::QSize,
        SizeF = QMetaType::QSizeF,
        Line = QMetaType::QLine,
        LineF = QMetaType::QLineF,
        Point = QMetaType::QPoint,
        PointF = QMetaType::QPointF,
        RegExp = QMetaType::QRegExp,
        Hash = QMetaType::QVariantHash,
        EasingCurve = QMetaType::QEasingCurve,
        JsonDocument = QMetaType::QJsonDocument,
        LastCoreType = QMetaType::LastCoreType,

        Font = QMetaType::QFont,
        Pixmap = QMetaType::QPixmap,
        Brush = QMetaType::QBrush,
        Color = QMetaType::QColor,
        Palette = QMetaType::QPalette,
        Icon = QMetaType::QIcon,
        Image = QMetaType::QImage,
        Polygon = QMetaType::QPolygon,
        Region = QMetaType::QRegion,
        Bitmap = QMetaType::QBitmap,
        Cursor = QMetaType::QCursor,
        SizePolicy = QMetaType::QSizePolicy,
        KeySequence = QMetaType::QKeySequence,
        Pen = QMetaType::QPen,
        TextLength = QMetaType::QTextLength,
        TextFormat = QMetaType::QTextFormat,
        Matrix = QMetaType::QMatrix,
        Transform = QMetaType::QTransform,
        Matrix4x4 = QMetaType::QMatrix4x4,
        Vector2D = QMetaType::QVector2D,
        Vector3D = QMetaType::QVector3D,
        Vector4D = QMetaType::QVector4D,
        Quaternion = QMetaType::QQuaternion,
        LastGuiType = QMetaType::LastGuiType,

        UserType = 127,
        LastType = 0xffffffff // need this so that gcc >= 3.4 allocates 32 bits for Type
    };

    inline QVariant() {}
    ~QVariant();
    QVariant(Type type);
    QVariant(int typeOrUserType, const void *copy);
    QVariant(const QVariant &other);

#ifndef QT_NO_DATASTREAM
    QVariant(QDataStream &s);
#endif

    QVariant(int i);
    QVariant(uint ui);
    QVariant(qlonglong ll);
    QVariant(qulonglong ull);
    QVariant(bool b);
    QVariant(double d);
    QVariant(float f);
#ifndef QT_NO_CAST_FROM_ASCII
    QT_ASCII_CAST_WARN_CONSTRUCTOR QVariant(const char *str);
#endif

    QVariant(const QByteArray &bytearray);
    QVariant(const QBitArray &bitarray);
    QVariant(const QString &string);
    QVariant(const QLatin1String &string);
    QVariant(const QStringList &stringlist);
    QVariant(const QChar &qchar);
    QVariant(const QDate &date);
    QVariant(const QTime &time);
    QVariant(const QDateTime &datetime);
    QVariant(const QList<QVariant> &list);
    QVariant(const QMap<QString,QVariant> &map);
    QVariant(const QHash<QString,QVariant> &hash);
#ifndef QT_BOOTSTRAPPED
    QVariant(const QSize &size);
    QVariant(const QSizeF &size);
    QVariant(const QPoint &pt);
    QVariant(const QPointF &pt);
    QVariant(const QLine &line);
    QVariant(const QLineF &line);
    QVariant(const QRect &rect);
    QVariant(const QRectF &rect);
#endif
    QVariant(const QUrl &url);
    QVariant(const QLocale &locale);
#ifndef QT_NO_REGEXP
    QVariant(const QRegExp &regExp);
#endif
#ifndef QT_BOOTSTRAPPED
    QVariant(const QEasingCurve &easing);
    QVariant(const QJsonDocument &jsonDocument);
#endif

    QVariant& operator=(const QVariant &other);
#ifdef Q_COMPILER_RVALUE_REFS
    inline QVariant &operator=(QVariant &&other)
    { qSwap(d, other.d); return *this; }
#endif

    inline void swap(QVariant &other) { qSwap(d, other.d); }

    Type type() const;
    int userType() const;
    const char *typeName() const;

    bool canConvert(Type t) const;
    bool convert(Type t);


    inline bool isValid() const { return d.type != Invalid; }
    bool isNull() const;

    void clear();

    void detach();

    int toInt(bool *ok = nullptr) const;
    uint toUInt(bool *ok = nullptr) const;
    qlonglong toLongLong(bool *ok = nullptr) const;
    qulonglong toULongLong(bool *ok = nullptr) const;
    bool toBool() const;
    double toDouble(bool *ok = nullptr) const;
    float toFloat(bool *ok = nullptr) const;
    qreal toReal(bool *ok = nullptr) const;
    QByteArray toByteArray() const;
    QBitArray toBitArray() const;
    QString toString() const;
    QStringList toStringList() const;
    QChar toChar() const;
    QDate toDate() const;
    QTime toTime() const;
    QDateTime toDateTime() const;
    QList<QVariant> toList() const;
    QMap<QString, QVariant> toMap() const;
    QHash<QString, QVariant> toHash() const;

#ifndef QT_BOOTSTRAPPED
    QPoint toPoint() const;
    QPointF toPointF() const;
    QRect toRect() const;
    QSize toSize() const;
    QSizeF toSizeF() const;
    QLine toLine() const;
    QLineF toLineF() const;
    QRectF toRectF() const;
#endif
    QUrl toUrl() const;
    QLocale toLocale() const;
#ifndef QT_NO_REGEXP
    QRegExp toRegExp() const;
#endif
#ifndef QT_BOOTSTRAPPED
    QEasingCurve toEasingCurve() const;
    QJsonDocument toJsonDocument() const;
#endif


#ifndef QT_NO_DATASTREAM
    void load(QDataStream &ds);
    void save(QDataStream &ds) const;
#endif
    static const char *typeToName(Type type);
    static Type nameToType(const char *name);

    void *data();
    const void *constData() const;
    inline const void *data() const { return constData(); }

    template<typename T>
    inline void setValue(const T &value);

    template<typename T>
    inline T value() const
    { return qvariant_cast<T>(*this); }

    template<typename T>
    static inline QVariant fromValue(const T &value)
    { return qVariantFromValue(value); }

    template<typename T>
    bool canConvert() const
    { return canConvert(Type(qMetaTypeId<T>())); }

 public:
    struct PrivateShared
    {
        inline PrivateShared(void *v) : ptr(v), ref(1) { }
        void *ptr;
        QAtomicInt ref;
    };
    struct Private
    {
        inline Private(): type(Invalid), is_shared(false), is_null(true) { data.ptr = nullptr; }
        inline Private(const Private &other)
            : data(other.data), type(other.type),
              is_shared(other.is_shared), is_null(other.is_null)
        {}
        union Data
        {
            int i;
            uint u;
            bool b;
            double d;
            float f;
            qlonglong ll;
            qulonglong ull;
            QObject *o;
            void *ptr;
            PrivateShared *shared;
        } data;
        int type;
        bool is_shared;
        bool is_null;
    };
 public:
    typedef void (*f_construct)(Private *, const void *);
    typedef void (*f_clear)(Private *);
    typedef bool (*f_null)(const Private *);
#ifndef QT_NO_DATASTREAM
    typedef void (*f_load)(Private *, QDataStream &);
    typedef void (*f_save)(const Private *, QDataStream &);
#endif
    typedef bool (*f_compare)(const Private *, const Private *);
    typedef bool (*f_convert)(const QVariant::Private *d, int t, void *, bool *);
    typedef void (*f_debugStream)(QDebug, const QVariant &);
    struct Handler {
        f_construct construct;
        f_clear clear;
        f_null isNull;
#ifndef QT_NO_DATASTREAM
        f_load load;
        f_save save;
#endif
        f_compare compare;
        f_convert convert;
        f_debugStream debugStream;
    };

    inline bool operator==(const QVariant &v) const
    { return cmp(v); }
    inline bool operator!=(const QVariant &v) const
    { return !cmp(v); }

protected:
    friend inline bool qvariant_cast_helper(const QVariant &, QVariant::Type, void *);
    friend int qRegisterGuiVariant();
    friend int qUnregisterGuiVariant();
#ifndef QT_NO_DEBUG_STREAM
    friend Q_CORE_EXPORT QDebug operator<<(QDebug, const QVariant &);
#endif
    Private d;

    static const Handler *handler;

    void create(int type, const void *copy);
    bool cmp(const QVariant &other) const;

private:
    // force compile error, prevent QVariant(bool) to be called
    inline QVariant(void *) { Q_ASSERT(false); }
#ifdef QT_NO_CAST_FROM_ASCII
    // force compile error when implicit conversion is not wanted
    inline QVariant(const char *) { Q_ASSERT(false); }
#endif
    // force compile error, prevent QVariant(QVariant::Type, int) to be called
    inline QVariant(bool, int) { Q_ASSERT(false); }
};

typedef QList<QVariant> QVariantList;
typedef QMap<QString, QVariant> QVariantMap;
typedef QHash<QString, QVariant> QVariantHash;

inline bool qvariant_cast_helper(const QVariant &v, QVariant::Type tp, void *ptr)
{ return QVariant::handler->convert(&v.d, tp, ptr, nullptr); }

template <typename T>
inline QVariant qVariantFromValue(const T &t)
{
    return QVariant(qMetaTypeId<T>(), &t);
}

template <>
inline QVariant qVariantFromValue(const QVariant &t) { return t; }

template <typename T>
inline void qVariantSetValue(QVariant &v, const T &t)
{
    v = QVariant(qMetaTypeId<T>(), &t);
}

template <>
inline void qVariantSetValue<QVariant>(QVariant &v, const QVariant &t)
{
    v = t;
}


template<typename T>
inline void QVariant::setValue(const T &avalue)
{ qVariantSetValue(*this, avalue); }

#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream& operator>> (QDataStream& s, QVariant& p);
Q_CORE_EXPORT QDataStream& operator<< (QDataStream& s, const QVariant& p);
Q_CORE_EXPORT QDataStream& operator>> (QDataStream& s, QVariant::Type& p);
Q_CORE_EXPORT QDataStream& operator<< (QDataStream& s, const QVariant::Type p);
#endif

#ifndef QT_MOC
template<typename T> inline T qvariant_cast(const QVariant &v)
{
    const int vid = qMetaTypeId<T>();
    if (vid == v.userType())
        return *reinterpret_cast<const T *>(v.constData());
    if (vid < int(QMetaType::User)) {
        T t;
        if (qvariant_cast_helper(v, QVariant::Type(vid), &t))
            return t;
    }
    return T();
}

template<> inline QVariant qvariant_cast<QVariant>(const QVariant &v)
{
    if (v.userType() == QMetaType::QVariant)
        return *reinterpret_cast<const QVariant *>(v.constData());
    return v;
}
#endif // QT_MOC

Q_DECLARE_TYPEINFO(QVariant, Q_MOVABLE_TYPE);

#ifndef QT_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug, const QVariant &);
Q_CORE_EXPORT QDebug operator<<(QDebug, const QVariant::Type);
#endif

QT_END_NAMESPACE

Q_DECLARE_BUILTIN_METATYPE(QVariantList, QVariantList)
Q_DECLARE_BUILTIN_METATYPE(QVariantMap, QVariantMap)
Q_DECLARE_BUILTIN_METATYPE(QVariantHash, QVariantHash)


#endif // QVARIANT_H
