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

/*!
    \class QUrl

    \brief The QUrl class provides a convenient interface for working
    with URLs.

    \reentrant
    \ingroup io
    \ingroup network
    \ingroup shared


    It can parse and construct URLs in both encoded and unencoded
    form. QUrl also has support for internationalized domain names
    (IDNs).

    The most common way to use QUrl is to initialize it via the
    constructor by passing a QString. Otherwise, setUrl() and
    setEncodedUrl() can also be used.

    URLs can be represented in two forms: encoded or unencoded. The
    unencoded representation is suitable for showing to users, but
    the encoded representation is typically what you would send to
    a web server. For example, the unencoded URL
    "http://b\uuml\c{}hler.example.com/List of applicants.xml" would be sent to the server as
    "http://xn--bhler-kva.example.com/List%20of%20applicants.xml",
    and this can be verified by calling the toEncoded() function.

    A URL can also be constructed piece by piece by calling
    setScheme(), setUserName(), setPassword(), setHost(), setPort(),
    setPath(), setEncodedQuery() and setFragment(). Some convenience
    functions are also available: setAuthority() sets the user name,
    password, host and port. setUserInfo() sets the user name and
    password at once.

    Call isValid() to check if the URL is valid. This can be done at
    any point during the constructing of a URL.

    Constructing a query is particularly convenient through the use
    of setQueryItems(), addQueryItem() and removeQueryItem(). Use
    setQueryDelimiters() to customize the delimiters used for
    generating the query string.

    For the convenience of generating encoded URL strings or query
    strings, there are two static functions called
    fromPercentEncoding() and toPercentEncoding() which deal with
    percent encoding and decoding of QStrings.

    Calling isRelative() will tell whether or not the URL is
    relative. A relative URL can be resolved by passing it as argument
    to resolved(), which returns an absolute URL. isParentOf() is used
    for determining whether one URL is a parent of another.

    fromLocalFile() constructs a QUrl by parsing a local
    file path. toLocalFile() converts a URL to a local file path.

    The human readable representation of the URL is fetched with
    toString(). This representation is appropriate for displaying a
    URL to a user in unencoded form. The encoded form however, as
    returned by toEncoded(), is for internal use, passing to web
    servers, mail clients and so on.

    QUrl conforms to the URI specification from
    \l{RFC 3986} (Uniform Resource Identifier: Generic Syntax), and includes
    scheme extensions from \l{RFC 1738} (Uniform Resource Locators). Case
    folding rules in QUrl conform to \l{RFC 3491} (Nameprep: A Stringprep
    Profile for Internationalized Domain Names (IDN)).

    \section2 Character Conversions

    Follow these rules to avoid erroneous character conversion when
    dealing with URLs and strings:

    \list
    \o When creating an QString to contain a URL from a QByteArray or a
       char*, always use QString::fromUtf8().
    \o Favor the use of QUrl::fromEncoded() and QUrl::toEncoded() instead of
       QUrl(string) and QUrl::toString() when converting a QUrl to or from
       a string.
    \endlist

    \sa QUrlInfo
*/

/*!
    \enum QUrl::ParsingMode

    The parsing mode controls the way QUrl parses strings.

    \value TolerantMode QUrl will try to correct some common errors in URLs.
                        This mode is useful when processing URLs entered by
                        users.

    \value StrictMode Only valid URLs are accepted. This mode is useful for
                      general URL validation.

    In TolerantMode, the parser corrects the following invalid input:

    \list

    \o Spaces and "%20": If an encoded URL contains a space, this will be
    replaced with "%20". If a decoded URL contains "%20", this will be
    replaced with a single space before the URL is parsed.

    \o Single "%" characters: Any occurrences of a percent character "%" not
    followed by exactly two hexadecimal characters (e.g., "13% coverage.html")
    will be replaced by "%25".

    \o Reserved and unreserved characters: An encoded URL should only
    contain a few characters as literals; all other characters should
    be percent-encoded. In TolerantMode, these characters will be
    automatically percent-encoded where they are not allowed:
            space / double-quote / "<" / ">" / "[" / "\" /
            "]" / "^" / "`" / "{" / "|" / "}"

    \endlist
*/

/*!
    \enum QUrl::FormattingOption

    The formatting options define how the URL is formatted when written out
    as text.

    \value None The format of the URL is unchanged.
    \value RemoveScheme  The scheme is removed from the URL.
    \value RemovePassword  Any password in the URL is removed.
    \value RemoveUserInfo  Any user information in the URL is removed.
    \value RemovePort      Any specified port is removed from the URL.
    \value RemoveAuthority
    \value RemovePath   The URL's path is removed, leaving only the scheme,
                        host address, and port (if present).
    \value RemoveQuery  The query part of the URL (following a '?' character)
                        is removed.
    \value RemoveFragment
    \value StripTrailingSlash  The trailing slash is removed if one is present.

    Note that the case folding rules in \l{RFC 3491}{Nameprep}, which QUrl
    conforms to, require host names to always be converted to lower case,
    regardless of the Qt::FormattingOptions used.
*/

/*!
 \fn uint qHash(const QUrl &url)
 \since 4.7
 \relates QUrl

 Computes a hash key from the normalized version of \a url.
 */
#include "qplatformdefs.h"
#include "qurl.h"
#include "qatomic.h"
#include "qbytearray.h"
#include "qdir.h"
#include "qfile.h"
#include "qlist.h"
#ifndef QT_NO_REGEXP
#include "qregexp.h"
#endif
#include "qstring.h"
#include "qstringlist.h"
#include "qstack.h"
#include "qdebug.h"
#include "qlocale_tools_p.h"
#ifndef QT_BOOTSTRAPPED
#include "qtldurl_p.h"
#endif
#include "qmutex.h"
#include "qorderedmutexlocker_p.h"
#include "qcorecommon_p.h"

#include <unicode/uidna.h>

QT_BEGIN_NAMESPACE

extern void q_normalizePercentEncoding(QByteArray *ba, const char *exclude);
extern void q_toPercentEncoding(QByteArray *ba, const char *exclude, const char *include = 0);
extern void q_fromPercentEncoding(QByteArray *ba);

static QByteArray toPercentEncodingHelper(const QString &s, const char *exclude, const char *include = 0)
{
    if (s.isNull())
        return QByteArray();    // null
    QByteArray ba = s.toUtf8();
    q_toPercentEncoding(&ba, exclude, include);
    return ba;
}

static QString fromPercentEncodingHelper(const QByteArray &ba)
{
    if (ba.isNull())
        return QString();       // null
    QByteArray copy = ba;
    q_fromPercentEncoding(&copy);
    return QString::fromUtf8(copy.constData(), copy.length());
}

static QString fromPercentEncodingMutable(QByteArray *ba)
{
    if (ba->isNull())
        return QString();       // null
    q_fromPercentEncoding(ba);
    return QString::fromUtf8(ba->constData(), ba->length());
}

struct QIDNA {
    QIDNA();
    ~QIDNA();

    UIDNA *idna;
};

QIDNA::QIDNA()
    : idna(nullptr)
{
    UErrorCode error = U_ZERO_ERROR;
    idna = uidna_openUTS46(UIDNA_ALLOW_UNASSIGNED | UIDNA_USE_STD3_RULES, &error);
    if (Q_UNLIKELY(U_FAILURE(error))) {
        qWarning("uidna_openUTS46 failed %s", u_errorName(error));
    }
}

QIDNA::~QIDNA()
{
    if (idna) {
        uidna_close(idna);
    }
}

Q_GLOBAL_STATIC(QIDNA, qGlobalIDNA);

// ### Qt 5: Consider accepting empty strings as valid. See task 144227.

//#define QURL_DEBUG

#define QURL_SETFLAG(a, b) { (a) |= (b); }
#define QURL_UNSETFLAG(a, b) { (a) &= ~(b); }
#define QURL_HASFLAG(a, b) (((a) & (b)) == (b))

struct QUrlErrorInfo {
    inline QUrlErrorInfo() : _source(0), _message(0), _found(0)
    { }

    const char *_source;
    const char *_message;
    char _found;

    inline void setParams(const char *source, const char *message, char found)
    {
        _source = source;
        _message = message;
        _found = found;
    }
};

struct QUrlParseData
{
    const char *scheme;
    int schemeLength;

    const char *userInfo;
    int userInfoDelimIndex;
    int userInfoLength;

    const char *host;
    int hostLength;
    int port;

    const char *path;
    int pathLength;
    const char *query;
    int queryLength;
    const char *fragment;
    int fragmentLength;
};


class QUrlPrivate
{
public:
    QUrlPrivate();
    QUrlPrivate(const QUrlPrivate &other);

    bool setUrl(const QString &url);

    QString canonicalHost() const;
    void ensureEncodedParts() const;
    QString authority(QUrl::FormattingOptions options = QUrl::None) const;
    void setAuthority(const QString &auth);
    void setUserInfo(const QString &userInfo);
    QString userInfo(QUrl::FormattingOptions options = QUrl::None) const;
    void setEncodedUserInfo(const QUrlParseData *parseData);
    void setEncodedUrl(const QByteArray &encodedUrl, QUrl::ParsingMode parsingMode);
    QString fragmentImpl() const;

    QByteArray mergePaths(const QByteArray &relativePath) const;

    void queryItem(int pos, int *value, int *end) const;

    enum ParseOptions {
        ParseAndSet,
        ParseOnly
    };

    void validate() const;
    void parse(ParseOptions parseOptions = ParseAndSet) const;
    void clear();

    QByteArray toEncoded(QUrl::FormattingOptions options = QUrl::None) const;

    QAtomicInt ref;

    QString scheme;
    QString userName;
    QString password;
    QString host;
    QString path;
    QByteArray query;
    QString fragment;

    QByteArray encodedOriginal;
    QByteArray encodedUserName;
    QByteArray encodedPassword;
    QByteArray encodedPath;
    QByteArray encodedFragment;

    int port;
    QUrl::ParsingMode parsingMode;

    bool hasQuery;
    bool hasFragment;
    bool isValid;
    bool isHostValid;

    char valueDelimiter;
    char pairDelimiter;

    enum State {
        Parsed = 0x1,
        Validated = 0x2,
        Normalized = 0x4,
        HostCanonicalized = 0x8
    };
    int stateFlags;

    // This mutex protects stateFlags, errorInfo, host,
    // hasQuery, hasFragment, isValid, isHostValid,
    // encodedNormalized and all other encoded* variables above.
    // This also protects all other variables when set by parse,
    // but not when set by setters.
    mutable QMutex mutex;

    mutable QByteArray encodedNormalized;
    const QByteArray & normalized() const;

    mutable QUrlErrorInfo errorInfo;
    QString createErrorString() const;
};


static bool QT_FASTCALL _HEXDIG(const char **ptr)
{
    char ch = **ptr;
    if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')) {
        ++(*ptr);
        return true;
    }

    return false;
}

// pct-encoded = "%" HEXDIG HEXDIG
static bool QT_FASTCALL _pctEncoded(const char **ptr)
{
    const char *ptrBackup = *ptr;

    if (**ptr != '%')
        return false;
    ++(*ptr);

    if (!_HEXDIG(ptr)) {
        *ptr = ptrBackup;
        return false;
    }
    if (!_HEXDIG(ptr)) {
        *ptr = ptrBackup;
        return false;
    }

    return true;
}

#if 0
// gen-delims  = ":" / "/" / "?" / "#" / "[" / "]" / "@"
static bool QT_FASTCALL _genDelims(const char **ptr, char *c)
{
    char ch = **ptr;
    switch (ch) {
    case ':': case '/': case '?': case '#':
    case '[': case ']': case '@':
        *c = ch;
        ++(*ptr);
        return true;
    default:
        return false;
    }
}
#endif

// sub-delims  = "!" / "$" / "&" / "'" / "(" / ")"
//             / "*" / "+" / "," / ";" / "="
static bool QT_FASTCALL _subDelims(const char **ptr)
{
    char ch = **ptr;
    switch (ch) {
    case '!': case '$': case '&': case '\'':
    case '(': case ')': case '*': case '+':
    case ',': case ';': case '=':
        ++(*ptr);
        return true;
    default:
        return false;
    }
}

// unreserved  = ALPHA / DIGIT / "-" / "." / "_" / "~"
static bool QT_FASTCALL _unreserved(const char **ptr)
{
    char ch = **ptr;
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
        || (ch >= '0' && ch <= '9')
        || ch == '-' || ch == '.' || ch == '_' || ch == '~') {
        ++(*ptr);
        return true;
    }
    return false;
}

// scheme      = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
static bool QT_FASTCALL _scheme(const char **ptr, QUrlParseData *parseData)
{
    bool first = true;
    bool isSchemeValid = true;

    parseData->scheme = *ptr;
    for (;;) {
        char ch = **ptr;
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            ;
        } else if ((ch >= '0' && ch <= '9') || ch == '+' || ch == '-' || ch == '.') {
            if (first)
                isSchemeValid = false;
        } else {
            break;
        }

        ++(*ptr);
        first = false;
    }

    if (**ptr != ':') {
        isSchemeValid = true;
        *ptr = parseData->scheme;
    } else {
        parseData->schemeLength = *ptr - parseData->scheme;
        ++(*ptr); // skip ':'
    }

    return isSchemeValid;
}

// IPvFuture  = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
static bool QT_FASTCALL _IPvFuture(const char **ptr)
{
    if (**ptr != 'v')
        return false;

    const char *ptrBackup = *ptr;
    ++(*ptr);

    if (!_HEXDIG(ptr)) {
        *ptr = ptrBackup;
        return false;
    }

    while (_HEXDIG(ptr))
        ;

    if (**ptr != '.') {
        *ptr = ptrBackup;
        return false;
    }
    ++(*ptr);

    if (!_unreserved(ptr) && !_subDelims(ptr) && *((*ptr)++) != ':') {
        *ptr = ptrBackup;
        return false;
    }


    while (_unreserved(ptr) || _subDelims(ptr) || *((*ptr)++) == ':')
        ;

    return true;
}

