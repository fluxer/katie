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

class QIODevice;
class QTextDecoder;
class QTextEncoder;

class Q_CORE_EXPORT QTextCodec
{
    Q_DISABLE_COPY(QTextCodec)
public:
    static QTextCodec* codecForName(const QByteArray &name);
    static inline QTextCodec* codecForName(const char *name)
        { return codecForName(QByteArray(name)); }
    static QTextCodec* codecForMib(int mib);

    static QList<QByteArray> availableCodecs();
    static QList<int> availableMibs();

    static QTextCodec* codecForLocale();
    static void setCodecForLocale(QTextCodec *c);

    static inline QTextCodec* codecForTr()
        { return cftr; }
    static inline void setCodecForTr(QTextCodec *c)
        { cftr = c; }

    static inline QTextCodec* codecForCStrings()
        { return QString::codecForCStrings; }
    static inline void setCodecForCStrings(QTextCodec *c)
        { QString::codecForCStrings = c; }

    static QTextCodec *codecForHtml(const QByteArray &ba);
    static QTextCodec *codecForHtml(const QByteArray &ba, QTextCodec *defaultCodec);

    static QTextCodec *codecForUtfText(const QByteArray &ba);
    static QTextCodec *codecForUtfText(const QByteArray &ba, QTextCodec *defaultCodec);

    static QTextCodec *codecForText(const QByteArray &ba);
    static QTextCodec *codecForText(const QByteArray &ba, QTextCodec *defaultCodec);

    bool canEncode(QChar) const;
    bool canEncode(const QString&) const;

    QString toUnicode(const QByteArray&) const;
    QString toUnicode(const char* chars) const;
    QByteArray fromUnicode(const QString& uc) const;
    enum ConversionFlag {
        DefaultConversion,
        ConvertInvalidToNull = 0x80000000,
        IgnoreHeader = 0x1
    };
    Q_DECLARE_FLAGS(ConversionFlags, ConversionFlag)

    struct Q_CORE_EXPORT ConverterState {
        ConverterState(ConversionFlags f = DefaultConversion)
            : flags(f), invalidChars(0), d(nullptr)
        {
        }
        ~ConverterState();

        ConversionFlags flags;
        int invalidChars;
    private:
        void *d;

        friend class QIcuCodec;
        friend class QTextStreamPrivate;
        friend class QTextStream;

        ConverterState(const ConverterState &other);
        ConverterState& operator=(const ConverterState &other);
    };

    QString toUnicode(const char *in, int length, ConverterState *state = nullptr) const
        { return convertToUnicode(in, length, state); }
    QByteArray fromUnicode(const QChar *in, int length, ConverterState *state = nullptr) const
        { return convertFromUnicode(in, length, state); }

    QTextDecoder* makeDecoder(ConversionFlags flags = DefaultConversion) const;
    QTextEncoder* makeEncoder(ConversionFlags flags = DefaultConversion) const;

    virtual QByteArray name() const = 0;
    virtual QList<QByteArray> aliases() const;
    virtual int mibEnum() const = 0;

protected:
    virtual QString convertToUnicode(const char *in, int length, ConverterState *state) const = 0;
    virtual QByteArray convertFromUnicode(const QChar *in, int length, ConverterState *state) const = 0;

    QTextCodec();
    virtual ~QTextCodec();

private:
    friend class QTextCodecCleanup;
    static QTextCodec *cftr;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QTextCodec::ConversionFlags)

class Q_CORE_EXPORT QTextEncoder {
    Q_DISABLE_COPY(QTextEncoder)
public:
    explicit QTextEncoder(const QTextCodec *codec) : c(codec) {}
    QTextEncoder(const QTextCodec *codec, QTextCodec::ConversionFlags flags);
    ~QTextEncoder();
    QByteArray fromUnicode(const QString& str);
    QByteArray fromUnicode(const QChar *uc, int len);
    bool hasFailure() const;
private:
    const QTextCodec *c;
    QTextCodec::ConverterState state;
};

class Q_CORE_EXPORT QTextDecoder {
    Q_DISABLE_COPY(QTextDecoder)
public:
    explicit QTextDecoder(const QTextCodec *codec) : c(codec) {}
    QTextDecoder(const QTextCodec *codec, QTextCodec::ConversionFlags flags);
    ~QTextDecoder();
    QString toUnicode(const char* chars, int len);
    QString toUnicode(const QByteArray &ba);
    void toUnicode(QString *target, const char *chars, int len);
    bool hasFailure() const;
private:
    const QTextCodec *c;
    QTextCodec::ConverterState state;
};

#endif // QT_NO_TEXTCODEC

QT_END_NAMESPACE


#endif // QTEXTCODEC_H
