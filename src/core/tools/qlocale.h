/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
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

#ifndef QLOCALE_H
#define QLOCALE_H

#include <QtCore/qvariant.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QDataStream;
class QDate;
class QDateTime;
class QTime;
class QVariant;
class QTextStream;
class QTextStreamPrivate;

class QLocale;

#ifndef QT_NO_SYSTEMLOCALE
class Q_CORE_EXPORT QSystemLocale
{
public:
    QSystemLocale();
    virtual ~QSystemLocale();

    struct CurrencyToStringArgument
    {
        CurrencyToStringArgument() { }
        CurrencyToStringArgument(const QVariant &v, const QString &s)
            : value(v), symbol(s) { }
        QVariant value;
        QString symbol;
    };

    enum QueryType {
        LanguageId, // uint
        CountryId, // uint
        DecimalPoint, // QString
        GroupSeparator, // QString
        ZeroDigit, // QString
        NegativeSign, // QString
        DateFormatLong, // QString
        DateFormatShort, // QString
        TimeFormatLong, // QString
        TimeFormatShort, // QString
        DayNameLong, // QString, in: int
        DayNameShort, // QString, in: int
        MonthNameLong, // QString, in: int
        MonthNameShort, // QString, in: int
        DateToStringLong, // QString, in: QDate
        DateToStringShort, // QString in: QDate
        TimeToStringLong, // QString in: QTime
        TimeToStringShort, // QString in: QTime
        DateTimeFormatLong, // QString
        DateTimeFormatShort, // QString
        DateTimeToStringLong, // QString in: QDateTime
        DateTimeToStringShort, // QString in: QDateTime
        MeasurementSystem, // uint
        PositiveSign, // QString
        AMText, // QString
        PMText, // QString
        FirstDayOfWeek, // Qt::DayOfWeek
        Weekdays, // QList<Qt::DayOfWeek>
        CurrencySymbol, // QString in: CurrencyToStringArgument
        CurrencyToString, // QString in: qlonglong, qulonglong or double
        UILanguages, // QStringList
        StringToStandardQuotation, // QString in: QStringRef to quote
        StringToAlternateQuotation, // QString in: QStringRef to quote
        ScriptId, // uint
        ListToSeparatedString, // QString
        LocaleChanged, // system locale changed
        NativeLanguageName, // QString
        NativeCountryName, // QString
        StandaloneMonthNameLong, // QString, in: int
        StandaloneMonthNameShort // QString, in: int
    };
    virtual QVariant query(QueryType type, QVariant in) const;
    virtual QLocale fallbackLocale() const;

private:
    QSystemLocale(bool);
    friend class QSystemLocaleSingleton;
};
#endif // QT_NO_SYSTEMLOCALE

