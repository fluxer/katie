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

#include <QtTest/QtTest>
#include <qfileiconprovider.h>
#include <qfileinfo.h>
#include <qdir.h>

Q_DECLARE_METATYPE(QFileIconProvider::IconType)
Q_DECLARE_METATYPE(QFileInfo)

class tst_QFileIconProvider : public QObject
{
    Q_OBJECT

private slots:
    void qfileiconprovider();

    void iconType_data();
    void iconType();

    void iconInfo_data();
    void iconInfo();

    void type_data();
    void type();
};

// Subclass that exposes the protected functions.
class SubQFileIconProvider : public QFileIconProvider
{
public:

};

void tst_QFileIconProvider::qfileiconprovider()
{
    // don't crash
    SubQFileIconProvider provider;
}

void tst_QFileIconProvider::iconType_data()
{
    QTest::addColumn<QFileIconProvider::IconType>("type");

    QTest::newRow("computer") << QFileIconProvider::Computer;
    QTest::newRow("desktop") << QFileIconProvider::Desktop;
    QTest::newRow("trashcan") << QFileIconProvider::Trashcan;
    QTest::newRow("network") << QFileIconProvider::Network;
    QTest::newRow("drive") << QFileIconProvider::Drive;
    QTest::newRow("folder") << QFileIconProvider::Folder;
    QTest::newRow("file") << QFileIconProvider::File;
}

// public QIcon icon(QFileIconProvider::IconType const& type) const
void tst_QFileIconProvider::iconType()
{
    QFETCH(QFileIconProvider::IconType, type);

    const QString icontheme = QIcon::themeName();
    if (icontheme.isEmpty() || icontheme == QLatin1String("hicolor")) {
        QSKIP("This test requires system icon theme", SkipAll);
    }

    SubQFileIconProvider provider;
    QVERIFY(!provider.icon(type).isNull());
}

void tst_QFileIconProvider::iconInfo_data()
{
    QTest::addColumn<QFileInfo>("info");
    QTest::addColumn<bool>("setPath");

    QTest::newRow("null") << QFileInfo() << false;
    QTest::newRow("drive") << QFileInfo(QDir::rootPath()) << true;
    QTest::newRow("home") << QFileInfo(QDir::homePath()) << true;
    QTest::newRow("current") << QFileInfo(QDir::currentPath()) << true;
}

// public QIcon icon(QFileInfo const& info) const
void tst_QFileIconProvider::iconInfo()
{
    QFETCH(QFileInfo, info);
    QFETCH(bool, setPath);

    const QString icontheme = QIcon::themeName();
    if (icontheme.isEmpty() || icontheme == QLatin1String("hicolor")) {
        QSKIP("This test requires system icon theme", SkipAll);
    }

    if (setPath) {
        QVERIFY(info.exists());
    }
    SubQFileIconProvider provider;
    QVERIFY(!provider.icon(info).isNull());
}

void tst_QFileIconProvider::type_data()
{
    QTest::addColumn<QFileInfo>("info");
    // Return value is _very_ system dependent, hard to test
    // QTest::addColumn<QString>("type");

    QTest::newRow("null") << QFileInfo();
    QTest::newRow("drive") << QFileInfo(QDir::rootPath());
    QTest::newRow("home") << QFileInfo(QDir::homePath());
    QTest::newRow("current") << QFileInfo(QDir::currentPath());
    QTest::newRow("exe") << QFileInfo(QCoreApplication::applicationFilePath());
}

// public QString type(QFileInfo const& info) const
void tst_QFileIconProvider::type()
{
    QFETCH(QFileInfo, info);
    SubQFileIconProvider provider;
    QVERIFY(!provider.type(info).isEmpty());
}

QTEST_MAIN(tst_QFileIconProvider)

#include "moc_tst_qfileiconprovider.cpp"
