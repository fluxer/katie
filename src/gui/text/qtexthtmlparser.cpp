/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#include "qtexthtmlparser_p.h"

#include "qbytearray.h"
#include "qtextcodec.h"
#include "qapplication.h"
#include "qstack.h"
#include "qdebug.h"
#include "qthread.h"
#include "qtextdocument.h"
#include "qtextformat_p.h"
#include "qtextdocument_p.h"
#include "qtextcursor.h"
#include "qfont_p.h"

#ifndef QT_NO_TEXTHTMLPARSER

QT_BEGIN_NAMESPACE

// see also tst_qtextdocumentfragment.cpp
#define MAX_ENTITY 258
static const struct entitiesTblData {
    const QLatin1String name;
    quint16 code;
} entitiesTbl[MAX_ENTITY]= {
    { QLatin1String("AElig"), 0x00c6 },
    { QLatin1String("AMP"), 38 },
    { QLatin1String("Aacute"), 0x00c1 },
    { QLatin1String("Acirc"), 0x00c2 },
    { QLatin1String("Agrave"), 0x00c0 },
    { QLatin1String("Alpha"), 0x0391 },
    { QLatin1String("Aring"), 0x00c5 },
    { QLatin1String("Atilde"), 0x00c3 },
    { QLatin1String("Auml"), 0x00c4 },
    { QLatin1String("Beta"), 0x0392 },
    { QLatin1String("Ccedil"), 0x00c7 },
    { QLatin1String("Chi"), 0x03a7 },
    { QLatin1String("Dagger"), 0x2021 },
    { QLatin1String("Delta"), 0x0394 },
    { QLatin1String("ETH"), 0x00d0 },
    { QLatin1String("Eacute"), 0x00c9 },
    { QLatin1String("Ecirc"), 0x00ca },
    { QLatin1String("Egrave"), 0x00c8 },
    { QLatin1String("Epsilon"), 0x0395 },
    { QLatin1String("Eta"), 0x0397 },
    { QLatin1String("Euml"), 0x00cb },
    { QLatin1String("GT"), 62 },
    { QLatin1String("Gamma"), 0x0393 },
    { QLatin1String("Iacute"), 0x00cd },
    { QLatin1String("Icirc"), 0x00ce },
    { QLatin1String("Igrave"), 0x00cc },
    { QLatin1String("Iota"), 0x0399 },
    { QLatin1String("Iuml"), 0x00cf },
    { QLatin1String("Kappa"), 0x039a },
    { QLatin1String("LT"), 60 },
    { QLatin1String("Lambda"), 0x039b },
    { QLatin1String("Mu"), 0x039c },
    { QLatin1String("Ntilde"), 0x00d1 },
    { QLatin1String("Nu"), 0x039d },
    { QLatin1String("OElig"), 0x0152 },
    { QLatin1String("Oacute"), 0x00d3 },
    { QLatin1String("Ocirc"), 0x00d4 },
    { QLatin1String("Ograve"), 0x00d2 },
    { QLatin1String("Omega"), 0x03a9 },
    { QLatin1String("Omicron"), 0x039f },
    { QLatin1String("Oslash"), 0x00d8 },
    { QLatin1String("Otilde"), 0x00d5 },
    { QLatin1String("Ouml"), 0x00d6 },
    { QLatin1String("Phi"), 0x03a6 },
    { QLatin1String("Pi"), 0x03a0 },
    { QLatin1String("Prime"), 0x2033 },
    { QLatin1String("Psi"), 0x03a8 },
    { QLatin1String("QUOT"), 34 },
    { QLatin1String("Rho"), 0x03a1 },
    { QLatin1String("Scaron"), 0x0160 },
    { QLatin1String("Sigma"), 0x03a3 },
    { QLatin1String("THORN"), 0x00de },
    { QLatin1String("Tau"), 0x03a4 },
    { QLatin1String("Theta"), 0x0398 },
    { QLatin1String("Uacute"), 0x00da },
    { QLatin1String("Ucirc"), 0x00db },
    { QLatin1String("Ugrave"), 0x00d9 },
    { QLatin1String("Upsilon"), 0x03a5 },
    { QLatin1String("Uuml"), 0x00dc },
    { QLatin1String("Xi"), 0x039e },
    { QLatin1String("Yacute"), 0x00dd },
    { QLatin1String("Yuml"), 0x0178 },
    { QLatin1String("Zeta"), 0x0396 },
    { QLatin1String("aacute"), 0x00e1 },
    { QLatin1String("acirc"), 0x00e2 },
    { QLatin1String("acute"), 0x00b4 },
    { QLatin1String("aelig"), 0x00e6 },
    { QLatin1String("agrave"), 0x00e0 },
    { QLatin1String("alefsym"), 0x2135 },
    { QLatin1String("alpha"), 0x03b1 },
    { QLatin1String("amp"), 38 },
    { QLatin1String("and"), 0x22a5 },
    { QLatin1String("ang"), 0x2220 },
    { QLatin1String("apos"), 0x0027 },
    { QLatin1String("aring"), 0x00e5 },
    { QLatin1String("asymp"), 0x2248 },
    { QLatin1String("atilde"), 0x00e3 },
    { QLatin1String("auml"), 0x00e4 },
    { QLatin1String("bdquo"), 0x201e },
    { QLatin1String("beta"), 0x03b2 },
    { QLatin1String("brvbar"), 0x00a6 },
    { QLatin1String("bull"), 0x2022 },
    { QLatin1String("cap"), 0x2229 },
    { QLatin1String("ccedil"), 0x00e7 },
    { QLatin1String("cedil"), 0x00b8 },
    { QLatin1String("cent"), 0x00a2 },
    { QLatin1String("chi"), 0x03c7 },
    { QLatin1String("circ"), 0x02c6 },
    { QLatin1String("clubs"), 0x2663 },
    { QLatin1String("cong"), 0x2245 },
    { QLatin1String("copy"), 0x00a9 },
    { QLatin1String("crarr"), 0x21b5 },
    { QLatin1String("cup"), 0x222a },
    { QLatin1String("curren"), 0x00a4 },
    { QLatin1String("dArr"), 0x21d3 },
    { QLatin1String("dagger"), 0x2020 },
    { QLatin1String("darr"), 0x2193 },
    { QLatin1String("deg"), 0x00b0 },
    { QLatin1String("delta"), 0x03b4 },
    { QLatin1String("diams"), 0x2666 },
    { QLatin1String("divide"), 0x00f7 },
    { QLatin1String("eacute"), 0x00e9 },
    { QLatin1String("ecirc"), 0x00ea },
    { QLatin1String("egrave"), 0x00e8 },
    { QLatin1String("empty"), 0x2205 },
    { QLatin1String("emsp"), 0x2003 },
    { QLatin1String("ensp"), 0x2002 },
    { QLatin1String("epsilon"), 0x03b5 },
    { QLatin1String("equiv"), 0x2261 },
    { QLatin1String("eta"), 0x03b7 },
    { QLatin1String("eth"), 0x00f0 },
    { QLatin1String("euml"), 0x00eb },
    { QLatin1String("euro"), 0x20ac },
    { QLatin1String("exist"), 0x2203 },
    { QLatin1String("fnof"), 0x0192 },
    { QLatin1String("forall"), 0x2200 },
    { QLatin1String("frac12"), 0x00bd },
    { QLatin1String("frac14"), 0x00bc },
    { QLatin1String("frac34"), 0x00be },
    { QLatin1String("frasl"), 0x2044 },
    { QLatin1String("gamma"), 0x03b3 },
    { QLatin1String("ge"), 0x2265 },
    { QLatin1String("gt"), 62 },
    { QLatin1String("hArr"), 0x21d4 },
    { QLatin1String("harr"), 0x2194 },
    { QLatin1String("hearts"), 0x2665 },
    { QLatin1String("hellip"), 0x2026 },
    { QLatin1String("iacute"), 0x00ed },
    { QLatin1String("icirc"), 0x00ee },
    { QLatin1String("iexcl"), 0x00a1 },
    { QLatin1String("igrave"), 0x00ec },
    { QLatin1String("image"), 0x2111 },
    { QLatin1String("infin"), 0x221e },
    { QLatin1String("int"), 0x222b },
    { QLatin1String("iota"), 0x03b9 },
    { QLatin1String("iquest"), 0x00bf },
    { QLatin1String("isin"), 0x2208 },
    { QLatin1String("iuml"), 0x00ef },
    { QLatin1String("kappa"), 0x03ba },
    { QLatin1String("lArr"), 0x21d0 },
    { QLatin1String("lambda"), 0x03bb },
    { QLatin1String("lang"), 0x2329 },
    { QLatin1String("laquo"), 0x00ab },
    { QLatin1String("larr"), 0x2190 },
    { QLatin1String("lceil"), 0x2308 },
    { QLatin1String("ldquo"), 0x201c },
    { QLatin1String("le"), 0x2264 },
    { QLatin1String("lfloor"), 0x230a },
    { QLatin1String("lowast"), 0x2217 },
    { QLatin1String("loz"), 0x25ca },
    { QLatin1String("lrm"), 0x200e },
    { QLatin1String("lsaquo"), 0x2039 },
    { QLatin1String("lsquo"), 0x2018 },
    { QLatin1String("lt"), 60 },
    { QLatin1String("macr"), 0x00af },
    { QLatin1String("mdash"), 0x2014 },
    { QLatin1String("micro"), 0x00b5 },
    { QLatin1String("middot"), 0x00b7 },
    { QLatin1String("minus"), 0x2212 },
    { QLatin1String("mu"), 0x03bc },
    { QLatin1String("nabla"), 0x2207 },
    { QLatin1String("nbsp"), 0x00a0 },
    { QLatin1String("ndash"), 0x2013 },
    { QLatin1String("ne"), 0x2260 },
    { QLatin1String("ni"), 0x220b },
    { QLatin1String("not"), 0x00ac },
    { QLatin1String("notin"), 0x2209 },
    { QLatin1String("nsub"), 0x2284 },
    { QLatin1String("ntilde"), 0x00f1 },
    { QLatin1String("nu"), 0x03bd },
    { QLatin1String("oacute"), 0x00f3 },
    { QLatin1String("ocirc"), 0x00f4 },
    { QLatin1String("oelig"), 0x0153 },
    { QLatin1String("ograve"), 0x00f2 },
    { QLatin1String("oline"), 0x203e },
    { QLatin1String("omega"), 0x03c9 },
    { QLatin1String("omicron"), 0x03bf },
    { QLatin1String("oplus"), 0x2295 },
    { QLatin1String("or"), 0x22a6 },
    { QLatin1String("ordf"), 0x00aa },
    { QLatin1String("ordm"), 0x00ba },
    { QLatin1String("oslash"), 0x00f8 },
    { QLatin1String("otilde"), 0x00f5 },
    { QLatin1String("otimes"), 0x2297 },
    { QLatin1String("ouml"), 0x00f6 },
    { QLatin1String("para"), 0x00b6 },
    { QLatin1String("part"), 0x2202 },
    { QLatin1String("percnt"), 0x0025 },
    { QLatin1String("permil"), 0x2030 },
    { QLatin1String("perp"), 0x22a5 },
    { QLatin1String("phi"), 0x03c6 },
    { QLatin1String("pi"), 0x03c0 },
    { QLatin1String("piv"), 0x03d6 },
    { QLatin1String("plusmn"), 0x00b1 },
    { QLatin1String("pound"), 0x00a3 },
    { QLatin1String("prime"), 0x2032 },
    { QLatin1String("prod"), 0x220f },
    { QLatin1String("prop"), 0x221d },
    { QLatin1String("psi"), 0x03c8 },
    { QLatin1String("quot"), 34 },
    { QLatin1String("rArr"), 0x21d2 },
    { QLatin1String("radic"), 0x221a },
    { QLatin1String("rang"), 0x232a },
    { QLatin1String("raquo"), 0x00bb },
    { QLatin1String("rarr"), 0x2192 },
    { QLatin1String("rceil"), 0x2309 },
    { QLatin1String("rdquo"), 0x201d },
    { QLatin1String("real"), 0x211c },
    { QLatin1String("reg"), 0x00ae },
    { QLatin1String("rfloor"), 0x230b },
    { QLatin1String("rho"), 0x03c1 },
    { QLatin1String("rlm"), 0x200f },
    { QLatin1String("rsaquo"), 0x203a },
    { QLatin1String("rsquo"), 0x2019 },
    { QLatin1String("sbquo"), 0x201a },
    { QLatin1String("scaron"), 0x0161 },
    { QLatin1String("sdot"), 0x22c5 },
    { QLatin1String("sect"), 0x00a7 },
    { QLatin1String("shy"), 0x00ad },
    { QLatin1String("sigma"), 0x03c3 },
    { QLatin1String("sigmaf"), 0x03c2 },
    { QLatin1String("sim"), 0x223c },
    { QLatin1String("spades"), 0x2660 },
    { QLatin1String("sub"), 0x2282 },
    { QLatin1String("sube"), 0x2286 },
    { QLatin1String("sum"), 0x2211 },
    { QLatin1String("sup"), 0x2283 },
    { QLatin1String("sup1"), 0x00b9 },
    { QLatin1String("sup2"), 0x00b2 },
    { QLatin1String("sup3"), 0x00b3 },
    { QLatin1String("supe"), 0x2287 },
    { QLatin1String("szlig"), 0x00df },
    { QLatin1String("tau"), 0x03c4 },
    { QLatin1String("there4"), 0x2234 },
    { QLatin1String("theta"), 0x03b8 },
    { QLatin1String("thetasym"), 0x03d1 },
    { QLatin1String("thinsp"), 0x2009 },
    { QLatin1String("thorn"), 0x00fe },
    { QLatin1String("tilde"), 0x02dc },
    { QLatin1String("times"), 0x00d7 },
    { QLatin1String("trade"), 0x2122 },
    { QLatin1String("uArr"), 0x21d1 },
    { QLatin1String("uacute"), 0x00fa },
    { QLatin1String("uarr"), 0x2191 },
    { QLatin1String("ucirc"), 0x00fb },
    { QLatin1String("ugrave"), 0x00f9 },
    { QLatin1String("uml"), 0x00a8 },
    { QLatin1String("upsih"), 0x03d2 },
    { QLatin1String("upsilon"), 0x03c5 },
    { QLatin1String("uuml"), 0x00fc },
    { QLatin1String("weierp"), 0x2118 },
    { QLatin1String("xi"), 0x03be },
    { QLatin1String("yacute"), 0x00fd },
    { QLatin1String("yen"), 0x00a5 },
    { QLatin1String("yuml"), 0x00ff },
    { QLatin1String("zeta"), 0x03b6 },
    { QLatin1String("zwj"), 0x200d },
    { QLatin1String("zwnj"), 0x200c }
};

