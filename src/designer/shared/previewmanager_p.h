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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Katie Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef PREVIEWMANAGER_H
#define PREVIEWMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/qshareddata.h>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;
class QWidget;
class QPixmap;
class QAction;
class QActionGroup;
class QMenu;
class QWidget;
class QDesignerSettingsInterface;

namespace qdesigner_internal {

// ----------- PreviewConfiguration

class PreviewConfigurationData;

class Q_DESIGNER_EXPORT PreviewConfiguration {
public:
    PreviewConfiguration();
    explicit PreviewConfiguration(const QString &style,
                                  const QString &applicationStyleSheet = QString());

    PreviewConfiguration(const PreviewConfiguration&);
    PreviewConfiguration& operator=(const PreviewConfiguration&);
    ~PreviewConfiguration();

    QString style() const;
    void setStyle(const QString &);

    // Style sheet to prepend (to simulate the effect od QApplication::setSyleSheet()).
    QString applicationStyleSheet() const;
    void setApplicationStyleSheet(const QString &);

    void clear();
    void toSettings(const QString &prefix, QDesignerSettingsInterface *settings) const;
    void fromSettings(const QString &prefix, const QDesignerSettingsInterface *settings);

private:
    QSharedDataPointer<PreviewConfigurationData> m_d;
};

Q_DESIGNER_EXPORT bool operator<(const PreviewConfiguration &pc1, const PreviewConfiguration &pc2);
Q_DESIGNER_EXPORT bool operator==(const PreviewConfiguration &pc1, const PreviewConfiguration &pc2);
Q_DESIGNER_EXPORT bool operator!=(const PreviewConfiguration &pc1, const PreviewConfiguration &pc2);

// ----------- Preview window manager.
// Maintains a list of preview widgets with their associated form windows and configuration.

class PreviewManagerPrivate;

class Q_DESIGNER_EXPORT PreviewManager : public QObject
{
    Q_OBJECT
public:

    enum PreviewMode {
        // Modal preview. Do not use on Macs as dialogs would have no close button
        ApplicationModalPreview,
        // Non modal previewing of one form in different configurations (closes if form window changes)
        SingleFormNonModalPreview,
        // Non modal previewing of several forms in different configurations
        MultipleFormNonModalPreview };

    explicit PreviewManager(PreviewMode mode, QObject *parent);
    virtual ~PreviewManager();

    // Show preview. Raise existing preview window if there is one with a matching
    // configuration, else create a new preview.
    QWidget *showPreview(const QDesignerFormWindowInterface *, const PreviewConfiguration &pc, int deviceProfileIndex /*=-1*/, QString *errorMessage);
    // Convenience that creates a preview using a configuration taken from the settings.
    QWidget *showPreview(const QDesignerFormWindowInterface *, const QString &style, int deviceProfileIndex /*=-1*/, QString *errorMessage);
    QWidget *showPreview(const QDesignerFormWindowInterface *, const QString &style, QString *errorMessage);

    int previewCount() const;

    // Create a pixmap for printing.
    QPixmap createPreviewPixmap(const QDesignerFormWindowInterface *fw, const PreviewConfiguration &pc, int deviceProfileIndex /*=-1*/, QString *errorMessage);
    // Convenience that creates a pixmap using a configuration taken from the settings.
    QPixmap createPreviewPixmap(const QDesignerFormWindowInterface *fw, const QString &style, int deviceProfileIndex /*=-1*/, QString *errorMessage);
    QPixmap createPreviewPixmap(const QDesignerFormWindowInterface *fw, const QString &style, QString *errorMessage);

    virtual bool eventFilter(QObject *watched, QEvent *event);

public slots:
    void closeAllPreviews();

signals:
    void firstPreviewOpened();
    void lastPreviewClosed();

private slots:
    void slotZoomChanged(int);

private:
    QWidget *raise(const QDesignerFormWindowInterface *, const PreviewConfiguration &pc);
    QWidget *createPreview(const QDesignerFormWindowInterface *,
                           const PreviewConfiguration &pc,
                           int deviceProfileIndex /* = -1 */,
                           QString *errorMessage,
                           /*Disabled by default, <0 */
                           int initialZoom = -1);

    void updatePreviewClosed(QWidget *w);

    PreviewManagerPrivate *d;

    PreviewManager(const PreviewManager &other);
    PreviewManager &operator =(const PreviewManager &other);
};
}

QT_END_NAMESPACE

#endif // PREVIEWMANAGER_H
