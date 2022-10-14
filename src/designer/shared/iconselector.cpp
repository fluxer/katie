/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#include "iconselector_p.h"
#include "qdesigner_utils_p.h"
#include "iconloader_p.h"
#include "qdesigner_integration_p.h"
#include "formwindowbase_p.h"

#include <abstractdialoggui_p.h>
#include <QtDesigner/abstractformeditor.h>
#include <QtDesigner/QExtensionManager>

#include <QtGui/QToolButton>
#include <QtCore/QSignalMapper>
#include <QtGui/QComboBox>
#include <QtGui/QAction>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QPushButton>
#include <QtGui/QDialog>
#include <QtGui/QMenu>
#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>
#include <QtGui/QImageReader>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QValidator>
#include <QtCore/QDebug>


QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

// ------------ IconSelectorPrivate

static inline QPixmap emptyPixmap()
{
    QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
    img.fill(0);
    return QPixmap::fromImage(img);
}

class IconSelectorPrivate
{
    IconSelector *q_ptr;
    Q_DECLARE_PUBLIC(IconSelector)
public:
    IconSelectorPrivate();

    void slotStateActivated();
    void slotSetActivated();
    void slotSetFileActivated();
    void slotResetActivated();
    void slotResetAllActivated();
    void slotUpdate();

    QList<QPair<QPair<QIcon::Mode, QIcon::State>, QString> > m_stateToName; // could be static map

    QMap<QPair<QIcon::Mode, QIcon::State>, int>  m_stateToIndex;
    QMap<int, QPair<QIcon::Mode, QIcon::State> > m_indexToState;

    const QIcon m_emptyIcon;
    QComboBox *m_stateComboBox;
    QToolButton *m_iconButton;
    QAction *m_resetAction;
    QAction *m_resetAllAction;
    PropertySheetIconValue m_icon;
    DesignerIconCache *m_iconCache;
    DesignerPixmapCache *m_pixmapCache;
    QDesignerFormEditorInterface *m_core;
};

IconSelectorPrivate::IconSelectorPrivate() :
    q_ptr(nullptr),
    m_emptyIcon(emptyPixmap()),
    m_stateComboBox(nullptr),
    m_iconButton(nullptr),
    m_resetAction(nullptr),
    m_resetAllAction(nullptr),
    m_iconCache(nullptr),
    m_pixmapCache(nullptr),
    m_core(nullptr)
{
}
void IconSelectorPrivate::slotUpdate()
{
    QIcon icon;
    if (m_iconCache)
        icon = m_iconCache->icon(m_icon);

    QMap<QPair<QIcon::Mode, QIcon::State>, PropertySheetPixmapValue> paths = m_icon.paths();
    QMapIterator<QPair<QIcon::Mode, QIcon::State>, int> itIndex(m_stateToIndex);
    while (itIndex.hasNext()) {
        const QPair<QIcon::Mode, QIcon::State> state = itIndex.next().key();
        const PropertySheetPixmapValue pixmap = paths.value(state);
        const int index = itIndex.value();

        QIcon pixmapIcon = QIcon(icon.pixmap(16, 16, state.first, state.second));
        if (pixmapIcon.isNull())
            pixmapIcon = m_emptyIcon;
        m_stateComboBox->setItemIcon(index, pixmapIcon);
        QFont font = q_ptr->font();
        if (!pixmap.path().isEmpty())
            font.setBold(true);
        m_stateComboBox->setItemData(index, font, Qt::FontRole);
    }

    QPair<QIcon::Mode, QIcon::State> state = m_indexToState.value(m_stateComboBox->currentIndex());
    PropertySheetPixmapValue currentPixmap = paths.value(state);
    m_resetAction->setEnabled(!currentPixmap.path().isEmpty());
    m_resetAllAction->setEnabled(!paths.isEmpty());
    m_stateComboBox->update();
}

void IconSelectorPrivate::slotStateActivated()
{
    slotUpdate();
}

void IconSelectorPrivate::slotSetActivated()
{
    QPair<QIcon::Mode, QIcon::State> state = m_indexToState.value(m_stateComboBox->currentIndex());
    const PropertySheetPixmapValue pixmap = m_icon.pixmap(state.first, state.second);
    if (!pixmap.path().isEmpty()) {
        slotSetFileActivated();
    }
}

