%module KtCore

%include "core/qbytearray.i"
%include "core/qchar.i"
%include "core/qlist.i"
%include "core/qregexp.i"
%include "core/qset.i"
%include "core/qstring.i"
%include "core/qstringlist.i"
%include "core/qvector.i"

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

%{
#include "QtCore/QObject"
#include "QtCore/QCoreApplication"
#include "QtCore/QFile"

QT_USE_NAMESPACE
%}

class QObject {
public:
    QObject(QObject *parent = Q_NULLPTR);
    virtual ~QObject();
};


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
