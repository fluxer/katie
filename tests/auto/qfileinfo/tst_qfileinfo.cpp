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


#include <qfile.h>
#include <qdir.h>
#include <qcoreapplication.h>
#include <qlibrary.h>
#include <qtemporaryfile.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qplatformdefs.h>
#include <qdebug.h>
#include <qfileinfo_p.h>
#include "../../shared/filesystem.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

// #define NO_SYMLINKS

//TESTED_CLASS=
//TESTED_FILES=

class tst_QFileInfo : public QObject
{
Q_OBJECT

public:
    tst_QFileInfo();
    ~tst_QFileInfo();

private slots:
    void getSetCheck();

    void copy();

    void isFile_data();
    void isFile();

    void isDir_data();
    void isDir();

    void isRoot_data();
    void isRoot();

    void exists_data();
    void exists();

    void absolutePath_data();
    void absolutePath();

    void absFilePath_data();
    void absFilePath();

    void canonicalPath();
    void canonicalFilePath();

    void fileName_data();
    void fileName();

    void dir_data();
    void dir();

    void suffix_data();
    void suffix();

    void completeSuffix_data();
    void completeSuffix();

    void baseName_data();
    void baseName();

    void completeBaseName_data();
    void completeBaseName();

    void permission_data();
    void permission();

    void size_data();
    void size();

    void compare_data();
    void compare();

    void consistent_data();
    void consistent();

    void fileTimes_data();
    void fileTimes();

    void isSymLink_data();
    void isSymLink();

    void isHidden_data();
    void isHidden();

    void isLocalFs_data();
    void isLocalFs();

    void refresh();

    void isWritable();
    void isExecutable();
    void testDecomposedUnicodeNames_data();
    void testDecomposedUnicodeNames();

    void equalOperator() const;
    void equalOperatorWithDifferentSlashes() const;
    void notEqualOperator() const;

    void detachingOperations();

    void owner();
    void group();

    void invalidState();
};

tst_QFileInfo::tst_QFileInfo()
{
}

tst_QFileInfo::~tst_QFileInfo()
{
    QFile::remove("brokenlink.lnk");
    QFile::remove("link.lnk");
    QFile::remove("file1");
    QFile::remove("dummyfile");
    QFile::remove("simplefile.txt");
    QFile::remove("longFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileNamelongFileName.txt");
    QFile::remove("tempfile.txt");

    QDir().rmdir("./.hidden-directory");
    QFile::remove("link_to_tst_qfileinfo");
}

// Testing get/set functions
void tst_QFileInfo::getSetCheck()
{
    QFileInfo obj1;
    // bool QFileInfo::caching()
    // void QFileInfo::setCaching(bool)
    obj1.setCaching(false);
    QCOMPARE(false, obj1.caching());
    obj1.setCaching(true);
    QCOMPARE(true, obj1.caching());
}

static QFileInfoPrivate* getPrivate(QFileInfo &info)
{
    return (*reinterpret_cast<QFileInfoPrivate**>(&info));
}

void tst_QFileInfo::copy()
{
    QTemporaryFile t;
    t.open();
    QFileInfo info(t.fileName());
    QVERIFY(info.exists());

    //copy constructor
    QFileInfo info2(info);
    QFileInfoPrivate *privateInfo = getPrivate(info);
    QFileInfoPrivate *privateInfo2 = getPrivate(info2);
    QCOMPARE(privateInfo, privateInfo2);

    //operator =
    QFileInfo info3 = info;
    QFileInfoPrivate *privateInfo3 = getPrivate(info3);
    QCOMPARE(privateInfo, privateInfo3);
    QCOMPARE(privateInfo2, privateInfo3);

    //refreshing info3 will detach it
    QFile file(info.absoluteFilePath());
    QVERIFY(file.open(QFile::WriteOnly));
    QCOMPARE(file.write("JAJAJAA"), qint64(7));
    file.flush();

    QTest::qWait(250);
    info3.refresh();
    privateInfo3 = getPrivate(info3);
    QVERIFY(privateInfo != privateInfo3);
    QVERIFY(privateInfo2 != privateInfo3);
    QCOMPARE(privateInfo, privateInfo2);
}

void tst_QFileInfo::isFile_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("expected");

    QTest::newRow("data0") << QDir::currentPath() << false;
    QTest::newRow("data1") << SRCDIR "tst_qfileinfo.cpp" << true;
    QTest::newRow("data2") << ":/tst_qfileinfo/resources/" << false;
    QTest::newRow("data3") << ":/tst_qfileinfo/resources/file1" << true;
    QTest::newRow("data4") << ":/tst_qfileinfo/resources/afilethatshouldnotexist" << false;
}