// Helpers for choosing image files: Check for valid image.
bool IconSelector::checkPixmap(const QString &fileName, CheckMode cm, QString *errorMessage)
{
    const QFileInfo fi(fileName);
    if (!fi.exists() || !fi.isFile() || !fi.isReadable()) {
        if (errorMessage)
            *errorMessage = tr("The pixmap file '%1' cannot be read.").arg(fileName);
        return false;
    }
    QImageReader reader(fileName);
    if (!reader.canRead()) {
        if (errorMessage)
            *errorMessage = tr("The file '%1' does not appear to be a valid pixmap file: %2").arg(fileName).arg(reader.errorString());
        return false;
    }
    if (cm == CheckFast)
        return true;

    const QImage image = reader.read();
    if (image.isNull()) {
        if (errorMessage)
            *errorMessage = tr("The file '%1' could not be read: %2").arg(fileName).arg(reader.errorString());
        return false;
    }
    return true;
}

// Helpers for choosing image files: Return an image filter for QFileDialog, courtesy of StyledButton
static QString imageFilter()
{
    QString filter = QApplication::translate("IconSelector", "All Pixmaps (");
    const QList<QByteArray> supportedImageFormats = QImageReader::supportedImageFormats();
    const int count = supportedImageFormats.count();
    for (int i = 0; i< count; ++i) {
        if (i)
            filter += QLatin1Char(' ');
        filter += QLatin1String("*.");
        const QString outputFormat = QString::fromUtf8(supportedImageFormats.at(i));
        filter += outputFormat.toLower();
    }
    filter += QLatin1Char(')');
    return filter;
}

// Helpers for choosing image files: Choose a file
QString IconSelector::choosePixmapFile(const QString &directory, QDesignerDialogGuiInterface *dlgGui,QWidget *parent)
{
    QString errorMessage;
    QString newPath;
    do {
        const QString title = tr("Choose a Pixmap");
        static const  QString filter = imageFilter();
        newPath =  dlgGui->getOpenImageFileName(parent, title, directory, filter);
        if (newPath.isEmpty())
            break;
        if (checkPixmap(newPath, CheckFully, &errorMessage))
            break;
        dlgGui->message(parent, QDesignerDialogGuiInterface::ResourceEditorMessage, QMessageBox::Warning, tr("Pixmap Read Error"), errorMessage);
    } while(true);
    return  newPath;
}

void IconSelectorPrivate::slotSetFileActivated()
{
    QPair<QIcon::Mode, QIcon::State> state = m_indexToState.value(m_stateComboBox->currentIndex());

    PropertySheetPixmapValue pixmap = m_icon.pixmap(state.first, state.second);
    const QString newPath = IconSelector::choosePixmapFile(pixmap.path(), m_core->dialogGui(), q_ptr);
    if (!newPath.isEmpty()) {
        const PropertySheetPixmapValue newPixmap = PropertySheetPixmapValue(newPath);
        if (!(newPixmap == pixmap)) {
            m_icon.setPixmap(state.first, state.second, newPixmap);
            slotUpdate();
            emit q_ptr->iconChanged(m_icon);
        }
    }
}

void IconSelectorPrivate::slotResetActivated()
{
    QPair<QIcon::Mode, QIcon::State> state = m_indexToState.value(m_stateComboBox->currentIndex());

    PropertySheetPixmapValue pixmap = m_icon.pixmap(state.first, state.second);
    const PropertySheetPixmapValue newPixmap;
    if (!(newPixmap == pixmap)) {
        m_icon.setPixmap(state.first, state.second, newPixmap);
        slotUpdate();
        emit q_ptr->iconChanged(m_icon);
    }
}

void IconSelectorPrivate::slotResetAllActivated()
{
    const PropertySheetIconValue newIcon;
    if (!(m_icon == newIcon)) {
        m_icon = newIcon;
        slotUpdate();
        emit q_ptr->iconChanged(m_icon);
    }
}

