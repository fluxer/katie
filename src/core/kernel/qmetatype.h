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

#ifndef QMETATYPE_H
#define QMETATYPE_H

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#else
#include <QtCore/qglobal.h>
#endif

#ifdef Bool
#error qmetatype.h must be included before any header file that defines Bool
#endif


QT_BEGIN_NAMESPACE


class Q_CORE_EXPORT QMetaType {
public:
    enum Type {
        // these are merged with QVariant
        Void = 0, Bool = 1, Int = 2, UInt = 3, LongLong = 4, ULongLong = 5,
        Double = 6, Float = 7, QChar = 8, QString = 9, QStringList = 10,
        QByteArray = 11, QBitArray = 12, QDate = 13, QTime = 14,
        QDateTime = 15, QUrl = 16, QLocale = 17, QRect = 18, QRectF = 19,
        QSize = 20, QSizeF = 21, QLine = 22, QLineF = 23, QPoint = 24,
        QPointF = 25, QRegExp = 26, QEasingCurve = 27, QJsonDocument = 28,
        QVariantHash = 29, QVariantList = 30, QVariantMap = 31,
        LastCoreType = QVariantMap,

        FirstGuiType = 64 /* QFont */,
        QFont = 64, QPixmap = 65, QBrush = 66, QColor = 67, QPalette = 68,
        QIcon = 69, QImage = 70, QPolygon = 71, QRegion = 72, QBitmap = 73,
        QCursor = 74, QSizePolicy = 75, QKeySequence = 76, QPen = 77,
        QTextLength = 78, QTextFormat = 79, QMatrix = 80, QTransform = 81,
        QMatrix4x4 = 82, QVector2D = 83, QVector3D = 84, QVector4D = 85,
        LastGuiType = QVector4D,

        FirstCoreExtType = 128 /* VoidStar */,
        VoidStar = 128, Long = 129, Short = 130, Char = 131, ULong = 132,
        UShort = 133, UChar = 134, QObjectStar = 135, QWidgetStar = 136,
        QVariant = 137,
        LastCoreExtType = QVariant,

#if defined(QT_NO_FPU)
        QReal = Float,
#else
        QReal = Double,
#endif

        User = 256
    };

    typedef void (*Destructor)(void *);
    typedef void *(*Constructor)(const void *);

#ifndef QT_NO_DATASTREAM
    typedef void (*SaveOperator)(QDataStream &, const void *);
    typedef void (*LoadOperator)(QDataStream &, void *);
    static void registerStreamOperators(const char *typeName, SaveOperator saveOp,
                                        LoadOperator loadOp);
    static void registerStreamOperators(int type, SaveOperator saveOp,
                                        LoadOperator loadOp);
#endif
    static int registerType(const char *typeName, Destructor destructor,
                            Constructor constructor);
    static int registerTypedef(const char *typeName, int aliasId);
    static int type(const char *typeName);
    static const char *typeName(int type);
    static bool isRegistered(int type);
    static void *construct(int type, const void *copy = nullptr);
    static void destroy(int type, void *data);
    static void unregisterType(const char *typeName);

#ifndef QT_NO_DATASTREAM
    static bool save(QDataStream &stream, int type, const void *data);
    static bool load(QDataStream &stream, int type, void *data);
#endif
};

template <typename T>
void qMetaTypeDeleteHelper(T *t)
{
    delete t;
}

template <typename T>
void *qMetaTypeConstructHelper(const T *t)
{
    if (!t)
        return new T();
    return new T(*static_cast<const T*>(t));
}

#ifndef QT_NO_DATASTREAM
template <typename T>
void qMetaTypeSaveHelper(QDataStream &stream, const T *t)
{
    stream << *t;
}

template <typename T>
void qMetaTypeLoadHelper(QDataStream &stream, T *t)
{
    stream >> *t;
}
#endif // QT_NO_DATASTREAM

template <typename T>
struct QMetaTypeId
{
    enum { Defined = 0 };
};

template <typename T>
struct QMetaTypeId2
{
    enum { Defined = QMetaTypeId<T>::Defined };
    static inline int qt_metatype_id() { return QMetaTypeId<T>::qt_metatype_id(); }
};

namespace QtPrivate {
    template <typename T, bool Defined = QMetaTypeId2<T>::Defined>
    struct QMetaTypeIdHelper {
        static inline int qt_metatype_id()
        { return QMetaTypeId2<T>::qt_metatype_id(); }
    };
    template <typename T> struct QMetaTypeIdHelper<T, false> {
        static inline int qt_metatype_id()
        { return -1; }
    };
}

