/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QTextCodec>
#include <QtTest/QtTest>

#if !defined(QWS) && defined(Q_OS_MAC)
#include "qcore_mac_p.h"
#endif

#ifdef Q_OS_UNIX
#include <sys/ipc.h>
#include <sys/mman.h>
#include <unistd.h>
#endif

// MAP_ANON is deprecated on Linux, and MAP_ANONYMOUS is not present on Mac
#ifndef MAP_ANONYMOUS
# define MAP_ANONYMOUS MAP_ANON
#endif

#include "data.h"

class tst_QString: public QObject
{
    Q_OBJECT
public:
    tst_QString();
private slots:
    void equals() const;
    void equals_data() const;
    void equals2_data() const;
    void equals2() const;
    void ucstrncmp_data() const;
    void ucstrncmp() const;
    void fromUtf8() const;
    void fromLatin1_data() const;
    void fromLatin1() const;
    void fromLatin1Alternatives_data() const;
    void fromLatin1Alternatives() const;
    void fromUtf8Alternatives_data() const;
    void fromUtf8Alternatives() const;

#if !defined(QWS) && defined(Q_OS_MAC)
    void QCFString_data() const;
    void QCFString_toCFStringRef_data() const;
    void QCFString_toCFStringRef() const;
    void QCFString_operatorCFStringRef_data() const;
    void QCFString_operatorCFStringRef() const;
    void QCFString_toQString_data() const;
    void QCFString_toQString() const;
    void QCFString_operatorQString_data() const;
    void QCFString_operatorQString() const;
#endif // !defined(QWS) && defined(Q_OS_MAC)
};

void tst_QString::equals() const
{
    QFETCH(QString, a);
    QFETCH(QString, b);

    QBENCHMARK {
        (void)(a == b);
    }
}

tst_QString::tst_QString()
{
}

void tst_QString::equals_data() const
{
    static const struct {
        ushort data[80];
        int dummy;              // just to ensure 4-byte alignment
    } data = {
        {
            64, 64, 64, 64,  64, 64, 64, 64,
            64, 64, 64, 64,  64, 64, 64, 64, // 16
            64, 64, 64, 64,  64, 64, 64, 64,
            64, 64, 64, 64,  64, 64, 64, 64, // 32
            64, 64, 64, 64,  64, 64, 64, 64,
            64, 64, 64, 64,  64, 64, 64, 64, // 48
            64, 64, 64, 64,  64, 64, 64, 64,
            64, 64, 64, 64,  64, 64, 64, 64, // 64
            64, 64, 64, 64,  96, 96, 96, 96,
            64, 64, 96, 96,  96, 96, 96, 96  // 80
        }, 0
    };
    const QChar *ptr = reinterpret_cast<const QChar *>(data.data);

    QTest::addColumn<QString>("a");
    QTest::addColumn<QString>("b");
    QString base = QString::fromRawData(ptr, 64);

    QTest::newRow("different-length") << base << QString::fromRawData(ptr, 4);
    QTest::newRow("same-string") << base << base;
    QTest::newRow("same-data") << base << QString::fromRawData(ptr, 64);

    // try to avoid crossing a cache line (that is, at ptr[64])
    QTest::newRow("aligned-aligned-4n")
            << QString::fromRawData(ptr, 60) << QString::fromRawData(ptr + 2, 60);
    QTest::newRow("aligned-unaligned-4n")
            << QString::fromRawData(ptr, 60) << QString::fromRawData(ptr + 1, 60);
    QTest::newRow("unaligned-unaligned-4n")
            << QString::fromRawData(ptr + 1, 60) << QString::fromRawData(ptr + 3, 60);

    QTest::newRow("aligned-aligned-4n+1")
            << QString::fromRawData(ptr, 61) << QString::fromRawData(ptr + 2, 61);
    QTest::newRow("aligned-unaligned-4n+1")
            << QString::fromRawData(ptr, 61) << QString::fromRawData(ptr + 1, 61);
    QTest::newRow("unaligned-unaligned-4n+1")
            << QString::fromRawData(ptr + 1, 61) << QString::fromRawData(ptr + 3, 61);

    QTest::newRow("aligned-aligned-4n-1")
            << QString::fromRawData(ptr, 59) << QString::fromRawData(ptr + 2, 59);
    QTest::newRow("aligned-unaligned-4n-1")
            << QString::fromRawData(ptr, 59) << QString::fromRawData(ptr + 1, 59);
    QTest::newRow("unaligned-unaligned-4n-1")
            << QString::fromRawData(ptr + 1, 59) << QString::fromRawData(ptr + 3, 59);

    QTest::newRow("aligned-aligned-2n")
            << QString::fromRawData(ptr, 58) << QString::fromRawData(ptr + 2, 58);
    QTest::newRow("aligned-unaligned-2n")
            << QString::fromRawData(ptr, 58) << QString::fromRawData(ptr + 1, 58);
    QTest::newRow("unaligned-unaligned-2n")
            << QString::fromRawData(ptr + 1, 58) << QString::fromRawData(ptr + 3, 58);
}

