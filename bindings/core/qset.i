%{
#include "QtCore/qset.h"
QT_USE_NAMESPACE
%}

template <class T>
class QSet
{
public:
    QSet();
    QSet(const QSet<T> &other);

    QSet<T> &operator=(const QSet<T> &other);
#ifdef Q_COMPILER_RVALUE_REFS
    QSet<T> &operator=(QSet<T> &&other);
#endif
    void swap(QSet<T> &other);

    bool operator==(const QSet<T> &other) const;
    bool operator!=(const QSet<T> &other) const;

    int size() const;

    bool isEmpty() const;

    int capacity() const;
    void reserve(int size);
    void squeeze();

    void detach();
    bool isDetached() const;
    void setSharable(bool sharable);

    void clear();

    bool remove(const T &value);

    bool contains(const T &value) const;

    bool contains(const QSet<T> &set) const;

    // more Qt
    int count() const;
    QSet<T> &unite(const QSet<T> &other);
    QSet<T> &intersect(const QSet<T> &other);
    QSet<T> &subtract(const QSet<T> &other);

    bool empty() const;
    // comfort
    QSet<T> &operator<<(const T &value);
    QSet<T> &operator|=(const QSet<T> &other);
    QSet<T> &operator|=(const T &value);
    QSet<T> &operator&=(const QSet<T> &other);
    QSet<T> &operator&=(const T &value);
    QSet<T> &operator+=(const QSet<T> &other);
    QSet<T> &operator+=(const T &value);
    QSet<T> &operator-=(const QSet<T> &other);
    QSet<T> &operator-=(const T &value);
    QSet<T> operator|(const QSet<T> &other) const;
    QSet<T> operator&(const QSet<T> &other) const;
    QSet<T> operator+(const QSet<T> &other) const;
    QSet<T> operator-(const QSet<T> &other) const;

    QList<T> toList() const;
    QList<T> values() const;

    static QSet<T> fromList(const QList<T> &list);
};
