%{
#include "QtCore/qjsonobject.h"
QT_USE_NAMESPACE
%}

class QJsonObject
{
 public:
   QJsonObject();
   ~QJsonObject();

   QJsonObject(const QJsonObject &other);
   QJsonObject &operator =(const QJsonObject &other);

   static QJsonObject fromVariantMap(const QVariantMap &map);
   QVariantMap toVariantMap() const;
   static QJsonObject fromVariantHash(const QVariantHash &map);
   QVariantHash toVariantHash() const;

   QStringList keys() const;
   int size() const;
   int count() const;
   int length() const;
   bool isEmpty() const;

   QJsonValue value(const QString &key) const;
   QJsonValue operator[] (const QString &key) const;
   QJsonValueRef operator[] (const QString &key);

   void remove(const QString &key);
   QJsonValue take(const QString &key);
   bool contains(const QString &key) const;

   bool operator==(const QJsonObject &other) const;
   bool operator!=(const QJsonObject &other) const;

   // STL compatibility
   bool empty() const;
};

QDebug operator<<(QDebug, const QJsonObject &);