static bool equals2_memcmp_call(const ushort *p1, const ushort *p2, int len)
{
    return memcmp(p1, p2, len * 2) == 0;
}

static bool equals2_bytewise(const ushort *p1, const ushort *p2, int len)
{
    if (p1 == p2 || !len)
        return true;
    uchar *b1 = (uchar *)p1;
    uchar *b2 = (uchar *)p2;
    len *= 2;
    while (len--)
        if (*b1++ != *b2++)
            return false;
    return true;
}

static bool equals2_shortwise(const ushort *p1, const ushort *p2, int len)
{
    if (p1 == p2 || !len)
        return true;
//    for (int counter; counter < len; ++counter)
//        if (p1[counter] != p2[counter])
//            return false;
    while (len--) {
        if (p1[len] != p2[len])
            return false;
    }
    return true;
}

static bool equals2_intwise(const ushort *p1, const ushort *p2, int length)
{
    if (p1 == p2 || !length)
        return true;
    union {
        const quint16 *w;
        const quint32 *d;
        quintptr value;
    } sa, sb;
    sa.w = p1;
    sb.w = p2;

    // check alignment
    if ((sa.value & 2) == (sb.value & 2)) {
        // both addresses have the same alignment
        if (sa.value & 2) {
            // both addresses are not aligned to 4-bytes boundaries
            // compare the first character
            if (*sa.w != *sb.w)
                return false;
            --length;
            ++sa.w;
            ++sb.w;

            // now both addresses are 4-bytes aligned
        }

        // both addresses are 4-bytes aligned
        // do a fast 32-bit comparison
        const quint32 *e = sa.d + (length >> 1);
        for ( ; sa.d != e; ++sa.d, ++sb.d) {
            if (*sa.d != *sb.d)
                return false;
        }

        // do we have a tail?
        return (length & 1) ? *sa.w == *sb.w : true;
    } else {
        // one of the addresses isn't 4-byte aligned but the other is
        const quint16 *e = sa.w + length;
        for ( ; sa.w != e; ++sa.w, ++sb.w) {
            if (*sa.w != *sb.w)
                return false;
        }
    }
    return true;
}

