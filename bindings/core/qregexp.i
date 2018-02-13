%{
#include "QtCore/qregexp.h"
QT_USE_NAMESPACE
%}

#ifndef QT_NO_REGEXP

class QRegExp
{
public:
    enum PatternSyntax {
        RegExp,
        Wildcard,
        FixedString,
        RegExp2,
        WildcardUnix,
        W3CXmlSchema11 };
    enum CaretMode { CaretAtZero, CaretAtOffset, CaretWontMatch };

    QRegExp();
    QRegExp(const QString &pattern, Qt::CaseSensitivity cs = Qt::CaseSensitive,
            PatternSyntax syntax = RegExp);
    QRegExp(const QRegExp &rx);
    ~QRegExp();
    QRegExp &operator=(const QRegExp &rx);
#ifdef Q_COMPILER_RVALUE_REFS
    QRegExp &operator=(QRegExp &&other);
#endif
    void swap(QRegExp &other);

    bool operator==(const QRegExp &rx) const;
    bool operator!=(const QRegExp &rx) const;

    bool isEmpty() const;
    bool isValid() const;
    QString pattern() const;
    void setPattern(const QString &pattern);
    Qt::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(Qt::CaseSensitivity cs);
    PatternSyntax patternSyntax() const;
    void setPatternSyntax(PatternSyntax syntax);

    bool isMinimal() const;
    void setMinimal(bool minimal);

    bool exactMatch(const QString &str) const;

    int indexIn(const QString &str, int offset = 0, CaretMode caretMode = CaretAtZero) const;
    int lastIndexIn(const QString &str, int offset = -1, CaretMode caretMode = CaretAtZero) const;
    int matchedLength() const;
#ifndef QT_NO_REGEXP_CAPTURE
    int captureCount() const;
    QStringList capturedTexts() const;
    QString cap(int nth = 0) const;
    int pos(int nth = 0) const;
    QString errorString() const;
#endif

    static QString escape(const QString &str);
};

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const QRegExp &regExp);
QDataStream &operator>>(QDataStream &in, QRegExp &regExp);
#endif

#endif // QT_NO_REGEXP
