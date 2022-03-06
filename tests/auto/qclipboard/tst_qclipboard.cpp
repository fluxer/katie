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
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>

#ifndef QT_NO_CLIPBOARD

// TESTED_CLASS=
// TESTED_FILES=

Q_DECLARE_METATYPE(QClipboard::Mode)

class tst_QClipboard : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void capabiliyFunctions();
    void modes();
    void testSignals();
    void setMimeData();
    void clearBeforeSetText();

private:
    static bool nativeClipboardWorking();
};

void tst_QClipboard::initTestCase()
{
}

bool tst_QClipboard::nativeClipboardWorking()
{
    return true;
}

/*
    Tests that the capability functions are implemented on all
    platforms.
*/
void tst_QClipboard::capabiliyFunctions()
{
    QClipboard * const clipboard =  QApplication::clipboard();

    clipboard->supportsSelection();
    clipboard->ownsSelection();
    clipboard->ownsClipboard();
}

/*
    Test that text inserted into the clipboard in different modes is
    kept separate.
*/
void tst_QClipboard::modes()
{
    QClipboard * const clipboard =  QApplication::clipboard();

    if (!nativeClipboardWorking())
        QSKIP("Native clipboard not working in this setup", SkipAll);

    const QString defaultMode = "default mode text;";
    clipboard->setText(defaultMode);
    QCOMPARE(clipboard->text(), defaultMode);

    if (clipboard->supportsSelection()) {
        const QString selectionMode = "selection mode text";
        clipboard->setText(selectionMode, QClipboard::Selection);
        QCOMPARE(clipboard->text(QClipboard::Selection), selectionMode);
        QCOMPARE(clipboard->text(), defaultMode);
    }
}

/*
    Test that the appropriate signals are emitted when the cliboard
    contents is changed by calling the qt functions.
*/
void tst_QClipboard::testSignals()
{
    qRegisterMetaType<QClipboard::Mode>("QClipboard::Mode");

    if (!nativeClipboardWorking())
        QSKIP("Native clipboard not working in this setup", SkipAll);

    QClipboard * const clipboard =  QApplication::clipboard();

    QSignalSpy changedSpy(clipboard, SIGNAL(changed(QClipboard::Mode)));
    QSignalSpy dataChangedSpy(clipboard, SIGNAL(dataChanged()));
    QSignalSpy selectionChangedSpy(clipboard, SIGNAL(selectionChanged()));

    const QString text = "clipboard text;";

    // Test the default mode signal.
    clipboard->setText(text);
    QCOMPARE(dataChangedSpy.count(), 1);
    QCOMPARE(selectionChangedSpy.count(), 0);
    QCOMPARE(changedSpy.count(), 1);
    QCOMPARE(changedSpy.at(0).count(), 1);
    QCOMPARE(qvariant_cast<QClipboard::Mode>(changedSpy.at(0).at(0)), QClipboard::Clipboard);

    changedSpy.clear();

    // Test the selection mode signal.
    if (clipboard->supportsSelection()) {
        clipboard->setText(text, QClipboard::Selection);
        QCOMPARE(selectionChangedSpy.count(), 1);
        QCOMPARE(changedSpy.count(), 1);
        QCOMPARE(changedSpy.at(0).count(), 1);
        QCOMPARE(qvariant_cast<QClipboard::Mode>(changedSpy.at(0).at(0)), QClipboard::Selection);
    } else {
        QCOMPARE(selectionChangedSpy.count(), 0);
    }
    QCOMPARE(dataChangedSpy.count(), 1);

    changedSpy.clear();

    // Test the search mode signal.
    QCOMPARE(dataChangedSpy.count(), 1);
}