void tst_QFileInfo::isFile()
{
    QFETCH(QString, path);
    QFETCH(bool, expected);

    QFileInfo fi(path);
    QCOMPARE(fi.isFile(), expected);
}


void tst_QFileInfo::isDir_data()
{
    // create a broken symlink
    QFile::remove("brokenlink.lnk");
    QFile::remove("dummyfile");
    QFile file3("dummyfile");
    file3.open(QIODevice::WriteOnly);
    if (file3.link("brokenlink.lnk")) {
        file3.remove();
        QFileInfo info3("brokenlink.lnk");
        QVERIFY( info3.isSymLink() );
    }

    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("expected");

    QTest::newRow("data0") << QDir::currentPath() << true;
    QTest::newRow("data1") << SRCDIR "tst_qfileinfo.cpp" << false;
    QTest::newRow("data2") << ":/tst_qfileinfo/resources/" << true;
    QTest::newRow("data3") << ":/tst_qfileinfo/resources/file1" << false;
    QTest::newRow("data4") << ":/tst_qfileinfo/resources/afilethatshouldnotexist" << false;

    QTest::newRow("simple dir") << SRCDIR "resources" << true;
    QTest::newRow("simple dir with slash") << SRCDIR "resources/" << true;

    QTest::newRow("broken link") << "brokenlink.lnk" << false;
}

void tst_QFileInfo::isDir()
{
    QFETCH(QString, path);
    QFETCH(bool, expected);

    QFileInfo fi(path);
    QCOMPARE(fi.isDir(), expected);
}

void tst_QFileInfo::isRoot_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("expected");
    QTest::newRow("data0") << QDir::currentPath() << false;
    QTest::newRow("data1") << "/" << true;
    QTest::newRow("data2") << "*" << false;
    QTest::newRow("data3") << "/*" << false;
    QTest::newRow("data4") << ":/tst_qfileinfo/resources/" << false;
    QTest::newRow("data5") << ":/" << true;

    QTest::newRow("simple dir") << SRCDIR "resources" << false;
    QTest::newRow("simple dir with slash") << SRCDIR "resources/" << false;
}

void tst_QFileInfo::isRoot()
{
    QFETCH(QString, path);
    QFETCH(bool, expected);

    QFileInfo fi(path);
    QCOMPARE(fi.isRoot(), expected);
}

void tst_QFileInfo::exists_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("expected");

    QTest::newRow("data0") << QDir::currentPath() << true;
    QTest::newRow("data1") << SRCDIR "tst_qfileinfo.cpp" << true;
    QTest::newRow("data2") << "/I/do_not_expect_this_path_to_exist/" << false;
    QTest::newRow("data3") << ":/tst_qfileinfo/resources/" << true;
    QTest::newRow("data4") << ":/tst_qfileinfo/resources/file1" << true;
    QTest::newRow("data5") << ":/I/do_not_expect_this_path_to_exist/" << false;
    QTest::newRow("data6") << SRCDIR "resources/*" << false;
    QTest::newRow("data7") << SRCDIR "resources/*.foo" << false;
    QTest::newRow("data8") << SRCDIR "resources/*.ext1" << false;
    QTest::newRow("data9") << SRCDIR "resources/file?.ext1" << false;
    QTest::newRow("data10") << "." << true;
    QTest::newRow("data11") << ". " << false;
    QTest::newRow("empty") << "" << false;

    QTest::newRow("simple dir") << SRCDIR "resources" << true;
    QTest::newRow("simple dir with slash") << SRCDIR "resources/" << true;
}

void tst_QFileInfo::exists()
{
    QFETCH(QString, path);
    QFETCH(bool, expected);

    QFileInfo fi(path);
    QCOMPARE(fi.exists(), expected);
}

void tst_QFileInfo::absolutePath_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("path");
    QTest::addColumn<QString>("filename");

    QString drivePrefix;
    QTest::newRow("0") << "/machine/share/dir1/" << drivePrefix + "/machine/share/dir1" << "";
    QTest::newRow("1") << "/machine/share/dir1" << drivePrefix + "/machine/share" << "dir1";
    QTest::newRow("2") << "/usr/local/bin" << drivePrefix + "/usr/local" << "bin";
    QTest::newRow("3") << "/usr/local/bin/" << drivePrefix + "/usr/local/bin" << "";
    QTest::newRow("/test") << "/test" << drivePrefix + "/" << "test";

    QTest::newRow("QTBUG-19995.1") << drivePrefix + "/System/Library/StartupItems/../Frameworks"
                                   << drivePrefix + "/System/Library"
                                   << "Frameworks";
    QTest::newRow("QTBUG-19995.2") << drivePrefix + "/System/Library/StartupItems/../Frameworks/"
                                   << drivePrefix + "/System/Library/Frameworks" << "";
}