static inline QChar resolveEntity(const QString &entity)
{
    for (qint16 i = 0; i < MAX_ENTITY; i++) {
        if (entity == entitiesTbl[i].name) {
            return entitiesTbl[i].code;
        }
    }
    return QChar();
}

static const ushort windowsLatin1ExtendedCharacters[0xA0 - 0x80] = {
    0x20ac, // 0x80
    0x0081, // 0x81 direct mapping
    0x201a, // 0x82
    0x0192, // 0x83
    0x201e, // 0x84
    0x2026, // 0x85
    0x2020, // 0x86
    0x2021, // 0x87
    0x02C6, // 0x88
    0x2030, // 0x89
    0x0160, // 0x8A
    0x2039, // 0x8B
    0x0152, // 0x8C
    0x008D, // 0x8D direct mapping
    0x017D, // 0x8E
    0x008F, // 0x8F directmapping
    0x0090, // 0x90 directmapping
    0x2018, // 0x91
    0x2019, // 0x92
    0x201C, // 0x93
    0x201D, // 0x94
    0x2022, // 0x95
    0x2013, // 0x96
    0x2014, // 0x97
    0x02DC, // 0x98
    0x2122, // 0x99
    0x0161, // 0x9A
    0x203A, // 0x9B
    0x0153, // 0x9C
    0x009D, // 0x9D direct mapping
    0x017E, // 0x9E
    0x0178  // 0x9F
};