template <typename T>
int qRegisterMetaType(const char *typeName, bool notypedef = false)
{
    const int typedefOf = (notypedef ? -1 : QtPrivate::QMetaTypeIdHelper<T>::qt_metatype_id());
    if (typedefOf != -1)
        return QMetaType::registerTypedef(typeName, typedefOf);

    typedef void*(*ConstructPtr)(const T*);
    ConstructPtr cptr = qMetaTypeConstructHelper<T>;
    typedef void(*DeletePtr)(T*);
    DeletePtr dptr = qMetaTypeDeleteHelper<T>;

    return QMetaType::registerType(typeName, reinterpret_cast<QMetaType::Destructor>(dptr),
                                   reinterpret_cast<QMetaType::Constructor>(cptr));
}

#ifndef QT_NO_DATASTREAM
template <typename T>
void qRegisterMetaTypeStreamOperators(const char *typeName)
{
    typedef void(*SavePtr)(QDataStream &, const T *);
    typedef void(*LoadPtr)(QDataStream &, T *);
    SavePtr sptr = qMetaTypeSaveHelper<T>;
    LoadPtr lptr = qMetaTypeLoadHelper<T>;

    qRegisterMetaType<T>(typeName);
    QMetaType::registerStreamOperators(typeName, reinterpret_cast<QMetaType::SaveOperator>(sptr),
                                       reinterpret_cast<QMetaType::LoadOperator>(lptr));
}
#endif // QT_NO_DATASTREAM

template <typename T>
inline int qMetaTypeId()
{
    return QMetaTypeId2<T>::qt_metatype_id();
}

template <typename T>
inline int qRegisterMetaType()
{
    return qMetaTypeId<T>();
}

#ifndef QT_NO_DATASTREAM
template <typename T>
inline int qRegisterMetaTypeStreamOperators()
{
    typedef void(*SavePtr)(QDataStream &, const T *);
    typedef void(*LoadPtr)(QDataStream &, T *);
    SavePtr sptr = qMetaTypeSaveHelper<T>;
    LoadPtr lptr = qMetaTypeLoadHelper<T>;

    int id = qMetaTypeId<T>();
    QMetaType::registerStreamOperators(id,
                                       reinterpret_cast<QMetaType::SaveOperator>(sptr),
                                       reinterpret_cast<QMetaType::LoadOperator>(lptr));

    return id;
}
#endif