// ------------- IconSelector
IconSelector::IconSelector(QWidget *parent) :
    QWidget(parent), d_ptr(new IconSelectorPrivate())
{
    d_ptr->q_ptr = this;

    d_ptr->m_stateComboBox = new QComboBox(this);

    QHBoxLayout *l = new QHBoxLayout(this);
    d_ptr->m_iconButton = new QToolButton(this);
    d_ptr->m_iconButton->setText(tr("..."));
    d_ptr->m_iconButton->setPopupMode(QToolButton::MenuButtonPopup);
    l->addWidget(d_ptr->m_stateComboBox);
    l->addWidget(d_ptr->m_iconButton);
    l->setMargin(0);

    d_ptr->m_stateToName << qMakePair(qMakePair(QIcon::Normal,   QIcon::Off), tr("Normal Off")   );
    d_ptr->m_stateToName << qMakePair(qMakePair(QIcon::Normal,   QIcon::On),  tr("Normal On")    );
    d_ptr->m_stateToName << qMakePair(qMakePair(QIcon::Disabled, QIcon::Off), tr("Disabled Off") );
    d_ptr->m_stateToName << qMakePair(qMakePair(QIcon::Disabled, QIcon::On),  tr("Disabled On")  );
    d_ptr->m_stateToName << qMakePair(qMakePair(QIcon::Active,   QIcon::Off), tr("Active Off")   );
    d_ptr->m_stateToName << qMakePair(qMakePair(QIcon::Active,   QIcon::On),  tr("Active On")    );
    d_ptr->m_stateToName << qMakePair(qMakePair(QIcon::Selected, QIcon::Off), tr("Selected Off") );
    d_ptr->m_stateToName << qMakePair(qMakePair(QIcon::Selected, QIcon::On),  tr("Selected On")  );

    QMenu *setMenu = new QMenu(this);

    QAction *setFileAction = new QAction(tr("Choose File..."), this);
    d_ptr->m_resetAction = new QAction(tr("Reset"), this);
    d_ptr->m_resetAllAction = new QAction(tr("Reset All"), this);
    d_ptr->m_resetAction->setEnabled(false);
    d_ptr->m_resetAllAction->setEnabled(false);
    //d_ptr->m_resetAction->setIcon(createIconSet(QString::fromUtf8("resetproperty.png")));

    setMenu->addAction(setFileAction);
    setMenu->addSeparator();
    setMenu->addAction(d_ptr->m_resetAction);
    setMenu->addAction(d_ptr->m_resetAllAction);

    int index = 0;
    QStringList items;
    QListIterator<QPair<QPair<QIcon::Mode, QIcon::State>, QString> > itName(d_ptr->m_stateToName);
    while (itName.hasNext()) {
        QPair<QPair<QIcon::Mode, QIcon::State>, QString> item = itName.next();
        const QPair<QIcon::Mode, QIcon::State> state = item.first;
        const QString name = item.second;

        items.append(name);
        d_ptr->m_stateToIndex[state] = index;
        d_ptr->m_indexToState[index] = state;
        index++;
    }
    d_ptr->m_stateComboBox->addItems(items);

    d_ptr->m_iconButton->setMenu(setMenu);

    connect(d_ptr->m_stateComboBox, SIGNAL(activated(int)), this, SLOT(slotStateActivated()));
    connect(d_ptr->m_iconButton, SIGNAL(clicked()), this, SLOT(slotSetActivated()));
    connect(setFileAction, SIGNAL(triggered()), this, SLOT(slotSetFileActivated()));
    connect(d_ptr->m_resetAction, SIGNAL(triggered()), this, SLOT(slotResetActivated()));
    connect(d_ptr->m_resetAllAction, SIGNAL(triggered()), this, SLOT(slotResetAllActivated()));

    d_ptr->slotUpdate();
}

IconSelector::~IconSelector()
{
}

void IconSelector::setIcon(const PropertySheetIconValue &icon)
{
    if (d_ptr->m_icon == icon)
        return;

    d_ptr->m_icon = icon;
    d_ptr->slotUpdate();
}

PropertySheetIconValue IconSelector::icon() const
{
    return d_ptr->m_icon;
}

void IconSelector::setFormEditor(QDesignerFormEditorInterface *core)
{
    d_ptr->m_core = core;
    d_ptr->slotUpdate();
}