// h16         = 1*4HEXDIG
//             ; 16 bits of address represented in hexadecimal
static bool QT_FASTCALL _h16(const char **ptr)
{
    int i = 0;
    for (; i < 4; ++i) {
        if (!_HEXDIG(ptr))
            break;
    }
    return (i != 0);
}

// dec-octet   = DIGIT                 ; 0-9
//             / %x31-39 DIGIT         ; 10-99
//             / "1" 2DIGIT            ; 100-199
//             / "2" %x30-34 DIGIT     ; 200-249
//             / "25" %x30-35          ; 250-255
static bool QT_FASTCALL _decOctet(const char **ptr)
{
    const char *ptrBackup = *ptr;
    char c1 = **ptr;

    if (c1 < '0' || c1 > '9')
        return false;

    ++(*ptr);

    if (c1 == '0')
        return true;

    char c2 = **ptr;

    if (c2 < '0' || c2 > '9')
        return true;

    ++(*ptr);

    char c3 = **ptr;
    if (c3 < '0' || c3 > '9')
        return true;

    // If there is a three digit number larger than 255, reject the
    // whole token.
    if (c1 >= '2' && c2 >= '5' && c3 > '5') {
        *ptr = ptrBackup;
        return false;
    }

    ++(*ptr);

    return true;
}

// IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
static bool QT_FASTCALL _IPv4Address(const char **ptr)
{
    const char *ptrBackup = *ptr;

    if (!_decOctet(ptr)) {
        *ptr = ptrBackup;
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        char ch = *((*ptr)++);
        if (ch != '.') {
            *ptr = ptrBackup;
            return false;
        }

        if (!_decOctet(ptr)) {
            *ptr = ptrBackup;
            return false;
        }
    }

    return true;
}

// ls32        = ( h16 ":" h16 ) / IPv4address
//             ; least-significant 32 bits of address
static bool QT_FASTCALL _ls32(const char **ptr)
{
    const char *ptrBackup = *ptr;
    if (_h16(ptr) && *((*ptr)++) == ':' && _h16(ptr))
        return true;

    *ptr = ptrBackup;
    return _IPv4Address(ptr);
}

// IPv6address =                            6( h16 ":" ) ls32 // case 1
//             /                       "::" 5( h16 ":" ) ls32 // case 2
//             / [               h16 ] "::" 4( h16 ":" ) ls32 // case 3
//             / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 // case 4
//             / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 // case 5
//             / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32 // case 6
//             / [ *4( h16 ":" ) h16 ] "::"              ls32 // case 7
//             / [ *5( h16 ":" ) h16 ] "::"              h16  // case 8
//             / [ *6( h16 ":" ) h16 ] "::"                   // case 9
static bool QT_FASTCALL _IPv6Address(const char **ptr)
{
    const char *ptrBackup = *ptr;

    // count of (h16 ":") to the left of and including ::
    int leftHexColons = 0;
    // count of (h16 ":") to the right of ::
    int rightHexColons = 0;

    // first count the number of (h16 ":") on the left of ::
    while (_h16(ptr)) {

        // an h16 not followed by a colon is considered an
        // error.
        if (**ptr != ':') {
            *ptr = ptrBackup;
            return false;
        }
        ++(*ptr);
        ++leftHexColons;

        // check for case 1, the only time when there can be no ::
        if (leftHexColons == 6 && _ls32(ptr)) {
            return true;
        }
    }

    // check for case 2 where the address starts with a :
    if (leftHexColons == 0 && *((*ptr)++) != ':') {
        *ptr = ptrBackup;
        return false;
    }

    // check for the second colon in ::
    if (*((*ptr)++) != ':') {
        *ptr = ptrBackup;
        return false;
    }

    int canBeCase = -1;
    bool ls32WasRead = false;

    // count the number of (h16 ":") on the right of ::
    for (;;) {
        const char *tmpBackup = *ptr;
        if (!_h16(ptr)) {
            if (!_ls32(ptr)) {
                if (rightHexColons != 0) {
                    *ptr = ptrBackup;
                    return false;
                }

                // the address ended with :: (case 9)
                // only valid if 1 <= leftHexColons <= 7
                canBeCase = 9;
            } else {
                ls32WasRead = true;
            }
            break;
        }
        ++rightHexColons;
        if (**ptr != ':') {
            // no colon could mean that what was read as an h16
            // was in fact the first part of an ls32. we backtrack
            // and retry.
            const char *pb = *ptr;
            *ptr = tmpBackup;
            if (_ls32(ptr)) {
                ls32WasRead = true;
                --rightHexColons;
            } else {
                *ptr = pb;
                // address ends with only 1 h16 after :: (case 8)
                if (rightHexColons == 1)
                    canBeCase = 8;
            }
            break;
        }
        ++(*ptr);
    }

    // determine which case it is based on the number of rightHexColons
    if (canBeCase == -1) {

        // check if a ls32 was read. If it wasn't and rightHexColons >= 2 then the
        // last 2 HexColons are in fact a ls32
        if (!ls32WasRead && rightHexColons >= 2)
            rightHexColons -= 2;

        canBeCase = 7 - rightHexColons;
    }

    // based on the case we need to check that the number of leftHexColons is valid
    if (leftHexColons > (canBeCase - 2)) {
        *ptr = ptrBackup;
        return false;
    }

    return true;
}

// IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
static bool QT_FASTCALL _IPLiteral(const char **ptr)
{
    const char *ptrBackup = *ptr;
    if (**ptr != '[')
        return false;
    ++(*ptr);

    if (!_IPv6Address(ptr) && !_IPvFuture(ptr)) {
        *ptr = ptrBackup;
        return false;
    }

    if (**ptr != ']') {
        *ptr = ptrBackup;
        return false;
    }
    ++(*ptr);

    return true;
}

// reg-name    = *( unreserved / pct-encoded / sub-delims )
static void QT_FASTCALL _regName(const char **ptr)
{
    for (;;) {
        if (!_unreserved(ptr) && !_subDelims(ptr)) {
            if (!_pctEncoded(ptr))
                break;
        }
    }
}

// host        = IP-literal / IPv4address / reg-name
static void QT_FASTCALL _host(const char **ptr, QUrlParseData *parseData)
{
    parseData->host = *ptr;
    if (!_IPLiteral(ptr)) {
        if (_IPv4Address(ptr)) {
            char ch = **ptr;
            if (ch && ch != ':' && ch != '/') {
                // reset
                *ptr = parseData->host;
                _regName(ptr);
            }
        } else {
            _regName(ptr);
        }
    }
    parseData->hostLength = *ptr - parseData->host;
}

// userinfo    = *( unreserved / pct-encoded / sub-delims / ":" )
static void QT_FASTCALL _userInfo(const char **ptr, QUrlParseData *parseData)
{
    parseData->userInfo = *ptr;
    for (;;) {
        if (_unreserved(ptr) || _subDelims(ptr)) {
            ;
        } else {
            if (_pctEncoded(ptr)) {
                ;
            } else if (**ptr == ':') {
                parseData->userInfoDelimIndex = *ptr - parseData->userInfo;
                ++(*ptr);
            } else {
                break;
            }
        }
    }
    if (**ptr != '@') {
        *ptr = parseData->userInfo;
        parseData->userInfoDelimIndex = -1;
        return;
    }
    parseData->userInfoLength = *ptr - parseData->userInfo;
    ++(*ptr);
}

// port        = *DIGIT
static void QT_FASTCALL _port(const char **ptr, int *port)
{
    bool first = true;

    for (;;) {
        const char *ptrBackup = *ptr;
        char ch = *((*ptr)++);
        if (ch < '0' || ch > '9') {
            *ptr = ptrBackup;
            break;
        }

        if (first) {
            first = false;
            *port = 0;
        }

        *port *= 10;
        *port += ch - '0';
    }
}

// authority   = [ userinfo "@" ] host [ ":" port ]
static void QT_FASTCALL _authority(const char **ptr, QUrlParseData *parseData)
{
    _userInfo(ptr, parseData);
    _host(ptr, parseData);

    if (**ptr != ':')
        return;

    ++(*ptr);
    _port(ptr, &parseData->port);
}

// pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
static bool QT_FASTCALL _pchar(const char **ptr)
{
    char c = *(*ptr);

    switch (c) {
    case '!': case '$': case '&': case '\'': case '(': case ')': case '*':
    case '+': case ',': case ';': case '=': case ':': case '@':
    case '-': case '.': case '_': case '~':
        ++(*ptr);
        return true;
    default:
        break;
    };

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
        ++(*ptr);
        return true;
    }

    if (_pctEncoded(ptr))
        return true;

    return false;
}

// segment       = *pchar
static bool QT_FASTCALL _segmentNZ(const char **ptr)
{
    if (!_pchar(ptr))
        return false;

    while(_pchar(ptr))
        ;

    return true;
}

// path-abempty  = *( "/" segment )
static void QT_FASTCALL _pathAbEmpty(const char **ptr)
{
    for (;;) {
        if (**ptr != '/')
            break;
        ++(*ptr);

        while (_pchar(ptr))
            ;
    }
}

// path-abs      = "/" [ segment-nz *( "/" segment ) ]
static bool QT_FASTCALL _pathAbs(const char **ptr)
{
    // **ptr == '/' already checked in caller
    ++(*ptr);

    // we might be able to unnest this to gain some performance.
    if (!_segmentNZ(ptr))
        return true;

    _pathAbEmpty(ptr);

    return true;
}

// path-rootless = segment-nz *( "/" segment )
static bool QT_FASTCALL _pathRootless(const char **ptr)
{
    // we might be able to unnest this to gain some performance.
    if (!_segmentNZ(ptr))
        return false;

    _pathAbEmpty(ptr);

    return true;
}


// hier-part   = "//" authority path-abempty
//             / path-abs
//             / path-rootless
//             / path-empty
static void QT_FASTCALL _hierPart(const char **ptr, QUrlParseData *parseData)
{
    const char *ptrBackup = *ptr;
    const char *pathStart = 0;
    if (*((*ptr)++) == '/' && *((*ptr)++) == '/') {
        _authority(ptr, parseData);
        pathStart = *ptr;
        _pathAbEmpty(ptr);
    } else {
        *ptr = ptrBackup;
        pathStart = *ptr;
        if (**ptr == '/')
            _pathAbs(ptr);
        else
            _pathRootless(ptr);
    }
    parseData->path = pathStart;
    parseData->pathLength = *ptr - pathStart;
}

// query       = *( pchar / "/" / "?" )
static void QT_FASTCALL _query(const char **ptr, QUrlParseData *parseData)
{
    parseData->query = *ptr;
    for (;;) {
        if (_pchar(ptr)) {
            ;
        } else if (**ptr == '/' || **ptr == '?') {
            ++(*ptr);
        } else {
            break;
        }
    }
    parseData->queryLength = *ptr - parseData->query;
}

// fragment    = *( pchar / "/" / "?" )
static void QT_FASTCALL _fragment(const char **ptr, QUrlParseData *parseData)
{
    parseData->fragment = *ptr;
    for (;;) {
        if (_pchar(ptr)) {
            ;
        } else if (**ptr == '/' || **ptr == '?' || **ptr == '#') {
            ++(*ptr);
        } else {
            break;
        }
    }
    parseData->fragmentLength = *ptr - parseData->fragment;
}

QUrlPrivate::QUrlPrivate()
    : ref(1),
    port(-1),
    parsingMode(QUrl::TolerantMode),
    hasQuery(false),
    hasFragment(false),
    isValid(false),
    isHostValid(true),
    valueDelimiter('='),
    pairDelimiter('&'),
    stateFlags(0)
{
}

// Called by normalized() and detach(). Must hold copy.mutex.
QUrlPrivate::QUrlPrivate(const QUrlPrivate &copy)
    : ref(1),
    scheme(copy.scheme),
    userName(copy.userName),
    password(copy.password),
    host(copy.host),
    path(copy.path),
    query(copy.query),
    fragment(copy.fragment),
    encodedOriginal(copy.encodedOriginal),
    encodedUserName(copy.encodedUserName),
    encodedPassword(copy.encodedPassword),
    encodedPath(copy.encodedPath),
    encodedFragment(copy.encodedFragment),
    port(copy.port),
    parsingMode(copy.parsingMode),
    hasQuery(copy.hasQuery),
    hasFragment(copy.hasFragment),
    isValid(copy.isValid),
    isHostValid(copy.isHostValid),
    valueDelimiter(copy.valueDelimiter),
    pairDelimiter(copy.pairDelimiter),
    stateFlags(copy.stateFlags),
    encodedNormalized(copy.encodedNormalized)
{
}

QString QUrlPrivate::canonicalHost() const
{
    // Caller must lock mutex first
    if (QURL_HASFLAG(stateFlags, HostCanonicalized) || host.isEmpty())
        return host;

    QUrlPrivate *that = const_cast<QUrlPrivate *>(this);
    QURL_SETFLAG(that->stateFlags, HostCanonicalized);
    if (host.contains(QLatin1Char(':'))) {
        // This is an IP Literal, use _IPLiteral to validate
        QByteArray ba = host.toLatin1();
        bool needsBraces = false;
        if (!ba.startsWith('[')) {
            // surround the IP Literal with [ ] if it's not already done so
            ba.reserve(ba.length() + 2);
            ba.prepend('[');
            ba.append(']');
            needsBraces = true;
        }

        const char *ptr = ba.constData();
        if (!_IPLiteral(&ptr))
            that->host.clear();
        else if (needsBraces)
            that->host = QString::fromLatin1(ba.toLower().constData());
        else
            that->host = host.toLower();
    } else {
        that->host = QUrl::fromAce(host.toUtf8());
    }
    that->isHostValid = !that->host.isNull();
    return that->host;
}

