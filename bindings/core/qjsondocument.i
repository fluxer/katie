%{
#include "QtCore/qjsondocument.h"
QT_USE_NAMESPACE
%}

struct QJsonParseError {
   enum ParseError {
      NoError = 0,
      UnterminatedObject,
      MissingNameSeparator,
      UnterminatedArray,
      MissingValueSeparator,
      IllegalValue,
      TerminationByNumber,
      IllegalNumber,
      IllegalEscapeSequence,
      IllegalUTF8String,
      UnterminatedString,
      MissingObject,
      DeepNesting,
      DocumentTooLarge
   };

   QString    errorString() const;

   int        offset;
   ParseError error;
};

class QJsonDocument
{
 public:

   QJsonDocument();
   QJsonDocument(const QJsonObject &object);
   QJsonDocument(const QJsonArray &array);
   ~QJsonDocument();

   QJsonDocument(const QJsonDocument &other);
   QJsonDocument &operator =(const QJsonDocument &other);

   enum DataValidation {
      Validate,
      BypassValidation
   };

   static QJsonDocument fromRawData(const char *data, int size, DataValidation validation = Validate);
   const char *rawData(int *size) const;

   static QJsonDocument fromBinaryData(const QByteArray &data, DataValidation validation  = Validate);
   QByteArray toBinaryData() const;

   static QJsonDocument fromVariant(const QVariant &variant);
   QVariant toVariant() const;

   enum JsonFormat {
      Indented,
      Compact
   };

   static QJsonDocument fromJson(const QByteArray &json, QJsonParseError *error = 0);

   QByteArray toJson(JsonFormat format = Indented) const;

   bool isEmpty() const;
   bool isArray() const;
   bool isObject() const;

   QJsonObject object() const;
   QJsonArray array() const;

   void setObject(const QJsonObject &object);
   void setArray(const QJsonArray &array);

   bool operator==(const QJsonDocument &other) const;
   bool operator!=(const QJsonDocument &other) const;

   bool isNull() const;
};

QDebug operator<<(QDebug, const QJsonDocument &);
