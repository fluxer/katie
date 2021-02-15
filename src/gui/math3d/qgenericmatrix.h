/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGENERICMATRIX_H
#define QGENERICMATRIX_H

#include <QtCore/qmetatype.h>
#include <QtCore/qdebug.h>
#include <QtCore/qdatastream.h>


QT_BEGIN_NAMESPACE


template <int N, int M>
class QGenericMatrix
{
public:
    QGenericMatrix();
    QGenericMatrix(const QGenericMatrix<N, M>& other);
    explicit QGenericMatrix(const qreal *values);

    const qreal& operator()(int row, int column) const;
    qreal& operator()(int row, int column);

    bool isIdentity() const;
    void setToIdentity();

    void fill(qreal value);

    QGenericMatrix<M, N> transposed() const;

    QGenericMatrix<N, M>& operator+=(const QGenericMatrix<N, M>& other);
    QGenericMatrix<N, M>& operator-=(const QGenericMatrix<N, M>& other);
    QGenericMatrix<N, M>& operator*=(qreal factor);
    QGenericMatrix<N, M>& operator/=(qreal divisor);
    bool operator==(const QGenericMatrix<N, M>& other) const;
    bool operator!=(const QGenericMatrix<N, M>& other) const;

    void copyDataTo(qreal *values) const;

    qreal *data() { return *m; }
    const qreal *data() const { return *m; }
    const qreal *constData() const { return *m; }

#if !defined(Q_NO_TEMPLATE_FRIENDS)
    template<int NN, int MM>
    friend QGenericMatrix<NN, MM> operator+(const QGenericMatrix<NN, MM>& m1, const QGenericMatrix<NN, MM>& m2);
    template<int NN, int MM>
    friend QGenericMatrix<NN, MM> operator-(const QGenericMatrix<NN, MM>& m1, const QGenericMatrix<NN, MM>& m2);
    template<int NN, int M1, int M2>
    friend QGenericMatrix<M1, M2> operator*(const QGenericMatrix<NN, M2>& m1, const QGenericMatrix<M1, NN>& m2);
    template<int NN, int MM>
    friend QGenericMatrix<NN, MM> operator-(const QGenericMatrix<NN, MM>& matrix);
    template<int NN, int MM>
    friend QGenericMatrix<NN, MM> operator*(qreal factor, const QGenericMatrix<NN, MM>& matrix);
    template<int NN, int MM>
    friend QGenericMatrix<NN, MM> operator*(const QGenericMatrix<NN, MM>& matrix, qreal factor);
    template<int NN, int MM>
    friend QGenericMatrix<NN, MM> operator/(const QGenericMatrix<NN, MM>& matrix, qreal divisor);

private:
#endif
    qreal m[N][M];    // Column-major order to match OpenGL.

    QGenericMatrix(int) {}       // Construct without initializing identity matrix.

#if !defined(Q_NO_TEMPLATE_FRIENDS)
    template <int NN, int MM>
    friend class QGenericMatrix;
#endif
};

template <int N, int M>
Q_INLINE_TEMPLATE QGenericMatrix<N, M>::QGenericMatrix()
{
    setToIdentity();
}

