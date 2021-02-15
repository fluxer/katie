/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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

#include <QVector>
#include <QDebug>
#include <QtTest>

#define TEST_RETURN 1

class tst_QVector: public QObject
{
    Q_OBJECT

private slots:
    void calibration();

    // Pure Qt solution
    void qvector_separator() { qWarning() << "QVector results: "; }
    void qvector_const_read_access();
    void qvector_mutable_read_access();
    #ifdef TEST_RETURN
    void qvector_fill_and_return();
    #endif

    // Purre Standard solution
    void stdvector() { qWarning() << "std::vector results: "; }
    void stdvector_const_read_access();
    void stdvector_mutable_read_access();
    #ifdef TEST_RETURN
    void stdvector_fill_and_return();
    #endif

    // Build using std, pass as QVector
    void mixedvector() { qWarning() << "mixed results: "; }
    #ifdef TEST_RETURN
    void mixedvector_fill_and_return();
    #endif
};

static const int N = 1000000;
static double s;

void tst_QVector::calibration()
{
    QVector<double> v(N);
    for (int i = 0; i != N; ++i)
        v[i] = i;
    QBENCHMARK {
        for (int i = 0; i != N; ++i)
            s += i;
    }
}

///////////////////// QVector /////////////////////

void tst_QVector::qvector_const_read_access()
{
    QVector<double> v(N);
    for (int i = 0; i != N; ++i)
        v[i] = i;

    const QVector<double> &vc = v;
    QBENCHMARK {
        for (int i = 0; i != N; ++i)
            s += vc[i];
    }
}

void tst_QVector::qvector_mutable_read_access()
{
    QVector<double> v(N);
    for (int i = 0; i != N; ++i)
        v[i] = i;

    QBENCHMARK {
        for (int i = 0; i != N; ++i)
            s += v[i];
    }
}

#ifdef TEST_RETURN
extern QVector<double> qvector_fill_and_return_helper();

void tst_QVector::qvector_fill_and_return()
{
    QBENCHMARK {
        QVector<double> v = qvector_fill_and_return_helper();
        s += v[1];
    }
}
#endif

///////////////////// std::vector /////////////////////

void tst_QVector::stdvector_const_read_access()
{
    std::vector<double> v(N);
    for (int i = 0; i != N; ++i)
        v[i] = i;

    const std::vector<double> &vc = v;
    QBENCHMARK {
        for (int i = 0; i != N; ++i)
            s += vc[i];
    }
}

void tst_QVector::stdvector_mutable_read_access()
{
    std::vector<double> v(N);
    for (int i = 0; i != N; ++i)
        v[i] = i;

    QBENCHMARK {
        for (int i = 0; i != N; ++i)
            s += v[i];
    }
}

#ifdef TEST_RETURN
extern std::vector<double> stdvector_fill_and_return_helper();

void tst_QVector::stdvector_fill_and_return()
{
    QBENCHMARK {
        std::vector<double> v = stdvector_fill_and_return_helper();
        s += v[1];
    }
}
#endif

///////////////////// mixed vector /////////////////////


#ifdef TEST_RETURN
extern QVector<double> mixedvector_fill_and_return_helper();

void tst_QVector::mixedvector_fill_and_return()
{
    QBENCHMARK {
        std::vector<double> v = stdvector_fill_and_return_helper();
        s += v[1];
    }
}
#endif

QTEST_MAIN(tst_QVector)

#include "moc_main.cpp"