void tst_QFileInfo::absolutePath()
{
    QFETCH(QString, file);
    QFETCH(QString, path);
    QFETCH(QString, filename);

    QFileInfo fi(file);

    QCOMPARE(fi.absolutePath(), path);
    QCOMPARE(fi.fileName(), filename);
}

void tst_QFileInfo::absFilePath_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("expected");

    QTest::newRow("relativeFile") << "tmp.txt" << QDir::currentPath() + "/tmp.txt";
    QTest::newRow("relativeFileInSubDir") << "temp/tmp.txt" << QDir::currentPath() + "/" + "temp/tmp.txt";
    QString drivePrefix;
    QTest::newRow("absFilePath") << "/home/andy/tmp.txt" << "/home/andy/tmp.txt";
    QTest::newRow("QTBUG-19995") << drivePrefix + "/System/Library/StartupItems/../Frameworks"
                                 << drivePrefix + "/System/Library/Frameworks";
}

void tst_QFileInfo::absFilePath()
{
    QFETCH(QString, file);
    QFETCH(QString, expected);

    QFileInfo fi(file);
    QCOMPARE(fi.absoluteFilePath(), expected);
}

void tst_QFileInfo::canonicalPath()
{
    QTemporaryFile tempFile;
    tempFile.setAutoRemove(true);
    tempFile.open();
    QFileInfo fi(tempFile.fileName());
    QCOMPARE(fi.canonicalPath(), QFileInfo(QDir::tempPath()).canonicalFilePath());
}

void tst_QFileInfo::canonicalFilePath()
{
    const QString fileName("tmp.canon");
    QFile tempFile(fileName);
    QVERIFY(tempFile.open(QFile::WriteOnly));
    QFileInfo fi(tempFile.fileName());
    QCOMPARE(fi.canonicalFilePath(), QDir::currentPath() + "/" + fileName);
    tempFile.remove();

    // This used to crash, verify that it doesn't anymore.
    QFileInfo info("/tmp/../../../../../../../../../../../../../../../../../");
    info.canonicalFilePath();

    QFileInfo dontCrash(QLatin1String("/"));
    QCOMPARE(dontCrash.canonicalFilePath(), QLatin1String("/"));

    // test symlinks
    QFile::remove("link.lnk");
    {
        QFile file(SRCDIR "tst_qfileinfo.cpp");
        if (file.link("link.lnk")) {
            QFileInfo info1(file);
            QFileInfo info2("link.lnk");
            QCOMPARE(info1.canonicalFilePath(), info2.canonicalFilePath());
        }
    }
    {
        const QString link(QDir::tempPath() + QDir::separator() + "tst_qfileinfo");
        QFile::remove(link);
        QFile file(QDir::currentPath());
        if (file.link(link)) {
            QFile tempfile("tempfile.txt");
            tempfile.open(QIODevice::ReadWrite);
            tempfile.write("This file is generated by the QFileInfo autotest.");
            QVERIFY(tempfile.flush());
            tempfile.close();

            QFileInfo info1("tempfile.txt");
            QFileInfo info2(link + QDir::separator() + "tempfile.txt");

            QVERIFY(info1.exists());
            QVERIFY(info2.exists());
            QCOMPARE(info1.canonicalFilePath(), info2.canonicalFilePath());

            QFileInfo info3(link + QDir::separator() + "link.lnk");
            QFileInfo info4(SRCDIR "tst_qfileinfo.cpp");
            QVERIFY(!info3.canonicalFilePath().isEmpty());
            QCOMPARE(info4.canonicalFilePath(), info3.canonicalFilePath());

            tempfile.remove();
        }
    }
    {
        QString link(QDir::tempPath() + QDir::separator() + "tst_qfileinfo"
                     + QDir::separator() + "link_to_tst_qfileinfo");
        QFile::remove(link);

        QFile file(QDir::tempPath() + QDir::separator() + "tst_qfileinfo"
                   + QDir::separator() + "tst_qfileinfo.cpp");
        if (file.link(link))
        {
            QFileInfo info1("tst_qfileinfo.cpp");
            QFileInfo info2(link);
            QCOMPARE(info1.canonicalFilePath(), info2.canonicalFilePath());
        }
    }
}

