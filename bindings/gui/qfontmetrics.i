%{
#include "QtGui/qfontmetrics.h"
QT_USE_NAMESPACE
%}

class QFontMetrics
{
public:
    QFontMetrics(const QFont &);
    QFontMetrics(const QFont &, QPaintDevice *pd);
    QFontMetrics(const QFontMetrics &);
    ~QFontMetrics();

    QFontMetrics &operator=(const QFontMetrics &);
#ifdef Q_COMPILER_RVALUE_REFS
    QFontMetrics &operator=(QFontMetrics &&other);
#endif

    int ascent() const;
    int descent() const;
    int height() const;
    int leading() const;
    int lineSpacing() const;
    int minLeftBearing() const;
    int minRightBearing() const;
    int maxWidth() const;

    int xHeight() const;
    int averageCharWidth() const;

    bool inFont(QChar) const;
    bool inFontUcs4(uint ucs4) const;

    int leftBearing(QChar) const;
    int rightBearing(QChar) const;
    int width(const QString &, int len = -1) const;
    int width(const QString &, int len, int flags) const;

    int width(QChar) const;
    int charWidth(const QString &str, int pos) const;

    QRect boundingRect(QChar) const;

    QRect boundingRect(const QString &text) const;
    QRect boundingRect(const QRect &r, int flags, const QString &text, int tabstops=0, int *tabarray=0) const;
    QRect boundingRect(int x, int y, int w, int h, int flags, const QString &text,
                              int tabstops=0, int *tabarray=0) const;
    QSize size(int flags, const QString& str, int tabstops=0, int *tabarray=0) const;

    QRect tightBoundingRect(const QString &text) const;

    QString elidedText(const QString &text, Qt::TextElideMode mode, int width, int flags = 0) const;

    int underlinePos() const;
    int overlinePos() const;
    int strikeOutPos() const;
    int lineWidth() const;

    bool operator==(const QFontMetrics &other) const;
    bool operator !=(const QFontMetrics &other) const;
};


class QFontMetricsF
{
public:
    QFontMetricsF(const QFont &);
    QFontMetricsF(const QFont &, QPaintDevice *pd);
    QFontMetricsF(const QFontMetrics &);
    QFontMetricsF(const QFontMetricsF &);
    ~QFontMetricsF();

    QFontMetricsF &operator=(const QFontMetricsF &);
    QFontMetricsF &operator=(const QFontMetrics &);
#ifdef Q_COMPILER_RVALUE_REFS
    QFontMetricsF &operator=(QFontMetricsF &&other);
#endif
    qreal ascent() const;
    qreal descent() const;
    qreal height() const;
    qreal leading() const;
    qreal lineSpacing() const;
    qreal minLeftBearing() const;
    qreal minRightBearing() const;
    qreal maxWidth() const;

    qreal xHeight() const;
    qreal averageCharWidth() const;

    bool inFont(QChar) const;
    bool inFontUcs4(uint ucs4) const;

    qreal leftBearing(QChar) const;
    qreal rightBearing(QChar) const;
    qreal width(const QString &string) const;

    qreal width(QChar) const;

    QRectF boundingRect(const QString &string) const;
    QRectF boundingRect(QChar) const;
    QRectF boundingRect(const QRectF &r, int flags, const QString& string, int tabstops=0, int *tabarray=0) const;
    QSizeF size(int flags, const QString& str, int tabstops=0, int *tabarray=0) const;

    QRectF tightBoundingRect(const QString &text) const;

    QString elidedText(const QString &text, Qt::TextElideMode mode, qreal width, int flags = 0) const;

    qreal underlinePos() const;
    qreal overlinePos() const;
    qreal strikeOutPos() const;
    qreal lineWidth() const;

    bool operator==(const QFontMetricsF &other) const;
    bool operator !=(const QFontMetricsF &other) const;
};
