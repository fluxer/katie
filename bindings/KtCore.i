%module KtCore

%include "tricks.i"

%include "core/qbytearray.i"
%include "core/qchar.i"
%include "core/qeasingcurve.i"
%include "core/qjsonarray.i"
%include "core/qjsondocument.i"
%include "core/qjsonobject.i"
%include "core/qjsonvalue.i"
%include "core/qlist.i"
%include "core/qobject.i"
%include "core/qregexp.i"
%include "core/qset.i"
%include "core/qstring.i"
%include "core/qstringlist.i"
%include "core/qvariant.i"
%include "core/qvector.i"

// maybe rename? (e.g. QCharList)
%template(QListInt) QList<int>;
%template(QListQByteArray) QList<QByteArray>;
%template(QListQChar) QList<QChar>;
%template(QListQString) QList<QString>;

%template(QSetInt) QSet<int>;
%template(QSetQByteArray) QSet<QByteArray>;
%template(QSetQChar) QSet<QChar>;
%template(QSetQString) QSet<QString>;

%template(QVectorInt) QVector<int>;
%template(QVectorQByteArray) QVector<QByteArray>;
%template(QVectorQChar) QVector<QChar>;
%template(QVectorQString) QVector<QString>;


// %template(QVariantList) QList<QVariant>;
// %template(QVariantMap) QMap<QVariant>;
// %template(QVariantHash) QHash<QVariant>;

%{
#include "QtCore/QCoreApplication"
#include "QtCore/QFile"

QT_USE_NAMESPACE
%}

// temporary implementations for testing purposes
class QCoreApplication : public QObject {
public:
    QCoreApplication(int argc, char *argv[]);
    ~QCoreApplication();

    static int exec();
};

class QFile {
public:
    QFile(QString filename);
    ~QFile();

    bool exists() const;
};


// stub implementations for QVariant testing
class QBitArray {
public:
    QBitArray();
    ~QBitArray();
};

class QDataStream;
class QDate;
class QDateTime;
class QLine;
class QLineF;
class QLocale;
class QMatrix;
class QTransform;
class QTime;
class QPoint;
class QPointF;
class QSize;
class QSizeF;
class QRect;
class QRectF;
class QTextFormat;
class QTextLength;
class QUrl;