void tst_QClipboard::setMimeData()
{
    if (!nativeClipboardWorking())
        QSKIP("Native clipboard not working in this setup", SkipAll);
    QMimeData *mimeData = new QMimeData;
    const QString TestName(QLatin1String("tst_QClipboard::setMimeData() mimeData"));
    mimeData->setObjectName(TestName);

    QApplication::clipboard()->setMimeData(mimeData);
    QCOMPARE(QApplication::clipboard()->mimeData(), (const QMimeData *)mimeData);
    QCOMPARE(QApplication::clipboard()->mimeData()->objectName(), TestName);

    // set it to the same data again, it shouldn't delete mimeData (and crash as a result)
    QApplication::clipboard()->setMimeData(mimeData);
    QCOMPARE(QApplication::clipboard()->mimeData(), (const QMimeData *)mimeData);
    QCOMPARE(QApplication::clipboard()->mimeData()->objectName(), TestName);
    QApplication::clipboard()->clear();
    const QMimeData *appMimeData = QApplication::clipboard()->mimeData();
    QVERIFY(appMimeData != mimeData || appMimeData->objectName() != TestName);

    // check for crash when using the same mimedata object on several clipboards
    QMimeData *data = new QMimeData;
    data->setText("foo");

    QApplication::clipboard()->setMimeData(data, QClipboard::Clipboard);
    QApplication::clipboard()->setMimeData(data, QClipboard::Selection);

    QSignalSpy spySelection(QApplication::clipboard(), SIGNAL(selectionChanged()));
    QSignalSpy spyData(QApplication::clipboard(), SIGNAL(dataChanged()));

    QApplication::clipboard()->clear(QClipboard::Clipboard);
    QApplication::clipboard()->clear(QClipboard::Selection); // used to crash on X11

#if defined(Q_WS_X11)
    QCOMPARE(spySelection.count(), 1);
    QCOMPARE(spyData.count(), 1);
#endif

    // an other crash test
    data = new QMimeData;
    data->setText("foo");

    QApplication::clipboard()->setMimeData(data, QClipboard::Clipboard);
    QApplication::clipboard()->setMimeData(data, QClipboard::Selection);

    QMimeData *newData = new QMimeData;
    newData->setText("bar");

    spySelection.clear();
    spyData.clear();

    QApplication::clipboard()->setMimeData(newData, QClipboard::Clipboard);
    QApplication::clipboard()->setMimeData(newData, QClipboard::Selection); // used to crash on X11

#if defined(Q_WS_X11)
    QCOMPARE(spySelection.count(), 1);
    QCOMPARE(spyData.count(), 1);
#endif
}

void tst_QClipboard::clearBeforeSetText()
{
    QApplication::processEvents();

    if (!nativeClipboardWorking())
        QSKIP("Native clipboard not working in this setup", SkipAll);

    const QString text = "tst_QClipboard::clearBeforeSetText()";

    // setText() should work after processEvents()
    QApplication::clipboard()->setText(text);
    QCOMPARE(QApplication::clipboard()->text(), text);
    QApplication::processEvents();
    QCOMPARE(QApplication::clipboard()->text(), text);

    // same with clear()
    QApplication::clipboard()->clear();
    QVERIFY(QApplication::clipboard()->text().isEmpty());
    QApplication::processEvents();
    QVERIFY(QApplication::clipboard()->text().isEmpty());

    // setText() again
    QApplication::clipboard()->setText(text);
    QCOMPARE(QApplication::clipboard()->text(), text);
    QApplication::processEvents();
    QCOMPARE(QApplication::clipboard()->text(), text);

    // clear() immediately followed by setText() should still return the text
    QApplication::clipboard()->clear();
    QVERIFY(QApplication::clipboard()->text().isEmpty());
    QApplication::clipboard()->setText(text);
    QCOMPARE(QApplication::clipboard()->text(), text);
    QApplication::processEvents();
    QCOMPARE(QApplication::clipboard()->text(), text);
}

QTEST_MAIN(tst_QClipboard)

#include "moc_tst_qclipboard.cpp"

#else // QT_NO_CLIPBOARD

QTEST_NOOP_MAIN

#endif // QT_NO_CLIPBOARD