// From RFC 3896, Appendix A Collected ABNF for URI
//    authority     = [ userinfo "@" ] host [ ":" port ]
//    userinfo      = *( unreserved / pct-encoded / sub-delims / ":" )
//    host          = IP-literal / IPv4address / reg-name
//    port          = *DIGIT
//[...]
//    pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
//
//    query         = *( pchar / "/" / "?" )
//
//    fragment      = *( pchar / "/" / "?" )
//
//    pct-encoded   = "%" HEXDIG HEXDIG
//
//    unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
//    reserved      = gen-delims / sub-delims
//    gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
//    sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
//                  / "*" / "+" / "," / ";" / "="

// use defines for concatenation:
#define ABNF_sub_delims         "!$&'()*+,;="
#define ABNF_gen_delims         ":/?#[]@"
#define ABNF_pchar              ABNF_sub_delims ":@"
#define ABNF_reserved           ABNF_sub_delims ABNF_gen_delims

// list the characters that don't have to be converted according to the list above.
// "unreserved" is already automatically not encoded, so we don't have to list it.
// the path component has a complex ABNF that basically boils down to
// slash-separated segments of "pchar"

static const char userNameExcludeChars[] = ABNF_sub_delims;
static const char passwordExcludeChars[] = ABNF_sub_delims ":";
static const char pathExcludeChars[]     = ABNF_pchar "/";
static const char queryExcludeChars[]    = ABNF_pchar "/?";
static const char fragmentExcludeChars[] = ABNF_pchar "/?";

void QUrlPrivate::ensureEncodedParts() const
{
    QUrlPrivate *that = const_cast<QUrlPrivate *>(this);

    if (encodedUserName.isNull())
        // userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
        that->encodedUserName = toPercentEncodingHelper(userName, userNameExcludeChars);
    if (encodedPassword.isNull())
        // userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
        that->encodedPassword = toPercentEncodingHelper(password, passwordExcludeChars);
    if (encodedPath.isNull())
        // pchar = unreserved / pct-encoded / sub-delims / ":" / "@" ... also "/"
        that->encodedPath = toPercentEncodingHelper(path, pathExcludeChars);
    if (encodedFragment.isNull())
        // fragment      = *( pchar / "/" / "?" )
        that->encodedFragment = toPercentEncodingHelper(fragment, fragmentExcludeChars);
}

QString QUrlPrivate::authority(QUrl::FormattingOptions options) const
{
    // Caller must lock mutex first

    if ((options & QUrl::RemoveAuthority) == QUrl::RemoveAuthority)
        return QString();

    QString tmp = userInfo(options);
    if (!tmp.isEmpty())
        tmp += QLatin1Char('@');
    tmp += canonicalHost();
    if (!(options & QUrl::RemovePort) && port != -1)
        tmp += QLatin1Char(':') + QString::number(port);

    return tmp;
}

void QUrlPrivate::setAuthority(const QString &auth)
{
    isHostValid = true;
    if (auth.isEmpty()) {
        setUserInfo(QString());
        if (auth.isNull())
            host.clear();
        else
            host = QLatin1String("");
        port = -1;
        return;
    }

    // find the port section of the authority by searching from the
    // end towards the beginning for numbers until a ':' is reached.
    int portIndex = auth.length() - 1;
    if (portIndex == 0) {
        portIndex = -1;
    } else {
        short c = auth.at(portIndex--).unicode();
        if (c < '0' || c > '9') {
            portIndex = -1;
        } else while (portIndex >= 0) {
            c = auth.at(portIndex).unicode();
            if (c == ':') {
                break;
            } else if (c == '.') {
                portIndex = -1;
                break;
            }
            --portIndex;
        }
    }

    if (portIndex != -1) {
        port = 0;
        for (int i = portIndex + 1; i < auth.length(); ++i)
            port = (port * 10) + (auth.at(i).unicode() - '0');
    } else {
        port = -1;
    }

    int userInfoIndex = auth.indexOf(QLatin1Char('@'));
    if (userInfoIndex != -1 && (portIndex == -1 || userInfoIndex < portIndex))
        setUserInfo(auth.left(userInfoIndex));

    int hostIndex = 0;
    if (userInfoIndex != -1)
        hostIndex = userInfoIndex + 1;
    int hostLength = auth.length() - hostIndex;
    if (portIndex != -1)
        hostLength -= (auth.length() - portIndex);

    host = auth.mid(hostIndex, hostLength).trimmed();
}

void QUrlPrivate::setUserInfo(const QString &userInfo)
{
    encodedUserName.clear();
    encodedPassword.clear();

    int delimIndex = userInfo.indexOf(QLatin1Char(':'));
    if (delimIndex == -1) {
        userName = userInfo;
        password.clear();
        return;
    }
    userName = userInfo.left(delimIndex);
    password = userInfo.right(userInfo.length() - delimIndex - 1);
}

void QUrlPrivate::setEncodedUserInfo(const QUrlParseData *parseData)
{
    userName.clear();
    password.clear();
    if (!parseData->userInfoLength) {
        encodedUserName.clear();
        encodedPassword.clear();
    } else if (parseData->userInfoDelimIndex == -1) {
        encodedUserName = QByteArray(parseData->userInfo, parseData->userInfoLength);
        encodedPassword.clear();
    } else {
        encodedUserName = QByteArray(parseData->userInfo, parseData->userInfoDelimIndex);
        encodedPassword = QByteArray(parseData->userInfo + parseData->userInfoDelimIndex + 1,
                                     parseData->userInfoLength - parseData->userInfoDelimIndex - 1);
    }
}

QString QUrlPrivate::userInfo(QUrl::FormattingOptions options) const
{
    if ((options & QUrl::RemoveUserInfo) == QUrl::RemoveUserInfo)
        return QString();

    QUrlPrivate *that = const_cast<QUrlPrivate *>(this);
    if (userName.isNull())
        that->userName = fromPercentEncodingHelper(encodedUserName);
    if (password.isNull())
        that->password = fromPercentEncodingHelper(encodedPassword);

    QString tmp = userName;

    if (!(options & QUrl::RemovePassword) && !password.isEmpty()) {
        tmp += QLatin1Char(':');
        tmp += password;
    }

    return tmp;
}

/*
    From http://www.ietf.org/rfc/rfc3986.txt, 5.2.3: Merge paths

    Returns a merge of the current path with the relative path passed
    as argument.
*/
QByteArray QUrlPrivate::mergePaths(const QByteArray &relativePath) const
{
    // Caller must lock mutex first

    if (encodedPath.isNull())
        ensureEncodedParts();

    // If the base URI has a defined authority component and an empty
    // path, then return a string consisting of "/" concatenated with
    // the reference's path; otherwise,
    if (!authority().isEmpty() && encodedPath.isEmpty())
        return '/' + relativePath;

    // Return a string consisting of the reference's path component
    // appended to all but the last segment of the base URI's path
    // (i.e., excluding any characters after the right-most "/" in the
    // base URI path, or excluding the entire base URI path if it does
    // not contain any "/" characters).
    QByteArray newPath;
    if (!encodedPath.contains('/'))
        newPath = relativePath;
    else
        newPath = encodedPath.left(encodedPath.lastIndexOf('/') + 1) + relativePath;

    return newPath;
}

void QUrlPrivate::queryItem(int pos, int *value, int *end) const
{
    *end = query.indexOf(pairDelimiter, pos);
    if (*end == -1)
        *end = query.size();
    *value = pos;
    while (*value < *end) {
        if (query[*value] == valueDelimiter)
            break;
        ++*value;
    }
}

/*
    From http://www.ietf.org/rfc/rfc3986.txt, 5.2.4: Remove dot segments

    Removes unnecessary ../ and ./ from the path. Used for normalizing
    the URL.
*/
static void removeDotsFromPath(QByteArray *path)
{
    // The input buffer is initialized with the now-appended path
    // components and the output buffer is initialized to the empty
    // string.
    char *out = path->data();
    const char *in = out;
    const char *end = out + path->size();

    // If the input buffer consists only of
    // "." or "..", then remove that from the input
    // buffer;
    if (path->size() == 1 && in[0] == '.')
        ++in;
    else if (path->size() == 2 && in[0] == '.' && in[1] == '.')
        in += 2;
    // While the input buffer is not empty, loop:
    while (in < end) {

        // otherwise, if the input buffer begins with a prefix of "../" or "./",
        // then remove that prefix from the input buffer;
        if (path->size() >= 2 && in[0] == '.' && in[1] == '/')
            in += 2;
        else if (path->size() >= 3 && in[0] == '.' && in[1] == '.' && in[2] == '/')
            in += 3;

        // otherwise, if the input buffer begins with a prefix of
        // "/./" or "/.", where "." is a complete path segment,
        // then replace that prefix with "/" in the input buffer;
        if (in <= end - 3 && in[0] == '/' && in[1] == '.' && in[2] == '/') {
            in += 2;
            continue;
        } else if (in == end - 2 && in[0] == '/' && in[1] == '.') {
            *out++ = '/';
            in += 2;
            break;
        }

        // otherwise, if the input buffer begins with a prefix
        // of "/../" or "/..", where ".." is a complete path
        // segment, then replace that prefix with "/" in the
        // input buffer and remove the last //segment and its
        // preceding "/" (if any) from the output buffer;
        if (in <= end - 4 && in[0] == '/' && in[1] == '.' && in[2] == '.' && in[3] == '/') {
            while (out > path->constData() && *(--out) != '/')
                ;
            if (out == path->constData() && *out != '/')
                ++in;
            in += 3;
            continue;
        } else if (in == end - 3 && in[0] == '/' && in[1] == '.' && in[2] == '.') {
            while (out > path->constData() && *(--out) != '/')
                ;
            if (*out == '/')
                ++out;
            in += 3;
            break;
        }

        // otherwise move the first path segment in
        // the input buffer to the end of the output
        // buffer, including the initial "/" character
        // (if any) and any subsequent characters up
        // to, but not including, the next "/"
        // character or the end of the input buffer.
        *out++ = *in++;
        while (in < end && *in != '/')
            *out++ = *in++;
    }
    path->truncate(out - path->constData());
}

void QUrlPrivate::validate() const
{
    // Caller must lock mutex first
    QUrlPrivate *that = const_cast<QUrlPrivate *>(this);
    that->encodedOriginal = that->toEncoded(); // may detach
    parse(ParseOnly);

    QURL_SETFLAG(that->stateFlags, Validated);

    if (!isValid)
        return;

    QString auth = authority(); // causes the non-encoded forms to be valid

    // authority() calls canonicalHost() which sets this
    if (!isHostValid)
        return;

    if (scheme == QLatin1String("mailto")) {
        if (!host.isEmpty() || port != -1 || !userName.isEmpty() || !password.isEmpty()) {
            that->isValid = false;
            that->errorInfo.setParams(0, QT_TRANSLATE_NOOP(QUrl, "expected empty host, username,"
                                                           "port and password"),
                                      0);
        }
    } else if (scheme == QLatin1String("ftp") || scheme == QLatin1String("http")) {
        if (host.isEmpty() && !(path.isEmpty() && encodedPath.isEmpty())) {
            that->isValid = false;
            that->errorInfo.setParams(0, QT_TRANSLATE_NOOP(QUrl, "the host is empty, but not the path"),
                                      0);
        }
    }
}

