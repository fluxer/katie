%{
#include "QtCore/qjsonvalue.h"
QT_USE_NAMESPACE
%}

class QJsonValue
{
 public:
   enum Type {
      Null =  0x0,
      Bool = 0x1,
      Double = 0x2,
      String = 0x3,
      Array = 0x4,
      Object = 0x5,
      Undefined = 0x80
   };

   QJsonValue(Type = Null);
   QJsonValue(bool b);
   QJsonValue(double n);
   QJsonValue(int n);
   QJsonValue(qint64 n);
   QJsonValue(const QString &s);
   QJsonValue(QLatin1String s);
   QJsonValue(const QJsonArray &a);
   QJsonValue(const QJsonObject &o);

   ~QJsonValue();

   QJsonValue(const QJsonValue &other);
   QJsonValue &operator =(const QJsonValue &other);

   static QJsonValue fromVariant(const QVariant &variant);
   QVariant toVariant() const;

   Type type() const;
   bool isNull() const;
   bool isBool() const;
   bool isDouble() const;
   bool isString() const;
   bool isArray() const;
   bool isObject() const;
   bool isUndefined() const;

   bool toBool(bool defaultValue = false) const;
   int toInt(int defaultValue = 0) const;
   double toDouble(double defaultValue = 0) const;
   QString toString(const QString &defaultValue = QString()) const;
   QJsonArray toArray() const;
   QJsonArray toArray(const QJsonArray &defaultValue) const;
   QJsonObject toObject() const;
   QJsonObject toObject(const QJsonObject &defaultValue) const;

   bool operator==(const QJsonValue &other) const;
   bool operator!=(const QJsonValue &other) const;
};

class QJsonValueRef
{
 public:
   QJsonValueRef(QJsonArray *array, int idx);
   QJsonValueRef(QJsonObject *object, int idx);

   operator QJsonValue() const;
   QJsonValueRef &operator = (const QJsonValue &val);
   QJsonValueRef &operator = (const QJsonValueRef &val);

   QJsonValue::Type type() const;
   bool isNull() const;
   bool isBool() const;
   bool isDouble() const;
   bool isString() const;
   bool isArray() const;
   bool isObject() const;
   bool isUndefined() const;

   bool toBool() const;
   int toInt() const;
   double toDouble() const;
   QString toString() const;
   QJsonArray toArray() const;
   QJsonObject toObject() const;

   bool operator==(const QJsonValue &other) const;
   bool operator!=(const QJsonValue &other) const;
};

// QJsonValueRefPtr and QJsonValuePtr not implemented

QDebug operator<<(QDebug, const QJsonValue &);