void tst_QFileInfo::fileName_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("expected");

    QTest::newRow("relativeFile") << "tmp.txt" << "tmp.txt";
    QTest::newRow("relativeFileInSubDir") << "temp/tmp.txt" << "tmp.txt";
    QTest::newRow("absFilePath") << "/home/andy/tmp.txt" << "tmp.txt";
    QTest::newRow("resource1") << ":/tst_qfileinfo/resources/file1.ext1" << "file1.ext1";
    QTest::newRow("resource2") << ":/tst_qfileinfo/resources/file1.ext1.ext2" << "file1.ext1.ext2";

    QTest::newRow("ending slash [small]") << QString::fromLatin1("/a/") << QString::fromLatin1("");
    QTest::newRow("no ending slash [small]") << QString::fromLatin1("/a") << QString::fromLatin1("a");

    QTest::newRow("ending slash") << QString::fromLatin1("/somedir/") << QString::fromLatin1("");
    QTest::newRow("no ending slash") << QString::fromLatin1("/somedir") << QString::fromLatin1("somedir");
}

void tst_QFileInfo::fileName()
{
    QFETCH(QString, file);
    QFETCH(QString, expected);

    QFileInfo fi(file);
    QCOMPARE(fi.fileName(), expected);
}

void tst_QFileInfo::dir_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<bool>("absPath");
    QTest::addColumn<QString>("expected");

    QTest::newRow("relativeFile") << "tmp.txt" << false << ".";
    QTest::newRow("relativeFileAbsPath") << "tmp.txt" << true << QDir::currentPath();
    QTest::newRow("relativeFileInSubDir") << "temp/tmp.txt" << false << "temp";
    QTest::newRow("relativeFileInSubDirAbsPath") << "temp/tmp.txt" << true << QDir::currentPath() + "/temp";
    QTest::newRow("absFilePath") << QDir::currentPath() + "/tmp.txt" << false << QDir::currentPath();
    QTest::newRow("absFilePathAbsPath") << QDir::currentPath() + "/tmp.txt" << true << QDir::currentPath();
    QTest::newRow("resource1") << ":/tst_qfileinfo/resources/file1.ext1" << true << ":/tst_qfileinfo/resources";
}

void tst_QFileInfo::dir()
{
    QFETCH(QString, file);
    QFETCH(bool, absPath);
    QFETCH(QString, expected);

    QFileInfo fi(file);
    if (absPath) {
        QCOMPARE(fi.absolutePath(), expected);
        QCOMPARE(fi.absoluteDir().path(), expected);
    } else {
        QCOMPARE(fi.path(), expected);
        QCOMPARE(fi.dir().path(), expected);
    }
}


void tst_QFileInfo::suffix_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("expected");

    QTest::newRow("noextension0") << "file" << "";
    QTest::newRow("noextension1") << "/path/to/file" << "";
    QTest::newRow("data0") << "file.tar" << "tar";
    QTest::newRow("data1") << "file.tar.gz" << "gz";
    QTest::newRow("data2") << "/path/file/file.tar.gz" << "gz";
    QTest::newRow("data3") << "/path/file.tar" << "tar";
    QTest::newRow("resource1") << ":/tst_qfileinfo/resources/file1.ext1" << "ext1";
    QTest::newRow("resource2") << ":/tst_qfileinfo/resources/file1.ext1.ext2" << "ext2";
    QTest::newRow("hidden1") << ".ext1" << "ext1";
    QTest::newRow("hidden1") << ".ext" << "ext";
    QTest::newRow("hidden1") << ".ex" << "ex";
    QTest::newRow("hidden1") << ".e" << "e";
    QTest::newRow("hidden2") << ".ext1.ext2" << "ext2";
    QTest::newRow("hidden2") << ".ext.ext2" << "ext2";
    QTest::newRow("hidden2") << ".ex.ext2" << "ext2";
    QTest::newRow("hidden2") << ".e.ext2" << "ext2";
    QTest::newRow("hidden2") << "..ext2" << "ext2";
}

void tst_QFileInfo::suffix()
{
    QFETCH(QString, file);
    QFETCH(QString, expected);

    QFileInfo fi(file);
    QCOMPARE(fi.suffix(), expected);
}


void tst_QFileInfo::completeSuffix_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("expected");

    QTest::newRow("noextension0") << "file" << "";
    QTest::newRow("noextension1") << "/path/to/file" << "";
    QTest::newRow("data0") << "file.tar" << "tar";
    QTest::newRow("data1") << "file.tar.gz" << "tar.gz";
    QTest::newRow("data2") << "/path/file/file.tar.gz" << "tar.gz";
    QTest::newRow("data3") << "/path/file.tar" << "tar";
    QTest::newRow("resource1") << ":/tst_qfileinfo/resources/file1.ext1" << "ext1";
    QTest::newRow("resource2") << ":/tst_qfileinfo/resources/file1.ext1.ext2" << "ext1.ext2";
}

