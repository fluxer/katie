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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "colorbutton.h"
#include "previewframe.h"
#include "paletteeditoradvanced.h"

#include <QLabel>
#include <QApplication>
#include <QComboBox>
#include <QStyleFactory>
#include <QFontDatabase>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QAction>
#include <QStatusBar>
#include <QSettings>
#include <QMessageBox>
#include <QStyle>
#include <QCloseEvent>
#include <QDebug>
#include <QPixmap>
#include <QGuiPlatformPlugin>

#include <stdlib.h>

QT_BEGIN_NAMESPACE

// from qapplication.cpp and qapplication_x11.cpp - These are NOT for
// external use ignore them
// extern bool Q_CORE_EXPORT qt_resolve_symlinks;

static const char *appearance_text = QT_TRANSLATE_NOOP("MainWindow",
"<p><b><font size+=2>Appearance</font></b></p>"
"<hr>"
"<p>Use this tab to customize the appearance of your Katie applications.</p>"
"<p>You can select the default GUI Style from the drop down list and "
"customize the colors.</p>"
"<p>Any GUI Style plugins in your plugin path will automatically be added "
"to the list of built-in Katie styles. (See the Library Paths tab for "
"information on adding new plugin paths.)</p>"
"<p>When you choose 3-D Effects and Window Background colors, the Katie "
"Configuration program will automatically generate a palette for you. "
"To customize colors further, press the Tune Palette button to open "
"the advanced palette editor.</p>"
"<p>The Preview Window shows what the selected Style and colors look "
"like.</p>"
);

static const char *font_text = QT_TRANSLATE_NOOP("MainWindow",
"<p><b><font size+=2>Fonts</font></b></p>"
"<hr>"
"<p>Use this tab to select the default font for your Katie applications. "
"The selected font is shown (initially as 'Sample Text') in the line "
"edit below the Family, Style and Point Size drop down lists.</p>"
"<p>You can specify if Katie should try to embed fonts into its generated output "
"when printing. If you enable font embedding, the resulting postscript will be "
"more portable and will more accurately reflect the "
"visual output on the screen; however the resulting postscript file "
"size will be bigger.</p>"
);

static const char *interface_text = QT_TRANSLATE_NOOP("MainWindow",
"<p><b><font size+=2>Interface</font></b></p>"
"<hr>"
"<p>Use this tab to customize the feel of your Katie applications.</p>"
"<p>If the Resolve Symlinks checkbox is checked Katie will follow symlinks "
"when handling URLs. For example, in the file dialog, if this setting is turned "
"on and /usr/tmp is a symlink to /var/tmp, entering the /usr/tmp directory "
"will cause the file dialog to change to /var/tmp.  With this setting turned "
"off, symlinks are not resolved or followed.</p>"
"<p>The Global Strut setting is useful for people who require a "
"minimum size for all widgets (e.g. when using a touch panel or for users "
"who are visually impaired).  Leaving the Global Strut width and height "
"at 0 will disable the Global Strut feature.</p>"
);
// ### What does the 'Enhanced support for languages written R2L do?

QPalette::ColorGroup MainWindow::groupFromIndex(int item)
{
    switch (item) {
    case 0:
    default:
        return QPalette::Active;
    case 1:
        return QPalette::Inactive;
    case 2:
        return QPalette::Disabled;
    }
}

static void setStyleHelper(QWidget *w, QStyle *s)
{
    const QObjectList children = w->children();
    for (int i = 0; i < children.size(); ++i) {
        QObject *child = children.at(i);
        if (child->isWidgetType())
            setStyleHelper((QWidget *) child, s);
    }
    w->setStyle(s);
}

static QString qtconfigPixmap()
{
    QString result = QString::fromLatin1(KATIE_PIXMAPS_PATH);
    result.append("/qtconfig");
    result.append(QLatin1String(KATIE_TOOLS_SUFFIX));
    result.append(QLatin1String(".png"));
    return result;
}