// the displayMode value is according to the what are blocks in the piecetable, not
// what the w3c defines. same as those in qglobal.cpp
static const QTextHtmlElement elementsTbl[Html_NumElements]= {
    { QLatin1String("a"),          Html_a,          QTextHtmlElement::DisplayInline },
    { QLatin1String("address"),    Html_address,    QTextHtmlElement::DisplayInline },
    { QLatin1String("b"),          Html_b,          QTextHtmlElement::DisplayInline },
    { QLatin1String("big"),        Html_big,        QTextHtmlElement::DisplayInline },
    { QLatin1String("blockquote"), Html_blockquote, QTextHtmlElement::DisplayBlock },
    { QLatin1String("body"),       Html_body,       QTextHtmlElement::DisplayBlock },
    { QLatin1String("br"),         Html_br,         QTextHtmlElement::DisplayInline },
    { QLatin1String("caption"),    Html_caption,    QTextHtmlElement::DisplayBlock },
    { QLatin1String("center"),     Html_center,     QTextHtmlElement::DisplayBlock },
    { QLatin1String("cite"),       Html_cite,       QTextHtmlElement::DisplayInline },
    { QLatin1String("code"),       Html_code,       QTextHtmlElement::DisplayInline },
    { QLatin1String("dd"),         Html_dd,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("dfn"),        Html_dfn,        QTextHtmlElement::DisplayInline },
    { QLatin1String("div"),        Html_div,        QTextHtmlElement::DisplayBlock },
    { QLatin1String("dl"),         Html_dl,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("dt"),         Html_dt,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("em"),         Html_em,         QTextHtmlElement::DisplayInline },
    { QLatin1String("font"),       Html_font,       QTextHtmlElement::DisplayInline },
    { QLatin1String("h1"),         Html_h1,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("h2"),         Html_h2,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("h3"),         Html_h3,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("h4"),         Html_h4,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("h5"),         Html_h5,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("h6"),         Html_h6,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("head"),       Html_head,       QTextHtmlElement::DisplayNone },
    { QLatin1String("hr"),         Html_hr,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("html"),       Html_html,       QTextHtmlElement::DisplayInline },
    { QLatin1String("i"),          Html_i,          QTextHtmlElement::DisplayInline },
    { QLatin1String("img"),        Html_img,        QTextHtmlElement::DisplayInline },
    { QLatin1String("kbd"),        Html_kbd,        QTextHtmlElement::DisplayInline },
    { QLatin1String("li"),         Html_li,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("link"),       Html_link,       QTextHtmlElement::DisplayNone },
    { QLatin1String("meta"),       Html_meta,       QTextHtmlElement::DisplayNone },
    { QLatin1String("nobr"),       Html_nobr,       QTextHtmlElement::DisplayInline },
    { QLatin1String("ol"),         Html_ol,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("p"),          Html_p,          QTextHtmlElement::DisplayBlock },
    { QLatin1String("pre"),        Html_pre,        QTextHtmlElement::DisplayBlock },
    { QLatin1String("qt"),         Html_body /*deliberate mapping*/, QTextHtmlElement::DisplayBlock },
    { QLatin1String("s"),          Html_s,          QTextHtmlElement::DisplayInline },
    { QLatin1String("samp"),       Html_samp,       QTextHtmlElement::DisplayInline },
    { QLatin1String("script"),     Html_script,     QTextHtmlElement::DisplayNone },
    { QLatin1String("small"),      Html_small,      QTextHtmlElement::DisplayInline },
    { QLatin1String("span"),       Html_span,       QTextHtmlElement::DisplayInline },
    { QLatin1String("strong"),     Html_strong,     QTextHtmlElement::DisplayInline },
    { QLatin1String("style"),      Html_style,      QTextHtmlElement::DisplayNone },
    { QLatin1String("sub"),        Html_sub,        QTextHtmlElement::DisplayInline },
    { QLatin1String("sup"),        Html_sup,        QTextHtmlElement::DisplayInline },
    { QLatin1String("table"),      Html_table,      QTextHtmlElement::DisplayTable },
    { QLatin1String("tbody"),      Html_tbody,      QTextHtmlElement::DisplayTable },
    { QLatin1String("td"),         Html_td,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("tfoot"),      Html_tfoot,      QTextHtmlElement::DisplayTable },
    { QLatin1String("th"),         Html_th,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("thead"),      Html_thead,      QTextHtmlElement::DisplayTable },
    { QLatin1String("title"),      Html_title,      QTextHtmlElement::DisplayNone },
    { QLatin1String("tr"),         Html_tr,         QTextHtmlElement::DisplayTable },
    { QLatin1String("tt"),         Html_tt,         QTextHtmlElement::DisplayInline },
    { QLatin1String("u"),          Html_u,          QTextHtmlElement::DisplayInline },
    { QLatin1String("ul"),         Html_ul,         QTextHtmlElement::DisplayBlock },
    { QLatin1String("var"),        Html_var,        QTextHtmlElement::DisplayInline },
};

static const QTextHtmlElement *lookupElementHelper(const QString &element)
{
    for (qint16 i = 0; i < Html_NumElements; i++) {
        if (element == elementsTbl[i].name) {
            return &elementsTbl[i];
        }
    }
    return nullptr;
}

// quotes newlines as "\\n"
static QString quoteNewline(const QString &s)
{
    QString n = s;
    n.replace(QLatin1Char('\n'), QLatin1String("\\n"));
    return n;
}

QTextHtmlParserNode::QTextHtmlParserNode()
    : parent(0), id(Html_unknown),
      cssFloat(QTextFrameFormat::InFlow), hasOwnListStyle(false),
      hasCssListIndent(false), isEmptyParagraph(false), isTextFrame(false), isRootFrame(false),
      hasHref(false), displayMode(QTextHtmlElement::DisplayInline),
      listStyle(QTextListFormat::ListStyleUndefined), imageWidth(-1), imageHeight(-1), tableBorder(0),
      tableCellRowSpan(1), tableCellColSpan(1), tableCellSpacing(2), tableCellPadding(0),
      borderBrush(Qt::darkGray), borderStyle(QTextFrameFormat::BorderStyle_Outset),
      userState(-1), cssListIndent(0), wsm(WhiteSpaceModeUndefined)
{
    margin[QTextHtmlParser::MarginLeft] = 0;
    margin[QTextHtmlParser::MarginRight] = 0;
    margin[QTextHtmlParser::MarginTop] = 0;
    margin[QTextHtmlParser::MarginBottom] = 0;
}

void QTextHtmlParser::dumpHtml()
{
    for (int i = 0; i < count(); ++i) {
        qDebug().nospace() << qPrintable(QString(depth(i)*4, QLatin1Char(' ')))
                           << qPrintable(at(i).tag) << ':'
                           << quoteNewline(at(i).text);
            ;
    }
}

QTextHtmlParserNode *QTextHtmlParser::newNode(int parent)
{
    QTextHtmlParserNode *lastNode = &nodes.last();
    QTextHtmlParserNode *newNode = 0;

    bool reuseLastNode = true;

    if (nodes.count() == 1) {
        reuseLastNode = false;
    } else if (lastNode->tag.isEmpty()) {

        if (lastNode->text.isEmpty()) {
            reuseLastNode = true;
        } else { // last node is a text node (empty tag) with some text

            if (lastNode->text.length() == 1 && lastNode->text.at(0).isSpace()) {

                int lastSibling = count() - 2;
                while (lastSibling
                       && at(lastSibling).parent != lastNode->parent
                       && at(lastSibling).displayMode == QTextHtmlElement::DisplayInline) {
                    lastSibling = at(lastSibling).parent;
                }

                if (at(lastSibling).displayMode == QTextHtmlElement::DisplayInline) {
                    reuseLastNode = false;
                } else {
                    reuseLastNode = true;
                }
            } else {
                // text node with real (non-whitespace) text -> nothing to re-use
                reuseLastNode = false;
            }

        }

    } else {
        // last node had a proper tag -> nothing to re-use
        reuseLastNode = false;
    }

    if (reuseLastNode) {
        newNode = lastNode;
        newNode->tag.clear();
        newNode->text.clear();
        newNode->id = Html_unknown;
    } else {
        nodes.resize(nodes.size() + 1);
        newNode = &nodes.last();
    }

    newNode->parent = parent;
    return newNode;
}

void QTextHtmlParser::parse(const QString &text, const QTextDocument *_resourceProvider)
{
    nodes.clear();
    nodes.resize(1);
    txt = text;
    pos = 0;
    len = txt.length();
    textEditMode = false;
    resourceProvider = _resourceProvider;
    parse();
    //dumpHtml();
}

int QTextHtmlParser::depth(int i) const
{
    int depth = 0;
    while (i) {
        i = at(i).parent;
        ++depth;
    }
    return depth;
}

int QTextHtmlParser::margin(int i, int mar) const {
    int m = 0;
    const QTextHtmlParserNode *node;
    if (mar == MarginLeft
        || mar == MarginRight) {
        while (i) {
            node = &at(i);
            if (!node->isBlock() && node->id != Html_table)
                break;
            if (node->isTableCell())
                break;
            m += node->margin[mar];
            i = node->parent;
        }
    }
    return m;
}

int QTextHtmlParser::topMargin(int i) const
{
    if (!i)
        return 0;
    return at(i).margin[MarginTop];
}

int QTextHtmlParser::bottomMargin(int i) const
{
    if (!i)
        return 0;
    return at(i).margin[MarginBottom];
}

void QTextHtmlParser::eatSpace()
{
    while (pos < len && txt.at(pos).isSpace() && txt.at(pos) != QChar::ParagraphSeparator)
        pos++;
}

void QTextHtmlParser::parse()
{
    while (pos < len) {
        QChar c = txt.at(pos++);
        if (c == QLatin1Char('<')) {
            parseTag();
        } else if (c == QLatin1Char('&')) {
            nodes.last().text += parseEntity();
        } else {
            nodes.last().text += c;
        }
    }
}