void QUrlPrivate::parse(ParseOptions parseOptions) const
{
    // Caller must lock mutex first
    QUrlPrivate *that = const_cast<QUrlPrivate *>(this);
    that->errorInfo.setParams(0, 0, 0);
    if (encodedOriginal.isEmpty()) {
        that->isValid = false;
        that->errorInfo.setParams(0, QT_TRANSLATE_NOOP(QUrl, "empty"),
                                  0);
        QURL_SETFLAG(that->stateFlags, Validated | Parsed);
        return;
    }


    QUrlParseData parseData;
    memset(&parseData, 0, sizeof(parseData));
    parseData.userInfoDelimIndex = -1;
    parseData.port = -1;

    const char *pptr = (char *) encodedOriginal.constData();
    const char **ptr = &pptr;

#if defined (QURL_DEBUG)
    qDebug("QUrlPrivate::parse(), parsing \"%s\"", pptr);
#endif

    // optional scheme
    bool isSchemeValid = _scheme(ptr, &parseData);

    if (isSchemeValid == false) {
        that->isValid = false;
        char ch = *((*ptr)++);
        that->errorInfo.setParams(*ptr, QT_TRANSLATE_NOOP(QUrl, "unexpected URL scheme"),
                                  ch);
        QURL_SETFLAG(that->stateFlags, Validated | Parsed);
#if defined (QURL_DEBUG)
        qDebug("QUrlPrivate::parse(), unrecognized: %c%s", ch, *ptr);
#endif
        return;
    }

    // hierpart
    _hierPart(ptr, &parseData);

    // optional query
    char ch = *((*ptr)++);
    if (ch == '?') {
        that->hasQuery = true;
        _query(ptr, &parseData);
        ch = *((*ptr)++);
    }

    // optional fragment
    if (ch == '#') {
        that->hasFragment = true;
        _fragment(ptr, &parseData);
    } else if (ch != '\0') {
        that->isValid = false;
        that->errorInfo.setParams(*ptr, QT_TRANSLATE_NOOP(QUrl, "expected end of URL"),
                                  ch);
        QURL_SETFLAG(that->stateFlags, Validated | Parsed);
#if defined (QURL_DEBUG)
        qDebug("QUrlPrivate::parse(), unrecognized: %c%s", ch, *ptr);
#endif
        return;
    }

    // when doing lazy validation, this function is called after
    // encodedOriginal has been constructed from the individual parts,
    // only to see if the constructed URL can be parsed. in that case,
    // parse() is called in ParseOnly mode; we don't want to set all
    // the members over again.
    if (parseOptions == ParseAndSet) {
        QURL_UNSETFLAG(that->stateFlags, HostCanonicalized);

        if (parseData.scheme) {
            QByteArray s(parseData.scheme, parseData.schemeLength);
            that->scheme = fromPercentEncodingMutable(&s);
        }

        that->setEncodedUserInfo(&parseData);

        QByteArray h(parseData.host, parseData.hostLength);
        that->host = fromPercentEncodingMutable(&h);
        that->port = uint(parseData.port) <= 0xffffU ? parseData.port : -1;

        that->path.clear();
        that->encodedPath = QByteArray(parseData.path, parseData.pathLength);

        if (that->hasQuery)
            that->query = QByteArray(parseData.query, parseData.queryLength);
        else
            that->query.clear();

        that->fragment.clear();
        if (that->hasFragment) {
            that->encodedFragment = QByteArray(parseData.fragment, parseData.fragmentLength);
        } else {
            that->encodedFragment.clear();
        }
    }

    that->isValid = true;
    QURL_SETFLAG(that->stateFlags, Parsed);

#if defined (QURL_DEBUG)
    qDebug("QUrl::setUrl(), scheme = %s", that->scheme.toLatin1().constData());
    qDebug("QUrl::setUrl(), userInfo = %s", that->userInfo.toLatin1().constData());
    qDebug("QUrl::setUrl(), host = %s", that->host.toLatin1().constData());
    qDebug("QUrl::setUrl(), port = %i", that->port);
    qDebug("QUrl::setUrl(), path = %s", fromPercentEncodingHelper(__path).toLatin1().constData());
    qDebug("QUrl::setUrl(), query = %s", __query.constData());
    qDebug("QUrl::setUrl(), fragment = %s", fromPercentEncodingHelper(__fragment).toLatin1().constData());
#endif
}

void QUrlPrivate::clear()
{
    scheme.clear();
    userName.clear();
    password.clear();
    host.clear();
    port = -1;
    path.clear();
    query.clear();
    fragment.clear();

    encodedOriginal.clear();
    encodedUserName.clear();
    encodedPassword.clear();
    encodedPath.clear();
    encodedFragment.clear();
    encodedNormalized.clear();

    isValid = false;
    hasQuery = false;
    hasFragment = false;

    valueDelimiter = '=';
    pairDelimiter = '&';

    QURL_UNSETFLAG(stateFlags, Parsed | Validated | Normalized | HostCanonicalized);
}

QByteArray QUrlPrivate::toEncoded(QUrl::FormattingOptions options) const
{
    // Caller must lock mutex first
    if (!QURL_HASFLAG(stateFlags, Parsed)) parse();
    else ensureEncodedParts();

    if (options==0x100) // private - see qHash(QUrl)
        return normalized();

    QByteArray url;

    if (!(options & QUrl::RemoveScheme) && !scheme.isEmpty()) {
        url += scheme.toLatin1();
        url += ':';
    }
    QString savedHost = host;  // pre-validation, may be invalid!
    QString auth = authority();
    bool doFileScheme = scheme == QLatin1String("file") && encodedPath.startsWith('/');
    if ((options & QUrl::RemoveAuthority) != QUrl::RemoveAuthority && (!auth.isNull() || doFileScheme || !savedHost.isEmpty())) {
        if (doFileScheme && !encodedPath.startsWith('/'))
            url += '/';
        url += "//";

        if ((options & QUrl::RemoveUserInfo) != QUrl::RemoveUserInfo) {
            bool hasUserOrPass = false;
            if (!userName.isEmpty()) {
                url += encodedUserName;
                hasUserOrPass = true;
            }
            if (!(options & QUrl::RemovePassword) && !password.isEmpty()) {
                url += ':';
                url += encodedPassword;
                hasUserOrPass = true;
            }
            if (hasUserOrPass)
                url += '@';
        }

        if (host.startsWith(QLatin1Char('['))) {
            url += host.toLatin1();
        } else if (host.contains(QLatin1Char(':'))) {
            url += '[';
            url += host.toLatin1();
            url += ']';
        } else if (host.isEmpty() && !savedHost.isEmpty()) {
            // this case is only possible with an invalid URL
            // it's here only so that we can keep the original, invalid hostname
            // in encodedOriginal.
            // QUrl::isValid() will return false, so toEncoded() can be anything (it's not valid)
            url += savedHost.toUtf8();
        } else {
            url += QUrl::toAce(host);
        }
        if (!(options & QUrl::RemovePort) && port != -1) {
            url += ':';
            url += QString::number(port).toAscii();
        }
    }

    if (!(options & QUrl::RemovePath)) {
        // check if we need to insert a slash
        if (!encodedPath.isEmpty() && !auth.isEmpty()) {
            if (!encodedPath.startsWith('/'))
                url += '/';
        }
        url += encodedPath;

        // check if we need to remove trailing slashes
        while ((options & QUrl::StripTrailingSlash) && url.endsWith('/'))
            url.chop(1);
    }

    if (!(options & QUrl::RemoveQuery) && hasQuery) {
        url += '?';
        url += query;
    }
    if (!(options & QUrl::RemoveFragment) && hasFragment) {
        url += '#';
        url += encodedFragment;
    }

    return url;
}

const QByteArray &QUrlPrivate::normalized() const
{
    // Caller must lock mutex first
    if (QURL_HASFLAG(stateFlags, QUrlPrivate::Normalized))
        return encodedNormalized;

    QUrlPrivate *that = const_cast<QUrlPrivate *>(this);
    QURL_SETFLAG(that->stateFlags, QUrlPrivate::Normalized);

    QUrlPrivate tmp = *this;
    tmp.scheme = tmp.scheme.toLower();
    tmp.host = tmp.canonicalHost();

    // ensure the encoded and normalized parts of the URL
    tmp.ensureEncodedParts();
    if (tmp.encodedUserName.contains('%'))
        q_normalizePercentEncoding(&tmp.encodedUserName, userNameExcludeChars);
    if (tmp.encodedPassword.contains('%'))
        q_normalizePercentEncoding(&tmp.encodedPassword, passwordExcludeChars);
    if (tmp.encodedFragment.contains('%'))
        q_normalizePercentEncoding(&tmp.encodedFragment, fragmentExcludeChars);

    if (tmp.encodedPath.contains('%')) {
        // the path is a bit special:
        // the slashes shouldn't be encoded or decoded.
        // They should remain exactly like they are right now
        //
        // treat the path as a slash-separated sequence of pchar
        QByteArray result;
        result.reserve(tmp.encodedPath.length());
        if (tmp.encodedPath.startsWith('/'))
            result.append('/');

        const char *data = tmp.encodedPath.constData();
        int lastSlash = 0;
        int nextSlash;
        do {
            ++lastSlash;
            nextSlash = tmp.encodedPath.indexOf('/', lastSlash);
            int len;
            if (nextSlash == -1)
                len = tmp.encodedPath.length() - lastSlash;
            else
                len = nextSlash - lastSlash;

            if (memchr(data + lastSlash, '%', len)) {
                // there's at least one percent before the next slash
                QByteArray block = QByteArray(data + lastSlash, len);
                q_normalizePercentEncoding(&block, pathExcludeChars);
                result.append(block);
            } else {
                // no percents in this path segment, append wholesale
                result.append(data + lastSlash, len);
            }

            // append the slash too, if it's there
            if (nextSlash != -1)
                result.append('/');

            lastSlash = nextSlash;
        } while (lastSlash != -1);

        tmp.encodedPath = result;
    }

    if (!tmp.scheme.isEmpty()) // relative test
        removeDotsFromPath(&tmp.encodedPath);

    int qLen = tmp.query.length();
    for (int i = 0; i < qLen; i++) {
        if (qLen - i > 2 && tmp.query.at(i) == '%') {
            ++i;
            tmp.query[i] = qToLower(tmp.query.at(i));
            ++i;
            tmp.query[i] = qToLower(tmp.query.at(i));
        }
    }
    encodedNormalized = tmp.toEncoded();

    return encodedNormalized;
}

QString QUrlPrivate::createErrorString() const
{
    if (isValid && isHostValid)
        return QString();

    QString errorString(QLatin1String(QT_TRANSLATE_NOOP(QUrl, "Invalid URL \"")));
    errorString += QLatin1String(encodedOriginal.constData());
    errorString += QLatin1String(QT_TRANSLATE_NOOP(QUrl, "\""));

    if (errorInfo._source) {
        int position = encodedOriginal.indexOf(errorInfo._source) - 1;
        if (position > 0) {
            errorString += QLatin1String(QT_TRANSLATE_NOOP(QUrl, ": error at position "));
            errorString += QString::number(position);
        } else {
            errorString += QLatin1String(QT_TRANSLATE_NOOP(QUrl, ": "));
            errorString += QLatin1String(errorInfo._source);
        }
    }

    errorString += QLatin1String(QT_TRANSLATE_NOOP(QUrl, ": "));
    if (isHostValid)
        errorString += QLatin1String(errorInfo._message);
    else
        errorString += QLatin1String(QT_TRANSLATE_NOOP(QUrl, "invalid hostname"));
    if (errorInfo._found) {
        errorString += QLatin1String(QT_TRANSLATE_NOOP(QUrl, ", but found \'"));
        errorString += QLatin1Char(errorInfo._found);
        errorString += QLatin1String(QT_TRANSLATE_NOOP(QUrl, "\'"));
    }
    return errorString;
}

/*!
    \macro QT_NO_URL_CAST_FROM_STRING
    \relates QUrl

    Disables automatic conversions from QString (or char *) to QUrl.

    Compiling your code with this define is useful when you have a lot of
    code that uses QString for file names and you wish to convert it to
    use QUrl for network transparency. In any code that uses QUrl, it can
    help avoid missing QUrl::resolved() calls, and other misuses of
    QString to QUrl conversions.

    \oldcode
        url = filename; // probably not what you want
    \newcode
        url = QUrl::fromLocalFile(filename);
        url = baseurl.resolved(QUrl(filename));
    \endcode

    \sa QT_NO_CAST_FROM_ASCII
*/


/*!
    Constructs a URL by parsing \a url. \a url is assumed to be in human
    readable representation, with no percent encoding. QUrl will automatically
    percent encode all characters that are not allowed in a URL.
    The default parsing mode is TolerantMode.

    Example:

    \snippet doc/src/snippets/code/src_corelib_io_qurl.cpp 0

    To construct a URL from an encoded string, call fromEncoded():

    \snippet doc/src/snippets/code/src_corelib_io_qurl.cpp 1

    \sa setUrl(), setEncodedUrl(), fromEncoded(), TolerantMode
*/
QUrl::QUrl(const QString &url, ParsingMode parsingMode) : d(0)
{
    if (!url.isEmpty())
        setUrl(url, parsingMode);
    else {
        d = new QUrlPrivate;
        d->parsingMode = parsingMode;
    }
}

/*!
    Constructs an empty QUrl object.
*/
QUrl::QUrl() : d(0)
{
}

/*!
    Constructs a copy of \a other.
*/
QUrl::QUrl(const QUrl &other) : d(other.d)
{
    if (d)
        d->ref.ref();
}

/*!
    Destructor; called immediately before the object is deleted.
*/
QUrl::~QUrl()
{
    if (d && !d->ref.deref())
        delete d;
}

/*!
    Returns true if the URL is valid; otherwise returns false.

    The URL is run through a conformance test. Every part of the URL
    must conform to the standard encoding rules of the URI standard
    for the URL to be reported as valid.

    \snippet doc/src/snippets/code/src_corelib_io_qurl.cpp 2
*/
bool QUrl::isValid() const
{
    if (!d) return false;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Validated)) d->validate();

    return d->isValid && d->isHostValid;
}

/*!
    Returns true if the URL has no data; otherwise returns false.
*/
bool QUrl::isEmpty() const
{
    if (!d) return true;

    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed))
        return d->encodedOriginal.isEmpty();
    else
        return d->scheme.isEmpty()   // no encodedScheme
        && d->userName.isEmpty() && d->encodedUserName.isEmpty()
        && d->password.isEmpty() && d->encodedPassword.isEmpty()
        && d->host.isEmpty()   // no encodedHost
        && d->port == -1
        && d->path.isEmpty() && d->encodedPath.isEmpty()
        && d->query.isEmpty()
        && d->fragment.isEmpty() && d->encodedFragment.isEmpty();
}

/*!
    Resets the content of the QUrl. After calling this function, the
    QUrl is equal to one that has been constructed with the default
    empty constructor.
*/
void QUrl::clear()
{
    if (d && !d->ref.deref())
        delete d;
    d = 0;
}