MainWindow::MainWindow()
    : ui(new Ui::MainWindow),
      editPalette(palette()),
      previewPalette(palette()),
      previewstyle(0)
{
    ui->setupUi(this);

    // signals and slots connections
    connect(ui->fontStyleCombo, SIGNAL(activated(int)), SLOT(buildFont()));
    connect(ui->pointSizeCombo, SIGNAL(activated(int)), SLOT(buildFont()));
    connect(ui->fontFamilyCombo, SIGNAL(activated(QString)), SLOT(familySelected(QString)));
    connect(ui->fileExitAction, SIGNAL(triggered()), SLOT(fileExit()));
    connect(ui->fileSaveAction, SIGNAL(triggered()), SLOT(fileSave()));
    connect(ui->helpAboutAction, SIGNAL(triggered()), SLOT(helpAbout()));
    connect(ui->helpAboutQtAction, SIGNAL(triggered()), SLOT(helpAboutQt()));
    connect(ui->mainTabWidget, SIGNAL(currentChanged(int)), SLOT(pageChanged(int)));
    connect(ui->paletteCombo, SIGNAL(activated(int)), SLOT(paletteSelected(int)));
    connect(ui->doubleClickIntervalSpinBox, SIGNAL(valueChanged(int)), SLOT(somethingModified()));
    connect(ui->cursorFlashTimeSpinBox, SIGNAL(valueChanged(int)), SLOT(somethingModified()));
    connect(ui->wheelScrollLinesSpinBox, SIGNAL(valueChanged(int)), SLOT(somethingModified()));
    connect(ui->menuEffectCombo, SIGNAL(activated(int)), SLOT(somethingModified()));
    connect(ui->toolTipEffectCombo, SIGNAL(activated(int)), SLOT(somethingModified()));
    connect(ui->strutWidthSpinBox, SIGNAL(valueChanged(int)), SLOT(somethingModified()));
    connect(ui->strutHeightSpinBox, SIGNAL(valueChanged(int)), SLOT(somethingModified()));
    connect(ui->effectsCheckBox, SIGNAL(toggled(bool)), SLOT(effectsToggled(bool)));
    connect(ui->resolveLinksCheckBox, SIGNAL(toggled(bool)), SLOT(somethingModified()));
    connect(ui->fontEmbeddingCheckBox, SIGNAL(clicked()), SLOT(somethingModified()));
    connect(ui->rtlExtensionsCheckBox, SIGNAL(toggled(bool)), SLOT(somethingModified()));
    connect(ui->guiStyleCombo, SIGNAL(activated(QString)), SLOT(styleSelected(QString)));
    connect(ui->tunePaletteButton, SIGNAL(clicked()), SLOT(tunePalette()));

    modified = true;
    desktopThemeName = tr("Desktop Settings (Default)");
    setWindowIcon(QPixmap(qtconfigPixmap()));
    QStringList gstyles = QStyleFactory::keys();
    gstyles.sort();
    ui->guiStyleCombo->addItem(desktopThemeName);
    ui->guiStyleCombo->setItemData(ui->guiStyleCombo->findText(desktopThemeName),
                                   tr("Choose style and palette based on your desktop settings."),
                                   Qt::ToolTipRole);
    ui->guiStyleCombo->addItems(gstyles);

    QSettings settings(QLatin1String("Katie"), QSettings::NativeFormat);

    QString currentstyle = settings.value(QLatin1String("Qt/style")).toString();
    if (currentstyle.isEmpty()) {
        ui->guiStyleCombo->setCurrentIndex(ui->guiStyleCombo->findText(desktopThemeName));
        currentstyle = QApplication::style()->objectName();
    } else {
        int index = ui->guiStyleCombo->findText(currentstyle, Qt::MatchFixedString);
        if (index != -1) {
            ui->guiStyleCombo->setCurrentIndex(index);
        } else { // we give up
            ui->guiStyleCombo->addItem(tr("Unknown"));
            ui->guiStyleCombo->setCurrentIndex(ui->guiStyleCombo->count() - 1);
        }
    }
    ui->buttonMainColor->setColor(palette().color(QPalette::Active, QPalette::Button));
    ui->buttonWindowColor->setColor(palette().color(QPalette::Active, QPalette::Window));
    connect(ui->buttonMainColor, SIGNAL(colorChanged(QColor)), SLOT(buildPalette()));
    connect(ui->buttonWindowColor, SIGNAL(colorChanged(QColor)), SLOT(buildPalette()));

#ifndef Q_WS_X11
    ui->colorConfig->hide();
#endif

    QFontDatabase db;
    QStringList families = db.families();
    ui->fontFamilyCombo->addItems(families);

    QList<int> sizes = db.standardSizes();
    foreach(int i, sizes)
        ui->pointSizeCombo->addItem(QString::number(i));

    ui->doubleClickIntervalSpinBox->setValue(QApplication::doubleClickInterval());
    ui->cursorFlashTimeSpinBox->setValue(QApplication::cursorFlashTime());
#ifndef QT_NO_WHEELEVENT
    ui->wheelScrollLinesSpinBox->setValue(QApplication::wheelScrollLines());
#else // QT_NO_WHEELEVENT
    ui->wheelScrollLinesLabel->hide();
    ui->wheelScrollLinesSpinBox->hide();
#endif // QT_NO_WHEELEVENT
    // #############
    // resolveLinksCheckBox->setChecked(qt_resolve_symlinks);

    ui->effectsCheckBox->setChecked(QApplication::isEffectEnabled(Qt::UI_General));
    ui->effectsFrame->setEnabled(ui->effectsCheckBox->isChecked());

    if (QApplication::isEffectEnabled(Qt::UI_FadeMenu))
        ui->menuEffectCombo->setCurrentIndex(1);

    if (QApplication::isEffectEnabled(Qt::UI_FadeTooltip))
        ui->toolTipEffectCombo->setCurrentIndex(1);

    QSize globalStrut = QApplication::globalStrut();
    ui->strutWidthSpinBox->setValue(globalStrut.width());
    ui->strutHeightSpinBox->setValue(globalStrut.height());

    // find the default family
    QStringList::Iterator sit = families.begin();
    int i = 0, possible = -1;
    while (sit != families.end()) {
        if (*sit == QApplication::font().family())
            break;
        if ((*sit).contains(QApplication::font().family()))
            possible = i;

        i++;
        sit++;
    }
    if (sit == families.end())
        i = possible;
    if (i == -1) // no clue about the current font
        i = 0;

    ui->fontFamilyCombo->setCurrentIndex(i);

    QStringList styles = db.styles(ui->fontFamilyCombo->currentText());
    ui->fontStyleCombo->addItems(styles);

    QString stylestring = db.styleString(QApplication::font());
    sit = styles.begin();
    i = 0;
    possible = -1;
    while (sit != styles.end()) {
        if (*sit == stylestring)
            break;
        if ((*sit).contains(stylestring))
            possible = i;

        i++;
        sit++;
    }
    if (sit == styles.end())
        i = possible;
    if (i == -1) // no clue about the current font
        i = 0;
    ui->fontStyleCombo->setCurrentIndex(i);

    i = 0;
    for (int psize = QApplication::font().pointSize(); i < ui->pointSizeCombo->count(); ++i) {
        const int sz = ui->pointSizeCombo->itemText(i).toInt();
        if (sz == psize) {
            ui->pointSizeCombo->setCurrentIndex(i);
            break;
        } else if(sz > psize) {
            ui->pointSizeCombo->insertItem(i, QString::number(psize));
            ui->pointSizeCombo->setCurrentIndex(i);
            break;
        }
    }

    ui->rtlExtensionsCheckBox->setChecked(settings.value(QLatin1String("Qt/useRtlExtensions"), false)
                                          .toBool());

    ui->fontEmbeddingCheckBox->setChecked(settings.value(QLatin1String("Qt/embedFonts"), true)
                                          .toBool());

    ui->helpView->setText(tr(appearance_text));

    setModified(false);
    updateStyleLayout();
}