struct QLocalePrivate;
class Q_CORE_EXPORT QLocale
{
    Q_GADGET
    Q_ENUMS(Language)
    Q_ENUMS(Country)
    friend class QString;
    friend class QByteArray;
    friend class QIntValidator;
    friend class QDoubleValidatorPrivate;
    friend class QTextStream;
    friend class QTextStreamPrivate;

public:
// GENERATED PART STARTS HERE
// see qlocale_data_p.h for more info on generated data
    enum Language {
        AnyLanguage = 0,
        C = 1,
        Abkhazian = 2,
        Achinese = 3,
        Acoli = 4,
        Adangme = 5,
        Adyghe = 6,
        Afar = 7,
        Afrihili = 8,
        Afrikaans = 9,
        Aghem = 10,
        Ainu = 11,
        Akan = 12,
        Akkadian = 13,
        Akoose = 14,
        Alabama = 15,
        Albanian = 16,
        Aleut = 17,
        AlgerianArabic = 18,
        AmericanEnglish = 19,
        AmericanSignLanguage = 20,
        Amharic = 21,
        AncientEgyptian = 22,
        AncientGreek = 23,
        Angika = 24,
        AoNaga = 25,
        Arabic = 26,
        Aragonese = 27,
        Aramaic = 28,
        Araona = 29,
        Arapaho = 30,
        Arawak = 31,
        Armenian = 32,
        Aromanian = 33,
        Arpitan = 34,
        Assamese = 35,
        Asturian = 36,
        Asu = 37,
        Atsam = 38,
        AustralianEnglish = 39,
        AustrianGerman = 40,
        Avaric = 41,
        Avestan = 42,
        Awadhi = 43,
        Aymara = 44,
        Azerbaijani = 45,
        Azeri = 46,
        Badaga = 47,
        Bafia = 48,
        Bafut = 49,
        Bakhtiari = 50,
        Balinese = 51,
        Baluchi = 52,
        Bambara = 53,
        Bamun = 54,
        Bangla = 55,
        Banjar = 56,
        Basaa = 57,
        Bashkir = 58,
        Basque = 59,
        BatakToba = 60,
        Bavarian = 61,
        Beja = 62,
        Belarusian = 63,
        Bemba = 64,
        Bena = 65,
        Betawi = 66,
        Bhojpuri = 67,
        Bikol = 68,
        Bini = 69,
        Bishnupriya = 70,
        Bislama = 71,
        Blin = 72,
        Blissymbols = 73,
        Bodo = 74,
        Bosnian = 75,
        Brahui = 76,
        Braj = 77,
        BrazilianPortuguese = 78,
        Breton = 79,
        BritishEnglish = 80,
        Buginese = 81,
        Bulgarian = 82,
        Bulu = 83,
        Buriat = 84,
        Burmese = 85,
        Caddo = 86,
        CajunFrench = 87,
        CanadianEnglish = 88,
        CanadianFrench = 89,
        Cantonese = 90,
        Capiznon = 91,
        Carib = 92,
        Catalan = 93,
        Cayuga = 94,
        Cebuano = 95,
        CentralAtlasTamazight = 96,
        CentralDusun = 97,
        CentralKurdish = 98,
        CentralYupik = 99,
        ChadianArabic = 100,
        Chagatai = 101,
        Chakma = 102,
        Chamorro = 103,
        Chechen = 104,
        Cherokee = 105,
        Cheyenne = 106,
        Chibcha = 107,
        Chiga = 108,
        ChimborazoHighlandQuichua = 109,
        Chinese = 110,
        ChinookJargon = 111,
        Chipewyan = 112,
        Choctaw = 113,
        ChurchSlavic = 114,
        Chuukese = 115,
        Chuvash = 116,
        ClassicalNewari = 117,
        ClassicalSyriac = 118,
        Colognian = 119,
        Comorian = 120,
        CongoSwahili = 121,
        Coptic = 122,
        Cornish = 123,
        Corsican = 124,
        Cree = 125,
        Creek = 126,
        CrimeanTurkish = 127,
        Croatian = 128,
        Czech = 129,
        Dakota = 130,
        Danish = 131,
        Dargwa = 132,
        Dari = 133,
        Dazaga = 134,
        Delaware = 135,
        Dinka = 136,
        Divehi = 137,
        Dogri = 138,
        Dogrib = 139,
        Duala = 140,
        Dutch = 141,
        Dyula = 142,
        Dzongkha = 143,
        EasternFrisian = 144,
        Efik = 145,
        EgyptianArabic = 146,
        Ekajuk = 147,
        Elamite = 148,
        Embu = 149,
        Emilian = 150,
        English = 151,
        Erzya = 152,
        Esperanto = 153,
        Estonian = 154,
        EuropeanPortuguese = 155,
        EuropeanSpanish = 156,
        Ewe = 157,
        Ewondo = 158,
        Extremaduran = 159,
        Fang = 160,
        Fanti = 161,
        Faroese = 162,
        FijiHindi = 163,
        Fijian = 164,
        Filipino = 165,
        Finnish = 166,
        Flemish = 167,
        Fon = 168,
        Frafra = 169,
        French = 170,
        Friulian = 171,
        Fulah = 172,
        Ga = 173,
        Gagauz = 174,
        Galician = 175,
        GanChinese = 176,
        Ganda = 177,
        Gayo = 178,
        Gbaya = 179,
        Geez = 180,
        Georgian = 181,
        German = 182,
        GhegAlbanian = 183,
        Ghomala = 184,
        Gilaki = 185,
        Gilbertese = 186,
        GoanKonkani = 187,
        Gondi = 188,
        Gorontalo = 189,
        Gothic = 190,
        Grebo = 191,
        Greek = 192,
        Guarani = 193,
        Gujarati = 194,
        Gusii = 195,
        Gwichin = 196,
        Haida = 197,
        HaitianCreole = 198,
        HakkaChinese = 199,
        Hausa = 200,
        Hawaiian = 201,
        Hebrew = 202,
        Herero = 203,
        Hiligaynon = 204,
        Hindi = 205,
        HiriMotu = 206,
        Hittite = 207,
        Hmong = 208,
        Hungarian = 209,
        Hupa = 210,
        Iban = 211,
        Ibibio = 212,
        Icelandic = 213,
        Ido = 214,
        Igbo = 215,
        Iloko = 216,
        InariSami = 217,
        Indonesian = 218,
        Ingrian = 219,
        Ingush = 220,
        Interlingua = 221,
        Interlingue = 222,
        Inuktitut = 223,
        Inupiaq = 224,
        Irish = 225,
        Italian = 226,
        JamaicanCreoleEnglish = 227,
        Japanese = 228,
        Javanese = 229,
        Jju = 230,
        JolaFonyi = 231,
        JudeoArabic = 232,
        JudeoPersian = 233,
        Jutish = 234,
        Kabardian = 235,
        Kabuverdianu = 236,
        Kabyle = 237,
        Kachin = 238,
        Kaingang = 239,
        Kako = 240,
        Kalaallisut = 241,
        Kalenjin = 242,
        Kalmyk = 243,
        Kamba = 244,
        Kanembu = 245,
        Kannada = 246,
        Kanuri = 247,
        KaraKalpak = 248,
        KarachayBalkar = 249,
        Karelian = 250,
        Kashmiri = 251,
        Kashubian = 252,
        Kawi = 253,
        Kazakh = 254,
        Kenyang = 255,
        Khasi = 256,
        Khmer = 257,
        Khotanese = 258,
        Khowar = 259,
        Kiche = 260,
        Kikuyu = 261,
        Kimbundu = 262,
        Kinaraya = 263,
        Kinyarwanda = 264,
        Kirghiz = 265,
        Kirmanjki = 266,
        Klingon = 267,
        Kom = 268,
        Komi = 269,
        KomiPermyak = 270,
        Kongo = 271,
        Konkani = 272,
        Korean = 273,
        Koro = 274,
        Kosraean = 275,
        Kotava = 276,
        KoyraChiini = 277,
        KoyraboroSenni = 278,
        Kpelle = 279,
        Krio = 280,
        Kuanyama = 281,
        Kumyk = 282,
        Kurdish = 283,
        Kurukh = 284,
        Kutenai = 285,
        Kwasio = 286,
        Kyrgyz = 287,
        Ladino = 288,
        Lahnda = 289,
        Lakota = 290,
        Lamba = 291,
        Langi = 292,
        Lao = 293,
        Latgalian = 294,
        Latin = 295,
        LatinAmericanSpanish = 296,
        Latvian = 297,
        Laz = 298,
        Lezghian = 299,
        Ligurian = 300,
        Limburgish = 301,
        Lingala = 302,
        LinguaFrancaNova = 303,
        LiteraryChinese = 304,
        Lithuanian = 305,
        Livonian = 306,
        Lojban = 307,
        Lombard = 308,
        LouisianaCreole = 309,
        LowGerman = 310,
        LowSaxon = 311,
        LowerSilesian = 312,
        LowerSorbian = 313,
        Lozi = 314,
        LubaKatanga = 315,
        LubaLulua = 316,
        Luiseno = 317,
        LuleSami = 318,
        Lunda = 319,
        Luo = 320,
        Luxembourgish = 321,
        Luyia = 322,
        Maba = 323,
        Macedonian = 324,
        Machame = 325,
        Madurese = 326,
        Mafa = 327,
        Magahi = 328,
        MainFranconian = 329,
        Maithili = 330,
        Makasar = 331,
        MakhuwaMeetto = 332,
        Makonde = 333,
        Malagasy = 334,
        Malay = 335,
        Malayalam = 336,
        Maltese = 337,
        Manchu = 338,
        Mandar = 339,
        MandarinChinese = 340,
        Mandingo = 341,
        Manipuri = 342,
        Manx = 343,
        Maori = 344,
        Mapuche = 345,
        Marathi = 346,
        Mari = 347,
        Marshallese = 348,
        Marwari = 349,
        Masai = 350,
        Mazanderani = 351,
        Medumba = 352,
        Mende = 353,
        Mentawai = 354,
        Meru = 355,
        Meta = 356,
        MexicanSpanish = 357,
        MiddleDutch = 358,
        MiddleEnglish = 359,
        MiddleFrench = 360,
        MiddleHighGerman = 361,
        MiddleIrish = 362,
        Mikmaq = 363,
        MinNanChinese = 364,
        Minangkabau = 365,
        Mingrelian = 366,
        Mirandese = 367,
        Mizo = 368,
        ModernStandardArabic = 369,
        Mohawk = 370,
        Moksha = 371,
        Moldavian = 372,
        Mongo = 373,
        Mongolian = 374,
        Montenegrin = 375,
        Morisyen = 376,
        MoroccanArabic = 377,
        Mossi = 378,
        Multiplelanguages = 379,
        Mundang = 380,
        MuslimTat = 381,
        MyanmarLanguage = 382,
        Myene = 383,
        NKo = 384,
        NajdiArabic = 385,
        Nama = 386,
        NauruLanguage = 387,
        Navajo = 388,
        Ndonga = 389,
        Neapolitan = 390,
        Nepali = 391,
        Newari = 392,
        Ngambay = 393,
        Ngiemboon = 394,
        Ngomba = 395,
        Nheengatu = 396,
        Nias = 397,
        NigerianPidgin = 398,
        Niuean = 399,
        Nogai = 400,
        Nolinguisticcontent = 401,
        NorthNdebele = 402,
        NorthernFrisian = 403,
        NorthernLuri = 404,
        NorthernSami = 405,
        NorthernSotho = 406,
        Norwegian = 407,
        NorwegianBokmal = 408,
        NorwegianNynorsk = 409,
        Novial = 410,
        Nuer = 411,
        Nyamwezi = 412,
        Nyanja = 413,
        Nyankole = 414,
        NyasaTonga = 415,
        Nyoro = 416,
        Nzima = 417,
        Occitan = 418,
        Odia = 419,
        Ojibwa = 420,
        OldEnglish = 421,
        OldFrench = 422,
        OldHighGerman = 423,
        OldIrish = 424,
        OldNorse = 425,
        OldPersian = 426,
        OldProvencal = 427,
        Oromo = 428,
        Osage = 429,
        Ossetic = 430,
        OttomanTurkish = 431,
        Pahlavi = 432,
        PalatineGerman = 433,
        Palauan = 434,
        Pali = 435,
        Pampanga = 436,
        Pangasinan = 437,
        Papiamento = 438,
        Pashto = 439,
        PennsylvaniaGerman = 440,
        Persian = 441,
        Phoenician = 442,
        Picard = 443,
        Piedmontese = 444,
        Plautdietsch = 445,
        Pohnpeian = 446,
        Polish = 447,
        Pontic = 448,
        Portuguese = 449,
        Prussian = 450,
        Punjabi = 451,
        Pushto = 452,
        Quechua = 453,
        Rajasthani = 454,
        Rapanui = 455,
        Rarotongan = 456,
        Riffian = 457,
        Romagnol = 458,
        Romanian = 459,
        Romansh = 460,
        Romany = 461,
        Rombo = 462,
        Root = 463,
        Rotuman = 464,
        Roviana = 465,
        Rundi = 466,
        Russian = 467,
        Rusyn = 468,
        Rwa = 469,
        Saho = 470,
        Sakha = 471,
        SamaritanAramaic = 472,
        Samburu = 473,
        Samoan = 474,
        Samogitian = 475,
        Sandawe = 476,
        Sango = 477,
        Sangu = 478,
        Sanskrit = 479,
        Santali = 480,
        Sardinian = 481,
        Sasak = 482,
        SassareseSardinian = 483,
        SaterlandFrisian = 484,
        Saurashtra = 485,
        Scots = 486,
        ScottishGaelic = 487,
        Selayar = 488,
        Selkup = 489,
        Sena = 490,
        Seneca = 491,
        Serbian = 492,
        SerboCroatian = 493,
        Serer = 494,
        Seri = 495,
        SeselwaCreoleFrench = 496,
        Shambala = 497,
        Shan = 498,
        Shona = 499,
        SichuanYi = 500,
        Sicilian = 501,
        Sidamo = 502,
        Siksika = 503,
        Silesian = 504,
        SimplifiedChinese = 505,
        Sindhi = 506,
        Sinhala = 507,
        SkoltSami = 508,
        Slave = 509,
        Slovak = 510,
        Slovenian = 511,
        Soga = 512,
        Sogdien = 513,
        Somali = 514,
        Soninke = 515,
        SouthNdebele = 516,
        SouthernAltai = 517,
        SouthernKurdish = 518,
        SouthernSami = 519,
        SouthernSotho = 520,
        Spanish = 521,
        SrananTongo = 522,
        StandardMoroccanTamazight = 523,
        Sukuma = 524,
        Sumerian = 525,
        Sundanese = 526,
        Susu = 527,
        Swahili = 528,
        Swati = 529,
        Swedish = 530,
        SwissFrench = 531,
        SwissGerman = 532,
        SwissHighGerman = 533,
        Syriac = 534,
        Tachelhit = 535,
        Tagalog = 536,
        Tahitian = 537,
        Taita = 538,
        Tajik = 539,
        Talysh = 540,
        Tamashek = 541,
        Tamil = 542,
        Taroko = 543,
        Tasawaq = 544,
        Tatar = 545,
        Telugu = 546,
        Tereno = 547,
        Teso = 548,
        Tetum = 549,
        Thai = 550,
        Tibetan = 551,
        Tigre = 552,
        Tigrinya = 553,
        Timne = 554,
        Tiv = 555,
        Tlingit = 556,
        TokPisin = 557,
        TokelauLanguage = 558,
        Tongan = 559,
        TornedalenFinnish = 560,
        TraditionalChinese = 561,
        Tsakhur = 562,
        Tsakonian = 563,
        Tsimshian = 564,
        Tsonga = 565,
        Tswana = 566,
        Tulu = 567,
        Tumbuka = 568,
        TunisianArabic = 569,
        Turkish = 570,
        Turkmen = 571,
        Turoyo = 572,
        TuvaluLanguage = 573,
        Tuvinian = 574,
        Twi = 575,
        Tyap = 576,
        UKEnglish = 577,
        USEnglish = 578,
        Udmurt = 579,
        Ugaritic = 580,
        Uighur = 581,
        Ukrainian = 582,
        Umbundu = 583,
        Unknownlanguage = 584,
        UpperSorbian = 585,
        Urdu = 586,
        Uyghur = 587,
        Uzbek = 588,
        Vai = 589,
        Venda = 590,
        Venetian = 591,
        Veps = 592,
        Vietnamese = 593,
        Volapuk = 594,
        Voro = 595,
        Votic = 596,
        Vunjo = 597,
        Walloon = 598,
        Walser = 599,
        Waray = 600,
        Warlpiri = 601,
        Washo = 602,
        Wayuu = 603,
        Welsh = 604,
        WestFlemish = 605,
        WesternBalochi = 606,
        WesternFrisian = 607,
        WesternMari = 608,
        Wolaytta = 609,
        Wolof = 610,
        WuChinese = 611,
        Xhosa = 612,
        XiangChinese = 613,
        Yangben = 614,
        Yao = 615,
        Yapese = 616,
        Yemba = 617,
        Yiddish = 618,
        Yoruba = 619,
        Zapotec = 620,
        Zarma = 621,
        Zaza = 622,
        Zeelandic = 623,
        Zenaga = 624,
        Zhuang = 625,
        ZoroastrianDari = 626,
        Zulu = 627,
        Zuni = 628,