template <int N, int M>
Q_INLINE_TEMPLATE QGenericMatrix<N, M>::QGenericMatrix(const QGenericMatrix<N, M>& other)
{
    for (int col = 0; col < N; ++col)
        for (int row = 0; row < M; ++row)
            m[col][row] = other.m[col][row];
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<N, M>::QGenericMatrix(const qreal *values)
{
    for (int col = 0; col < N; ++col)
        for (int row = 0; row < M; ++row)
            m[col][row] = values[row * N + col];
}

template <int N, int M>
Q_INLINE_TEMPLATE const qreal& QGenericMatrix<N, M>::operator()(int row, int column) const
{
    Q_ASSERT(row >= 0 && row < M && column >= 0 && column < N);
    return m[column][row];
}

template <int N, int M>
Q_INLINE_TEMPLATE qreal& QGenericMatrix<N, M>::operator()(int row, int column)
{
    Q_ASSERT(row >= 0 && row < M && column >= 0 && column < N);
    return m[column][row];
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE bool QGenericMatrix<N, M>::isIdentity() const
{
    for (int col = 0; col < N; ++col) {
        for (int row = 0; row < M; ++row) {
            if (row == col) {
                if (m[col][row] != 1.0f)
                    return false;
            } else {
                if (m[col][row] != 0.0f)
                    return false;
            }
        }
    }
    return true;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE void QGenericMatrix<N, M>::setToIdentity()
{
    for (int col = 0; col < N; ++col) {
        for (int row = 0; row < M; ++row) {
            if (row == col)
                m[col][row] = 1.0f;
            else
                m[col][row] = 0.0f;
        }
    }
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE void QGenericMatrix<N, M>::fill(qreal value)
{
    for (int col = 0; col < N; ++col)
        for (int row = 0; row < M; ++row)
            m[col][row] = value;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<M, N> QGenericMatrix<N, M>::transposed() const
{
    QGenericMatrix<M, N> result(1);
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            result.m[row][col] = m[col][row];
    return result;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<N, M>& QGenericMatrix<N, M>::operator+=(const QGenericMatrix<N, M>& other)
{
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            m[col][row] += other.m[col][row];
    return *this;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<N, M>& QGenericMatrix<N, M>::operator-=(const QGenericMatrix<N, M>& other)
{
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            m[col][row] -= other.m[col][row];
    return *this;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<N, M>& QGenericMatrix<N, M>::operator*=(qreal factor)
{
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            m[col][row] *= factor;
    return *this;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE bool QGenericMatrix<N, M>::operator==(const QGenericMatrix<N, M>& other) const
{
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)  {
            if (m[col][row] != other.m[col][row])
                return false;
        }
    return true;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE bool QGenericMatrix<N, M>::operator!=(const QGenericMatrix<N, M>& other) const
{
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col) {
            if (m[col][row] != other.m[col][row])
                return true;
        }
    return false;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<N, M>& QGenericMatrix<N, M>::operator/=(qreal divisor)
{
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            m[col][row] /= divisor;
    return *this;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<N, M> operator+(const QGenericMatrix<N, M>& m1, const QGenericMatrix<N, M>& m2)
{
    QGenericMatrix<N, M> result(1);
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            result.m[col][row] = m1.m[col][row] + m2.m[col][row];
    return result;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<N, M> operator-(const QGenericMatrix<N, M>& m1, const QGenericMatrix<N, M>& m2)
{
    QGenericMatrix<N, M> result(1);
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            result.m[col][row] = m1.m[col][row] - m2.m[col][row];
    return result;
}

template <int N, int M1, int M2>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<M1, M2> operator*(const QGenericMatrix<N, M2>& m1, const QGenericMatrix<M1, N>& m2)
{
    QGenericMatrix<M1, M2> result(1);
    for (int row = 0; row < M2; ++row) {
        for (int col = 0; col < M1; ++col) {
            qreal sum(0.0f);
            for (int j = 0; j < N; ++j)
                sum += m1.m[j][row] * m2.m[col][j];
            result.m[col][row] = sum;
        }
    }
    return result;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<N, M> operator-(const QGenericMatrix<N, M>& matrix)
{
    QGenericMatrix<N, M> result(1);
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            result.m[col][row] = -matrix.m[col][row];
    return result;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<N, M> operator*(qreal factor, const QGenericMatrix<N, M>& matrix)
{
    QGenericMatrix<N, M> result(1);
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            result.m[col][row] = matrix.m[col][row] * factor;
    return result;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<N, M> operator*(const QGenericMatrix<N, M>& matrix, qreal factor)
{
    QGenericMatrix<N, M> result(1);
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            result.m[col][row] = matrix.m[col][row] * factor;
    return result;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE QGenericMatrix<N, M> operator/(const QGenericMatrix<N, M>& matrix, qreal divisor)
{
    QGenericMatrix<N, M> result(1);
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            result.m[col][row] = matrix.m[col][row] / divisor;
    return result;
}

template <int N, int M>
Q_OUTOFLINE_TEMPLATE void QGenericMatrix<N, M>::copyDataTo(qreal *values) const
{
    for (int col = 0; col < N; ++col)
        for (int row = 0; row < M; ++row)
            values[row * N + col] = m[col][row];
}

// Define aliases for the useful variants of QGenericMatrix.
typedef QGenericMatrix<2, 2> QMatrix2x2;
typedef QGenericMatrix<2, 3> QMatrix2x3;
typedef QGenericMatrix<2, 4> QMatrix2x4;
typedef QGenericMatrix<3, 2> QMatrix3x2;
typedef QGenericMatrix<3, 3> QMatrix3x3;
typedef QGenericMatrix<3, 4> QMatrix3x4;
typedef QGenericMatrix<4, 2> QMatrix4x2;
typedef QGenericMatrix<4, 3> QMatrix4x3;

#ifndef QT_NO_DEBUG_STREAM

template <int N, int M>
QDebug operator<<(QDebug dbg, const QGenericMatrix<N, M> &m)
{
    dbg.nospace() << "QGenericMatrix<" << N << ", " << M
        << ">(" << endl << qSetFieldWidth(10);
    for (int row = 0; row < M; ++row) {
        for (int col = 0; col < N; ++col)
            dbg << m(row, col);
        dbg << endl;
    }
    dbg << qSetFieldWidth(0) << ')';
    return dbg.space();
}

#endif

#ifndef QT_NO_DATASTREAM

template <int N, int M>
QDataStream &operator<<(QDataStream &stream, const QGenericMatrix<N, M> &matrix)
{
    for (int row = 0; row < M; ++row)
        for (int col = 0; col < N; ++col)
            stream << double(matrix(row, col));
    return stream;
}

template <int N, int M>
QDataStream &operator>>(QDataStream &stream, QGenericMatrix<N, M> &matrix)
{
    double x;
    for (int row = 0; row < M; ++row) {
        for (int col = 0; col < N; ++col) {
            stream >> x;
            matrix(row, col) = qreal(x);
        }
    }
    return stream;
}

#endif

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QMatrix2x2)
Q_DECLARE_METATYPE(QMatrix2x3)
Q_DECLARE_METATYPE(QMatrix2x4)
Q_DECLARE_METATYPE(QMatrix3x2)
Q_DECLARE_METATYPE(QMatrix3x3)
Q_DECLARE_METATYPE(QMatrix3x4)
Q_DECLARE_METATYPE(QMatrix4x2)
Q_DECLARE_METATYPE(QMatrix4x3)


#endif
