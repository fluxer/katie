%module KtCore

%include "tricks.i"

%include "core/qbytearray.i"
%include "core/qchar.i"
%include "core/qcoreapplication.i"
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
#include "QtCore/QFile"
#include "QtCore/QBitArray"
#include "QtCore/QDataStream"
#include "QtCore/QDate"
#include "QtCore/QDateTime"
#include "QtCore/QLine"
#include "QtCore/QLineF"
#include "QtCore/QLocale"
#include "QtCore/QTime"
#include "QtCore/QPoint"
#include "QtCore/QPointF"
#include "QtCore/QSize"
#include "QtCore/QSizeF"
#include "QtCore/QRect"
#include "QtCore/QRectF"
#include "QtCore/QUrl"

QT_USE_NAMESPACE
%}

// temporary implementations for testing purposes
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

class QDataStream {
public:
    QDataStream();
    ~QDataStream();
};

class QDate {
public:
    QDate();
    ~QDate();
};

class QDateTime {
public:
    QDateTime();
    ~QDateTime();
};

class QLine {
public:
    QLine();
    ~QLine();
};

class QLineF {
public:
    QLineF();
    ~QLineF();
};

class QLocale {
public:
    QLocale();
    ~QLocale();
};

class QTime {
public:
    QTime();
    ~QTime();
};

class QPoint {
public:
    QPoint();
    ~QPoint();
};

class QPointF {
public:
    QPointF();
    ~QPointF();
};

class QSize {
public:
    QSize();
    ~QSize();
};

class QSizeF {
public:
    QSizeF();
    ~QSizeF();
};

class QRect {
public:
    QRect();
    ~QRect();
};

class QRectF {
public:
    QRectF();
    ~QRectF();
};

class QUrl {
public:
    QUrl();
    ~QUrl();
};