void tst_QFileInfo::completeSuffix()
{
    QFETCH(QString, file);
    QFETCH(QString, expected);

    QFileInfo fi(file);
    QCOMPARE(fi.completeSuffix(), expected);
}

void tst_QFileInfo::baseName_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("expected");

    QTest::newRow("data0") << "file.tar" << "file";
    QTest::newRow("data1") << "file.tar.gz" << "file";
    QTest::newRow("data2") << "/path/file/file.tar.gz" << "file";
    QTest::newRow("data3") << "/path/file.tar" << "file";
    QTest::newRow("data4") << "/path/file" << "file";
    QTest::newRow("resource1") << ":/tst_qfileinfo/resources/file1.ext1" << "file1";
    QTest::newRow("resource2") << ":/tst_qfileinfo/resources/file1.ext1.ext2" << "file1";
}

void tst_QFileInfo::baseName()
{
    QFETCH(QString, file);
    QFETCH(QString, expected);

    QFileInfo fi(file);
    QCOMPARE(fi.baseName(), expected);
}

void tst_QFileInfo::completeBaseName_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("expected");

    QTest::newRow("data0") << "file.tar" << "file";
    QTest::newRow("data1") << "file.tar.gz" << "file.tar";
    QTest::newRow("data2") << "/path/file/file.tar.gz" << "file.tar";
    QTest::newRow("data3") << "/path/file.tar" << "file";
    QTest::newRow("data4") << "/path/file" << "file";
    QTest::newRow("resource1") << ":/tst_qfileinfo/resources/file1.ext1" << "file1";
    QTest::newRow("resource2") << ":/tst_qfileinfo/resources/file1.ext1.ext2" << "file1.ext1";
}

void tst_QFileInfo::completeBaseName()
{
    QFETCH(QString, file);
    QFETCH(QString, expected);

    QFileInfo fi(file);
    QCOMPARE(fi.completeBaseName(), expected);
}

void tst_QFileInfo::permission_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<int>("perms");
    QTest::addColumn<bool>("expected");

    QTest::newRow("data0") << QCoreApplication::instance()->applicationFilePath() << int(QFile::ExeUser) << true;
    QTest::newRow("data1") << SRCDIR "tst_qfileinfo.cpp" << int(QFile::ReadUser) << true;
//    QTest::newRow("data2") << "tst_qfileinfo.cpp" << int(QFile::WriteUser) << false;
    QTest::newRow("resource1") << ":/tst_qfileinfo/resources/file1.ext1" << int(QFile::ReadUser) << true;
    QTest::newRow("resource2") << ":/tst_qfileinfo/resources/file1.ext1" << int(QFile::WriteUser) << false;
    QTest::newRow("resource3") << ":/tst_qfileinfo/resources/file1.ext1" << int(QFile::ExeUser) << false;
}

void tst_QFileInfo::permission()
{
    QFETCH(QString, file);
    QFETCH(int, perms);
    QFETCH(bool, expected);
    QFileInfo fi(file);
    QCOMPARE(fi.permission(QFile::Permissions(perms)), expected);
}

void tst_QFileInfo::size_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<int>("size");

    QTest::newRow("resource1") << ":/tst_qfileinfo/resources/file1.ext1" << 0;
    QFile::remove("file1");
    QFile file("file1");
    QVERIFY(file.open(QFile::WriteOnly));
    QCOMPARE(file.write("JAJAJAA"), qint64(7));
    QTest::newRow("created-file") << "file1" << 7;

    QTest::newRow("resource2") << ":/tst_qfileinfo/resources/file1.ext1.ext2" << 0;
}

void tst_QFileInfo::size()
{
    QFETCH(QString, file);

    QFileInfo fi(file);
    (void)fi.permissions();     // see task 104198
    QTEST(int(fi.size()), "size");
}

void tst_QFileInfo::compare_data()
{
    QTest::addColumn<QString>("file1");
    QTest::addColumn<QString>("file2");
    QTest::addColumn<bool>("same");

    QTest::newRow("data0")
        << QString::fromLatin1(SRCDIR "tst_qfileinfo.cpp")
        << QString::fromLatin1(SRCDIR "tst_qfileinfo.cpp")
        << true;
    QTest::newRow("data1")
        << QString::fromLatin1(SRCDIR "tst_qfileinfo.cpp")
        << QString::fromLatin1("/tst_qfileinfo.cpp")
        << false;
    QTest::newRow("data2")
        << QString::fromLatin1("tst_qfileinfo.cpp")
        << QDir::currentPath() + QString::fromLatin1("/tst_qfileinfo.cpp")
        << true;
    QTest::newRow("casesense1")
        << QString::fromLatin1(SRCDIR "tst_qfileInfo.cpp")
        << QString::fromLatin1(SRCDIR "tst_qfileinfo.cpp")
        << false;
}