/*!
    Constructs a URL by parsing the contents of \a url.

    \a url is assumed to be in unicode format, with no percent
    encoding.

    Calling isValid() will tell whether or not a valid URL was
    constructed.

    \sa setEncodedUrl()
*/
void QUrl::setUrl(const QString &url, ParsingMode parsingMode)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    detach(lock);
    d->clear();

    // escape all reserved characters and delimiters
    // reserved      = gen-delims / sub-delims
    if (parsingMode != TolerantMode) {
        d->setEncodedUrl(toPercentEncodingHelper(url, ABNF_reserved), parsingMode);
        return;
    }

    // Tolerant preprocessing
    QString tmp = url;

    // Allow %20 in the QString variant
    tmp.replace(QLatin1String("%20"), QLatin1String(" "));

    // Percent-encode unsafe ASCII characters after host part
    int start = tmp.indexOf(QLatin1String("//"));
    if (start != -1) {
        // Has host part, find delimiter
        start += 2; // skip "//"
        const char delims[] = "/#?";
        const char *delimster = delims;
        int hostEnd = -1;
        while (*delimster && (hostEnd = tmp.indexOf(QLatin1Char(*delimster), start)) == -1)
            ++delimster;
        start = (hostEnd == -1) ? -1 : hostEnd + 1;
    } else {
        start = 0; // Has no host part
    }
    QByteArray encodedUrl;
    if (start != -1) {
        QString hostPart = tmp.left(start);
        QString otherPart = tmp.mid(start);
        encodedUrl = toPercentEncodingHelper(hostPart, ":/?#[]@!$&'()*+,;=")
                   + toPercentEncodingHelper(otherPart, ":/?#@!$&'()*+,;=");
    } else {
        encodedUrl = toPercentEncodingHelper(tmp, ABNF_reserved);
    }
    d->setEncodedUrl(encodedUrl, StrictMode);
}

inline static bool isHex(char c)
{
    c |= 0x20;
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

static inline char toHex(quint8 c)
{
    return c > 9 ? c - 10 + 'A' : c + '0';
}

/*!
    Constructs a URL by parsing the contents of \a encodedUrl using
    the given \a parsingMode.

    \a encodedUrl is assumed to be a URL string in percent encoded
    form, containing only ASCII characters.

    Use isValid() to determine if a valid URL was constructed.

    \sa setUrl()
*/
void QUrl::setEncodedUrl(const QByteArray &encodedUrl, ParsingMode parsingMode)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    detach(lock);

    d->clear();
    d->setEncodedUrl(encodedUrl, parsingMode);
}

void QUrlPrivate::setEncodedUrl(const QByteArray &encodedUrl, QUrl::ParsingMode parsing)
{
    // Caller must lock mutex first
    QByteArray tmp = encodedUrl;
    if ((parsingMode = parsing) == QUrl::TolerantMode) {
        // Replace stray % with %25
        QByteArray copy = tmp;
        for (int i = 0, j = 0; i < copy.size(); ++i, ++j) {
            if (copy.at(i) == '%') {
                if (i + 2 >= copy.size() || !isHex(copy.at(i + 1)) || !isHex(copy.at(i + 2))) {
                    tmp.replace(j, 1, "%25");
                    j += 2;
                }
            }
        }

        // Find the host part
        int hostStart = tmp.indexOf("//");
        int hostEnd = -1;
        if (hostStart != -1) {
            // Has host part, find delimiter
            hostStart += 2; // skip "//"
            hostEnd = tmp.indexOf('/', hostStart);
            if (hostEnd == -1)
                hostEnd = tmp.indexOf('#', hostStart);
            if (hostEnd == -1)
                hostEnd = tmp.indexOf('?');
            if (hostEnd == -1)
                hostEnd = tmp.length() - 1;
        }

        // Reserved and unreserved characters are fine
//         unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
//         reserved      = gen-delims / sub-delims
//         gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
//         sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
//                         / "*" / "+" / "," / ";" / "="
        // Replace everything else with percent encoding
        static const char doEncode[] = " \"<>[\\]^`{|}";
        static const char doEncodeHost[] = " \"<>\\^`{|}";
        for (int i = 0; i < tmp.size(); ++i) {
            quint8 c = quint8(tmp.at(i));
            if (c < 32 || c > 127 ||
                strchr(hostStart <= i && i <= hostEnd ? doEncodeHost : doEncode, c)) {
                const char buf[4] = { '%', toHex(c >> 4), toHex(c & 0xf), '\0' };
                tmp.replace(i, 1, buf);
                i += 2;
            }
        }
    }

    encodedOriginal = tmp;
}

/*!
    Sets the scheme of the URL to \a scheme. As a scheme can only
    contain ASCII characters, no conversion or encoding is done on the
    input.

    The scheme describes the type (or protocol) of the URL. It's
    represented by one or more ASCII characters at the start the URL,
    and is followed by a ':'. The following example shows a URL where
    the scheme is "ftp":

    \img qurl-authority2.png

    The scheme can also be empty, in which case the URL is interpreted
    as relative.

    \sa scheme(), isRelative()
*/
void QUrl::setScheme(const QString &scheme)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    d->scheme = scheme;
}

/*!
    Returns the scheme of the URL. If an empty string is returned,
    this means the scheme is undefined and the URL is then relative.

    \sa setScheme(), isRelative()
*/
QString QUrl::scheme() const
{
    if (!d) return QString();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    return d->scheme;
}

/*!
    Sets the authority of the URL to \a authority.

    The authority of a URL is the combination of user info, a host
    name and a port. All of these elements are optional; an empty
    authority is therefore valid.

    The user info and host are separated by a '@', and the host and
    port are separated by a ':'. If the user info is empty, the '@'
    must be omitted; although a stray ':' is permitted if the port is
    empty.

    The following example shows a valid authority string:

    \img qurl-authority.png
*/
void QUrl::setAuthority(const QString &authority)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized | QUrlPrivate::HostCanonicalized);

    d->setAuthority(authority);
}

/*!
    Returns the authority of the URL if it is defined; otherwise
    an empty string is returned.

    \sa setAuthority()
*/
QString QUrl::authority() const
{
    if (!d) return QString();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    return d->authority();
}

/*!
    Sets the user info of the URL to \a userInfo. The user info is an
    optional part of the authority of the URL, as described in
    setAuthority().

    The user info consists of a user name and optionally a password,
    separated by a ':'. If the password is empty, the colon must be
    omitted. The following example shows a valid user info string:

    \img qurl-authority3.png

    \sa userInfo(), setUserName(), setPassword(), setAuthority()
*/
void QUrl::setUserInfo(const QString &userInfo)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    d->setUserInfo(userInfo.trimmed());
}

/*!
    Returns the user info of the URL, or an empty string if the user
    info is undefined.
*/
QString QUrl::userInfo() const
{
    if (!d) return QString();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    return d->userInfo();
}

/*!
    Sets the URL's user name to \a userName. The \a userName is part
    of the user info element in the authority of the URL, as described
    in setUserInfo().

    \sa setEncodedUserName(), userName(), setUserInfo()
*/
void QUrl::setUserName(const QString &userName)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    d->userName = userName;
    d->encodedUserName.clear();
}

/*!
    Returns the user name of the URL if it is defined; otherwise
    an empty string is returned.

    \sa setUserName(), encodedUserName()
*/
QString QUrl::userName() const
{
    if (!d) return QString();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    d->userInfo();              // causes the unencoded form to be set
    return d->userName;
}

/*!
    \since 4.4

    Sets the URL's user name to the percent-encoded \a userName. The \a
    userName is part of the user info element in the authority of the
    URL, as described in setUserInfo().

    Note: this function does not verify that \a userName is properly
    encoded. It is the caller's responsibility to ensure that the any
    delimiters (such as colons or slashes) are properly encoded.

    \sa setUserName(), encodedUserName(), setUserInfo()
*/
void QUrl::setEncodedUserName(const QByteArray &userName)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    d->encodedUserName = userName;
    d->userName.clear();
}

/*!
    \since 4.4

    Returns the user name of the URL if it is defined; otherwise
    an empty string is returned. The returned value will have its
    non-ASCII and other control characters percent-encoded, as in
    toEncoded().

    \sa setEncodedUserName()
*/
QByteArray QUrl::encodedUserName() const
{
    if (!d) return QByteArray();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    d->ensureEncodedParts();
    return d->encodedUserName;
}

/*!
    Sets the URL's password to \a password. The \a password is part of
    the user info element in the authority of the URL, as described in
    setUserInfo().

    \sa password(), setUserInfo()
*/
void QUrl::setPassword(const QString &password)
{
    if (!d) d = new QUrlPrivate;
    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    d->password = password;
    d->encodedPassword.clear();
}

/*!
    Returns the password of the URL if it is defined; otherwise
    an empty string is returned.

    \sa setPassword()
*/
QString QUrl::password() const
{
    if (!d) return QString();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    d->userInfo();              // causes the unencoded form to be set
    return d->password;
}

/*!
    \since 4.4

    Sets the URL's password to the percent-encoded \a password. The \a
    password is part of the user info element in the authority of the
    URL, as described in setUserInfo().

    Note: this function does not verify that \a password is properly
    encoded. It is the caller's responsibility to ensure that the any
    delimiters (such as colons or slashes) are properly encoded.

    \sa setPassword(), encodedPassword(), setUserInfo()
*/
void QUrl::setEncodedPassword(const QByteArray &password)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    d->encodedPassword = password;
    d->password.clear();
}

/*!
    \since 4.4

    Returns the password of the URL if it is defined; otherwise an
    empty string is returned. The returned value will have its
    non-ASCII and other control characters percent-encoded, as in
    toEncoded().

    \sa setEncodedPassword(), toEncoded()
*/
QByteArray QUrl::encodedPassword() const
{
    if (!d) return QByteArray();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    d->ensureEncodedParts();
    return d->encodedPassword;
}

/*!
    Sets the host of the URL to \a host. The host is part of the
    authority.

    \sa host(), setAuthority()
*/
void QUrl::setHost(const QString &host)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    d->isHostValid = true;
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized | QUrlPrivate::HostCanonicalized);

    d->host = host;
}

/*!
    Returns the host of the URL if it is defined; otherwise
    an empty string is returned.
*/
QString QUrl::host() const
{
    if (!d) return QString();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    QString result = d->canonicalHost();
    if (result.startsWith(QLatin1Char('[')))
        return result.mid(1, result.length() - 2);
    return result;
}

/*!
    \since 4.4

    Sets the URL's host to the ACE- or percent-encoded \a host. The \a
    host is part of the user info element in the authority of the
    URL, as described in setAuthority().

    \sa setHost(), encodedHost(), setAuthority(), fromAce()
*/
void QUrl::setEncodedHost(const QByteArray &host)
{
    setHost(fromPercentEncodingHelper(host));
}

/*!
    \since 4.4

    Returns the host part of the URL if it is defined; otherwise
    an empty string is returned.

    Note: encodedHost() does not return percent-encoded hostnames. Instead,
    the ACE-encoded (bare ASCII in Punycode encoding) form will be
    returned for any non-ASCII hostname.

    This function is equivalent to calling QUrl::toAce() on the return
    value of host().

    \sa setEncodedHost()
*/
QByteArray QUrl::encodedHost() const
{
    // should we cache this in d->encodedHost?
    return QUrl::toAce(host());
}

/*!
    Sets the port of the URL to \a port. The port is part of the
    authority of the URL, as described in setAuthority().

    \a port must be between 0 and 65535 inclusive. Setting the
    port to -1 indicates that the port is unspecified.
*/
void QUrl::setPort(int port)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    if (Q_UNLIKELY(port < -1 || port > 65535)) {
        qWarning("QUrl::setPort: Out of range");
        port = -1;
    }

    d->port = port;
}

/*!
    Returns the port of the URL, or \a defaultPort if the port is
    unspecified.

    Example:

    \snippet doc/src/snippets/code/src_corelib_io_qurl.cpp 3
*/
int QUrl::port(int defaultPort) const
{
    if (!d) return defaultPort;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    return d->port == -1 ? defaultPort : d->port;
}

/*!
    Sets the path of the URL to \a path. The path is the part of the
    URL that comes after the authority but before the query string.

    \img qurl-ftppath.png

    For non-hierarchical schemes, the path will be everything
    following the scheme declaration, as in the following example:

    \img qurl-mailtopath.png

    \sa path()
*/
void QUrl::setPath(const QString &path)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    d->path = path;
    d->encodedPath.clear();
}

/*!
    Returns the path of the URL.

    \sa setPath()
*/
QString QUrl::path() const
{
    if (!d) return QString();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    if (d->path.isNull()) {
        QUrlPrivate *that = const_cast<QUrlPrivate *>(d);
        that->path = fromPercentEncodingHelper(d->encodedPath);
    }
    return d->path;
}

/*!
    \since 4.4

    Sets the URL's path to the percent-encoded \a path.  The path is
    the part of the URL that comes after the authority but before the
    query string.

    \img qurl-ftppath.png

    For non-hierarchical schemes, the path will be everything
    following the scheme declaration, as in the following example:

    \img qurl-mailtopath.png

    Note: this function does not verify that \a path is properly
    encoded. It is the caller's responsibility to ensure that the any
    delimiters (such as '?' and '#') are properly encoded.

    \sa setPath(), encodedPath(), setUserInfo()
*/
void QUrl::setEncodedPath(const QByteArray &path)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    d->encodedPath = path;
    d->path.clear();
}

/*!
    \since 4.4

    Returns the path of the URL if it is defined; otherwise an
    empty string is returned. The returned value will have its
    non-ASCII and other control characters percent-encoded, as in
    toEncoded().

    \sa setEncodedPath(), toEncoded()
*/
QByteArray QUrl::encodedPath() const
{
    if (!d) return QByteArray();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    d->ensureEncodedParts();
    return d->encodedPath;
}

/*!
    \since 4.2

    Returns true if this URL contains a Query (i.e., if ? was seen on it).

    \sa hasQueryItem(), encodedQuery()
*/
bool QUrl::hasQuery() const
{
    if (!d) return false;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    return d->hasQuery;
}

