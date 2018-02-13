%{
#include "QtCore/qjsonarray.h"
QT_USE_NAMESPACE
%}

class QJsonArray
{
 public:
   QJsonArray();
   ~QJsonArray();

   QJsonArray(const QJsonArray &other);
   QJsonArray &operator =(const QJsonArray &other);

   static QJsonArray fromStringList(const QStringList &list);
   static QJsonArray fromVariantList(const QVariantList &list);
   QVariantList toVariantList() const;

   int size() const;
   int count() const;

   bool isEmpty() const;
   QJsonValue at(int i) const;
   QJsonValue first() const;
   QJsonValue last() const;

   void prepend(const QJsonValue &value);
   void append(const QJsonValue &value);
   void removeAt(int i);
   QJsonValue takeAt(int i);
   void removeFirst();
   void removeLast();

   void insert(int i, const QJsonValue &value);
   void replace(int i, const QJsonValue &value);

   bool contains(const QJsonValue &element) const;
   QJsonValueRef operator[](int i);
   QJsonValue operator[](int i) const;

   bool operator==(const QJsonArray &other) const;
   bool operator!=(const QJsonArray &other) const;

   // stl compatibility
   void push_back(const QJsonValue &t);
   void push_front(const QJsonValue &t);
   void pop_front();
   void pop_back();
   bool empty();
};

QDebug operator<<(QDebug, const QJsonArray &);
