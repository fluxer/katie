%module KtCore

%{
#include "katie/QtCore/QtCore"

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

class QChar {
public:
    QChar(const char c);
    QChar(const uchar c);
    QChar(const uchar c, uchar r);
    QChar(const ushort rc);
    QChar(const short rc);
    QChar(const uint rc);
    QChar(const int rc);

    char toAscii() const;
    char toLatin1() const;
    ushort unicode() const;

    bool isNull() const;
    bool isPrint() const;
    bool isPunct() const;
    bool isSpace() const;
    bool isMark() const;
    bool isLetter() const;
    bool isNumber() const;
    bool isLetterOrNumber() const;
    bool isDigit() const;
    bool isSymbol() const;
    bool isLower() const;
    bool isUpper() const;
    bool isTitleCase() const;
};

class QString {
public:
    QString(const char *ch);
    QString(const QChar *unicode, int size = -1);
    QString(const QChar c);
    QString(const int size, const QChar c);
    QString(const QString &);
    ~QString();
};

class QFile {
public:
    QFile(QString filename);
    ~QFile();

    bool exists() const;
};