// parses a tag after "<"
void QTextHtmlParser::parseTag()
{
    eatSpace();

    // handle comments and other exclamation mark declarations
    if (hasPrefix(QLatin1Char('!'))) {
        parseExclamationTag();
        if (nodes.last().wsm != QTextHtmlParserNode::WhiteSpacePre
            && nodes.last().wsm != QTextHtmlParserNode::WhiteSpacePreWrap
            && !textEditMode)
            eatSpace();
        return;
    }

    // if close tag just close
    if (hasPrefix(QLatin1Char('/'))) {
        if (nodes.last().id == Html_style) {
#ifndef QT_NO_CSSPARSER
            QCss::Parser parser(nodes.last().text);
            QCss::StyleSheet sheet;
            sheet.origin = QCss::StyleSheetOrigin_Author;
            parser.parse(&sheet, Qt::CaseInsensitive);
            inlineStyleSheets.append(sheet);
            resolveStyleSheetImports(sheet);
#endif
        }
        parseCloseTag();
        return;
    }

    int p = last();
    while (p && at(p).tag.size() == 0)
        p = at(p).parent;

    QTextHtmlParserNode *node = newNode(p);

    // parse tag name
    node->tag = parseWord().toLower();

    const QTextHtmlElement *elem = lookupElementHelper(node->tag);
    if (elem) {
        node->id = elem->id;
        node->displayMode = elem->displayMode;
    } else {
        node->id = Html_unknown;
    }

    node->attributes.clear();
    // _need_ at least one space after the tag name, otherwise there can't be attributes
    if (pos < len && txt.at(pos).isSpace())
        node->attributes = parseAttributes();

    // resolveParent() may have to change the order in the tree and
    // insert intermediate nodes for buggy HTML, so re-initialize the 'node'
    // pointer through the return value
    node = resolveParent();
    resolveNode();

    const int nodeIndex = nodes.count() - 1; // this new node is always the last
#ifndef QT_NO_CSSPARSER
    node->applyCssDeclarations(declarationsForNode(nodeIndex), resourceProvider);
#endif
    applyAttributes(node->attributes);

    // finish tag
    bool tagClosed = false;
    while (pos < len && txt.at(pos) != QLatin1Char('>')) {
        if (txt.at(pos) == QLatin1Char('/'))
            tagClosed = true;

        pos++;
    }
    pos++;

    // in a white-space preserving environment strip off a initial newline
    // since the element itself already generates a newline
    if ((node->wsm == QTextHtmlParserNode::WhiteSpacePre
         || node->wsm == QTextHtmlParserNode::WhiteSpacePreWrap)
        && node->isBlock()) {
        if (pos < len - 1 && txt.at(pos) == QLatin1Char('\n'))
            ++pos;
    }

    if (node->mayNotHaveChildren() || tagClosed) {
        newNode(node->parent);
        resolveNode();
    }
}

// parses a tag beginning with "/"
void QTextHtmlParser::parseCloseTag()
{
    ++pos;
    QString tag = parseWord().toLower().trimmed();
    while (pos < len) {
        QChar c = txt.at(pos++);
        if (c == QLatin1Char('>'))
            break;
    }

    // find corresponding open node
    int p = last();
    if (p > 0
        && at(p - 1).tag == tag
        && at(p - 1).mayNotHaveChildren())
        p--;

    while (p && at(p).tag != tag)
        p = at(p).parent;

    // simply ignore the tag if we can't find
    // a corresponding open node, for broken
    // html such as <font>blah</font></font>
    if (!p)
        return;

    // in a white-space preserving environment strip off a trailing newline
    // since the closing of the opening block element will automatically result
    // in a new block for elements following the <pre>
    // ...foo\n</pre><p>blah -> foo</pre><p>blah
    if ((at(p).wsm == QTextHtmlParserNode::WhiteSpacePre
         || at(p).wsm == QTextHtmlParserNode::WhiteSpacePreWrap)
        && at(p).isBlock()) {
        if (at(last()).text.endsWith(QLatin1Char('\n')))
            nodes[last()].text.chop(1);
    }

    newNode(at(p).parent);
    resolveNode();
}

// parses a tag beginning with "!"
void QTextHtmlParser::parseExclamationTag()
{
    ++pos;
    if (hasPrefix(QLatin1Char('-'),1) && hasPrefix(QLatin1Char('-'),2)) {
        pos += 3;
        // eat comments
        int end = txt.indexOf(QLatin1String("-->"), pos);
        pos = (end >= 0 ? end + 3 : len);
    } else {
        // eat internal tags
        while (pos < len) {
            QChar c = txt.at(pos++);
            if (c == QLatin1Char('>'))
                break;
        }
    }
}

// parses an entity after "&", and returns it
QString QTextHtmlParser::parseEntity()
{
    int recover = pos;
    QString entity;
    while (pos < len) {
        QChar c = txt.at(pos++);
        if (c.isSpace() || pos - recover > 9) {
            goto error;
        }
        if (c == QLatin1Char(';'))
            break;
        entity += c;
    }
    {
        QChar resolved = resolveEntity(entity);
        if (!resolved.isNull())
            return QString(resolved);
    }
    if (entity.length() > 1 && entity.at(0) == QLatin1Char('#')) {
        entity.remove(0, 1); // removing leading #

        int base = 10;
        bool ok = false;

        if (entity.at(0).toLower() == QLatin1Char('x')) { // hex entity?
            entity.remove(0, 1);
            base = 16;
        }

        uint uc = entity.toUInt(&ok, base);
        if (ok) {
            if (uc >= 0x80 && uc < 0x80 + (sizeof(windowsLatin1ExtendedCharacters)/sizeof(windowsLatin1ExtendedCharacters[0])))
                uc = windowsLatin1ExtendedCharacters[uc - 0x80];
            QString str;
            if (QChar::requiresSurrogates(uc)) {
                str += QChar(QChar::highSurrogate(uc));
                str += QChar(QChar::lowSurrogate(uc));
            } else {
                str = QChar(uc);
            }
            return str;
        }
    }
error:
    pos = recover;
    return QLatin1String("&");
}

// parses one word, possibly quoted, and returns it
QString QTextHtmlParser::parseWord()
{
    QString word;
    if (hasPrefix(QLatin1Char('\"'))) { // double quotes
        ++pos;
        while (pos < len) {
            QChar c = txt.at(pos++);
            if (c == QLatin1Char('\"'))
                break;
            else if (c == QLatin1Char('&'))
                word += parseEntity();
            else
                word += c;
        }
    } else if (hasPrefix(QLatin1Char('\''))) { // single quotes
        ++pos;
        while (pos < len) {
            QChar c = txt.at(pos++);
            if (c == QLatin1Char('\''))
                break;
            else
                word += c;
        }
    } else { // normal text
        while (pos < len) {
            QChar c = txt.at(pos++);
            if (c == QLatin1Char('>')
                || (c == QLatin1Char('/') && hasPrefix(QLatin1Char('>'), 1))
                || c == QLatin1Char('<')
                || c == QLatin1Char('=')
                || c.isSpace()) {
                --pos;
                break;
            }
            if (c == QLatin1Char('&'))
                word += parseEntity();
            else
                word += c;
        }
    }
    return word;
}

// gives the new node the right parent
QTextHtmlParserNode *QTextHtmlParser::resolveParent()
{
    QTextHtmlParserNode *node = &nodes.last();

    int p = node->parent;

    // Excel gives us buggy HTML with just tr without surrounding table tags
    // or with just td tags

    if (node->id == Html_td) {
        int n = p;
        while (n && at(n).id != Html_tr)
            n = at(n).parent;

        if (!n) {
            nodes.insert(nodes.count() - 1, QTextHtmlParserNode());
            nodes.insert(nodes.count() - 1, QTextHtmlParserNode());

            QTextHtmlParserNode *table = &nodes[nodes.count() - 3];
            table->parent = p;
            table->id = Html_table;
            table->tag = QLatin1String("table");
            table->children.append(nodes.count() - 2); // add row as child

            QTextHtmlParserNode *row = &nodes[nodes.count() - 2];
            row->parent = nodes.count() - 3; // table as parent
            row->id = Html_tr;
            row->tag = QLatin1String("tr");

            p = nodes.count() - 2;
            node = &nodes.last(); // re-initialize pointer
        }
    }

    if (node->id == Html_tr) {
        int n = p;
        while (n && at(n).id != Html_table)
            n = at(n).parent;

        if (!n) {
            nodes.insert(nodes.count() - 1, QTextHtmlParserNode());
            QTextHtmlParserNode *table = &nodes[nodes.count() - 2];
            table->parent = p;
            table->id = Html_table;
            table->tag = QLatin1String("table");
            p = nodes.count() - 2;
            node = &nodes.last(); // re-initialize pointer
        }
    }

    // permit invalid html by letting block elements be children
    // of inline elements with the exception of paragraphs:
    //
    // a new paragraph closes parent inline elements (while loop),
    // unless they themselves are children of a non-paragraph block
    // element (if statement)
    //
    // For example:
    //
    // <body><p><b>Foo<p>Bar <-- second <p> implicitly closes <b> that
    //                           belongs to the first <p>. The self-nesting
    //                           check further down prevents the second <p>
    //                           from nesting into the first one then.
    //                           so Bar is not bold.
    //
    // <body><b><p>Foo <-- Foo should be bold.
    //
    // <body><b><p>Foo<p>Bar <-- Foo and Bar should be bold.
    //
    if (node->id == Html_p) {
        while (p && !at(p).isBlock())
            p = at(p).parent;

        if (!p || at(p).id != Html_p)
            p = node->parent;
    }

    // some elements are not self nesting
    if (node->id == at(p).id
        && node->isNotSelfNesting())
        p = at(p).parent;

    // some elements are not allowed in certain contexts
    while ((p && !node->allowedInContext(at(p).id))
           // ### make new styles aware of empty tags
           || at(p).mayNotHaveChildren()
       ) {
        p = at(p).parent;
    }

    node->parent = p;

    // makes it easier to traverse the tree, later
    nodes[p].children.append(nodes.count() - 1);
    return node;
}