/*!
    Sets the characters used for delimiting between keys and values,
    and between key-value pairs in the URL's query string. The default
    value delimiter is '=' and the default pair delimiter is '&'.

    \img qurl-querystring.png

    \a valueDelimiter will be used for separating keys from values,
    and \a pairDelimiter will be used to separate key-value pairs.
    Any occurrences of these delimiting characters in the encoded
    representation of the keys and values of the query string are
    percent encoded.

    If \a valueDelimiter is set to '-' and \a pairDelimiter is '/',
    the above query string would instead be represented like this:

    \snippet doc/src/snippets/code/src_corelib_io_qurl.cpp 4

    Calling this function does not change the delimiters of the
    current query string. It only affects queryItems(),
    setQueryItems() and addQueryItems().
*/
void QUrl::setQueryDelimiters(char valueDelimiter, char pairDelimiter)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    detach(lock);

    d->valueDelimiter = valueDelimiter;
    d->pairDelimiter = pairDelimiter;
}

/*!
    Returns the character used to delimit between key-value pairs in
    the query string of the URL.
*/
char QUrl::queryPairDelimiter() const
{
    if (!d) return '&';
    return d->pairDelimiter;
}

/*!
    Returns the character used to delimit between keys and values in
    the query string of the URL.
*/
char QUrl::queryValueDelimiter() const
{
    if (!d) return '=';
    return d->valueDelimiter;
}

/*!
    Sets the query string of the URL to \a query. The string is
    inserted as-is, and no further encoding is performed when calling
    toEncoded().

    This function is useful if you need to pass a query string that
    does not fit into the key-value pattern, or that uses a different
    scheme for encoding special characters than what is suggested by
    QUrl.

    Passing a value of QByteArray() to \a query (a null QByteArray) unsets
    the query completely. However, passing a value of QByteArray("")
    will set the query to an empty value, as if the original URL
    had a lone "?".

    \sa encodedQuery(), hasQuery()
*/
void QUrl::setEncodedQuery(const QByteArray &query)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    d->query = query;
    d->hasQuery = !query.isNull();
}

/*!
    Sets the query string of the URL to an encoded version of \a
    query. The contents of \a query are converted to a string
    internally, each pair delimited by the character returned by
    pairDelimiter(), and the key and value are delimited by
    valueDelimiter().

    \note This method does not encode spaces (ASCII 0x20) as plus (+) signs,
    like HTML forms do. If you need that kind of encoding, you must encode
    the value yourself and use QUrl::setEncodedQueryItems.

    \sa setQueryDelimiters(), queryItems(), setEncodedQueryItems()
*/
void QUrl::setQueryItems(const QList<QPair<QString, QString> > &query)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);

    const char alsoEncode[3] = { d->valueDelimiter , d->pairDelimiter, 0 };
    QByteArray queryTmp;
    for (int i = 0; i < query.size(); i++) {
        if (i) queryTmp += d->pairDelimiter;
        // query = *( pchar / "/" / "?" )
        queryTmp += toPercentEncodingHelper(query.at(i).first, queryExcludeChars, alsoEncode);
        queryTmp += d->valueDelimiter;
        // query = *( pchar / "/" / "?" )
        queryTmp += toPercentEncodingHelper(query.at(i).second, queryExcludeChars, alsoEncode);
    }

    d->query = queryTmp;
    d->hasQuery = !query.isEmpty();
}

/*!
    \since 4.4

    Sets the query string of the URL to the encoded version of \a
    query. The contents of \a query are converted to a string
    internally, each pair delimited by the character returned by
    pairDelimiter(), and the key and value are delimited by
    valueDelimiter().

    Note: this function does not verify that the key-value pairs
    are properly encoded. It is the caller's responsibility to ensure
    that the query delimiters are properly encoded, if any.

    \sa setQueryDelimiters(), encodedQueryItems(), setQueryItems()
*/
void QUrl::setEncodedQueryItems(const QList<QPair<QByteArray, QByteArray> > &query)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);

    QByteArray queryTmp;
    for (int i = 0; i < query.size(); i++) {
        if (i) queryTmp += d->pairDelimiter;
        queryTmp += query.at(i).first;
        queryTmp += d->valueDelimiter;
        queryTmp += query.at(i).second;
    }

    d->query = queryTmp;
    d->hasQuery = !query.isEmpty();
}

/*!
    Inserts the pair \a key = \a value into the query string of the
    URL.

    The key/value pair is encoded before it is added to the query. The
    pair is converted into separate strings internally. The \a key and
    \a value is first encoded into UTF-8 and then delimited by the
    character returned by valueDelimiter(). Each key/value pair is
    delimited by the character returned by pairDelimiter().

    \note This method does not encode spaces (ASCII 0x20) as plus (+) signs,
    like HTML forms do. If you need that kind of encoding, you must encode
    the value yourself and use QUrl::addEncodedQueryItem.

    \sa addEncodedQueryItem()
*/
void QUrl::addQueryItem(const QString &key, const QString &value)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);

    const char alsoEncode[3] = { d->valueDelimiter , d->pairDelimiter, 0 };

    if (!d->query.isEmpty())
        d->query += d->pairDelimiter;

    // query = *( pchar / "/" / "?" )
    d->query += toPercentEncodingHelper(key, queryExcludeChars, alsoEncode);
    d->query += d->valueDelimiter;
    // query = *( pchar / "/" / "?" )
    d->query += toPercentEncodingHelper(value, queryExcludeChars, alsoEncode);

    d->hasQuery = !d->query.isEmpty();
}

/*!
    \since 4.4

    Inserts the pair \a key = \a value into the query string of the
    URL.

    Note: this function does not verify that either \a key or \a value
    are properly encoded. It is the caller's responsibility to ensure
    that the query delimiters are properly encoded, if any.

    \sa addQueryItem(), setQueryDelimiters()
*/
void QUrl::addEncodedQueryItem(const QByteArray &key, const QByteArray &value)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);

    if (!d->query.isEmpty())
        d->query += d->pairDelimiter;

    d->query += key;
    d->query += d->valueDelimiter;
    d->query += value;

    d->hasQuery = !d->query.isEmpty();
}

/*!
    Returns the query string of the URL, as a map of keys and values.

    \note This method does not decode spaces plus (+) signs as spaces (ASCII
    0x20), like HTML forms do. If you need that kind of decoding, you must
    use QUrl::encodedQueryItems and decode the data yourself.

    \sa setQueryItems(), setEncodedQuery()
*/
QList<QPair<QString, QString> > QUrl::queryItems() const
{
    if (!d) return QList<QPair<QString, QString> >();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    QList<QPair<QString, QString> > itemMap;

    int pos = 0;
    const char *query = d->query.constData();
    while (pos < d->query.size()) {
        int valuedelim, end;
        d->queryItem(pos, &valuedelim, &end);
        QByteArray q(query + pos, valuedelim - pos);
        if (valuedelim < end) {
            QByteArray v(query + valuedelim + 1, end - valuedelim - 1);
            itemMap += qMakePair(fromPercentEncodingMutable(&q),
                                 fromPercentEncodingMutable(&v));
        } else {
            itemMap += qMakePair(fromPercentEncodingMutable(&q), QString());
        }
        pos = end + 1;
    }

    return itemMap;
}

/*!
    \since 4.4

    Returns the query string of the URL, as a map of encoded keys and values.

    \sa setEncodedQueryItems(), setQueryItems(), setEncodedQuery()
*/
QList<QPair<QByteArray, QByteArray> > QUrl::encodedQueryItems() const
{
    if (!d) return QList<QPair<QByteArray, QByteArray> >();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    QList<QPair<QByteArray, QByteArray> > itemMap;

    int pos = 0;
    const char *query = d->query.constData();
    while (pos < d->query.size()) {
        int valuedelim, end;
        d->queryItem(pos, &valuedelim, &end);
        if (valuedelim < end)
            itemMap += qMakePair(QByteArray(query + pos, valuedelim - pos),
                                 QByteArray(query + valuedelim + 1, end - valuedelim - 1));
        else
            itemMap += qMakePair(QByteArray(query + pos, valuedelim - pos), QByteArray());
        pos = end + 1;
    }

    return itemMap;
}

/*!
    Returns true if there is a query string pair whose key is equal
    to \a key from the URL.

    \sa hasEncodedQueryItem()
*/
bool QUrl::hasQueryItem(const QString &key) const
{
    return hasEncodedQueryItem(toPercentEncoding(key, queryExcludeChars));
}

/*!
    \since 4.4

    Returns true if there is a query string pair whose key is equal
    to \a key from the URL.

    Note: if the encoded \a key does not match the encoded version of
    the query, this function will return false. That is, if the
    encoded query of this URL is "search=Qt%20Rules", calling this
    function with \a key = "%73earch" will return false.

    \sa hasQueryItem()
*/
bool QUrl::hasEncodedQueryItem(const QByteArray &key) const
{
    if (!d) return false;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    int pos = 0;
    const char *query = d->query.constData();
    while (pos < d->query.size()) {
        int valuedelim, end;
        d->queryItem(pos, &valuedelim, &end);
        if (key == QByteArray::fromRawData(query + pos, valuedelim - pos))
            return true;
        pos = end + 1;
    }
    return false;
}

/*!
    Returns the first query string value whose key is equal to \a key
    from the URL.

    \note This method does not decode spaces plus (+) signs as spaces (ASCII
    0x20), like HTML forms do. If you need that kind of decoding, you must
    use QUrl::encodedQueryItemValue and decode the data yourself.

    \sa allQueryItemValues()
*/
QString QUrl::queryItemValue(const QString &key) const
{
    if (!d) return QString();
    QByteArray tmp = encodedQueryItemValue(toPercentEncoding(key, queryExcludeChars));
    return fromPercentEncodingMutable(&tmp);
}

/*!
    \since 4.4

    Returns the first query string value whose key is equal to \a key
    from the URL.

    Note: if the encoded \a key does not match the encoded version of
    the query, this function will not work. That is, if the
    encoded query of this URL is "search=Qt%20Rules", calling this
    function with \a key = "%73earch" will return an empty string.

    \sa queryItemValue(), allQueryItemValues()
*/
QByteArray QUrl::encodedQueryItemValue(const QByteArray &key) const
{
    if (!d) return QByteArray();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    int pos = 0;
    const char *query = d->query.constData();
    while (pos < d->query.size()) {
        int valuedelim, end;
        d->queryItem(pos, &valuedelim, &end);
        if (key == QByteArray::fromRawData(query + pos, valuedelim - pos))
            return valuedelim < end ?
                QByteArray(query + valuedelim + 1, end - valuedelim - 1) : QByteArray();
        pos = end + 1;
    }
    return QByteArray();
}

/*!
    Returns the a list of query string values whose key is equal to
    \a key from the URL.

    \note This method does not decode spaces plus (+) signs as spaces (ASCII
    0x20), like HTML forms do. If you need that kind of decoding, you must
    use QUrl::allEncodedQueryItemValues and decode the data yourself.

    \sa queryItemValue()
*/
QStringList QUrl::allQueryItemValues(const QString &key) const
{
    if (!d) return QStringList();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    QByteArray encodedKey = toPercentEncoding(key, queryExcludeChars);
    QStringList values;

    int pos = 0;
    const char *query = d->query.constData();
    while (pos < d->query.size()) {
        int valuedelim, end;
        d->queryItem(pos, &valuedelim, &end);
        if (encodedKey == QByteArray::fromRawData(query + pos, valuedelim - pos)) {
            QByteArray v(query + valuedelim + 1, end - valuedelim - 1);
            values += valuedelim < end ?
                      fromPercentEncodingMutable(&v)
                      : QString();
        }
        pos = end + 1;
    }

    return values;
}

/*!
    \since 4.4

    Returns the a list of query string values whose key is equal to
    \a key from the URL.

    Note: if the encoded \a key does not match the encoded version of
    the query, this function will not work. That is, if the
    encoded query of this URL is "search=Qt%20Rules", calling this
    function with \a key = "%73earch" will return an empty list.

    \sa allQueryItemValues(), queryItemValue(), encodedQueryItemValue()
*/
QList<QByteArray> QUrl::allEncodedQueryItemValues(const QByteArray &key) const
{
    if (!d) return QList<QByteArray>();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    QList<QByteArray> values;

    int pos = 0;
    const char *query = d->query.constData();
    while (pos < d->query.size()) {
        int valuedelim, end;
        d->queryItem(pos, &valuedelim, &end);
        if (key == QByteArray::fromRawData(query + pos, valuedelim - pos))
            values += valuedelim < end ?
                      QByteArray(query + valuedelim + 1, end - valuedelim - 1)
                      : QByteArray();
        pos = end + 1;
    }

    return values;
}

/*!
    Removes the first query string pair whose key is equal to \a key
    from the URL.

    \sa removeAllQueryItems()
*/
void QUrl::removeQueryItem(const QString &key)
{
    removeEncodedQueryItem(toPercentEncoding(key, queryExcludeChars));
}

/*!
    \since 4.4

    Removes the first query string pair whose key is equal to \a key
    from the URL.

    Note: if the encoded \a key does not match the encoded version of
    the query, this function will not work. That is, if the
    encoded query of this URL is "search=Qt%20Rules", calling this
    function with \a key = "%73earch" will do nothing.

    \sa removeQueryItem(), removeAllQueryItems()
*/
void QUrl::removeEncodedQueryItem(const QByteArray &key)
{
    if (!d) return;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);

    int pos = 0;
    const char *query = d->query.constData();
    while (pos < d->query.size()) {
        int valuedelim, end;
        d->queryItem(pos, &valuedelim, &end);
        if (key == QByteArray::fromRawData(query + pos, valuedelim - pos)) {
            if (end < d->query.size())
                ++end; // remove additional '%'
            d->query.remove(pos, end - pos);
            return;
        }
        pos = end + 1;
    }
}

