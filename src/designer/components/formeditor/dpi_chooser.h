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

#ifndef DPICHOOSER_H
#define DPICHOOSER_H

#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class QSpinBox;
class QComboBox;

namespace qdesigner_internal {

struct DPI_Entry;

/* Let the user choose a DPI settings */
class DPI_Chooser : public QWidget {
    Q_DISABLE_COPY(DPI_Chooser)
    Q_OBJECT

public:
    explicit DPI_Chooser(QWidget *parent = nullptr);
    ~DPI_Chooser();

    void getDPI(int *dpiX, int *dpiY) const;
    void setDPI(int dpiX, int dpiY);

private slots:
    void syncSpinBoxes();

private:
    void setUserDefinedValues(int dpiX, int dpiY);

    struct DPI_Entry *m_systemEntry;
    QComboBox *m_predefinedCombo;
    QSpinBox *m_dpiXSpinBox;
    QSpinBox *m_dpiYSpinBox;
};
}

QT_END_NAMESPACE

#endif // DPICHOOSER_H
