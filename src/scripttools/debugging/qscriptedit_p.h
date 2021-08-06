/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtSCriptTools module of the Katie Toolkit.
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

#ifndef QSCRIPTEDIT_P_H
#define QSCRIPTEDIT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGui/qplaintextedit.h>

#include <QtCore/qhash.h>
#include <QtCore/qset.h>

QT_BEGIN_NAMESPACE

class QScriptEditExtraArea;

class Q_AUTOTEST_EXPORT QScriptEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    QScriptEdit(QWidget *parent = nullptr);
    ~QScriptEdit();

    int baseLineNumber() const;
    void setBaseLineNumber(int base);

    int executionLineNumber() const;
    void setExecutionLineNumber(int lineNumber, bool error);
    void setExecutableLineNumbers(const QSet<int> &lineNumbers);
    bool isExecutableLine(int lineNumber) const;

    int currentLineNumber() const;
    void gotoLine(int lineNumber);

    void setBreakpoint(int lineNumber);
    void setBreakpointEnabled(int lineNumber, bool enable);
    void deleteBreakpoint(int lineNumber);

    int extraAreaWidth() const;

signals:
    void breakpointToggleRequest(int lineNumber, bool on);
    void breakpointEnableRequest(int lineNumber, bool enable);

protected:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);

    void extraAreaPaintEvent(QPaintEvent *e);
    void extraAreaMouseEvent(QMouseEvent *e);
    bool extraAreaEvent(QEvent *e);

private slots:
    void updateExtraAreaWidth();
    void updateExtraArea(const QRect &, int);
    void highlightCurrentLine();

private:
    QTextEdit::ExtraSelection currentLineSelection() const;
    QTextEdit::ExtraSelection currentExecutionLineSelection() const;
    void updateExtraSelections();

private:
    QScriptEditExtraArea *m_extraArea;
    int m_baseLineNumber;
    int m_executionLineNumber;
    QSet<int> m_executableLineNumbers;
    bool m_executionLineNumberHasError;
    int m_extraAreaToggleBlockNumber;

    struct BreakpointData {
        BreakpointData() : enabled(true) {}
        bool enabled;
    };
    QHash<int, BreakpointData> m_breakpoints;

private:
    Q_DISABLE_COPY(QScriptEdit)
    friend class QScriptEditExtraArea;
};

QT_END_NAMESPACE

#endif