MainWindow::~MainWindow()
{
    delete ui;
}

#ifdef Q_WS_X11
extern void qt_x11_apply_settings_in_all_apps();
#endif

void MainWindow::fileSave()
{
    if (! modified) {
#ifndef QT_NO_STATUSBAR
        statusBar()->showMessage(tr("No changes to be saved."), 2000);
#endif // QT_NO_STATUSBAR
        return;
    }

#ifndef QT_NO_STATUSBAR
    statusBar()->showMessage(tr("Saving changes..."));
#endif // QT_NO_STATUSBAR

    {
        QSettings settings(QLatin1String("Katie"), QSettings::NativeFormat);
        QFontDatabase db;
        QFont font = db.font(ui->fontFamilyCombo->currentText(),
                             ui->fontStyleCombo->currentText(),
                             ui->pointSizeCombo->currentText().toInt());

        QStringList actcg, inactcg, discg;
        bool overrideDesktopSettings = (ui->guiStyleCombo->currentText() != desktopThemeName);
        if (overrideDesktopSettings) {
            int i;
            for (i = 0; i < QPalette::NColorRoles; i++)
                actcg << editPalette.color(QPalette::Active,
                                           QPalette::ColorRole(i)).name();
            for (i = 0; i < QPalette::NColorRoles; i++)
                inactcg << editPalette.color(QPalette::Inactive,
                                             QPalette::ColorRole(i)).name();
            for (i = 0; i < QPalette::NColorRoles; i++)
                discg << editPalette.color(QPalette::Disabled,
                                           QPalette::ColorRole(i)).name();
        }

        settings.setValue(QLatin1String("Qt/font"), font.toString());
        settings.setValue(QLatin1String("Qt/Palette/active"), actcg);
        settings.setValue(QLatin1String("Qt/Palette/inactive"), inactcg);
        settings.setValue(QLatin1String("Qt/Palette/disabled"), discg);

        settings.setValue(QLatin1String("Qt/embedFonts"), ui->fontEmbeddingCheckBox->isChecked());
        settings.setValue(QLatin1String("Qt/style"),
                          overrideDesktopSettings ? ui->guiStyleCombo->currentText() : QString());

        settings.setValue(QLatin1String("Qt/doubleClickInterval"), ui->doubleClickIntervalSpinBox->value());
        settings.setValue(QLatin1String("Qt/cursorFlashTime"),
                          ui->cursorFlashTimeSpinBox->value() == 9 ? 0 : ui->cursorFlashTimeSpinBox->value());
        settings.setValue(QLatin1String("Qt/wheelScrollLines"), ui->wheelScrollLinesSpinBox->value());
        settings.setValue(QLatin1String("Qt/resolveSymlinks"), ui->resolveLinksCheckBox->isChecked());

        QSize strut(ui->strutWidthSpinBox->value(), ui->strutHeightSpinBox->value());
        settings.setValue(QLatin1String("Qt/globalStrut/width"), strut.width());
        settings.setValue(QLatin1String("Qt/globalStrut/height"), strut.height());

        settings.setValue(QLatin1String("Qt/useRtlExtensions"), ui->rtlExtensionsCheckBox->isChecked());

        QStringList effects;
        if (ui->effectsCheckBox->isChecked()) {
            effects << QLatin1String("general");

            switch (ui->menuEffectCombo->currentIndex()) {
                case 1: effects << QLatin1String("fademenu"); break;
            }

            switch (ui->toolTipEffectCombo->currentIndex()) {
                case 1: effects << QLatin1String("fadetooltip"); break;
            }
        } else
            effects << QLatin1String("none");
        settings.setValue(QLatin1String("Qt/GUIEffects"), effects);
    }

#if defined(Q_WS_X11)
    qt_x11_apply_settings_in_all_apps();
#endif // Q_WS_X11

    setModified(false);
#ifndef QT_NO_STATUSBAR
    statusBar()->showMessage(tr("Saved changes."));
#endif // QT_NO_STATUSBAR
}