static inline bool equals2_short_tail(const ushort *p1, const ushort *p2, int len)
{
    if (len) {
        if (*p1 != *p2)
            return false;
        if (--len) {
            if (p1[1] != p2[1])
                return false;
            if (--len) {
                if (p1[2] != p2[2])
                    return false;
                if (--len) {
                    if (p1[3] != p2[3])
                        return false;
                    if (--len) {
                        if (p1[4] != p2[4])
                            return false;
                        if (--len) {
                            if (p1[5] != p2[5])
                                return false;
                            if (--len) {
                                if (p1[6] != p2[6])
                                    return false;
                                return p1[7] == p2[7];
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

typedef bool (* FuncPtr)(const ushort *, const ushort *, int);
static const FuncPtr func[] = {
    equals2_memcmp_call, // 0
    equals2_bytewise, // 1
    equals2_shortwise, // 1
    equals2_intwise, // 3
    0
};
static const int functionCount = sizeof(func)/sizeof(func[0]) - 1;

void tst_QString::equals2_data() const
{
    QTest::addColumn<int>("algorithm");
    QTest::newRow("selftest") << -1;
    QTest::newRow("memcmp_call") << 0;
    QTest::newRow("bytewise") << 1;
    QTest::newRow("shortwise") << 2;
    QTest::newRow("intwise") << 3;
}

static void __attribute__((noinline)) equals2_selftest()
{
#if defined(Q_OS_UNIX)
    const long pagesize = sysconf(_SC_PAGESIZE);
    void *page1, *page3;
    ushort *page2;
    page1 = mmap(0, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    page2 = (ushort *)mmap(0, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    page3 = mmap(0, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    Q_ASSERT(quintptr(page2) == quintptr(page1) + pagesize || quintptr(page2) == quintptr(page1) - pagesize);
    Q_ASSERT(quintptr(page3) == quintptr(page2) + pagesize || quintptr(page3) == quintptr(page2) - pagesize);
    munmap(page1, pagesize);
    munmap(page3, pagesize);

    // populate our page
    for (uint i = 0; i < pagesize / sizeof(long long); ++i)
        ((long long *)page2)[i] = Q_INT64_C(0x0041004100410041);

    // the following should crash:
    //page2[-1] = 0xdead;
    //page2[pagesize / sizeof(ushort) + 1] = 0xbeef;

    static const ushort needle[] = {
        0x41, 0x41, 0x41, 0x41,   0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41,   0x41, 0x41, 0x41, 0x41,
        0x41
    };

    for (int algo = 0; algo < functionCount; ++algo) {
        // boundary condition test:
        for (int i = 0; i < 8; ++i) {
            (func[algo])(page2 + i, needle, sizeof needle / 2);
            (func[algo])(page2 - i - 1 - sizeof(needle)/2 + pagesize/2, needle, sizeof needle/2);
        }
    }

    munmap(page2, pagesize);
#endif

    for (int algo = 0; algo < functionCount; ++algo) {
        for (int i = 0; i < stringCollectionCount; ++i) {
            const ushort *p1 = stringCollectionData + stringCollection[i].offset1;
            const ushort *p2 = stringCollectionData + stringCollection[i].offset2;
            bool expected = memcmp(p1, p2, stringCollection[i].len * 2) == 0;

            bool result = (func[algo])(p1, p2, stringCollection[i].len);
            if (expected != result)
                qWarning().nospace()
                        << "algo=" << algo
                        << " i=" << i
                        << " failed (" << result << "!=" << expected
                        << "); strings were "
                        << QByteArray((char*)p1, stringCollection[i].len).toHex()
                        << " and "
                        << QByteArray((char*)p2, stringCollection[i].len).toHex();
        }
    }
}

void tst_QString::equals2() const
{
    QFETCH(int, algorithm);
    if (algorithm == -1) {
        equals2_selftest();
        return;
    }

    QBENCHMARK {
        for (int i = 0; i < stringCollectionCount; ++i) {
            const ushort *p1 = stringCollectionData + stringCollection[i].offset1;
            const ushort *p2 = stringCollectionData + stringCollection[i].offset2;
            bool result = (func[algorithm])(p1, p2, stringCollection[i].len);
            Q_UNUSED(result);
        }
    }
}

static int ucstrncmp_shortwise(const ushort *a, const ushort *b, int l)
{
    while (l-- && *a == *b)
        a++,b++;
    if (l==-1)
        return 0;
    return *a - *b;
}

static int ucstrncmp_intwise(const ushort *a, const ushort *b, int len)
{
    // do both strings have the same alignment?
    if ((quintptr(a) & 2) == (quintptr(b) & 2)) {
        // are we aligned to 4 bytes?
        if (quintptr(a) & 2) {
            if (*a != *b)
                return *a - *b;
            ++a;
            ++b;
            --len;
        }

        const uint *p1 = (const uint *)a;
        const uint *p2 = (const uint *)b;
        quintptr counter = 0;
        for ( ; len > 1 ; len -= 2, ++counter) {
            if (p1[counter] != p2[counter]) {
                // which ushort isn't equal?
                int diff = a[2*counter] - b[2*counter];
                return diff ? diff : a[2*counter + 1] - b[2*counter + 1];
            }
        }

        return len ? a[2*counter] - b[2*counter] : 0;
    } else {
        while (len-- && *a == *b)
            a++,b++;
        if (len==-1)
            return 0;
        return *a - *b;
    }
}

typedef int (* UcstrncmpFunction)(const ushort *, const ushort *, int);
Q_DECLARE_METATYPE(UcstrncmpFunction)

void tst_QString::ucstrncmp_data() const
{
    QTest::addColumn<UcstrncmpFunction>("function");
    QTest::newRow("selftest") << UcstrncmpFunction(0);
    QTest::newRow("shortwise") << &ucstrncmp_shortwise;
    QTest::newRow("intwise") << &ucstrncmp_intwise;
}

void tst_QString::ucstrncmp() const
{
    QFETCH(UcstrncmpFunction, function);
    if (!function) {
        static const UcstrncmpFunction func[] = {
            &ucstrncmp_shortwise,
            &ucstrncmp_intwise,
        };
        static const int functionCount = sizeof func / sizeof func[0];

#if defined(Q_OS_UNIX)
        const long pagesize = sysconf(_SC_PAGESIZE);
        void *page1, *page3;
        ushort *page2;
        page1 = mmap(0, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        page2 = (ushort *)mmap(0, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        page3 = mmap(0, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        Q_ASSERT(quintptr(page2) == quintptr(page1) + pagesize || quintptr(page2) == quintptr(page1) - pagesize);
        Q_ASSERT(quintptr(page3) == quintptr(page2) + pagesize || quintptr(page3) == quintptr(page2) - pagesize);
        munmap(page1, pagesize);
        munmap(page3, pagesize);

        // populate our page
        for (uint i = 0; i < pagesize / sizeof(long long); ++i)
            ((long long *)page2)[i] = Q_INT64_C(0x0041004100410041);

        // the following should crash:
        //page2[-1] = 0xdead;
        //page2[pagesize / sizeof(ushort) + 1] = 0xbeef;

        static const ushort needle[] = {
            0x41, 0x41, 0x41, 0x41,   0x41, 0x41, 0x41, 0x41,
            0x41, 0x41, 0x41, 0x41,   0x41, 0x41, 0x41, 0x41,
            0x41
        };

        for (int algo = 0; algo < functionCount; ++algo) {
            // boundary condition test:
            for (int i = 0; i < 8; ++i) {
                (func[algo])(page2 + i, needle, sizeof needle / 2);
                (func[algo])(page2 - i - 1 - sizeof(needle)/2 + pagesize/2, needle, sizeof needle/2);
            }
        }

        munmap(page2, pagesize);
#endif

        for (int algo = 0; algo < functionCount; ++algo) {
            for (int i = 0; i < stringCollectionCount; ++i) {
                const ushort *p1 = stringCollectionData + stringCollection[i].offset1;
                const ushort *p2 = stringCollectionData + stringCollection[i].offset2;
                int expected = ucstrncmp_shortwise(p1, p2, stringCollection[i].len);
                expected = qBound(-1, expected, 1);

                int result = (func[algo])(p1, p2, stringCollection[i].len);
                result = qBound(-1, result, 1);
                if (expected != result)
                    qWarning().nospace()
                        << "algo=" << algo
                        << " i=" << i
                        << " failed (" << result << "!=" << expected
                        << "); strings were "
                        << QByteArray((char*)p1, stringCollection[i].len).toHex()
                        << " and "
                        << QByteArray((char*)p2, stringCollection[i].len).toHex();
            }
        }
        return;
    }

    QBENCHMARK {
        for (int i = 0; i < stringCollectionCount; ++i) {
            const ushort *p1 = stringCollectionData + stringCollection[i].offset1;
            const ushort *p2 = stringCollectionData + stringCollection[i].offset2;
            (function)(p1, p2, stringCollection[i].len);
        }
    }
}

void tst_QString::fromUtf8() const
{
    QFile file(QLatin1String(SRCDIR "utf-8.txt"));
    if (!file.open(QFile::ReadOnly)) {
        qFatal("Cannot open input file");
        return;
    }
    QByteArray data = file.readAll();
    const char *d = data.constData();
    int size = data.size();

    QBENCHMARK {
        QString::fromUtf8(d, size);
    }
}

void tst_QString::fromLatin1_data() const
{
    QTest::addColumn<QByteArray>("latin1");

    // make all the strings have the same length
    QTest::newRow("ascii-only") << QByteArray("HelloWorld");
    QTest::newRow("ascii+control") << QByteArray("Hello\1\r\n\x7f\t");
    QTest::newRow("ascii+nul") << QByteArray("a\0zbc\0defg", 10);
    QTest::newRow("non-ascii") << QByteArray("\x80\xc0\xff\x81\xc1\xfe\x90\xd0\xef\xa0");
}

void tst_QString::fromLatin1() const
{
    QFETCH(QByteArray, latin1);

    while (latin1.length() < 128) {
        latin1 += latin1;
    }

    QByteArray copy1 = latin1, copy2 = latin1, copy3 = latin1;
    copy1.chop(1);
    copy2.detach();
    copy3 += latin1; // longer length
    copy2.clear();

    QBENCHMARK {
        QString s1 = QString::fromLatin1(latin1);
        QString s2 = QString::fromLatin1(latin1);
        QString s3 = QString::fromLatin1(copy1);
        QString s4 = QString::fromLatin1(copy3);
        s3 = QString::fromLatin1(copy3);
    }
}

typedef void (* FromLatin1Function)(ushort *, const char *, int);
Q_DECLARE_METATYPE(FromLatin1Function)

void fromLatin1_regular(ushort *dst, const char *str, int size)
{
    // from qstring.cpp:
    while (size--)
        *dst++ = (uchar)*str++;
}

void tst_QString::fromLatin1Alternatives_data() const
{
    QTest::addColumn<FromLatin1Function>("function");
    QTest::newRow("empty") << FromLatin1Function(0);
    QTest::newRow("regular") << &fromLatin1_regular;
}

extern StringData fromLatin1Data;
static void fromLatin1Alternatives_internal(FromLatin1Function function, QString &dst, bool doVerify)
{
    struct Entry
    {
        int len;
        int offset1, offset2;
        int align1, align2;
    };
    const Entry *entries = reinterpret_cast<const Entry *>(fromLatin1Data.entries);

    for (int i = 0; i < fromLatin1Data.entryCount; ++i) {
        int len = entries[i].len;
        const char *src = fromLatin1Data.charData + entries[i].offset1;

        if (!function)
            continue;
        if (!doVerify) {
            (function)(&dst.data()->unicode(), src, len);
        } else {
            dst.fill(QLatin1Char('x'), dst.length());

            (function)(&dst.data()->unicode() + 8, src, len);

            QString zeroes(8, QLatin1Char('x'));
            QString final = dst.mid(8, len);
            QCOMPARE(final, QString::fromLatin1(src, len));
            QCOMPARE(dst.left(8), zeroes);
            QCOMPARE(dst.mid(len + 8, 8), zeroes);
        }
    }
}

void tst_QString::fromLatin1Alternatives() const
{
    QFETCH(FromLatin1Function, function);

    QString dst(fromLatin1Data.maxLength + 16, QLatin1Char('x'));
    fromLatin1Alternatives_internal(function, dst, true);

    QBENCHMARK {
        fromLatin1Alternatives_internal(function, dst, false);
    }
}

typedef int (* FromUtf8Function)(ushort *, const char *, int);
Q_DECLARE_METATYPE(FromUtf8Function)

extern QTextCodec::ConverterState *state;
QTextCodec::ConverterState *state = 0; // just because the code in qutfcodec.cpp uses a state

int fromUtf8_latin1_regular(ushort *dst, const char *chars, int len)
{
    fromLatin1_regular(dst, chars, len);
    return len;
}

static inline bool isUnicodeNonCharacter(uint ucs4)
{
    // Unicode has a couple of "non-characters" that one can use internally,
    // but are not allowed to be used for text interchange.
    //
    // Those are the last two entries each Unicode Plane (U+FFFE, U+FFFF,
    // U+1FFFE, U+1FFFF, etc.) as well as the entries between U+FDD0 and
    // U+FDEF (inclusive)

    return (ucs4 & 0xfffe) == 0xfffe
            || (ucs4 - 0xfdd0U) < 16;
}

int fromUtf8_qt47(ushort *dst, const char *chars, int len)
{
    // this is almost the code found in Qt 4.7's qutfcodec.cpp QUtf8Codec::convertToUnicode
    // That function returns a QString, this one returns the number of characters converted
    // That's to avoid doing malloc() inside the benchmark test
    // Any differences between this code and the original are just because of that, I promise

    bool headerdone = false;
    ushort replacement = QChar::ReplacementCharacter;
    int need = 0;
    int error = -1;
    uint uc = 0;
    uint min_uc = 0;
    if (state) {
        if (state->flags & QTextCodec::IgnoreHeader)
            headerdone = true;
        if (state->flags & QTextCodec::ConvertInvalidToNull)
            replacement = QChar::Null;
        need = state->remainingChars;
        if (need) {
            uc = state->state_data[0];
            min_uc = state->state_data[1];
        }
    }
    if (!headerdone && len > 3
        && (uchar)chars[0] == 0xef && (uchar)chars[1] == 0xbb && (uchar)chars[2] == 0xbf) {
        // starts with a byte order mark
        chars += 3;
        len -= 3;
        headerdone = true;
    }

    // QString result(need + len + 1, Qt::Uninitialized); // worst case
    // ushort *qch = (ushort *)result.unicode();
    ushort *qch = dst;
    uchar ch;
    int invalid = 0;

    for (int i = 0; i < len; ++i) {
        ch = chars[i];
        if (need) {
            if ((ch&0xc0) == 0x80) {
                uc = (uc << 6) | (ch & 0x3f);
                --need;
                if (!need) {
                    // utf-8 bom composes into 0xfeff code point
                    bool nonCharacter;
                    if (!headerdone && uc == 0xfeff) {
                        // don't do anything, just skip the BOM
                    } else if (!(nonCharacter = isUnicodeNonCharacter(uc)) && uc > 0xffff && uc < 0x110000) {
                        // surrogate pair
                        //Q_ASSERT((qch - (ushort*)result.unicode()) + 2 < result.length());
                        *qch++ = QChar::highSurrogate(uc);
                        *qch++ = QChar::lowSurrogate(uc);
                    } else if ((uc < min_uc) || (uc >= 0xd800 && uc <= 0xdfff) || nonCharacter || uc >= 0x110000) {
                        // error: overlong sequence, UTF16 surrogate or non-character
                        *qch++ = replacement;
                        ++invalid;
                    } else {
                        *qch++ = uc;
                    }
                    headerdone = true;
                }
            } else {
                // error
                i = error;
                *qch++ = replacement;
                ++invalid;
                need = 0;
                headerdone = true;
            }
        } else {
            if (ch < 128) {
                *qch++ = ushort(ch);
                headerdone = true;
            } else if ((ch & 0xe0) == 0xc0) {
                uc = ch & 0x1f;
                need = 1;
                error = i;
                min_uc = 0x80;
                headerdone = true;
            } else if ((ch & 0xf0) == 0xe0) {
                uc = ch & 0x0f;
                need = 2;
                error = i;
                min_uc = 0x800;
            } else if ((ch&0xf8) == 0xf0) {
                uc = ch & 0x07;
                need = 3;
                error = i;
                min_uc = 0x10000;
                headerdone = true;
            } else {
                // error
                *qch++ = replacement;
                ++invalid;
                headerdone = true;
            }
        }
    }
    if (!state && need > 0) {
        // unterminated UTF sequence
        for (int i = error; i < len; ++i) {
            *qch++ = replacement;
            ++invalid;
        }
    }
    //result.truncate(qch - (ushort *)result.unicode());
    if (state) {
        state->invalidChars += invalid;
        state->remainingChars = need;
        if (headerdone)
            state->flags |= QTextCodec::IgnoreHeader;
        state->state_data[0] = need ? uc : 0;
        state->state_data[1] = need ? min_uc : 0;
    }
    //return result;
    return qch - dst;
}

int fromUtf8_qt47_stateless(ushort *dst, const char *chars, int len)
{
    // This is the same code as above, but for stateless UTF-8 conversion
    // no other improvements
    bool headerdone = false;
    const ushort replacement = QChar::ReplacementCharacter;
    int need = 0;
    int error = -1;
    uint uc = 0;
    uint min_uc = 0;

    if (len > 3
        && (uchar)chars[0] == 0xef && (uchar)chars[1] == 0xbb && (uchar)chars[2] == 0xbf) {
        // starts with a byte order mark
        chars += 3;
        len -= 3;
    }

    // QString result(need + len + 1, Qt::Uninitialized); // worst case
    // ushort *qch = (ushort *)result.unicode();
    ushort *qch = dst;
    uchar ch;
    int invalid = 0;

    for (int i = 0; i < len; ++i) {
        ch = chars[i];
        if (need) {
            if ((ch&0xc0) == 0x80) {
                uc = (uc << 6) | (ch & 0x3f);
                --need;
                if (!need) {
                    // utf-8 bom composes into 0xfeff code point
                    bool nonCharacter;
                    if (!headerdone && uc == 0xfeff) {
                        // don't do anything, just skip the BOM
                    } else if (!(nonCharacter = isUnicodeNonCharacter(uc)) && uc > 0xffff && uc < 0x110000) {
                        // surrogate pair
                        //Q_ASSERT((qch - (ushort*)result.unicode()) + 2 < result.length());
                        *qch++ = QChar::highSurrogate(uc);
                        *qch++ = QChar::lowSurrogate(uc);
                    } else if ((uc < min_uc) || (uc >= 0xd800 && uc <= 0xdfff) || nonCharacter || uc >= 0x110000) {
                        // error: overlong sequence, UTF16 surrogate or non-character
                        *qch++ = replacement;
                        ++invalid;
                    } else {
                        *qch++ = uc;
                    }
                    headerdone = true;
                }
            } else {
                // error
                i = error;
                *qch++ = replacement;
                ++invalid;
                need = 0;
                headerdone = true;
            }
        } else {
            if (ch < 128) {
                *qch++ = ushort(ch);
                headerdone = true;
            } else if ((ch & 0xe0) == 0xc0) {
                uc = ch & 0x1f;
                need = 1;
                error = i;
                min_uc = 0x80;
                headerdone = true;
            } else if ((ch & 0xf0) == 0xe0) {
                uc = ch & 0x0f;
                need = 2;
                error = i;
                min_uc = 0x800;
            } else if ((ch&0xf8) == 0xf0) {
                uc = ch & 0x07;
                need = 3;
                error = i;
                min_uc = 0x10000;
                headerdone = true;
            } else {
                // error
                *qch++ = replacement;
                ++invalid;
                headerdone = true;
            }
        }
    }
    if (need > 0) {
        // unterminated UTF sequence
        for (int i = error; i < len; ++i) {
            *qch++ = replacement;
            ++invalid;
        }
    }
    //result.truncate(qch - (ushort *)result.unicode());
    //return result;
    return qch - dst;
}

template <bool trusted>
static inline void extract_utf8_multibyte(ushort *&dst, const char *&chars, qptrdiff &counter, int &len)
{
    uchar ch = chars[counter];

    // is it a leading or a continuation one?
    if (!trusted && (ch & 0xc0) == 0x80) {
        // continuation character found without the leading
        dst[counter++] = QChar::ReplacementCharacter;
        return;
    }

    if ((ch & 0xe0) == 0xc0) {
        // two-byte UTF-8 sequence
        if (!trusted && counter + 1 == len) {
            dst[counter++] = QChar::ReplacementCharacter;
            return;
        }

        uchar ch2 = chars[counter + 1];
        if (!trusted)
            if ((ch2 & 0xc0) != 0x80) {
                dst[counter++] = QChar::ReplacementCharacter;
                return;
            }

        ushort ucs = (ch & 0x1f);
        ucs <<= 6;
        ucs |= (ch2 & 0x3f);

        // dst[counter] will correspond to chars[counter..counter+1], so adjust
        ++chars;
        --len;
        if (trusted || ucs >= 0x80)
            dst[counter] = ucs;
        else
            dst[counter] = QChar::ReplacementCharacter;
        ++counter;
        return;
    }

    if ((ch & 0xf0) == 0xe0) {
        // three-byte UTF-8 sequence
        if (!trusted && counter + 2 >= len) {
            dst[counter++] = QChar::ReplacementCharacter;
            return;
        }

        uchar ch2 = chars[counter + 1];
        uchar ch3 = chars[counter + 2];
        if (!trusted)
            if ((ch2 & 0xc0) != 0x80 || (ch3 & 0xc0) != 0x80) {
                dst[counter++] = QChar::ReplacementCharacter;
                return;
            }

        ushort ucs = (ch & 0x1f) << 12 | (ch2 & 0x3f) << 6 | (ch3 & 0x3f);

        // dst[counter] will correspond to chars[counter..counter+2], so adjust
        chars += 2;
        len -= 2;
        if (!trusted &&
            (ucs < 0x800 || isUnicodeNonCharacter(ucs) || (ucs >= 0xd800 && ucs <= 0xdfff)))
            dst[counter] = QChar::ReplacementCharacter;
        else
            dst[counter] = ucs;
        ++counter;
        return;
    }

    if ((ch & 0xf8) == 0xf0) {
        // four-byte UTF-8 sequence
        // will require an UTF-16 surrogate pair
        if (!trusted && counter + 3 >= len) {
            dst[counter++] = QChar::ReplacementCharacter;
            return;
        }

        uchar ch2 = chars[counter + 1];
        uchar ch3 = chars[counter + 2];
        uchar ch4 = chars[counter + 3];
        if (!trusted)
            if ((ch2 & 0xc0) != 0x80 || (ch3 & 0xc0) != 0x80 || (ch4 & 0xc0) != 0x80) {
                dst[counter++] = QChar::ReplacementCharacter;
                return;
            }

        uint ucs = (ch & 0x1f) << 18 | (ch2 & 0x3f) << 12
                   | (ch3 & 0x3f) << 6 | (ch4 & 0x3f);

        // dst[counter] will correspond to chars[counter..counter+2], so adjust
        chars += 3;
        len -= 3;
        if (trusted || (ucs >= 0x10000 && ucs < 0x110000 && !isUnicodeNonCharacter(ucs))) {
            dst[counter + 0] = QChar::highSurrogate(ucs);
            dst[counter + 1] = QChar::lowSurrogate(ucs);
            counter += 2;
        } else {
            dst[counter++] = QChar::ReplacementCharacter;
        }
        return;
    }

    ++counter;
}

int fromUtf8_optimised_for_ascii(ushort *qch, const char *chars, int len)
{
    if (len > 3
        && (uchar)chars[0] == 0xef && (uchar)chars[1] == 0xbb && (uchar)chars[2] == 0xbf) {
        // starts with a byte order mark
        chars += 3;
        len -= 3;
    }

    qptrdiff counter = 0;
    ushort *dst = qch;
    while (counter < len) {
        uchar ch = chars[counter];
        if ((ch & 0x80) == 0) {
            dst[counter] = ch;
            ++counter;
            continue;
        }

        // UTF-8 character found
        extract_utf8_multibyte<false>(dst, chars, counter, len);
    }
    return dst + counter - qch;
}

void tst_QString::fromUtf8Alternatives_data() const
{
    QTest::addColumn<FromUtf8Function>("function");
    QTest::newRow("empty") << FromUtf8Function(0);
    QTest::newRow("qt-4.7") << &fromUtf8_qt47;
    QTest::newRow("qt-4.7-stateless") << &fromUtf8_qt47_stateless;
    QTest::newRow("optimized-for-ascii") << &fromUtf8_optimised_for_ascii;
    QTest::newRow("latin1-generic") << &fromUtf8_latin1_regular;
}

extern StringData fromUtf8Data;
static void fromUtf8Alternatives_internal(FromUtf8Function function, QString &dst, bool doVerify)
{
    if (!doVerify) {
        // NOTE: this only works because the Latin1 data is ASCII-only
        fromLatin1Alternatives_internal(reinterpret_cast<FromLatin1Function>(function), dst, doVerify);
    } else {
        if (strncmp(QTest::currentDataTag(), "latin1-", 7) == 0)
            return;
    }

    struct Entry
    {
        int len;
        int offset1, offset2;
        int align1, align2;
    };
    const Entry *entries = reinterpret_cast<const Entry *>(fromUtf8Data.entries);

    for (int i = 0; i < fromUtf8Data.entryCount; ++i) {
        int len = entries[i].len;
        const char *src = fromUtf8Data.charData + entries[i].offset1;

        if (!function)
            continue;
        if (!doVerify) {
            (function)(&dst.data()->unicode(), src, len);
        } else {
            dst.fill(QLatin1Char('x'), dst.length());

            int utf8len = (function)(&dst.data()->unicode() + 8, src, len);

            QString expected = QString::fromUtf8(src, len);
            QString final = dst.mid(8, expected.length());
            if (final != expected || utf8len != expected.length())
                qDebug() << i << entries[i].offset1 << utf8len << final << expected.length() << expected;

            QCOMPARE(final, expected);
            QCOMPARE(utf8len, expected.length());

            QString zeroes(8, QLatin1Char('x'));
            QCOMPARE(dst.left(8), zeroes);
            QCOMPARE(dst.mid(len + 8, 8), zeroes);
        }
    }
}

void tst_QString::fromUtf8Alternatives() const
{
    QFETCH(FromUtf8Function, function);

    QString dst(fromUtf8Data.maxLength + 16, QLatin1Char('x'));
    fromUtf8Alternatives_internal(function, dst, true);

    QBENCHMARK {
        fromUtf8Alternatives_internal(function, dst, false);
    }
}

QTEST_MAIN(tst_QString)

#include "moc_main.cpp"