// sets all properties on the new node
void QTextHtmlParser::resolveNode()
{
    QTextHtmlParserNode *node = &nodes.last();
    const QTextHtmlParserNode *parent = &nodes.at(node->parent);
    node->initializeProperties(parent, this);
}

bool QTextHtmlParserNode::isNestedList(const QTextHtmlParser *parser) const
{
    if (!isListStart())
        return false;

    int p = parent;
    while (p) {
        if (parser->at(p).isListStart())
            return true;
        p = parser->at(p).parent;
    }
    return false;
}

void QTextHtmlParserNode::initializeProperties(const QTextHtmlParserNode *parent, const QTextHtmlParser *parser)
{
    // inherit properties from parent element
    charFormat = parent->charFormat;

    if (id == Html_html)
        blockFormat.setLayoutDirection(Qt::LeftToRight); // HTML default
    else if (parent->blockFormat.hasProperty(QTextFormat::LayoutDirection))
        blockFormat.setLayoutDirection(parent->blockFormat.layoutDirection());

    if (parent->displayMode == QTextHtmlElement::DisplayNone)
        displayMode = QTextHtmlElement::DisplayNone;

    if (parent->id != Html_table || id == Html_caption) {
        if (parent->blockFormat.hasProperty(QTextFormat::BlockAlignment))
            blockFormat.setAlignment(parent->blockFormat.alignment());
        else
            blockFormat.clearProperty(QTextFormat::BlockAlignment);
    }
    // we don't paint per-row background colors, yet. so as an
    // exception inherit the background color here
    // we also inherit the background between inline elements
    if ((parent->id != Html_tr || !isTableCell())
        && (displayMode != QTextHtmlElement::DisplayInline || parent->displayMode != QTextHtmlElement::DisplayInline)) {
        charFormat.clearProperty(QTextFormat::BackgroundBrush);
    }

    listStyle = parent->listStyle;
    // makes no sense to inherit that property, a named anchor is a single point
    // in the document, which is set by the DocumentFragment
    charFormat.clearProperty(QTextFormat::AnchorName);
    wsm = parent->wsm;

    // initialize remaining properties
    margin[QTextHtmlParser::MarginLeft] = 0;
    margin[QTextHtmlParser::MarginRight] = 0;
    margin[QTextHtmlParser::MarginTop] = 0;
    margin[QTextHtmlParser::MarginBottom] = 0;
    cssFloat = QTextFrameFormat::InFlow;

    for (int i = 0; i < 4; ++i)
        padding[i] = -1;

    // set element specific attributes
    switch (id) {
        case Html_a:
            charFormat.setAnchor(true);
            for (int i = 0; i < attributes.count(); i += 2) {
                const QString key = attributes.at(i);
                if (key.compare(QLatin1String("href"), Qt::CaseInsensitive) == 0
                    && !attributes.at(i + 1).isEmpty()) {
                    hasHref = true;
                    charFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
                    charFormat.setForeground(QApplication::palette().link());
                }
            }

            break;
        case Html_em:
        case Html_i:
        case Html_cite:
        case Html_address:
        case Html_var:
        case Html_dfn:
            charFormat.setFontItalic(true);
            break;
        case Html_big:
            charFormat.setProperty(QTextFormat::FontSizeAdjustment, int(1));
            break;
        case Html_small:
            charFormat.setProperty(QTextFormat::FontSizeAdjustment, int(-1));
            break;
        case Html_strong:
        case Html_b:
            charFormat.setFontWeight(QFont::Bold);
            break;
        case Html_h1:
            charFormat.setFontWeight(QFont::Bold);
            charFormat.setProperty(QTextFormat::FontSizeAdjustment, int(3));
            margin[QTextHtmlParser::MarginTop] = 18;
            margin[QTextHtmlParser::MarginBottom] = 12;
            break;
        case Html_h2:
            charFormat.setFontWeight(QFont::Bold);
            charFormat.setProperty(QTextFormat::FontSizeAdjustment, int(2));
            margin[QTextHtmlParser::MarginTop] = 16;
            margin[QTextHtmlParser::MarginBottom] = 12;
            break;
        case Html_h3:
            charFormat.setFontWeight(QFont::Bold);
            charFormat.setProperty(QTextFormat::FontSizeAdjustment, int(1));
            margin[QTextHtmlParser::MarginTop] = 14;
            margin[QTextHtmlParser::MarginBottom] = 12;
            break;
        case Html_h4:
            charFormat.setFontWeight(QFont::Bold);
            charFormat.setProperty(QTextFormat::FontSizeAdjustment, int(0));
            margin[QTextHtmlParser::MarginTop] = 12;
            margin[QTextHtmlParser::MarginBottom] = 12;
            break;
        case Html_h5:
            charFormat.setFontWeight(QFont::Bold);
            charFormat.setProperty(QTextFormat::FontSizeAdjustment, int(-1));
            margin[QTextHtmlParser::MarginTop] = 12;
            margin[QTextHtmlParser::MarginBottom] = 4;
            break;
        case Html_p:
            margin[QTextHtmlParser::MarginTop] = 12;
            margin[QTextHtmlParser::MarginBottom] = 12;
            break;
        case Html_center:
            blockFormat.setAlignment(Qt::AlignCenter);
            break;
        case Html_ul:
            listStyle = QTextListFormat::ListDisc;
            // nested lists don't have margins, except for the toplevel one
            if (!isNestedList(parser)) {
                margin[QTextHtmlParser::MarginTop] = 12;
                margin[QTextHtmlParser::MarginBottom] = 12;
            }
            // no left margin as we use indenting instead
            break;
        case Html_ol:
            listStyle = QTextListFormat::ListDecimal;
            // nested lists don't have margins, except for the toplevel one
            if (!isNestedList(parser)) {
                margin[QTextHtmlParser::MarginTop] = 12;
                margin[QTextHtmlParser::MarginBottom] = 12;
            }
            // no left margin as we use indenting instead
            break;
        case Html_code:
        case Html_tt:
        case Html_kbd:
        case Html_samp:
            charFormat.setFontFamily(QString::fromLatin1("Courier New,courier"));
            // <tt> uses a fixed font, so set the property
            charFormat.setFontFixedPitch(true);
            break;
        case Html_br:
            text = QChar(QChar::LineSeparator);
            wsm = QTextHtmlParserNode::WhiteSpacePre;
            break;
        // ##### sub / sup
        case Html_pre:
            charFormat.setFontFamily(QString::fromLatin1("Courier New,courier"));
            wsm = WhiteSpacePre;
            margin[QTextHtmlParser::MarginTop] = 12;
            margin[QTextHtmlParser::MarginBottom] = 12;
            // <pre> uses a fixed font
            charFormat.setFontFixedPitch(true);
            break;
        case Html_blockquote:
            margin[QTextHtmlParser::MarginTop] = 12;
            margin[QTextHtmlParser::MarginBottom] = 12;
            margin[QTextHtmlParser::MarginLeft] = 40;
            margin[QTextHtmlParser::MarginRight] = 40;
            break;
        case Html_dl:
            margin[QTextHtmlParser::MarginTop] = 8;
            margin[QTextHtmlParser::MarginBottom] = 8;
            break;
        case Html_dd:
            margin[QTextHtmlParser::MarginLeft] = 30;
            break;
        case Html_u:
            charFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
            break;
        case Html_s:
            charFormat.setFontStrikeOut(true);
            break;
        case Html_nobr:
            wsm = WhiteSpaceNoWrap;
            break;
        case Html_th:
            charFormat.setFontWeight(QFont::Bold);
            blockFormat.setAlignment(Qt::AlignCenter);
            break;
        case Html_td:
            blockFormat.setAlignment(Qt::AlignLeft);
            break;
        case Html_sub:
            charFormat.setVerticalAlignment(QTextCharFormat::AlignSubScript);
            break;
        case Html_sup:
            charFormat.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
            break;
        default: break;
    }
}

