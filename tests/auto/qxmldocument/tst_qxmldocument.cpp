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

//TESTED_CLASS=QXmlDocument
//TESTED_FILES=qxmldocument.cpp,qxmldocument.h

static QString testinvalidfile = QString::fromLatin1(SRCDIR "/invalidxml.xml");
static QString testxmlfile = QString::fromLatin1(SRCDIR "/qxmldocument.xml");
static QByteArray testxmldata = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<root>\n  <Qt_GUIEffects>none</Qt_GUIEffects>\n</root>";

class TestXmlDocument : public QXmlDocument
{
    bool beginElement(const QByteArray &element, const QList<QXmlAttribute> &attributes) final;
    bool cData(const QByteArray &cdata) final;
    bool endElement(const QByteArray &element) final;
};

bool TestXmlDocument::beginElement(const QByteArray &element, const QList<QXmlAttribute> &attributes)
{
    // qDebug() << Q_FUNC_INFO << element;
    foreach (const QXmlAttribute& attribute, attributes) {
        // qDebug() << Q_FUNC_INFO << attribute.name << attribute.value;
    }
    return true;
}
bool TestXmlDocument::cData(const QByteArray &cdata)
{
    // qDebug() << Q_FUNC_INFO << cdata;
    return true;
}

bool TestXmlDocument::endElement(const QByteArray &element)
{
    // qDebug() << Q_FUNC_INFO << element;
    return true;
}

class tst_QXmlDocument : public QObject
{
    Q_OBJECT

public slots:
    void init();
    void cleanup();

private slots:
    void read();
    void error();
    void eq_not_eq();
};

void tst_QXmlDocument::init()
{
}

void tst_QXmlDocument::cleanup()
{
}

void tst_QXmlDocument::read()
{
    TestXmlDocument xmldoc;
    xmldoc.parse(testxmldata);
    QVERIFY(xmldoc.errorString().isEmpty());

    QFile xmlfile(testxmlfile);
    QVERIFY(xmlfile.open(QIODevice::ReadOnly));

    xmldoc.parse(xmlfile.readAll());
    QVERIFY(xmldoc.errorString().isEmpty());
}

void tst_QXmlDocument::error()
{
    QFile xmlfile(testinvalidfile);
    QVERIFY(xmlfile.open(QIODevice::ReadOnly));
    QXmlDocument xmldoc;
    xmldoc.parse(xmlfile.readAll());
    // not testing for specific string on purpose
    QVERIFY(!xmldoc.errorString().isEmpty());
    QVERIFY(xmldoc.isNull());

    // TODO: indermediate error test, e.g. maximum depth reached
}

void tst_QXmlDocument::eq_not_eq()
{
    QXmlDocument nullxmldoc;
    QXmlDocument nullxmldoc2;
    QCOMPARE(nullxmldoc, nullxmldoc2);

    QXmlDocument xmldoc;
    xmldoc.parse(testxmldata);
    QVERIFY(nullxmldoc != xmldoc);

    // assignment crash test
    QXmlDocument xmldoc2;
    xmldoc2.parse(testxmldata);
    nullxmldoc = xmldoc;
    xmldoc2 = nullxmldoc2;

    // constructor crash test
    QXmlDocument nullxmldoc3(nullxmldoc);
    QCOMPARE(nullxmldoc3, nullxmldoc);
}

QTEST_MAIN(tst_QXmlDocument)

#include "moc_tst_qxmldocument.cpp"
