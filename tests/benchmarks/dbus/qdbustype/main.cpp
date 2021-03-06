/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the FOO module of the Katie Toolkit.
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

#include "QtTest/QtTest"
#include "qcoreapplication.h"
#include "qdbusutil_p.h"

#include <dbus/dbus.h>

class tst_QDBusType: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void benchmarkSignature_data();
    void benchmarkSignature();
};

static inline void benchmarkAddRow(const char *name, const char *data)
{
    QByteArray nativeName = QByteArray("native-") + name;
    QTest::newRow(nativeName) << data << true;
    QTest::newRow(name) << data << false;
}

void tst_QDBusType::benchmarkSignature_data()
{
    QTest::addColumn<QString>("data");
    QTest::addColumn<bool>("useNative");

    for (int loopCount = 0; loopCount < 2; ++loopCount) {
        bool useNative = loopCount;
        QByteArray prefix = useNative ? "native-" : "";

        benchmarkAddRow("single-invalid", "~");
        benchmarkAddRow("single-invalid-array", "a~");
        benchmarkAddRow("single-invalid-struct", "(.)");

        benchmarkAddRow("single-char", "b");
        benchmarkAddRow("single-array", "as");
        benchmarkAddRow("single-simplestruct", "(y)");
        benchmarkAddRow("single-simpledict", "a{sv}");
        benchmarkAddRow("single-complexdict", "a{s(aya{io})}");

        benchmarkAddRow("multiple-char", "ssg");
        benchmarkAddRow("multiple-arrays", "asasay");

        benchmarkAddRow("struct-missingclose", "(ayyyy");
        benchmarkAddRow("longstruct", "(yyyyyyayasy)");
        benchmarkAddRow("invalid-longstruct", "(yyyyyyayas.y)");
        benchmarkAddRow("complexstruct", "(y(aasay)oga{sv})");
        benchmarkAddRow("multiple-simple-structs", "(y)(y)(y)");
    }
}

void tst_QDBusType::benchmarkSignature()
{
    QFETCH(QString, data);
    QFETCH(bool, useNative);

    bool result;
    if (useNative) {
        dbus_signature_validate(data.toLatin1(), 0);
        QBENCHMARK {
            result = dbus_signature_validate(data.toLatin1(), 0);
        }
    } else {
        QDBusUtil::isValidSignature(data);
        QBENCHMARK {
            result = QDBusUtil::isValidSignature(data);
        }
    }
    Q_UNUSED(result);
}

QTEST_MAIN(tst_QDBusType)

#include "moc_main.cpp"