#ifndef QT_NO_CSSPARSER
void QTextHtmlParserNode::setListStyle(const QVector<QCss::Value> &cssValues)
{
    for (int i = 0; i < cssValues.count(); ++i) {
        if (cssValues.at(i).type == QCss::Value::KnownIdentifier) {
            switch (static_cast<QCss::KnownValue>(cssValues.at(i).variant.toInt())) {
                case QCss::Value_Disc: hasOwnListStyle = true; listStyle = QTextListFormat::ListDisc; break;
                case QCss::Value_Square: hasOwnListStyle = true; listStyle = QTextListFormat::ListSquare; break;
                case QCss::Value_Circle: hasOwnListStyle = true; listStyle = QTextListFormat::ListCircle; break;
                case QCss::Value_Decimal: hasOwnListStyle = true; listStyle = QTextListFormat::ListDecimal; break;
                case QCss::Value_LowerAlpha: hasOwnListStyle = true; listStyle = QTextListFormat::ListLowerAlpha; break;
                case QCss::Value_UpperAlpha: hasOwnListStyle = true; listStyle = QTextListFormat::ListUpperAlpha; break;
                case QCss::Value_LowerRoman: hasOwnListStyle = true; listStyle = QTextListFormat::ListLowerRoman; break;
                case QCss::Value_UpperRoman: hasOwnListStyle = true; listStyle = QTextListFormat::ListUpperRoman; break;
                default: break;
            }
        }
    }
    // allow individual list items to override the style
    if (id == Html_li && hasOwnListStyle)
        blockFormat.setProperty(QTextFormat::ListStyle, listStyle);
}

void QTextHtmlParserNode::applyCssDeclarations(const QVector<QCss::Declaration> &declarations, const QTextDocument *resourceProvider)
{
    QCss::ValueExtractor extractor(declarations);
    extractor.extractBox(margin, padding);

    for (int i = 0; i < declarations.count(); ++i) {
        const QCss::Declaration &decl = declarations.at(i);
        if (decl.d->values.isEmpty()) continue;

        QCss::KnownValue identifier = QCss::UnknownValue;
        if (decl.d->values.first().type == QCss::Value::KnownIdentifier)
            identifier = static_cast<QCss::KnownValue>(decl.d->values.first().variant.toInt());

        switch (decl.d->propertyId) {
        case QCss::BorderColor: borderBrush = QBrush(decl.colorValue()); break;
        case QCss::BorderStyles:
            if (decl.styleValue() != QCss::BorderStyle_Unknown && decl.styleValue() != QCss::BorderStyle_Native)
                borderStyle = static_cast<QTextFrameFormat::BorderStyle>(decl.styleValue() - 1);
            break;
        case QCss::BorderWidth:
            tableBorder = extractor.lengthValue(decl);
            break;
        case QCss::Color: charFormat.setForeground(decl.colorValue()); break;
        case QCss::Float:
            cssFloat = QTextFrameFormat::InFlow;
            switch (identifier) {
            case QCss::Value_Left: cssFloat = QTextFrameFormat::FloatLeft; break;
            case QCss::Value_Right: cssFloat = QTextFrameFormat::FloatRight; break;
            default: break;
            }
            break;
        case QCss::QtBlockIndent:
            blockFormat.setIndent(decl.d->values.first().variant.toInt());
            break;
       case QCss::LineHeight: {
            qreal lineHeight;
            if (decl.realValue(&lineHeight, "px")) {
                blockFormat.setLineHeight(lineHeight, QTextBlockFormat::FixedHeight);
            } else {
                bool ok;
                QString value = decl.d->values.first().toString();
                lineHeight = value.toDouble(&ok);
                if (ok)
                    blockFormat.setLineHeight(lineHeight, QTextBlockFormat::ProportionalHeight);
                else
                    blockFormat.setLineHeight(0, QTextBlockFormat::SingleHeight);
            }
            break; }
        case QCss::TextIndent: {
            qreal indent = 0;
            if (decl.realValue(&indent, "px"))
                blockFormat.setTextIndent(indent);
            break; }
        case QCss::QtListIndent:
            if (decl.intValue(&cssListIndent))
                hasCssListIndent = true;
            break;
        case QCss::QtParagraphType:
            if (decl.d->values.first().variant.toString().compare(QLatin1String("empty"), Qt::CaseInsensitive) == 0)
                isEmptyParagraph = true;
            break;
        case QCss::QtTableType:
            if (decl.d->values.first().variant.toString().compare(QLatin1String("frame"), Qt::CaseInsensitive) == 0)
                isTextFrame = true;
            else if (decl.d->values.first().variant.toString().compare(QLatin1String("root"), Qt::CaseInsensitive) == 0) {
                isTextFrame = true;
                isRootFrame = true;
            }
            break;
        case QCss::QtUserState:
            userState = decl.d->values.first().variant.toInt();
            break;
        case QCss::Whitespace:
            switch (identifier) {
            case QCss::Value_Normal: wsm = QTextHtmlParserNode::WhiteSpaceNormal; break;
            case QCss::Value_Pre: wsm = QTextHtmlParserNode::WhiteSpacePre; break;
            case QCss::Value_NoWrap: wsm = QTextHtmlParserNode::WhiteSpaceNoWrap; break;
            case QCss::Value_PreWrap: wsm = QTextHtmlParserNode::WhiteSpacePreWrap; break;
            default: break;
            }
            break;
        case QCss::VerticalAlignment:
            switch (identifier) {
            case QCss::Value_Sub: charFormat.setVerticalAlignment(QTextCharFormat::AlignSubScript); break;
            case QCss::Value_Super: charFormat.setVerticalAlignment(QTextCharFormat::AlignSuperScript); break;
            case QCss::Value_Middle: charFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle); break;
            case QCss::Value_Top: charFormat.setVerticalAlignment(QTextCharFormat::AlignTop); break;
            case QCss::Value_Bottom: charFormat.setVerticalAlignment(QTextCharFormat::AlignBottom); break;
            default: charFormat.setVerticalAlignment(QTextCharFormat::AlignNormal); break;
            }
            break;
        case QCss::PageBreakBefore:
            switch (identifier) {
            case QCss::Value_Always: blockFormat.setPageBreakPolicy(blockFormat.pageBreakPolicy() | QTextFormat::PageBreak_AlwaysBefore); break;
            case QCss::Value_Auto: blockFormat.setPageBreakPolicy(blockFormat.pageBreakPolicy() & ~QTextFormat::PageBreak_AlwaysBefore); break;
            default: break;
            }
            break;
        case QCss::PageBreakAfter:
            switch (identifier) {
            case QCss::Value_Always: blockFormat.setPageBreakPolicy(blockFormat.pageBreakPolicy() | QTextFormat::PageBreak_AlwaysAfter); break;
            case QCss::Value_Auto: blockFormat.setPageBreakPolicy(blockFormat.pageBreakPolicy() & ~QTextFormat::PageBreak_AlwaysAfter); break;
            default: break;
            }
            break;
        case QCss::TextUnderlineStyle:
            switch (identifier) {
            case QCss::Value_None: charFormat.setUnderlineStyle(QTextCharFormat::NoUnderline); break;
            case QCss::Value_Solid: charFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline); break;
            case QCss::Value_Dashed: charFormat.setUnderlineStyle(QTextCharFormat::DashUnderline); break;
            case QCss::Value_Dotted: charFormat.setUnderlineStyle(QTextCharFormat::DotLine); break;
            case QCss::Value_DotDash: charFormat.setUnderlineStyle(QTextCharFormat::DashDotLine); break;
            case QCss::Value_DotDotDash: charFormat.setUnderlineStyle(QTextCharFormat::DashDotDotLine); break;
            case QCss::Value_Wave: charFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline); break;
            default: break;
            }
            break;
        case QCss::ListStyleType:
        case QCss::ListStyle:
            setListStyle(decl.d->values);
            break;
        case QCss::QtListNumberPrefix:
            textListNumberPrefix = decl.d->values.first().variant.toString();
            break;
        case QCss::QtListNumberSuffix:
            textListNumberSuffix = decl.d->values.first().variant.toString();
            break;
        default: break;
        }
    }

    QFont f;
    int adjustment = -255;
    extractor.extractFont(&f, &adjustment);
    if (f.resolve() & QFont::SizeResolved) {
        if (f.pointSize() > 0) {
            charFormat.setFontPointSize(f.pointSize());
        } else if (f.pixelSize() > 0) {
            charFormat.setProperty(QTextFormat::FontPixelSize, f.pixelSize());
        }
    }
    if (f.resolve() & QFont::StyleResolved)
        charFormat.setFontItalic(f.style() != QFont::StyleNormal);

    if (f.resolve() & QFont::WeightResolved)
        charFormat.setFontWeight(f.weight());

    if (f.resolve() & QFont::FamilyResolved)
        charFormat.setFontFamily(f.family());

    if (f.resolve() & QFont::UnderlineResolved)
        charFormat.setUnderlineStyle(f.underline() ? QTextCharFormat::SingleUnderline : QTextCharFormat::NoUnderline);

    if (f.resolve() & QFont::OverlineResolved)
        charFormat.setFontOverline(f.overline());

    if (f.resolve() & QFont::StrikeOutResolved)
        charFormat.setFontStrikeOut(f.strikeOut());

    if (f.resolve() & QFont::CapitalizationResolved)
        charFormat.setFontCapitalization(f.capitalization());

    if (adjustment >= -1)
        charFormat.setProperty(QTextFormat::FontSizeAdjustment, adjustment);

    {
        Qt::Alignment ignoredAlignment;
        QCss::Repeat ignoredRepeat;
        QString bgImage;
        QBrush bgBrush;
        QCss::Origin ignoredOrigin, ignoredClip;
        QCss::Attachment ignoredAttachment;
        extractor.extractBackground(&bgBrush, &bgImage, &ignoredRepeat, &ignoredAlignment,
                                    &ignoredOrigin, &ignoredAttachment, &ignoredClip);

        if (!bgImage.isEmpty() && resourceProvider) {
            applyBackgroundImage(bgImage, resourceProvider);
        } else if (bgBrush.style() != Qt::NoBrush) {
            charFormat.setBackground(bgBrush);
        }
    }
}