void tst_QFileInfo::compare()
{
    QFETCH(QString, file1);
    QFETCH(QString, file2);
    QFETCH(bool, same);
    QFileInfo fi1(file1), fi2(file2);
    QCOMPARE(fi1 == fi2, same);
}

void tst_QFileInfo::consistent_data()
{
    QTest::addColumn<QString>("file");
    QTest::addColumn<QString>("expected");

    QTest::newRow("ending slash") << QString::fromLatin1("/a/somedir/") << QString::fromLatin1("/a/somedir/");
    QTest::newRow("no ending slash") << QString::fromLatin1("/a/somedir") << QString::fromLatin1("/a/somedir");
}

void tst_QFileInfo::consistent()
{
    QFETCH(QString, file);
    QFETCH(QString, expected);

    QFileInfo fi(file);
    QCOMPARE(fi.filePath(), expected);
    QCOMPARE(fi.dir().path() + "/" + fi.fileName(), expected);
}


void tst_QFileInfo::fileTimes_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::newRow("simple") << QString::fromLatin1("simplefile.txt");
    QTest::newRow( "longfile" ) << QString::fromLatin1("longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName.txt");
    QTest::newRow( "longfile absolutepath" ) << QFileInfo(QString::fromLatin1("longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName"
                                                     "longFileNamelongFileNamelongFileNamelongFileName.txt")).absoluteFilePath();
}

void tst_QFileInfo::fileTimes()
{
    int sleepTime = 2000;
    QFETCH(QString, fileName);
    if (QFile::exists(fileName)) {
        QVERIFY(QFile::remove(fileName));
    }
    QTest::qSleep(sleepTime);
    {
        QFile file(fileName);
        QVERIFY(file.open(QFile::WriteOnly | QFile::Text));
        QTextStream ts(&file);
        ts << fileName << endl;
    }
    QTest::qSleep(sleepTime);
    QDateTime beforeWrite = QDateTime::currentDateTime();
    QTest::qSleep(sleepTime);
    {
        QFileInfo fileInfo(fileName);
        QVERIFY(fileInfo.created() < beforeWrite);
        QFile file(fileName);
        QVERIFY(file.open(QFile::ReadWrite | QFile::Text));
        QTextStream ts(&file);
        ts << fileName << endl;
    }
    QTest::qSleep(sleepTime);
    QDateTime beforeRead = QDateTime::currentDateTime();
    QTest::qSleep(sleepTime);
    {
        QFileInfo fileInfo(fileName);
        QVERIFY(fileInfo.lastModified() > beforeWrite);
        QFile file(fileName);
        QVERIFY(file.open(QFile::ReadOnly | QFile::Text));
        QTextStream ts(&file);
        QString line = ts.readLine();
        QCOMPARE(line, fileName);
    }

    QFileInfo fileInfo(fileName);
    QVERIFY(fileInfo.lastRead() > beforeRead);
    QVERIFY(fileInfo.lastModified() > beforeWrite);
    QVERIFY(fileInfo.lastModified() < beforeRead);
}

void tst_QFileInfo::isSymLink_data()
{
#ifndef NO_SYMLINKS
    QFile::remove("link.lnk");
    QFile::remove("brokenlink.lnk");
    QFile::remove("dummyfile");

    QFile file1(SRCDIR "tst_qfileinfo.cpp");
    QVERIFY(file1.link("link.lnk"));

    QFile file2("dummyfile");
    file2.open(QIODevice::WriteOnly);
    QVERIFY(file2.link("brokenlink.lnk"));
    file2.remove();

    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("isSymLink");
    QTest::addColumn<QString>("linkTarget");

    QTest::newRow("existent file") << SRCDIR "tst_qfileinfo.cpp" << false << "";
    QTest::newRow("link") << "link.lnk" << true << QFileInfo(SRCDIR "tst_qfileinfo.cpp").absoluteFilePath();
    QTest::newRow("broken link") << "brokenlink.lnk" << true << QFileInfo("dummyfile").absoluteFilePath();
#endif
}

