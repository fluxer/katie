%{
#include "QtCore/qstringlist.h"
QT_USE_NAMESPACE
%}

#define Q_NO_USING_KEYWORD

typedef QListIterator<QString> QStringListIterator;
typedef QMutableListIterator<QString> QMutableStringListIterator;

class QStringList : public QList<QString>
{
public:
    QStringList();
    QStringList(const QString &i);
    QStringList(const QStringList &l);
    QStringList(const QList<QString> &l);
#ifdef Q_COMPILER_INITIALIZER_LISTS
    QStringList(std::initializer_list<QString> args);
#endif

    void sort();
    int removeDuplicates();

    QString join(const QString &sep) const;

    QStringList filter(const QString &str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool contains(const QString &str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    QStringList &replaceInStrings(const QString &before, const QString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive);

    QStringList operator+(const QStringList &other) const;
    QStringList &operator<<(const QString &str);
    QStringList &operator<<(const QStringList &l);

#ifndef QT_NO_REGEXP
    QStringList filter(const QRegExp &rx) const;
    QStringList &replaceInStrings(const QRegExp &rx, const QString &after);
    int indexOf(const QRegExp &rx, int from = 0) const;
    int lastIndexOf(const QRegExp &rx, int from = -1) const;
    int indexOf(QRegExp &rx, int from = 0) const;
    int lastIndexOf(QRegExp &rx, int from = -1) const;
#endif
#if !defined(Q_NO_USING_KEYWORD)
    using QList<QString>::indexOf;
    using QList<QString>::lastIndexOf;
#else
    int indexOf(const QString &str, int from = 0) const;
    int lastIndexOf(const QString &str, int from = -1) const;
#endif
};


#ifndef QT_NO_DATASTREAM
QDataStream &operator>>(QDataStream &in, QStringList &list);
QDataStream &operator<<(QDataStream &out, const QStringList &list);
#endif // QT_NO_DATASTREAM
