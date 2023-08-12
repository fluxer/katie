/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QTEXTCODEC_H
#define QTEXTCODEC_H

#include <QtCore/qstring.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_TEXTCODEC

class QTextCodecPrivate;
class QTextConverterPrivate;

class Q_CORE_EXPORT QTextConverter
{
public:
    enum ConversionFlag {
        DefaultConversion = 0x0,
        ConvertInvalidToNull = 0x1
    };
    Q_DECLARE_FLAGS(ConversionFlags, ConversionFlag)

    QTextConverter();
    QTextConverter(const QTextConverter &other);
    QTextConverter(const QByteArray &name);
    QTextConverter(const int mib);
    ~QTextConverter();

    ConversionFlags flags() const;
    void setFlags(const ConversionFlags flags);
    bool hasFailure() const;
    void reset();

    QString toUnicode(const char *in, int length) const;
    inline QString toUnicode(const QByteArray &ba) const
        { return toUnicode(ba.constData(), ba.size()); }
    inline QString toUnicode(const char* chars) const
        { return toUnicode(chars, qstrlen(chars)); }

    QByteArray fromUnicode(const QChar *in, int length) const;
    inline QByteArray fromUnicode(const QString &uc) const
        { return fromUnicode(uc.unicode(), uc.size()); }

    QTextConverter& operator=(const QTextConverter &other);

private:
    QTextConverterPrivate* d_ptr;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QTextConverter::ConversionFlags)

class Q_CORE_EXPORT QTextCodec
{
public:
    static QTextCodec* codecForName(const QByteArray &name);
    static inline QTextCodec* codecForName(const char *name)
        { return codecForName(QByteArray(name)); }
    static QTextCodec* codecForMib(int mib);

    static QList<QByteArray> availableCodecs();
    static QList<int> availableMibs();

    static QTextCodec* codecForLocale();
    static void setCodecForLocale(QTextCodec *c);

    static QTextCodec *codecForHtml(const QByteArray &ba);
    static QTextCodec *codecForHtml(const QByteArray &ba, QTextCodec *defaultCodec);

    static QTextCodec *codecForUtfText(const QByteArray &ba);
    static QTextCodec *codecForUtfText(const QByteArray &ba, QTextCodec *defaultCodec);

    static QTextCodec *codecForText(const QByteArray &ba);
    static QTextCodec *codecForText(const QByteArray &ba, QTextCodec *defaultCodec);

    bool canEncode(QChar) const;
    bool canEncode(const QString&) const;

    QString toUnicode(const char *in, int length) const;
    QByteArray fromUnicode(const QChar *in, int length) const;

    inline QString toUnicode(const QByteArray &ba) const
        { return toUnicode(ba.constData(), ba.size()); }
    inline QString toUnicode(const char* chars) const
        { return toUnicode(chars, qstrlen(chars)); }
    inline QByteArray fromUnicode(const QString &uc) const
        { return fromUnicode(uc.unicode(), uc.size()); }

    QByteArray name() const;
    QList<QByteArray> aliases() const;
    int mibEnum() const;

    inline QTextConverter converter() const
        { return QTextConverter(name()); }

private:
    Q_DISABLE_COPY(QTextCodec);

    QTextCodec(const QByteArray &codec);
    QTextCodec(const int codec);
    ~QTextCodec();

    friend class QTextCodecCleanup;

    QTextCodecPrivate* const d_ptr;
};

#endif // QT_NO_TEXTCODEC

QT_END_NAMESPACE

#endif // QTEXTCODEC_H