void tst_QFileInfo::isSymLink()
{
#ifndef NO_SYMLINKS
    QFETCH(QString, path);
    QFETCH(bool, isSymLink);
    QFETCH(QString, linkTarget);

    QFileInfo fi(path);
    QCOMPARE(fi.isSymLink(), isSymLink);
    QCOMPARE(fi.readLink(), linkTarget);
#else
    QSKIP("no symbolic link support on this platform", SkipAll);
#endif
}

void tst_QFileInfo::isHidden_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("isHidden");

    QVERIFY(QDir("./.hidden-directory").exists() || QDir().mkdir("./.hidden-directory"));
    QTest::newRow("/path/to/.hidden-directory") << QDir::currentPath() + QString("/.hidden-directory") << true;
    QTest::newRow("/path/to/.hidden-directory/.") << QDir::currentPath() + QString("/.hidden-directory/.") << true;
    QTest::newRow("/path/to/.hidden-directory/..") << QDir::currentPath() + QString("/.hidden-directory/..") << true;

    QTest::newRow("/bin/") << QString::fromLatin1("/bin/") << false;
}

void tst_QFileInfo::isHidden()
{
    QFETCH(QString, path);
    QFETCH(bool, isHidden);
    QFileInfo fi(path);

    QCOMPARE(fi.isHidden(), isHidden);
}

void tst_QFileInfo::isLocalFs_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("isLocalFs");

    QTest::newRow("local root") << QString::fromLatin1("/") << true;
    QTest::newRow("local non-existent file") << QString::fromLatin1("/abrakadabra.boo") << true;

    QTest::newRow("qresource root") << QString::fromLatin1(":/") << false;
}

void tst_QFileInfo::isLocalFs()
{
    QFETCH(QString, path);
    QFETCH(bool, isLocalFs);

    QFileInfo info(path);
    QFileInfoPrivate *privateInfo = getPrivate(info);
    QCOMPARE(bool(privateInfo->fileEngine->fileFlags(QAbstractFileEngine::LocalDiskFlag)
                     & QAbstractFileEngine::LocalDiskFlag), isLocalFs);
}

void tst_QFileInfo::refresh()
{
    int sleepTime = 2000;

    QFile::remove("file1");
    QFile file("file1");
    QVERIFY(file.open(QFile::WriteOnly));
    QCOMPARE(file.write("JAJAJAA"), qint64(7));
    file.flush();

    QFileInfo info(file);
    QDateTime lastModified = info.lastModified();
    QCOMPARE(info.size(), qint64(7));

    QTest::qSleep(sleepTime);

    QCOMPARE(file.write("JOJOJO"), qint64(6));
    file.flush();
    QCOMPARE(info.lastModified(), lastModified);

    QCOMPARE(info.size(), qint64(7));
    info.refresh();
    QCOMPARE(info.size(), qint64(13));
    QVERIFY(info.lastModified() > lastModified);

    QFileInfo info2 = info;
    QCOMPARE(info2.size(), info.size());

    info2.refresh();
    QCOMPARE(info2.size(), info.size());
}

void tst_QFileInfo::isWritable()
{
    QFile tempfile("tempfile.txt");
    tempfile.open(QIODevice::WriteOnly);
    tempfile.write("This file is generated by the QFileInfo autotest.");
    tempfile.close();

    QVERIFY(QFileInfo("tempfile.txt").isWritable());
    tempfile.remove();

    if (::getuid() == 0)
        QVERIFY(QFileInfo("/etc/passwd").isWritable());
    else
        QVERIFY(!QFileInfo("/etc/passwd").isWritable());
}

void tst_QFileInfo::isExecutable()
{
    QFileInfo fi(QCoreApplication::applicationFilePath());
    QCOMPARE(fi.isExecutable(), true);

    QCOMPARE(QFileInfo(SRCDIR "/CMakeLists.txt").isExecutable(), false);
}


void tst_QFileInfo::testDecomposedUnicodeNames_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<bool>("exists");
    QString currPath = QDir::currentPath();
    QTest::newRow("latin-only") << currPath + "/4.pdf" << "4.pdf" << true;
    QTest::newRow("one-decomposed uni") << currPath + QString::fromUtf8("/4 ä.pdf") << QString::fromUtf8("4 ä.pdf") << true;
    QTest::newRow("many-decomposed uni") << currPath + QString::fromUtf8("/4 äääcopy.pdf") << QString::fromUtf8("4 äääcopy.pdf") << true;
    QTest::newRow("no decomposed") << currPath + QString::fromUtf8("/4 øøøcopy.pdf") << QString::fromUtf8("4 øøøcopy.pdf") << true;
}