void MainWindow::fileExit()
{
    qApp->closeAllWindows();
}

void MainWindow::setModified(bool m)
{
    if (modified == m)
        return;

    modified = m;
    ui->fileSaveAction->setEnabled(m);
}

void MainWindow::buildPalette()
{
    QPalette temp(ui->buttonMainColor->color(), ui->buttonWindowColor->color());
    for (int i = 0; i < QPalette::NColorGroups; i++)
        temp = PaletteEditorAdvanced::buildEffect(QPalette::ColorGroup(i), temp);

    editPalette = temp;
    setPreviewPalette(editPalette);
    updateColorButtons();

    setModified(true);
}

void MainWindow::setPreviewPalette(const QPalette &pal)
{
    QPalette::ColorGroup colorGroup = groupFromIndex(ui->paletteCombo->currentIndex());

    for (int i = 0; i < QPalette::NColorGroups; i++) {
        for (int j = 0; j < QPalette::NColorRoles; j++) {
            QPalette::ColorGroup targetGroup = QPalette::ColorGroup(i);
            QPalette::ColorRole targetRole = QPalette::ColorRole(j);
            previewPalette.setColor(targetGroup, targetRole, pal.color(colorGroup, targetRole));
        }
    }

    ui->previewFrame->setPreviewPalette(previewPalette);
}

