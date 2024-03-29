/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVETEXTEDIT_H
#define QDECLARATIVETEXTEDIT_H

#include "qdeclarativetext_p.h"
#include "qdeclarativeimplicitsizeitem_p.h"

#include <QtGui/qtextdocument.h>
#include <QtGui/qtextoption.h>
#include <QtGui/qtextcursor.h>
#include <QtGui/qtextformat.h>


QT_BEGIN_NAMESPACE



class QDeclarativeTextEditPrivate;
class Q_AUTOTEST_EXPORT QDeclarativeTextEdit : public QDeclarativeImplicitSizePaintedItem
{
    Q_OBJECT
    Q_ENUMS(VAlignment)
    Q_ENUMS(HAlignment)
    Q_ENUMS(WrapMode)
    Q_ENUMS(SelectionMode)

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor NOTIFY selectionColorChanged)
    Q_PROPERTY(QColor selectedTextColor READ selectedTextColor WRITE setSelectedTextColor NOTIFY selectedTextColorChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(HAlignment horizontalAlignment READ hAlign WRITE setHAlign RESET resetHAlign NOTIFY horizontalAlignmentChanged)
    Q_PROPERTY(VAlignment verticalAlignment READ vAlign WRITE setVAlign NOTIFY verticalAlignmentChanged)
    Q_PROPERTY(WrapMode wrapMode READ wrapMode WRITE setWrapMode NOTIFY wrapModeChanged)
    Q_PROPERTY(int lineCount READ lineCount NOTIFY lineCountChanged REVISION 1)
    Q_PROPERTY(qreal paintedWidth READ paintedWidth NOTIFY paintedSizeChanged)
    Q_PROPERTY(qreal paintedHeight READ paintedHeight NOTIFY paintedSizeChanged)
    Q_PROPERTY(Qt::TextFormat textFormat READ textFormat WRITE setTextFormat NOTIFY textFormatChanged)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(bool cursorVisible READ isCursorVisible WRITE setCursorVisible NOTIFY cursorVisibleChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(QRect cursorRectangle READ cursorRectangle NOTIFY cursorRectangleChanged)
    Q_PROPERTY(QDeclarativeComponent* cursorDelegate READ cursorDelegate WRITE setCursorDelegate NOTIFY cursorDelegateChanged)
    Q_PROPERTY(int selectionStart READ selectionStart NOTIFY selectionStartChanged)
    Q_PROPERTY(int selectionEnd READ selectionEnd NOTIFY selectionEndChanged)
    Q_PROPERTY(QString selectedText READ selectedText NOTIFY selectionChanged)
    Q_PROPERTY(bool activeFocusOnPress READ focusOnPress WRITE setFocusOnPress NOTIFY activeFocusOnPressChanged)
    Q_PROPERTY(bool persistentSelection READ persistentSelection WRITE setPersistentSelection NOTIFY persistentSelectionChanged)
    Q_PROPERTY(qreal textMargin READ textMargin WRITE setTextMargin NOTIFY textMarginChanged)
    Q_PROPERTY(bool selectByMouse READ selectByMouse WRITE setSelectByMouse NOTIFY selectByMouseChanged)
    Q_PROPERTY(SelectionMode mouseSelectionMode READ mouseSelectionMode WRITE setMouseSelectionMode NOTIFY mouseSelectionModeChanged REVISION 1)
    Q_PROPERTY(bool canPaste READ canPaste NOTIFY canPasteChanged REVISION 1)

public:
    QDeclarativeTextEdit(QDeclarativeItem *parent=0);

    enum HAlignment {
        AlignLeft = Qt::AlignLeft,
        AlignRight = Qt::AlignRight,
        AlignHCenter = Qt::AlignHCenter,
        AlignJustify = Qt::AlignJustify // ### VERSIONING: Only in QtQuick 1.1
    };

    enum VAlignment {
        AlignTop = Qt::AlignTop,
        AlignBottom = Qt::AlignBottom,
        AlignVCenter = Qt::AlignVCenter
    };

    enum WrapMode { NoWrap = QTextOption::NoWrap,
                    WordWrap = QTextOption::WordWrap,
                    WrapAnywhere = QTextOption::WrapAnywhere,
                    WrapAtWordBoundaryOrAnywhere = QTextOption::WrapAtWordBoundaryOrAnywhere, // COMPAT
                    Wrap = QTextOption::WrapAtWordBoundaryOrAnywhere
                  };

    enum SelectionMode {
        SelectCharacters,
        SelectWords
    };

    Q_INVOKABLE void openSoftwareInputPanel();
    Q_INVOKABLE void closeSoftwareInputPanel();

    QString text() const;
    void setText(const QString &);

    Qt::TextFormat textFormat() const;
    void setTextFormat(Qt::TextFormat format);

    QFont font() const;
    void setFont(const QFont &font);

    QColor color() const;
    void setColor(const QColor &c);

    QColor selectionColor() const;
    void setSelectionColor(const QColor &c);

    QColor selectedTextColor() const;
    void setSelectedTextColor(const QColor &c);

    HAlignment hAlign() const;
    void setHAlign(HAlignment align);
    void resetHAlign();
    HAlignment effectiveHAlign() const;

    VAlignment vAlign() const;
    void setVAlign(VAlignment align);

    WrapMode wrapMode() const;
    void setWrapMode(WrapMode w);

    int lineCount() const;

    bool isCursorVisible() const;
    void setCursorVisible(bool on);

    int cursorPosition() const;
    void setCursorPosition(int pos);

    QDeclarativeComponent* cursorDelegate() const;
    void setCursorDelegate(QDeclarativeComponent*);

    int selectionStart() const;
    int selectionEnd() const;

    QString selectedText() const;

    bool focusOnPress() const;
    void setFocusOnPress(bool on);

    bool persistentSelection() const;
    void setPersistentSelection(bool on);

    qreal textMargin() const;
    void setTextMargin(qreal margin);

    bool selectByMouse() const;
    void setSelectByMouse(bool);

    SelectionMode mouseSelectionMode() const;
    void setMouseSelectionMode(SelectionMode mode);

    bool canPaste() const;

    virtual void componentComplete();

    /* FROM EDIT */
    void setReadOnly(bool);
    bool isReadOnly() const;

    void setTextInteractionFlags(Qt::TextInteractionFlags flags);
    Qt::TextInteractionFlags textInteractionFlags() const;

    QRect cursorRectangle() const;

    qreal paintedWidth() const;
    qreal paintedHeight() const;

    Q_INVOKABLE QRectF positionToRectangle(int) const;
    Q_INVOKABLE int positionAt(int x, int y) const;
    Q_INVOKABLE void moveCursorSelection(int pos);
    Q_INVOKABLE Q_REVISION(1) void moveCursorSelection(int pos, SelectionMode mode);

    QRectF boundingRect() const;

Q_SIGNALS:
    void textChanged(const QString &);
    void paintedSizeChanged();
    void cursorPositionChanged();
    void cursorRectangleChanged();
    void selectionStartChanged();
    void selectionEndChanged();
    void selectionChanged();
    void colorChanged(const QColor &color);
    void selectionColorChanged(const QColor &color);
    void selectedTextColorChanged(const QColor &color);
    void fontChanged(const QFont &font);
    void horizontalAlignmentChanged(HAlignment alignment);
    void verticalAlignmentChanged(VAlignment alignment);
    void wrapModeChanged();
    void lineCountChanged();
    void textFormatChanged(Qt::TextFormat textFormat);
    void readOnlyChanged(bool isReadOnly);
    void cursorVisibleChanged(bool isCursorVisible);
    void cursorDelegateChanged();
    void activeFocusOnPressChanged(bool activeFocusOnPressed);
    void persistentSelectionChanged(bool isPersistentSelection);
    void textMarginChanged(qreal textMargin);
    void selectByMouseChanged(bool selectByMouse);
    Q_REVISION(1) void mouseSelectionModeChanged(SelectionMode mode);
    Q_REVISION(1) void linkActivated(const QString &link);
    Q_REVISION(1) void canPasteChanged();
    Q_REVISION(1) void inputMethodComposingChanged();

public Q_SLOTS:
    void selectAll();
    void selectWord();
    void select(int start, int end);
    Q_REVISION(1) void deselect();
    Q_REVISION(1) bool isRightToLeft(int start, int end);
#ifndef QT_NO_CLIPBOARD
    void cut();
    void copy();
    void paste();
#endif

private Q_SLOTS:
    void updateImgCache(const QRectF &rect);
    void q_textChanged();
    void updateSelectionMarkers();
    void moveCursorDelegate();
    void loadCursorDelegate();
    void q_canPasteChanged();

private:
    void updateSize();
    void updateTotalLines();

protected:
    virtual void geometryChanged(const QRectF &newGeometry, 
                                 const QRectF &oldGeometry);

    bool event(QEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void focusInEvent(QFocusEvent *event);

    // mouse filter?
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void drawContents(QPainter *, const QRect &);
private:
    Q_DISABLE_COPY(QDeclarativeTextEdit)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeTextEdit)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeTextEdit)


#endif
