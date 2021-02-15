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

#include <QtCore/QCoreApplication>
#include <QtCore/QUuid>
#include <QtTest/QtTest>

QT_USE_NAMESPACE

class tst_bench_QUuid : public QObject
{
    Q_OBJECT

public:
    tst_bench_QUuid()
    { }

private slots:
    void createUuid();
    void fromChar();
    void toString();
    void fromString();
    void toByteArray();
    void fromByteArray();
    void toRfc4122();
    void fromRfc4122();
    void toDataStream();
    void fromDataStream();
    void isNull();
    void operatorLess();
    void operatorMore();
};

void tst_bench_QUuid::createUuid()
{
    QBENCHMARK {
        QUuid::createUuid();
    }
}

void tst_bench_QUuid::fromChar()
{
    QBENCHMARK {
        QUuid uuid("{67C8770B-44F1-410A-AB9A-F9B5446F13EE}");
    }
}

void tst_bench_QUuid::toString()
{
    QUuid uuid = QUuid::createUuid();
    QBENCHMARK {
        uuid.toString();
    }
}

void tst_bench_QUuid::fromString()
{
    QString string = QLatin1String("{67C8770B-44F1-410A-AB9A-F9B5446F13EE}");
    QBENCHMARK {
        QUuid uuid(string);
    }
}

void tst_bench_QUuid::toByteArray()
{
    QUuid uuid = QUuid::createUuid();
    QBENCHMARK {
        uuid.toByteArray();
    }
}

void tst_bench_QUuid::fromByteArray()
{
    QByteArray string = "{67C8770B-44F1-410A-AB9A-F9B5446F13EE}";
    QBENCHMARK {
        QUuid uuid(string);
    }
}

void tst_bench_QUuid::toRfc4122()
{
    QUuid uuid = QUuid::createUuid();
    QBENCHMARK {
        uuid.toRfc4122();
    }
}

void tst_bench_QUuid::fromRfc4122()
{
    QByteArray string = QByteArray::fromHex("67C8770B44F1410AAB9AF9B5446F13EE");
    QBENCHMARK {
        QUuid::fromRfc4122(string);
    }
}

void tst_bench_QUuid::toDataStream()
{
    QUuid uuid1, uuid2;
    uuid1 = QUuid::createUuid();
    QByteArray ar;
    {
        QDataStream out(&ar,QIODevice::WriteOnly);
        QBENCHMARK {
            out << uuid1;
        }
    }
}

void tst_bench_QUuid::fromDataStream()
{
    QUuid uuid1, uuid2;
    uuid1 = QUuid::createUuid();
    QByteArray ar;
    {
        QDataStream out(&ar,QIODevice::WriteOnly);
        out << uuid1;
    }
    {
        QDataStream in(&ar,QIODevice::ReadOnly);
        QBENCHMARK {
            in >> uuid2;
        }
    }
}

void tst_bench_QUuid::isNull()
{
    QUuid uuid = QUuid();
    QBENCHMARK {
        uuid.isNull();
    }
}

void tst_bench_QUuid::operatorLess()
{
    QUuid uuid1, uuid2;
    uuid1 = QUuid::createUuid();
    uuid2 = QUuid::createUuid();
    QBENCHMARK {
        (void)(uuid1 < uuid2);
    }
}

void tst_bench_QUuid::operatorMore()
{
    QUuid uuid1, uuid2;
    uuid1 = QUuid::createUuid();
    uuid2 = QUuid::createUuid();
    QBENCHMARK {
        (void)(uuid1 > uuid2);
    }
}

QTEST_MAIN(tst_bench_QUuid);

#include "moc_tst_quuid.cpp"