#endif // QT_NO_CSSPARSER

void QTextHtmlParserNode::applyBackgroundImage(const QString &url, const QTextDocument *resourceProvider)
{
    if (!url.isEmpty() && resourceProvider) {
        QVariant val = resourceProvider->resource(QTextDocument::ImageResource, url);

        if (qApp->thread() != QThread::currentThread()) {
            // must use images in non-GUI threads
            if (val.type() == QVariant::Image) {
                QImage image = qvariant_cast<QImage>(val);
                charFormat.setBackground(image);
            } else if (val.type() == QVariant::ByteArray) {
                QImage image = QImage::fromData(val.toByteArray());
                if (!image.isNull()) {
                    charFormat.setBackground(image);
                }
            }
        } else {
            if (val.type() == QVariant::Image || val.type() == QVariant::Pixmap) {
                charFormat.setBackground(qvariant_cast<QPixmap>(val));
            } else if (val.type() == QVariant::ByteArray) {
                QPixmap pm;
                if (pm.loadFromData(val.toByteArray())) {
                    charFormat.setBackground(pm);
                }
            }
        }
    }
    if (!url.isEmpty())
        charFormat.setProperty(QTextFormat::BackgroundImageUrl, url);
}

bool QTextHtmlParserNode::hasOnlyWhitespace() const
{
    for (int i = 0; i < text.count(); ++i)
        if (!text.at(i).isSpace() || text.at(i) == QChar::LineSeparator)
            return false;
    return true;
}

static bool setIntAttribute(int *destination, const QString &value)
{
    bool ok = false;
    int val = value.toInt(&ok);
    if (ok)
        *destination = val;

    return ok;
}

static bool setFloatAttribute(qreal *destination, const QString &value)
{
    bool ok = false;
    qreal val = value.toDouble(&ok);
    if (ok)
        *destination = val;

    return ok;
}

static void setWidthAttribute(QTextLength *width, QString value)
{
    bool ok = false;
    qreal realVal = value.toDouble(&ok);
    if (ok) {
        *width = QTextLength(QTextLength::FixedLength, realVal);
    } else {
        value = value.trimmed();
        if (!value.isEmpty() && value.endsWith(QLatin1Char('%'))) {
            value.chop(1);
            realVal = value.toDouble(&ok);
            if (ok)
                *width = QTextLength(QTextLength::PercentageLength, realVal);
        }
    }
}

#ifndef QT_NO_CSSPARSER
void QTextHtmlParserNode::parseStyleAttribute(const QString &value, const QTextDocument *resourceProvider)
{
    QString css = value;
    css.prepend(QLatin1String("* {"));
    css.append(QLatin1Char('}'));
    QCss::Parser parser(css);
    QCss::StyleSheet sheet;
    parser.parse(&sheet, Qt::CaseInsensitive);
    if (sheet.styleRules.count() != 1) return;
    applyCssDeclarations(sheet.styleRules.at(0).declarations, resourceProvider);
}
#endif

QStringList QTextHtmlParser::parseAttributes()
{
    QStringList attrs;

    while (pos < len) {
        eatSpace();
        if (hasPrefix(QLatin1Char('>')) || hasPrefix(QLatin1Char('/')))
            break;
        QString key = parseWord().toLower();
        QString value = QLatin1String("1");
        if (key.size() == 0)
            break;
        eatSpace();
        if (hasPrefix(QLatin1Char('='))){
            pos++;
            eatSpace();
            value = parseWord();
        }
        if (value.size() == 0)
            continue;
        attrs << key << value;
    }

    return attrs;
}

void QTextHtmlParser::applyAttributes(const QStringList &attributes)
{
    // local state variable for qt3 textedit mode
    bool seenQt3Richtext = false;
    QString linkHref;
    QString linkType;

    if (attributes.count() % 2 == 1)
        return;

    QTextHtmlParserNode *node = &nodes.last();

    for (int i = 0; i < attributes.count(); i += 2) {
        QString key = attributes.at(i);
        QString value = attributes.at(i + 1);

        switch (node->id) {
            case Html_font:
                // the infamous font tag
                if (key == QLatin1String("size") && value.size()) {
                    int n = value.toInt();
                    if (value.at(0) != QLatin1Char('+') && value.at(0) != QLatin1Char('-'))
                        n -= 3;
                    node->charFormat.setProperty(QTextFormat::FontSizeAdjustment, n);
                } else if (key == QLatin1String("face")) {
                    node->charFormat.setFontFamily(value);
                } else if (key == QLatin1String("color")) {
                    QColor c(value);
                    if (!c.isValid())
                        qWarning("QTextHtmlParser::applyAttributes: Unknown color name '%s'",value.toLatin1().constData());
                    node->charFormat.setForeground(c);
                }
                break;
            case Html_ol:
            case Html_ul:
                if (key == QLatin1String("type")) {
                    node->hasOwnListStyle = true;
                    if (value == QLatin1String("1")) {
                        node->listStyle = QTextListFormat::ListDecimal;
                    } else if (value == QLatin1String("a")) {
                        node->listStyle = QTextListFormat::ListLowerAlpha;
                    } else if (value == QLatin1String("A")) {
                        node->listStyle = QTextListFormat::ListUpperAlpha;
                    } else if (value == QLatin1String("i")) {
                        node->listStyle = QTextListFormat::ListLowerRoman;
                    } else if (value == QLatin1String("I")) {
                        node->listStyle = QTextListFormat::ListUpperRoman;
                    } else {
                        value = value.toLower();
                        if (value == QLatin1String("square"))
                            node->listStyle = QTextListFormat::ListSquare;
                        else if (value == QLatin1String("disc"))
                            node->listStyle = QTextListFormat::ListDisc;
                        else if (value == QLatin1String("circle"))
                            node->listStyle = QTextListFormat::ListCircle;
                    }
                }
                break;
            case Html_a:
                if (key == QLatin1String("href"))
                    node->charFormat.setAnchorHref(value);
                else if (key == QLatin1String("name"))
                    node->charFormat.setAnchorName(value);
                break;
            case Html_img:
                if (key == QLatin1String("src") || key == QLatin1String("source")) {
                    node->imageName = value;
                } else if (key == QLatin1String("width")) {
                    node->imageWidth = -2; // register that there is a value for it.
                    setFloatAttribute(&node->imageWidth, value);
                } else if (key == QLatin1String("height")) {
                    node->imageHeight = -2; // register that there is a value for it.
                    setFloatAttribute(&node->imageHeight, value);
                }
                break;
            case Html_tr:
            case Html_body:
                if (key == QLatin1String("bgcolor")) {
                    QColor c(value);
                    if (!c.isValid())
                        qWarning("QTextHtmlParser::applyAttributes: Unknown color name '%s'",value.toLatin1().constData());
                    node->charFormat.setBackground(c);
                } else if (key == QLatin1String("background")) {
                    node->applyBackgroundImage(value, resourceProvider);
                }
                break;
            case Html_th:
            case Html_td:
                if (key == QLatin1String("width")) {
                    setWidthAttribute(&node->width, value);
                } else if (key == QLatin1String("bgcolor")) {
                    QColor c(value);
                    if (!c.isValid())
                        qWarning("QTextHtmlParser::applyAttributes: Unknown color name '%s'",value.toLatin1().constData());
                    node->charFormat.setBackground(c);
                } else if (key == QLatin1String("background")) {
                    node->applyBackgroundImage(value, resourceProvider);
                } else if (key == QLatin1String("rowspan")) {
                    if (setIntAttribute(&node->tableCellRowSpan, value))
                        node->tableCellRowSpan = qMax(1, node->tableCellRowSpan);
                } else if (key == QLatin1String("colspan")) {
                    if (setIntAttribute(&node->tableCellColSpan, value))
                        node->tableCellColSpan = qMax(1, node->tableCellColSpan);
                }
                break;
            case Html_table:
                if (key == QLatin1String("border")) {
                    setFloatAttribute(&node->tableBorder, value);
                } else if (key == QLatin1String("bgcolor")) {
                    QColor c(value);
                    if (!c.isValid())
                        qWarning("QTextHtmlParser::applyAttributes: Unknown color name '%s'",value.toLatin1().constData());
                    node->charFormat.setBackground(c);
                } else if (key == QLatin1String("background")) {
                    node->applyBackgroundImage(value, resourceProvider);
                } else if (key == QLatin1String("cellspacing")) {
                    setFloatAttribute(&node->tableCellSpacing, value);
                } else if (key == QLatin1String("cellpadding")) {
                    setFloatAttribute(&node->tableCellPadding, value);
                } else if (key == QLatin1String("width")) {
                    setWidthAttribute(&node->width, value);
                } else if (key == QLatin1String("height")) {
                    setWidthAttribute(&node->height, value);
                }
                break;
            case Html_meta:
                if (key == QLatin1String("name")
                    && value == QLatin1String("qrichtext")) {
                    seenQt3Richtext = true;
                } else if (key == QLatin1String("content")
                    && value == QLatin1String("1")
                    && seenQt3Richtext) {

                    textEditMode = true;
                }
                break;
            case Html_hr:
                if (key == QLatin1String("width"))
                    setWidthAttribute(&node->width, value);
                break;
            case Html_link:
                if (key == QLatin1String("href"))
                    linkHref = value;
                else if (key == QLatin1String("type"))
                    linkType = value;
                break;
            default:
                break;
        }

        if (key == QLatin1String("style")) {
#ifndef QT_NO_CSSPARSER
            node->parseStyleAttribute(value, resourceProvider);
#endif
        } else if (key == QLatin1String("align")) {
            value = value.toLower();
            bool alignmentSet = true;

            if (value == QLatin1String("left"))
                node->blockFormat.setAlignment(Qt::AlignLeft|Qt::AlignAbsolute);
            else if (value == QLatin1String("right"))
                node->blockFormat.setAlignment(Qt::AlignRight|Qt::AlignAbsolute);
            else if (value == QLatin1String("center"))
                node->blockFormat.setAlignment(Qt::AlignHCenter);
            else if (value == QLatin1String("justify"))
                node->blockFormat.setAlignment(Qt::AlignJustify);
            else
                alignmentSet = false;

            if (node->id == Html_img) {
                // HTML4 compat
                if (alignmentSet) {
                    if (node->blockFormat.alignment() & Qt::AlignLeft)
                        node->cssFloat = QTextFrameFormat::FloatLeft;
                    else if (node->blockFormat.alignment() & Qt::AlignRight)
                        node->cssFloat = QTextFrameFormat::FloatRight;
                } else if (value == QLatin1String("middle")) {
                    node->charFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
                } else if (value == QLatin1String("top")) {
                    node->charFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
                }
            }
        } else if (key == QLatin1String("valign")) {
            value = value.toLower();
            if (value == QLatin1String("top"))
                node->charFormat.setVerticalAlignment(QTextCharFormat::AlignTop);
            else if (value == QLatin1String("middle"))
                node->charFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
            else if (value == QLatin1String("bottom"))
                node->charFormat.setVerticalAlignment(QTextCharFormat::AlignBottom);
        } else if (key == QLatin1String("dir")) {
            value = value.toLower();
            if (value == QLatin1String("ltr"))
                node->blockFormat.setLayoutDirection(Qt::LeftToRight);
            else if (value == QLatin1String("rtl"))
                node->blockFormat.setLayoutDirection(Qt::RightToLeft);
        } else if (key == QLatin1String("title")) {
            node->charFormat.setToolTip(value);
        } else if (key == QLatin1String("id")) {
            node->charFormat.setAnchor(true);
            node->charFormat.setAnchorName(value);
        }
    }

#ifndef QT_NO_CSSPARSER
    if (resourceProvider && !linkHref.isEmpty() && linkType == QLatin1String("text/css"))
        importStyleSheet(linkHref);
#endif
}

