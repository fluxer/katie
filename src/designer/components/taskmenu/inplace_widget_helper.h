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

#ifndef INPLACE_WIDGETHELPER_H
#define INPLACE_WIDGETHELPER_H


#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtCore/QPointer>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;

namespace qdesigner_internal {

    // A helper class to make an editor widget suitable for form inline
    // editing. Derive from the editor widget class and  make InPlaceWidgetHelper  a member.
    //
    // Sets "destructive close" on the editor widget and
    // wires "ESC" to it.
    // Installs an event filter on the parent to listen for
    // resize events and passes them on to the child.
    // You might want to connect editingFinished() to close() of the editor widget.
    class InPlaceWidgetHelper: public QObject
    {
        Q_OBJECT
    public:
        InPlaceWidgetHelper(QWidget *editorWidget, QWidget *parentWidget, QDesignerFormWindowInterface *fw);
        virtual ~InPlaceWidgetHelper();

        virtual bool eventFilter(QObject *object, QEvent *event);

        // returns a recommended alignment for the editor widget determined from the parent.
        Qt::Alignment alignment() const;
    private:
        QWidget *m_editorWidget;
        QPointer<QWidget> m_parentWidget;
        const bool m_noChildEvent;
        QPoint m_posOffset;
        QSize m_sizeOffset;
    };

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // INPLACE_WIDGETHELPER_H