/*!
    Removes all the query string pairs whose key is equal to \a key
    from the URL.

   \sa removeQueryItem()
*/
void QUrl::removeAllQueryItems(const QString &key)
{
    removeAllEncodedQueryItems(toPercentEncoding(key, queryExcludeChars));
}

/*!
    \since 4.4

    Removes all the query string pairs whose key is equal to \a key
    from the URL.

    Note: if the encoded \a key does not match the encoded version of
    the query, this function will not work. That is, if the
    encoded query of this URL is "search=Qt%20Rules", calling this
    function with \a key = "%73earch" will do nothing.

   \sa removeQueryItem()
*/
void QUrl::removeAllEncodedQueryItems(const QByteArray &key)
{
    if (!d) return;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);

    int pos = 0;
    const char *query = d->query.constData();
    while (pos < d->query.size()) {
        int valuedelim, end;
        d->queryItem(pos, &valuedelim, &end);
        if (key == QByteArray::fromRawData(query + pos, valuedelim - pos)) {
            if (end < d->query.size())
                ++end; // remove additional '%'
            d->query.remove(pos, end - pos);
            query = d->query.constData(); //required if remove detach;
        } else {
            pos = end + 1;
        }
    }
}

/*!
    Returns the query string of the URL in percent encoded form.
*/
QByteArray QUrl::encodedQuery() const
{
    if (!d) return QByteArray();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    return d->query;
}

/*!
    Sets the fragment of the URL to \a fragment. The fragment is the
    last part of the URL, represented by a '#' followed by a string of
    characters. It is typically used in HTTP for referring to a
    certain link or point on a page:

    \img qurl-fragment.png

    The fragment is sometimes also referred to as the URL "reference".

    Passing an argument of QString() (a null QString) will unset the fragment.
    Passing an argument of QString("") (an empty but not null QString)
    will set the fragment to an empty string (as if the original URL
    had a lone "#").

    \sa fragment(), hasFragment()
*/
void QUrl::setFragment(const QString &fragment)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    d->fragment = fragment;
    d->hasFragment = !fragment.isNull();
    d->encodedFragment.clear();
}

/*!
    Returns the fragment of the URL.

    \sa setFragment()
*/
QString QUrl::fragment() const
{
    if (!d) return QString();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    return d->fragmentImpl();
}

QString QUrlPrivate::fragmentImpl() const
{
    if (fragment.isNull() && !encodedFragment.isNull()) {
        QUrlPrivate *that = const_cast<QUrlPrivate *>(this);
        that->fragment = fromPercentEncodingHelper(encodedFragment);
    }
    return fragment;
}

/*!
    \since 4.4

    Sets the URL's fragment to the percent-encoded \a fragment. The fragment is the
    last part of the URL, represented by a '#' followed by a string of
    characters. It is typically used in HTTP for referring to a
    certain link or point on a page:

    \img qurl-fragment.png

    The fragment is sometimes also referred to as the URL "reference".

    Passing an argument of QByteArray() (a null QByteArray) will unset
    the fragment.  Passing an argument of QByteArray("") (an empty but
    not null QByteArray) will set the fragment to an empty string (as
    if the original URL had a lone "#").

    \sa setFragment(), encodedFragment()
*/
void QUrl::setEncodedFragment(const QByteArray &fragment)
{
    if (!d) d = new QUrlPrivate;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    detach(lock);
    QURL_UNSETFLAG(d->stateFlags, QUrlPrivate::Validated | QUrlPrivate::Normalized);

    d->encodedFragment = fragment;
    d->hasFragment = !fragment.isNull();
    d->fragment.clear();
}

/*!
    \since 4.4

    Returns the fragment of the URL if it is defined; otherwise an
    empty string is returned. The returned value will have its
    non-ASCII and other control characters percent-encoded, as in
    toEncoded().

    \sa setEncodedFragment(), toEncoded()
*/
QByteArray QUrl::encodedFragment() const
{
    if (!d) return QByteArray();

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    d->ensureEncodedParts();
    return d->encodedFragment;
}

/*!
    \since 4.2

    Returns true if this URL contains a fragment (i.e., if # was seen on it).

    \sa fragment(), setFragment()
*/
bool QUrl::hasFragment() const
{
    if (!d) return false;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    return d->hasFragment;
}

/*!
    \since 4.8

    Returns the TLD (Top-Level Domain) of the URL, (e.g. .co.uk, .net).
    Note that the return value is prefixed with a '.' unless the
    URL does not contain a valid TLD, in which case the function returns
    an empty string.
*/
QString QUrl::topLevelDomain() const
{
    return qTopLevelDomain(host());
}

/*!
    Returns the result of the merge of this URL with \a relative. This
    URL is used as a base to convert \a relative to an absolute URL.

    If \a relative is not a relative URL, this function will return \a
    relative directly. Otherwise, the paths of the two URLs are
    merged, and the new URL returned has the scheme and authority of
    the base URL, but with the merged path, as in the following
    example:

    \snippet doc/src/snippets/code/src_corelib_io_qurl.cpp 5

    Calling resolved() with ".." returns a QUrl whose directory is
    one level higher than the original. Similarly, calling resolved()
    with "../.." removes two levels from the path. If \a relative is
    "/", the path becomes "/".

    \sa isRelative()
*/
QUrl QUrl::resolved(const QUrl &relative) const
{
    if (!d) return relative;
    if (!relative.d) return *this;

    QOrderedMutexLocker locker(&d->mutex, &relative.d->mutex);

    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    if (!QURL_HASFLAG(relative.d->stateFlags, QUrlPrivate::Parsed))
        relative.d->parse();

    d->ensureEncodedParts();
    relative.d->ensureEncodedParts();

    QUrl t;
    // be non strict and allow scheme in relative url
    if (!relative.d->scheme.isEmpty() && relative.d->scheme != d->scheme) {
        t = relative;
        // t.detach(locker) would unlock, so bypass it
        qAtomicDetach(t.d);
    } else {
        if (!relative.d->authority().isEmpty()) {
            t = relative;
            qAtomicDetach(t.d);
        } else {
            t.d = new QUrlPrivate;
            if (relative.d->encodedPath.isEmpty()) {
                t.d->encodedPath = d->encodedPath;
                t.d->query = relative.d->hasQuery ? relative.d->query : d->query;
            } else {
                t.d->encodedPath = relative.d->encodedPath.at(0) == '/'
                                       ? relative.d->encodedPath
                                       : d->mergePaths(relative.d->encodedPath);
                t.d->query = relative.d->query;
            }
            t.d->hasQuery = !t.d->query.isNull();
            t.d->encodedUserName = d->encodedUserName;
            t.d->encodedPassword = d->encodedPassword;
            t.d->host = d->host;
            t.d->port = d->port;
        }
        t.setScheme(d->scheme);
    }
    t.setFragment(relative.d->fragmentImpl());
    removeDotsFromPath(&t.d->encodedPath);
    t.d->path.clear();

#if defined(QURL_DEBUG)
    qDebug("QUrl(\"%s\").resolved(\"%s\") = \"%s\"",
           d->toEncoded().constData(),
           relative.d->toEncoded().constData(),
           t.toEncoded().constData());
#endif
    return t;
}

/*!
    Returns true if the URL is relative; otherwise returns false. A
    URL is relative if its scheme is undefined; this function is
    therefore equivalent to calling scheme().isEmpty().
*/
bool QUrl::isRelative() const
{
    if (!d) return true;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    return d->scheme.isEmpty();
}

/*!
    Returns the human-displayable string representation of the
    URL. The output can be customized by passing flags with \a
    options.

    \sa FormattingOptions, toEncoded()
*/
QString QUrl::toString(FormattingOptions options) const
{
    if (!d) return QString();

    QString ourPath = path();
    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    QString url;

    if (!(options & QUrl::RemoveScheme) && !d->scheme.isEmpty())
        url += d->scheme + QLatin1Char(':');
    if ((options & QUrl::RemoveAuthority) != QUrl::RemoveAuthority) {
        bool doFileScheme = d->scheme == QLatin1String("file") && ourPath.startsWith(QLatin1Char('/'));
        QString tmp = d->authority(options);
        if (!tmp.isNull() || doFileScheme) {
            if (doFileScheme && !ourPath.startsWith(QLatin1Char('/')))
                url += QLatin1Char('/');
            url += QLatin1String("//");
            url += tmp;
        }
    }
    if (!(options & QUrl::RemovePath)) {
        // check if we need to insert a slash
        if ((options & QUrl::RemoveAuthority) != QUrl::RemoveAuthority
            && !d->authority(options).isEmpty() && !ourPath.isEmpty() && ourPath.at(0) != QLatin1Char('/'))
            url += QLatin1Char('/');
        url += ourPath;
        // check if we need to remove trailing slashes
        while ((options & StripTrailingSlash) && url.endsWith(QLatin1Char('/')))
            url.chop(1);
    }

    if (!(options & QUrl::RemoveQuery) && d->hasQuery) {
        url += QLatin1Char('?');
        url += fromPercentEncoding(d->query);
    }
    if (!(options & QUrl::RemoveFragment) && d->hasFragment) {
        url += QLatin1Char('#');
        url += d->fragmentImpl();
    }

    return url;
}

/*!
    Returns the encoded representation of the URL if it's valid;
    otherwise an empty QByteArray is returned. The output can be
    customized by passing flags with \a options.

    The user info, path and fragment are all converted to UTF-8, and
    all non-ASCII characters are then percent encoded. The host name
    is encoded using Punycode.
*/
QByteArray QUrl::toEncoded(FormattingOptions options) const
{
    if (!d) return QByteArray();

    QMutexLocker lock(&d->mutex);

    return d->toEncoded(options);
}

/*!
    Parses \a input using \a parsingMode and returns the corresponding
    QUrl. \a input is assumed to be in encoded form, containing only
    ASCII characters.

    \sa toEncoded(), setUrl()
*/
QUrl QUrl::fromEncoded(const QByteArray &input, ParsingMode parsingMode)
{
    QUrl tmp;
    tmp.setEncodedUrl(input, parsingMode);
    return tmp;
}

/*!
    Returns a decoded copy of \a input. \a input is first decoded from
    percent encoding, then converted from UTF-8 to unicode.
*/
QString QUrl::fromPercentEncoding(const QByteArray &input)
{
    return fromPercentEncodingHelper(input);
}

/*!
    Returns an encoded copy of \a input. \a input is first converted
    to UTF-8, and all ASCII-characters that are not in the unreserved group
    are percent encoded. To prevent characters from being percent encoded
    pass them to \a exclude. To force characters to be percent encoded pass
    them to \a include.

    Unreserved is defined as:
       ALPHA / DIGIT / "-" / "." / "_" / "~"

    \snippet doc/src/snippets/code/src_corelib_io_qurl.cpp 6
*/
QByteArray QUrl::toPercentEncoding(const QString &input, const QByteArray &exclude, const QByteArray &include)
{
    return toPercentEncodingHelper(input, exclude.constData(), include.constData());
}

/*!
    \obsolete
    Returns a \a uc in Punycode encoding.

    Punycode is a Unicode encoding used for internationalized domain
    names, as defined in RFC3492. If you want to convert a domain name from
    Unicode to its ASCII-compatible representation, use toAce().
*/
QByteArray QUrl::toPunycode(const QString &uc)
{
    return QUrl::toAce(uc);
}

/*!
    \obsolete
    Returns the Punycode decoded representation of \a pc.

    Punycode is a Unicode encoding used for internationalized domain
    names, as defined in RFC3492. If you want to convert a domain from
    its ASCII-compatible encoding to the Unicode representation, use
    fromAce().
*/
QString QUrl::fromPunycode(const QByteArray &pc)
{
    return QUrl::fromAce(pc);
}

/*!
    \since 4.2

    Returns the Unicode form of the given domain name
    \a domain, which is encoded in the ASCII Compatible Encoding (ACE).
    The result of this function is considered equivalent to \a domain.

    If the value in \a domain cannot be encoded, it will be converted
    to QString and returned.

    The ASCII Compatible Encoding (ACE) is defined by RFC 3490, RFC 3491
    and RFC 3492. It is part of the Internationalizing Domain Names in
    Applications (IDNA) specification, which allows for domain names
    (like \c "example.com") to be written using international
    characters.
*/
QString QUrl::fromAce(const QByteArray &domain)
{
    if (domain.isEmpty()) {
        return QString();
    }

    const UIDNA *globalidna = qGlobalIDNA()->idna;
    if (Q_UNLIKELY(!globalidna)) {
        qWarning("QUrl::fromAce: no IDNA instance");
        return QString();
    }

    const QString utf8 = QString::fromUtf8(domain);
    UErrorCode error = U_ZERO_ERROR;
    UIDNAInfo info = UIDNA_INFO_INITIALIZER;
    const int maxlength = utf8.size() * 4;
    QSTACKARRAY(UChar, result, maxlength);
    const int idnaresult = uidna_nameToUnicode(globalidna,
        reinterpret_cast<const UChar*>(utf8.unicode()), utf8.size(),
        result, maxlength, &info, &error);

    if (Q_UNLIKELY(U_FAILURE(error) && info.errors != 0)) {
        qWarning("QUrl::fromAce: failed %s", u_errorName(error));
        return utf8;
    }

    return QString(reinterpret_cast<QChar*>(result), idnaresult);
}