        LastLanguage = Zuni
    };

    enum Script {
        AnyScript = 0,
        AdlamScript = 1,
        AfakaScript = 2,
        AhomScript = 3,
        AnatolianHieroglyphsScript = 4,
        ArabicScript = 5,
        ArmenianScript = 6,
        AvestanScript = 7,
        BalineseScript = 8,
        BamumScript = 9,
        BanglaScript = 10,
        BassaVahScript = 11,
        BatakScript = 12,
        BhaiksukiScript = 13,
        BlissymbolsScript = 14,
        BookPahlaviScript = 15,
        BopomofoScript = 16,
        BrahmiScript = 17,
        BrailleScript = 18,
        BugineseScript = 19,
        BuhidScript = 20,
        CarianScript = 21,
        CaucasianAlbanianScript = 22,
        ChakmaScript = 23,
        ChamScript = 24,
        CherokeeScript = 25,
        CirthScript = 26,
        CopticScript = 27,
        CypriotScript = 28,
        CyrillicScript = 29,
        DeseretScript = 30,
        DevanagariScript = 31,
        DograScript = 32,
        DuployanshorthandScript = 33,
        EasternSyriacScript = 34,
        EgyptiandemoticScript = 35,
        EgyptianhieraticScript = 36,
        EgyptianhieroglyphsScript = 37,
        ElbasanScript = 38,
        ElymaicScript = 39,
        EmojiScript = 40,
        EstrangeloSyriacScript = 41,
        EthiopicScript = 42,
        FrakturLatinScript = 43,
        FraserScript = 44,
        GaelicLatinScript = 45,
        GeorgianKhutsuriScript = 46,
        GeorgianScript = 47,
        GlagoliticScript = 48,
        GothicScript = 49,
        GranthaScript = 50,
        GreekScript = 51,
        GujaratiScript = 52,
        GunjalaGondiScript = 53,
        GurmukhiScript = 54,
        HanScript = 55,
        HangulScript = 56,
        HanifiRohingyaScript = 57,
        HanunooScript = 58,
        HanwithBopomofoScript = 59,
        HatranScript = 60,
        HebrewScript = 61,
        HiraganaScript = 62,
        ImperialAramaicScript = 63,
        IndusScript = 64,
        InheritedScript = 65,
        InscriptionalPahlaviScript = 66,
        InscriptionalParthianScript = 67,
        JamoScript = 68,
        JapaneseScript = 69,
        JapanesesyllabariesScript = 70,
        JavaneseScript = 71,
        JurchenScript = 72,
        KaithiScript = 73,
        KannadaScript = 74,
        KatakanaScript = 75,
        KayahLiScript = 76,
        KharoshthiScript = 77,
        KhmerScript = 78,
        KhojkiScript = 79,
        KhudawadiScript = 80,
        KoreanScript = 81,
        KpelleScript = 82,
        LannaScript = 83,
        LaoScript = 84,
        LatinScript = 85,
        LepchaScript = 86,
        LimbuScript = 87,
        LinearAScript = 88,
        LinearBScript = 89,
        LomaScript = 90,
        LycianScript = 91,
        LydianScript = 92,
        MahajaniScript = 93,
        MakasarScript = 94,
        MalayalamScript = 95,
        MandaeanScript = 96,
        ManichaeanScript = 97,
        MarchenScript = 98,
        MasaramGondiScript = 99,
        MathematicalNotationScript = 100,
        MayanhieroglyphsScript = 101,
        MedefaidrinScript = 102,
        MeiteiMayekScript = 103,
        MendeScript = 104,
        MeroiticCursiveScript = 105,
        MeroiticScript = 106,
        ModiScript = 107,
        MongolianScript = 108,
        MoonScript = 109,
        MroScript = 110,
        MultaniScript = 111,
        MyanmarScript = 112,
        NKoScript = 113,
        NabataeanScript = 114,
        NandinagariScript = 115,
        NaxiGebaScript = 116,
        NewTaiLueScript = 117,
        NewaScript = 118,
        NushuScript = 119,
        NyiakengPuachueHmongScript = 120,
        OdiaScript = 121,
        OghamScript = 122,
        OlChikiScript = 123,
        OldChurchSlavonicCyrillicScript = 124,
        OldHungarianScript = 125,
        OldItalicScript = 126,
        OldNorthArabianScript = 127,
        OldPermicScript = 128,
        OldPersianScript = 129,
        OldSogdianScript = 130,
        OldSouthArabianScript = 131,
        OrkhonScript = 132,
        OsageScript = 133,
        OsmanyaScript = 134,
        PahawhHmongScript = 135,
        PalmyreneScript = 136,
        PauCinHauScript = 137,
        PersoArabicScript = 138,
        PhagspaScript = 139,
        PhoenicianScript = 140,
        PollardPhoneticScript = 141,
        PsalterPahlaviScript = 142,
        RejangScript = 143,
        RongorongoScript = 144,
        RunicScript = 145,
        SACuneiformScript = 146,
        SamaritanScript = 147,
        SaratiScript = 148,
        SaurashtraScript = 149,
        SharadaScript = 150,
        ShavianScript = 151,
        SiddhamScript = 152,
        SignWritingScript = 153,
        SimplifiedHanScript = 154,
        SimplifiedScript = 155,
        SinhalaScript = 156,
        SogdianScript = 157,
        SoraSompengScript = 158,
        SoyomboScript = 159,
        SumeroAkkadianCuneiformScript = 160,
        SundaneseScript = 161,
        SylotiNagriScript = 162,
        SymbolsScript = 163,
        SyriacScript = 164,
        TagalogScript = 165,
        TagbanwaScript = 166,
        TaiLeScript = 167,
        TaiVietScript = 168,
        TakriScript = 169,
        TamilScript = 170,
        TangutScript = 171,
        TeluguScript = 172,
        TengwarScript = 173,
        ThaanaScript = 174,
        ThaiScript = 175,
        TibetanScript = 176,
        TifinaghScript = 177,
        TirhutaScript = 178,
        TraditionalHanScript = 179,
        TraditionalScript = 180,
        UCASScript = 181,
        UgariticScript = 182,
        UnifiedCanadianAboriginalSyllabicsScript = 183,
        UnwrittenScript = 184,
        VaiScript = 185,
        VarangKshitiScript = 186,
        VisibleSpeechScript = 187,
        WanchoScript = 188,
        WesternSyriacScript = 189,
        WoleaiScript = 190,
        YiScript = 191,
        ZanabazarSquareScript = 192,

