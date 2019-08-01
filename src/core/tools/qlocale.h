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
        Afar = 2,
        Abkhazian = 3,
        Achinese = 4,
        Acoli = 5,
        Adangme = 6,
        Adyghe = 7,
        Avestan = 8,
        TunisianArabic = 9,
        Afrikaans = 10,
        Afrihili = 11,
        Aghem = 12,
        Ainu = 13,
        Akan = 14,
        Akkadian = 15,
        Alabama = 16,
        Aleut = 17,
        GhegAlbanian = 18,
        SouthernAltai = 19,
        Amharic = 20,
        Aragonese = 21,
        OldEnglish = 22,
        Angika = 23,
        Arabic = 24,
        ModernStandardArabic = 25,
        Aramaic = 26,
        Mapuche = 27,
        Araona = 28,
        Arapaho = 29,
        AlgerianArabic = 30,
        NajdiArabic = 31,
        Arawak = 32,
        MoroccanArabic = 33,
        EgyptianArabic = 34,
        Assamese = 35,
        Asu = 36,
        AmericanSignLanguage = 37,
        Asturian = 38,
        Avaric = 39,
        Kotava = 40,
        Awadhi = 41,
        Aymara = 42,
        Azerbaijani = 43,
        Azeri = 44,
        Bashkir = 45,
        Baluchi = 46,
        Balinese = 47,
        Bavarian = 48,
        Basaa = 49,
        Bamun = 50,
        BatakToba = 51,
        Ghomala = 52,
        Belarusian = 53,
        Beja = 54,
        Bemba = 55,
        Betawi = 56,
        Bena = 57,
        Bafut = 58,
        Badaga = 59,
        Bulgarian = 60,
        WesternBalochi = 61,
        Bhojpuri = 62,
        Bislama = 63,
        Bikol = 64,
        Bini = 65,
        Banjar = 66,
        Kom = 67,
        Siksika = 68,
        Bambara = 69,
        Bangla = 70,
        Tibetan = 71,
        Bishnupriya = 72,
        Bakhtiari = 73,
        Breton = 74,
        Braj = 75,
        Brahui = 76,
        Bodo = 77,
        Bosnian = 78,
        Akoose = 79,
        Buriat = 80,
        Buginese = 81,
        Bulu = 82,
        Blin = 83,
        Medumba = 84,
        Catalan = 85,
        Caddo = 86,
        Carib = 87,
        Cayuga = 88,
        Atsam = 89,
        Chakma = 90,
        Chechen = 91,
        Cebuano = 92,
        Chiga = 93,
        Chamorro = 94,
        Chibcha = 95,
        Chagatai = 96,
        Chuukese = 97,
        Mari = 98,
        ChinookJargon = 99,
        Choctaw = 100,
        Chipewyan = 101,
        Cherokee = 102,
        Cheyenne = 103,
        CentralKurdish = 104,
        Corsican = 105,
        Coptic = 106,
        Capiznon = 107,
        Cree = 108,
        CrimeanTurkish = 109,
        SeselwaCreoleFrench = 110,
        Czech = 111,
        Kashubian = 112,
        ChurchSlavic = 113,
        Chuvash = 114,
        Welsh = 115,
        Danish = 116,
        Dakota = 117,
        Dargwa = 118,
        Taita = 119,
        German = 120,
        AustrianGerman = 121,
        SwissHighGerman = 122,
        Delaware = 123,
        Slave = 124,
        Dogrib = 125,
        Dinka = 126,
        Zarma = 127,
        Dogri = 128,
        LowerSorbian = 129,
        CentralDusun = 130,
        Duala = 131,
        MiddleDutch = 132,
        Divehi = 133,
        JolaFonyi = 134,
        Dyula = 135,
        Dzongkha = 136,
        Dazaga = 137,
        Embu = 138,
        Ewe = 139,
        Efik = 140,
        Emilian = 141,
        AncientEgyptian = 142,
        Ekajuk = 143,
        Greek = 144,
        Elamite = 145,
        English = 146,
        AustralianEnglish = 147,
        CanadianEnglish = 148,
        BritishEnglish = 149,
        UKEnglish = 150,
        AmericanEnglish = 151,
        USEnglish = 152,
        MiddleEnglish = 153,
        Esperanto = 154,
        Spanish = 155,
        LatinAmericanSpanish = 156,
        EuropeanSpanish = 157,
        MexicanSpanish = 158,
        CentralYupik = 159,
        Estonian = 160,
        Basque = 161,
        Ewondo = 162,
        Extremaduran = 163,
        Persian = 164,
        Dari = 165,
        Fang = 166,
        Fanti = 167,
        Fulah = 168,
        Finnish = 169,
        Filipino = 170,
        TornedalenFinnish = 171,
        Fijian = 172,
        Faroese = 173,
        Fon = 174,
        French = 175,
        CanadianFrench = 176,
        SwissFrench = 177,
        CajunFrench = 178,
        MiddleFrench = 179,
        OldFrench = 180,
        Arpitan = 181,
        NorthernFrisian = 182,
        EasternFrisian = 183,
        Friulian = 184,
        WesternFrisian = 185,
        Irish = 186,
        Ga = 187,
        Gagauz = 188,
        GanChinese = 189,
        Gayo = 190,
        Gbaya = 191,
        ZoroastrianDari = 192,
        ScottishGaelic = 193,
        Geez = 194,
        Gilbertese = 195,
        Galician = 196,
        Gilaki = 197,
        MiddleHighGerman = 198,
        Guarani = 199,
        OldHighGerman = 200,
        GoanKonkani = 201,
        Gondi = 202,
        Gorontalo = 203,
        Gothic = 204,
        Grebo = 205,
        AncientGreek = 206,
        SwissGerman = 207,
        Gujarati = 208,
        Wayuu = 209,
        Frafra = 210,
        Gusii = 211,
        Manx = 212,
        Gwichin = 213,
        Hausa = 214,
        Haida = 215,
        HakkaChinese = 216,
        Hawaiian = 217,
        Hebrew = 218,
        Hindi = 219,
        FijiHindi = 220,
        Hiligaynon = 221,
        Hittite = 222,
        Hmong = 223,
        HiriMotu = 224,
        Croatian = 225,
        UpperSorbian = 226,
        XiangChinese = 227,
        HaitianCreole = 228,
        Hungarian = 229,
        Hupa = 230,
        Armenian = 231,
        Herero = 232,
        Interlingua = 233,
        Iban = 234,
        Ibibio = 235,
        Indonesian = 236,
        Interlingue = 237,
        Igbo = 238,
        SichuanYi = 239,
        Inupiaq = 240,
        Iloko = 241,
        Ingush = 242,
        Ido = 243,
        Icelandic = 244,
        Italian = 245,
        Inuktitut = 246,
        Ingrian = 247,
        Japanese = 248,
        JamaicanCreoleEnglish = 249,
        Lojban = 250,
        Ngomba = 251,
        Machame = 252,
        JudeoPersian = 253,
        JudeoArabic = 254,
        Jutish = 255,
        Javanese = 256,
        Georgian = 257,
        KaraKalpak = 258,
        Kabyle = 259,
        Kachin = 260,
        Jju = 261,
        Kamba = 262,
        Kawi = 263,
        Kabardian = 264,
        Kanembu = 265,
        Tyap = 266,
        Makonde = 267,
        Kabuverdianu = 268,
        Kenyang = 269,
        Koro = 270,
        Kongo = 271,
        Kaingang = 272,
        Khasi = 273,
        Khotanese = 274,
        KoyraChiini = 275,
        Khowar = 276,
        Kikuyu = 277,
        Kirmanjki = 278,
        Kuanyama = 279,
        Kazakh = 280,
        Kako = 281,
        Kalaallisut = 282,
        Kalenjin = 283,
        Khmer = 284,
        Kimbundu = 285,
        Kannada = 286,
        Korean = 287,
        KomiPermyak = 288,
        Konkani = 289,
        Kosraean = 290,
        Kpelle = 291,
        Kanuri = 292,
        KarachayBalkar = 293,
        Krio = 294,
        Kinaraya = 295,
        Karelian = 296,
        Kurukh = 297,
        Kashmiri = 298,
        Shambala = 299,
        Bafia = 300,
        Colognian = 301,
        Kurdish = 302,
        Kumyk = 303,
        Kutenai = 304,
        Komi = 305,
        Cornish = 306,
        Kyrgyz = 307,
        Kirghiz = 308,
        Latin = 309,
        Ladino = 310,
        Langi = 311,
        Lahnda = 312,
        Lamba = 313,
        Luxembourgish = 314,
        Lezghian = 315,
        LinguaFrancaNova = 316,
        Ganda = 317,
        Limburgish = 318,
        Ligurian = 319,
        Livonian = 320,
        Lakota = 321,
        Lombard = 322,
        Lingala = 323,
        Lao = 324,
        Mongo = 325,
        LouisianaCreole = 326,
        Lozi = 327,
        NorthernLuri = 328,
        Lithuanian = 329,
        Latgalian = 330,
        LubaKatanga = 331,
        LubaLulua = 332,
        Luiseno = 333,
        Lunda = 334,
        Luo = 335,
        Mizo = 336,
        Luyia = 337,
        Latvian = 338,
        LiteraryChinese = 339,
        Laz = 340,
        Madurese = 341,
        Mafa = 342,
        Magahi = 343,
        Maithili = 344,
        Makasar = 345,
        Mandingo = 346,
        Masai = 347,
        Maba = 348,
        Moksha = 349,
        Mandar = 350,
        Mende = 351,
        Meru = 352,
        Morisyen = 353,
        Malagasy = 354,
        MiddleIrish = 355,
        MakhuwaMeetto = 356,
        Meta = 357,
        Marshallese = 358,
        Maori = 359,
        Mikmaq = 360,
        Minangkabau = 361,
        Macedonian = 362,
        Malayalam = 363,
        Mongolian = 364,
        Manchu = 365,
        Manipuri = 366,
        Mohawk = 367,
        Mossi = 368,
        Marathi = 369,
        WesternMari = 370,
        Malay = 371,
        Maltese = 372,
        Mundang = 373,
        Multiplelanguages = 374,
        Creek = 375,
        Mirandese = 376,
        Marwari = 377,
        Mentawai = 378,
        Burmese = 379,
        MyanmarLanguage = 380,
        Myene = 381,
        Erzya = 382,
        Mazanderani = 383,
        Nauru = 384,
        MinNanChinese = 385,
        Neapolitan = 386,
        Nama = 387,
        NorwegianBokmal = 388,
        NorthNdebele = 389,
        LowGerman = 390,
        LowSaxon = 391,
        Nepali = 392,
        Newari = 393,
        Ndonga = 394,
        Nias = 395,
        Niuean = 396,
        AoNaga = 397,
        Dutch = 398,
        Flemish = 399,
        Kwasio = 400,
        NorwegianNynorsk = 401,
        Ngiemboon = 402,
        Norwegian = 403,
        Nogai = 404,
        OldNorse = 405,
        Novial = 406,
        NKo = 407,
        SouthNdebele = 408,
        NorthernSotho = 409,
        Nuer = 410,
        Navajo = 411,
        ClassicalNewari = 412,
        Nyanja = 413,
        Nyamwezi = 414,
        Nyankole = 415,
        Nyoro = 416,
        Nzima = 417,
        Occitan = 418,
        Ojibwa = 419,
        Oromo = 420,
        Odia = 421,
        Ossetic = 422,
        Osage = 423,
        OttomanTurkish = 424,
        Punjabi = 425,
        Pangasinan = 426,
        Pahlavi = 427,
        Pampanga = 428,
        Papiamento = 429,
        Palauan = 430,
        Picard = 431,
        NigerianPidgin = 432,
        PennsylvaniaGerman = 433,
        Plautdietsch = 434,
        OldPersian = 435,
        PalatineGerman = 436,
        Phoenician = 437,
        Pali = 438,
        Polish = 439,
        Piedmontese = 440,
        Pontic = 441,
        Pohnpeian = 442,
        Prussian = 443,
        OldProvencal = 444,
        Pashto = 445,
        Pushto = 446,
        Portuguese = 447,
        BrazilianPortuguese = 448,
        EuropeanPortuguese = 449,
        Quechua = 450,
        Kiche = 451,
        ChimborazoHighlandQuichua = 452,
        Rajasthani = 453,
        Rapanui = 454,
        Rarotongan = 455,
        Romagnol = 456,
        Riffian = 457,
        Romansh = 458,
        Rundi = 459,
        Romanian = 460,
        Moldavian = 461,
        Rombo = 462,
        Romany = 463,
        Root = 464,
        Rotuman = 465,
        Russian = 466,
        Rusyn = 467,
        Roviana = 468,
        Aromanian = 469,
        Kinyarwanda = 470,
        Rwa = 471,
        Sanskrit = 472,
        Sandawe = 473,
        Sakha = 474,
        SamaritanAramaic = 475,
        Samburu = 476,
        Sasak = 477,
        Santali = 478,
        Saurashtra = 479,
        Ngambay = 480,
        Sangu = 481,
        Sardinian = 482,
        Sicilian = 483,
        Scots = 484,
        Sindhi = 485,
        SassareseSardinian = 486,
        SouthernKurdish = 487,
        NorthernSami = 488,
        Seneca = 489,
        Sena = 490,
        Seri = 491,
        Selkup = 492,
        KoyraboroSenni = 493,
        Sango = 494,
        OldIrish = 495,
        Samogitian = 496,
        SerboCroatian = 497,
        Tachelhit = 498,
        Shan = 499,
        ChadianArabic = 500,
        Sinhala = 501,
        Sidamo = 502,
        Slovak = 503,
        Slovenian = 504,
        LowerSilesian = 505,
        Selayar = 506,
        Samoan = 507,
        SouthernSami = 508,
        LuleSami = 509,
        InariSami = 510,
        SkoltSami = 511,
        Shona = 512,
        Soninke = 513,
        Somali = 514,
        Sogdien = 515,
        Albanian = 516,
        Serbian = 517,
        Montenegrin = 518,
        SrananTongo = 519,
        Serer = 520,
        Swati = 521,
        Saho = 522,
        SouthernSotho = 523,
        SaterlandFrisian = 524,
        Sundanese = 525,
        Sukuma = 526,
        Susu = 527,
        Sumerian = 528,
        Swedish = 529,
        Swahili = 530,
        CongoSwahili = 531,
        Comorian = 532,
        ClassicalSyriac = 533,
        Syriac = 534,
        Silesian = 535,
        Tamil = 536,
        Tulu = 537,
        Telugu = 538,
        Timne = 539,
        Teso = 540,
        Tereno = 541,
        Tetum = 542,
        Tajik = 543,
        Thai = 544,
        Tigrinya = 545,
        Tigre = 546,
        Tiv = 547,
        Turkmen = 548,
        Tokelau = 549,
        Tsakhur = 550,
        Tagalog = 551,
        Klingon = 552,
        Tlingit = 553,
        Talysh = 554,
        Tamashek = 555,
        Tswana = 556,
        Tongan = 557,
        NyasaTonga = 558,
        TokPisin = 559,
        Turkish = 560,
        Turoyo = 561,
        Taroko = 562,
        Tsonga = 563,
        Tsakonian = 564,
        Tsimshian = 565,
        Tatar = 566,
        MuslimTat = 567,
        Tumbuka = 568,
        Tuvalu = 569,
        Twi = 570,
        Tasawaq = 571,
        Tahitian = 572,
        Tuvinian = 573,
        CentralAtlasTamazight = 574,
        Udmurt = 575,
        Uyghur = 576,
        Uighur = 577,
        Ugaritic = 578,
        Ukrainian = 579,
        Umbundu = 580,
        Unknownlanguage = 581,
        Urdu = 582,
        Uzbek = 583,
        Vai = 584,
        Venda = 585,
        Venetian = 586,
        Veps = 587,
        Vietnamese = 588,
        WestFlemish = 589,
        MainFranconian = 590,
        Volapuk = 591,
        Votic = 592,
        Voro = 593,
        Vunjo = 594,
        Walloon = 595,
        Walser = 596,
        Wolaytta = 597,
        Waray = 598,
        Washo = 599,
        Warlpiri = 600,
        Wolof = 601,
        WuChinese = 602,
        Kalmyk = 603,
        Xhosa = 604,
        Mingrelian = 605,
        Soga = 606,
        Yao = 607,
        Yapese = 608,
        Yangben = 609,
        Yemba = 610,
        Yiddish = 611,
        Yoruba = 612,
        Nheengatu = 613,
        Cantonese = 614,
        Zhuang = 615,
        Zapotec = 616,
        Blissymbols = 617,
        Zeelandic = 618,
        Zenaga = 619,
        StandardMoroccanTamazight = 620,
        Chinese = 621,
        MandarinChinese = 622,
        SimplifiedChinese = 623,
        TraditionalChinese = 624,
        Zulu = 625,
        Zuni = 626,
        Nolinguisticcontent = 627,
        Zaza = 628,
        LastLanguage = Zaza
    };

    enum Script {
        AnyScript = 0,
        AdlamScript = 1,
        AfakaScript = 2,
        CaucasianAlbanianScript = 3,
        AhomScript = 4,
        ArabicScript = 5,
        PersoArabicScript = 6,
        ImperialAramaicScript = 7,
        ArmenianScript = 8,
        AvestanScript = 9,
        BalineseScript = 10,
        BamumScript = 11,
        BassaVahScript = 12,
        BatakScript = 13,
        BanglaScript = 14,
        BhaiksukiScript = 15,
        BlissymbolsScript = 16,
        BopomofoScript = 17,
        BrahmiScript = 18,
        BrailleScript = 19,
        BugineseScript = 20,
        BuhidScript = 21,
        ChakmaScript = 22,
        UnifiedCanadianAboriginalSyllabicsScript = 23,
        UCASScript = 24,
        CarianScript = 25,
        ChamScript = 26,
        CherokeeScript = 27,
        CirthScript = 28,
        CopticScript = 29,
        CypriotScript = 30,
        CyrillicScript = 31,
        OldChurchSlavonicCyrillicScript = 32,
        DevanagariScript = 33,
        DograScript = 34,
        DeseretScript = 35,
        DuployanshorthandScript = 36,
        EgyptiandemoticScript = 37,
        EgyptianhieraticScript = 38,
        EgyptianhieroglyphsScript = 39,
        ElbasanScript = 40,
        ElymaicScript = 41,
        EthiopicScript = 42,
        GeorgianKhutsuriScript = 43,
        GeorgianScript = 44,
        GlagoliticScript = 45,
        GunjalaGondiScript = 46,
        MasaramGondiScript = 47,
        GothicScript = 48,
        GranthaScript = 49,
        GreekScript = 50,
        GujaratiScript = 51,
        GurmukhiScript = 52,
        HanwithBopomofoScript = 53,
        HangulScript = 54,
        HanScript = 55,
        HanunooScript = 56,
        SimplifiedScript = 57,
        SimplifiedHanScript = 58,
        TraditionalScript = 59,
        TraditionalHanScript = 60,
        HatranScript = 61,
        HebrewScript = 62,
        HiraganaScript = 63,
        AnatolianHieroglyphsScript = 64,
        PahawhHmongScript = 65,
        NyiakengPuachueHmongScript = 66,
        JapanesesyllabariesScript = 67,
        OldHungarianScript = 68,
        IndusScript = 69,
        OldItalicScript = 70,
        JamoScript = 71,
        JavaneseScript = 72,
        JapaneseScript = 73,
        JurchenScript = 74,
        KayahLiScript = 75,
        KatakanaScript = 76,
        KharoshthiScript = 77,
        KhmerScript = 78,
        KhojkiScript = 79,
        KannadaScript = 80,
        KoreanScript = 81,
        KpelleScript = 82,
        KaithiScript = 83,
        LannaScript = 84,
        LaoScript = 85,
        FrakturLatinScript = 86,
        GaelicLatinScript = 87,
        LatinScript = 88,
        LepchaScript = 89,
        LimbuScript = 90,
        LinearAScript = 91,
        LinearBScript = 92,
        FraserScript = 93,
        LomaScript = 94,
        LycianScript = 95,
        LydianScript = 96,
        MahajaniScript = 97,
        MakasarScript = 98,
        MandaeanScript = 99,
        ManichaeanScript = 100,
        MarchenScript = 101,
        MayanhieroglyphsScript = 102,
        MedefaidrinScript = 103,
        MendeScript = 104,
        MeroiticCursiveScript = 105,
        MeroiticScript = 106,
        MalayalamScript = 107,
        ModiScript = 108,
        MongolianScript = 109,
        MoonScript = 110,
        MroScript = 111,
        MeiteiMayekScript = 112,
        MultaniScript = 113,
        MyanmarScript = 114,
        NandinagariScript = 115,
        OldNorthArabianScript = 116,
        NabataeanScript = 117,
        NewaScript = 118,
        NaxiGebaScript = 119,
        NKoScript = 120,
        NushuScript = 121,
        OghamScript = 122,
        OlChikiScript = 123,
        OrkhonScript = 124,
        OdiaScript = 125,
        OsageScript = 126,
        OsmanyaScript = 127,
        PalmyreneScript = 128,
        PauCinHauScript = 129,
        OldPermicScript = 130,
        PhagspaScript = 131,
        InscriptionalPahlaviScript = 132,
        PsalterPahlaviScript = 133,
        BookPahlaviScript = 134,
        PhoenicianScript = 135,
        PollardPhoneticScript = 136,
        InscriptionalParthianScript = 137,
        RejangScript = 138,
        HanifiRohingyaScript = 139,
        RongorongoScript = 140,
        RunicScript = 141,
        SamaritanScript = 142,
        SaratiScript = 143,
        OldSouthArabianScript = 144,
        SaurashtraScript = 145,
        SignWritingScript = 146,
        ShavianScript = 147,
        SharadaScript = 148,
        SiddhamScript = 149,
        KhudawadiScript = 150,
        SinhalaScript = 151,
        SogdianScript = 152,
        OldSogdianScript = 153,
        SoraSompengScript = 154,
        SoyomboScript = 155,
        SundaneseScript = 156,
        SylotiNagriScript = 157,
        SyriacScript = 158,
        EstrangeloSyriacScript = 159,
        WesternSyriacScript = 160,
        EasternSyriacScript = 161,
        TagbanwaScript = 162,
        TakriScript = 163,
        TaiLeScript = 164,
        NewTaiLueScript = 165,
        TamilScript = 166,
        TangutScript = 167,
        TaiVietScript = 168,
        TeluguScript = 169,
        TengwarScript = 170,
        TifinaghScript = 171,
        TagalogScript = 172,
        ThaanaScript = 173,
        ThaiScript = 174,
        TibetanScript = 175,
        TirhutaScript = 176,
        UgariticScript = 177,
        VaiScript = 178,
        VisibleSpeechScript = 179,
        VarangKshitiScript = 180,
        WanchoScript = 181,
        WoleaiScript = 182,
        OldPersianScript = 183,
        SumeroAkkadianCuneiformScript = 184,
        SACuneiformScript = 185,
        YiScript = 186,
        ZanabazarSquareScript = 187,
        InheritedScript = 188,
        MathematicalNotationScript = 189,
        EmojiScript = 190,
        SymbolsScript = 191,
        UnwrittenScript = 192,
        CommonScript = 193,
        UnknownScript = 194,
        LastScript = UnknownScript
    };
    enum Country {
        AnyCountry = 0,
        Africa = 1,
        NorthAmerica = 2,
        SouthAmerica = 3,
        Oceania = 4,
        WesternAfrica = 5,
        CentralAmerica = 6,
        EasternAfrica = 7,
        NorthernAfrica = 8,
        MiddleAfrica = 9,
        SouthernAfrica = 10,
        Americas = 11,
        NorthernAmerica = 12,
        Caribbean = 13,
        EasternAsia = 14,
        SouthernAsia = 15,
        SoutheastAsia = 16,
        SouthernEurope = 17,
        Australasia = 18,
        Melanesia = 19,
        MicronesianRegion = 20,
        Polynesia = 21,
        Asia = 22,
        CentralAsia = 23,
        WesternAsia = 24,
        Europe = 25,
        EasternEurope = 26,
        NorthernEurope = 27,
        WesternEurope = 28,
        SubSaharanAfrica = 29,
        LatinAmerica = 30,
        AscensionIsland = 31,
        Andorra = 32,
        UnitedArabEmirates = 33,
        Afghanistan = 34,
        AntiguaAndBarbuda = 35,
        Anguilla = 36,
        Albania = 37,
        Armenia = 38,
        Angola = 39,
        Antarctica = 40,
        Argentina = 41,
        AmericanSamoa = 42,
        Austria = 43,
        Australia = 44,
        Aruba = 45,
        AlandIslands = 46,
        Azerbaijan = 47,
        BosniaAndHerzegovina = 48,
        Bosnia = 49,
        Barbados = 50,
        Bangladesh = 51,
        Belgium = 52,
        BurkinaFaso = 53,
        Bulgaria = 54,
        Bahrain = 55,
        Burundi = 56,
        Benin = 57,
        StBarthelemy = 58,
        Bermuda = 59,
        Brunei = 60,
        Bolivia = 61,
        CaribbeanNetherlands = 62,
        Brazil = 63,
        Bahamas = 64,
        Bhutan = 65,
        BouvetIsland = 66,
        Botswana = 67,
        Belarus = 68,
        Belize = 69,
        Canada = 70,
        CocosKeelingIslands = 71,
        CongoKinshasa = 72,
        CongoDRC = 73,
        CentralAfricanRepublic = 74,
        CongoBrazzaville = 75,
        CongoRepublic = 76,
        Switzerland = 77,
        CotedIvoire = 78,
        IvoryCoast = 79,
        CookIslands = 80,
        Chile = 81,
        Cameroon = 82,
        China = 83,
        Colombia = 84,
        ClippertonIsland = 85,
        CostaRica = 86,
        Cuba = 87,
        CapeVerde = 88,
        Curacao = 89,
        ChristmasIsland = 90,
        Cyprus = 91,
        Czechia = 92,
        CzechRepublic = 93,
        Germany = 94,
        DiegoGarcia = 95,
        Djibouti = 96,
        Denmark = 97,
        Dominica = 98,
        DominicanRepublic = 99,
        Algeria = 100,
        CeutaAndMelilla = 101,
        Ecuador = 102,
        Estonia = 103,
        Egypt = 104,
        WesternSahara = 105,
        Eritrea = 106,
        Spain = 107,
        Ethiopia = 108,
        EuropeanUnion = 109,
        Eurozone = 110,
        Finland = 111,
        Fiji = 112,
        FalklandIslands = 113,
        FalklandIslandsIslasMalvinas = 114,
        Micronesia = 115,
        FaroeIslands = 116,
        France = 117,
        Gabon = 118,
        UnitedKingdom = 119,
        UK = 120,
        Grenada = 121,
        Georgia = 122,
        FrenchGuiana = 123,
        Guernsey = 124,
        Ghana = 125,
        Gibraltar = 126,
        Greenland = 127,
        Gambia = 128,
        Guinea = 129,
        Guadeloupe = 130,
        EquatorialGuinea = 131,
        Greece = 132,
        SouthGeorgiaAndSouthSandwichIslands = 133,
        Guatemala = 134,
        Guam = 135,
        GuineaBissau = 136,
        Guyana = 137,
        HongKongSARChina = 138,
        HongKong = 139,
        HeardAndMcDonaldIslands = 140,
        Honduras = 141,
        Croatia = 142,
        Haiti = 143,
        Hungary = 144,
        CanaryIslands = 145,
        Indonesia = 146,
        Ireland = 147,
        Israel = 148,
        IsleofMan = 149,
        India = 150,
        BritishIndianOceanTerritory = 151,
        Iraq = 152,
        Iran = 153,
        Iceland = 154,
        Italy = 155,
        Jersey = 156,
        Jamaica = 157,
        Jordan = 158,
        Japan = 159,
        Kenya = 160,
        Kyrgyzstan = 161,
        Cambodia = 162,
        Kiribati = 163,
        Comoros = 164,
        StKittsAndNevis = 165,
        NorthKorea = 166,
        SouthKorea = 167,
        Kuwait = 168,
        CaymanIslands = 169,
        Kazakhstan = 170,
        Laos = 171,
        Lebanon = 172,
        StLucia = 173,
        Liechtenstein = 174,
        SriLanka = 175,
        Liberia = 176,
        Lesotho = 177,
        Lithuania = 178,
        Luxembourg = 179,
        Latvia = 180,
        Libya = 181,
        Morocco = 182,
        Monaco = 183,
        Moldova = 184,
        Montenegro = 185,
        StMartin = 186,
        Madagascar = 187,
        MarshallIslands = 188,
        NorthMacedonia = 189,
        Mali = 190,
        MyanmarBurma = 191,
        Myanmar = 192,
        Mongolia = 193,
        MacaoSARChina = 194,
        Macao = 195,
        NorthernMarianaIslands = 196,
        Martinique = 197,
        Mauritania = 198,
        Montserrat = 199,
        Malta = 200,
        Mauritius = 201,
        Maldives = 202,
        Malawi = 203,
        Mexico = 204,
        Malaysia = 205,
        Mozambique = 206,
        Namibia = 207,
        NewCaledonia = 208,
        Niger = 209,
        NorfolkIsland = 210,
        Nigeria = 211,
        Nicaragua = 212,
        Netherlands = 213,
        Norway = 214,
        Nepal = 215,
        // Nauru = 216,
        Niue = 217,
        NewZealand = 218,
        Oman = 219,
        Panama = 220,
        Peru = 221,
        FrenchPolynesia = 222,
        PapuaNewGuinea = 223,
        Philippines = 224,
        Pakistan = 225,
        Poland = 226,
        StPierreAndMiquelon = 227,
        PitcairnIslands = 228,
        PuertoRico = 229,
        PalestinianTerritories = 230,
        Palestine = 231,
        Portugal = 232,
        Palau = 233,
        Paraguay = 234,
        Qatar = 235,
        OutlyingOceania = 236,
        Reunion = 237,
        Romania = 238,
        Serbia = 239,
        Russia = 240,
        Rwanda = 241,
        SaudiArabia = 242,
        SolomonIslands = 243,
        Seychelles = 244,
        Sudan = 245,
        Sweden = 246,
        Singapore = 247,
        StHelena = 248,
        Slovenia = 249,
        SvalbardAndJanMayen = 250,
        Slovakia = 251,
        SierraLeone = 252,
        SanMarino = 253,
        Senegal = 254,
        Somalia = 255,
        Suriname = 256,
        SouthSudan = 257,
        SaoTomeAndPrincipe = 258,
        ElSalvador = 259,
        SintMaarten = 260,
        Syria = 261,
        Eswatini = 262,
        Swaziland = 263,
        TristandaCunha = 264,
        TurksAndCaicosIslands = 265,
        Chad = 266,
        FrenchSouthernTerritories = 267,
        Togo = 268,
        Thailand = 269,
        Tajikistan = 270,
        // Tokelau = 271,
        TimorLeste = 272,
        EastTimor = 273,
        Turkmenistan = 274,
        Tunisia = 275,
        Tonga = 276,
        Turkey = 277,
        TrinidadAndTobago = 278,
        // Tuvalu = 279,
        Taiwan = 280,
        Tanzania = 281,
        Ukraine = 282,
        Uganda = 283,
        UnitedStatesOutlyingIslands = 284,
        UnitedNations = 285,
        UN = 286,
        UnitedStates = 287,
        US = 288,
        Uruguay = 289,
        Uzbekistan = 290,
        VaticanCity = 291,
        StVincentAndGrenadines = 292,
        Venezuela = 293,
        BritishVirginIslands = 294,
        UnitedStatesVirginIslands = 295,
        Vietnam = 296,
        Vanuatu = 297,
        WallisAndFutuna = 298,
        Samoa = 299,
        PseudoAccents = 300,
        PseudoBidi = 301,
        Kosovo = 302,
        Yemen = 303,
        Mayotte = 304,
        SouthAfrica = 305,
        Zambia = 306,
        Zimbabwe = 307,
        UnknownRegion = 308,
        LastCountry = UnknownRegion
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