#ifndef QT_NO_CSSPARSER
class QTextHtmlStyleSelector : public QCss::StyleSelector
{
public:
    inline QTextHtmlStyleSelector(const QTextHtmlParser *parser)
        : parser(parser) { nameCaseSensitivity = Qt::CaseInsensitive; }

    virtual QStringList nodeNames(NodePtr node) const;
    virtual QString attribute(NodePtr node, const QString &name) const;
    virtual bool hasAttributes(NodePtr node) const;
    virtual bool isNullNode(NodePtr node) const;
    virtual NodePtr parentNode(NodePtr node) const;
    virtual NodePtr previousSiblingNode(NodePtr node) const;
    virtual NodePtr duplicateNode(NodePtr node) const;
    virtual void freeNode(NodePtr node) const;

private:
    const QTextHtmlParser *parser;
};

QStringList QTextHtmlStyleSelector::nodeNames(NodePtr node) const
{
    return QStringList(parser->at(node.id).tag.toLower());
}

#endif // QT_NO_CSSPARSER

static inline int findAttribute(const QStringList &attributes, const QString &name)
{
    int idx = -1;
    do {
        idx = attributes.indexOf(name, idx + 1);
    } while (idx != -1 && (idx % 2 == 1));
    return idx;
}

#ifndef QT_NO_CSSPARSER

QString QTextHtmlStyleSelector::attribute(NodePtr node, const QString &name) const
{
    const QStringList &attributes = parser->at(node.id).attributes;
    const int idx = findAttribute(attributes, name);
    if (idx == -1)
        return QString();
    return attributes.at(idx + 1);
}

bool QTextHtmlStyleSelector::hasAttributes(NodePtr node) const
{
   const QStringList &attributes = parser->at(node.id).attributes;
   return !attributes.isEmpty();
}

bool QTextHtmlStyleSelector::isNullNode(NodePtr node) const
{
    return node.id == 0;
}

QCss::StyleSelector::NodePtr QTextHtmlStyleSelector::parentNode(NodePtr node) const
{
    NodePtr parent;
    parent.id = 0;
    if (node.id) {
        parent.id = parser->at(node.id).parent;
    }
    return parent;
}

QCss::StyleSelector::NodePtr QTextHtmlStyleSelector::duplicateNode(NodePtr node) const
{
    return node;
}

QCss::StyleSelector::NodePtr QTextHtmlStyleSelector::previousSiblingNode(NodePtr node) const
{
    NodePtr sibling;
    sibling.id = 0;
    if (!node.id)
        return sibling;
    int parent = parser->at(node.id).parent;
    if (!parent)
        return sibling;
    const int childIdx = parser->at(parent).children.indexOf(node.id);
    if (childIdx <= 0)
        return sibling;
    sibling.id = parser->at(parent).children.at(childIdx - 1);
    return sibling;
}

void QTextHtmlStyleSelector::freeNode(NodePtr) const
{
}

void QTextHtmlParser::resolveStyleSheetImports(const QCss::StyleSheet &sheet)
{
    for (int i = 0; i < sheet.importRules.count(); ++i) {
        const QCss::ImportRule &rule = sheet.importRules.at(i);
        if (rule.media.isEmpty()
            || rule.media.contains(QLatin1String("screen"), Qt::CaseInsensitive))
            importStyleSheet(rule.href);
    }
}

void QTextHtmlParser::importStyleSheet(const QString &href)
{
    if (!resourceProvider)
        return;
    for (int i = 0; i < externalStyleSheets.count(); ++i)
        if (externalStyleSheets.at(i).url == href)
            return;

    QVariant res = resourceProvider->resource(QTextDocument::StyleSheetResource, href);
    QString css;
    if (res.type() == QVariant::String) {
        css = res.toString();
    } else if (res.type() == QVariant::ByteArray) {
        // #### detect @charset
        css = QString::fromUtf8(res.toByteArray());
    }
    if (!css.isEmpty()) {
        QCss::Parser parser(css);
        QCss::StyleSheet sheet;
        parser.parse(&sheet, Qt::CaseInsensitive);
        externalStyleSheets.append(ExternalStyleSheet(href, sheet));
        resolveStyleSheetImports(sheet);
    }
}

QVector<QCss::Declaration> QTextHtmlParser::declarationsForNode(int node) const
{
    QTextHtmlStyleSelector selector(this);

    int idx = 0;
    selector.styleSheets.resize((resourceProvider ? 1 : 0)
                                + externalStyleSheets.count()
                                + inlineStyleSheets.count());
    if (resourceProvider)
        selector.styleSheets[idx++] = resourceProvider->docHandle()->parsedDefaultStyleSheet;

    for (int i = 0; i < externalStyleSheets.count(); ++i, ++idx)
        selector.styleSheets[idx] = externalStyleSheets.at(i).sheet;

    for (int i = 0; i < inlineStyleSheets.count(); ++i, ++idx)
        selector.styleSheets[idx] = inlineStyleSheets.at(i);

    selector.medium = QLatin1String("screen");

    QCss::StyleSelector::NodePtr n;
    n.id = node;

    if (nodes.at(node).id == Html_a && nodes.at(node).hasHref)
        return selector.declarationsForNode(n, "link");
    return selector.declarationsForNode(n, 0);
}

bool QTextHtmlParser::nodeIsChildOf(int i, QTextHTMLElements id) const
{
    while (i) {
        if (at(i).id == id)
            return true;
        i = at(i).parent;
    }
    return false;
}

QT_END_NAMESPACE
#endif // QT_NO_CSSPARSER

#endif // QT_NO_TEXTHTMLPARSER




