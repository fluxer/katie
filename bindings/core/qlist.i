%{
#include "QtCore/qlist.h"
QT_USE_NAMESPACE
%}

template <typename T>
class QList
{
public:
    QList();
    QList(const QList<T> &l);
    ~QList();
    QList<T> &operator=(const QList<T> &l);
#ifdef Q_COMPILER_RVALUE_REFS
    QList &operator=(QList &&other);
#endif
    void swap(QList<T> &other);
#ifdef Q_COMPILER_INITIALIZER_LISTS
    QList(std::initializer_list<T> args);
#endif
    bool operator==(const QList<T> &l) const;
    bool operator!=(const QList<T> &l) const;

    int size() const;

    void detach();

    void detachShared();

    bool isDetached() const;
    void setSharable(bool sharable);
    bool isSharedWith(const QList<T> &other) const;

    bool isEmpty() const;

    void clear();

    const T &at(int i) const;
    const T &operator[](int i) const;
    T &operator[](int i);

    void reserve(int size);
    void append(const T &t);
    void append(const QList<T> &t);
    void prepend(const T &t);
    void insert(int i, const T &t);
    void replace(int i, const T &t);
    void removeAt(int i);
    int removeAll(const T &t);
    bool removeOne(const T &t);
    T takeAt(int i);
    T takeFirst();
    T takeLast();
    void move(int from, int to);
    void swap(int i, int j);
    int indexOf(const T &t, int from = 0) const;
    int lastIndexOf(const T &t, int from = -1) const;
    bool contains(const T &t) const;
    int count(const T &t) const;

    // more Qt
    int count() const;
    int length() const;
    T& first();
    const T& first() const;
    T& last();
    const T& last() const;
    void removeFirst();
    void removeLast();
    bool startsWith(const T &t) const;
    bool endsWith(const T &t) const;
    QList<T> mid(int pos, int length = -1) const;

    T value(int i) const;
    T value(int i, const T &defaultValue) const;

    // stl compatibility
    void push_back(const T &t);
    void push_front(const T &t);
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    void pop_front();
    void pop_back();
    bool empty() const;


    // comfort
    QList<T> &operator+=(const QList<T> &l);
    QList<T> operator+(const QList<T> &l) const;
    QList<T> &operator+=(const T &t);
    QList<T> &operator<< (const T &t);
    QList<T> &operator<<(const QList<T> &l);

    QVector<T> toVector() const;
    QSet<T> toSet() const;

    static QList<T> fromVector(const QVector<T> &vector);
    static QList<T> fromSet(const QSet<T> &set);

    static QList<T> fromStdList(const std::list<T> &list);
    std::list<T> toStdList() const;
};