void IconSelector::setIconCache(DesignerIconCache *iconCache)
{
    d_ptr->m_iconCache = iconCache;
    connect(iconCache, SIGNAL(reloaded()), this, SLOT(slotUpdate()));
    d_ptr->slotUpdate();
}

void IconSelector::setPixmapCache(DesignerPixmapCache *pixmapCache)
{
    d_ptr->m_pixmapCache = pixmapCache;
    connect(pixmapCache, SIGNAL(reloaded()), this, SLOT(slotUpdate()));
    d_ptr->slotUpdate();
}

// --- IconThemeEditor

// Validator for theme line edit, accepts empty or non-blank strings.
class BlankSuppressingValidator : public QValidator {
public:
    explicit BlankSuppressingValidator(QObject * parent = nullptr) : QValidator(parent) {}

    virtual State validate(QString &input, int &pos) const {
        const int blankPos = input.indexOf(QLatin1Char(' '));
        if (blankPos != -1) {
            pos = blankPos;
            return Invalid;
        }
        return Acceptable;
    }
};

struct IconThemeEditorPrivate {
    IconThemeEditorPrivate();

    const QPixmap m_emptyPixmap;
    QLineEdit *m_themeLineEdit;
    QLabel *m_themeLabel;
};

IconThemeEditorPrivate::IconThemeEditorPrivate() :
    m_emptyPixmap(emptyPixmap()),
    m_themeLineEdit(new QLineEdit),
    m_themeLabel(new QLabel)
{
}

IconThemeEditor::IconThemeEditor(QWidget *parent, bool wantResetButton) :
    QWidget (parent), d(new IconThemeEditorPrivate)
{
    QHBoxLayout *mainHLayout = new QHBoxLayout;
    mainHLayout->setMargin(0);

    // Vertically center theme preview label
    d->m_themeLabel->setPixmap(d->m_emptyPixmap);

    QVBoxLayout *themeLabelVLayout = new QVBoxLayout;
    d->m_themeLabel->setMargin(1);
    themeLabelVLayout->setMargin(0);
    themeLabelVLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    themeLabelVLayout->addWidget(d->m_themeLabel);
    themeLabelVLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    mainHLayout->addLayout(themeLabelVLayout);

    d->m_themeLineEdit = new QLineEdit;
    d->m_themeLineEdit->setValidator(new BlankSuppressingValidator(d->m_themeLineEdit));
    connect(d->m_themeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotChanged(QString)));
    connect(d->m_themeLineEdit, SIGNAL(textEdited(QString)), this, SIGNAL(edited(QString)));
    mainHLayout->addWidget(d->m_themeLineEdit);

    if (wantResetButton) {
        QToolButton *themeResetButton = new QToolButton;
        themeResetButton->setIcon(createIconSet(QLatin1String("resetproperty.png")));
        connect(themeResetButton, SIGNAL(clicked()), this, SLOT(reset()));
        mainHLayout->addWidget(themeResetButton);
    }

    setLayout(mainHLayout);
    setFocusProxy(d->m_themeLineEdit);
}

IconThemeEditor::~IconThemeEditor()
{
}

void IconThemeEditor::reset()
{
    d->m_themeLineEdit->clear();
    emit edited(QString());
}

void IconThemeEditor::slotChanged(const QString &theme)
{
    updatePreview(theme);
}

void IconThemeEditor::updatePreview(const QString &t)
{
    // Update preview label with icon.
    if (t.isEmpty() || !QIcon::hasThemeIcon(t)) { // Empty
        const QPixmap *currentPixmap = d->m_themeLabel->pixmap();
        if (!currentPixmap || currentPixmap->cacheKey() != d->m_emptyPixmap.cacheKey())
            d->m_themeLabel->setPixmap(d->m_emptyPixmap);
    } else {
        const QIcon icon = QIcon::fromTheme(t);
        d->m_themeLabel->setPixmap(icon.pixmap(d->m_emptyPixmap.size()));
    }
}

QString IconThemeEditor::theme() const
{
    return d->m_themeLineEdit->text();
}

void IconThemeEditor::setTheme(const QString &t)
{
    d->m_themeLineEdit->setText(t);
}

} // qdesigner_internal

QT_END_NAMESPACE

#include "moc_iconselector_p.h"