void MainWindow::updateColorButtons()
{
    ui->buttonMainColor->setColor(editPalette.color(QPalette::Active, QPalette::Button));
    ui->buttonWindowColor->setColor(editPalette.color(QPalette::Active, QPalette::Window));
}

void MainWindow::tunePalette()
{
    bool ok;
    QPalette pal = PaletteEditorAdvanced::getPalette(&ok, editPalette,
                                                     backgroundRole(), this);
    if (!ok)
        return;

    editPalette = pal;
    setPreviewPalette(editPalette);
    setModified(true);
}

void MainWindow::paletteSelected(int)
{
    setPreviewPalette(editPalette);
}

void MainWindow::updateStyleLayout()
{
    ui->previewFrame->setPreviewVisible(true);
    ui->buildPaletteGroup->setEnabled(true);
}

void MainWindow::styleSelected(const QString &stylename)
{
    QString realstyle(stylename);
    if (stylename == desktopThemeName) {
        QGuiPlatformPlugin* platformplugin = qt_guiPlatformPlugin();
        if (platformplugin) {
            realstyle = platformplugin->styleName();
        }
    }
    QStyle *style = QStyleFactory::create(realstyle);
    if (!style)
        return;
    setStyleHelper(ui->previewFrame, style);
    delete previewstyle;
    previewstyle = style;
    setModified(true);
    updateStyleLayout();
}

void MainWindow::familySelected(const QString &family)
{
    QFontDatabase db;
    QStringList styles = db.styles(family);
    ui->fontStyleCombo->clear();
    ui->fontStyleCombo->addItems(styles);
    buildFont();
}

void MainWindow::buildFont()
{
    QFontDatabase db;
    QFont font = db.font(ui->fontFamilyCombo->currentText(),
                         ui->fontStyleCombo->currentText(),
                         ui->pointSizeCombo->currentText().toInt());
    ui->sampleLineEdit->setFont(font);
    setModified(true);
}

void MainWindow::somethingModified()
{
    setModified(true);
}

void MainWindow::helpAbout()
{
    QMessageBox box(this);
    box.setText(tr("<h3>%1</h3>"
                   "<br/>Version %2"
                   "<br/><br/>Copyright (C) 2015 The Qt Company Ltd."
                   "<br/><br/>Copyright (C) 2016 Ivailo Monev"
                    )
                   .arg(tr("Katie Configuration")).arg(QLatin1String(QT_VERSION_STR)));
    box.setWindowTitle(tr("Katie Configuration"));
    box.setIcon(QMessageBox::NoIcon);
    box.exec();
}

void MainWindow::helpAboutQt()
{
    QMessageBox::aboutQt(this, tr("Katie Configuration"));
}

void MainWindow::pageChanged(int pageNumber)
{
    QWidget *page = ui->mainTabWidget->widget(pageNumber);
    if (page == ui->interfaceTab)
        ui->helpView->setText(tr(interface_text));
    else if (page == ui->appearanceTab)
        ui->helpView->setText(tr(appearance_text));
    else if (page == ui->fontsTab)
        ui->helpView->setText(tr(font_text));
}

void MainWindow::effectsToggled(bool toggled)
{
    ui->effectsFrame->setEnabled(toggled);
    setModified(true);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (modified) {
        switch (QMessageBox::warning(this, tr("Save Changes"),
                                     tr("Save changes to settings?"),
                                     (QMessageBox::Yes | QMessageBox::No
                                     | QMessageBox::Cancel))) {
        case QMessageBox::Yes: // save
            qApp->processEvents();
            fileSave();

            // fall through intended
        case QMessageBox::No: // don't save
            e->accept();
            break;

        case QMessageBox::Cancel: // cancel
            e->ignore();
            break;

        default: break;
        }
    } else
        e->accept();
}

#include "moc_mainwindow.h"

QT_END_NAMESPACE
