%{
#include "QtCore/qvariant.h"
QT_USE_NAMESPACE
%}

template <typename T>
QVariant qVariantFromValue(const T &);

template<typename T>
T qvariant_cast(const QVariant &);

class QVariant
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
        JsonValue = QMetaType::QJsonValue,
        JsonObject = QMetaType::QJsonObject,
        JsonArray = QMetaType::QJsonArray,
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

    QVariant();
    ~QVariant();
    QVariant(Type type);
    QVariant(int typeOrUserType, const void *copy);
    QVariant(int typeOrUserType, const void *copy, uint flags);
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
    QVariant(const char *str);
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
#ifndef QT_NO_GEOM_VARIANT
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
    QVariant(const QJsonValue &jsonValue);
    QVariant(const QJsonObject &jsonObject);
    QVariant(const QJsonArray &jsonArray);
    QVariant(const QJsonDocument &jsonDocument);
#ifndef QT_BOOTSTRAPPED
    QVariant(const QEasingCurve &easing);
#endif

    QVariant& operator=(const QVariant &other);
#ifdef Q_COMPILER_RVALUE_REFS
    QVariant &operator=(QVariant &&other);
#endif

    void swap(QVariant &other);

    Type type() const;
    int userType() const;
    const char *typeName() const;

    bool canConvert(Type t) const;
    bool convert(Type t);


    bool isValid() const;
    bool isNull() const;

    void clear();

    void detach();
    bool isDetached() const;



    int toInt(bool *ok = Q_NULLPTR) const;
    uint toUInt(bool *ok = Q_NULLPTR) const;
    qlonglong toLongLong(bool *ok = Q_NULLPTR) const;
    qulonglong toULongLong(bool *ok = Q_NULLPTR) const;
    bool toBool() const;
    double toDouble(bool *ok = Q_NULLPTR) const;
    float toFloat(bool *ok = Q_NULLPTR) const;
    qreal toReal(bool *ok = Q_NULLPTR) const;
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

#ifndef QT_NO_GEOM_VARIANT
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
    QJsonValue toJsonValue() const;
    QJsonObject toJsonObject() const;
    QJsonArray toJsonArray() const;
    QJsonDocument toJsonDocument() const;
    QEasingCurve toEasingCurve() const;
#endif


#ifndef QT_NO_DATASTREAM
    void load(QDataStream &ds);
    void save(QDataStream &ds) const;
#endif
    static const char *typeToName(Type type);
    static Type nameToType(const char *name);


    void *data();
    const void *constData() const;
    const void *data() const;

    template<typename T>
    void setValue(const T &value);

    template<typename T>
    T value() const;

    template<typename T>
    static QVariant fromValue(const T &value);

    template<typename T>
    bool canConvert() const;

    bool operator==(const QVariant &v) const;
    bool operator!=(const QVariant &v) const;
};

bool qvariant_cast_helper(const QVariant &v, QVariant::Type tp, void *ptr);

template <typename T>
QVariant qVariantFromValue(const T &t);

template <>
QVariant qVariantFromValue(const QVariant &t);

template <typename T>
void qVariantSetValue(QVariant &v, const T &t);

template <>
void qVariantSetValue<QVariant>(QVariant &v, const QVariant &t);

template<typename T>
void QVariant::setValue(const T &avalue);

#ifndef QT_NO_DATASTREAM
QDataStream& operator>> (QDataStream& s, QVariant& p);
QDataStream& operator<< (QDataStream& s, const QVariant& p);
QDataStream& operator>> (QDataStream& s, QVariant::Type& p);
QDataStream& operator<< (QDataStream& s, const QVariant::Type p);
#endif

#ifndef QT_MOC
template<typename T> T qvariant_cast(const QVariant &v);

template<> QVariant qvariant_cast<QVariant>(const QVariant &v);
#endif // QT_MOC

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const QVariant &);
QDebug operator<<(QDebug, const QVariant::Type);
#endif