static void createFileNative(const QString &filePath)
{
    int fd = open(filePath.normalized(QString::NormalizationForm_D).toUtf8().constData(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        QFAIL("couldn't create file");
    } else {
        close(fd);
    }
}

static void removeFileNative(const QString &filePath)
{
    unlink(filePath.normalized(QString::NormalizationForm_D).toUtf8().constData());
}

void tst_QFileInfo::testDecomposedUnicodeNames()
{
    QSKIP("This is a OS X only test (unless you know more about filesystems, then maybe you should try it ;)", SkipAll);
    QFETCH(QString, filePath);
    createFileNative(filePath);

    QFileInfo file(filePath);
    QTEST(file.fileName(), "fileName");
    QTEST(file.exists(), "exists");
    removeFileNative(filePath);
}

void tst_QFileInfo::equalOperator() const
{
    /* Compare two default constructed values. Yes, to me it seems it should be the opposite too, but
     * this is how the code was written. */
    QVERIFY(!(QFileInfo() == QFileInfo()));
}


void tst_QFileInfo::equalOperatorWithDifferentSlashes() const
{
    const QFileInfo fi1("/usr");
    const QFileInfo fi2("/usr/");

    QCOMPARE(fi1, fi2);
}

void tst_QFileInfo::notEqualOperator() const
{
    /* Compare two default constructed values. Yes, to me it seems it should be the opposite too, but
     * this is how the code was written. */
    QVERIFY(QFileInfo() != QFileInfo());
}

void tst_QFileInfo::detachingOperations()
{
    QFileInfo info1;
    QVERIFY(info1.caching());
    info1.setCaching(false);

    {
        QFileInfo info2 = info1;

        QVERIFY(!info1.caching());
        QVERIFY(!info2.caching());

        info2.setCaching(true);
        QVERIFY(info2.caching());

        info1.setFile("foo");
        QVERIFY(!info1.caching());
    }

    {
        QFile file("foo");
        info1.setFile(file);
        QVERIFY(!info1.caching());
    }

    info1.setFile(QDir(), "foo");
    QVERIFY(!info1.caching());

    {
        QFileInfo info3;
        QVERIFY(info3.caching());

        info3 = info1;
        QVERIFY(!info3.caching());
    }

    info1.refresh();
    QVERIFY(!info1.caching());

    QVERIFY(info1.makeAbsolute());
    QVERIFY(!info1.caching());
}

void tst_QFileInfo::owner()
{
    QString userName;
    {
        struct passwd *pw = ::getpwuid(::geteuid());
        QVERIFY(pw);
        userName = QString::fromLocal8Bit(pw->pw_name);
    }
    if (userName.isEmpty())
        QSKIP("Can't retrieve the user name", SkipAll);
    QString fileName("ownertest.txt");
    QVERIFY(!QFile::exists(fileName) || QFile::remove(fileName));
    {
        QFile testFile(fileName);
        QVERIFY(testFile.open(QIODevice::WriteOnly | QIODevice::Text));
        QByteArray testData("testfile");
        QVERIFY(testFile.write(testData) != -1);
    }
    QFileInfo fi(fileName);
    QVERIFY(fi.exists());
    QCOMPARE(fi.owner(), userName);

    QFile::remove(fileName);
}

void tst_QFileInfo::group()
{
    struct group *gr = ::getgrgid(::getegid());
    QString expected = QString::fromLocal8Bit(gr->gr_name);

    QString fileName("ownertest.txt");
    if (QFile::exists(fileName))
        QFile::remove(fileName);
    QFile testFile(fileName);
    QVERIFY(testFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QByteArray testData("testfile");
    QVERIFY(testFile.write(testData) != -1);
    testFile.close();
    QFileInfo fi(fileName);
    QVERIFY(fi.exists());

    QCOMPARE(fi.group(), expected);
}

void tst_QFileInfo::invalidState()
{
    // Shouldn't crash;

    {
        QFileInfo info;
        QCOMPARE(info.size(), qint64(0));
        QVERIFY(!info.exists());

        info.setCaching(false);

        info.created();
        info.lastRead();
        info.lastModified();
    }

    {
        QFileInfo info("");
        QCOMPARE(info.size(), qint64(0));
        QVERIFY(!info.exists());

        info.setCaching(false);

        info.created();
        info.lastRead();
        info.lastModified();
    }

    {
        QFileInfo info("file-doesn't-really-exist.txt");
        QCOMPARE(info.size(), qint64(0));
        QVERIFY(!info.exists());

        info.setCaching(false);

        info.created();
        info.lastRead();
        info.lastModified();
    }

    QVERIFY(true);
}

QTEST_MAIN(tst_QFileInfo)

#include "moc_tst_qfileinfo.cpp"
#include "qrc_qfileinfo.cpp"