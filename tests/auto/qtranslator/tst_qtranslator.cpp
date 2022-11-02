/****************************************************************************
**
** Copyright (C) 2022 Ivailo Monev
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


#include <QtTest/QtTest>
#include <QtCore/QtCore>

//TESTED_CLASS=QTranslator
//TESTED_FILES=qtranslator.cpp,qtranslator.h

#ifndef QT_NO_TRANSLATION
class tst_QTranslator : public QObject
{
    Q_OBJECT

public slots:
    void init();
    void cleanup();

private slots:
    void fromdata();
};

void tst_QTranslator::init()
{
}

void tst_QTranslator::cleanup()
{
}

void tst_QTranslator::fromdata()
{
    const QString trfilepath = QFile::encodeName(SRCDIR "/test.tr");
    QFile trfile(trfilepath);
    QVERIFY(trfile.open(QFile::ReadOnly));
    QTranslator translator;
    QVERIFY(translator.loadFromData(trfile.readAll()));
    QCOMPARE(translator.translate("", "foo"), QString::fromUtf8("фоо"));
    QCOMPARE(translator.translate("foo", "bar"), QString::fromUtf8("фообар"));
}

QTEST_MAIN(tst_QTranslator)

#include "moc_tst_qtranslator.cpp"

#else // QT_NO_TRANSLATION

QTEST_NOOP_MAIN

#endif // QT_NO_TRANSLATION