        LastScript = ZanabazarSquareScript
    };
    enum Country {
        AnyCountry = 0,
        Afghanistan = 1,
        Africa = 2,
        AlandIslands = 3,
        Albania = 4,
        Algeria = 5,
        AmericanSamoa = 6,
        Americas = 7,
        Andorra = 8,
        Angola = 9,
        Anguilla = 10,
        Antarctica = 11,
        AntiguaAndBarbuda = 12,
        Argentina = 13,
        Armenia = 14,
        Aruba = 15,
        AscensionIsland = 16,
        Asia = 17,
        Australasia = 18,
        Australia = 19,
        Austria = 20,
        Azerbaijan = 21,
        Bahamas = 22,
        Bahrain = 23,
        Bangladesh = 24,
        Barbados = 25,
        Belarus = 26,
        Belgium = 27,
        Belize = 28,
        Benin = 29,
        Bermuda = 30,
        Bhutan = 31,
        Bolivia = 32,
        Bosnia = 33,
        BosniaAndHerzegovina = 34,
        Botswana = 35,
        BouvetIsland = 36,
        Brazil = 37,
        BritishIndianOceanTerritory = 38,
        BritishVirginIslands = 39,
        Brunei = 40,
        Bulgaria = 41,
        BurkinaFaso = 42,
        Burundi = 43,
        Cambodia = 44,
        Cameroon = 45,
        Canada = 46,
        CanaryIslands = 47,
        CapeVerde = 48,
        Caribbean = 49,
        CaribbeanNetherlands = 50,
        CaymanIslands = 51,
        CentralAfricanRepublic = 52,
        CentralAmerica = 53,
        CentralAsia = 54,
        CeutaAndMelilla = 55,
        Chad = 56,
        Chile = 57,
        China = 58,
        ChristmasIsland = 59,
        ClippertonIsland = 60,
        CocosKeelingIslands = 61,
        Colombia = 62,
        Comoros = 63,
        CongoBrazzaville = 64,
        CongoDRC = 65,
        CongoKinshasa = 66,
        CongoRepublic = 67,
        CookIslands = 68,
        CostaRica = 69,
        CotedIvoire = 70,
        Croatia = 71,
        Cuba = 72,
        Curacao = 73,
        Cyprus = 74,
        CzechRepublic = 75,
        Czechia = 76,
        Denmark = 77,
        DiegoGarcia = 78,
        Djibouti = 79,
        Dominica = 80,
        DominicanRepublic = 81,
        EastTimor = 82,
        EasternAfrica = 83,
        EasternAsia = 84,
        EasternEurope = 85,
        Ecuador = 86,
        Egypt = 87,
        ElSalvador = 88,
        EquatorialGuinea = 89,
        Eritrea = 90,
        Estonia = 91,
        Eswatini = 92,
        Ethiopia = 93,
        Europe = 94,
        EuropeanUnion = 95,
        Eurozone = 96,
        FalklandIslands = 97,
        FalklandIslandsIslasMalvinas = 98,
        FaroeIslands = 99,
        Fiji = 100,
        Finland = 101,
        France = 102,
        FrenchGuiana = 103,
        FrenchPolynesia = 104,
        FrenchSouthernTerritories = 105,
        Gabon = 106,
        Gambia = 107,
        Georgia = 108,
        Germany = 109,
        Ghana = 110,
        Gibraltar = 111,
        Greece = 112,
        Greenland = 113,
        Grenada = 114,
        Guadeloupe = 115,
        Guam = 116,
        Guatemala = 117,
        Guernsey = 118,
        Guinea = 119,
        GuineaBissau = 120,
        Guyana = 121,
        Haiti = 122,
        HeardAndMcDonaldIslands = 123,
        Honduras = 124,
        HongKong = 125,
        HongKongSARChina = 126,
        Hungary = 127,
        Iceland = 128,
        India = 129,
        Indonesia = 130,
        Iran = 131,
        Iraq = 132,
        Ireland = 133,
        IsleofMan = 134,
        Israel = 135,
        Italy = 136,
        IvoryCoast = 137,
        Jamaica = 138,
        Japan = 139,
        Jersey = 140,
        Jordan = 141,
        Kazakhstan = 142,
        Kenya = 143,
        Kiribati = 144,
        Kosovo = 145,
        Kuwait = 146,
        Kyrgyzstan = 147,
        Laos = 148,
        LatinAmerica = 149,
        Latvia = 150,
        Lebanon = 151,
        Lesotho = 152,
        Liberia = 153,
        Libya = 154,
        Liechtenstein = 155,
        Lithuania = 156,
        Luxembourg = 157,
        Macao = 158,
        MacaoSARChina = 159,
        Madagascar = 160,
        Malawi = 161,
        Malaysia = 162,
        Maldives = 163,
        Mali = 164,
        Malta = 165,
        MarshallIslands = 166,
        Martinique = 167,
        Mauritania = 168,
        Mauritius = 169,
        Mayotte = 170,
        Melanesia = 171,
        Mexico = 172,
        Micronesia = 173,
        MicronesianRegion = 174,
        MiddleAfrica = 175,
        Moldova = 176,
        Monaco = 177,
        Mongolia = 178,
        Montenegro = 179,
        Montserrat = 180,
        Morocco = 181,
        Mozambique = 182,
        Myanmar = 183,
        MyanmarBurma = 184,
        Namibia = 185,
        Nauru = 186,
        Nepal = 187,
        Netherlands = 188,
        NewCaledonia = 189,
        NewZealand = 190,
        Nicaragua = 191,
        Niger = 192,
        Nigeria = 193,
        Niue = 194,
        NorfolkIsland = 195,
        NorthAmerica = 196,
        NorthKorea = 197,
        NorthMacedonia = 198,
        NorthernAfrica = 199,
        NorthernAmerica = 200,
        NorthernEurope = 201,
        NorthernMarianaIslands = 202,
        Norway = 203,
        Oceania = 204,
        Oman = 205,
        OutlyingOceania = 206,
        Pakistan = 207,
        Palau = 208,
        Palestine = 209,
        PalestinianTerritories = 210,
        Panama = 211,
        PapuaNewGuinea = 212,
        Paraguay = 213,
        Peru = 214,
        Philippines = 215,
        PitcairnIslands = 216,
        Poland = 217,
        Polynesia = 218,
        Portugal = 219,
        PseudoAccents = 220,
        PseudoBidi = 221,
        PuertoRico = 222,
        Qatar = 223,
        Reunion = 224,
        Romania = 225,
        Russia = 226,
        Rwanda = 227,
        Samoa = 228,
        SanMarino = 229,
        SaoTomeAndPrincipe = 230,
        SaudiArabia = 231,
        Senegal = 232,
        Serbia = 233,
        Seychelles = 234,
        SierraLeone = 235,
        Singapore = 236,
        SintMaarten = 237,
        Slovakia = 238,
        Slovenia = 239,
        SolomonIslands = 240,
        Somalia = 241,
        SouthAfrica = 242,
        SouthAmerica = 243,
        SouthGeorgiaAndSouthSandwichIslands = 244,
        SouthKorea = 245,
        SouthSudan = 246,
        SoutheastAsia = 247,
        SouthernAfrica = 248,
        SouthernAsia = 249,
        SouthernEurope = 250,
        Spain = 251,
        SriLanka = 252,
        StBarthelemy = 253,
        StHelena = 254,
        StKittsAndNevis = 255,
        StLucia = 256,
        StMartin = 257,
        StPierreAndMiquelon = 258,
        StVincentAndGrenadines = 259,
        SubSaharanAfrica = 260,
        Sudan = 261,
        Suriname = 262,
        SvalbardAndJanMayen = 263,
        Swaziland = 264,
        Sweden = 265,
        Switzerland = 266,
        Syria = 267,
        Taiwan = 268,
        Tajikistan = 269,
        Tanzania = 270,
        Thailand = 271,
        TimorLeste = 272,
        Togo = 273,
        Tokelau = 274,
        Tonga = 275,
        TrinidadAndTobago = 276,
        TristandaCunha = 277,
        Tunisia = 278,
        Turkey = 279,
        Turkmenistan = 280,
        TurksAndCaicosIslands = 281,
        Tuvalu = 282,
        UK = 283,
        UN = 284,
        US = 285,
        Uganda = 286,
        Ukraine = 287,
        UnitedArabEmirates = 288,
        UnitedKingdom = 289,
        UnitedNations = 290,
        UnitedStates = 291,
        UnitedStatesOutlyingIslands = 292,
        UnitedStatesVirginIslands = 293,
        UnknownRegion = 294,
        Uruguay = 295,
        Uzbekistan = 296,
        Vanuatu = 297,
        VaticanCity = 298,
        Venezuela = 299,
        Vietnam = 300,
        WallisAndFutuna = 301,
        WesternAfrica = 302,
        WesternAsia = 303,
        WesternEurope = 304,
        WesternSahara = 305,
        Yemen = 306,
        Zambia = 307,
        Zimbabwe = 308,

