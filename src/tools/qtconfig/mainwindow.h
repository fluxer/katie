/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    void closeEvent(QCloseEvent *);

public slots:
    virtual void buildPalette();
    virtual void buildFont();
    virtual void tunePalette();
    virtual void paletteSelected(int);
    virtual void styleSelected(const QString &);
    virtual void familySelected(const QString &);
    virtual void substituteSelected(const QString &);
    virtual void removeSubstitute();
    virtual void addSubstitute();
    virtual void downSubstitute();
    virtual void upSubstitute();
    virtual void removeFontpath();
    virtual void addFontpath();
    virtual void downFontpath();
    virtual void upFontpath();
    virtual void browseFontpath();
    virtual void fileSave();
    virtual void fileExit();
    virtual void somethingModified();
    virtual void helpAbout();
    virtual void helpAboutQt();
    virtual void pageChanged(int);


private:
    void updateColorButtons();
    void updateFontSample();
    void updateStyleLayout();

    static QPalette::ColorGroup groupFromIndex(int);

    void setPreviewPalette(const QPalette &);

    void setModified(bool);

    Ui::MainWindow *ui;
    QString desktopThemeName;
    QPalette editPalette, previewPalette;
    QStyle *previewstyle;
    QStringList fontpaths;
    bool modified;
};

QT_END_NAMESPACE

#endif // MAINWINDOW_H