#define Q_DECLARE_METATYPE(TYPE)                                        \
    QT_BEGIN_NAMESPACE                                                  \
    template <>                                                         \
    struct QMetaTypeId< TYPE >                                          \
    {                                                                   \
        enum { Defined = 1 };                                           \
        static int qt_metatype_id()                                     \
            {                                                           \
                static int metatype_id =                                \
                    qRegisterMetaType< TYPE >(#TYPE, true);             \
                return metatype_id;                                     \
            }                                                           \
    };                                                                  \
    QT_END_NAMESPACE

#define Q_DECLARE_BUILTIN_METATYPE(TYPE, NAME) \
    template<> struct QMetaTypeId2<TYPE> \
    { \
        enum { Defined = 1 }; \
        static inline int qt_metatype_id() { return QMetaType::NAME; } \
    };

class QString;
class QByteArray;
class QChar;
class QStringList;
class QBitArray;
class QDate;
class QTime;
class QDateTime;
class QUrl;
class QLocale;
class QRect;
class QRectF;
class QSize;
class QSizeF;
class QLine;
class QLineF;
class QPoint;
class QPointF;
#ifndef QT_NO_REGEXP
class QRegExp;
#endif
class QJsonDocument;
class QEasingCurve;
class QWidget;
class QObject;

class QFont;
class QPixmap;
class QBrush;
class QColor;
class QPalette;
class QIcon;
class QImage;
class QPolygon;
class QRegion;
class QBitmap;
class QCursor;
class QSizePolicy;
class QKeySequence;
class QPen;
class QTextLength;
class QTextFormat;
class QMatrix;
class QTransform;
class QMatrix4x4;
class QVector2D;
class QVector3D;
class QVector4D;
class QVariant;

// Void
Q_DECLARE_BUILTIN_METATYPE(bool, Bool)
Q_DECLARE_BUILTIN_METATYPE(int, Int)
Q_DECLARE_BUILTIN_METATYPE(uint, UInt)
Q_DECLARE_BUILTIN_METATYPE(qlonglong, LongLong)
Q_DECLARE_BUILTIN_METATYPE(qulonglong, ULongLong)
Q_DECLARE_BUILTIN_METATYPE(double, Double)
Q_DECLARE_BUILTIN_METATYPE(float, Float)
Q_DECLARE_BUILTIN_METATYPE(QChar, QChar)
Q_DECLARE_BUILTIN_METATYPE(QString, QString)
Q_DECLARE_BUILTIN_METATYPE(QStringList, QStringList)
Q_DECLARE_BUILTIN_METATYPE(QByteArray, QByteArray)
Q_DECLARE_BUILTIN_METATYPE(QBitArray, QBitArray)
Q_DECLARE_BUILTIN_METATYPE(QDate, QDate)
Q_DECLARE_BUILTIN_METATYPE(QTime, QTime)
Q_DECLARE_BUILTIN_METATYPE(QDateTime, QDateTime)
Q_DECLARE_BUILTIN_METATYPE(QUrl, QUrl)
Q_DECLARE_BUILTIN_METATYPE(QLocale, QLocale)
Q_DECLARE_BUILTIN_METATYPE(QRect, QRect)
Q_DECLARE_BUILTIN_METATYPE(QRectF, QRectF)
Q_DECLARE_BUILTIN_METATYPE(QSize, QSize)
Q_DECLARE_BUILTIN_METATYPE(QSizeF, QSizeF)
Q_DECLARE_BUILTIN_METATYPE(QLine, QLine)
Q_DECLARE_BUILTIN_METATYPE(QLineF, QLineF)
Q_DECLARE_BUILTIN_METATYPE(QPoint, QPoint)
Q_DECLARE_BUILTIN_METATYPE(QPointF, QPointF)
#ifndef QT_NO_REGEXP
Q_DECLARE_BUILTIN_METATYPE(QRegExp, QRegExp)
#endif
Q_DECLARE_BUILTIN_METATYPE(QEasingCurve, QEasingCurve)
Q_DECLARE_BUILTIN_METATYPE(QJsonDocument, QJsonDocument)
// QVariantHash
// QVariantList
// QVariantMap

Q_DECLARE_BUILTIN_METATYPE(QFont, QFont)
Q_DECLARE_BUILTIN_METATYPE(QPixmap, QPixmap)
Q_DECLARE_BUILTIN_METATYPE(QBrush, QBrush)
Q_DECLARE_BUILTIN_METATYPE(QColor, QColor)
Q_DECLARE_BUILTIN_METATYPE(QPalette, QPalette)
Q_DECLARE_BUILTIN_METATYPE(QIcon, QIcon)
Q_DECLARE_BUILTIN_METATYPE(QImage, QImage)
Q_DECLARE_BUILTIN_METATYPE(QPolygon, QPolygon)
Q_DECLARE_BUILTIN_METATYPE(QRegion, QRegion)
Q_DECLARE_BUILTIN_METATYPE(QBitmap, QBitmap)
Q_DECLARE_BUILTIN_METATYPE(QCursor, QCursor)
Q_DECLARE_BUILTIN_METATYPE(QSizePolicy, QSizePolicy)
Q_DECLARE_BUILTIN_METATYPE(QKeySequence, QKeySequence)
Q_DECLARE_BUILTIN_METATYPE(QPen, QPen)
Q_DECLARE_BUILTIN_METATYPE(QTextLength, QTextLength)
Q_DECLARE_BUILTIN_METATYPE(QTextFormat, QTextFormat)
Q_DECLARE_BUILTIN_METATYPE(QMatrix, QMatrix)
Q_DECLARE_BUILTIN_METATYPE(QTransform, QTransform)
Q_DECLARE_BUILTIN_METATYPE(QMatrix4x4, QMatrix4x4)
Q_DECLARE_BUILTIN_METATYPE(QVector2D, QVector2D)
Q_DECLARE_BUILTIN_METATYPE(QVector3D, QVector3D)
Q_DECLARE_BUILTIN_METATYPE(QVector4D, QVector4D)

Q_DECLARE_BUILTIN_METATYPE(void *, VoidStar)
Q_DECLARE_BUILTIN_METATYPE(long, Long)
Q_DECLARE_BUILTIN_METATYPE(short, Short)
Q_DECLARE_BUILTIN_METATYPE(char, Char)
Q_DECLARE_BUILTIN_METATYPE(signed char, Char)
Q_DECLARE_BUILTIN_METATYPE(ulong, ULong)
Q_DECLARE_BUILTIN_METATYPE(ushort, UShort)
Q_DECLARE_BUILTIN_METATYPE(uchar, UChar)
Q_DECLARE_BUILTIN_METATYPE(QObject *, QObjectStar)
Q_DECLARE_BUILTIN_METATYPE(QWidget *, QWidgetStar)
Q_DECLARE_BUILTIN_METATYPE(QVariant, QVariant)

QT_END_NAMESPACE

#endif // QMETATYPE_H