/*!
    \since 4.2

    Returns the ASCII Compatible Encoding of the given domain name \a domain.
    The result of this function is considered equivalent to \a domain.

    The ASCII-Compatible Encoding (ACE) is defined by RFC 3490, RFC 3491
    and RFC 3492. It is part of the Internationalizing Domain Names in
    Applications (IDNA) specification, which allows for domain names
    (like \c "example.com") to be written using international
    characters.

    This function return an empty QByteArra if \a domain is not a valid
    hostname. Note, in particular, that IPv6 literals are not valid domain
    names.
*/
QByteArray QUrl::toAce(const QString &domain)
{
    if (domain.isEmpty()) {
        return QByteArray();
    }

    const UIDNA *globalidna = qGlobalIDNA()->idna;
    if (Q_UNLIKELY(!globalidna)) {
        qWarning("QUrl::fromAce: no IDNA instance");
        return QByteArray();
    }

    UErrorCode error = U_ZERO_ERROR;
    UIDNAInfo info = UIDNA_INFO_INITIALIZER;
    const int maxlength = domain.size() * 4;
    QSTACKARRAY(UChar, result, maxlength);
    const int idnaresult = uidna_nameToASCII(globalidna,
        reinterpret_cast<const UChar*>(domain.unicode()), domain.size(),
        result, maxlength, &info, &error);

    if (Q_UNLIKELY(U_FAILURE(error) && info.errors != 0)) {
        qWarning("QUrl::toAce: failed %s", u_errorName(error));
        return QByteArray();
    }

    return QString(reinterpret_cast<QChar*>(result), idnaresult).toLatin1();
}

/*!
    \internal

    Returns true if this URL is "less than" the given \a url. This
    provides a means of ordering URLs.
*/
bool QUrl::operator <(const QUrl &url) const
{
    QOrderedMutexLocker(d ? &d->mutex : 0, url.d ? &url.d->mutex : 0);
    if (!d) return url.d ? QByteArray() < url.d->normalized() : false;
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    if (!url.d) return d->normalized() < QByteArray();
    if (!QURL_HASFLAG(url.d->stateFlags, QUrlPrivate::Parsed)) url.d->parse();
    return d->normalized() < url.d->normalized();
}

/*!
    Returns true if this URL and the given \a url are equal;
    otherwise returns false.
*/
bool QUrl::operator ==(const QUrl &url) const
{
    if (!d) return url.isEmpty();
    if (!url.d) return isEmpty();
    QOrderedMutexLocker(&d->mutex, &url.d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    if (!QURL_HASFLAG(url.d->stateFlags, QUrlPrivate::Parsed)) url.d->parse();
    return d->normalized() == url.d->normalized();
}

/*!
    Returns true if this URL and the given \a url are not equal;
    otherwise returns false.
*/
bool QUrl::operator !=(const QUrl &url) const
{
    return !(*this == url);
}

/*!
    Assigns the specified \a url to this object.
*/
QUrl &QUrl::operator =(const QUrl &url)
{
    if (!d) {
        if (url.d) {
            url.d->ref.ref();
            d = url.d;
        }
    } else {
        if (url.d)
            qAtomicAssign(d, url.d);
        else
            clear();
    }
    return *this;
}

/*!
    Assigns the specified \a url to this object.
*/
QUrl &QUrl::operator =(const QString &url)
{
    if (url.isEmpty()) {
        clear();
    } else {
        QUrl tmp(url);
        if (!d) d = new QUrlPrivate;
        qAtomicAssign(d, tmp.d);
    }
    return *this;
}

/*!
    \fn void QUrl::swap(QUrl &other)
    \since 4.8

    Swaps URL \a other with this URL. This operation is very
    fast and never fails.
*/

/*! \internal

    Forces a detach. Unlocks \a locker once the detaching is done.

    It's ok to access private members afterwards, without lock, since
    after detaching we have our private copy of the d pointer, that
    no other QUrl instance can know about yet.
*/
void QUrl::detach(QMutexLocker &locker)
{
    Q_ASSERT(d); // we have a locker, so we have a d pointer
    // Ensure that we hold the mutex until after making our private copy,
    // so that another thread cannot deref + delete d meanwhile.
    // So this is a modified version of qAtomicDetach(d)
    if (d->ref == 1) {
        locker.unlock();
        return;
    }
    QUrlPrivate * x = d;
    d = new QUrlPrivate(*x);
    locker.unlock();
    if (!x->ref.deref())
        delete x;
}

/*!
    Returns a QUrl representation of \a localFile, interpreted as a local
    file. This function accepts paths separated by slashes as well as the
    native separator for this platform.

    This function also accepts paths with a doubled leading slash (or
    backslash) to indicate a remote file, as in
    "//servername/path/to/file.txt". Note that only certain platforms can
    actually open this file using QFile::open().

    \sa toLocalFile(), isLocalFile()
*/
QUrl QUrl::fromLocalFile(const QString &localFile)
{
    QUrl url;
    url.setScheme(QLatin1String("file"));
    url.setPath(localFile);

    return url;
}

/*!
    Returns the path of this URL formatted as a local file path. The path
    returned will use forward slashes, even if it was originally created
    from one with backslashes.

    If this is a relative URL, in Qt 4.x this function returns the path to
    maintain backward compatability. This will change from 5.0 onwards. Then
    the path is returned only for URLs where the scheme is "file", and for
    all other URLs an empty string is returned.

    \sa fromLocalFile(), isLocalFile()
*/
QString QUrl::toLocalFile() const
{
    if (!d) return QString();

    // the call to isLocalFile() also ensures that we're parsed
    if (!isLocalFile() && !scheme().isEmpty())
        return QString();

    return path();
}

/*!
    \since 4.8
    Returns true if this URL is pointing to a local file path. A URL is a
    local file path if the scheme is "file".

    Note that this function considers URLs with hostnames to be local file
    paths, even if the eventual file path cannot be opened with
    QFile::open().

    \sa fromLocalFile(), toLocalFile()
*/
bool QUrl::isLocalFile() const
{
    if (!d) return false;

    QMutexLocker lock(&d->mutex);
    if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();

    if (d->scheme.compare(QLatin1String("file"), Qt::CaseInsensitive) != 0)
        return false;   // not file
    return true;
}

/*!
    Returns true if this URL is a parent of \a childUrl. \a childUrl is a child
    of this URL if the two URLs share the same scheme and authority,
    and this URL's path is a parent of the path of \a childUrl.
*/
bool QUrl::isParentOf(const QUrl &childUrl) const
{
    QString childPath = childUrl.path();

    if (!d)
        return ((childUrl.scheme().isEmpty())
            && (childUrl.authority().isEmpty())
            && childPath.length() > 0 && childPath.at(0) == QLatin1Char('/'));

    {
        QMutexLocker lock(&d->mutex);
        if (!QURL_HASFLAG(d->stateFlags, QUrlPrivate::Parsed)) d->parse();
    }

    QString ourPath = path();

    return ((childUrl.scheme().isEmpty() || scheme() == childUrl.scheme())
            && (childUrl.authority().isEmpty() || authority() == childUrl.authority())
            &&  childPath.startsWith(ourPath)
            && ((ourPath.endsWith(QLatin1Char('/')) && childPath.length() > ourPath.length())
                || (!ourPath.endsWith(QLatin1Char('/'))
                    && childPath.length() > ourPath.length() && childPath.at(ourPath.length()) == QLatin1Char('/'))));
}

/*!
    \fn void QUrl::setProtocol(const QString &s)

    Use setScheme() instead.
*/

/*!
    \fn void QUrl::setUser(const QString &s)

    Use setUserName() instead.
*/

/*!
    \fn bool QUrl::hasUser() const

    Use !userName().isEmpty() instead.
*/

/*!
    \fn bool QUrl::hasPassword() const

    Use !password().isEmpty() instead.
*/

/*!
    \fn bool QUrl::hasHost() const

    Use !host().isEmpty() instead.
*/

/*!
    \fn bool QUrl::hasPort() const

    Use port() != -1 instead.
*/

/*!
    \fn bool QUrl::hasPath() const

    Use !path().isEmpty() instead.
*/

/*!
    \fn void QUrl::setQuery(const QString &txt)

    Use setEncodedQuery() instead.
*/

/*!
    \fn void QUrl::setRef(const QString &txt)

    Use setFragment() instead.
*/

/*!
    \fn bool QUrl::hasRef() const

    Use !fragment().isEmpty() instead.
*/

/*!
    \fn void QUrl::addPath(const QString &p)

    Use setPath() instead.
*/

/*!
    \fn void QUrl::setFileName(const QString &txt)

    Use setPath() instead.
*/

/*!
    \fn void QUrl::decode(QString &url)

    Use fromPercentEncoding() instead.
*/

/*!
    \fn void QUrl::encode(QString &url)

    Use toPercentEncoding() instead.
*/

/*!
    \fn bool QUrl::cdUp()

    Use resolved("..") instead.

    \oldcode
        QUrl url("http://example.com/Developer/");
        url.cdUp();
    \newcode
        QUrl url("http://example.com/Developer/");
        url = url.resolved("..");
    \endcode
*/

/*!
    \fn bool QUrl::isRelativeUrl(const QString &url)

    Use isRelative() instead.
*/

/*!
    \fn void QUrl::reset()

    Use clear() instead.
*/

/*!
    \fn  QUrl::operator QString() const

    Use toString() instead.
*/

/*!
    \fn QString QUrl::protocol() const

    Use scheme() instead.
*/

/*!
    \fn QString QUrl::user() const

    Use userName() instead.
*/

/*!
    \fn QString QUrl::query() const

    Use encodedQuery() instead.
*/

/*!
    \fn QString QUrl::ref() const

    Use fragment() instead.
*/

/*!
    \fn QString QUrl::fileName() const

    Use QFileInfo(path()).fileName() instead.
*/

/*!
    \fn QString QUrl::dirPath() const

    Use QFileInfo(path()).absolutePath() or QFileInfo(path()) instead.
*/



#ifndef QT_NO_DATASTREAM
/*! \relates QUrl

    Writes url \a url to the stream \a out and returns a reference
    to the stream.

    \sa \link datastreamformat.html Format of the QDataStream operators \endlink
*/
QDataStream &operator<<(QDataStream &out, const QUrl &url)
{
    out << (QByteArray)url.toEncoded();
    return out;
}

/*! \relates QUrl

    Reads a url into \a url from the stream \a in and returns a
    reference to the stream.

    \sa \link datastreamformat.html Format of the QDataStream operators \endlink
*/
QDataStream &operator>>(QDataStream &in, QUrl &url)
{
    QByteArray u;
    in >> u;
    url = QUrl::fromEncoded(u);
    return in;
}
#endif // QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug d, const QUrl &url)
{
    d.maybeSpace() << "QUrl(" << url.toString() << ')';
    return d.space();
}
#endif

/*!
    \since 4.2

    Returns a text string that explains why an URL is invalid in the case being;
    otherwise returns an empty string.
*/
QString QUrl::errorString() const
{
    if (!d)
        return QLatin1String(QT_TRANSLATE_NOOP(QUrl, "Invalid URL \"\": ")); // XXX not a good message, but the one an empty URL produces
    QMutexLocker lock(&d->mutex);
    return d->createErrorString();
}

// The following code has the following copyright:
/*
   Copyright (C) Research In Motion Limited 2009. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Research In Motion Limited nor the
      contributors may be used to endorse or promote products derived
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Research In Motion Limited ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Research In Motion Limited BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/*!
    Returns a valid URL from a user supplied \a userInput string if one can be
    deducted. In the case that is not possible, an invalid QUrl() is returned.

    \since 4.6

    Most applications that can browse the web, allow the user to input a URL
    in the form of a plain string. This string can be manually typed into
    a location bar, obtained from the clipboard, or passed in via command
    line arguments.

    When the string is not already a valid URL, a best guess is performed,
    making various web related assumptions.

    In the case the string corresponds to a valid file path on the system,
    a file:// URL is constructed, using QUrl::fromLocalFile().

    If that is not the case, an attempt is made to turn the string into a
    http:// or ftp:// URL. The latter in the case the string starts with
    'ftp'. The result is then passed through QUrl's tolerant parser, and
    in the case or success, a valid QUrl is returned, or else a QUrl().

    \section1 Examples:

    \list
    \o qt.nokia.com becomes http://qt.nokia.com
    \o ftp.qt.nokia.com becomes ftp://ftp.qt.nokia.com
    \o hostname becomes http://hostname
    \o /home/user/test.html becomes file:///home/user/test.html
    \endlist
*/
QUrl QUrl::fromUserInput(const QString &userInput)
{
    const QString trimmedString = userInput.trimmed();

    // Check first for files, since on Windows drive letters can be interpretted as schemes
    if (QDir::isAbsolutePath(trimmedString))
        return QUrl::fromLocalFile(trimmedString);

    const QUrl url = QUrl::fromEncoded(trimmedString.toUtf8(), QUrl::TolerantMode);
    QUrl urlPrepended = QUrl::fromEncoded("http://" + trimmedString.toUtf8(), QUrl::TolerantMode);

    // Check the most common case of a valid url with a scheme
    // We check if the port would be valid by adding the scheme to handle the case host:port
    // where the host would be interpretted as the scheme
    if (url.isValid()
        && !url.scheme().isEmpty()
        && urlPrepended.port() == -1)
        return url;

    // Else, try the prepended one and adjust the scheme from the host name
    if (urlPrepended.isValid() && (!urlPrepended.host().isEmpty() || !urlPrepended.path().isEmpty()))
    {
        int dotIndex = trimmedString.indexOf(QLatin1Char('.'));
        const QString hostscheme = trimmedString.left(dotIndex).toLower();
        if (hostscheme == QLatin1String("ftp"))
            urlPrepended.setScheme(QLatin1String("ftp"));
        return urlPrepended;
    }

    return QUrl();
}
// end of BSD code

QT_END_NAMESPACE