        LastCountry = Zimbabwe
    };
// GENERATED PART ENDS HERE

    enum MeasurementSystem { MetricSystem, ImperialSystem };

    enum FormatType { LongFormat, ShortFormat, NarrowFormat };
    enum NumberOption {
        OmitGroupSeparator = 0x01,
        RejectGroupSeparator = 0x02
    };
    Q_DECLARE_FLAGS(NumberOptions, NumberOption)

    enum CurrencySymbolFormat {
        CurrencyIsoCode,
        CurrencySymbol,
        CurrencyDisplayName
    };

    QLocale();
    QLocale(const QString &name);
    QLocale(Language language, Country country = AnyCountry);
    QLocale(Language language, Script script, Country country);
    QLocale(const QLocale &other);

    QLocale &operator=(const QLocale &other);

    Language language() const;
    Script script() const;
    Country country() const;
    QString name() const;

    QString bcp47Name() const;
    QString nativeLanguageName() const;
    QString nativeCountryName() const;

    short toShort(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    ushort toUShort(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    int toInt(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    uint toUInt(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    qlonglong toLongLong(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    qulonglong toULongLong(const QString &s, bool *ok = Q_NULLPTR, int base = 0) const;
    float toFloat(const QString &s, bool *ok = Q_NULLPTR) const;
    double toDouble(const QString &s, bool *ok = Q_NULLPTR) const;

    QString toString(qlonglong i) const;
    QString toString(qulonglong i) const;
    inline QString toString(short i) const;
    inline QString toString(ushort i) const;
    inline QString toString(int i) const;
    inline QString toString(uint i) const;
    QString toString(double i, char f = 'g', int prec = 6) const;
    inline QString toString(float i, char f = 'g', int prec = 6) const;
    QString toString(const QDate &date, const QString &formatStr) const;
    QString toString(const QDate &date, FormatType format = LongFormat) const;
    QString toString(const QTime &time, const QString &formatStr) const;
    QString toString(const QTime &time, FormatType format = LongFormat) const;
    QString toString(const QDateTime &dateTime, FormatType format = LongFormat) const;
    QString toString(const QDateTime &dateTime, const QString &format) const;

    QString dateFormat(FormatType format = LongFormat) const;
    QString timeFormat(FormatType format = LongFormat) const;
    QString dateTimeFormat(FormatType format = LongFormat) const;
#ifndef QT_NO_DATESTRING
    QDate toDate(const QString &string, FormatType = LongFormat) const;
    QTime toTime(const QString &string, FormatType = LongFormat) const;
    QDateTime toDateTime(const QString &string, FormatType format = LongFormat) const;
    QDate toDate(const QString &string, const QString &format) const;
    QTime toTime(const QString &string, const QString &format) const;
    QDateTime toDateTime(const QString &string, const QString &format) const;
#endif

    // ### Qt 5: We need to return QString from these function since
    //           unicode data contains several characters for these fields.
    QChar decimalPoint() const;
    QChar groupSeparator() const;
    QChar percent() const;
    QChar zeroDigit() const;
    QChar negativeSign() const;
    QChar positiveSign() const;
    QChar exponential() const;

    QString monthName(int, FormatType format = LongFormat) const;
    QString standaloneMonthName(int, FormatType format = LongFormat) const;
    QString dayName(int, FormatType format = LongFormat) const;
    QString standaloneDayName(int, FormatType format = LongFormat) const;

    Qt::DayOfWeek firstDayOfWeek() const;
    QList<Qt::DayOfWeek> weekdays() const;

    QString amText() const;
    QString pmText() const;

    MeasurementSystem measurementSystem() const;

    Qt::LayoutDirection textDirection() const;

    QString toUpper(const QString &str) const;
    QString toLower(const QString &str) const;

    QString currencySymbol(CurrencySymbolFormat = CurrencySymbol) const;
    QString toCurrencyString(qlonglong, const QString &symbol = QString()) const;
    QString toCurrencyString(qulonglong, const QString &symbol = QString()) const;
    inline QString toCurrencyString(short, const QString &symbol = QString()) const;
    inline QString toCurrencyString(ushort, const QString &symbol = QString()) const;
    inline QString toCurrencyString(int, const QString &symbol = QString()) const;
    inline QString toCurrencyString(uint, const QString &symbol = QString()) const;
    QString toCurrencyString(double, const QString &symbol = QString()) const;
    inline QString toCurrencyString(float, const QString &symbol = QString()) const;

    QStringList uiLanguages() const;

    inline bool operator==(const QLocale &other) const;
    inline bool operator!=(const QLocale &other) const;

    static QString languageToString(Language language);
    static QString countryToString(Country country);
    static QString scriptToString(Script script);
    static void setDefault(const QLocale &locale);

    static QLocale c() { return QLocale(C); }
    static QLocale system();

    static QList<QLocale> matchingLocales(QLocale::Language language, QLocale::Script script, QLocale::Country country);
    static QList<Country> countriesForLanguage(Language lang);

    void setNumberOptions(NumberOptions options);
    NumberOptions numberOptions() const;

    enum QuotationStyle { StandardQuotation, AlternateQuotation };
    QString quoteString(const QString &str, QuotationStyle style = StandardQuotation) const;
    QString quoteString(const QStringRef &str, QuotationStyle style = StandardQuotation) const;

    QString createSeparatedList(const QStringList &strl) const;
private:
    friend struct QLocalePrivate;
    // ### We now use this field to pack an index into locale_data and NumberOptions.
    // ### Qt 5: change to a QLocaleData *d; uint numberOptions.
    struct Data {
        quint16 index;
        quint16 numberOptions;
    };

    Data p;
    const QLocalePrivate *d() const;
};
Q_DECLARE_TYPEINFO(QLocale, Q_MOVABLE_TYPE);
Q_DECLARE_OPERATORS_FOR_FLAGS(QLocale::NumberOptions)

inline QString QLocale::toString(short i) const
    { return toString(qlonglong(i)); }
inline QString QLocale::toString(ushort i) const
    { return toString(qulonglong(i)); }
inline QString QLocale::toString(int i) const
    { return toString(qlonglong(i)); }
inline QString QLocale::toString(uint i) const
    { return toString(qulonglong(i)); }
inline QString QLocale::toString(float i, char f, int prec) const
    { return toString(double(i), f, prec); }
inline bool QLocale::operator==(const QLocale &other) const
    { return d() == other.d() && numberOptions() == other.numberOptions(); }
inline bool QLocale::operator!=(const QLocale &other) const
    { return d() != other.d() || numberOptions() != other.numberOptions(); }

inline QString QLocale::toCurrencyString(short i, const QString &symbol) const
    { return toCurrencyString(qlonglong(i), symbol); }
inline QString QLocale::toCurrencyString(ushort i, const QString &symbol) const
    { return toCurrencyString(qulonglong(i), symbol); }
inline QString QLocale::toCurrencyString(int i, const QString &symbol) const
{ return toCurrencyString(qlonglong(i), symbol); }
inline QString QLocale::toCurrencyString(uint i, const QString &symbol) const
{ return toCurrencyString(qulonglong(i), symbol); }
inline QString QLocale::toCurrencyString(float i, const QString &symbol) const
{ return toCurrencyString(double(i), symbol); }

#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QLocale &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QLocale &);
#endif

QT_END_NAMESPACE

#ifndef QT_NO_SYSTEMLOCALE
Q_DECLARE_METATYPE(QSystemLocale::CurrencyToStringArgument)
#endif

QT_END_HEADER

#endif // QLOCALE_H
