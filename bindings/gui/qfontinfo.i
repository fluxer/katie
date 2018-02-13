%{
#include "QtGui/qfontinfo.h"
QT_USE_NAMESPACE
%}

class QFontInfo
{
public:
    QFontInfo(const QFont &);
    QFontInfo(const QFontInfo &);
    ~QFontInfo();

    QFontInfo &operator=(const QFontInfo &);

    QString family() const;
    QString styleName() const;
    int pixelSize() const;
    int pointSize() const;
    qreal pointSizeF() const;
    bool italic() const;
    QFont::Style style() const;
    int weight() const;
    bool bold() const;
    bool underline() const;
    bool overline() const;
    bool strikeOut() const;
    bool fixedPitch() const;
    QFont::StyleHint styleHint() const;

    bool exactMatch() const;
};
