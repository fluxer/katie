%{
#include "QtCore/qvector.h"
QT_USE_NAMESPACE
%}

template <typename T>
class QVector
{
public:
    QVector();
    QVector(int size);
    QVector(int size, const T &t);
    QVector(const QVector<T> &v);
    ~QVector();
    QVector<T> &operator=(const QVector<T> &v);
#ifdef Q_COMPILER_RVALUE_REFS
    QVector<T> operator=(QVector<T> &&other);
#endif
    void swap(QVector<T> &other);
#ifdef Q_COMPILER_INITIALIZER_LISTS
    QVector(std::initializer_list<T> args);
#endif
    bool operator==(const QVector<T> &v) const;
    bool operator!=(const QVector<T> &v) const;

    int size() const;

    bool isEmpty() const;

    void resize(int size);

    int capacity() const;
    void reserve(int size);
    void squeeze();

    void detach();
    bool isDetached() const;
    void setSharable(bool sharable);
    bool isSharedWith(const QVector<T> &other) const;

    T *data();
    const T *data() const;
    const T *constData() const;
    void clear();

    const T &at(int i) const;
    T &operator[](int i);
    const T &operator[](int i) const;
    void append(const T &t);
    void prepend(const T &t);
    void insert(int i, const T &t);
    void insert(int i, int n, const T &t);
    void replace(int i, const T &t);
    void remove(int i);
    void remove(int i, int n);

    QVector<T> &fill(const T &t, int size = -1);

    int indexOf(const T &t, int from = 0) const;
    int lastIndexOf(const T &t, int from = -1) const;
    bool contains(const T &t) const;
    int count(const T &t) const;

    // more Qt
    int count() const;
    T& first();
    const T &first() const;
    T& last();
    const T &last() const;
    bool startsWith(const T &t) const;
    bool endsWith(const T &t) const;
    QVector<T> mid(int pos, int length = -1) const;

    T value(int i) const;
    T value(int i, const T &defaultValue) const;

    // STL compatibility
    void push_back(const T &t);
    void push_front(const T &t);
    void pop_back();
    void pop_front();
    bool empty() const;
    T& front();
    const T &front() const;
    T &back();
    const T &back();

    // comfort
    QVector<T> &operator+=(const QVector<T> &l);
    QVector<T> operator+(const QVector<T> &l) const;
    QVector<T> &operator+=(const T &t);
    QVector<T> &operator<< (const T &t);
    QVector<T> &operator<<(const QVector<T> &l);

    QList<T> toList() const;

    static QVector<T> fromList(const QList<T> &list);

    static QVector<T> fromStdVector(const std::vector<T> &vector);
    std::vector<T> toStdVector() const;
};
